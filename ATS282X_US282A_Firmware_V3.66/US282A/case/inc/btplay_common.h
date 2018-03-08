/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ���غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ�����Ƹ�ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BTPLAY_COMMON_H__
#define __BTPLAY_COMMON_H__

#include "psp_includes.h"
#include "common_btmanager.h"
#include "common_func.h"

typedef enum
{
    BTPLAY_IDLE  = 0,
    BTPLAY_STOP  = 1,
    BTPLAY_PLAY  = 2,
    BTPLAY_PAUSE = 3,
    //����������ӿ����
} btplay_status_e;

typedef struct
{
    btplay_status_e status;
    uint8 reserve[3];//�����ѯ�ṹ����Ҫ4�ֽڶ���
} btplay_info_t;

#endif
