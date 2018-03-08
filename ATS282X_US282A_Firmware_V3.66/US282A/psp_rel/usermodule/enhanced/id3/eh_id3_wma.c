#include "eh_id3.h"

typedef struct
{ //����WMA TAG �����ݸ�ʽ
    uint16 title_len;
    uint16 author_len;
    uint16 copyright_len;
    uint16 description_Len;
    uint16 rating_len;
//char *TaginfoBuf;
} asf_tagobj_t;

typedef struct
{
    uint8* str_buf;
    uint8 str_len;
} tag_string_t;

#define EXTENDED_TYPE_CN             8

//Header object ��unicode
static const uint8 asf_header_obj[16] =
{ 0x30, 0x26, 0xb2, 0x75, 0x8e, 0x66, 0xcf, 0x11, 0xa6, 0xd9, 0x00, 0xaa, 0x00, 0x62, 0xce, 0x6c };
//Content Description Object ��unicode
static const uint8 content_description_obj[16] =
{ 0x33, 0x26, 0xb2, 0x75, 0x8e, 0x66, 0xcf, 0x11, 0xa6, 0xd9, 0x00, 0xaa, 0x00, 0x62, 0xce, 0x6c };
//Extended Content Description Object��unicode
static const uint8 extended_content_description_obj[16] =
{ 0x40, 0xA4, 0xD0, 0xD2, 0x07, 0xE3, 0xD2, 0x11, 0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50 };

//  WM/AlbumTitle ��unicode
static const uint8 alb_descriptor_name[26] =
{
    0x57, 0x00, 0x4D, 0x00, 0x2F, 0x00, 0x41, 0x00,
    0x6C, 0x00, 0x62, 0x00, 0x75, 0x00, 0x6D, 0x00,
    0x54, 0x00, 0x69, 0x00, 0x74, 0x00, 0x6C, 0x00,
    0x65, 0x00
};
//  WM/Genre ��unicode
static const uint8 genre_descriptor_name[18] =
{ 0x57, 0x00, 0x4D, 0x00, 0x2F, 0x00, 0x47, 0x00, 0x65, 0x00, 0x6E, 0x00, 0x72, 0x00, 0x65, 0x00, 0x00, 0x00 };
//  WM/TrackNumber ��unicode
static const uint8 tracknum_descriptor_name[28] =
{
    0x57, 0x00, 0x4D, 0x00, 0x2F, 0x00, 0x54, 0x00,
    0x72, 0x00, 0x61, 0x00, 0x63, 0x00, 0x6B, 0x00,
    0x4E, 0x00, 0x75, 0x00, 0x6D, 0x00, 0x62, 0x00,
    0x65, 0x00, 0x72, 0x00
};
//  WM/Picture ��unicode
static const uint8 picture_descriptor_name[20] =
{
    0x57, 0x00, 0x4D, 0x00, 0x2F, 0x00, 0x50, 0x00,
    0x69, 0x00, 0x63, 0x00, 0x74, 0x00, 0x75, 0x00,
    0x72, 0x00, 0x65, 0x00
};

static const tag_string_t tag_string_info[EXTENDED_TYPE_CN] =
{
    { 0x00, 0x00 },
    { 0x00, 0x00 },
    { (uint8*) alb_descriptor_name, 26 },
    { (uint8*) genre_descriptor_name, 18 },
    { 0x00, 0x00 },
    { (uint8*) tracknum_descriptor_name, 28 },
    { (uint8*) picture_descriptor_name, 20 },
    { 0x00, 0x00 }
};

//#pragma memory=default
//#pragma memory=dataseg(MOD_EH_WMATAG)
//static char check_flag[EXTENDED_TYPE_CN];
//static uint8 id3_temp_buf[SECTOR_SIZE];
//static uint8 key_buf[KEYSTR_BUF_LEN];
static uint8 tag_info_tab[sizeof(asf_tagobj_t)] _BANK_DATA_ATTR_; //
//static handle id3_handle;

//bool TAGInfoFlag;//Ϊ���ʾ��TAG��Ϣ
//#pragma memory=default

static bool wma_content_object(uint32 objsize);
static bool wma_excontent_object(uint32 objsize);
static uint16 wma_mini_num(uint16 a, uint16 b);

