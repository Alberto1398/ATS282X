/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_poweron_option.c
 * \brief    ����ѡ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"
#include  "stub_interface.h"

extern app_result_e com_sys_deal_uhost_in(void *ev_param);
extern app_result_e com_sys_deal_sdcard_in(void *ev_param);
extern app_result_e com_sys_deal_linein_in(void *ev_param);
extern app_result_e com_sys_deal_rtc_alarm(void *ev_param);

//���ϵͳ״̬
static app_result_e _power_on_sys_option(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t private_msg;
    bool check_uhost_flag = FALSE;

    check_msg_again: while (get_app_msg(&private_msg) == TRUE)
    {
        PRINT_INFO_INT("poweron msg:", private_msg.msg.type);

        if (private_msg.msg.type == MSG_USB_STICK)
        {
            usb_in_flag = TRUE;
            continue;
        }

        if (private_msg.msg.type == MSG_USB_UNSTICK)
        {
            usb_in_flag = FALSE;
            continue;
        }

        if (private_msg.msg.type == MSG_UH_IN)
        {
            uhost_in_flag = TRUE;
            continue;
        }

        if (private_msg.msg.type == MSG_UH_OUT)
        {
            uhost_in_flag = FALSE;
            continue;
        }

        if (private_msg.msg.type == MSG_ADAPTOR_IN)
        {
            adaptor_in_flag = TRUE;
            continue;
        }

        if (private_msg.msg.type == MSG_ADAPTOR_OUT)
        {
            adaptor_in_flag = FALSE;
            continue;
        }

        if (private_msg.msg.type == MSG_SD_IN)
        {
            sd_in_flag = TRUE;
            continue;
        }

        if (private_msg.msg.type == MSG_SD_OUT)
        {
            sd_in_flag = FALSE;
            continue;
        }

        if (private_msg.msg.type == MSG_LINEIN_IN)
        {
            linein_in_flag = TRUE;
            continue;
        }

        if (private_msg.msg.type == MSG_LINEIN_OUT)
        {
            linein_in_flag = FALSE;
            continue;
        }

        if (private_msg.msg.type == MSG_RTCALARM)
        {
            rtcalarm_flag = TRUE;
            continue;
        }

        if (private_msg.msg.type == MSG_LOW_POWER)
        {
            if (g_app_info_state.bat_value == 1)
            {
                need_lowpower_tts_flag = TRUE;
            }
            else
            {
                result = RESULT_LOW_POWER;
                break;
            }
        }
    }

    if ((usb_in_flag == FALSE) && (adaptor_in_flag == FALSE) && (check_uhost_flag == FALSE))
    {
        if ((g_comval.hard_support_uhost == 1) && (g_comval.support_uhostplay == 1))
        {
            //������⵽uhost����
            key_peripheral_detect_handle(PER_DETECT_UHOST_FORCE);
            if(g_uhost_delay != 0)
            {
                sys_os_time_dly(2);     //�ȴ�U�̵�ƽ�ȶ�
            }
            check_uhost_flag = TRUE;
            goto check_msg_again;
        }
    }

    //for clr card or uhost insert msg
    g_ap_switch_var.card_in_out_flag = 0;
    g_ap_switch_var.uhost_in_out_flag = 0;

    return result;
}

//��ⰴ��״̬
static app_result_e _power_on_key_option(void)
{
    input_gui_msg_t gui_msg;
    app_result_e result = RESULT_NULL;
    key_value_e poweron_key_val = KEY_NULL;

    //��⿪������
    while (get_gui_msg(&gui_msg) == TRUE)
    {
        poweron_key_val = gui_msg.data.kmsg.val;
        com_filter_key_hold();
    }

    //����ǲ������ط���������Ҫ����������������������ڿ��������������²����ػ���λ�Ĵ������� RESULT_POWER_OFF

    return result;
}

