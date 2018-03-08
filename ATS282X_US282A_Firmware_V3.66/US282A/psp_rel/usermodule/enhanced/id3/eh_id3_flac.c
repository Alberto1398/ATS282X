#include "eh_id3.h"

#define VORBIS_COMMENT   0x04
#define BLK_PICTURE      0x06
#define FLAC_ITEM_COUNT   4

static const char item_key_tab[FLAC_ITEM_COUNT][8] =
{ "Title=", "Artist=", "Album=", "Genre=" };

static const char item_key_tab1[FLAC_ITEM_COUNT][8] =
{ "title=", "artist=", "album=", "genre=" };

static const uint8 item_key_len[FLAC_ITEM_COUNT] =
{ 6, 7, 6, 6 };
static const char flac_file_tag[] =
{ "fLaC" };
static const char flac_image_tag[] =
{ "image/jpeg" };

//���ܶ��� BANK_DATA,����BANK
//static uint32 tag_info_len _BANK_DATA_ATTR_;//TAG��Ϣ�ĳ���
//static uint32 tag_info_count _BANK_DATA_ATTR_;//TAG��Ϣ��da
//
//static uint32 vorbis_size _BANK_DATA_ATTR_;

static bool flac_find_vorbis_blk(uint8 blk_flag, uint32 *vorbis_size_p);
static bool flac_get_items(uint32 vb_size_param);
static bool flac_get_picture(void);

/******************************************************************************/
/*
 * \par  Description: �ַ���ת����д
 *
 * \param[in]    str

 * \param[out]

 * \return        �ַ�������

 * \note
 *******************************************************************************/
//static uint8 flac_to_capital(char *str)
//{
//	uint32 i=0;
//	while(str[i]!='=')
//	{
//		if(str[i]>='a' && str[i]<='z')
//		{
//			str[i]=str[i]-'a'+'A';
//		}
//		i++;
//
//		if(i>128)
//		{
//		    return 0;
//		}
//
//	}
//
//	return i;
//}
/******************************************************************************/
/*
 * \par  Description:ƥ���ַ���
 *
 * \param[in]    buf

 * \param[out]

 * \return        �ɹ�>0,ʧ��<0

 * \note
 *******************************************************************************/
static int8 flac_check_tag(char *buf)
{
    uint8 i;
    uint8 cmp_ret1, cmp_ret2;

    for (i = 0; i < FLAC_ITEM_COUNT; i++)
    {
        if (check_flag[i] == 1)
        {
            cmp_ret1 = (uint8)libc_memcmp(buf, &item_key_tab[i], (uint32 ) item_key_len[i]);
            cmp_ret2 = (uint8)libc_memcmp(buf, &item_key_tab1[i], (uint32 ) item_key_len[i]);
            if ((cmp_ret1 == 0) || (cmp_ret2 == 0))
            {
                check_flag[i] = 0;
                //���ý�Ҫ����ĵ�ַ�ͳ���
                id3_save_p->saveaddr = (void*) *((uint32*) id3_info_p + i);
                id3_save_p->savelen = *((uint8*) id3_info_p + 24 + i);
                return i;
            }
        }
    }

    return -1;
}

