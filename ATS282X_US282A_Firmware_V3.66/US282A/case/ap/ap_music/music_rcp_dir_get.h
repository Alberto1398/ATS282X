#ifndef __MUSIC_DIR_FILE_H__
#define __MUSIC_DIR_FILE_H__

#include "music.h"

typedef struct
{
    //��ʼ��Ŀ��
    uint16 begin_index;
    //������Ŀ��
    uint16 end_index;
    /*! �ļ���buffer */
    uint8 *pdir_buf;
    /*! �ļ���buffer���� */
    uint8 name_len;
} music_dir_info_t;

typedef enum
{
    FILE_LIST_VIEW = 0,     //�����б���ʾ
    DIR_LIST_VIEW = 1,               //Ŀ¼�б���ʾ
    OTHER,     //������ʾ��ʽ����
} list_show_way_e; 	//֧����ʾ��ʽ

typedef struct
{
    uint8 list_show_way;	//֧����ʾ��ʽ
    uint8 dir_amount;   //Ŀ¼����
    uint8 error_num;    //�����
} music_rcp_dir_list_status_t;  //  Ŀ¼list״̬

typedef struct
{
    uint8 sys_random[4];
    uint8 bl_random[6];
}music_rcp_dlist_random_t;//�����������6λ������ַ��4λϵͳʱ�������

typedef struct
{
    /*! ��ʼ��Ŀ�� */
    uint16 begin_index;
    /*! ������Ŀ�� */
    uint16 end_index;
    /*! Ŀ¼���������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 enctype;
    /*! Ŀ¼��buffer */
    uint8 dir_buffer[65];
} music_rcp_dir_list_info_t;    //Ŀ¼�б����ݰ�

#define MUSIC_LIST_SHOW_WAY DIR_LIST_VIEW

#endif //__MUSIC_H__

