/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP COMMON �����RCP ģ���ʼ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/*RCP ״̬RESET*/
void __section__(".bank_2") com_rcp_state_init(void)
{
    g_rcp_next_seq = -1;
    g_rcp_seg_flag = FALSE;
    g_rcp_long_buffer_flag = FALSE;
    g_rcp_long_length = 0;
    g_rcp_long_index = 0;
}

/*RCP ģ���ʼ��������BUFFER�ռ䣻��ʼ��ȫ�ֱ��� */
void __section__(".bank_2") com_rcp_init(void)
{
    int i;
    g_rcp_long_buffer = g_rcp_default_long_buffer;
    g_rcp_pkg_max_len = RCP_LONG_COMMAND_BUFFER_LEN;

    com_rcp_state_init();

    g_app_rcp_cmd_cb_tbl = NULL;
    g_app_get_global_info = NULL;
    //clr
    for (i=0; i<RCP_SERVICE_MAX; i++)
    {
        g_app_ext_serv_cb[i] = NULL;
    }
    //for common ancs use
    if(com_get_config_default(BTMANAGER_ENABLE_ANCS)!=0)
    {
        com_ancs_init();
    }
    //for common tip use
    if(com_get_config_default(BTMANAGER_ENABLE_TIP)!=0)
    {
        com_tip_init();
    }

}

/*Ӧ������RCP�ص���������ȡȫ��״̬�ص��������봫�ݽ���*/
void __section__(".bank_2") com_rcp_set_callback(const rcp_cmd_cb_t * app_rcp_cmd_cb_tbl,
        rcp_get_func app_get_global_info_cb)
{
    g_app_rcp_cmd_cb_tbl = app_rcp_cmd_cb_tbl;
    g_app_get_global_info = app_get_global_info_cb;
}
///*Ӧ������RCP�ص���������չЭ�鲿��*/
void __section__(".bank_2") com_rcp_set_ext_cb(rcp_ext_serv_func* app_rcp_cb, uint32 server_id)
{
    if (server_id < RCP_SERVICE_MAX)
    {
        g_app_ext_serv_cb[server_id] = app_rcp_cb;
    }
}

/*Ӧ�����RCP�ص���������չЭ�鲿��*/
void __section__(".bank_2") com_rcp_clr_ext_cb(void)
{
    int i;
    for (i=0; i<RCP_SERVICE_MAX; i++)
    {
        g_app_ext_serv_cb[i] = NULL;
    }
}

uint8 bat_val_tap[5]={6,4,2,1,0};//


