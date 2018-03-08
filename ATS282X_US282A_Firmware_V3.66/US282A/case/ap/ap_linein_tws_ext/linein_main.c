/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������line in tws�Ƹ�ǰ̨Ӧ����ڣ�����Ӧ�ó�ʼ����Ӧ���˳��ȡ�
 * ���ߣ�
 ********************************************************************************/

#include  "ap_linein.h"

/*ȫ�ֱ�������*/
//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     3

app_timer_t g_mainmenu_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
//ϵͳ����
comval_t g_comval;

btplay_status_e g_last_btplay_status = BTPLAY_IDLE;
uint8 g_last_dev_role = 0;

bool g_need_draw = FALSE;

uint8 g_btplay_tick_250ms = 0;
int8 tick_timer_id = -1;
uint8 g_btplay_flag;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
//�������������

uint8 flag2=0;
uint8 flag_con_ok_one=0;
uint8 flag_pair;

uint8 flag_no_tts=0;
uint8 tws_vol_sync_flag = 0;//tws�豸���һ��ͬ������
int8 tws_sync_timer_id = -1;
uint8 tws_volume=0;

//tws�µĲ��Ŵ���
bool g_tws_need_tts_play = FALSE;
uint8 flag_dis_tws_dev=0;

//for linein_ext phone connect 
uint8 g_enter_disc_flag = 0;
uint8 g_set_undicover_flag= 0;
//uint8 g_set_dicover_flag=0;

#endif
uint32 g_last_change_state_time = 0;
uint32 g_second_change_state_flag = 0;

//��ʼ���׶Σ����Թ��˵�һЩ�¼�����
bool g_btplay_init_flag;

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
INT8U prio = AP_FRONT_LOW_PRIO;

