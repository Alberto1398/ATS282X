#include "app_radio.h"

uint8 radio_rcp_buffer[512 + RCP_COMMAND_HEAD_SIZE] _BANK_DATA_ATTR_;


/* COMMON set/get/control������ص�����ע��� */
const rcp_cmd_cb_t radio_rcp_cmd_cb_tbl[] =
{
    { RCP_CMD_SETTING, SETTING_ID_STATION_BAND, (void *) radio_set_station_band },
    { RCP_CMD_SETTING, SETTING_ID_STATION_FREQ, (void *) radio_set_station_freq },

    { RCP_CMD_QUERY, QUERY_ID_GLOBAL_RCP_INFO, (void *) radio_get_global_rcp_info },
    { RCP_CMD_QUERY, QUERY_ID_STATION_LIST, (void *) radio_get_station_list },
    { RCP_CMD_QUERY, QUERY_ID_STATION_NAME, (void *) radio_get_station_name },

    //ToDo
    { RCP_CMD_CONTROL, CONTROL_FM_SEARCH, (void *) radio_apk_key_search },
    { RCP_CMD_CONTROL, CONTROL_FM_STOP_SCH, (void *) radio_apk_key_stop_search },
    { RCP_CMD_CONTROL, CONTROL_FM_CHADD, (void *) radio_apk_key_chadd },
    { RCP_CMD_CONTROL, CONTROL_FM_CHSUB, (void *) radio_apk_key_chsub },
    { RCP_CMD_CONTROL, CONTROL_FM_MUTE, (void *) radio_apk_key_mute },

    /* ������ */
    { 0, 0, NULL },
};

/*��ʼ��RCP�ص������б�*/
void radio_rcp_var_init(void)
{
    com_rcp_set_callback(radio_rcp_cmd_cb_tbl, radio_get_global_rcp_info);
}

/*����RCP�ص������б�*/
void radio_rcp_var_exit(void)
{
    com_rcp_set_callback(NULL, NULL);
}

/*RCP��ȡȫ����Ϣ�ṹ��*/
app_result_e __section__(".rcode") radio_get_global_rcp_info(uint32 data1, uint32 data2)
{
    uint8 tmp_radio_global_rcp_info[sizeof(radio_global_rcp_info_t) + RCP_COMMAND_HEAD_SIZE];
    radio_global_rcp_info_t * p_radio_global_rcp_info = (radio_global_rcp_info_t *) (tmp_radio_global_rcp_info
            + RCP_COMMAND_HEAD_SIZE);
    uint8 is_station;

    libc_memset(p_radio_global_rcp_info, 0x00, sizeof(radio_global_rcp_info_t));

    //��ȡ������Ϣ
    com_get_com_info(&(p_radio_global_rcp_info->com_info));
    //��ȡAPP��Ϣ
    p_radio_global_rcp_info->app_info.searching = g_searching_flag;
    p_radio_global_rcp_info->app_info.mute = !g_playing_flag;
    p_radio_global_rcp_info->app_info.band = g_radio_config.band_mode;

    p_radio_global_rcp_info->app_info.freq = g_engine_status.FM_CurrentFreq;/*�ñ������뱣֤������ʵ��Ƶ��һ��*/

    is_station = whether_in_list(g_engine_status.FM_CurrentFreq, pfreq);
    p_radio_global_rcp_info->app_info.is_station = (uint8) (is_station != 0xff);

    com_rcp_send_pkg(0, 0, tmp_radio_global_rcp_info, sizeof(radio_global_rcp_info_t),
            sizeof(radio_global_rcp_info_t));

    return RESULT_NULL;
}

/*��ȡ��Ч��̨�б�*/
app_result_e radio_get_station_list(uint32 data1, uint32 data2)
{ //�����ص�ǰƵ�ε���Ч��̨�б�
    uint32* p_station_list = (uint32*) (radio_rcp_buffer + RCP_COMMAND_HEAD_SIZE);
    uint32 tmp_max_freq, tmp_min_freq, tmp_freq;
    uint8 freq_count;

    tmp_max_freq = max_freq[g_radio_config.band_mode];
    tmp_min_freq = min_freq[g_radio_config.band_mode];

    for (freq_count = 0; freq_count < MAX_STATION_COUNT; freq_count++)
    {
        tmp_freq = *(pfreq + freq_count);
        if ((tmp_freq >= tmp_min_freq) && (tmp_freq <= tmp_max_freq))
        {
            *(p_station_list + freq_count) = tmp_freq;
        }
        else
        {
            //�����Ƿ�Ƶ������Ժ���Ƶ��
            break;
        }
    }

    com_rcp_send_pkg(freq_count, 0, radio_rcp_buffer, sizeof(uint32) * freq_count, sizeof(uint32) * freq_count);

    return RESULT_NULL;
}