/*��ȡ������Ϣ*/
void __section__(".rcode") com_get_com_info(com_rcp_info_t *p_com_rcp_info)
{
    uint8 i;
#if (SUPPORT_RCP_FUNC == 1)

    uint8 cur_func_id = get_cur_func_id();

#if (SUPPORT_SLAVE_CONTROL == 1)
    p_com_rcp_info->reserve_bytes[0] = 0x5a;
#else
    if ((cur_func_id == APP_FUNC_LINREC_TOC) || (cur_func_id == APP_FUNC_FMREC_TOC))
    {
        cur_func_id = APP_FUNC_MICREC_TOC;
    }
    if ((cur_func_id == APP_FUNC_LINREC_TOU) || (cur_func_id == APP_FUNC_FMREC_TOU))
    {
        cur_func_id = APP_FUNC_MICREC_TOU;
    }
    if (g_ap_switch_var.apk_alarm_flag == 1)
    {
        cur_func_id = APP_FUNC_CLOCK;
    }
    p_com_rcp_info->app_type = cur_func_id;
    p_com_rcp_info->max_volume = VOLUME_VALUE_MAX;
    p_com_rcp_info->min_volume = 0;
    p_com_rcp_info->cur_volume = sys_comval->volume_current;
    p_com_rcp_info->cur_eqtype = sys_comval->eq_type;
    p_com_rcp_info->mute_flag = get_mute_enable();
    p_com_rcp_info->battery_state = get_charge_state();
    p_com_rcp_info->battery_value = get_bat_value();
    for(i = 0; i < 5; i++)
    {
        if(p_com_rcp_info->battery_value >= bat_val_tap[i])
        {
            p_com_rcp_info->battery_value = 5 - i;
            break;
        }
    }
    p_com_rcp_info->sdcard_in = get_card_state();
    p_com_rcp_info->uhost_in = get_uhost_state();
    p_com_rcp_info->usb_in = get_cable_state();
    p_com_rcp_info->linein_in = get_linein_state();
    p_com_rcp_info->hp_in = get_headphone_state();
    p_com_rcp_info->config_flag = FALSE;/*������ã���Ӧ�õ���com_get_com_info֮�����и�ֵ*/
    p_com_rcp_info->alarm_clock = g_app_info_state.ring_alarm;
    p_com_rcp_info->app_argv = 0;
    p_com_rcp_info->dialog_type = g_dialog_type;
    g_dialog_type = DIALOG_TYPE_NONE; //g_dialog_typeֻ�ܶ�һ��
    p_com_rcp_info->dialog_button_type = g_dialog_button_type;
    p_com_rcp_info->dialog_desc_id = g_dialog_desc_id;
    p_com_rcp_info->dialog_active_default = g_dialog_active_default;
    if (sys_comval->dae_cfg.bypass == 1)
    {
        p_com_rcp_info->dae_mode = DAE_MODE_NONE;
    }
    else
    {
        p_com_rcp_info->dae_mode = DAE_MODE_DAE;
    }
    p_com_rcp_info->dae_option = (sys_comval->dae_cfg.vbass_enable << DAE_OPTION_VBASS)
    | (sys_comval->dae_cfg.treble_enhance_enable << DAE_OPTION_TREBLE)
    | (sys_comval->dae_cfg.vsurround_enable << DAE_OPTION_VSURROUND)
    | (sys_comval->dae_cfg.spk_compensation_enable << DAE_OPTION_SFR)
    | (sys_comval->dae_cfg.peq_enable << DAE_OPTION_PEQ)
    | (sys_comval->dae_cfg.mdrc_enable << DAE_OPTION_MDRC)
    | (sys_comval->dae_enhanced_enable << DAE_OPTION_DAE_ENH)
    | (sys_comval->dae_weaken_enable << DAE_OPTION_DAE_WKN);
#endif

#endif
}

/*��ѯȫ��״̬��Ϣ����Ĭ��Ӧ��*/
app_result_e __section__(".rcode") com_get_global_rcp_info(uint32 data1, uint32 data2)
{
    uint8 tmp_com_rcp_info[sizeof(com_rcp_info_t) + RCP_COMMAND_HEAD_SIZE];
    com_rcp_info_t *p_com_rcp_info = (com_rcp_info_t *) (tmp_com_rcp_info + RCP_COMMAND_HEAD_SIZE);

    //��ȡ������Ϣ
    com_get_com_info(p_com_rcp_info);
#if     0 // modi
    if (g_this_app_info->app_id == APP_ID_BTCALL)
    {
        libc_print("APPState:",tmp_com_rcp_info[16],2);
        if (g_btmanager_gl_var.last_hfp_status[0] == HFP_STATUS_INCOMING)
        {
            /*
             if (sys_share_query_read(APP_ID_BTSTACK, &g_bt_stack_cur_info) == -1)
             {
             PRINT_ERR("btstack share query not exist!!");
             }

             if(g_bt_stack_cur_info.rmt_dev[0].hfp_status != HFP_STATUS_INCOMING)
             */
            {
                tmp_com_rcp_info[16]=0; // pb tts�У��ֻ��ܽ����磬��ѯ״̬�����ܼ�ʱ���£�
                // ����Ԥȡ��ǰ��:apk��̨ʱ������ѯ�������θķ�,������
            }
        }
    }
#endif

    com_rcp_send_pkg(0, 0, tmp_com_rcp_info, sizeof(com_rcp_info_t), sizeof(com_rcp_info_t));

    return RESULT_NULL;
}

