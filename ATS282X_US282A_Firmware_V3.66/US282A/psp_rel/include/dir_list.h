/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2012-12-25 15:00     1.0             build this file
********************************************************************************/
#ifndef __DIR_LIST_H__
#define __DIR_LIST_H__

#include "vfs_interface.h"

//���Ŀ¼����
#define SUPPORT_MUSIC_DIR 204

//����ļ�����
#define SUPPORT_MUSIC_FILE 5000

//��ǰ��λ��־
#define PREV_FLAG 0x80000000

//Ŀ¼�б�����Ϣ
typedef struct
{
    //��ǰĿ¼����ʼ��Ŀ��(��1��ʼ����)
    uint16 begin_index;
    //��ǰĿ¼�Ĳ㼶
    uint8 cur_layer;
    uint8 reserve;
    //��ǰĿ¼�ĸ�Ŀ¼Ŀ¼��Ĵغ�
    uint32 far_cluster;
    //��ǰĿ¼�ĸ�Ŀ¼Ŀ¼���ƫ��
    uint32 far_direntry;
    //��ǰ�ļ���Ŀ¼��غ�
    uint32 cur_cluster;
    //��ǰ�ļ���Ŀ¼��ƫ��
    uint32 cur_direntry;
} list_dir_info_t;//��20���ֽ�

//�б�ͷ��Ϣ
typedef struct
{
	//�ļ�����
	uint16 file_total;
	//Ŀ¼����
	uint16 dir_total;
	//��ǰ�������
	uint8 cur_disk;
	//����������չ
	uint8  retain[3];
    uint32 file_newest_time;
}list_head_info_t;//��12���ֽ�

#endif //__DIR_LIST_H__
