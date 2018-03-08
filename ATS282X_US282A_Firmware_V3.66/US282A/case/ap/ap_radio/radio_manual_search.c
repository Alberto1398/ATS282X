#include "app_radio.h"

//���Զ���̨������ʼ��
bool scene_manual_search_init(void)
{
    bool ret = TRUE;

    //ȡ�õ�ǰ����Ƶ��
    radio_get_freq();

    //����������ʼƵ��
    startfreq = g_engine_status.FM_CurrentFreq;

    g_radio_config.FMStatus.freq = startfreq;

    //��ʼ���Զ���̨ǰ����mute
    radio_set_mute(SetMUTE);
    return ret;
}

//���Զ���̨�����˳�
bool scene_manual_search_exit(void)
{
    uint8 num;
    bool result;

    //��ȡ��ǰƵ��ֵ
    result = radio_get_freq();
    if (result == TRUE)
    {
        //������ȡ�����ŵ�ǰƵ��
        g_radio_config.FMStatus.freq = g_engine_status.FM_CurrentFreq;

        if (g_radio_config.FMStatus.freq > max_freq[g_radio_config.band_mode])
        {
            //�������Ƶ��
            g_radio_config.FMStatus.freq = startfreq;
        }

        if (g_radio_config.FMStatus.freq < min_freq[g_radio_config.band_mode])
        {
            //С����СƵ��(87-87.5֮���ֵ)
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        //��ȡʧ�ܣ����Ű��Զ���̨ǰƵ��
        g_radio_config.FMStatus.freq = startfreq;
    }

    //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //�л����Ƶ����δ����Ƶ��
    if (num == 0xff)
    {
        g_radio_config.FMStatus.station = 0;
    }
    else
    {
        //�л����Ƶ�����ѱ���Ƶ��
        g_radio_config.FMStatus.station = num;
    }

    //����Ƶ�㲥��
    radio_set_freq(g_radio_config.FMStatus.freq);

    //�ȵ�����Ƶ�ʵ�ʱ��Ž������
    //radio_set_mute(releaseMUTE);

    return TRUE;
}

//���Զ���̨����
//dir: ���Զ���̨����
app_result_e radio_scene_manual_search(FM_SeekDir_e dir)
{
    bool result;
    //����ֵ
    app_result_e ret = RESULT_NULL;
    //for RCP, ��������״̬
    g_searching_flag = TRUE;
    result = scene_manual_search_init();
    if (result == RESULT_NULL)
    {
        g_searching_flag = FALSE;
        //�����������ų���
        return ret;
    }

    //������̨����ͼ
    g_search_current_freq = g_radio_config.FMStatus.freq;
    if (g_radio_config.seek_mode == HARDSEEK)
    {
        radio_hard_search_create_main_view();
    }
    else
    {
        radio_soft_search_create_main_view();
    }

    com_tts_state_play(TTS_MODE_ONLYONE, (void *) TTS_SEARCHING);
    //��ȡ����ֵ
    g_search_throd = (uint8) com_get_config_default(FMENGINE_AP_ID_SEEKTH);
    //������̨����
    radio_set_throd(g_search_throd);

    if (g_radio_config.seek_mode == HARDSEEK)
    {
        //����Ӳ�����Զ���̨
        ret = deal_hard_manual_search(dir);
    }
    else
    {
        //����������Զ���̨
        ret = deal_soft_manual_search(dir);
    }

    //for RCP, �˳�����״̬
    g_searching_flag = FALSE;

    scene_manual_search_exit();

    return ret;
}
