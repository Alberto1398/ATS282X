#include "app_radio.h"

//for border check
const uint32 end_border_freq[][2] =
{
    //dir_down,dir_up
    { MIN_US, MAX_US },
    { MIN_JP, MAX_JP },
    { MIN_EU, MAX_EU },
    { MIN_US, MAX_US }
};

const uint32 start_border_freq[][2] =
{
    //dir_down,dir_up
    { MAX_US, MIN_US },
    { MAX_JP, MIN_JP },
    { MAX_EU, MIN_EU },
    { MAX_US, MIN_US }
};

//�õ���ʼƵ������
uint32 get_start_freq(FM_SeekDir_e dir)
{
    uint32 start_freq;

    start_freq = start_border_freq[g_radio_config.band_mode][dir];

    return start_freq;
}

//����Ƶ��
uint32 get_end_freq(FM_SeekDir_e dir)
{
    uint32 stop_freq;
    //uint8 i;
    //��ȡ����Ƶ��
    //i = (dir & DIR_UP) >> 4;

    stop_freq = end_border_freq[g_radio_config.band_mode][dir];

    return stop_freq;
}

//��ʼ��ָ�򲥷�Ƶ�ʱ��ָ��
void init_point_freq(void)
{
    /* ��ǰ����Ƶ��*/
    if (g_radio_config.band_mode == Band_Japan)
    {
        pfreq = &g_radio_config.fmstation_jp[0];
    }
    else if (g_radio_config.band_mode == Band_Europe)
    {
        pfreq = &g_radio_config.fmstation_eu[0];
    }
    else
    {
        pfreq = &g_radio_config.fmstation_us[0];
    }
}

void radio_load_cfg(void)
{
    uint8 default_band;

    //ϵͳ������ȡ
    com_setting_comval_init(&g_comval);

    //��ȡradio ������Ϣ
    sys_vm_read(&g_radio_config, VM_AP_RADIO, sizeof(radio_config_t));

    if (g_radio_config.magic != VRAM_MAGIC(VM_AP_RADIO))
    {
        g_radio_config.magic = VRAM_MAGIC(VM_AP_RADIO);

        //��������Ԥ���̨�б�����
        libc_memset(g_radio_config.fmstation_us, 0, MAX_STATION_COUNT * 2 * 3);

        default_band = (uint8) com_get_config_default(RADIO_AP_ID_BAND);
        g_radio_config.FMStatus.freq = min_freq[default_band];
        g_radio_config.FMStatus.station = 0;
        g_radio_config.FMStatus.station_count = 0;
        g_radio_config.band_mode = (radio_band_e) default_band; // Band_China_US
        g_radio_config.FMStatus_eu.freq = MIN_EU;
        g_radio_config.FMStatus_jp.freq = MIN_JP;
        g_radio_config.FMStatus_us.freq = MIN_US;

        //Ĭ��δ����voice ����
        g_radio_config.enter_voice = NO_ERTER_VOICE;
        //Ĭ����̨��ʽ
        g_radio_config.seek_mode = (FM_SeekMode_e) com_get_config_default(RADIO_AP_ID_SEEKMODE);
        g_radio_config.hint_mode_cfg = (uint8) com_get_config_default(RADIO_AP_ID_HINT_MODE);

        sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));
    }

    /* ָ��ָ��ǰ���ŵ�Ƶ�ʱ���*/
    init_point_freq();
}

//����������Ϣ
void radio_save_cfg(void)
{
    /*����VM����*/
    sys_vm_write(&g_comval, VM_AP_SETTING, sizeof(comval_t));

    sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));
}

//Ӧ�ó�ʼ��
bool radio_app_init(void)
{
    //��ȡ������Ϣ (����vm��Ϣ)
    radio_load_cfg();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_RADIO, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(radio_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    applib_message_init();

    //��ʼ��ϵͳ��ʱ��
    sys_timer_init();

    com_rcp_init();//������xxx_rcp_var_init֮ǰ
    //����RCP�ص������б�
    radio_rcp_var_init();

    com_view_manager_init();

#ifdef SUPPORT_ASET_TEST
    if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        aset_test_init();
    }
#endif 

    com_set_dae_chan(TRUE, TRUE);
    return TRUE;
}

