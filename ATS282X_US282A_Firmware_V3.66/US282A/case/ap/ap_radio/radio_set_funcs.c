#include "app_radio.h"

//FM��ʼ��
//band_mode: ����
bool radio_modul_open(uint8 band_mode)
{
    return radio_send_msg(&band_mode, NULL, MSG_FM_MODE_OPEN);
}

//�����ص�ͨ·
bool radio_connect_ain(void)
{
    return radio_send_msg(NULL, NULL, MSG_FM_AIN_OPEN);
}


int8 s_mute_count = 0; //���ﲻ����������������Ϊ���÷��ᱣ֤
//freq��λkHz
bool radio_set_freq(uint32 freq)
{
    bool bret = TRUE;
    
    //�������(��һ�εĶ�ʱ��û�е�����Ҫȡ����ʱ������releasemute ����)
    //radio_set_mute(releaseMUTE);
    if (g_mute_set_timer != -1)
    {
        kill_app_timer(g_mute_set_timer);
        g_mute_set_timer = -1;
        //��������һ������Э:�������muteֱ�ӵ�release,Ҳʹ֮�ܳ�����
        if (s_mute_count > 0) 
        {
            s_mute_count--;
        }
    }

    //����, ����Ƶ���л������е�����
    radio_set_mute(SetMUTE);
    //send message
    bret = radio_send_msg(&freq, NULL, MSG_FMENGINE_SETFREQ_SYNC);

    if (g_mute_set_timer == -1)
    {
        g_mute_set_timer = set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL | (APP_TIMER_TAG_SYS << 8),
                150, release_mute_timer);

        if (g_mute_set_timer == -1)
        {
            sys_os_time_dly(15);
            //�������
            radio_set_mute(releaseMUTE);
        }
    }

    //��FM������ͣʱ���л�Ƶ���Ҫplay�£������ܲ���
    if (g_playing_flag == FALSE)
    {
        radio_set_mute(releaseMUTE);
        radio_send_msg(NULL, NULL, MSG_FMENGINE_PLAY);
        g_playing_flag = TRUE;
    }

    //for RCP, ͬ����̨Ƶ��
    radio_get_freq();

    return bret;
}

//������������
//  (�����õ��Ǽ�����)
bool radio_set_mute(FM_MUTE_e mode)
{
    //����mute�Ĵ���
    //static int8 s_mute_count = 0; //���ﲻ����������������Ϊ���÷��ᱣ֤

    if (mode == SetMUTE)
    {
        if (s_mute_count == 0)
        {
            s_mute_count++;
        }
        else
        {
            s_mute_count++;
            return TRUE;
        }
    }
    else
    {
        if (s_mute_count > 0) //��������һ������Э:�������muteֱ�ӵ�release,Ҳʹ֮�ܳ�����
        {
            s_mute_count--;
        }
        if (s_mute_count == 0)
        {
        }
        else
        {
            return TRUE;
        }
    }

    return radio_send_msg(&mode, NULL, MSG_FMENGINE_SETMUTE);
}

//���õ�̨����
bool radio_set_band(radio_band_e mode)
{
    return radio_send_msg(&mode, NULL, MSG_FMENGINE_SETBAND);
}

//������̨����
//val: ����(��Ӧ�������е���������)
bool radio_set_throd(uint8 val)
{
    return radio_send_msg(&val, NULL, MSG_FMENGINE_SETTHRED);
}

/*! \endcond */

