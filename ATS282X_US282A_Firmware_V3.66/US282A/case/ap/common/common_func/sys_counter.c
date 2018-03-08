/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE ϵͳ2Hz��ʱ��HANDLE�������Ŀ���������Ȱβ壬AUX�Ȱβ壬ʡ��ػ���
 *       LED����ܹ��������͵�����APK/APP״̬��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

static sys_status_e get_cur_sys_status(void);

void __section__(".bank") enter_lower_mode(void)
{
    msg_apps_t msg;

    //����ģʽ����Ӧ����S2,S3״̬
    if (g_app_info_state.stub_tools_type == 0)
    {
        //�㲥����͹�����Ϣ
        if (sys_comval->lowpower_mode == LOW_POWER_MODE_S2)
        {
            msg.type = MSG_SYSTEM_ENTER_S2;
        }
        else
        {
            msg.type = MSG_SYSTEM_ENTER_S3;
            act_writel(act_readl(WAKE_PD), WAKE_PD); //��pending
            sys_mdelay(5);
        }
        broadcast_msg(&msg, TRUE);    
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ϵͳ��ʱ��ʱ���������̣�������Ļ���⣬������ʡ��ػ���˯�߹ػ����ص�����
 *    ���ŵȡ�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ϵͳ��ʱ���ܣ��������⣬������ʡ��ػ���˯�߹ػ���app���ߣ���ǰ̨AP���м�ʱ��
 *      ����ǰ̨AP����һ����ʱ����ר��������������ܼ�ʱ���ļ�ʱ���� get_app_msg ��
 *      ���� handle_timers ȥ����⣬һ����ʱ�ѵ���������Ӧ��ʱ���ܡ�
 * \li  ��Ļ������ȫ�� common ģ����ƣ������û����ģ��û�����ͨ�� get_backlight_state
 *      ����ȡ����״̬��
 * \li  ��Ļ����ͬ����ȫ�� common ģ����ƣ������û����ġ�����Ҫ������Ļ����ʱ���ȷ���
 *      �첽��Ϣ MSG_ENTER_SCREEN_SAVE ����ǰAP���� com_message_box �н���������
 * \li  ʡ��ػ���˯�ߣ���ʱ���ػ����ǹ㲥 MSG_POWER_OFF ��Ϣ��manager ap ���յ�����Ϣ
 *      �󣬷��� MSG_APP_QUIT ��ǰ̨�ͺ�̨AP�Լ��˳���
 * \li  �������ڲ��Ž���ͨ����"�������ڲ��ŵ�ģʽ"������Ӧ��ǰ̨AP���߷����첽��Ϣ back_to_playing
 *      ��ǰ̨AP�Լ��ص����ڲ��ų������������ڲ��Ž�����ҪAP�����ڲ��ų���Э�������ڽ��볡��ʱ
 *      change_app_state(APP_STATE_PLAYING)�����˳�����ʱ change_app_state(APP_STATE_NO_PLAY)��
 *******************************************************************************/
void sys_counter_handle(void)
{

    bool usb_cable_adapter = FALSE;
    sys_status_e cur_sys_status;
    bool enter_lp_flag = FALSE;
    uint16 cur_poff_timer; //ʡ��ػ�ʱ��(0.5s��λ)

    if ((act_readl(CHG_DET) & (1 << CHG_DET_UVLO)) != 0) //USB���»��������߲���
    {
        usb_cable_adapter = TRUE;
    }

#if (SUPPORT_OUTER_CHARGE == 1)
    if (sys_comval->bat_charge_mode == BAT_CHARGE_MODE_OUTER)
    {
        if (IS_EXTERN_CAHREGE_IN() == 1)
        {
            usb_cable_adapter = TRUE;
        }
    }
#endif

    //��ؼ������⴦��
    com_battery_charge_deal();

    if ((g_app_last_state.last_ui_status != UI_STATUS_IDLE)
            && (g_app_info_state_all.ui_status == UI_STATUS_IDLE))
    {
        g_sys_counter.ui_idle_counter = 0;
    }

    /* �رձ��⼰ǰ̨���빦���Ż�ģʽ */
    if (usb_cable_adapter == FALSE)
    {
        if ((sys_comval->light_timer > 0) && (get_backlight_state() == BACKLIGHT_STATE_NORMAL))
        {
            g_sys_counter.light_counter++;
            if (g_sys_counter.light_counter >= (2u * sys_comval->light_timer)) //light_timer ��1SΪ��λ
            {
                com_set_display_onoff(FALSE);
            }
        }

        /* ����һ��ʱ��ͽ���Ƶ�� */
        if (g_app_info_state_all.ui_status == UI_STATUS_IDLE)
        {
            g_sys_counter.ui_idle_counter++;
            if (g_sys_counter.ui_idle_counter >= UI_IDLE_TIMER)
            {
                adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);
                g_app_info_state_all.ui_status = UI_STATUS_IDLE_LOWFREQ;
            }
        }
    }

    /* ��ȡ��ǰϵͳ����״̬ */
    cur_sys_status = get_cur_sys_status();

    /* ����ʡ��ػ���ʱ */
    if (((g_app_last_state.last_sys_status != SYS_STATUS_IDLE) && (cur_sys_status == SYS_STATUS_IDLE))
            || ((g_app_last_state.last_sys_status != SYS_STATUS_WAIT_LINK) && (cur_sys_status == SYS_STATUS_WAIT_LINK)))
    {
        g_sys_counter.poweroff_counter = 0;
    }

    /* ʡ��ػ���ʱ������USB����û�в���ʱ��Ч */
    if ((usb_cable_adapter == FALSE) && (cur_sys_status != SYS_STATUS_WORKING))
    {
        if (sys_comval->lowpower_mode != LOW_POWER_MODE_NONE)
        {
            if (sys_comval->lowpower_timer == 0)
            {
                enter_lp_flag = TRUE;
            }
            else
            {
                cur_poff_timer = 120u * sys_comval->lowpower_timer;

                g_sys_counter.poweroff_counter++;
                if (g_sys_counter.poweroff_counter >= cur_poff_timer)
                {
                    enter_lp_flag = TRUE;
                }
            }

            if (enter_lp_flag == TRUE)
            {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                if(g_bt_stack_cur_info.dev_role==NORMAL_DEV)
                {
                    enter_lower_mode();
                }
#else
                enter_lower_mode();                
#endif      

            }
        }
#if 0
        else
        {
            //poweroff_timer ����0��ʡ��ػ�
            if (sys_comval->poweroff_timer > 0)
            {
                if (cur_sys_status == SYS_STATUS_WAIT_LINK)
                {
                    //����ʡ��ʱ�䣬һ�㳤��ʡ��ػ�ʱ�䣬Ĭ��Ϊ5����
                    cur_poff_timer = 120u * sys_comval->poweroff_bt_waitpair_timer;
                }
                else
                {
                    cur_poff_timer = 120u * sys_comval->poweroff_timer;
                }

                g_sys_counter.poweroff_counter++;
                if (g_sys_counter.poweroff_counter >= cur_poff_timer)
                {
                    //�㲥�ػ���Ϣ
                    msg.type = MSG_POWER_OFF;
                    broadcast_msg(&msg, TRUE);
                }
            }
        }
#endif
    }

    /* 3����û��ȡ��APK�����ʾAPK�����̨ģʽ */
    g_sys_counter.apk_counter++;
    if (g_sys_counter.apk_counter >= APK_BACKGROND_OVERTIME)
    {
        set_apk_status(APK_BACKGROUND);
    }

    /* �������״̬ */
    g_app_last_state.last_sys_status = cur_sys_status;
    g_app_last_state.last_ui_status = g_app_info_state_all.ui_status;
}

static sys_status_e get_cur_sys_status(void)
{
    if (g_app_info_vector[APP_TYPE_BTSTACK].used == 1)
    {
        if (g_btmanager_gl_var.btmanager_status != BTMANAGER_STATUS_IDLE)
        {
            return SYS_STATUS_WORKING;
        }
    }

    if ((g_app_info_state_all.engine_state == ENGINE_STATE_PLAYING)
            || (g_app_info_state_all.app_state != APP_STATE_NO_PLAY)
            || (g_app_info_state_all.ui_status != UI_STATUS_IDLE_LOWFREQ))
    {
        return SYS_STATUS_WORKING;
    }

    //��̨������ǰ̨Ӧ�ñ��ֿ���״̬������û��ͨ�ţ�ǰ̨û�в�������7��������
    if (g_app_info_vector[APP_TYPE_BTSTACK].used == 1)
    {
        if ((g_bt_stack_cur_info.conn_status == CONN_STATUS_WAIT_PAIR)
                && ((get_cur_func_id() == APP_FUNC_BTPLAY) || (get_cur_func_id() == APP_FUNC_BTCALL)))
        {
            return SYS_STATUS_WAIT_LINK;
        }
    }

    return SYS_STATUS_IDLE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ⲿ�豸��������,200msִ��һ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 *******************************************************************************/
void peripheral_detect_handle(void)
{
    uint32 detect_mode = 0;
    
#if (SUPPORT_CARD_DETECT != 0)
    if (sys_comval->hard_support_card == 1)
    {
        detect_mode |= PER_DETECT_CARD;
        if (get_card_state() == CARD_STATE_CARD_NULL)
        {
            detect_mode |= PRE_DETECT_CARD_FOR_IN;
        }
        if (g_detect_card_force_flag == 1)
        {
            g_detect_card_force_flag = 0;
            detect_mode |= PER_DETECT_CARD_FORCE;
        }
    }
#endif

    if (sys_comval->support_linein == 1)
    {
        detect_mode |= PER_DETECT_AUX;
        if (get_linein_state() == LINEIN_STATE_LINEIN_NULL)
        {
            detect_mode |= PER_DETECT_AUX_FOR_IN;
        }
    }

#if (SUPPORT_HP_DETECT != 0)
    if (sys_comval->hard_support_headphone == 1)
    {
        detect_mode |= PER_DETECT_HP;
        if (get_headphone_state() == HEADPHONE_STATE_NULL)
        {
            detect_mode |= PER_DETECT_HP_FOR_IN;
        }
    }
#endif

#if ((CASE_BOARD_TYPE != CASE_BOARD_ATS2823) && (CASE_BOARD_TYPE != CASE_BOARD_DVB_ATS2823))
    //ֻ��֧��U�̲��Ź��ܲŶ�̬���usb��u�̲��룻����ֻ���usb����
    if ((sys_comval->hard_support_uhost == 1) && (sys_comval->support_uhostplay == 1))
    {
        detect_mode |= PER_DETECT_USBUHOST_SWITCH;
    }
#endif

    if (detect_mode != 0)
    {
        key_peripheral_detect_handle(detect_mode);
    }
}

//LED����ܹ������ָ�
void __section__(".bank") com_set_display_onoff(bool onoff)
{
    if (onoff == TRUE)
    {
        led_wake_up_display();
        g_app_info_state.backlight_state = BACKLIGHT_STATE_NORMAL;
    }
    else
    {
        led_sleep_display();
        g_app_info_state.backlight_state = BACKLIGHT_STATE_OFF;
    }
}

/*! \endcond */
