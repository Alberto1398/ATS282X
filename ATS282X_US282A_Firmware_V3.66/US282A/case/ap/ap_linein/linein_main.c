/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������LineIn�����,����
 * ���ߣ�Gary Wang
 ********************************************************************************/
#include "app_linein.h"

//��ǰ����״̬��Ϣ
linein_eg_status_t g_linein_eg_status;

//ui����������Ϣ
linein_config_t g_linein_config;

//ϵͳȫ�ֱ���
comval_t g_comval;

bool g_neednot_tts_play;

//���ڳ�ʼ������Щ�¼����Բ�������
bool g_linein_init_flag;

//for energy detect use 
int8 g_energy_detect_cnt_timer = -1;
//for period cnt save 
uint8 g_period_cnt;

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     (2)

//Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t linein_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

void _load_cfg(void)
{
    //ϵͳ������ȡ
    com_setting_comval_init(&g_comval);

    //linein ������ȡ
    sys_vm_read(&g_linein_config, VM_AP_LINEIN, sizeof(linein_config_t));

    if (g_linein_config.magic != VRAM_MAGIC(VM_AP_LINEIN))
    {
        g_linein_config.magic = VRAM_MAGIC(VM_AP_LINEIN);
        //eq mode
        g_linein_config.eq_type = EQ_NOR;
        g_linein_config.linin_open = (uint8) com_get_config_default(SETTING_APP_SUPPORT_LINEIN);
        sys_vm_write(&g_linein_config, VM_AP_LINEIN, sizeof(linein_config_t));
    }
}

void _save_cfg(void)
{
    /*����VM����*/
    sys_vm_write(&g_linein_config, VM_AP_LINEIN, sizeof(linein_config_t));
    sys_vm_write(&g_comval, VM_AP_SETTING, sizeof(comval_t));
}

bool _app_init(void)
{
    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_LINEIN, APP_TYPE_GUI);

    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(linein_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //������init_app_timers �������
    sys_timer_init();

    com_rcp_init();//������xxx_rcp_var_init֮ǰ

    //��ʼ��RCP�ص�����
    linein_rcp_var_init();

    com_view_manager_init();

#ifdef SUPPORT_ASET_TEST
    if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        aset_test_init();
    }
#endif

    return TRUE;
}

//�˳�app�Ĺ��ܺ���,����������Ϣ
bool _app_deinit(void)
{   
#ifdef SUPPORT_ASET_TEST
    if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        aset_test_exit();
    }
#endif

    //ע��RCP�ص�����
    linein_rcp_var_exit();

    com_view_manager_exit();

    sys_timer_exit();

    //ִ��applib���ע������
    applib_quit();

    //дvram
    _save_cfg();

    return TRUE;
}

int main(int argc, const char *argv[])
{
    app_result_e retval;

    g_standby_exit_flag = (bool) ((argc & PARAM_S3_EXIT) != 0);
    argc = argc & (~PARAM_S3_EXIT);

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    discrete_led_set(LED_ID_AUX_PLAY, LED_ALWAYS_ON, NULL);
#endif

    g_customer_state &= ~CUSTOMER_ESD_WAKEUP;

    //��ʼ��
    if (_app_init() == TRUE)
    {
        
        //for energy detcet use
        g_period_cnt = g_comval.dae_cfg.period_count;
        g_comval.dae_cfg.period_count=1; 
        com_update_dae_config(&g_comval.dae_cfg);
        g_energy_detect_cnt_timer = set_app_timer(APP_TIMER_ATTRB_COM_CONTROL, 5000, linein_modify_timer_proc);
        retval = scene_linein(); //����������
        if(g_energy_detect_cnt_timer !=-1)
        {
        	kill_app_timer(g_energy_detect_cnt_timer);
        	g_energy_detect_cnt_timer = -1;
        }
    }
    else
    {
        //��ʾ����(��ʾERRO)
        retval = RESULT_NEXT_FUNCTION;
    }

    //AAͨ��ʱ���������ܱ��ص�����Ҫ����
    keytone_set_on_off(TRUE);

    if (retval == RESULT_SYSTEM_ENTER_S3)
    {
        PRINT_INFO("ap enter s3bt");
    }

    com_ap_switch_deal(retval);

    //�˳�
    _app_deinit();

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    discrete_led_set(LED_ID_AUX_PLAY, LED_ALWAYS_OFF, NULL);
#endif

    return retval;
}
