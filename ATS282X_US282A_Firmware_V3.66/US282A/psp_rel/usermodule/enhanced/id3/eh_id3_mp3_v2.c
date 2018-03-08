#include "eh_id3.h"

#define ID3_COUNT1 7
#define ID3_COUNT2 7

typedef struct
{
    uint8 header[3];/*����Ϊ"ID3"������Ϊ��ǩ������*/
    uint8 ver;/*�汾��;ID3V2.3�ͼ�¼03,ID3V2.4�ͼ�¼04*/
    uint8 revision;/*���汾��;�˰汾��¼Ϊ00*/
    uint8 flag;/*��ű�־���ֽڣ�����汾ֻ��������λ*/
    uint8 size[4];/*��ǩ��С��������ǩͷ��10���ֽں����еı�ǩ֡�Ĵ�С*/
} id3_10_header; //ID3V2.*��ǩͷ(ǰ10byte)

typedef struct
{
    uint8 frame_id[4]; /*���ĸ��ַ���ʶһ��֡��˵�������ݣ��Ժ��г��õı�ʶ���ձ�*/
    uint8 size[4]; /*֡���ݵĴ�С��������֡ͷ������С��1*/
    uint8 flags[2];/*��ű�־��ֻ������6λ*/
    uint8 encode_type; //frame  �������
} id3_2_3_header;

typedef struct
{
    uint8 frame_id[3]; /*���ĸ��ַ���ʶһ��֡��˵��������*/
    uint8 size[3]; /*֡���ݵĴ�С��������֡ͷ������С��1*/
    uint8 encode_type; //frame  �������
} id3_2_2_header;

//extern char *id3_temp_buf;
static const char id3_tab_v2_3[] = "TIT2TPE1TALBTCONTENCTRCKAPIC";
// ID3V2.3��Ҫ���ҵ�FrameID Table
static const char id3_tab_v2_2[] = "TT2TP1TALXXXTENTRKXXX";
// ID3V2.2��Ҫ���ҵ�FrameID Table

//���ܶ��� BANK_DATA,����BANK
//static uint8 id3_version _BANK_DATA_ATTR_; // v2.4.0 ��־ add by lmx 2011-8-8 11:15
//static uint32 id3_size _BANK_DATA_ATTR_;

extern bool mp3_select_genre(id3_info_t* id3_info_r, uint8 genre_index) __FAR__;
bool check_genre_string(void);

/******************************************************************************/
/*
 * \par  Description: ƥ���ַ���
 *
 * \param[in]    check_str
 tab_str
 size_str--tab_str�ַ����ĳ���
 tab_count--tab_str�ַ�����ĸ���
 * \param[out]

 * \return        �ɹ�>0,ʧ��<0

 * \note
 *******************************************************************************/

static int8 mp3_check_tag(uint8 *check_str, uint8 *tab_str, uint8 size_str, uint8 tab_count)
{
    uint8 i;

    for (i = 0; i < tab_count; i++)
    {
        if (check_flag[i] == 1)
        {
            if (libc_memcmp(check_str, tab_str, (uint32) size_str) == 0)
            {
                check_flag[i] = 0;
                break;
            }
        }
        tab_str = tab_str + size_str;
    }

    if (i == tab_count)
    {
        return -1;
    }

    //���ý�Ҫ����ĵ�ַ�ͳ���
    id3_save_p->saveaddr = (void*) *((uint32*) id3_info_p + i);
    id3_save_p->savelen = *((uint8*) id3_info_p + 24 + i);

    return i;
}


