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
 * \file     mainmenu_main.c
 * \brief    MainMenu��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_upgrade.h"
#include "ap_manager_test.h"

//�������ݽṹ��
PCFWDecrypt_struct g_decrypt;

typedef uint32 (*p_fw_decrypt_init)(PCFWDecrypt_struct *decrypt_rw);
typedef void (*p_fw_decrypt_run)(void *, void *, void *);

/******************************************************************************/
/*!
 * \par  Description:
 *      ��ȡ�ļ����ݲ�����
 * \param[in]    mode           0 ��ʼ�� 1��������
 * \param[in]    read_len       ��ȡ�����ݳ���
 * \return       the read len
 * \note
 *******************************************************************************/
uint32 _read_file_translate(uint8 *read_buffer, uint32 read_len)
{
    uint32 read_file_offset;
    uint32 decrypt_mode;
    uint32 once_read_len;
    //uint32 true_read;
    uint32 need_read_len;
    uint32 true_read_offset;
    uint32 ret_val = 0;

    //vfs_file_tell(_para.g_file_sys_id, &read_file_offset, _para.file_handle);
    read_file_offset = g_upg_file_pos;

    if (read_file_offset < MIN_ONCE_READ_LEN)
    {
        decrypt_mode = 0;

        //ǰ18k������Ҫ�����ȡ������
        if (read_file_offset != 0)
        {
            //vfs_file_seek(_para.g_file_sys_id, 0, SEEK_SET, _para.file_handle);
            g_upg_file_pos = 0;
        }
    } 
    else
    {
        decrypt_mode = 1;
    }

    //true_read = 1;

    need_read_len = read_len;

    //�п��ܳ�����Ҫ��ζ�ȡ���ݣ��ҿ�Խ����ͷ������
    while (need_read_len != 0)
    {
        if (need_read_len < MIN_ONCE_READ_LEN)
        {
            once_read_len = need_read_len;
        }
        else
        {
            once_read_len = MIN_ONCE_READ_LEN;
        }

        //���ٱ�֤��ȡ18k����
        if ((once_read_len < MIN_ONCE_READ_LEN) && (decrypt_mode == 0))
        {
            once_read_len = MIN_ONCE_READ_LEN;
        }

        //if (true_read == 1)
        {
            //��ȡ�ļ�
            //vfs_file_read(_para.g_file_sys_id,  once_read_len, _para.file_handle);
            att_test_fread(g_decrypt.pInOutBuffer, once_read_len, g_upgrade_file_offset + g_upg_file_pos);
            
            g_upg_file_pos += once_read_len;

            if (decrypt_mode == 0)
            {
                g_decrypt.InOutLen = DECRYPT_USR_BUF_LEN;

                PRINT_INFO("firt decrypt init");
                PCFWDecryptInit(&g_decrypt);
                once_read_len = (uint32) g_decrypt.InOutLen;

                PRINT_INFO("firt decrypt over");
            }
            else
            {
                PCFWDecrypt(g_decrypt.pInOutBuffer, (void *) once_read_len, g_decrypt.pGLBuffer);
            }
        }

        if (once_read_len != 0)
        {
            if (decrypt_mode != 0)
            {
                libc_memcpy(read_buffer, g_decrypt.pInOutBuffer, once_read_len);

            }
            else
            {
                //�жϽ���ͷ�����Ƿ񳬹�16k�ĳ���
                if (once_read_len > read_file_offset)
                {
                    once_read_len = once_read_len - read_file_offset;
                }
                else
                {
                    //��������ʵ�ʽ��ܳ���Ϊ18k��ȥӦ�еĳ���
                    once_read_len = MIN_ONCE_READ_LEN - read_file_offset;
                }

                if (read_file_offset != 0)
                {
                    true_read_offset = (uint32)(read_file_offset - (uint32) g_firmware_offset);
                }
                else
                {
                    true_read_offset = 0;
                }

                libc_memcpy(read_buffer, g_decrypt.pInOutBuffer + true_read_offset, once_read_len);
                decrypt_mode = 1;
            }

            need_read_len -= once_read_len;
            read_buffer += once_read_len;
        }
    }

    return read_len;
}

void decrypt_init(void)
{
    int32 file_len = 0;

    g_decrypt.initusebuffer = (uint8 *) DECRYPT_INIT_BUF_ADDR;
    g_decrypt.initusebufferlen = DECRYPT_INIT_BUF_LEN;
    g_decrypt.pInOutBuffer = (uint8 *) DECRYPT_USE_BUF_ADDR;
    g_decrypt.InOutLen = DECRYPT_USR_BUF_LEN;
    g_decrypt.pGLBuffer = (uint8 *) DECRYPT_GL_BUF_ADDR;

    //vfs_file_get_size(_para.g_file_sys_id, &file_len, _para.file_handle, 0);
    g_decrypt.FileLength = (long) g_upg_file_len;

    g_upg_file_pos = 0;

    g_write_percent = 0;

    libc_memset(&_para, 0, sizeof(_para));
} 

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡ�ļ����ݲ�����
 * \param[in]    read_len       ��ȡ�����ݳ���
 * \param[in]    fp             �ļ����
 * \return       the result
 * \note
 *******************************************************************************/
uint32 _read_file(uint32 read_len)
{
    uint32 read_bytes;

    read_bytes = _read_file_translate(BUFFER_ADDR, read_len);

    return read_bytes;
}

