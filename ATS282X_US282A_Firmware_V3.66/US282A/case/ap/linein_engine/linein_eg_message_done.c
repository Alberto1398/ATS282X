#include "app_linein_eg.h"
#include <analog.h>

app_result_e linein_eg_message_done_bank(private_msg_t* cur_msg)
{
    g_linein_eg_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /*���ò������*/
        //case MSG_LINEIN_EG_SET_EQ_SYNC://����eq +eq curve data
        case MSG_SET_DAE_CFG_SYNC:
        linein_eg_set_eq(cur_msg);
        break;

        /*�������*/
        case MSG_LINEIN_EG_PLAY_SYNC: //��ʼ����
        linein_eg_play(cur_msg);
        break;

        case MSG_LINEIN_EG_STOP_SYNC: //ֹͣ����
        linein_eg_stop(cur_msg);
        break;

        case MSG_ENGINE_STANDBY_SYNC:
        g_linein_eg_result = linein_eg_standby(cur_msg);
        break;

        case MSG_APP_QUIT: //�˳�app
        if ((cur_msg->msg.content.data[0] & 0x01) != 0)
        {
            PRINT_INFO("eg enter s3bt");
        }
        g_linein_eg_result = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        case MSG_SWITCH_AOUT_START_SYNC: //׼���л���Ƶ���
        g_linein_eg_result = linein_eg_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        case MSG_SWITCH_AOUT_STOP_SYNC: //����л���Ƶ���
        g_linein_eg_result = linein_eg_tts_stop(cur_msg);
        break;

        default:
        break;
    }

    return g_linein_eg_result;
}

//����
app_result_e linein_eg_play(void* msg_ptr)
{
    bool ret_vals;

    if (g_linein_channel_sel == LINEIN_CH_A)
    {
        ret_vals = linein_eg_aa_play();
        g_eg_status_p->play_status = linein_play_status_play; //���ò���״̬
    }
    else
    {
        //�����ļ�����
        ret_vals = play(); //����
    }

    linein_eg_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//��ͣ
app_result_e linein_eg_stop(void* msg_ptr)
{
    bool ret_vals;

    if (g_linein_channel_sel == LINEIN_CH_A)
    {
        ret_vals = linein_eg_aa_stop();
        g_eg_status_p->play_status = linein_play_status_stop;
    }
    else
    {
        ret_vals = stop(); //ֹͣ����
    }

    linein_eg_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

void _error_handle(void)
{
    //�رս����м��
    if (g_linein_channel_sel != LINEIN_CH_A)
    {
        stop();
    }
}

//ģ��ͨ��ʱ������
bool linein_eg_aa_play(void)
{
    uint8 gain_val = 2;
    gain_val = (uint8) com_get_config_default(LINEIN_GAIN_INPUT);


    enable_ain(AI_SOURCE_LINEIN, (uint32) gain_val);

    sys_mdelay(200);

    //enable_adc(2, com_get_config_default(LINEIN_GAIN_ADC));

    //disable_aa_mute();
    enable_ain_out(AI_SOURCE_LINEIN);

    return TRUE;
}

//ģ��ͨ��ʱ����ͣ
bool linein_eg_aa_stop(void)
{
    //enable_aa_mute();
    disable_ain_out(AI_SOURCE_LINEIN);

    disable_ain(AI_SOURCE_LINEIN);

    //disable_adc();

    return TRUE;
}