btplay_info_t g_btplay_cur_info;
btplay_status_e g_btplay_enter_s3bt_status; //����S3BTǰ״̬
a2dp_status_e g_btplay_enter_s3bt_a2dp_status; //����S3BTǰA2DP����״̬
hfp_status_e g_btplay_enter_s3bt_hfp_status; //����S3BTǰHFP����״̬
uint8 g_btplay_s3bt_param_buffer[512]; //�������غͱ����������ʱ������

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
    //��ȡcommon VM����������Ϣ
    com_setting_comval_init(&g_comval);

    //g_btplay_flag = (uint8) ((act_readl(RTC_BAK0) & (0x7 << MY_RTC_BTPLAY_FLAG)) >> MY_RTC_BTPLAY_FLAG);

    sys_vm_read(&g_config_var,VM_AP_CONFIG,sizeof(g_config_var_t));
    g_btplay_flag = g_config_var.engine_state;
}

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

    /*��ʼ��applib�⣬ǰ̨AP*/
    applib_init(APP_ID_LINEIN, APP_TYPE_GUI);

    /*��ʼ����ʱ��*/
    init_app_timers(g_mainmenu_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    /*��ʼ�� applib ��Ϣģ��*/
    applib_message_init();

    /*��ʼ��ϵͳ��ʱ��*/
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
    com_set_dae_chan(TRUE, TRUE);
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
void _app_exit(void)
{
    //com_set_dae_chan(FALSE, FALSE);
    //ע��RCP�ص�����
    linein_rcp_var_exit();
    
    com_view_manager_exit();

    /*����ϵͳ��ʱ��*/
    sys_timer_exit();

    sys_vm_write(&g_comval, VM_AP_SETTING, sizeof(comval_t));

    /*ִ��applib���ע������*/
    applib_quit();
    
#ifdef SUPPORT_ASET_TEST
        if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
        {
            aset_test_exit();
        }
#endif   
}

void kill_conflict_console(void)
{
    engine_type_e engine_type = get_engine_type();

    if (engine_type == ENGINE_NULL)
    {
        return;
    }

    if (engine_type != ENGINE_LINEIN)
    {
        msg_apps_t msg;

        msg.type = MSG_KILL_APP_SYNC;
        msg.content.data[0] = APP_ID_THEENGINE;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

void create_console(void)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;
    engine_type_e engine_type = get_engine_type();

    if (engine_type == ENGINE_NULL)
    {
        msg.content.data[0] = APP_ID_LINEIN_EG;
        msg.content.data[1] = 0;
        msg.type = MSG_CREAT_APP_SYNC;

        send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0);
    }
}

void kill_console(void)
{
    engine_type_e engine_type = get_engine_type();

    if (engine_type != ENGINE_NULL)
    {
        msg_apps_t msg;

        msg.type = MSG_KILL_APP_SYNC;
        msg.content.data[0] = APP_ID_THEENGINE;
        msg.content.data[1] = 0;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

#ifdef SUPPORT_BTT_TEST
void enter_btt_test(void)
{
    msg_apps_t msg;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTSTACK_ENTER_BTT_TEST_SYNC;

    //����ͬ����Ϣ
    send_sync_msg_btmanager(NULL, &msg, NULL, 0);
}
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void tws_timer_kill(int8 *timer_id)
{
    if (*timer_id != -1)
    {
        kill_app_timer(*timer_id);
    }
    *timer_id = -1;
}
#endif

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

    g_standby_exit_flag = (bool) ((argc & PARAM_S3_EXIT) != 0);
    argc = argc & (~PARAM_S3_EXIT);

    /*ap��ʼ��*/
    //PRINT_INFO("-d-");
    _app_init();

    if (g_standby_exit_flag == TRUE)
    {
        if(sys_vm_read(g_btplay_s3bt_param_buffer, VM_S3BT_APP_STATUS, 3) == 0)
        {
            g_btplay_enter_s3bt_status = g_btplay_s3bt_param_buffer[0];
            g_btplay_enter_s3bt_a2dp_status = g_btplay_s3bt_param_buffer[1];
            g_btplay_enter_s3bt_hfp_status = g_btplay_s3bt_param_buffer[2];
        }
        else
        {
            g_standby_exit_flag = FALSE;    
        }        
    }

    //��������ͼ
    //    g_last_btplay_status = g_btplay_cur_info.status = BTPLAY_IDLE;
    linein_create_main_view();

    kill_conflict_console();

    g_btplay_init_flag = TRUE;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
#if 1
    if( g_app_info_state_all.power_on_flag==1)
    {
        g_app_info_state_all.power_on_flag=0;
    }
    else
    {
        flag_pair=g_bt_stack_cur_info.pair;
        flag_dis_tws_dev=g_bt_stack_cur_info.tws_dev_discon;
    }
#endif
    tws_volume = sys_comval->volume_current;
    //������ƴ����н�Ӧ��
    //��������0
    g_tws_m_switchap_flag = 0; 
#endif
   
    //�����������������ģʽ
    com_set_mute(FALSE);
    com_reset_sound_volume(SET_VOLUME_SYNC_TO_REMOTE);

    if ((g_ap_switch_var.call_background_status != CALL_BACK_STATUS_BACK) && (g_standby_exit_flag == FALSE))
    {
        if (g_bt_stack_cur_info.dev_role != 2)
        {
            com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_NOBLOCK, TTS_PLAY_LINEIN);
        }

        //����������̨
        com_btmanager_init(FALSE);

        //�ȴ�TTS����
        if (com_tts_state_play_wait() == TTS_PLAY_RET_BY_KEY)
        {
            sys_os_time_dly(10); //�ȴ�100ms���Ա�̰�̧���ܹ����
        }
    }

    while (1)
    {
        result = com_view_loop();
        if (result == RESULT_NONE_EVENT)
        {
            result = RESULT_NULL;
            break;
        }
        else if (result > RESULT_COMMON_RESERVE)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            //if (g_bt_stack_cur_info.dev_role == TWS_SLAVE)
            //if (g_bt_stack_cur_info.dev_role != 0)
            if (g_bt_stack_cur_info.dev_role == TWS_MASTER)
            {
                //�����л�Ӧ��,֪ͨ������ͣ���ֲ���
                uint8 val_tmp = 1;
                if(g_bt_stack_cur_info.dev_role == 1)
                {
                    val_tmp = 0xf1;
                }
                tws_sync_deal(val_tmp);
                PRINT_INFO("a1");

            }
#endif
            goto btplay_main_exit;
            // PRINT_INFO("-a-");
        }
        else
        {
            ; //nothing
        }
    }

    g_btplay_init_flag = FALSE;
    
    linein_set_discover_ctl(1);
    //enter disconn
    linein_check_disconn_deal();

#ifdef SUPPORT_BTT_TEST
    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_BTT_MODE)
    {
        while (1)
        {
            if (g_bt_stack_cur_info.conn_status != CONN_STATUS_NONE)
            {
                enter_btt_test();
                break;
            }

            sys_os_time_dly(1);

            result = com_view_loop(); //���� g_bt_stack_cur_info ״̬
            if (result> RESULT_COMMON_RESERVE)
            {
                goto btplay_main_exit;
            }
        }
    }
