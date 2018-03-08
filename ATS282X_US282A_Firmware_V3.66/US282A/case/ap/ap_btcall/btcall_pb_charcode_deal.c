/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *        brief   �ַ���ת��
 *      <author>       <time>
 *       Wekan   2015-3-27
 *******************************************************************************/

#include "ap_btcall.h"

const char codepage_table[] = "FTBL_GB.$$$"; //unicode to charת����

/******************************************************************************
 * \par  Description:   ��utf8��ʽ���ַ���ת����unicode��ʽ
 * \param[in]    tts_info
 * \param[out]   tts_info
 * \return       void
 * \  note  :   ע��������Ͳ�������ͬһ��ָ��
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

void utf8_to_unicode(uint8 *tts_info)
{
    //save dest data
    uint8 tmp_buf[MAX_TTS_INFO_LENGTH];
    //save source data
    uint8 src_buf[MAX_TTS_INFO_LENGTH];

    uint8* dst_info;

    uint8* src_info;

    uint8 i, j, tmp;

    uint16 c;
    libc_memset(tmp_buf, 0x0, MAX_TTS_INFO_LENGTH);

    libc_memcpy(src_buf, tts_info, MAX_TTS_INFO_LENGTH);

    src_info = src_buf;

    dst_info = tmp_buf;
    j = 0;
    for (i = 0; i < MAX_TTS_INFO_LENGTH; i += 2)
    {
        tmp = *src_info;
        //����
        if (tmp == 0x0)
        {
            break;
        }
        if (tmp < 0x80)// ���ֽ�    0xxxxxxx(00-7F) ==> 0xxxxxxx
        {
            c = *src_info;
            src_info++;
            j += 1;
        }
        else if (tmp < 0xe0)// ˫�ֽ�`  110xxxxx(C0-DF) 10yyyyyy(80-BF) ==> 00000xxx xxyyyyyy
        {
            c = ((uint16) (*src_info++) & 0x1f) << 6;
            c += ((uint16) (*src_info++) & 0x3f);
            j += 2;
        }
        else if (tmp < 0xf0)// ���ֽ�   1110xxxx(E0-EF) 10yyyyyy 10zzzzzz ==> xxxxyyyy yyzzzzzz
        {
            c = ((uint16) (*src_info++) & 0x0f) << 12;
            c += ((uint16) (*src_info++) & 0x3f) << 6;
            c += ((uint16) (*src_info++) & 0x3f);
            j += 3;
        }
        else // ���ֽڻ���� ��֧��
        {
            c = 0;
        }
        if (j > MAX_TTS_INFO_LENGTH)//Դ���ȳ���
        {
            c = 0;
        }
        *dst_info = (uint8) (c & 0xff);
        dst_info++;
        *dst_info = (uint8) (c >> 8);
        dst_info++;
        if (c == 0)
        {
            break;
        }
    }
    libc_memcpy(tts_info, tmp_buf, MAX_TTS_INFO_LENGTH);
}

/******************************************************************************
 * \par  Description:   �����ֿ�ת������ȡunicode���ַ���Ӧ���������
 * \param[in]    codepage_hd
 * \param[in]    code_rename
 * \param[out]   null
 * \return       void
 * \  note  :
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

uint16 one_uni_to_char(sd_handle codepage_hd, uint16 code_rename)
{
    uint16 CharCode;
    uint32 FontCode;
    CharCode = code_rename;
    if (code_rename > 127)
    {
        FontCode = code_rename;
        FontCode = FontCode << 1;//����2
        if ((FontCode < 0x1FE00) && (FontCode >= 0x13F50))
        {
            CharCode = 0x2000; //ע���С��ת��

            return CharCode;
        }
        else if (FontCode >= 0x1FE00)
        {
            FontCode = FontCode - 0xBEB0;
        }
        else
        {
            ;//for qac worning
        }
        sys_sd_fseek(codepage_hd, SEEK_SET, FontCode);

        sys_sd_fread(codepage_hd, &CharCode, 2);

        if (CharCode == 0)
        {
            CharCode = '?';
        }
    }
    return CharCode;//TRUE;
}

/******************************************************************************
 * \par  Description:  ��ת����unicode�ַ������д���
 * \param[in]    codepage_hd
 * \param[in]    str_info
 * \param[out]   null
 * \return
 * \  note  :
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

uint8 string_uni_to_char(sd_handle codepage_hd, uint8 *str_info)
{
    uint16 s_cnt;
    uint16 d_cnt;
    uint16 FontCode;
    uint16 CharCode;
    uint8 des_str[MAX_TTS_INFO_LENGTH];
    uint8 src_str[MAX_TTS_INFO_LENGTH];
    uint8 *str;
    libc_memcpy(src_str, str_info, MAX_TTS_INFO_LENGTH);
    str = src_str;
    d_cnt = 0;
    for (s_cnt = 0; s_cnt < MAX_TTS_INFO_LENGTH; s_cnt += 2)
    {
        if ((str[s_cnt] == 0) && (str[s_cnt + 1] == 0))
        {
            break;
        }

        FontCode = *((uint16 *) &str[s_cnt]);
        if (FontCode == 0x0000)
        {
            break;
        }
        CharCode = one_uni_to_char(codepage_hd, FontCode);
        if (CharCode > 0x7f) //���ֽ������ת��
        {
            des_str[d_cnt] = *((uint8 *) (&CharCode)); //�Ѹ�λ�Ƶ���λȥ
            d_cnt++;
            des_str[d_cnt] = *((uint8 *) (&CharCode) + 1); //�Ѹ�λ�Ƶ���λȥ
            d_cnt++;
        }
        else
        {
            des_str[d_cnt] = *((uint8 *) (&CharCode)); //���˵�λ(&code_rename)+1);
            d_cnt++;
        }
    }

    des_str[d_cnt] = 0;

    libc_memcpy(str_info, des_str, MAX_TTS_INFO_LENGTH);
    return d_cnt;
}

/******************************************************************************
 * \par  Description:  ��unicode��ʽ���ַ���ת���������ʽ
 * \param[in]    str_info
 * \param[out]   null
 * \return
 * \  note  :
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

uint8 unicode_to_char(uint8 *str_info)
{
    sd_handle codepage_hd;

    //convert table file
    codepage_hd = sys_sd_fopen(codepage_table);
    //file open fail
    if (codepage_hd == NULL)
    {
        return FALSE;
    }
    string_uni_to_char(codepage_hd, str_info);

    sys_sd_fclose(codepage_hd);

    return TRUE;
}

