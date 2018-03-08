#include "App_fmengine.h"

#ifdef WAVES_ASET_TOOLS
    waves_t g_waves;
#endif

output_channel_e g_output_channel_config;

//globle variable
//FM ����״̬��Ϣ
Engine_Status_t g_fmengine_status;

//����������Ϣ
FMEngine_cfg_t g_fmengine_cfg;

//FM ģ������״̬
Module_Status_e g_module_status;

//fmͨ��ѡ��
uint8 g_fm_chanel;

//globle variable
//�����м�����
void *g_mmm_pp_handle = NULL;

mmm_pp_ainout_setting_t g_ainout_param;

//��TTS��������ͣ������TTS������������Ҫ�ָ�����
bool g_stop_by_tts_flag;

//for dae adjust freq
dae_config_t *g_dae_cfg_shm;

void fmengine_load_cfg(void)
{
    //��ȡFM ����������Ϣ
    sys_vm_read(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(FMEngine_cfg_t));
    if (g_fmengine_cfg.magic != VRAM_MAGIC(VM_AP_FMENGINE))
    {
        g_fmengine_cfg.magic = VRAM_MAGIC(VM_AP_FMENGINE);
        g_fmengine_cfg.fm_threshold = (uint8) com_get_config_default(FMENGINE_AP_ID_SEEKTH);
        /*����VM����*/
        sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(g_fmengine_cfg));
    }
    g_fm_chanel = (uint8) com_get_config_default(FMENGINE_CHANNEL_SEL); //fmͨ��ѡ��;

	g_output_channel_config = (uint8)com_get_config_default(OUTPUT_CHANNEL_CONFIG);
}

void fmengine_save_cfg(void)
{
    /*����VM����*/
    sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(g_fmengine_cfg));
}

static void _set_ainout_param(void)
{
    g_ainout_param.input_type = MMM_PP_FM_IN;
    g_ainout_param.asrc_index = K_OUT1_P2_IN_U1;
    g_ainout_param.dac_sample = 48;
    g_ainout_param.dac_chanel = 1;
    g_ainout_param.asrc_mode_sel = 3;
    g_ainout_param.adc_sample = 48;
    g_ainout_param.adc_insel = AI_SOURCE_FM;
    g_ainout_param.ain_gain = (uint8) com_get_config_default(FMENGINE_GAIN_INPUT);
    g_ainout_param.adc_gain = (uint8) com_get_config_default(FMENGINE_GAIN_ADC);
}

void get_fm_init_param(FM_init_parm_t *p_fm)
{
    p_fm->freq_mode =  (uint8) com_get_config_default(FMENGINE_FREQ_32K_13M);
    p_fm->gpio_level = (uint8) com_get_config_default(FMENGINE_CLK_PAD_LEVEL);
    libc_print("fm-init-parm:",((uint16)p_fm->freq_mode << 8) + p_fm->gpio_level, 2);
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

//��ȡ������Ϣ����װFM����, ��װ�м��(�����ADDA�ķ�ʽ)
/*******************************************************************************/
int fmengine_init(void)
{
    int app_init_ret = 0;
    FM_init_parm_t fm_arg;
    char *mmm_name =
    { "mmm_pp.al" };
    
#ifdef WAVES_ASET_TOOLS
    waves_init();
#endif


    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_FMENGINE, APP_TYPE_CONSOLE);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL8, FREQ_NULL);

    //��ȡ������Ϣ (����vm��Ϣ)
    fmengine_load_cfg();
    get_fm_init_param(&fm_arg);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init();

    //��װfm  ����
    app_init_ret = sys_drv_install(DRV_GROUP_FM, &fm_arg, "drv_fm.drv");

    //������װ�ɹ�
    if (app_init_ret == 0)
    {
        //ģ����δ��ʼ�����账��MSG_FMENGINE_INIT  ��Ϣ����г�ʼ��
        g_module_status = MODULE_NO_INIT;
    }
    //for eq set ad da chanel set
    if (g_fm_chanel == 1)
    {
        //��ʼ���м����
        //�����м������
        sys_load_mmm(mmm_name, TRUE);
        //app_init_ret = mmm_pp_cmd(&g_mmm_pp_handle, MMM_PP_OPEN, (unsigned int) NULL);
        _set_ainout_param();
    }

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

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

    return app_init_ret;
}

//ж���м��������vram, ж��fm����
bool fmengine_deinit(void)
{
    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL8, FREQ_NULL);
    if (g_fm_chanel == 1)
    {
        //�ر��м���豸
        //mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
        //ж�ؽ�������
        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);
        sys_free_mmm(TRUE);
    }

    //save config дvram
    fmengine_save_cfg();

    //ж��fm ����
    sys_drv_uninstall(DRV_GROUP_FM);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    //ִ��applib���ע������
    applib_quit();

    return TRUE;
}

//��ں���
int main(int argc, const char *argv[])
{
    app_result_e result = RESULT_NULL;
    //��ʼ��
    if (fmengine_init() == 0)
    {
        //����հ�װʱ��Ĭ��Ϊ����
        change_engine_state(ENGINE_STATE_PLAYING);
        result = fmengine_control_block();
    }
    fmengine_deinit(); //�˳�

    return result;
}

