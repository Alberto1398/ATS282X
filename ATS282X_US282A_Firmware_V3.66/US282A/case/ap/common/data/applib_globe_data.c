/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������APPLIB ȫ�ֱ���������Ӧ�ã�����ap manager��ǰ̨Ӧ�ã�����Ӧ�ã�BT STACK��
 *       �����ȫ�ֱ��������뱣֤����Ӧ�ö����ӵ���ͬ�ĵ�ַ����Щ��������ap manager
 *       ��ʼ���׶ν��г�ʼ��������ֻ���ʼ��һ�Σ�֮���һֱ��פ�ڴ档
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*!
 *  \brief
 *  g_app_info_vector ȫ��Ӧ����Ϣ�ṹ������
 */
app_info_t __section__(".applib_globe_data.g_app_info_vector") g_app_info_vector[MAX_APP_COUNT];

/*!
 *  \brief
 *  config_fp ���ýű��ļ����
 */
sd_handle __section__(".applib_globe_data.config_fp") config_fp;

/*!
 *  \brief
 *  g_app_info_state_all ȫ��ϵͳ��ǰ״̬�ṹ�壬�����ϵͳȫ�ֿռ�
 *  NOTE!!! ����ȷ�� app_info_state_all_t ǰ�沿�ֳ�Ա����
 */
app_info_state_all_t __section__(".applib_globe_data.g_app_info_state_all") g_app_info_state_all;

uint32 __section__(".applib_globe_data.g_app_info_state_all") g_customer_state;//Ԥ���ͻ�ȫ��������
bool __section__(".applib_globe_data.g_app_info_state_all")g_neednot_tts_play_timeout1;//���LT_ADDR