app_result_e check_ota_upgrade(void)
{
    otaval_t g_otaval;
    sys_vm_read(&g_otaval, VM_OTA_UPGRADE, sizeof(otaval_t));
    if ( (g_otaval.magic == VRAM_MAGIC(VM_OTA_UPGRADE))
        && (g_otaval.ota_upgrade_flag == OTA_UPGRADE_ENABLE) )
    {
        return RESULT_ENTER_OTA_UPGRADE;
    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ����ѡ�����ϵͳ״̬�Ͱ�������ѡ��
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_poweron_option(void)
{
    app_result_e result = RESULT_NULL;

    // ����Ƿ����OTA����
    result = check_ota_upgrade();
    if (result != RESULT_NULL)
    {
        goto power_on_option_ok;
    }

    //����Ƿ�����Զ����Ի��Զ�����
    result = _power_on_sys_option();
    if (result != RESULT_NULL)
    {
        goto power_on_option_ok;
        //�͵�
    }

#if ((CASE_BOARD_TYPE != CASE_BOARD_ATS2823) && (CASE_BOARD_TYPE != CASE_BOARD_DVB_ATS2823))
    //���ȼ�1���Զ����Ի��Զ�����
    if ((sd_in_flag == TRUE) || (uhost_in_flag == TRUE))
#endif
    {
        //�ȴ�PA��ʼ���꣬�����̳߳�ͻ����������
        config_wait_pa_ok();
        result = check_enter_test_mode();
        if (result != RESULT_NULL)
        {
            //if (result != RESULT_ENTER_UPGRADE)
            //{
            //    result = RESULT_APP_QUIT;
            //}

            goto power_on_option_ok;
        }
    }

    //���ȼ�2�����ӿ���
    if (rtcalarm_flag == TRUE)
    {
        g_ap_switch_var.rtcalarm_poweron = TRUE;
        result = com_sys_deal_rtc_alarm(NULL);
        if (result != RESULT_NULL)
        {
            goto power_on_option_ok;
            //����������Ч�ָ��ػ�
        }
    }

    //���ȼ�3���������عػ�
    //NOTE������ͨ�� get_gui_msg ����ȡ������Ϣ�����뱣֤����������װ���������г��� 60MS
    result = _power_on_key_option();
    if (result != RESULT_NULL)
    {
        goto power_on_option_ok;
    }

    //���ȼ�4��U�̲��� > ���̲��� > AUX���� > ������[��USB����] > ���
#if 0
    if (uhost_in_flag == TRUE)
    {
        result = com_sys_deal_uhost_in(NULL);
        if (result != RESULT_NULL)
        {
            goto power_on_option_ok;
        }
    }
#endif
#if 0
    if (sd_in_flag == TRUE)
    {
        result = com_sys_deal_sdcard_in(NULL);
        if (result != RESULT_NULL)
        {
            goto power_on_option_ok;
        }
    }
#endif
#if 0
    if (linein_in_flag == TRUE)
    {
        result = com_sys_deal_linein_in(NULL);
        if (result != RESULT_NULL)
        {
            goto power_on_option_ok;
        }
    }
#endif
    if (usb_in_flag == TRUE)
    {
        if ((g_comval.hard_support_card == 1) && (g_comval.support_usbmsc == 1))
        {
            result = RESULT_USB_TRANS;
            goto power_on_option_ok;
        }

        //if (g_comval.support_usbbox == 1)
        //{
        //    result = RESULT_USB_SOUND;
        //    goto power_on_option_ok;
        //}
    }
    if ((usb_in_flag == TRUE) || (adaptor_in_flag == TRUE))
    {
        result = RESULT_CONFIG_CHARGE;
        goto power_on_option_ok;
    }

    power_on_option_ok:

    if(g_app_info_state.stub_phy_type == STUB_PHY_UART)
    {
        if (stub_get_connect_state(0) > 0)
        {
            config_wait_pa_ok(); 
#if ((CASE_BOARD_TYPE == CASE_BOARD_ATS2823) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823))            
            //��⵽���̣�ȷ���Ƿ�������ģʽ��������
            if (config_detect_card() != -1)
#else
            if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
#endif
            {        
                result = bluetooth_parse_cfg_sub(DISK_H);

                if(result == RESULT_NULL)
                {
                    result = open_stub();    
                }            
            }
            else
            {
                result = open_stub();
            }
        }
    }
    else
    {
        if ((usb_in_flag == TRUE) && (g_comval.stub_enable == TRUE))
        {
            result = open_stub();
        }
    }    
    
    if(g_app_info_state.stub_tools_type != 0)
    {
        g_config_esd_restart = FALSE;        
    }

    return result;
}
