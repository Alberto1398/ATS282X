/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������ⲿ��緽���Ŀ���ģʽ������ģʽ�����˲���USB�ϵ���������ģʽ���Լ�����
 *       ΪON״̬ʱ����������ģʽ���ģʽ���ڸ�ģʽ�г��˼�⵽���ز���ON״̬�л���
 *       ��������ģʽ֮�⣬����Ӧ�κ������¼���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  �ⲿ��緽���Ŀ���ģʽ�Ĵ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_idle_mode_loop(void)
{
    app_result_e result = RESULT_NULL;
    cable_state_e cur_cable_state, last_cable_state;

    //�������ģʽ
    set_cur_func_id(APP_FUNC_IDLE_MODE);

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    if ((g_discrete_led_a23_available == TRUE) && (g_app_info_state.charge_state == BAT_CHARGING))
    {
        discrete_led_set(LED_ID_POWER, LED_ALWAYS_OFF, NULL);
        discrete_led_set(LED_ID_POWER, LED_ALWAYS_ON, NULL);
    }
#endif

    last_cable_state = get_cable_state();

    while (1)
    {
        com_view_loop();

        //��⵽Ӳ���ز���ON״̬�����빤��״̬
        if (IS_EXTERN_BATTERY_IN() == 1)
        {
            result = RESULT_NULL;
            break;
        }

        //��⵽USB�߲��룬���������ģʽ
        cur_cable_state = get_cable_state();
        if ((last_cable_state == CABLE_STATE_CABLE_NULL) && (cur_cable_state == CABLE_STATE_CABLE_IN))
        {
            result = RESULT_USB_TRANS;
            break;
        }
        last_cable_state = cur_cable_state;

        //����10ms�����������
        sys_os_time_dly(1);
    }

    return result;
}

