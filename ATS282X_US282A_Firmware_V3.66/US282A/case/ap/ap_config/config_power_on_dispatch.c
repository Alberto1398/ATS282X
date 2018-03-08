#include "ap_config.h"
#include "uhost.h"

bool need_lowpower_tts_flag = FALSE;
bool g_config_volume_init = FALSE; //ȷ��DACֻ��ʼ��1��

void config_wait_pa_ok(void)
{
    while (g_app_info_state.inner_pa_inited == FALSE)
    { //�ȴ� IC PA ��ʼ�����
        sys_os_time_dly(1);
    }
    if (g_config_volume_init == FALSE) //ȷ��DACֻ��ʼ��1��
    {
        com_reset_sound_volume(0);
        set_dac_rate(48, DAF0_EN); //DAC�����ʳ�ʼ������Ϊ48K
        g_config_volume_init = TRUE;
    }
}

//��������̨�����Ĺ���
void deal_bt_inback(app_result_e result)
{
    if (result == RESULT_ESD_RESTART)
    {
        return;
    }
    
    if (g_comval.support_bt_inback == 1)
    {
        com_btmanager_init(FALSE);
    }
    else
    {
        if (g_app_info_state.bt_power_on == TRUE)
        {
            uint8 tmp_next_func_id;

            tmp_next_func_id = com_ap_switch_ask_next_func(result);
            if ((tmp_next_func_id != APP_FUNC_BTPLAY) && (tmp_next_func_id != APP_FUNC_BTCALL))
            {
                PRINT_INFO("BT_PW_OFF");
                sys_drv_install(DRV_GROUP_BT, 0, "bt_ctrl.drv");
                bt_drv_controller_poweroff();
                sys_drv_uninstall(DRV_GROUP_BT);
                g_app_info_state.patch_download_flag = FALSE;
                g_app_info_state.bt_power_on = FALSE;
            }
        }
    }
}

bool app_based_on_btstack(uint8 app_id)
{
    if((app_id == APP_ID_BTPLAY) || (app_id == APP_ID_BTCALL))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 *  ���˿�������Ϣ��ͬʱ����Ϣ���д���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void config_power_on_dispatch(app_result_e result)
{
    bool boot_by_rtcalarm = FALSE;

    if (result == RESULT_SYSTEM_EXIT_S3)
    {//�˳����͹���ģʽS3����Ҫ����TTS��ֱ�Ӽ���BTSTACK
        config_wait_pa_ok();

        //����ap
        com_ap_switch_deal(result);

        return;
    }

    if (result == RESULT_ENTER_UPGRADE)
    {//�̼�����
        config_wait_pa_ok();

        //����ap
        com_ap_switch_deal(result);

        return ;
    }
	
    if (result == RESULT_ENTER_OTA_UPGRADE)
    {//�̼�����
        config_wait_pa_ok();

        //����ap
        com_ap_switch_deal(result);

        return ;
    }

    //������������
    result_dispatch_again: if (result == RESULT_CONFIG_CHARGE)
    {
        result = config_charging(SWITCH_ON);

        //����ȴ�KEYTONE���Ž���������KEYTONE���ܻ���ΪTTS���ŵ�Ƶ�����
        keytone_play_deal_wait();
    }

    if ((result == RESULT_POWER_OFF) || (result == RESULT_LOW_POWER) || (result == RESULT_ERROR))
    {
        if (result == RESULT_LOW_POWER)
        {
            config_wait_pa_ok();
            //��������
            com_tts_state_play(TTS_MODE_ONLYONE, (void *) TTS_BATTERY_LOW);
            sys_os_time_dly(50);
        }
        
        g_config_var.esd_flag = FALSE;          //���ESD���
        sys_vm_write(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var));

        result = sys_power_off(FALSE);
        goto result_dispatch_again;
    }
    else
    {
#if (SUPPORT_OUTER_CHARGE == 1)
        if (sys_comval->bat_charge_mode == BAT_CHARGE_MODE_OUTER)
        {
            if (result == RESULT_NULL)
            {
                if ((IS_EXTERN_BATTERY_IN () == 0) && (IS_EXTERN_CAHREGE_IN() == 1))
                {
                    result = config_idle_mode_loop(); //�������ģʽ
                    goto result_dispatch_again;
                }
            }
        }
#endif

        if (result == RESULT_ENTER_ALARM)
        {
            boot_by_rtcalarm = TRUE;
        }

        if ((result != RESULT_USB_TRANS) && (result != RESULT_ALARM_RING_FOLDER_SCAN)) //���ģʽ�²忨����ɨ������Ŀ¼
        {
#if (SUPPORT_RING_FOLDER == 1)
            if ((result != RESULT_ESD_RESTART) && (com_get_config_default(SETTING_APP_SUPPORT_CARDPLAY) != 0))
            {
                if ((int8) sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
                {
                    g_ap_switch_var.result_alarm_ring_scan_backup = result;
                    result = RESULT_ALARM_RING_FOLDER_SCAN;
                }
            }
#endif
        }

        if (boot_by_rtcalarm == FALSE)//���ӿ�������Ҫ����TTS��Ҳ����Ҫ����BTSTACK
        {
            //��һ��Ӧ����bt��أ��ȳ�ʼ����������ΪttsҪ��pa
            bool app_by_bt = app_based_on_btstack(com_ap_switch_ask_next_func(result));
            if(app_by_bt == TRUE)//��һ��Ӧ����bt���
            {
                if(result != RESULT_USB_TRANS)
                {
                    deal_bt_inback(result);
                }
            }
            if (result != RESULT_ESD_RESTART)
            {
                config_wait_pa_ok();//��pa����
                //ESD RESTART����Ҫ����TTSs
                com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_NOBLOCK | TTS_MODE_SYS_NORECV, (void *) TTS_POWERON);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                g_app_info_state_all.power_on_flag=1;
#endif                    
                if (need_lowpower_tts_flag == TRUE)
                {
                    //�ȴ�TTS����
                    com_tts_state_play_wait();
                    //�����ͣ�����
                    com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_NOBLOCK, (void *) TTS_BAT_LOW_CHARGE);
                }
            }
            if(app_by_bt == FALSE)//��һ��Ӧ�ò���bt���
            {
                if(result != RESULT_USB_TRANS)
                {
                    deal_bt_inback(result);
                }
            }
            if (result != RESULT_ESD_RESTART)
            {
                //�ȴ�TTS����
                com_tts_state_play_wait();
                //����а������£���ôӦ�÷���������
                {
                    input_gui_msg_t input_msg;
                    get_gui_msg(&input_msg);
                }
            }
        }
        config_wait_pa_ok();//ȷ��pa����
        
        //����ap
        com_ap_switch_deal(result);

        if(g_app_info_state.stub_tools_type == 0)
        {
            g_config_var.esd_flag = TRUE;
        
            sys_vm_write(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var));
        }
    }
}

