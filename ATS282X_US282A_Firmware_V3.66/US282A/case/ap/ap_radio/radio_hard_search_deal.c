#include "app_radio.h"

//���Զ���̨��ʼ��
//dir: ��̨����
//����ֵ: ����
static __section__(".text.bank2") uint8 init_start_freq(FM_SeekDir_e dir)
{
    uint8 space;
    //��̨����
    space = 100;
    //ŷ��Ƶ��,����Ϊ50
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲��λ������û�ģʽ����������Ϊ50K
        space = 50;
    }

    //���ó�ʼ�ж�Ƶ��
    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
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
        if (g_radio_config.FMStatus.freq> min_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //��СƵ���л������
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }
    return space;
}

//��һ��Ƶ���Ƿ���Auto_tab[0..cur_max - 1]�д���
//����: Auto_tab���±�
static uint8 fliter_exist_freq(uint32 input_freq, uint8 cur_max)
{
    uint8 number;
    for (number = 0; number < cur_max; number++)
    {
        if (input_freq == Auto_tab[(number)])
        {
            break;
        }
    }
    return number;
}

//����Auto_tab
//ǰ�᣺Auto_tab�ǰ��մ�С�����˳�� ��ʼ��ʱ��������0
//����ֵ���������ɹ������ز���λ�õ�Auto_tab���±�
//          ����Ѿ���Auto_tab���ڣ�����-1
//          �������������-2
//          �����������-3
static int32 _insert_tab(uint32 freq)
{
    uint8 n;
    uint8 nLast;
    uint8 n2;

    for (n = 0; n < ARR_COUNT(Auto_tab); n++)
    {
        if (Auto_tab[n] == 0)
        {
            Auto_tab[n] = freq;
            return (int32) n;
        }

        if (Auto_tab[n] == freq)
        {
            //�������Ӧ�ò����ڣ���Ϊ�ڵ��ñ�����֮ǰ���Ѿ������˼��
            return -1;
        }

        if (Auto_tab[n] < freq)
        {
            continue;
        }

        //Auto_tab[n] > freq
        //�����ƶ�

        //���һ����0�����±�
        for (nLast = ARR_COUNT(Auto_tab) - 1; nLast >= n; nLast--)
        {
            if (Auto_tab[nLast] != 0)
            {
                break;
            }
        }
        if (nLast == (ARR_COUNT(Auto_tab) - 1))
        {
            //�Ѿ��岻��ȥ��, ����
            return -2;
        }

        for (n2 = nLast; n2 >= n; n2--)
        {
            Auto_tab[n2 + 1] = Auto_tab[n2];
        }
        Auto_tab[n] = freq;
        return (uint32) n;
    }

    //����ִ�е�����
    return -3;
}

static bool _is_search_over(uint32 freq, uint32 end, FM_SeekDir_e dir)
{
    if (((freq >= end) && (dir == DIR_UP)) || ((freq <= end) && (dir == DIR_DOWN)))
    {
        return TRUE;
    }

    return FALSE;
}

//Ӳ��ȫ�Զ���̨����
app_result_e deal_hard_auto_seek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 tab_num, i;
    bool result, need_restart = TRUE;
    uint32 current_freq, end_freq;
    uint8 space;
    uint32 last_timestamp, cur_timestamp;

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    if (g_radio_config.hint_mode_cfg == 1)
    {
        led_duty_t fm_link_duty;
        fm_link_duty.cycle_count = -1;
        fm_link_duty.duty_on_time = 250;
        fm_link_duty.duty_off_time = 250;
        discrete_led_set(LED_ID_FM_PLAY, LED_TWINKLE, &fm_link_duty);
    }

