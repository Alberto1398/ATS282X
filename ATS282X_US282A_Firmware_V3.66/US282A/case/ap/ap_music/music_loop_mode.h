/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ѭ��ģʽ���ô���
 * ���ߣ�Gary Wang
 ********************************************************************************/
#ifndef __MUSIC_LOOP_MODE_H__
#define __MUSIC_LOOP_MODE_H__

#include "case_include.h"

//ѭ��ģʽ����Сֵ
#define MIN_LOOP_MODE   (((g_enter_mode == PARAM_RECORD_CPLAY)\
    || (g_enter_mode == PARAM_RECORD_UPLAY)) ? FSEL_MODE_LOOPONE : FSEL_MODE_LOOPALL)

//ѭ��ģʽ��ȱʡֵ
#define DEF_LOOP_MODE   (((g_enter_mode == PARAM_RECORD_CPLAY)\
    || (g_enter_mode == PARAM_RECORD_UPLAY)) ? FSEL_MODE_LOOPONEDIR : FSEL_MODE_LOOPALL)

app_result_e create_view_loop_mode(void);

#endif//__MUSIC_LOOP_MODE_H__
