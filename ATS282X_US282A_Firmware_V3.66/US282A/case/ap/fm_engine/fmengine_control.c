#include "app_fmengine.h"

app_result_e fmengine_message_deal_bank(private_msg_t* cur_msg) __FAR__;

void fm_eg_reply_msg(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);
}

//�����ǰ̨��������Ϣ
app_result_e __section__(".bank") fmengine_message_deal_bank(private_msg_t* cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /* ģ���ʼ����ж�����*/
        /*��Fmģ��*/
        case MSG_FM_MODE_OPEN:
        ret_value = fm_module_open(cur_msg);
        break;

        /*��Fm��Ƶ����ͨ��*/
        case MSG_FM_AIN_OPEN:
        ret_value = fm_Ain_open(cur_msg);
        break;

        /* FM ģ�����Standby */
        case MSG_FMENGINE_STANDBY:
        ret_value = fm_module_standby();
        break;

        case MSG_ENGINE_STANDBY_SYNC:
        ret_value = fmengine_standby(cur_msg);
        break;

        case MSG_APP_QUIT: //�˳�app
        /* Ҫ�������˳������غ�ж������*/
        if ((cur_msg->msg.content.data[0] & 0x01) != 0)
        {
            PRINT_INFO("eg enter s3bt");
        }
        case MSG_FMENGINE_CLOSE:
        /*��������Ҳ����FM ���淢MSG_FMENGINE_CLOSE  ��ϢҪ��FM �����˳�*/
        if (g_module_status != MODULE_STANDBY)
        {
            fm_module_standby();
        }
        ret_value = RESULT_APP_QUIT;
        break;

        /* �����������*/

        /* ������������*/
        case MSG_FMENGINE_SETMUTE:
        ret_value = fmengine_mute_sync(cur_msg);
        break;

        /* ���õ�̨����*/
        case MSG_FMENGINE_SETBAND:
        ret_value = fmengine_set_band_sync(cur_msg);
        break;

        /* ������̨����*/
        case MSG_FMENGINE_SETTHRED:
        ret_value = fmengine_set_throd(cur_msg);
        break;


        /* */
        case MSG_FMENGINE_PLAY:
        ret_value = fmengine_play(cur_msg);
        break;

        /* */
        case MSG_FMENGINE_PAUSE:
        ret_value = fmengine_stop(cur_msg);
        break;

        /* ��ȡ��Ϣ���*/

        /* ��ȡ��ǰ״̬��Ϣ*/
        case MSG_FMENGINE_GET_STATUS:
        ret_value = fmengine_get_status(cur_msg);
        break;

        /* ��ȡ��ǰ������Ϣ*/
        case MSG_FMENGINE_GET_BAND:
        ret_value = fmengine_get_band(cur_msg);
        break;

        /* ��ȡ��ǰ��̨�ź�ǿ����Ϣ*/
        case MSG_FMENGINE_GET_INTENSITY:
        ret_value = fmengine_get_intensity(cur_msg);
        break;

        /* ��ȡ����״̬( �����Ƿ����)*/
        case MSG_FMENGINE_GET_ANTENNA:
        ret_value = fmengine_get_antenna(cur_msg);
        break;

        /* ��ȡ������״̬��Ϣ*/
        case MSG_FMENGINE_GET_STEREO:
        ret_value = fmengine_get_stereo(cur_msg);
        break;

        /* ��ȡ��ǰ����״̬*/
        case MSG_FMENGINE_GET_PLAYSTATUS:
        ret_value = fmengine_get_playflag(cur_msg);
        break;

        /* ��ȡ��ǰģ��״̬*/
        case MSG_FMENGINE_GET_MODUAL_STATUS:
        ret_value = fmengine_get_modual_status(cur_msg);
        break;

        /*!����eq*/
        case MSG_SET_DAE_CFG_SYNC:
        //if (g_fm_chanel == 1)
        //{
        ret_value = fmengine_set_eq(cur_msg);
        //}
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        case MSG_SWITCH_AOUT_START_SYNC: //׼���л���Ƶ���
        ret_value = fmengine_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        case MSG_SWITCH_AOUT_STOP_SYNC: //����л���Ƶ���
        ret_value = fmengine_tts_stop(cur_msg);
        break;

        case MSG_FMENGINE_DEBUG:
        ret_value = fmengine_debug(cur_msg);
        break;

        default:
        break;
    }
    return ret_value;
}

//�����ǰ̨��������Ϣ
app_result_e fmengine_message_deal(private_msg_t* cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /* �����������*/

        /* ͬ������Ƶ�㲥��*/
        case MSG_FMENGINE_SETFREQ_SYNC:
        ret_value = fmengine_set_freq_sync(cur_msg);
        break;

        /* ���������̨*/
        case MSG_FMENGINE_SET_SOFTSEEK:
        ret_value = fmengine_set_softseek(cur_msg);
        break;

        /* ����Ӳ����̨*/
        case MSG_FMENGINE_SET_HARDSEEK:
        ret_value = fmengine_set_hardseek(cur_msg);
        break;

        /* �˳�Ӳ��seek ����*/
        case MSG_FMENGINE_SET_SEEKBREAK:
        ret_value = fmengine_seek_break(cur_msg);
        break;

        /* ��ȡ��Ϣ���*/

        /* ��ȡӲ���Զ���̨�Ƿ���ɱ�ʶ*/
        case MSG_FMENGINE_AUTO_SEEK_OVER:
        ret_value = fmengine_get_seekflag(cur_msg);
        break;

        /* ��ȡ��ǰƵ����Ϣ*/
        case MSG_FMENGINE_GET_FREQ:
        ret_value = fmengine_get_freq(cur_msg);
        break;

        default:
        ret_value = fmengine_message_deal_bank(cur_msg);
        break;
    }
    return ret_value;
}

//FM ����Ӧ��Ϊ����ģ�飬Ҳ������Ӧ�ý��̺�FM ����ͨѶ�Ľӿ�ģ��
app_result_e fmengine_control_block(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t fmengine_msg;
    //uint32 msg_type;

    while (1)
    {
        //��ȡ��������Ϣ
        if (get_app_msg(&fmengine_msg) == TRUE)
        {
            result = fmengine_message_deal(&fmengine_msg);
            if (result == RESULT_APP_QUIT)
            {
                break;
            }
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }
    return result;
}
