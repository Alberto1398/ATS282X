#include "app_radio.h"

//ȫ�Զ���̨����������ʼ��
bool radio_auto_search_init(FM_SeekDir_e dir)
{
    uint8* freq_tab;

    if (g_radio_config.band_mode == Band_China_US)
    {
        //��ͨ����
        //libc_memset(g_radio_config.fmstation_us, 0, MAX_STATION_COUNT * 4);
        freq_tab = (uint8*) g_radio_config.fmstation_us;
        g_radio_config.FMStatus.freq = MIN_US;
    }
    else if (g_radio_config.band_mode == Band_Japan)
    {
        //�ձ�����
        //libc_memset(g_radio_config.fmstation_jp, 0, MAX_STATION_COUNT * 4);
        freq_tab = (uint8*) g_radio_config.fmstation_jp;
        g_radio_config.FMStatus.freq = MIN_JP;
    }
    else
    {
        //ŷ�޲���
        //libc_memset(g_radio_config.fmstation_eu, 0, MAX_STATION_COUNT * 4);
        freq_tab = (uint8*) g_radio_config.fmstation_eu;
        g_radio_config.FMStatus.freq = MIN_EU;
    }
    //�����̨��������ǰ��̨��
    g_radio_config.FMStatus.station = 0;
    g_radio_config.FMStatus.station_count = 0;

    libc_memset(freq_tab, 0, MAX_STATION_COUNT * 4);
    //��ջ����̨�б�
    libc_memset(&Auto_tab[0], 0, MAX_STATION_COUNT * 4);

    return TRUE;
}

//ȫ�Զ���̨�����˳�
bool radio_auto_search_exit(void)
{
    uint32 play_freq;

    uint8* ret_freq;

    uint8* ret_status;

    if (g_radio_config.FMStatus.station_count != 0)
    {
        //���������ĵ�̨��ѡ���һ������
        g_radio_config.FMStatus.station = 1;
        g_radio_config.FMStatus.freq = Auto_tab[0];
    }
    else
    {
        //һ����̨Ҳδ��������ѡ��Ĭ��Ƶ�㲥��
        g_radio_config.FMStatus.station = 0;
        if (g_radio_config.band_mode == Band_Japan)
        {
            play_freq = MIN_JP;
        }
        else
        {
            play_freq = MIN_US;
        }
        g_radio_config.FMStatus.freq = play_freq;
    }

    if (g_radio_config.band_mode == Band_China_US)
    {
        //��ͨƵ��
        //libc_memcpy(&g_radio_config.fmstation_us[0], &Auto_tab[0], MAX_STATION_COUNT * 4);
        //libc_memcpy(&g_radio_config.FMStatus_us, &g_radio_config.FMStatus, sizeof(fm_play_status_t));
        ret_freq = (uint8*) &g_radio_config.fmstation_us[0];
        ret_status = (uint8*) &g_radio_config.FMStatus_us;

    }
    else if (g_radio_config.band_mode == Band_Japan)
    {
        //�ձ�Ƶ��
        //libc_memcpy(&g_radio_config.fmstation_jp[0], &Auto_tab[0], MAX_STATION_COUNT * 4);
        //libc_memcpy(&g_radio_config.FMStatus_jp, &g_radio_config.FMStatus, sizeof(fm_play_status_t));
        ret_freq = (uint8*) &g_radio_config.fmstation_jp[0];
        ret_status = (uint8*) &g_radio_config.FMStatus_jp;
    }
    else
    {
        //ŷ��Ƶ��
        //libc_memcpy(&g_radio_config.fmstation_eu[0], &Auto_tab[0], MAX_STATION_COUNT * 4);
        //libc_memcpy(&g_radio_config.FMStatus_eu, &g_radio_config.FMStatus, sizeof(fm_play_status_t));
        ret_freq = (uint8*) &g_radio_config.fmstation_eu[0];
        ret_status = (uint8*) &g_radio_config.FMStatus_eu;
    }
    libc_memcpy(ret_freq, &Auto_tab[0], MAX_STATION_COUNT * 4);
    libc_memcpy(ret_status, &g_radio_config.FMStatus, sizeof(fm_play_status_t));
    //����Ƶ�㲥��
    radio_set_freq(g_radio_config.FMStatus.freq);

    return TRUE;
}

//ȫ�Զ���̨����
//dir: ��̨����( ���ϻ�����)
app_result_e radio_scene_auto_search(FM_SeekDir_e dir)
{
    app_result_e result;
    bool bret;
    FM_SeekMode_e mode;

    mode = g_radio_config.seek_mode;
    g_searching_flag = TRUE;
    /* ��ʼ��*/
    bret = radio_auto_search_init(dir);
    if (!bret)
    {
        g_searching_flag = FALSE;
        //��ʼ��ʧ�ܷ���
        return RESULT_ERROR;
    }


    //��ʼȫ�Զ���̨ǰ����mute
    radio_set_mute(SetMUTE);

    //������̨����ͼ
    g_search_current_freq = get_start_freq(dir);
    if (mode == HARDSEEK)
    {
        radio_hard_search_create_main_view();
    }
    else
    {
        radio_soft_search_create_main_view();
    }

    //��������
    com_tts_state_play(TTS_MODE_ONLYONE, (void *) TTS_SEARCHING);

    //��ȡ����ֵ
    g_search_throd = (uint8) com_get_config_default(FMENGINE_AP_ID_SEEKTH);
    //������̨����
    radio_set_throd(g_search_throd);
    PRINT_INFO_INT("start FM seek.throd&mode=", (g_search_throd << 16) + mode);
    if (mode == HARDSEEK)
    {
        
        //����Ӳ��ȫ�Զ���̨
        result = deal_hard_auto_seek(dir);
    }
    else
    {
        //�������ȫ�Զ���̨
        result = deal_soft_auto_seek(dir);
    }
    PRINT_INFO_INT("FM seek end.total-ch=",g_radio_config.FMStatus.station_count);
    radio_auto_search_exit();
    g_searching_flag = FALSE;
    //�ȵ�����Ƶ�ʵ�ʱ��Ž������
    //radio_set_mute(releaseMUTE);

    return result;
}

