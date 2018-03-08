/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ���ڲ� COMMON ȫ�ֱ���������ǰ̨Ӧ�ö���Ӧ�ã����ӵ���ͬ�ĵ�ַ����
 *       ��֤COMMON BANK �δ�����һ�µģ���Щȫ�ֱ���ÿ��ǰ̨Ӧ�ü���ʱ�����ʼ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*!
 *  \brief
 *  sys_comval ϵͳ����ָ�룬ͨ�� com_set_sys_comval �ӿڴ��ݸ� common ��������ȫ��ָ��
 */
comval_t *sys_comval;

/*!
 *  \brief
 *  this_filter_key_value ���˵���������������Ҫ���˵İ���
 */
uint8 this_filter_key_value;

/*!
 *  \brief
 *  sys_counter_timer_id ϵͳ��ʱ���ܣ��������⣬������ʡ��ػ�����ʱ�ػ����ȣ���ʱ�� timer id
 */
int8 sys_counter_timer_id;

/*!
 *  \brief
 *  peripheral_detect_timer_id �����⣨����USB��UHOST��CARD��AUX����ʱ�� timer id
 */
int8 peripheral_detect_timer_id;

/*!
 *  \brief
 *  �Ƿ���Ҫ�����л�����
 */
bool g_ignore_switch_mute;

bool g_detect_card_force_flag;

clock_show_var_t *g_p_clock_show;
set_calendar_var_t *g_p_set_calendar;
set_alarm_var_t *g_p_set_alarm;

view_infor_t g_view_infor[MVC_VIEW_DEEPTH];
view_manager_infor_t g_view_manager_infor;
key_event_t * g_p_view_ke;

low_power_var_t g_low_power_var;

standby_enter_hook g_standby_enter_cb;
standby_exit_hook g_standby_exit_cb;
bool g_standby_exit_flag;
bool g_esd_restart_flag;

bool g_need_auto_connect_timeout;  //��������controller�������ʱ����

restore_breakpoint_t g_ota_restore_data;//OTA��¼��VRAM������

uint32 g_APKsend_pakg_num;
#ifdef __ESD_MODE_
uint8 g_end_autoconnt = 0;
#endif
bool g_need_reset_controller_timeout; //���ڳ�ʱ�������Ӳ���ʱ����controller
bool g_neednot_tts_play_timeout; //���ڳ�ʱ�Ͽ�������TTS
bool g_neednot_tts_play_reset; //��������controller�󲻲���TTS


uint8  g_prev_next_ch_start=0;
int8  prev_next_ch_timer_id=0;
