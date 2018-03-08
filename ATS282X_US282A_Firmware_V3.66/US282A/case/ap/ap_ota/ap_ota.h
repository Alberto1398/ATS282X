/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������켰����Ŀ¼ɨ�蹦����غ궨�壬�ṹ�嶨�壬ģ����ȫ�ֱ���������
 *       ģ�����ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __APP_OTA_H__
#define __APP_OTA_H__

#include "psp_includes.h"
#include "case_include.h"
#include "music_common.h"
#include "enhanced.h"

/*!��������Ӧ�õ�����ջ�����߳����ȼ�*/
#define    AP_OTA_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_OTA_PRIO     AP_FRONT_LOW_PRIO

typedef app_result_e (*key_func)(void);

//ϵͳȫ�ֱ���
extern comval_t g_comval;

extern bool display_flag;
extern int8 display_timer_id;
extern uint8 g_ota_status;
extern bool g_need_draw;
extern uint8 g_tick;

enum
{
	OTA_STATUS_IDLE = 0,
	OTA_STATUS_CONNECT = 1,
	OTA_STATUS_UPGRADE = 2,
};

extern app_result_e get_message_loop(void) __FAR__;
extern void ota_main_view(view_update_e mode) __FAR__;
extern void ota_create_main_view(void) __FAR__;

extern const sys_event_map_t ota_se_maplist[];
extern const key_event_map_t ota_ke_maplist[];

#if (SUPPORT_LED_DRIVER == 1)
extern void wait_ota_display(void) __FAR__;
extern void ota_ui_display(void) __FAR__;
#endif

#endif //__APP_OTA_H__
