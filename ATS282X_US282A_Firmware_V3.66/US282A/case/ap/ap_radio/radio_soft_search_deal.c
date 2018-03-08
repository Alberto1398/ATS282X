#include "app_radio.h"


uint32 Dec2Hex(uint32 h)
{
    uint32 d = 0;
    uint8 i = 0;
    while(h > 0)
    {
        d *= 10;
        d += h % 10;
        h /= 10;
        i++;
    }
    h = d;
    d = 0;
    while(i > 0)
    {
        d <<= 4;
        d += (h % 10);
        h /= 10;
        i--;
    }
    return d;
}


//���Զ���̨��ʼ��
//dir: ��̨����
//����ֵ: ����
static uint8 init_start_freq(FM_SeekDir_e dir)
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
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
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
    int8 n;
    int8 nLast;
    int8 n2;

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

//���ȫ�Զ���̨����
app_result_e deal_soft_auto_seek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space, tab_num;
    //��ʼ������Ƶ��
    uint32 seek_freq, end_freq;
    bool new_freq = FALSE;
    uint8 seek_times = 0;
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

    //��̨����
    space = 100;
    if (g_radio_config.band_mode == Band_Europe)
    {
        //ŷ�޲���, ��������Ϊ50K
        space = 50;
    }
    tab_num = 0;


    Seek_again:
    //��ȡ��ʼƵ��
    seek_freq = get_start_freq(dir);
    //��ȡ����Ƶ��
    end_freq = get_end_freq(dir);
    PRINT_DBG("�Զ���̨(��)");

    last_timestamp = sys_get_ab_timer();

    while (1)
    {
        new_freq = FALSE;
        //�жϵ�ǰƵ���Ƿ���Ч̨
        if (radio_set_softseek(seek_freq, dir) == TRUE)
        {
            if (!should_be_passed(seek_freq))
            {
                //������Ч��̨
                if (fliter_exist_freq(seek_freq, tab_num) == tab_num)
                {
                    if (_insert_tab(seek_freq) >= 0)
                    {
                        tab_num++;
                        new_freq = TRUE;
                    }
                }
            }
            if(new_freq == TRUE)
            {
                uint32 j = 0;

                libc_print("FM search ok=",Dec2Hex(seek_freq),2);
                //need_test_play
                radio_set_mute(releaseMUTE);
                while (1)
                {
                    ret = com_view_loop();
                    if (ret > RESULT_COMMON_RESERVE)
                    {
                        break;
                    }
                    sys_os_time_dly(1);
                    j++;
                    if (j >= 100)
                    {
                        break;
                    }
                }
                radio_set_mute(SetMUTE);
                sys_os_time_dly(1);
                last_timestamp = sys_get_ab_timer();
                if (ret > RESULT_COMMON_RESERVE)
                {
                    break;
                }
            }
        }

        //��̨�б�����
        if (tab_num >= MAX_STATION_COUNT)
        {
            break;
        }

        if (dir == DIR_UP)
        {
            if ((seek_freq + space) > end_freq)
            {
                //����ɨ��,�ص����ų���
                break;
            }
            else
            {
                //����Ƶ��
                seek_freq += space;
            }
        }
        else
        {
            if ((seek_freq - space) < end_freq)
            {
                //����ɨ��,�ص����ų���
                break;
            }
            else
            {
                //����Ƶ��
                seek_freq -= space;
            }
        }

        //ˢ��Ƶ��
        g_search_current_freq = seek_freq;
        com_view_update(RADIO_MAIN_VIEW_SOFT_SEARCH);

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
                keytone_play(KEYTONE_WARNING, KEYTONE_BLOCK);

                last_timestamp = cur_timestamp;
            }
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }
    seek_times++;
    if((seek_times < 2)&&(ret <= RESULT_COMMON_RESERVE))
    {
       goto Seek_again;
    }
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

//������Զ���̨����
//  һ������һ��Ƶ�����tune�����ж��Ƿ���̨
//  �ҵ�һ����̨��ֹͣ���߽���ƣ�����һ�֡�
//dir: ��̨����
app_result_e deal_soft_manual_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space;
    uint8 num;
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
    space = init_start_freq(dir);

    last_timestamp = sys_get_ab_timer();

    while (1)
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

        //����ˢ��,ÿ��ѭ����ˢ
        g_search_current_freq = g_radio_config.FMStatus.freq;
        com_view_update(RADIO_MAIN_VIEW_SOFT_SEARCH);

        //�жϵ�ǰƵ���Ƿ���Ч̨

        if (radio_set_softseek(g_radio_config.FMStatus.freq, dir) == TRUE)
        {
            if (!should_be_passed(g_radio_config.FMStatus.freq))
            {
                //����Ч��̨������˳�
                break;
            }
        }

        //�˳�����
        if (dir == DIR_UP)
        {
            //�߽����
            if (g_radio_config.FMStatus.freq >= max_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
            }
            //����һ�֣�δ�ҵ���Ч��̨���˳�
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //����Ƶ��
                g_radio_config.FMStatus.freq += space;
            }
        }
        else //DIR_DOWN
        {
            //�߽����
            if (g_radio_config.FMStatus.freq <= min_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
            }
            //����һ�֣�δ�ҵ���Ч��̨���˳�
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //����Ƶ��
                g_radio_config.FMStatus.freq -= space;
            }
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

//�����̨(�����õ�һ��Ƶ�ʣ�������û���ź�
//freq: Ƶ��
//dir: ����
bool radio_set_softseek(uint32 freq, uint8 dir)
{
    bool bret = TRUE;
    //seek paramter
    seek_param_t search_parm;
    //frequncy
    search_parm.freq = freq;
    //seek dir
    search_parm.dir = dir;

    bret = radio_send_msg(&search_parm, NULL, MSG_FMENGINE_SET_SOFTSEEK);

    //for RCP, ͬ����̨Ƶ��
    //radio_get_freq();
    g_engine_status.FM_CurrentFreq = freq;
    return bret;
}

