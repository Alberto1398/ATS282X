#include "app_radio.h"

app_result_e radio_play_key_deal_next_freq(void);
app_result_e radio_play_key_deal_prev_freq(void);
app_result_e radio_play_key_deal_seek_next(void);
app_result_e radio_play_key_deal_seek_prev(void);
app_result_e radio_play_key_deal_switch_next_preset(void);
app_result_e radio_play_key_deal_switch_prev_preset(void);
app_result_e radio_play_key_deal_auto_search(void);
app_result_e radio_play_key_deal_switch_band(void);
app_result_e radio_play_key_deal_play_mute(void);
app_result_e radio_play_key_deal_play_pause(void);
app_result_e radio_play_key_deal_number_input(void);

const key_event_map_t __section__(".rodata.ke_maplist") radio_play_ke_maplist[] =
{
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    /*! �л�����һ��Ƶ�� */
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_next_preset },
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_next_preset },
    /*! �л�����һ��Ƶ�� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_prev_preset },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_prev_preset },
    /*! �����ֶ���̨ */
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, KEY_DEAL_FILTER }, radio_play_key_deal_seek_next },
    /*! �����ֶ���̨ */
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, KEY_DEAL_FILTER }, radio_play_key_deal_seek_prev },
#else
    /*! �л�����һ��Ƶ�� */
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_next_freq },
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_next_freq },
    /*! �л�����һ��Ƶ�� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_prev_freq },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_prev_freq },
    /*! �����ֶ���̨ */
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, KEY_DEAL_FILTER }, radio_play_key_deal_seek_next },
    /*! �����ֶ���̨ */
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, KEY_DEAL_FILTER }, radio_play_key_deal_seek_prev },
    /*! �л�����һ��̨ */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_next_preset },
#endif
    /*! �л�����һ��̨ */
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_play_pause },
    { { KEY_PHONEPLAY, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_play_pause },
#else
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_next_preset },
#endif
    /*! �Զ���̨ */
    //#if(_DEBUG == 1)
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    { { KEY_PLAY, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, KEY_DEAL_FILTER}, radio_play_key_deal_auto_search},
    { { KEY_PHONEPLAY, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, KEY_DEAL_FILTER}, radio_play_key_deal_auto_search},
#endif
    /*! ѭ���л����� */
    { { KEY_REPEAT, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_switch_band },
    /*! ���� */
    { { KEY_MUTE, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, radio_play_key_deal_play_mute },
    /*! ����Ƶ�� */
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG_UP, KEY_DEAL_NULL }, radio_play_key_deal_number_input },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, KEY_DEAL_NULL }, NULL },
};

app_result_e radio_play_key_deal_next_freq(void)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = change_freq(DIR_UP);

    return result;
}

app_result_e radio_play_key_deal_prev_freq(void)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = change_freq(DIR_DOWN);

    return result;
}

app_result_e radio_play_key_deal_seek_next(void)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = RESULT_MANUAL_SEARCH_UP;

    return result;
}

app_result_e radio_play_key_deal_seek_prev(void)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = RESULT_MANUAL_SEARCH_DOWN;

    return result;
}

app_result_e radio_play_key_deal_switch_next_preset(void)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = select_station(SELECT_NEXT_DIRECTION);

    return result;
}

app_result_e radio_play_key_deal_switch_prev_preset(void)
{
    app_result_e result;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = select_station(SELECT_PREV_DIRECTION);

    return result;
}

app_result_e radio_play_key_deal_auto_search(void)
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

__section__(".text.bank2") app_result_e radio_play_key_deal_switch_band(void)
{
    app_result_e result;
    radio_band_e old_band;

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    old_band = g_radio_config.band_mode;
    //ѭ���л�����
    if (g_radio_config.band_mode < Band_Europe)
    {
        g_radio_config.band_mode++;
    }
    else
    {
        g_radio_config.band_mode = Band_China_US;
    }

    radio_do_switch_band(old_band, g_radio_config.band_mode);

    result = RESULT_NULL;

    return result;
}

