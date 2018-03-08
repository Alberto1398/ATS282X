#include "eh_id3.h"

typedef bool (*id3_func_f)(void);

static const id3_func_f get_id3_func[ID3_TYPE_END] =
{ NULL, NULL, NULL, get_id3_ape, get_id3_flac, get_id3_mp3, NULL, get_id3_wma };
id3_save_t id3_save_buf;

uint8 *id3_temp_buf;
uint8 *key_buf;
uint8 check_count;
uint8 check_flag[8]; //����ID3Ҫ�ر���:0--Ϊ��Ҫ����,1--Ϊ����Ҫ����
uint16 id3_pos_buffer; //��ǰTempBufδ�����ַ�������
uint32 id3_pos_file; //��ǰ�ļ��ľ�ȷָ��
id3_save_t *id3_save_p; //FrameID�洢λ��
id3_info_t *id3_info_p; //ap���ṩ�Ĵ洢�ṹ

extern bool mp3_v2_parse(void) __FAR__;
extern bool mp3_v1_parse(void) __FAR__;

extern void check_id3_buffer(id3_info_t *id3_info, id3_type_e file_type) __FAR__;

/******************************************************************************/
/*
 * \par  Description: ��ȡָ���ļ���id3��Ϣ
 *
 * \param[in][out]    id3_info,���������������id3��Ϣ
 * \param[in]       filename,�ļ���,ΪNULL���Ѷ�λ��ǰĿ¼��
 * \param[in]       file_type,id3����
 * \param[out]      none
 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_get_id3_info(id3_info_t *id3_info, const char *filename, id3_type_e file_type)
{
    bool ret_val;
    uint8 i;
    uint8 *pt;
    uint32 *str_buf;
    int32 ret = TRUE;
    //id3_func_f id3_func_p;

    if (id3_info == NULL)
    {
        return FALSE;
    }

    id3_info_p = id3_info;
    id3_save_p = &id3_save_buf;

    id3_temp_buf = sys_malloc_large_data(SECTOR_SIZE);

    key_buf = sys_malloc(KEY_BUF_LEN);

    libc_memset(check_flag, 0, sizeof(check_flag));
    pt = (uint8*) id3_info_p + 24;
    str_buf = (uint32*) id3_info_p;
    check_count = 0;

    for (i = 0; i < 6; i++)
    {
        if (*pt > 0)
        {
            check_flag[i] = 1; //�û�ȡ��־
            check_count++;
            *(uint16*) (*str_buf) = 0x00; //Ԥ�����������
        }
        pt++;
        str_buf++;
    }

    id3_info_p->track_num = 0xffff;

    if (id3_info->apic_flag == 1)
    {
        check_flag[6] = 1;
        check_count++;
    }

    if (id3_info->ldb_flag == 1)
    {
        check_flag[7] = 1;
        check_count++;
    }

    id3_info->apic_type = 0;

    //��֧�ֵ��ļ���ʽ��ҲҪĬ������ID3��Ϣ
    if (file_type < ID3_TYPE_END)
    {
        id3_handle = vfs_file_open(eh_vfs_mount, filename, R_NORMAL_SEEK);

        if (0 == id3_handle)
        {          
            ret = FALSE;
            goto get_end;
        }

        if (get_id3_func[file_type - ID3_TYPE_AA] != NULL)
        {
            ret_val = get_id3_func[file_type - ID3_TYPE_AA]();

            vfs_file_close(eh_vfs_mount, id3_handle);
        }
    }

    //��bank1
    check_id3_buffer(id3_info, file_type);
    get_end:
    sys_free(key_buf);

    sys_free_large_data(id3_temp_buf);

    return ret;

}

/******************************************************************************/
/*
 * \par  Description: �����ļ�ǰ����ʼ������
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool get_init(void)
{
    id3_pos_buffer = 0;
    id3_pos_file = 0;

    vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle); //��һPAGE����
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ���ļ����ݵ�ָ��buffer
 *
 * \param[in]    Tbuff������buffer
 size����ȡ�����ݳ��ȣ���λbyte��
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool read_buf_data(uint8 *Tbuff, uint32 size)
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

    id3_pos_file = id3_pos_file + size; //update address in file
    id3_pos_buffer = (uint16) (id3_pos_file & (SECTOR_SIZE - 1)); //update position in buffer

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: �������ļ�ָ��
 *
 * \param[in]    size--�������ȣ�byte
 last_pos_file--�ļ�������λ�ã�����Ϊ0ʱ�������¶�λ�ļ���λ��
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
char reset_file_pos(uint32 size, uint32 last_pos_file)
{
    uint32 next_pos;
    uint32 sector;

    if (last_pos_file != 0)
    {
        id3_pos_file = last_pos_file;
    }

    id3_pos_file = id3_pos_file + size; // update address in file
    next_pos = id3_pos_buffer + size;

    if ((last_pos_file == 0) && (next_pos < SECTOR_SIZE)) //if next position is within buffer
    {
        id3_pos_buffer = (uint16) next_pos;
    }
    else
    {
        sector = id3_pos_file / SECTOR_SIZE;
        vfs_file_seek(eh_vfs_mount, sector * SECTOR_SIZE, SEEK_SET, id3_handle);
        vfs_file_read(eh_vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle);
        id3_pos_buffer = (uint16) (id3_pos_file & (SECTOR_SIZE - 1));
    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:��ָ���ַ���ת���ɴ�д
 *
 * \param[in]    str--�ַ���

 * \param[out]   end_char--�ַ���������

 * \return       �����ַ����ĳ���

 * \note
 *******************************************************************************/
uint8 str_to_capital(char *str, char end_char)
{
    uint32 i = 0;
    while ((str[i] != end_char) && (str[i] != '\0'))
    {
        if ((str[i] >= 'a') && (str[i] <= 'z'))
        {
            str[i] = str[i] - 'a' + 'A';
        }
        i++;

        if (i == KEY_BUF_LEN)
        {
            return 0;
        }

    }

    return i;
}

/******************************************************************************/
/*
 * \par  Description: ����mp3�����ļ�
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_mp3(void)
{
    if (FALSE == mp3_v2_parse())
    {
        return mp3_v1_parse();
    }
    return TRUE;
}
//����ַ����������������ֽ�����
void check_odd_char_in_str(uint8 *buf,uint8 length)
{
    uint8 offset=1,odd_count = 0,even_count = 0,i = 0;
    for(i=0;i<length;i+=offset)
    {
        if(buf[i]<=0x80)
        {
            odd_count++;
            offset=1;
        }
        else
        {
            even_count++;
            offset=2;
        }
    }
    //û��ż��(�����ֽ�����)
    //��������������(��������˫�ֽڵ���)
    if((even_count==0)||((odd_count%2)!=0))
    {
        return ;
    }
    buf[length-1]=0x0;
    
}