/******************************************************************************/
/*
 * \par  Description: ����flac���͵�id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_flac(void)
{
    uint32 vorbis_size;
    bool result1 = FALSE;
    bool result2 = FALSE;

    get_init();
    read_buf_data(key_buf, 4);
    if (libc_memcmp(key_buf, flac_file_tag, 4) != 0)
    {
        //����flac�ļ�
        return FALSE;
    }

    result1 = flac_find_vorbis_blk(VORBIS_COMMENT, &vorbis_size);
    if ((FALSE != result1) && (vorbis_size != 0))
    {
        //��ȡID3��Ϣ
        result1 = flac_get_items(vorbis_size);
    }

    if (id3_info_p->apic_flag == 1)
    {
        //��ͷ��ʼ��
        reset_file_pos(0, 4);
        result2 = flac_find_vorbis_blk(BLK_PICTURE, &vorbis_size);
        if ((FALSE != result2) && (vorbis_size != 0))
        {
            //��ȡIMAGE��Ϣ
            result2 = flac_get_picture();
        }
    }
    return (result1 | result2);
}

/******************************************************************************/
/*
 * \par  Description: ����ָ���ı��
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool flac_find_vorbis_blk(uint8 blk_flag, uint32 *vorbis_size_p)
{
    uint32 temp_size;

    while (id3_pos_file < 0x2000)
    {
        read_buf_data(key_buf, 4);
        id3_pos_file = id3_pos_file; //Ϊ�˹�QAC, ֻ�ܳ����²�
        temp_size = (uint32) key_buf[1] * 0x10000 + (uint32) key_buf[2] * 0x100 + (uint32) key_buf[3];

        if ((key_buf[0] & 0x7F) == blk_flag) //
        {
            *vorbis_size_p = temp_size;
            return TRUE;
        }
        if ((key_buf[0] & 0x80) != 0) //end flag
        {
            return FALSE; // last metadata_block
        }

        reset_file_pos(temp_size, 0);

        //if(id3_pos_file>0x2000)
        //{
        //  return FALSE;
        //}

    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡid3��item��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
// ���� VORBIS_COMMENT �ṹ��ID3
static bool flac_get_items(uint32 vb_size_param)
{
    //char i;
    uint32 tag_info_len, tag_info_count, vb_size;
    int8 sn;
    uint8 find_count;

    find_count = 0; //������1��ʼ
    vb_size = vb_size_param;

    //����һ��
    read_buf_data((uint8*) &tag_info_len, 4);
    //��һ�����ݲ�Ҫ,����
    reset_file_pos(tag_info_len, 0);
    //����Ϣ��������
    read_buf_data((uint8*) &tag_info_count, 4);

    vb_size = vb_size - tag_info_len - 8 + id3_pos_file;

    while ((tag_info_count > 0) && (id3_pos_file < vb_size))
    {
        tag_info_count--;
        if (FALSE == read_buf_data((uint8*) &tag_info_len, 4))
        {
            return FALSE;
        }

        if (tag_info_len == 0)
        {
            continue;
        }

        if (FALSE == read_buf_data(key_buf, tag_info_len))
        {
            return FALSE;
        }

        sn = flac_check_tag(key_buf);

        if (sn != (int8) (-1))
        {
            find_count++;
            if (id3_save_p->savelen == 0)
            {
                continue;
            }
            else
            {
                tag_info_len -= item_key_len[sn]; //ȥ����ʶ��

                //utf8 ����
                tag_info_len = id3_read_utf8_deal(id3_save_p, tag_info_len, key_buf + item_key_len[sn]);

                //if ((uint32) (id3_save_p->savelen - 4) < tag_info_len)
                //{
                //  tag_info_len = id3_save_p->savelen - 4;
                //}
                //
                //libc_memcpy(id3_save_p->saveaddr + 3, key_buf + item_key_len[sn], tag_info_len);
                //
                //id3_save_p->saveaddr[0] = UTF8_FLAG_0;
                //id3_save_p->saveaddr[1] = UTF8_FLAG_1;
                //id3_save_p->saveaddr[2] = UTF8_FLAG_2;
                //id3_save_p->saveaddr[tag_info_len + 3] = '\0';
            }

            if (find_count >= FLAC_ITEM_COUNT)
            {
                break;
            }
        }

    }

    if (find_count > 0)
    {
        return TRUE;
    }

    return FALSE;

}

/******************************************************************************/
/*
 * \par  Description:����pic���
 *
 * \param[in]

 * \param[out]

 * \return        �ɹ�>0,ʧ��<0

 * \note
 *******************************************************************************/
static int8 flac_check_pic_flag(uint8 *buf)
{
    uint8 a;
    for (a = 0; a < 63; a++)
    {
        if ((buf[a] == 0xff) && (buf[a + 1] == 0xd8))
        {
            return a;
        }
    }
    return -1;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡpicture
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
// ���� PICTURE �ṹ
static bool flac_get_picture(void)
{
    int8 result;

    reset_file_pos(8, 0);
    read_buf_data(key_buf, 10);
    if (0 != libc_memcmp(flac_image_tag, key_buf, 10))
    {
        return FALSE;
    }

    read_buf_data(key_buf, 64);

    result = flac_check_pic_flag(key_buf);
    if (result != -1)
    {
        id3_info_p->apic_offset = id3_pos_file + (uint8) result - 64;
        id3_info_p->apic_type = 1;
        return TRUE;
    }
    return FALSE;

}