//�˳�app�Ĺ��ܺ��� ����������Ϣ
bool radio_app_exit(void)
{  
#ifdef SUPPORT_ASET_TEST
    if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        aset_test_exit();
    }
#endif 

    com_view_manager_exit();

    //����RCP�ص������б�
    radio_rcp_var_exit();
    //ɾ��ϵͳ��ʱ��
    sys_timer_exit();

    //������Ϣ��дVRAM
    radio_save_cfg();

    /*ִ��applib���ע������*/
    applib_quit();

    return TRUE;
}

//����������
//enter_mode:����ģʽ
app_result_e radio_scene_dispatch(app_param_e enter_mode)
{
    uint32 SD_clk_val;
    app_result_e scene_result = RESULT_NULL;

    g_radio_scene = FM_SCENE_PLAYING;
    g_first_enter_play = TRUE;
    SD_clk_val = act_readl(CMU_SDCLK);
    if ((SD_clk_val & (1 << 8)) != 0) //128��Ƶ--��Ƶ
    {
        //����SD_CLK�������FM�ĸ���
        act_writel(SD_clk_val | 0x0F, CMU_SDCLK);
    }
    com_set_mute(FALSE); //�������
    /* radioUI ����ѭ��*/
    while (g_radio_scene != FM_SCENE_EXIT)
    {
        switch (g_radio_scene)
        {
            /* ����RADIO ���ų���*/
            case FM_SCENE_PLAYING:
            scene_result = radio_scene_playing();
            g_first_enter_play = FALSE;
            if (scene_result > RESULT_COMMON_RESERVE)
            {
                if ((scene_result == RESULT_AUTO_SEARCH)
                        || (scene_result == RESULT_MANUAL_SEARCH_UP)
                        || (scene_result == RESULT_MANUAL_SEARCH_DOWN))
                {
                    //������̨����ͼ
                    g_radio_scene = FM_SCENE_SEARCH;
                }
                else
                {
                    g_radio_scene = FM_SCENE_EXIT;
                }
            }
            break;

            case FM_SCENE_SEARCH:
            if (scene_result == RESULT_AUTO_SEARCH)
            {
                scene_result = radio_scene_auto_search(DIR_UP);
            }
            else if (scene_result == RESULT_MANUAL_SEARCH_UP)
            {
                scene_result = radio_scene_manual_search(DIR_UP);
            }
            else if (scene_result == RESULT_MANUAL_SEARCH_DOWN)
            {
                scene_result = radio_scene_manual_search(DIR_DOWN);
            }
            else
            {
                ; //QAC
            }

            if ((scene_result > RESULT_COMMON_RESERVE) && (scene_result != RESULT_STOP_SEARCH))
            {
                //�˳� RADIO
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                //�ص���������ͼ
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            default:
            //�˳���������
            g_radio_scene = FM_SCENE_EXIT;
            break;
        }
    }
    act_writel(SD_clk_val, CMU_SDCLK);
    return scene_result;
}

//��ں���
int main(int argc, const char *argv[])
{
    app_result_e retval;
    bool result;
    app_param_e enter_mode;

    g_standby_exit_flag = (bool) ((argc & PARAM_S3_EXIT) != 0);
    argc = argc & (~PARAM_S3_EXIT);

    enter_mode = (app_param_e) (argc);

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    discrete_led_set(LED_ID_FM_PLAY, LED_ALWAYS_ON, NULL);
#endif

    g_enter_param = enter_mode;
    //����Ǵ���������
    if (enter_mode == PARAM_FROM_ALARM)
    {
        g_radio_from_alarm = 1;
    }

#if ((CASE_BOARD_TYPE == CASE_BOARD_DEMO) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2825) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823))//ʹ��ABģʽ����
    SPEAKER_D_AB_SWITCH(SPEAKER_A_PA_TYPE_AB);
    //��ΪLDOģʽ���Խ��Ͷ�FM�ĸ���
    act_writel((act_readl(VD15_DCDC_CTL) & (~VD15_DCDC_CTL_VD15_MODE_S1_MASK)), VD15_DCDC_CTL);
    // act_writel((act_readl(VD15_DCDC_CTL) | (0 << VD15_DCDC_CTL_VD15_MODE_S1_SHIFT)), VD15_DCDC_CTL);
#endif

    result = radio_app_init(); //��ʼ��

    if (!result)
    {
        //��ʼ��ʧ�ܣ�������Ӧ��
        retval = RESULT_MAIN_APP;
    }
    else
    {
        retval = radio_scene_dispatch(enter_mode); //����������
    }

    //AAͨ��ʱ���������ܱ��ص�����Ҫ����
    keytone_set_on_off(TRUE);

    if (retval == RESULT_SYSTEM_ENTER_S3)
    {
        PRINT_INFO("ap enter s3bt");
    }

    com_ap_switch_deal(retval);
    radio_app_exit(); //�˳�

#if ((CASE_BOARD_TYPE == CASE_BOARD_DEMO) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2825) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823))//��ԭΪDģʽ����
    SPEAKER_D_AB_SWITCH(SPEAKER_A_PA_TYPE_D);
    //�л�DC-DCģʽ
    act_writel((act_readl(VD15_DCDC_CTL) & (~VD15_DCDC_CTL_VD15_MODE_S1_MASK)), VD15_DCDC_CTL);
    act_writel((act_readl(VD15_DCDC_CTL) | (1 << VD15_DCDC_CTL_VD15_MODE_S1_SHIFT)), VD15_DCDC_CTL);
#endif

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    discrete_led_set(LED_ID_FM_PLAY, LED_ALWAYS_OFF, NULL);
#endif
    g_ap_switch_var.switch_to_radio_by_apkapp = FALSE;

    return retval;
}

uint8 radio_tts_fm_freq(uint16 value, bool europe_fm, uint8 *tts_sec_ids)
{
    uint8 section_cnt = 0;
    uint8 hundred, ten, one, dot, dot_euro;
    bool number_bitmap = 0x00; //bit 3 ~ 0 �ֱ�Ϊǧ��ʮ��

    dot_euro = (uint8) (value % 2) * 5; //50KhzΪ��λ
    value = value / 2; //100KhzΪ��λ
    hundred = (uint8) (value / 1000);
    ten = (uint8) ((value % 1000) / 100);
    one = (uint8) ((value % 100) / 10);
    dot = (uint8) (value % 10);

    //FM
    tts_sec_ids[section_cnt] = TTS_SECTION_CHAR_FM;
    section_cnt++;

    //��λ������
    if (hundred > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + hundred;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_HUNDRED;
        section_cnt++;
        number_bitmap |= 0x04;
    }

    //ʮλ������
    if (ten > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + ten;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_TEN;
        section_cnt++;
        number_bitmap |= 0x02;
    }
    else if ((number_bitmap & 0x04) != 0) //��λ����Ϊ0
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0;
        section_cnt++;
    }
    else
    {
        ; //for QAC
    }

    //��λ������
    if (one > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + one;
        section_cnt++;
    }

    //��
    tts_sec_ids[section_cnt] = TTS_SECTION_DOT;
    section_cnt++;

    //100Khz����
    tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + dot;
    section_cnt++;
    if (europe_fm == TRUE)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + dot_euro;
        section_cnt++;
    }

    tts_sec_ids[section_cnt] = 0xff; //��ӽ�����־
    return section_cnt;
}

//Ƶ�� TTS ����
void radio_freq_tts_play(uint32 freq)
{
    uint16 tmp_freq = (uint16) (freq / 50);

    radio_tts_fm_freq(tmp_freq, (bool) (g_radio_config.band_mode == Band_Europe), g_freq_tts_sec_ids);

    if ((g_neednot_freq_tts_play == FALSE)
            && (g_standby_exit_flag == FALSE))
    {
        com_tts_state_play(TTS_MODE_SECLIST, g_freq_tts_sec_ids);
    }

    g_standby_exit_flag = FALSE;
}