app_result_e radio_play_key_deal_play_pause(void)
{
    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }
    if (g_playing_flag == TRUE)
    {
        g_playing_flag = FALSE;
        radio_send_msg(NULL, NULL, MSG_FMENGINE_PAUSE);
        radio_set_mute(SetMUTE);
        com_tts_state_play(TTS_MODE_ONLYONE, (void*)TTS_PAUSE_SONG);
        
    }
    else
    {
        com_tts_state_play(TTS_MODE_ONLYONE, (void*)TTS_PLAY_SONG);
        radio_set_mute(releaseMUTE);
        radio_send_msg(NULL, NULL, MSG_FMENGINE_PLAY);
        g_playing_flag = TRUE;
    }

    return RESULT_NULL;
}

app_result_e radio_play_key_deal_play_mute(void)
{
    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    com_set_mute(!get_mute_enable());
    if (get_mute_enable() == FALSE)
    {
        radio_set_mute(SetMUTE);
        g_playing_flag = FALSE;
    }
    else
    {
        radio_set_mute(releaseMUTE);
        g_playing_flag = TRUE;
    }

    return RESULT_NULL;
}

__section__(".text.bank2") app_result_e radio_play_key_deal_number_input(void)
{
    app_result_e result;
    uint8 number = (uint8) (g_p_view_ke->val - KEY_NUM0);

    if (g_radio_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    result = radio_play_key_digital_store(number);

    return result;
}

//�л�����
//old_band: ��ǰƵ��
//new_band: ��Ƶ��
__section__(".text.bank2") void radio_do_switch_band(radio_band_e old_band, radio_band_e new_band)
{
    uint8 *pstatus_tmp;

    //play freq
    uint32 enter_preq;

    uint8 num;
    //save old band infomation
    if (old_band == Band_Japan)
    {
        pstatus_tmp = (uint8 *) &g_radio_config.FMStatus_jp;
    }
    else if (old_band == Band_Europe)
    {
        pstatus_tmp = (uint8 *) &g_radio_config.FMStatus_eu;
    }
    else
    {
        pstatus_tmp = (uint8 *) &g_radio_config.FMStatus_us;
    }
    //save info
    libc_memcpy(pstatus_tmp, &g_radio_config.FMStatus, sizeof(fm_play_status_t));

    //�ı䲨��
    radio_set_band(new_band);

    //��ʼ��Ƶ��ָ��
    init_point_freq();
    /* ��ǰ����Ƶ��*/
    if (new_band == Band_Japan)
    {
        pstatus_tmp = (uint8 *) &g_radio_config.FMStatus_jp;
    }
    else if (new_band == Band_Europe)
    {
        pstatus_tmp = (uint8 *) &g_radio_config.FMStatus_eu;
    }
    else
    {
        pstatus_tmp = (uint8 *) &g_radio_config.FMStatus_us;
    }
    //get new band play status
    libc_memcpy(&g_radio_config.FMStatus, pstatus_tmp, sizeof(fm_play_status_t));

    enter_preq = g_radio_config.FMStatus.freq;

    if ((enter_preq <= max_freq[g_radio_config.band_mode]) && (enter_preq >= min_freq[g_radio_config.band_mode]))
    {
        num = whether_in_list(enter_preq, pfreq);
        //�л����Ƶ����δ����Ƶ��
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
            com_view_update(RADIO_MAIN_VIEW_PLAY);
        }
        else
        {
            //�л����Ƶ�����ѱ���Ƶ��
            g_radio_config.FMStatus.station = num;
            //��̨�Ž���
            radio_station_create_msg_view(2000);
            //����Ƶ�㲥��
            radio_set_freq(g_radio_config.FMStatus.freq);
            return;
        }
    }
    else
    {
        //���Ԥ���̨���в�����,�򲥷Ÿò�����СƵ��
        g_radio_config.FMStatus.freq = min_freq[new_band];
    }

    //����Ƶ�㲥��
    radio_set_freq(g_radio_config.FMStatus.freq);
}