/*��ȡ֧�����Ի���*/
app_result_e __section__(".bank_2") com_get_support_feature(uint32 data1, uint32 data2)
{
    uint32 support_feature_low = 0;
    uint32 support_feature_high = 0;

    if (1 == sys_comval->dae_cfg.enable_2823T)
    {
        support_feature_low = SUPPORT_FEATURE_A2DP_PLAY
        | SUPPORT_FEATURE_SDCARD_PLAY
        | SUPPORT_FEATURE_UHOST_PLAY
        | SUPPORT_FEATURE_LINEIN_PLAY;
    }
    else
    {
        support_feature_low = SUPPORT_FEATURE_A2DP_PLAY
        | SUPPORT_FEATURE_SDCARD_PLAY
        | SUPPORT_FEATURE_UHOST_PLAY
        | SUPPORT_FEATURE_LINEIN_PLAY
        | SUPPORT_FEATURE_ALARM_CLOCK
        | SUPPORT_FEATURE_ALARM_BUILDIN_RING;
    }


    if (sys_comval->support_fmradio == 1)
    {
        support_feature_low |= SUPPORT_FEATURE_FMRADIO;
    }

    if (sys_comval->support_usbbox == 1)
    {
        support_feature_low |= SUPPORT_FEATURE_USBBOX_PLAY;
    }
    if (sys_comval->support_ota == 1)
    {
        support_feature_low |= SUPPORT_FEATURE_OTA;
    }

#if (SUPPORT_RECORD_PLAY == 1)
    support_feature_low |= SUPPORT_FEATURE_RECORD_PLAY;
#endif

#if (SUPPORT_MUSIC_FOLDER == 1)
    support_feature_low |= SUPPORT_FEATURE_FOLDER_PLAY;
#endif

#if (SUPPORT_RECORD == 1)
    support_feature_low |= SUPPORT_FEATURE_RECORD;
#endif

#if (SUPPORT_RING_FOLDER == 1)
    support_feature_low |= SUPPORT_FEATURE_ALARM_RING_FOLDER;
#endif

#if (SUPPORT_PLIST_FILE == 1)
    support_feature_low |= SUPPORT_FEATURE_PLIST_FILE;
#endif
#if (SUPPORT_PLIST_TITLE == 1)
    support_feature_low |= SUPPORT_FEATURE_PLIST_TITLE;
#endif
#if (SUPPORT_PLIST_ARTIST == 1)
    support_feature_low |= SUPPORT_FEATURE_PLIST_ARTIST;
#endif
#if (SUPPORT_PLIST_ALBUM == 1)
    support_feature_low |= SUPPORT_FEATURE_PLIST_ALBUM;
#endif
#if (SUPPORT_PLIST_GENRE == 1)
    support_feature_low |= SUPPORT_FEATURE_PLIST_GENRE;
#endif

    com_rcp_send_pkg(support_feature_low, support_feature_high, NULL, 0, 0);

    return RESULT_NULL;
}

/*��ȡ�豸��Ϣ���������Ƶ�*/
app_result_e __section__(".bank_2") com_get_bt_device_info(uint32 data1, uint32 data2)
{
    uint8 tmp_bt_device_info[sizeof(bt_device_info_t) + RCP_COMMAND_HEAD_SIZE];
    msg_apps_t rcp_msg;

    rcp_msg.type = MSG_BTSTACK_GET_DEVICE_INFO_SYNC;
    rcp_msg.content.addr = tmp_bt_device_info + RCP_COMMAND_HEAD_SIZE;
    send_sync_msg_btmanager(NULL, &rcp_msg, NULL, 0);

    com_rcp_send_pkg(0, 0, tmp_bt_device_info, sizeof(bt_device_info_t), sizeof(bt_device_info_t));

    return RESULT_NULL;
}

