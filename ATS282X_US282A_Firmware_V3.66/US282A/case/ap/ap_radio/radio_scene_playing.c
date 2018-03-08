#include "app_radio.h"

void radio_engine_kill(uint8 eg_id)
{
    msg_apps_t msg;
    //ע��music engine
    msg.type = MSG_KILL_APP_SYNC;
    msg.content.data[0] = eg_id;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}

bool radio_engine_create(uint8 eg_id)
{
    //��Ϣ����
    msg_reply_t msg_reply;
    msg_apps_t msg;
    //����radio ��̨�������
    msg.type = MSG_CREAT_APP_SYNC;
    msg.content.data[0] = eg_id;
    //����ͬ����Ϣ
    return (send_sync_msg(APP_ID_MANAGER, &msg, &msg_reply, 0) == ERR_NO_ERR);

}

//�����ʼ��
bool radio_engine_init(void)
{
    engine_type_e engine_type;
    bool result = TRUE;
    bool need_init = FALSE;

    //��ȡ��ǰ��̨��������
    engine_type = get_engine_type();

    //��ǰδ��װ���棬�谲װ
    if (engine_type != ENGINE_RADIO)
    {
        if (engine_type != ENGINE_NULL)
        {
            radio_engine_kill(APP_ID_THEENGINE);
        }

        //����radio ��̨�������
        //ʵ��ִֻ����fmdrv_init(���ṩ��FMģ���ʱ��)
        result = radio_engine_create(APP_ID_FMENGINE);
        if (result == TRUE)
        {
            //���ʼ��ģ��
            need_init = TRUE;
        }
    }

    if (need_init == TRUE)
    {
        radio_set_mute(SetMUTE);
        result = radio_modul_open((uint8) g_radio_config.band_mode);
    }

    if (result == TRUE)
    {
        //��ȡ��ǰƵ��
        radio_get_freq();
    }

    return result;
}

//���ų�����ʼ��
bool scene_play_init(void)
{
    bool bret;

    //��װ����,��ʼ��fmģ��
    bret = radio_engine_init();
    if (bret == FALSE)
    {
        return FALSE;
    }

    //radio_set_mute(SetMUTE);
    //sys_os_time_dly(10);
    //com_set_mute(FALSE);//�������

    //��������
    com_reset_sound_volume(0);

    //���벥�ų�������ʼ��Ƶ�ʱ�ָ��
    init_point_freq();

    return TRUE;
}


uint8 fm_eg_channel_sel = 0xff;

//���ų���
app_result_e radio_scene_playing(void)
{
    app_result_e loop_deal_ret = RESULT_NULL;
    bool bret;
    bool need_reset_freq = FALSE;
    if(fm_eg_channel_sel == 0xFF)
    {
       fm_eg_channel_sel = (uint8) com_get_config_default(FMENGINE_CHANNEL_SEL);
    }
    bool no_need_play;

    no_need_play = g_standby_exit_flag;

    //AAͨ��ʱҪ�ص�������
    if (fm_eg_channel_sel == 0)
    {
        keytone_set_on_off(FALSE);
    }

    bret = scene_play_init();
    if (bret == FALSE)
    {
#if(SUPPORT_LED_DRIVER == 1)
        if (g_comval.support_led_display == 1)
        {
            //��ʾError���ȴ�2���ӣ��ڼ��а�������ǰ�˳�����һ��Ӧ�ã��ҿ��Լ�����Ӧ����
            show_error(); //����ʾ��û�������κθ����ã����ַ���Ҫ���ã������ƻ�����ͼ������ˢ�µ�UI
            com_app_sleep(2000); //��ʱ����ʾ�������ⰴ������
        }

        return RESULT_NEXT_FUNCTION;
#endif
    }

    //for RCP, ͬ����̨Ƶ�㣬�����ڵ�һ��com_tts_state_play֮ǰ��������APK���ȡ�������Ƶ��
    if (g_radio_config.FMStatus.freq != g_engine_status.FM_CurrentFreq)
    {
        need_reset_freq = TRUE;
    }
    //ʹ��radio_get_freq���ȡ�������Ƶ��
    g_engine_status.FM_CurrentFreq = g_radio_config.FMStatus.freq;

    //��������ͼ
    radio_play_create_main_view();

    //tts
    if (g_first_enter_play == TRUE)
    {
        g_radio_init_flag = TRUE;

        if ((g_ap_switch_var.call_background_status != CALL_BACK_STATUS_BACK) && (g_standby_exit_flag == FALSE))
        {
            com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_NOBLOCK, (void *) TTS_ENTER_RADIO);

            if (g_comval.support_bt_inback == 1)
            {
                //����������̨
                com_btmanager_init(FALSE);
            }
            else
            {
                //ɱ��������̨
                com_btmanager_exit(FALSE, TRUE);
            }

            //�ȴ�TTS����
            if (com_tts_state_play_wait() == TTS_PLAY_RET_BY_KEY)
            {
                sys_os_time_dly(10); //�ȴ�100ms���Ա�̰�̧���ܹ����
            }
        }

        while (1)
        {
            loop_deal_ret = com_view_loop();
            if (loop_deal_ret == RESULT_NONE_EVENT)
            {
                loop_deal_ret = RESULT_NULL;
                break;
            }
            else if (loop_deal_ret > RESULT_COMMON_RESERVE)
            {
                goto radio_playing_exit;
            }
            else
            {
                ; //nothing
            }
        }

        g_radio_init_flag = FALSE;
    }

    if (g_ap_switch_var.switch_to_radio_by_apkapp == TRUE)
    {
        g_neednot_freq_tts_play = TRUE;
        g_ap_switch_var.switch_to_radio_by_apkapp = FALSE;
    }

    if (g_radio_config.FMStatus.station > 0)
    {
        //ˢ�µ�̨��
        radio_station_create_msg_view(2000);
    }
    else
    {
        radio_freq_tts_play(g_radio_config.FMStatus.freq);
    }

    g_neednot_freq_tts_play = FALSE;

    //��ǰ��ȡ��Ƶ���Ҫ���õ�Ƶ�㲻һ��ʱ������Ҫǿ������Ƶ��
    if (need_reset_freq == TRUE)
    {
        //����FM�Ĳ���״̬
        g_playing_flag = TRUE;
        
        //ѡ�����ڲ��Ž���, ������Ƶ��, �������һ�¶�������
        radio_set_freq(g_radio_config.FMStatus.freq);

        need_reset_freq = FALSE;
    }
    //for RCP, ͬ����̨Ƶ��
    radio_get_freq();

    radio_connect_ain();

    if (no_need_play == 0)
    {
        radio_set_mute(releaseMUTE);
        g_playing_flag = TRUE;
    }
    else 
    {
    	  g_playing_flag = FALSE;
    }

    g_searching_flag = FALSE;
    loop_deal_ret = playing_loop_deal();
    radio_playing_exit: 
    if((loop_deal_ret >= RESULT_APP_QUIT) && (loop_deal_ret < RESULT_AUTO_SEARCH))
    {
        com_set_mute(TRUE); //����
    }
    return loop_deal_ret;
}

