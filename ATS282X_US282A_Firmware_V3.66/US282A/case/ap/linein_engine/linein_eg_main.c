#include "app_linein_eg.h"
#include "audio_device.h"

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT         (1)

#ifdef WAVES_ASET_TOOLS
    waves_t g_waves;;
#endif

output_channel_e g_output_channel_config;

//globle variable
//�����м�����
void *g_mmm_pp_handle = NULL;

//�ؼ�����ֵ
app_result_e g_linein_eg_result;

//����״̬�����ṹ��ָ��
linein_eg_status_t g_eg_status[2];
linein_eg_status_t *g_eg_status_p;

//eq data
mmm_pp_eq_info_t g_eq_info;
//private_msg_t g_cur_msg;

//for clk adjust
uint32 g_save_freq = 0;

mmm_pp_ainout_setting_t g_ainout_param;

//lineinͨ��ѡ��
uint8 g_linein_channel_sel;

//app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t linein_eg_app_timer_vector[APP_TIMER_COUNT];

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
bool g_stop_by_tts_flag;

//for dae adjust freq
dae_config_t *g_dae_cfg_shm;

//if need open play
uint32 g_need_play;

//��ȡ������Ϣ
void _load_cfg(void)
{
    //for linein cheel
    g_linein_channel_sel = (uint8) com_get_config_default(LINEIN_CHANNEL_SEL);
    g_output_channel_config = (uint8)com_get_config_default(OUTPUT_CHANNEL_CONFIG);
}

void _save_cfg(void)
{
    /*����VM����*/
}

static void _set_ainout_param(void)
{
    g_ainout_param.input_type = MMM_PP_LINEIN_IN;
    g_ainout_param.asrc_index = K_OUT1_P2_IN_U1;
    g_ainout_param.dac_sample = 48;
    g_ainout_param.dac_chanel = 1;
    g_ainout_param.asrc_mode_sel = 3;
    g_ainout_param.adc_sample = 48;
    g_ainout_param.adc_insel = AI_SOURCE_LINEIN;
    g_ainout_param.ain_gain = (uint8) com_get_config_default(LINEIN_GAIN_INPUT);
    g_ainout_param.adc_gain = (uint8) com_get_config_default(LINEIN_GAIN_ADC);
}

#ifdef WAVES_ASET_TOOLS
void waves_init(void)
{
    if (STUB_PC_TOOL_WAVES_ASET_MODE == g_app_info_state_all.stub_pc_tools_type)
    {
        g_waves.tuning_status = TUNING;
    }
    else if (STUB_PC_TOOL_UNKOWN == g_app_info_state_all.stub_pc_tools_type)
    {
        g_waves.tuning_status = NO_TUNING;
    }
    g_waves.bin_number = g_app_info_state_all.bin_number;
    g_waves.input_para_enable = (uint8) com_get_config_default(SETTING_APP_SUPPORT_WAVES_INPUT_PARAM); 
}
#endif

int _app_init(void)
{
    uint8 init_fsel_ret = TRUE;
    char *mmm_name =
    { "mmm_pp.al" };
    
#ifdef WAVES_ASET_TOOLS
    waves_init();
#endif

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_LINEIN_EG, APP_TYPE_CONSOLE);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL8, FREQ_NULL);

    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ�� applib ��Ϣģ��
    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(linein_eg_app_timer_vector, APP_TIMER_COUNT);

    //�����м������
    if (g_linein_channel_sel == LINEIN_CH_A)
    {
    }
    else
    {
        sys_load_mmm(mmm_name, TRUE);
        _set_ainout_param();
    }

    com_set_sound_out(SOUND_OUT_START);

    if (g_need_play != 0)
    {
        if (g_linein_channel_sel == LINEIN_CH_A)
        {
            init_fsel_ret = linein_eg_aa_play();
            g_eg_status_p->play_status = linein_play_status_play; //���ò���״̬
        }
        else
        {
            init_fsel_ret = play();
        }
    }

    //���ų���
    if (init_fsel_ret == FALSE)
    {
        _error_handle();
    }
    else
    {
    }

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);
    return 0;
}

bool _app_deinit(void)
{
    com_set_sound_out(SOUND_OUT_STOP);
    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL8, FREQ_NULL);

    //save config дvram
    _save_cfg();
    if (g_linein_channel_sel != LINEIN_CH_A)
    {
        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);
        //ж�ؽ�������
        sys_free_mmm(TRUE);

    }
    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    //ִ��applib���ע������
    applib_quit();

    return TRUE;
}

int main(int argc, const char *argv[])
{

    //if need play
    g_need_play = argc;

    g_eg_status_p = &g_eg_status[0];
    g_eg_status_p->play_status = linein_play_status_stop;

    //���������ѯ����ʱ���뱣֤ǰ̨����ȡ״̬ʱ�ǿ��õ�
    g_eg_status_p = sys_share_query_creat(APP_ID_LINEIN_EG, g_eg_status_p, sizeof(linein_eg_status_t));
    if (g_eg_status_p == NULL)
    {
        PRINT_ERR("linein share query create fail!\n");
        while (1)
        {
            ;//QAC
        }
    }

    //get for dae freqency
    //����DAE���ò����������ڴ�
    g_dae_cfg_shm = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (g_dae_cfg_shm == NULL)
    {
        PRINT_ERR("dae_cfg shm not exist!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    //��ʼ��
    _app_init();

    //��Ϣ����ѭ��
    g_linein_eg_result = linein_eg_control_block(); //app���ܴ���

    //���ٹ����ѯ
    if (sys_share_query_destroy(APP_ID_LINEIN_EG) == -1)
    {
        PRINT_ERR("linein share query destroy fail!\n");
        while (1)
        {
            ;//QAC
        }
    }

    //�˳�
    _app_deinit();

    return g_linein_eg_result;
}
