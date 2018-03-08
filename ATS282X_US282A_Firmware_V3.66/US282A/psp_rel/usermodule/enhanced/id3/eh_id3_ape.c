#include "eh_id3.h"

#define APE_ITEM_COUNT   6

/* ý���ļ�APEv2��ǩ�ı�ǩͷ���ǩβ�ṹ�壬��СΪ32�ֽ� */
typedef struct
{
    uint8 preamble[8]; /* APEv2��ǩ�Ƿ���ڵı�־��ռ8�ֽ� */
    uint8 version[4]; /* APEv2��ǩ�İ汾��ռ4�ֽ� */
    uint32 tagsize; /* APEv2��ǩ�Ĵ�С��������ǩβ����������ǩͷ�Ĵ�С��ռ4�ֽ� */
    uint32 itemcount; /* APEv2��ǩԪ�صĸ�����ռ4�ֽ� */
    uint8 tagflags[4]; /* APEv2��ǩ�ı�ʶ��ռ4�ֽ� */
    uint8 reserved[8]; /* APEv2��ǩ������ֵȫΪ�㣬ռ8�ֽ� */

} apev2_head_t;

/* ý���ļ�APEv2��ǩ�ı�ǩԪ�� */
typedef struct
{
    uint32 itemvaluesize; /* APEv2��ǩ��ǩԪ�����ݵĴ�С��ռ4�ֽ� */
    uint8 itemflags[4]; /* APEv2��ǩ��ǩԪ�صı�ʶ��ռ4�ֽ� */
} apev2_item_t;

static const char ape_preamble[] = "APETAGEX";
static const char item_key_tab[APE_ITEM_COUNT][7] =
    { "TITLE", "ARTIST", "ALBUM", "GENRE", "XXXXX", "TRACK" };
static const uint8 item_key_len[APE_ITEM_COUNT] =
    { 5, 6, 5, 5, 5, 5 };

//static uint32 pos_item_stream _BANK_DATA_ATTR_;
//static uint32 file_len _BANK_DATA_ATTR_;
static char item_buf[8] _BANK_DATA_ATTR_;

extern bool mp3_v1_parse(void);
extern bool mp3_select_genre(id3_info_t* id3_info_r, uint8 genre_index);

char ape_v2_parse(void);

/******************************************************************************/
/*
 * \par  Description: ape��ȡ����
 *
 * \param[in]    Tbuff--����buf
 size--���ݳ��ȣ�byte

 * \param[out]

 * \return       TRUE OR FALSE

 * \note         �Խ���������
 *******************************************************************************/
static bool ape_read_datax(char *Tbuff, uint32 size)
{
    uint32 avail, remain;
    uint32 copy_size;
    uint32 next_pos;

    next_pos = id3_pos_buffer + size;

    copy_size = size;
    if (copy_size > KEY_BUF_LEN)
    {
        copy_size = KEY_BUF_LEN;
    }

    if (size == 0) //������ �ַ�������Ϊֹ'\0'
    {
        uint16 start_pos = 0;
        while (id3_temp_buf[next_pos] != '\0')
        {
            copy_size++;
            next_pos++;
            if (copy_size > KEY_BUF_LEN)
            {
                //�ṹ���ԣ�������ID3
                return 0;
            }
            if (next_pos > (SECTOR_SIZE - 1))
            {
                libc_memcpy(Tbuff, id3_temp_buf + id3_pos_buffer, copy_size);
                //FS_FRead(TempBuf,1, TagFileHandle );
                vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle);
                next_pos = 0;
                id3_pos_buffer = 0;
                start_pos = (uint16) copy_size;
            }
        }

        if (start_pos != copy_size)
        {
            libc_memcpy(Tbuff + start_pos, id3_temp_buf + id3_pos_buffer, copy_size - start_pos);
        }

        Tbuff[copy_size] = '\0';

        if (next_pos == (SECTOR_SIZE - 1)) // '\0'Ϊbuf���һ��byte
        {
            vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle);
            id3_pos_buffer = 0;
            next_pos = 0;
        }
        else
        {
            id3_pos_buffer = (uint16) next_pos + 1;
        }

        //pos_item_stream=pos_item_stream+copy_size+1;
        id3_pos_file = id3_pos_file + copy_size + 1;
    }

    else
    {
        if (next_pos < SECTOR_SIZE) // if end of data is within buffer
        {
            libc_memcpy(Tbuff, &id3_temp_buf[id3_pos_buffer], copy_size); // copy data to vector
        }
        else
        {
            avail = SECTOR_SIZE - id3_pos_buffer; //how much data is in current buffer
            if (avail > copy_size)
            {
                libc_memcpy(Tbuff, &id3_temp_buf[id3_pos_buffer], copy_size); //copy to vector
                vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle); // read next sector
            }
            else
            {
                libc_memcpy(Tbuff, &id3_temp_buf[id3_pos_buffer], avail); //copy to vector
                vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle); // read next sector
                remain = copy_size - avail; // how much data must still be copied
                libc_memcpy(&Tbuff[avail], id3_temp_buf, remain); // copy remain data
            }
        }

        //pos_item_stream=pos_item_stream+size; //update address in file
        //id3_pos_buffer = pos_item_stream & (SECTOR_SIZE-1); //update position in buffer
        id3_pos_buffer = (uint16) (next_pos & (SECTOR_SIZE - 1));
        id3_pos_file = id3_pos_file + size;
    }

    return TRUE;
}

