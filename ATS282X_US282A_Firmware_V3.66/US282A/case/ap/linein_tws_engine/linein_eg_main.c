/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������line in tws�Ƹ�����Ӧ����ڣ�����Ӧ�ó�ʼ����Ӧ���˳��ȡ�
 * ���ߣ�
 ********************************************************************************/

#include "linein_engine.h"


/*ȫ�ֱ�������*/
//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     3

app_timer_t btplay_timer_vector[APP_TIMER_COUNT];

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
OS_STK *ptos = (OS_STK *) AP_BACK_LOW_STK_POS;
INT8U prio = AP_BACK_LOW_PRIO;

//for dae adjust freq
dae_config_t *g_dae_cfg_shm;

//for chage current save
uint32 g_cha_current;

uint8 g_cha_sta;

//for chage current save
uint32 g_cha_need_restore;

//for count 
uint32 g_check_count;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
//�����жϿյ��м���������ٵ�btstack
uint8 send_once_cmd=0;
uint8 second_empty_flag=0;
uint8 second_empty_flag2 =0;
//  uint16 cur_buffer_time=0;
uint8 tws_or_notws_flag=0;
int8 clear_filter_timer_id = -1;
sbc_obj_t sbc_obj; 
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    //ȫ�ֱ�����0��Чֵ��ʼ��
    btplay_gl_var.codec_flag = FALSE;
    btplay_gl_var.tts_play_flag = FALSE;
    btplay_gl_var.media_type = A2DP_CODEC_NONA2DP;

    btplay_gl_var.aac_delay_start = (uint8) com_get_config_default(BTPLAY_AAC_PLAY_DELAY_START);
    btplay_gl_var.aac_delay_lowest = (uint8) com_get_config_default(BTPLAY_AAC_PLAY_DELAY_LOWEST);
    btplay_gl_var.sbc_delay_start = (uint8) com_get_config_default(BTPLAY_SBC_PLAY_DELAY_START);
    btplay_gl_var.sbc_delay_lowest = (uint8) com_get_config_default(BTPLAY_SBC_PLAY_DELAY_LOWEST);
	btplay_gl_var.output_channel_config = (uint8)com_get_config_default(OUTPUT_CHANNEL_CONFIG);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    btplay_gl_var.tws_m_output_channel = (uint8) com_get_config_default(BTPLAY_TWS_M_PLAY_OUTPUT_CHANNEL);
    btplay_gl_var.tws_s_output_channel = (uint8) com_get_config_default(BTPLAY_TWS_S_PLAY_OUTPUT_CHANNEL);
#endif

    g_p_play_pipe = (bt_stack_pipe_t *) sys_shm_mount(SHARE_MEM_ID_BTPLAYPIPE);
    if (g_p_play_pipe == NULL)
    {
        PRINT_ERR("btplay pipe shm not exist!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    g_p_bt_stack_cur_info = (bt_stack_info_t *) sys_shm_mount(SHARE_MEM_ID_BTSTACK_STATUS);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    g_p_tws_info = (bt_stack_twsinfo_t *) sys_shm_mount(SHARE_MEM_ID_TWS_INFO);
    if (g_p_tws_info == NULL)
    {
        PRINT_ERR("tws_info shm not exist!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

#endif
    if (g_p_bt_stack_cur_info == NULL)
    {
        PRINT_ERR("btstack status shm not exist!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    g_p_btplay_info = &g_btplay_info[0];

    if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status == A2DP_STATUS_NONE)
    {
        g_p_btplay_info->status = BTPLAY_IDLE;
    }
    else
    {
        g_p_btplay_info->status = BTPLAY_STOP;
    }

    //���������ѯ����ʱ���뱣֤ǰ̨����ȡ״̬ʱ�ǿ��õ�
    g_p_btplay_info = sys_share_query_creat(APP_ID_LINEIN_EG, g_p_btplay_info, sizeof(btplay_info_t));
    if (g_p_btplay_info == NULL)
    {
        PRINT_ERR("btplay share query create fail!!");
        while (1)
        {
            ; //nothing for QAC
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

}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu ����VRAM����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _save_var(void)
{

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

/******************************************************************************/
/*!
 * \par  Description:
 *    applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_init(void)
{
    /*����VM����*/
    _read_var();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_LINEIN_EG, APP_TYPE_CONSOLE);
    
    /*��ʼ����ʱ��*/
    init_app_timers(btplay_timer_vector, APP_TIMER_COUNT);

    /*��ʼ�� applib ��Ϣģ��*/
    applib_message_init();
#ifdef ENABLE_TRUE_WIRELESS_STEREO    
    btplay_engine_pipe_control(PIPE_OP_FLUSH);
 #endif

    change_engine_state(ENGINE_STATE_PAUSE);

#ifdef WAVES_ASET_TOOLS
    waves_init();
#endif

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_exit(void)
{
    /*ִ��applib���ע������*/
    applib_quit();

    change_engine_state(ENGINE_STATE_PAUSE);
    _save_var();
}

void message_loop_init(void)
{
	update_btstack_parm_control(1);
    
    if (g_p_bt_stack_cur_info->dev_role == 2)
    {
    	//libc_print("@e1:",0,0);
        g_need_adc_flag = 1;                

        btplay_engine_pipe_control(PIPE_OP_UNFILTER);
        
        tws_sync_cmd_send(TWS_SLAVE_ENTER_SYNC);
    }
    
    /*if (g_p_bt_stack_cur_info->dev_role == TWS_MASTER)
    {	
	   tws_or_notws_flag = g_p_bt_stack_cur_info->dev_role;
	  } */
	tws_or_notws_flag = g_p_bt_stack_cur_info->dev_role;
}

void message_loop_exit(void)
{
    //���ٹ����ѯ
    if (sys_share_query_destroy(APP_ID_LINEIN_EG) == -1)
    {
        // PRINT_ERR("btcall share query destroy fail!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }
  
    update_btstack_parm_control(0);	    
	
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
  
    app_result_e result = RESULT_NULL;

    /*ap��ʼ��*/
    _app_init();
    
    //sys_set_mem_lock((0x9fc16c00 - 0x800), (0x2980+0x800));

    sbc_encode_init();
    adc_set_init();   

    result = get_message_loop();
    
    adc_set_exit();
    sbc_encode_exit();
    
    _app_exit();
    return result;
}
