#include "app_fmengine.h"
#include "mmm_mr.h"
#include <analog.h>

//SpeakOut: TRUE��AA���,FLASE�ر�AA���
void fm_analog_deal(bool SpeakOut)
{
    if (SpeakOut == TRUE)
    {
        //enable_adc(AI_SOURCE_FM, 4);
        enable_ain(AI_SOURCE_FM, 7);
        //delay
        sys_mdelay(100);
        enable_ain_out(AI_SOURCE_FM);
    }
    else
    {
        //enable_aa_mute();
        disable_ain_out(AI_SOURCE_FM);
        disable_ain(AI_SOURCE_FM);
        //disable_adc();
    }
}

//FM ģ���
app_result_e fm_module_open(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    uint8 band_info, level;
    int result = RESULT_NULL;
    app_result_e ret = RESULT_NULL;

    band_info = *(uint8*) data_ptr->msg.content.addr;

    level = g_fmengine_cfg.fm_threshold; //��ʼ��������̨����

    //ģ���ʼ��
    result = fm_open(band_info, level, 0);

    if (result != -1)
    {
        g_module_status = MODULE_WORK;
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        g_module_status = MODULE_NO_INIT;
    }
    fm_eg_reply_msg(cur_msg);
    return ret;
}

//FMģ��ͨ����/�м��PLAY
app_result_e fm_Ain_open(private_msg_t* cur_msg)
{
    app_result_e ret;
    if (g_module_status == MODULE_WORK)
    {
        cur_msg->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;

        if (g_fm_chanel == 1)
        {
            play(); //play
        }
        else
        {
            fm_analog_deal(TRUE); //analog channel
        }

        com_set_sound_out(SOUND_OUT_START);
    }
    else
    {
        cur_msg->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL;
    }
    fm_eg_reply_msg(cur_msg);
    return ret;
}

//��fmģ���Standby
app_result_e fm_module_standby(void)
{
    int result;

    /* ��Щģ����ܴ���©�磬��δ��ʼ��ʱ����Ҫ�������standby */
    if (g_module_status != MODULE_STANDBY)
    {
        result = fm_close();
        //����Standby 0�ɹ� -1ʧ��
        if (result != -1)
        {
            if (g_module_status == MODULE_WORK)
            {
                if (g_fm_chanel == 1)
                {
                    stop();
                }
                else
                {
                    //disable_ain(fm_ain_type);
                    fm_analog_deal(FALSE);
                }

                com_set_sound_out(SOUND_OUT_STOP);
            }

            //ģ�����Standby ״̬
            g_module_status = MODULE_STANDBY;
        }
    }
    return RESULT_IGNORE;
}

//����fm�������߽������
app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8* parm_ptr;
    //uint8 vol_bak;
    FM_MUTE_e mode;
    int result = 0;
    //uint32 set_vol;
    //get param data
    parm_ptr = (uint8*) data_ptr->msg.content.addr;

    mode = (FM_MUTE_e) parm_ptr[0];

    result = fm_mute(mode);

    //0�ɹ� -1ʧ��
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        if (mode == releaseMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PLAY;
            change_engine_state(ENGINE_STATE_PLAYING);
        }
        else if (mode == SetMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PAUSE;
            change_engine_state(ENGINE_STATE_PAUSE);
        }
        else
        {
            ; //for QAC
        }
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}


//���ò���
app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    radio_band_e band_info;

    //�����õĵ�̨���Σ���radio_band_e
    band_info = *(radio_band_e*) data_ptr->msg.content.addr;
    result = fm_set_band(band_info);

    if (result != -1) //0�ɹ� -1ʧ��
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;

}

//������̨����
app_result_e fmengine_set_throd(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 level;
    int result;

    //level = data_ptr->msg.content.data[0];
    level = *(uint8*) data_ptr->msg.content.addr;
    result = fm_set_throd(level);
    if (result != -1) //0�ɹ� -1ʧ��
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_cfg.fm_threshold = level;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;

}


//����TTS����ǰ����ͣ����
app_result_e fmengine_tts_start(private_msg_t* cur_msg)
{
    if (g_fmengine_status.FM_VolStatus == STATUS_PLAY)
    {
        fm_mute(SetMUTE);
        g_stop_by_tts_flag = TRUE;
    }
    //for adda chanel
    if (g_fm_chanel == 1)
    {
        stop();
    }
    else
    {
        fm_analog_deal(FALSE);
    }

    cur_msg->reply->type = MSG_REPLY_SUCCESS;
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//TTS�����������ָ�����
app_result_e fmengine_tts_stop(private_msg_t* cur_msg)
{
    if (g_stop_by_tts_flag == TRUE)
    {
        fm_mute(releaseMUTE);
        g_stop_by_tts_flag = FALSE;
    }

    if (g_fm_chanel == 1)
    {
        play();
    }
    else
    {
        fm_analog_deal(TRUE);
    }

    cur_msg->reply->type = MSG_REPLY_SUCCESS;
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

app_result_e fmengine_debug(private_msg_t *cur_msg)
{
    *(int *) cur_msg->reply->content = fm_debug();
    cur_msg->reply->type = MSG_REPLY_SUCCESS;
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//STANDBY������Ӧ
app_result_e fmengine_standby(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *)(cur_msg->msg.content.addr);

    //�����ֳ���׼������˯��ģʽ
    PRINT_INFO("engine standby enter");

    //stop play
    if (g_fmengine_status.FM_VolStatus == STATUS_PLAY)
    {
        fm_mute(SetMUTE);
    }
    //for adda chanel
    if (g_fm_chanel == 1)
    {
        stop();
    }
    else
    {
        fm_analog_deal(FALSE);
    }

    //fm enter standby
    fm_close();

    //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
    cur_msg->reply->type = MSG_REPLY_SUCCESS;
    fm_eg_reply_msg(cur_msg);

    libc_sem_wait(standby_sem, 0);

    //��˯��ģʽ�������ָ��ֳ�
    PRINT_INFO("engine standby exit");

    //fm restore
    fm_open(g_fmengine_status.FM_CurBand,g_fmengine_cfg.fm_threshold,0);

    //start
    fm_set_freq(g_fmengine_status.FM_CurrentFreq);

    if (g_fmengine_status.FM_VolStatus == STATUS_PLAY)
    {
        fm_mute(releaseMUTE);
    }

    if (g_fm_chanel == 1)
    {
        play();
    }
    else
    {
        fm_analog_deal(TRUE);
    }

    standby_restart_all_app_timer(g_app_timer_vector, g_app_timer_count);

    return RESULT_IGNORE;
}