#endif

    //���õ�ǰ����, ��Ҫ��ȷ��space
    radio_set_band(g_radio_config.band_mode);
    //��ʼƵ��
    current_freq = get_start_freq(dir);
    //����Ƶ��
    end_freq = get_end_freq(dir);
    //��̨����
    tab_num = 0;

    //��̨����
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲���, ��������Ϊ50K
        space = 50;
    }
    else
    {
        space = 100;
    }

    PRINT_DBG("�Զ���̨(Ӳ)");

    //��ʾ��ǰƵ��
    g_search_current_freq = current_freq;
    com_view_update(RADIO_MAIN_VIEW_HARD_SEARCH);

    last_timestamp = sys_get_ab_timer();

    while (1)
    {
        if (need_restart == TRUE)
        {
            result = radio_set_hardseek(current_freq, dir);
            need_restart = FALSE;
            if (!result)
            {
                //Ӳ����̨����ʧ��
                break;
            }
        }

        //Ӳ����̨�����ɹ�,  ȡ�Ƿ���ɱ�־
        result = radio_get_seekflag();

        if (result == TRUE)
        {
            //��ȡ��ǰƵ��
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;

            if ((g_engine_status.STC_flag == HARDSEEK_NOT_START) || (_is_search_over(current_freq, end_freq, dir)
                    == TRUE))
            {
                //������̨���
                radio_break_hardseek();
                break;
            }

            if ((g_engine_status.STC_flag == HARDSEEK_COMPLETE) || (g_engine_status.STC_flag
                    == HARDSEEK_COMPLETE_NOT_VALID))
            {
                if ((g_engine_status.STC_flag == HARDSEEK_COMPLETE) && (!should_be_passed(current_freq)))
                {
                    //������Ч��̨
                    i = fliter_exist_freq(current_freq, tab_num);
                    if ((i == tab_num) && (_insert_tab(current_freq) >= 0))
                    {
                        tab_num++;
                    }
                }

                //��ʾ��ǰƵ��
                g_search_current_freq = current_freq;
                com_view_update(RADIO_MAIN_VIEW_HARD_SEARCH);

                if (dir == DIR_UP)
                {
                    current_freq += space;
                }
                else
                {
                    current_freq -= space;
                }

                if ((_is_search_over(current_freq, end_freq, dir) == TRUE))
                {
                    //������̨���
                    radio_break_hardseek();
                    break;
                }
                else if (g_engine_status.STC_flag == HARDSEEK_COMPLETE)
                {
                    uint32 j = 0;
                    radio_break_hardseek();
                    //for play
                    radio_set_mute(releaseMUTE);
                    while (1)
                    {
                        ret = com_view_loop();
                        if (ret > RESULT_COMMON_RESERVE)
                        {
                            goto hard_serach_over;
                        }
                        sys_os_time_dly(1);
                        j++;
                        if (j >= 100)
                        {
                            break;
                        }
                    }

                    radio_set_mute(SetMUTE);
                    need_restart = TRUE;
                    last_timestamp = sys_get_ab_timer();
                    continue;
                }
                else
                {
                    ;//
                }
            }

            //δ������Ƶ��, ��������һ����̨
            need_restart = TRUE;
        }

        //��̨�б�����
        if (tab_num >= MAX_STATION_COUNT)
        {
            radio_break_hardseek();
            break;
        }

        ret = com_view_loop();
        if (ret > RESULT_COMMON_RESERVE)
        {
            radio_break_hardseek();
            break;
        }
        if (g_radio_config.hint_mode_cfg == 0)
        {
            cur_timestamp = sys_get_ab_timer();
            if ((cur_timestamp - last_timestamp) >= 1500)
            {
                //��ʱ�ж���̨���Ա�֤������ʾ��ʱ�л�BANK����Ӱ�쵽��̨Ч��
                radio_break_hardseek();
                radio_get_freq();
                current_freq = g_engine_status.FM_CurrentFreq;
                need_restart = TRUE;
                keytone_play(KEYTONE_WARNING, KEYTONE_BLOCK);

                last_timestamp = cur_timestamp;
            }
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    hard_serach_over:

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    if (g_radio_config.hint_mode_cfg == 1)
    {
        discrete_led_set(LED_ID_FM_PLAY, LED_ALWAYS_ON, NULL);
    }
#endif
    //�����̨��
    g_radio_config.FMStatus.station_count = tab_num;
    return ret;
}

//Ӳ�����Զ���̨����
//dir: ��̨����
app_result_e deal_hard_manual_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    bool result, need_restart = TRUE;
    uint32 current_freq, end_freq;
    uint8 num;
    uint8 space;
    uint32 last_timestamp, cur_timestamp;

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    if (g_radio_config.hint_mode_cfg == 1)
    {
        led_duty_t fm_link_duty;
        fm_link_duty.cycle_count = -1;
        fm_link_duty.duty_on_time = 250;
        fm_link_duty.duty_off_time = 250;
        discrete_led_set(LED_ID_FM_PLAY, LED_TWINKLE, &fm_link_duty);
    }
#endif

    //���õ�ǰ����, ��Ҫ��ȷ��space
    radio_set_band(g_radio_config.band_mode);

    init_start_freq(dir);
    //��ʼƵ��
    current_freq = g_radio_config.FMStatus.freq;
    //����Ƶ��
    end_freq = get_end_freq(dir);

    //��̨����
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲���, ��������Ϊ50K
        space = 50;
    }
    else
    {
        space = 100;
    }

    last_timestamp = sys_get_ab_timer();

    while (1)
    {
        if (need_restart == TRUE)
        {
            //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
            //�����µ�̨��
            num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
            if (num == 0xff)
            {
                g_radio_config.FMStatus.station = 0;
            }
            else
            {
                g_radio_config.FMStatus.station = num;
            }
            //����ʾ��ʼ������Ƶ��
            //g_search_current_freq = g_radio_config.FMStatus.freq;
            //com_view_update(RADIO_MAIN_VIEW_HARD_SEARCH);

            //������һ��Ӳ��seek
            result = radio_set_hardseek(current_freq, dir);
            need_restart = FALSE;
            if (!result)
            {
                //Ӳ����̨����ʧ��,���ص����ų���
                break;
            }
        }

        //Ӳ����̨�����ɹ�,  ȡ�Ƿ���ɱ�־
        result = radio_get_seekflag();

        if (result == TRUE)
        {
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;

            if ((g_engine_status.STC_flag == HARDSEEK_NOT_START) || (_is_search_over(current_freq, end_freq, dir)
                    == TRUE))
            {
                //������̨��ɻ�seek fail, �˳�
                radio_break_hardseek();
                break;
            }

            if ((g_engine_status.STC_flag == HARDSEEK_COMPLETE) || (g_engine_status.STC_flag
                    == HARDSEEK_COMPLETE_NOT_VALID))
            {
                //�ѵ�̨��
                if ((g_engine_status.STC_flag == HARDSEEK_COMPLETE) && (!should_be_passed(current_freq)))
                {
                    //������̨
                    radio_break_hardseek();
                    break;
                }

                //�Ǳ���ĸ���
                if (dir == DIR_UP)
                {
                    current_freq += space;
                }
                else
                {
                    current_freq -= space;
                }
                if (_is_search_over(current_freq, end_freq, dir) == TRUE)
                {
                    //������̨���
                    radio_break_hardseek();
                    break;
                }
            }

            //δ������Ƶ��, ��������һ����̨
            need_restart = TRUE;
        }
        else
        {
            //��ʾ�����������ĸ�Ƶ����
            //������������I2CͨѶ��Ҳ������SPIͨѶ��
            //����Щ�����ϣ���ЩͨѶ�������̨����������ס��
            //���ԣ�������ѡ��
#if 0
            radio_get_freq();
            g_search_current_freq = g_engine_status.FM_CurrentFreq;
            com_view_update(RADIO_MAIN_VIEW_HARD_SEARCH);
#endif
        }

        ret = com_view_loop();
        if (ret > RESULT_COMMON_RESERVE)
        {
            break;
        }
        if (g_radio_config.hint_mode_cfg == 0)
        {
            cur_timestamp = sys_get_ab_timer();
            if ((cur_timestamp - last_timestamp) >= 1500)
            {
                //��ʱ�ж���̨���Ա�֤������ʾ��ʱ�л�BANK����Ӱ�쵽��̨Ч��
                radio_break_hardseek();
                radio_get_freq();
                current_freq = g_engine_status.FM_CurrentFreq;
                need_restart = TRUE;
                keytone_play(KEYTONE_WARNING, KEYTONE_BLOCK);

                last_timestamp = cur_timestamp;
            }
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    if (g_radio_config.hint_mode_cfg == 1)
    {
        discrete_led_set(LED_ID_FM_PLAY, LED_ALWAYS_ON, NULL);
    }
#endif

    return ret;
}

//Ӳ����̨
//freq: Ӳ����̨��ʼƵ��
//dir: ��̨����
bool radio_set_hardseek(uint32 freq, uint8 dir)
{
    //seek paramter
    seek_param_t search_parm;
    //frequncy
    search_parm.freq = freq;
    //seek dir
    search_parm.dir = dir;

    return radio_send_msg(&search_parm, NULL, MSG_FMENGINE_SET_HARDSEEK);

}

//��ȡӲ����̨�Ƿ���ɱ�־
bool radio_get_seekflag(void)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, &g_engine_status.STC_flag, MSG_FMENGINE_AUTO_SEEK_OVER);

    if (g_engine_status.STC_flag == HARDSEEK_DOING)
    {
        bret = FALSE;
    }
    return bret;
}

//�ж�Ӳ����̨����
bool radio_break_hardseek(void)
{
    return radio_send_msg(NULL, NULL, MSG_FMENGINE_SET_SEEKBREAK);
}
