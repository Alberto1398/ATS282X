/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������ǰ̨Ӧ�ù����ȫ�ֱ��������뱣֤����ǰ̨Ӧ�ö����ӵ���ͬ�ĵ�ַ����Щ
 *       ��������ap manager ��ʼ���׶ν��г�ʼ��������ֻ���ʼ��һ�Σ�֮���һֱ
 *       ��פ�ڴ档
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*!
 *  \brief
 *  g_app_info_state ȫ��ϵͳ��ǰ״̬�ṹ�壬Ҳ���� MANAGER Ӧ���з��ʲ��ֱ���
 */
app_info_state_t __section__(".g_app_info_state") g_app_info_state;

/*!
 *  \brief
 *  g_app_last_state ȫ��ϵͳ���״̬�ṹ��
 */
app_last_state_t __section__(".gl_com_data") g_app_last_state;

/*!
 *  \brief
 *  g_sys_counter ϵͳ��ʱ���ṹ��
 */
sys_counter_t __section__(".gl_com_data") g_sys_counter;

/*!
 *  \brief
 *  g_buffer_gui_msg ����gui��Ϣ
 */
input_gui_msg_t __section__(".gl_com_data") g_buffer_gui_msg;

/*!
 *  \brief
 *  g_buffer_app_msg ����ϵͳ��Ϣ
 */
private_msg_t __section__(".gl_com_data") g_buffer_app_msg;

/*!
 *  \brief
 *  g_com_app_timer_vct COMMON ȫ����ʱ������
 */
app_timer_t __section__(".gl_com_data") g_com_app_timer_vct[COM_APP_TIMER_MAX];

/*!
 *  \brief
 *  g_discrete_led_info COMMON ����LED������
 */
discrete_led_info_t __section__(".gl_com_data") g_discrete_led_info[LED_ID_MAX];
bool __section__(".gl_com_data") g_discrete_led_a23_available;

/*! ǰ̨Ӧ���л����ȫ�ֱ��� */
ap_switch_var_t __section__(".gl_com_data") g_ap_switch_var;


OTA_malloc_param_t __section__(".gl_com_data") *g_OTA_var;
bool __section__(".gl_com_data")g_rcp_ctrl_quit_flag;

//ESD VRAM����
g_config_var_t __section__(".gl_com_data")g_config_var;

//���������̷�֧��־
bool __section__(".gl_com_data")g_esd_cardreader_flag;

uint8 __section__(".gl_rcp_buffer") *g_rcp_default_long_buffer;