#endif

    keytone_set_dac_chan(DAF1_EN); //�л�������ͨ������ȷ���м��������
    //create_console();
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //����ӱ��Ӧ���л����������ʱ�򻹴��ڶ���״̬ ����Ҫ֪ͨ����
    //
    //if ((g_standby_exit_flag ==FALSE)&&(g_bt_stack_cur_info.dev_role == TWS_SLAVE))
    //if ((g_standby_exit_flag ==FALSE)&&(g_bt_stack_cur_info.dev_role != 0))
    if ((g_standby_exit_flag ==FALSE)&&(g_bt_stack_cur_info.dev_role == TWS_MASTER))
    {

        uint8 val_tmp = 2;
        if(g_bt_stack_cur_info.dev_role == 1)
        {
            val_tmp = 0xf2;
        }
        tws_sync_deal(val_tmp);
        PRINT_INFO("a3");
    }
#endif
    create_console();

    result = get_message_loop();

    kill_console();

    keytone_set_dac_chan(DAF0_EN); //�л�������ͨ������ȷ���м��������

    btplay_main_exit:
    	
    linein_set_discover_ctl(0);
    
    if (result == RESULT_SYSTEM_ENTER_S3)
    {
        sd_sec_param_t sd_sec_param;

        libc_memset(SRAM_S3BT_BUFFER, 0x0, 0x200);

        //if (g_ap_switch_var.s3bt_nor_erase_flag == FALSE)
        //{
        //    sd_sec_param.file_offset = NOR_S3BT_ERASE_WRITE;
        //    sd_sec_param.sram_addr = g_btplay_s3bt_param_buffer;
        //    sd_sec_param.sec_num = 1;
        //    base_ext_vram_write(&sd_sec_param);

        //    g_ap_switch_var.s3bt_nor_erase_flag = TRUE;
        //}

        g_btplay_s3bt_param_buffer[0] = g_btplay_cur_info.status;
        g_btplay_s3bt_param_buffer[1] = g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].a2dp_status;
        g_btplay_s3bt_param_buffer[2] = g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].hfp_status;

        //sd_sec_param.file_offset = NOR_S3BT_APP_STATUS;
        //sd_sec_param.sram_addr = g_btplay_s3bt_param_buffer;
        //sd_sec_param.sec_num = 1;
        //base_ext_vram_write(&sd_sec_param);

        sys_vm_write(g_btplay_s3bt_param_buffer, VM_S3BT_APP_STATUS, 3);

        PRINT_INFO("ap enter s3bt");
    }
    //create new app
    com_ap_switch_deal(result);

    _app_exit();

    return result;
}