/******************************************************************************/
/*
 * \par  Description: ����V2.2�汾
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool mp3_v2_2_parse(uint32 id3_size_param)
{
    uint8 num;
    uint8 str_len;  
    int8 result;
    uint32 frame_len;
    id3_2_2_header* frame_h;

    num = 0;
    frame_h = (id3_2_2_header*) key_buf;

    while ((id3_pos_file < id3_size_param) && (num < ID3_COUNT1))
    {
        read_buf_data((uint8*) frame_h, sizeof(id3_2_2_header));
        frame_len = ((uint32)frame_h->size[0]) * 0x10000
            + ((uint32)frame_h->size[1] * 0x100)
            + ((uint32)frame_h->size[2]);

        if (frame_len == 0)
        {
            if (*(uint16*) frame_h->frame_id == 0x00)
            {
                //padding data
                break;
            }
            //����Ϊ�գ�������Ƚ�����
            //֮ǰ�����һ���ֽڣ����µ���ָ��
            reset_file_pos(0, id3_pos_file - 1);
            continue;
        }

        frame_len--; //��ȥ��������1���ֽڳ���

        if (frame_h->frame_id[0] != 'T')
        {
            reset_file_pos(frame_len, 0);
            continue;
        }

        result = mp3_check_tag(frame_h->frame_id, (uint8 *)&id3_tab_v2_2,
            3, ID3_COUNT1);
        if (result == (int8) -1)
        {
            reset_file_pos(frame_len, 0);
            continue;
        }

        num++;

        if (id3_save_p->savelen == 0)
        {
            reset_file_pos(frame_len, 0);
            continue;
        }

        str_len = (uint8) frame_len;

        if (frame_h->encode_type == 0)
        {
            //asc
            if (str_len > (id3_save_p->savelen - 1))
            {
                str_len = id3_save_p->savelen - 1;
            }
            read_buf_data(id3_save_p->saveaddr, str_len);
            
//            if(check_long_name_flag==1) //qac����û�ù�����
//            {
//                check_long_name_flag=0;
//                check_odd_char_in_str(id3_save_p->saveaddr, str_len);
//            }

            *(uint8*) (id3_save_p->saveaddr + str_len) = 0x00;
//            if(((id3_save_p->savelen & 1) == 0)
//                && (id3_save_p->saveaddr[str_len - 1] & 0x80) != 0)
//            {
//                //savelen��ż��
//                //��������ַ������ǹ�����
//                //���ԣ�Ҫ����������ֽڶ���0
//                id3_save_p->saveaddr[str_len - 1] = 0;
//            }
            *(uint8*) (id3_save_p->saveaddr + str_len) = 0x00;
        }
        else if (frame_h->encode_type == 1)
        {
            //unicode
            if (str_len > (id3_save_p->savelen - 2))
            {
                str_len = id3_save_p->savelen - 2;
            }

            read_buf_data(id3_save_p->saveaddr, str_len);
            if (*(uint16*) id3_save_p->saveaddr != UNICODE_FLAG)
            {
                //�쳣��������
                *(uint16*) id3_save_p->saveaddr = 0x00;
            }
            else
            {
                *(uint16*) (id3_save_p->saveaddr + (str_len & 0xFFFE)) = 0x00; //&0xFFFE,bit0Ϊ1��ֹ�쳣
            }

        }
        else if (frame_h->encode_type == 3)
        {
            //utf8

            str_len = (uint8)id3_read_utf8_deal(id3_save_p, str_len, NULL);

            //if (str_len > (id3_save_p->savelen - 4))
            //{
            //  str_len = id3_save_p->savelen - 4;
            //}
            //
            //id3_save_p->saveaddr[0] = UTF8_FLAG_0;
            //id3_save_p->saveaddr[1] = UTF8_FLAG_1;
            //id3_save_p->saveaddr[2] = UTF8_FLAG_2;
            //read_buf_data(id3_save_p->saveaddr + 3, str_len);
            //*(uint8*) (id3_save_p->saveaddr + 3 + str_len) = 0x00;

        }
        else
        {
            //�������벻֧�֣����
            id3_save_p->saveaddr[0] = 0;
        }

        if ((frame_len - str_len) > 0)
        {
            reset_file_pos(frame_len - str_len, 0);
        }

    } //while

    if (num == 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ����pic���
 *
 * \param[in]    buf

 * \param[out]

 * \return       �ɹ�>0,ʧ��<0

 * \note
 *******************************************************************************/
