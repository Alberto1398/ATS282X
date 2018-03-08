/********************************************************************************
 *                 Copyright(c) 2003-2015 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * ����������������صĺꡢö�����͡��ṹ��ȶ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BTCALL_COMMON_H__
#define __BTCALL_COMMON_H__

#include "psp_includes.h"
#include "common_btmanager.h"
#include "common_func.h"

#define CALLIN_RING_MODE_NORMAL   0 //����ģʽ����������
#define CALLIN_RING_MODE_ONLY_PB  1 //���е绰��
#define CALLIN_RING_MODE_RING_PB  2 //����+�绰��

typedef enum
{
    BTCALL_IDLE    = 0,
    BTCALL_STOP    = 1,
    BTCALL_CALLIN  = 2,
    BTCALL_CALLOUT = 3,
    BTCALL_HFP     = 4,
    BTCALL_PHONE   = 5,
	BTCALL_SIRI    = 6,
} btcall_status_e;

typedef struct
{
    btcall_status_e status;
    uint8 reserve[3];//�����ѯ�ṹ����Ҫ4�ֽڶ���
} btcall_info_t;

#endif