//�л�����һ������һ��Ƶ��
//dir: �л�����
app_result_e change_freq(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space;
    uint8 num;
    //����
    space = 100;

    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ��Ƶ��,  ��50kΪ����
        space = 50;
    }

    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
            //�����л�Ƶ��
            g_radio_config.FMStatus.freq += space;
        }
        else
        {
            //���Ƶ���л�����С
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
        {
            //�����л�
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //��СƵ���л������
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }

    //�ж��л����Ƶ���Ƿ��ڵ�ǰ�����ŵ�Ԥ���̨����
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //�л����Ƶ����δ����Ƶ��
    if (num == 0xff)
    {
        //û����Ԥ���̨����
        g_radio_config.FMStatus.station = 0;
        com_view_update(RADIO_MAIN_VIEW_PLAY);
        //����Ƶ�㲥��
        radio_set_freq(g_radio_config.FMStatus.freq);
    }
    else
    {
        //�л����Ƶ�����ѱ���Ƶ��
        g_radio_config.FMStatus.station = num;
        //��̨�Ž���
        radio_station_create_msg_view(2000);

        //����Ƶ�㲥��
        radio_set_freq(g_radio_config.FMStatus.freq);
    }

    return ret;
}

//�����������밴����Ϣ
//number: 0~9
/*******************************************************************************/
__section__(".text.bank2") app_result_e radio_play_key_digital_store(uint8 number)
{
    //buffer����������±���,�����뿪���ֵ㲥����ҲҪ���
    if ((g_digital_count >= MAX_DIGITAL_COUNT) || (g_digit_input_flag == FALSE))
    {
        g_digital_count = 0;
        libc_memset(g_digital_buf, 0, MAX_DIGITAL_COUNT);
    }

    g_digital_buf[g_digital_count] = number;

    g_digital_count++;

    g_digit_input_flag = TRUE;

    //��ʾ����
    radio_digit_create_sub_view(2000);

    return RESULT_NULL;
}

//�л�����һ���ѱ����̨
//direction: �л���̨����0��ʾ�л�����һ����̨��1��ʾ�л�����һ����̨
app_result_e select_station(uint8 direction)
{
    app_result_e ret = RESULT_NULL;

    //ѭ������
    uint8 i, cur_num;
    //��̨Ƶ��ֵ
    uint32 station_freq;

    radio_set_mute(SetMUTE);

    //Ԥ���б������ѱ����̨
    if (g_radio_config.FMStatus.station_count > 0)
    {
        if (g_radio_config.FMStatus.station == 0)
        {
            //��ǰ����Ԥ���б��е�Ƶ���ϣ��л�����һ��Ƶ��
            g_radio_config.FMStatus.station = 1;
            for (i = 0; i < g_radio_config.FMStatus.station_count; i++)
            {
                if (pfreq[i] >= g_radio_config.FMStatus.freq)
                {
                    g_radio_config.FMStatus.station = i + 1;
                    break;
                }
            }
        }
        else
        {
            if (direction == SELECT_NEXT_DIRECTION)
            {
                if (g_radio_config.FMStatus.station < g_radio_config.FMStatus.station_count)
                {
                    //�л���̨��
                    g_radio_config.FMStatus.station++;
                }
                else
                {
                    //���ص�һ��
                    g_radio_config.FMStatus.station = 1;
                }
            }
            else
            {
                if (g_radio_config.FMStatus.station > 1)
                {
                    //�л���̨��
                    g_radio_config.FMStatus.station--;
                }
                else
                {
                    //���ص�һ��
                    g_radio_config.FMStatus.station = g_radio_config.FMStatus.station_count;
                }
            }
        }
        cur_num = g_radio_config.FMStatus.station;
        for (i = 0; i < g_radio_config.FMStatus.station_count; i++)
        {
            station_freq = *(pfreq + cur_num - 1);
            if ((station_freq >= min_freq[g_radio_config.band_mode]) && (station_freq
                    <= max_freq[g_radio_config.band_mode]))
            {
                //�ҵ���Ч̨���˳�
                break;
            }
            else
            {
                //���²���
                cur_num++;
                if (cur_num > g_radio_config.FMStatus.station_count)
                {
                    cur_num = 1;
                }
            }
        }
        //�ҵ���Ч��̨
        g_radio_config.FMStatus.station = cur_num;
        //ͨ����̨�Ż�ȡƵ��ֵ
        g_radio_config.FMStatus.freq = *(pfreq + g_radio_config.FMStatus.station - 1);

        //ˢ�µ�̨��
        radio_station_create_msg_view(2000);
    }
    else
    {
        //û�е�̨ҲҪ����Ƶ��
        radio_freq_tts_play(g_radio_config.FMStatus.freq);
    }

    //����Ƶ�㲥��(û�б����̨Ҳ��Ҫ��������Ƶ��)
    radio_set_freq(g_radio_config.FMStatus.freq);
    radio_set_mute(releaseMUTE);
    return ret;
}