/******************************************************************************/
/*
 * \par  Description:����wma���͵�id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_wma(void)
{
    bool chk_content_flag, chk_excontent_flag;
    uint32 tmp_pos_file;
    uint32 obj_total_size, object_size;

    chk_excontent_flag = TRUE;
    chk_content_flag = TRUE;

    get_init();

    if (libc_memcmp(id3_temp_buf, asf_header_obj, 16) != 0) //compare header object
    {
        return FALSE;
    }

    obj_total_size = *(uint32*) (id3_temp_buf + 16); //����header object ��ObjectSize

    id3_pos_file = 30;
    id3_pos_buffer = 30;

    while (id3_pos_file < obj_total_size)
    {
        read_buf_data(key_buf, 24);
        object_size = *(uint32*) (key_buf + 16);
        tmp_pos_file = 0;

        if (chk_content_flag == TRUE)
        {
            if (libc_memcmp(key_buf, content_description_obj, 16) == 0)
            {
                tmp_pos_file = id3_pos_file;
                wma_content_object(object_size);
                chk_content_flag = FALSE;
                goto next_obj;
            }
        }

        if (chk_excontent_flag == TRUE)
        {
            if (libc_memcmp(key_buf, extended_content_description_obj, 16) == 0)
            {
                tmp_pos_file = id3_pos_file;
                wma_excontent_object(object_size);
                chk_excontent_flag = FALSE;
                goto next_obj;
            }

        }

        next_obj:

        if ((chk_content_flag == FALSE) && (chk_excontent_flag == FALSE))
        {
            break;
        }

        if (object_size >= 24)
        {
            reset_file_pos(object_size - 24, tmp_pos_file);
        }
        else
        {
            return FALSE; //error file
        }

    }

    return TRUE;
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
static int8 wma_check_pic_flag(uint8 *buf)
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
 * \par  Description:����conten֡
 *
 * \param[in]    objsize--֡�ĳ��ȣ�byte

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool wma_content_object(uint32 objsize)
{
    //----��TAG������Ϣ�ĳ��ȱ�

    uint16 j;
    asf_tagobj_t *tag_info_tab_p;

    sys_os_sched_lock();
    
    tag_info_tab_p = (asf_tagobj_t *) tag_info_tab;

    read_buf_data(tag_info_tab, sizeof(asf_tagobj_t));

    //-----�� TIT2 ��Ϣ---
    //ȡ��Ҫ�������ʵ���������С���Ǹ�ֵ�����

    if (id3_info_p->tit2_length > 4) //��ʶ��+������=4
    {
        //���Ȱ���������
        j = wma_mini_num(id3_info_p->tit2_length - 2, tag_info_tab_p->title_len);
        if (j > 2) //���Ȱ������������ȣ�=2bytes��
        {
            read_buf_data(&id3_info_p->tit2_buffer[2], j);
            *(uint16*) &id3_info_p->tit2_buffer[0] = UNICODE_FLAG;
            *(uint16*) &id3_info_p->tit2_buffer[j & 0xFFFE] = 0x00;

            if (j < tag_info_tab_p->title_len)
            {
                //����û�ж�����id3��Ϣ
                reset_file_pos(tag_info_tab_p->title_len - j, 0);
            }

        }
        else
        {
            reset_file_pos(tag_info_tab_p->title_len, 0);
        }

    }

    //------�� TPE1 ��Ϣ---
    if (id3_info_p->tpe1_length > 4)
    {
        //���Ȱ���������
        j = wma_mini_num(id3_info_p->tpe1_length - 2, tag_info_tab_p->author_len);
        if (j > 2)
        {
            read_buf_data(&id3_info_p->tpe1_buffer[2], j);
            *(uint16*) &id3_info_p->tpe1_buffer[0] = UNICODE_FLAG;
            *(uint16*) &id3_info_p->tpe1_buffer[j & 0xFFFE] = 0x00;

            if (j < tag_info_tab_p->author_len)
            {
                //����û�ж�����id3��Ϣ
                reset_file_pos(tag_info_tab_p->author_len - j, 0);
            }
        }
        else
        {
            reset_file_pos(tag_info_tab_p->author_len, 0);
        }

    }

    //------�� drm ��Ϣ---
    if (id3_info_p->drm_length > 4)
    {
        //���Ȱ���������
        j = wma_mini_num(id3_info_p->drm_length - 2, tag_info_tab_p->copyright_len);
        if (j > 2)
        {
            read_buf_data(&id3_info_p->drm_buffer[2], j);
            *(uint16*) &id3_info_p->drm_buffer[0] = UNICODE_FLAG;
            *(uint16*) &id3_info_p->drm_buffer[j & 0xFFFE] = 0x00;
            /*
             if(j < tag_info_tab_p->copyright_len)
             {
             //����û�ж�����id3��Ϣ
             reset_file_pos(tag_info_tab_p->copyright_len-j,0);
             }
             */
        }
        /*
         else
         {
         reset_file_pos(tag_info_tab_p->copyright_len,0);
         }
         */
    }

    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ƥ���ַ���
 *
 * \param[in]

 * \param[out]

 * \return        �ɹ�>0,ʧ��<0

 * \note
 *******************************************************************************/