static int8 mp3_check_pic_flag(uint8 *buf)
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
 * \par  Description:����V2.3��V2.4�汾
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool mp3_v2_x_parse(uint32 id3_size_param, uint8 version)
{
    uint8 num;
    uint8 str_len;
    int8 result;
    uint32 frame_len;
    uint8 check_long_name_flag=0;
    id3_2_3_header* frame_h;

    num = 0;
    frame_h = (id3_2_3_header*) key_buf;

    while ((id3_pos_file < id3_size_param) && (num < ID3_COUNT2))
    {
        read_buf_data((uint8*) frame_h, sizeof(id3_2_3_header));

        if (4 == version) // v2.4.0
        {
            frame_len = ((uint32)(frame_h->size[0] & 0x7F)) * 0x200000
                + ((uint32)(frame_h->size[1] & 0x7F)) * 0x4000
                + ((uint32)(frame_h->size[2] & 0x7F)) * 0x80
                + ((uint32)(frame_h->size[3] & 0x7F));

        }
        else
        {
            frame_len = ((uint32)frame_h->size[0]) * 0x1000000
                + ((uint32)frame_h->size[1]) * 0x10000
                + ((uint32)frame_h->size[2]) * 0x100
                + ((uint32)frame_h->size[3]);
        }

        if (frame_len == 0)
        {
            if (*(uint32*) frame_h->frame_id == 0)
            {
                //padding data
                break;
            }
            //����Ϊ�գ�������Ƚ�����
            //֮ǰ�����һ���ֽڣ����µ���ָ��
            reset_file_pos(0, id3_pos_file - 1);
            continue;
        }
        else
        {
            frame_len--; //��ȥ��������1���ֽڳ���
        }

        if ((frame_h->frame_id[0] == 'A') && (id3_info_p->apic_flag))
        {
            result = mp3_check_tag(frame_h->frame_id, (uint8*) &id3_tab_v2_3, 4, ID3_COUNT2);
            if(result == -1)
            {
                reset_file_pos(frame_len, 0);
                continue;
            }
            num++;
            read_buf_data(key_buf, 64);
            result = mp3_check_pic_flag(key_buf);
            if (result != -1)
            {
                id3_info_p->apic_offset = id3_pos_file - 64 + (uint32)result;
                id3_info_p->apic_type = 1;
            }
            else
            {
                id3_info_p->apic_type = 0;
            }

            reset_file_pos(frame_len - 64, 0);
            continue;
        }

        if (frame_h->frame_id[0] != 'T')
        {
            reset_file_pos(frame_len, 0);
            continue;
        }

        result = mp3_check_tag(frame_h->frame_id, (uint8*) &id3_tab_v2_3, 4, ID3_COUNT2 - 1);
        if(result == -1)
        {
            reset_file_pos(frame_len, 0);
            continue;
        }

        num++;

        if (id3_save_p->savelen == 0)
        {
            reset_file_pos(frame_len, 0);
            continue;
        }

        str_len = (uint8) frame_len;

        if (frame_h->encode_type == 0)
        {
            //asc
            if (str_len > (id3_save_p->savelen - 1))
            {
                str_len = id3_save_p->savelen - 1;
                check_long_name_flag=1;
            }
            read_buf_data(id3_save_p->saveaddr, str_len);
            if(check_long_name_flag==1)
            {
                check_long_name_flag=0;
                check_odd_char_in_str(id3_save_p->saveaddr, str_len);
            }
            *(uint8*) (id3_save_p->saveaddr + str_len) = 0x00;
            
        }
        else if (frame_h->encode_type == 1)
        {
            //unicode
            if (str_len > (id3_save_p->savelen - 2))
            {
                str_len = id3_save_p->savelen - 2;
            }

            read_buf_data(id3_save_p->saveaddr, str_len);
            if (*(uint16*) id3_save_p->saveaddr != UNICODE_FLAG)
            {
                //�쳣��������
                *(uint16*) id3_save_p->saveaddr = 0x00;
            }
            else
            {
                *(uint16*) (id3_save_p->saveaddr + (str_len & 0xFFFE)) = 0x00;
            }
        }
        else if (frame_h->encode_type == 3)
        {
            //utf8
            str_len = (uint8)id3_read_utf8_deal(id3_save_p, str_len, NULL);

            //if (str_len > (id3_save_p->savelen - 4))
            //{
            //  str_len = id3_save_p->savelen - 4;
            //}
            //
            //id3_save_p->saveaddr[0] = UTF8_FLAG_0;
            //id3_save_p->saveaddr[1] = UTF8_FLAG_1;
            //id3_save_p->saveaddr[2] = UTF8_FLAG_2;
            //read_buf_data(id3_save_p->saveaddr + 3, str_len);
            //*(uint8*) (id3_save_p->saveaddr + 3 + str_len) = 0x00;
        }
        else
        {
            //�������벻֧�֣����
            id3_save_p->saveaddr[0] = 0;
        }

        if ((frame_len - str_len) > 0)
        {
            reset_file_pos(frame_len - str_len, 0);
        }
    } //while

    if (num == 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:����mp3���͵�V2�汾id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool mp3_v2_parse(void)
{
    //uint32 temp_num = 0;
    id3_10_header *id3h;
    uint32 id3_size;
    bool ret_val;
    uint8 id3_version;

    get_init(); //��ȡ512byte�� id3_temp_buf
    read_buf_data(key_buf, sizeof(id3_10_header));
    id3h = (id3_10_header*) key_buf;

    if ((id3h->header[0] != 'I') || (id3h->header[1] != 'D') || (id3h->header[2] != '3'))
    {
        return FALSE;
    }

    id3_version = id3h->ver;
    //ID3Size������ID3V2�ĳ���
    id3_size = sizeof(id3_10_header)
        + (uint32)(id3h->size[0] & 0x7F) * 0x200000
        + (uint32)(id3h->size[1] & 0x7F) * 0x4000
        + (uint32)(id3h->size[2] & 0x7F) * 0x80
        + (uint32)(id3h->size[3] & 0x7F);

    if (id3_version < 3)
    {
        ret_val = mp3_v2_2_parse(id3_size);
    }
    else
    {
        ret_val = mp3_v2_x_parse(id3_size, id3_version);
    }

    if ((FALSE != ret_val) && (id3_info_p->genre_length > 0))
    {
        check_genre_string();
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//���genre�Ƿ���"(num)"����ʽ���Ǿ�ת�����ַ���
bool check_genre_string(void)
{
    uint8 i, temp;
    uint8 genre_num;
    bool ret_val = FALSE;

    if (id3_info_p->genre_buffer == NULL)
    {
        return FALSE;
    }
    if (id3_info_p->genre_buffer[0] != '(')
    {
        return FALSE;
    }

    i = 1;
    genre_num = 0;
    temp = id3_info_p->genre_buffer[i];

    while (temp != '\0')
    {
        if (temp == ')')
        {
            if ((i != 1) && (i < 5))
            {
                //1~3λ����
                ret_val = TRUE;
            }
            break;
        }

        if ((temp >= '0') && (temp <= '9'))
        {
            genre_num = genre_num * 10 + (temp - '0');
        }
        else
        {
            ret_val = FALSE;
            break;
        }
        i++;
        temp = id3_info_p->genre_buffer[i];
    }

    if (FALSE != ret_val)
    {
        mp3_select_genre(id3_info_p, genre_num);
    }

    return ret_val;
}

