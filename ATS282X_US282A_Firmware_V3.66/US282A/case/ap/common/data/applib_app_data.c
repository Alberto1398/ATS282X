/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ���ڲ� APPLIB ȫ�ֱ���������ǰ̨Ӧ�ö���Ӧ�ã����ӵ���ͬ�ĵ�ַ����
 *       ��֤COMMON BANK �δ�����һ�µģ���Щȫ�ֱ���ÿ��ǰ̨Ӧ�ü���ʱ�����ʼ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*!
 *  \brief
 *  thread_mutex �̻߳����ź���ָ��
 */
os_event_t *thread_mutex;

/*!
 *  \brief
 *  g_this_app_info ��ǰӦ����Ϣ�ṹ��ָ��
 */
const app_info_t *g_this_app_info;

/*!
 *  \brief
 *  g_app_timer_vector Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t *g_app_timer_vector;

/*!
 *  \brief
 *  g_app_timer_count Ӧ����ʱ����Ŀ��Ϊ 0 ��ʾӦ�ò���Ҫʹ�ö�ʱ��
 */
uint8 g_app_timer_count = 0;
uint8 g_app_timer_id;

/*!
 *  \brief
 *  g_this_app_timer_tag ��ʱ��������־��AP����Ϊ0
 */
uint8 g_this_app_timer_tag;

/*!
 *  \brief
 *  g_this_gui_msg_hook Ӧ��˽����Ϣ������
 */
gui_msg_hook g_this_gui_msg_hook;

/*! 
 *  \brief
 *  g_this_app_msg_hook ������Ϣ���⴦������
 */
app_msg_hook g_this_app_msg_hook;
