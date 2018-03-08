/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE ���������ݣ���Ҫ�Թ����ַ���Ϊ����ͨ��һ���ӿڽ��м��ء�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

#ifndef MUSIC_SCAN_SHARE_DATA
const uint8 g_ring_folder_root_short[] =
{
    0xc4, 0xd6, 0xd6, 0xd3, 0x00 //����
};
const uint8 g_ring_folder_root_long[] =
{
    0xff, 0xfe, 0xf9, 0x95, 0x9f, 0x94, 0x0, 0x0 //����
};

const uint8 g_ring_folder_long[RCP_RING_FOLDER_MAX][12] =
{
    "\xff\xfe\x77\x8d\x8a\x5e\x00\x00",/*��*/
    "\xff\xfe\x03\x54\x6d\x99\x00\x00",/*�Է�*/
    "\xff\xfe\x66\x5b\x60\x4e\x00\x00",/*ѧϰ*/
    "\xff\xfe\x61\x77\xc9\x89\x00\x00", /*˯��*/
};
const uint8 g_ring_folder_short[RCP_RING_FOLDER_MAX][12] =
{
    "\xc6\xf0\xb4\xb2\x00",/*��*/
    "\xb3\xd4\xb7\xb9\x00",/*�Է�*/
    "\xd1\xa7\xcf\xb0\x00",/*ѧϰ*/
    "\xcb\xaf\xbe\xf5\x00", /*˯��*/
};

const uint8 g_ring_floder_type[RCP_RING_FOLDER_MAX] =
{
    RING_TYPE_FOLDER1, RING_TYPE_FOLDER2, RING_TYPE_FOLDER3, RING_TYPE_FOLDER4
};

//���������б�
const uint8 g_sd_ring_file[BUILDIN_RING_MAX][12] =
{
    "alarm1.mp3",
};
#endif
//ע�⣺�������Ҫ�����ͬʱҪ�ĵĵط����У�enhanced�����rec_dir_name
const uint8 g_rec_folder_long[] =
{
    0xff, 0xfe, 'R', 0x00, 'E', 0x00, 'C', 0x00, 'O', 0x00,
    'R', 0x00, 'D', 0x00, 0x00, 0x00
};
const uint8 g_rec_folder_short[] = "RECORD  ";

int com_load_share_data(int data_id, uint8 *data_buf, int data_len, int param)
{
    int ret = 0;

    switch (data_id)
    {
#ifndef MUSIC_SCAN_SHARE_DATA    
        case COM_SHARE_DATA_RING_FLODER_ROOT:
        if (param == 0)
        {
            libc_memcpy(data_buf, g_ring_folder_root_short, data_len);
        }
        else
        {
            libc_memcpy(data_buf, g_ring_folder_root_long, data_len);
        }
        break;

        case COM_SHARE_DATA_RING_FOLDER_SUB:
        if ((param & 0xff) >= RCP_RING_FOLDER_MAX)
        {
            break;
        }
        if ((param & 0xff00) == 0)
        {
            libc_memcpy(data_buf, g_ring_folder_short[param & 0xff], data_len);
        }
        else
        {
            libc_memcpy(data_buf, g_ring_folder_long[param & 0xff], data_len);
        }
        break;

        case COM_SHARE_DATA_RING_BUILDIN:
        if(param >= BUILDIN_RING_MAX)
        {
            ret = -1;
            break;
        }
        libc_memcpy(data_buf, g_sd_ring_file[param], data_len);
        break;

        case COM_SHARE_DATA_RING_TYPE:
        libc_memcpy(data_buf, g_ring_floder_type, data_len);
        break;
#endif    
        case COM_SHARE_DATA_REC_FOLDER:
        if (param == 0)
        {
            libc_memcpy(data_buf, g_rec_folder_short, data_len);
        }
        else
        {
            libc_memcpy(data_buf, g_rec_folder_long, data_len);
        }
        break;

        default:
        ret = -1;
        break;
    }

    return ret;
}