//digital���С��g_radio_config.FMStatus.station_count
//  (Ԥ���̨����(������MAX_STATION_COUNT))
//  ��ô��digital�ͱ�ʾԤ���̨��������±ꡣ
//  ����digital��ʾ����Ƶ��(��λ50kHz)
//  (��ʱdigital����Сֵ��76M/50k=1520,
//  ����,g_digital_count�϶�������Ϊ4��)
app_result_e radio_digital_to_chanel(uint16 digital)
{
    //��̨Ƶ��
    uint32 chanel_freq;
    uint8 chanel_num;

    radio_set_mute(SetMUTE);

    if (digital <= g_radio_config.FMStatus.station_count)
    {
        //ѡ���̨��
        chanel_freq = *(pfreq + digital - 1);
        if ((chanel_freq >= min_freq[g_radio_config.band_mode]) && (chanel_freq <= max_freq[g_radio_config.band_mode]))
        {
            //����Ч��̨
            g_radio_config.FMStatus.station = (uint8) digital;
            //ͨ����̨�Ż�ȡƵ��ֵ
            g_radio_config.FMStatus.freq = chanel_freq;
            //��̨�Ž���
            radio_station_create_msg_view(2000);
            //����Ƶ�㲥��
            radio_set_freq(g_radio_config.FMStatus.freq);
        }
    }
    else if (g_digital_count >= 3)
    {
        //Ƶ������
        chanel_freq = digital * 50;
        if ((chanel_freq >= min_freq[g_radio_config.band_mode]) && (chanel_freq <= max_freq[g_radio_config.band_mode]))
        {
            g_radio_config.FMStatus.freq = chanel_freq;
            //����Ƶ����δ����Ƶ��
            chanel_num = whether_in_list(chanel_freq, pfreq);
            if (chanel_num == 0xff)
            {
                //û����Ԥ���̨����
                g_radio_config.FMStatus.station = 0;
                com_view_update(RADIO_MAIN_VIEW_PLAY);
            }
            else
            {
                //�л����Ƶ�����ѱ���Ƶ��
                g_radio_config.FMStatus.station = chanel_num;
                //��̨�Ž���
                radio_station_create_msg_view(2000);
            }
            //����Ƶ�㲥��
            radio_set_freq(g_radio_config.FMStatus.freq);
        }
    }
    else
    {
        ; //QAC
    }

    radio_set_mute(releaseMUTE);
    return RESULT_NULL;
}

//����ѡ̨
app_result_e radio_play_key_digital_sure(void)
{
    app_result_e deal_ret;
    //ѡ�е�̨
    uint16 digital_num = 0;
    uint16 curbit_multi = 1;
    uint8 i;
    for (i = 1; i < g_digital_count; i++)
    {
        curbit_multi = curbit_multi * 10;
    }
    for (i = g_digital_count; i > 0; i--)
    {
        digital_num = digital_num + g_digital_buf[g_digital_count - i] * curbit_multi;
        curbit_multi = curbit_multi / 10;
    }
    //digital_num�ĵ�λ��:100kHz

    deal_ret = radio_digital_to_chanel(digital_num * 2); //��50KHzΪ��λ
    //���buffer
    g_digital_count = 0;
    return deal_ret;
}

