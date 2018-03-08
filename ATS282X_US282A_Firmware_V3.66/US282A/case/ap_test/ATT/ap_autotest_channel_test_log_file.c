/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     music_channel_test.c
 * \brief    ͨ������ʵ��ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_manager_test.h"
#include "ap_autotest_channel_test.h"
#include "fm_interface.h"
#include "ap_autotest_channel_test_SNR.h"

#ifdef DEBUG_WRITE_CHANNEL_DATA
static const uint8 att_mic_file[] = "MIC.PCM";
static const uint8 att_linein_file[] = "LINEIN.PCM";

static uint8 tmp_buffer[30] _BANK_DATA_ATTR_;

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    �����ַ���תunicode�ַ���
 * \param[in]   src��Դ�ַ���ָ��
 * \param[in]   len��Ҫת���������ַ�������
 * \param[out]  dest��Ŀ���ַ���ָ��
 * \return      �Ƿ�ת���ɹ�
 * \retval      TRUE ת���ɹ�
 * \retval      FALSE ת��ʧ��
 * \ingroup     string
 * \note        Ŀ���ַ������������û���֤�����������������С��Ҫ���Ͻ�������
 *******************************************************************************/
bool char_to_unicode(uint8 *dest, uint8 *src, uint16 len)
{
    int s_cnt = 0;
    int d_cnt = 0;
    uint16 font_code;

    while (1)
    {
        if ((s_cnt >= len) || (src[s_cnt] == 0))//��β���߽�����

        {
            break;
        }

        if ((uint8) src[s_cnt] >= 0x80)
        {
            font_code = 0x3f;
        }
        else
        {
            font_code = src[s_cnt];
        }
        s_cnt++;

        dest[d_cnt] = *((uint8*) &font_code); //���ֽ�
        d_cnt++;
        dest[d_cnt] = *((uint8*) &font_code + 1); //���ֽ�
        d_cnt++;
    }
    dest[d_cnt] = 0;
    d_cnt++;
    dest[d_cnt] = 0;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����������ļ���ת��ΪUnicode�����ļ�����
 * \param[in]    file_name ��������ļ���
 * \param[out]   file_name ���Unicode�����ļ���
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1������������ļ���ת��ΪUnicode�����ļ���
 const char const_rec_name[] = "rec_001.wav";
 uint8 new_rec_name[26];

 libc_memcpy(new_rec_name, const_rec_name, 12);
 com_ansi_to_unicode(new_rec_name);
 //���ţ��Ϳ���ʹ�� new_rec_name ����������Ϊ rec_001.wav ���ļ���
 * \endcode
 * \note
 * \li  ���ڴ����ļ�ǰ���ļ���ת��ΪUnicode���룬����Ӧ exFat �ļ�ϵͳ��
 * \li  �û����Լ���֤ file_name �����Unicode�����ļ��������� Ӣ�������ļ���
 *      rec_001.wav��ת��Unicode�����ļ�������Ҫ��������СΪ 26 ���ֽڣ���
 *      Unicode�����־�ַ�0xfffe 2�ֽ� + 11���ַ� 2 * 11�ֽ� + ������0x0 0x0 2�ֽڡ�
 * \li  �����ڸ�����������С��52�ֽڣ����ļ������ܳ��� 24 ���ַ����ȡ�
 *******************************************************************************/
void com_ansi_to_unicode(uint8 *file_name)
{
    uint16 ansi_len = (uint16) libc_strlen(file_name);
    uint16 i;

    //�����ƶ�2�ֽڣ�ǰ����� 0xfffe
    for (i = ansi_len; i > 0; i--)
    {
        file_name[(i - 1) + 2] = file_name[i - 1];
    }
    file_name[0] = 0xff;
    file_name[1] = 0xfe;
    file_name[ansi_len + 2] = 0x0;

    //���ļ���ת��ΪUnicode���� com_name_buffer
    char_to_unicode(tmp_buffer, file_name + 2, ansi_len);

    //����Unicode����
    libc_memcpy(file_name + 2, tmp_buffer, ansi_len * 2 + 2);
}

void write_temp_file(uint8 file_index, uint8 *write_buffer, uint32 write_len)
{
    int file_handle;

    int ret_val;

    uint8 filename[64];

    libc_memset(filename, 0, 64);

    if(file_index == 0)
    {
        libc_memcpy(filename, att_mic_file, sizeof(att_mic_file));
    }
    else
    {
        libc_memcpy(filename, att_linein_file, sizeof(att_linein_file));
    }

    com_ansi_to_unicode(filename);

    //��λ����Ŀ¼
    vfs_cd(g_file_sys_id, CD_ROOT, 0);

    ret_val = vfs_file_dir_exist(g_file_sys_id, filename, 1);

    if(ret_val == 0)
    {
        file_handle = vfs_file_create(g_file_sys_id, filename, write_len);

        if(file_handle == 0)
        {
            libc_print("create recrod file error!\n", 0, 0);

            while(1);
        }

        vfs_file_write(g_file_sys_id, write_buffer, write_len, file_handle);

        vfs_file_close(g_file_sys_id, file_handle);
    }
    else
    {
        file_handle = vfs_file_open(g_file_sys_id, filename, OPEN_MODIFY);

        if(file_handle == 0)
        {
            libc_print("open file error!\n", 0, 0);

            while(1);
        }

        vfs_file_write(g_file_sys_id, write_buffer, write_len, file_handle);

        vfs_file_close(g_file_sys_id, file_handle);
    }

    return;

}

void read_temp_file(uint8 file_index, uint8 *read_buffer, uint32 read_len)
{
    int file_handle;

    int ret_val;

    uint8 filename[64];

    libc_memset(filename, 0, 64);

    if(file_index == 0)
    {
        libc_memcpy(filename, att_mic_file, sizeof(att_mic_file));
    }
    else
    {
        libc_memcpy(filename, att_linein_file, sizeof(att_linein_file));
    }

    com_ansi_to_unicode(filename);

    //��λ����Ŀ¼
    vfs_cd(g_file_sys_id, CD_ROOT, 0);

    ret_val = vfs_file_dir_exist(g_file_sys_id, filename, 1);

    if(ret_val == 0)
    {
        return;
    }
    else
    {
        file_handle = vfs_file_open(g_file_sys_id, filename, R_NORMAL_SEEK);

        if(file_handle == 0)
        {
            libc_print("open file error!\n", 0, 0);

            while(1);
        }

        vfs_file_read(g_file_sys_id, read_buffer, read_len, file_handle);

        vfs_file_close(g_file_sys_id, file_handle);
    }

    return;

}

#endif