//�Ƚ�ID3�� Itemkey
int8 ape_check_tag(char *buf)
{
    uint8 i;
    uint8 str_len;
    str_len = str_to_capital(buf, '\0');
    if (str_len > 0)
    {
        for (i = 0; i < APE_ITEM_COUNT; i++)
        {
            if (check_flag[i] == 1)
            {
                if (libc_memcmp(buf, &item_key_tab[i], (uint32) item_key_len[i]) == 0)
                {
                    check_flag[i] = 0;
                    //���ý�Ҫ����ĵ�ַ�ͳ���
                    id3_save_p->saveaddr = (void*) *((uint32*) id3_info_p + i);
                    id3_save_p->savelen = *((uint8*) id3_info_p + 24 + i);
                    return i;
                }
            }
        }
    }

    return -1;
}

/******************************************************************************/
/*
 * \par  Description: ����V2�汾
 *
 * \param[in]

 * \param[out]

 * \return        TRUR OR FALSE

 * \note
 *******************************************************************************/
char ape_v2_parse(void)
{
    apev2_head_t *ape_head_p;
    apev2_item_t *apev2_item_p;
    uint8 find_count;

    //    uint32 SeekLen;
    uint32 t_itemcount, t_itemsize;

    uint32 file_len;

    if (FALSE == vfs_file_get_size(eh_vfs_mount, &file_len, id3_handle, 0))
    {
        return FALSE;
    }
    ape_head_p = (apev2_head_t *) (id3_temp_buf + 96);
    apev2_item_p = (apev2_item_t *) item_buf;

    t_itemcount = ape_head_p->itemcount;
    t_itemsize = ape_head_p->tagsize;

    if (t_itemcount == 0)
    {
        return FALSE;
    }

    //    libc_memset(FindItemFlag,0,APE_ITEM_COUNT);

    find_count = 0; //������1��ʼ

    if (ape_head_p->tagsize > 128)
    {
        id3_pos_file = file_len - ape_head_p->tagsize;
        id3_pos_buffer = (uint16) (id3_pos_file % SECTOR_SIZE);
        //seek������Ϊ��С��λ
        vfs_file_seek(eh_vfs_mount, id3_pos_file - id3_pos_buffer, SEEK_SET, id3_handle);
        vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle);
        id3_pos_buffer = (uint16) (id3_pos_file & (SECTOR_SIZE - 1));
    }
    else
    {
        id3_pos_buffer = (uint16) (128 - ape_head_p->tagsize);
        id3_pos_file = file_len - ape_head_p->tagsize;
    }

    //pos_item_stream=0;

    sys_os_sched_lock();

    while ((t_itemcount > 0) && (id3_pos_file < file_len))
    {
        t_itemcount--;
        ape_read_datax((char*) apev2_item_p, 8);

        if (apev2_item_p->itemvaluesize == 0)
        {
            continue;
        }

        ape_read_datax(key_buf, 0x00); //�����ַ�������

        if (str_to_capital(key_buf, '\0') > 0)
        {
            if (ape_check_tag(key_buf) != (int8) -1)
            {
                find_count++;
                ape_read_datax(key_buf, apev2_item_p->itemvaluesize); //�������ݳ���
                if ((id3_save_p->savelen == 0) || (apev2_item_p->itemvaluesize == 0))
                {
                    continue;
                }

                if ((uint32) (id3_save_p->savelen - 1) < apev2_item_p->itemvaluesize)
                {
                    apev2_item_p->itemvaluesize = id3_save_p->savelen - 1;
                }

                libc_memcpy(id3_save_p->saveaddr, key_buf, apev2_item_p->itemvaluesize);
                id3_save_p->saveaddr[apev2_item_p->itemvaluesize] = '\0';

                if (find_count >= APE_ITEM_COUNT)
                {
                    break;
                }

            }
            else
            {
                reset_file_pos(apev2_item_p->itemvaluesize, 0);
            }
        }
        else
        {
            reset_file_pos(apev2_item_p->itemvaluesize, 0);
        }

    }

    sys_os_sched_unlock();

    if (find_count > 0)
    {
        if ((id3_info_p->genre_buffer != NULL) && (id3_info_p->genre_length != 0))
        {
            if ((id3_info_p->genre_buffer[0] >= '0') && (id3_info_p->genre_buffer[0] <= '9'))
            {
                mp3_select_genre(id3_info_p, id3_info_p->genre_buffer[0]);
            }
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/*
 * \par  Description:����ape���͵�id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_ape(void)
{

    uint32 file_len;
    if (FALSE == vfs_file_get_size(eh_vfs_mount, &file_len, id3_handle, 0))
    {
        return FALSE;
    }

    if (file_len < FILE_SIZE_MIN)
    {
        return FALSE;
    }

    //��mp3 v1����ʽһ��

    if (FALSE != mp3_v1_parse())
    {
        return TRUE;
    }
    else if (libc_memcmp(id3_temp_buf + 96, ape_preamble, 8) == 0)
    {
        return ape_v2_parse();
    }
    else
    {
        return FALSE;
    }

}