/*��ȡ��ǰ��̨����*/
app_result_e radio_get_station_name(uint32 data1, uint32 data2)
{
    return RESULT_NULL;
}

/*�л���̨Ƶ��*/
app_result_e radio_set_station_band(uint32 data1, uint32 data2)
{
    radio_band_e old_band;
    radio_band_e new_band;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    old_band = g_radio_config.band_mode;
    new_band = g_radio_config.band_mode = (radio_band_e) data1;

    g_neednot_freq_tts_play = TRUE;
    radio_do_switch_band(old_band, new_band);
    g_neednot_freq_tts_play = FALSE;

    return RESULT_STOP_SEARCH;//�л�ʱֹͣ�Զ���̨
}

/*���õ�̨Ƶ��*/
//data1:��λ:kHz
app_result_e radio_set_station_freq(uint32 data1, uint32 data2)
{
    uint32 set_freq = data1;
    uint16 digital_num;//��λ:50kHz, ����Ԥ���̨�����

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    if(set_freq <= g_radio_config.FMStatus.station_count)
    {
        digital_num = set_freq;//Ԥ���̨�����
    }
    else if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲���, ����Ϊ50K
        digital_num = (uint16) ((set_freq + 25) / 50);
    }
    else
    {
        digital_num = (uint16) ((set_freq + 50) / 100 * 100 / 50);
    }

    //g_digital_countΪradio_digital_to_chanel��Ҫ����
    if (digital_num > 999)
    {
        g_digital_count = 4;
    }
    else
    {
        g_digital_count = 3;
    }

    g_neednot_freq_tts_play = TRUE;
    radio_digital_to_chanel(digital_num);
    g_neednot_freq_tts_play = FALSE;

    return RESULT_STOP_SEARCH;//�л�ʱֹͣ�Զ���̨
}

app_result_e radio_apk_key_search(uint32 data1, uint32 data2)
{
    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }
    if(g_searching_flag == FALSE)
    {
        return RESULT_AUTO_SEARCH;
    }
    return RESULT_NULL;
}

app_result_e radio_apk_key_stop_search(uint32 data1, uint32 data2)
{
    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }
    if(g_searching_flag == TRUE)
    {
        return RESULT_STOP_SEARCH;
    }
    return RESULT_NULL;
}

app_result_e radio_apk_key_chadd(uint32 data1, uint32 data2)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    g_neednot_freq_tts_play = TRUE;
    result = select_station(SELECT_NEXT_DIRECTION);
    g_neednot_freq_tts_play = FALSE;

    return result;
}

app_result_e radio_apk_key_chsub(uint32 data1, uint32 data2)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    g_neednot_freq_tts_play = TRUE;
    result = select_station(SELECT_PREV_DIRECTION);
    g_neednot_freq_tts_play = FALSE;

    return result;
}

//520-->mute�����Ϊplay/stop����
app_result_e radio_apk_key_mute(uint32 data1, uint32 data2)
{

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }
    if (g_playing_flag == TRUE)
    {
        g_playing_flag = FALSE;
        sys_os_time_dly(2);//20ms���ⰴ�����������ص�
        radio_send_msg(NULL, NULL, MSG_FMENGINE_PAUSE);
        radio_set_mute(SetMUTE);
        com_tts_state_play(TTS_MODE_ONLYONE, (void*)TTS_PAUSE_SONG);
    }
    else
    {
        g_playing_flag = TRUE;
        radio_set_mute(releaseMUTE);
        com_tts_state_play(TTS_MODE_ONLYONE, (void*)TTS_PLAY_SONG);
        radio_send_msg(NULL, NULL, MSG_FMENGINE_PLAY);
        
    }

    return RESULT_STOP_SEARCH;//�л�ʱֹͣ�Զ���̨;
}
