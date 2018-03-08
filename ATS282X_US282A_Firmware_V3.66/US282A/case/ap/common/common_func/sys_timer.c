/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE ϵͳ��ʱ������������ʼ�������ٽӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ϵͳ��ʱ��ʱ���� headbar ���ڸ�����ʱ������ǰ̨Ӧ�ý���Ӧ��ʱ���á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ���� config.app��ǰ̨Ӧ���ڽ���ʱ������øýӿڣ����򱳹⣬������ʡ��ػ���
 *      ��ʱ�ػ���״̬�����£��ȵȹ��ܽ��޷�ʹ�á�
 *******************************************************************************/
void sys_timer_init(void)
{
    sys_counter_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL | (APP_TIMER_TAG_SYS << 8), 500, sys_counter_handle);

    peripheral_detect_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL | (APP_TIMER_TAG_SYS << 8), PER_DETECT_PERIOD, \
            peripheral_detect_handle);

    //��ؼ������⴦��
    //com_battery_charge_deal();

    //USB,UHOST,CARD,AUX���
    g_detect_card_force_flag = 1;
    peripheral_detect_handle();
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ɾ��ϵͳ��ʱ��ʱ���� headbar ���ڸ�����ʱ������ǰ̨Ӧ���˳�Ӧ��ʱ���á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  �˳�Ӧ��ʱ����Ҫ���� sys_counter_exit����ɾ�� sys_counter_init ��������ʱ����
 *******************************************************************************/
void sys_timer_exit(void)
{
    kill_app_timer(sys_counter_timer_id);

    kill_app_timer(peripheral_detect_timer_id);
}

/*! \endcond */