//return -1,0~7
static char wma_check_tag(void)
{
    uint8 i;
    for (i = 0; i < EXTENDED_TYPE_CN; i++)
    {
        if ((check_flag[i] == 1) && (tag_string_info[i].str_len > 0))
        {
            if (libc_memcmp(key_buf, tag_string_info[i].str_buf, (uint32) tag_string_info[i].str_len) == 0)
            {
                check_flag[i] = 0;
                break;
            }

        }

    }
    if (i >= EXTENDED_TYPE_CN)
    {
        return -1;
    }

    //���ý�Ҫ����ĵ�ַ�ͳ���,��talb��ʼ
    id3_save_p->saveaddr = (void*) *((uint32*) id3_info_p + i);
    id3_save_p->savelen = *((uint8*) id3_info_p + 24 + i);

    return i;
}

/******************************************************************************/
/*
 * \par  Description:����exconten֡
 *
 * \param[in]    objsize--֡�ĳ��ȣ�byte

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool wma_excontent_object(uint32 objsize)
{
    int8 result;
    uint8 count = EXTENDED_TYPE_CN;
    uint8 N = 0; //Ҫ�ҵĹؼ��ֵĸ���
    uint16 descriptors_count;
    uint16 namelen;
    uint16 value_length;
    uint16 value_type;
    //uint16 offset;

    read_buf_data(key_buf, 2);
    descriptors_count = *(uint16*) &key_buf[0];

    if (descriptors_count < count)
    {
        count = (uint8) descriptors_count;
    }
    objsize = objsize + id3_pos_file; //�ܵ�extend content �Ĵ�С

    while ((id3_pos_file < objsize) && (N < count))
    {
        read_buf_data(key_buf, 2);
        namelen = *((uint16*) &key_buf[0]);
        read_buf_data(key_buf, namelen);

        result = (int8) wma_check_tag();
        read_buf_data(key_buf, 4); //ǰ������ʾDescriptor Value Data Type
        value_type = *((uint16*) &key_buf[0]);
        value_length = *((uint16*) &key_buf[2]);

        if (result == (int8) -1)
        {
            reset_file_pos(value_length, 0);
            continue;
        }

        N++;

        if (value_length < 4)
        {
            //value_length==2��ֻ�н�����
            reset_file_pos(value_length, 0);
            continue;
        }

        if ((result == 6) && (id3_info_p->apic_flag))
        {
            read_buf_data(key_buf, 64);
            result = wma_check_pic_flag(key_buf);
            if (result != -1)
            {
                id3_info_p->apic_offset = id3_pos_file + (uint8) result - 64;
                id3_info_p->apic_type = 1;
                reset_file_pos(value_length - 64, 0);
            }
            else
            {
                id3_info_p->apic_type = 0;
                reset_file_pos(value_length, id3_pos_file - 64);
            }
            continue;
        }

        if (id3_save_p->savelen == 0)
        {
            reset_file_pos(value_length, 0);
            continue;
        }

        if (value_type == 0)
        {
            if (value_length > (id3_save_p->savelen - 4))
            {
                namelen = id3_save_p->savelen - 4;
            }
            else
            {
                namelen = value_length;
            }
            read_buf_data(id3_save_p->saveaddr + 2, namelen);
            //���ﲹ�ַ�������ע���βӦ��������0
            *(uint16*) (id3_save_p->saveaddr + (namelen & 0xFFFE) + 2) = 0;
            *(uint16*) (id3_save_p->saveaddr) = UNICODE_FLAG;

            if (namelen < value_length)
            {
                //����û�ж�����ID3��Ϣ
                reset_file_pos(value_length - namelen, 0);
            }
        }
        else if (result == 5)
        {
            id3_info_p->track_num = (uint16) (*((uint32 *) (&key_buf[0])));
        }
        else
        {
            ; //������
        }
    }//while

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:��a��b����Сֵ
 *
 * \param[in]

 * \param[out]

 * \return        ��Сֵ

 * \note
 *******************************************************************************/
static uint16 wma_mini_num(uint16 a, uint16 b)
{
    if (b < a)
    {
        return b;
    }
    return a;
}