/*�����������ϵͳ����*/
app_result_e com_set_system_setting(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    uint8 setting_id = (uint8) data1;

    switch (setting_id)
    {
        case SETTING_ID_SS_DATETIME:/*����ϵͳ����ʱ��*/
        {
            system_datetime_t *p_system_dt = (system_datetime_t *) data;

            sys_set_date(&(p_system_dt->date));
            sys_set_time(&(p_system_dt->time));
        }
        break;

        case SETTING_ID_SS_VOLUME:/*��������*/
        {
            uint8 volume = (uint8) data2;

            if (get_mute_enable() == TRUE)
            {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                com_switch_mute(1,0,NULL);
#else
                change_mute_enable(FALSE);
#endif

                com_reset_sound_volume(0);
            }
            //com_start_key_tone(KEYTONE_NOBLOCK);

            if(g_app_info_vector[1].app_id == 5)
            {
                msg_apps_t msg;
                //������Ϣ���ݴ�ű���
                msg_reply_t reply;
                
                //�����������ĵ�ַ
                msg.content.addr = &data2;
                
                //��Ż�ȡ�����ĵ�ַ
                reply.content = NULL;
                
                //��Ϣ����(����Ϣ����)
                msg.type = MSG_UENGINE_VOL_SEND_SYNC;
                
                //����ͬ����Ϣ
                send_sync_msg(APP_ID_UENGINE, &msg, &reply, 0);
            }
            else
            {
                com_set_sound_volume(volume, SET_VOLUME_SYNC_TO_REMOTE);
            }
        }
        break;

        case SETTING_ID_SS_EQ:/*EQģʽ*/
        {
            com_eqtype_e eq_type = (com_eqtype_e) data2;

            if (eq_type > EQ_USR_DEF) //��ֹ�������EQ����
            {
                eq_type = EQ_NOR;
            }
            com_set_sound_eq(SETEQ_TYPE, eq_type, NULL);
        }
        break;

        case SETTING_ID_SS_USREQ:/*�û�EQ*/
        {
            uint8 *p_eq_param = (uint8 *) data;

            com_set_sound_eq(SETEQ_TYPE, EQ_USR_DEF, p_eq_param);
        }
        break;

        case SETTING_ID_SS_DAEMODE:/*DAEģʽ*/
        {
            daemode_param_t *p_daemode_param = (daemode_param_t *) data;

            com_set_dae_config(p_daemode_param);

            //����DAE��������Ҫ������������
            com_reset_sound_volume(0);
        }
        break;

        default:
        break;
    }

    return RESULT_NULL;
}

/*�����������APK״̬*/
app_result_e com_set_system_status(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    set_apk_status((apk_status_e) data1);

    return RESULT_NULL;
}

/*������������Ի�����ʾ*/
app_result_e com_set_system_dialog(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    uint8 control_id = (uint8) data1;

    switch (control_id)
    {
        case SETTING_ID_SD_ANSWER:
        {
            //�ȴ�APK��Ӧ����
            if (g_dialog_state == DIALOG_STATE_LAUNCH)
            {
                g_dialog_overtime = (uint8) data2;
                g_dialog_state = DIALOG_STATE_CHOOSING;
            }
        }
        break;

        case SETTING_ID_SD_CONFIRM:
        {
            //�ȴ�APK�Ի���ѡ��
            if (g_dialog_state == DIALOG_STATE_CHOOSING)
            {
                g_dialog_state = DIALOG_STATE_CONFIRM;
                g_dialog_choosed = (uint8) data2;
            }
        }
        break;

        default:
        break;
    }

    return RESULT_NULL;
}

/*������������豸��Ϣ*/
app_result_e __section__(".bank_2") com_set_device_name(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    msg_apps_t rcp_msg;

    rcp_msg.type = MSG_BTSTACK_SET_DEVICE_NAME_SYNC;
    rcp_msg.content.addr = (bt_device_name_t *) data;
    send_sync_msg_btmanager(NULL, &rcp_msg, NULL, 0);

    return RESULT_NULL;
}

/*����������������л�*/
app_result_e com_ctrl_switch_mute(uint32 data1, uint32 data2)
{
    if (g_ignore_switch_mute == FALSE)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        com_switch_mute(1,0,NULL);
#else
        com_switch_mute();
#endif 
    }

    return RESULT_NULL;
}

/*! \endcond */
