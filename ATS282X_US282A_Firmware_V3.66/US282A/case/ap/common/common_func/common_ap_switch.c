/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ���л��ӿڣ�ǰ̨Ӧ���˳�ǰ���� app_result_e ����ǰ̨Ӧ���л���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"
#include "applib.h"
#include "case_independent.h"

const ap_switch_info_t ap_switch_info[] =
{
    { APP_ID_BTPLAY, 0, 1, RESULT_BLUETEETH_SOUND, APP_FUNC_BTPLAY },
    { APP_ID_MUSIC, PARAM_MUSIC_SDCARD, 1, RESULT_MUSIC_CPLAY, APP_FUNC_PLAYCARD },
    { APP_ID_MUSIC, PARAM_MUSIC_UHOST, 1, RESULT_MUSIC_UPLAY, APP_FUNC_PLAYUHOST },
    { APP_ID_LINEIN, 0, 1, RESULT_ENTER_LINEIN, APP_FUNC_PLAYLINEIN },
    { APP_ID_RADIO, 0, 1, RESULT_RADIO, APP_FUNC_RADIO },
    { APP_ID_USOUND, 0, 1, RESULT_USB_SOUND, APP_FUNC_USOUND },
    { APP_ID_OTA_TEST, 0, 1, RESULT_ENTER_OTA_SONG_TEST, APP_FUNC_OTA_TEST },
    
    /*!����Ϊ��ݼ�����Ĺ���ѡ��,���ǲ��ڹ���ѭ����,��Ҫ���ص���һ�ε�״̬*/
    { APP_ID_CONFIG, 1, 1, RESULT_POWER_OFF, APP_FUNC_CONFIG },
    { APP_ID_CONFIG, 2, 1, RESULT_LOW_POWER, APP_FUNC_CONFIG },
    { APP_ID_CONFIG, 3, 1, RESULT_SYSTEM_ENTER_S3, APP_FUNC_CONFIG },
    { APP_ID_CONFIG, 4, 1, RESULT_IDLE_MODE, APP_FUNC_CONFIG },

    { APP_ID_UDISK, 0, 1, RESULT_USB_TRANS, APP_FUNC_UDISK },

// �̼�̫�󣬹ر�����AP
    { APP_ID_ALARM, 0, 1, RESULT_ENTER_ALARM, APP_FUNC_CLOCK },
    { APP_ID_ALARM, 1, 1, RESULT_ALARM_RING_FOLDER_SCAN, APP_FUNC_CLOCK },

    { APP_ID_BTCALL, 1, 1, RESULT_BLUETEETH_CALL_BACKGROUND, APP_FUNC_BTCALL },

    { APP_ID_UPGRADE, 0, 1, RESULT_ENTER_UPGRADE, APP_FUNC_UPGRADE },

    { APP_ID_RECORD, PARAM_FROM_LINEIN_TOC, 1, RESULT_RECORD_LINEIN_TOC, APP_FUNC_LINREC_TOC },
    { APP_ID_RECORD, PARAM_FROM_RADIO_TOC, 0, RESULT_RECORD_FM_TOC, APP_FUNC_FMREC_TOC },
    { APP_ID_RECORD, PARAM_FROM_MIC_TOC, 1, RESULT_RECORD_MIC_TOC, APP_FUNC_MICREC_TOC },
    { APP_ID_RECORD, PARAM_FROM_LINEIN_TOU, 1, RESULT_RECORD_LINEIN_TOU, APP_FUNC_LINREC_TOU },
    { APP_ID_RECORD, PARAM_FROM_RADIO_TOU, 0, RESULT_RECORD_FM_TOU, APP_FUNC_FMREC_TOU },
    { APP_ID_RECORD, PARAM_FROM_MIC_TOU, 1, RESULT_RECORD_MIC_TOU, APP_FUNC_MICREC_TOU },

    { APP_ID_MUSIC, PARAM_RECORD_CPLAY, 1, RESULT_RECORD_CPLAY, APP_FUNC_PLAYCRECORD },
    { APP_ID_MUSIC, PARAM_RECORD_UPLAY, 1, RESULT_RECORD_UPLAY, APP_FUNC_PLAYURECORD },

    { APP_ID_BTCALL, 0, 1, RESULT_ENTER_ASQT_HF_TEST, APP_FUNC_BTCALL },
    
    { APP_ID_OTA_UPGRADE, 0, 1, RESULT_ENTER_OTA_UPGRADE, APP_FUNC_UPGRADE },
    //{ APP_ID_USER1, 0, 1, RESULT_ENTER_USER1, APP_FUNC_USER1 },
};

#define AP_SWITCH_INFO_COUNT    (ARR_COUNT(ap_switch_info))

static uint8 com_get_cfg_func_index(uint8 app_func_id);
static uint8 com_get_switch_seq_index(uint8 app_func_id);
static bool com_check_function_in(uint8 func_id);
static uint8 com_app_switch_next_function(uint8 last_index);
static uint8 com_app_switch_any_function(uint8 cur_index);
static void com_send_msg_to_manager(uint32 msg_type, uint8 param1, uint8 param2, bool synch_flag);
static uint8 com_ap_switch_next_index(app_result_e app_result);
static void restore_global_status(void);

//���ؼ����л����ĸ�FUNCTION
uint8 com_ap_switch_ask_next_func(app_result_e app_result)
{
    uint8 next_index;
    uint8 default_index = 0; //��Ӧ��ap_func_table������±�

    //�õ�ȱʡAPP_FUNC��ap_switch_info�����е��±�
    default_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);

    next_index = com_ap_switch_next_index(app_result);

    //for valid set
    if (next_index >= AP_SWITCH_INFO_COUNT)
    {
        //check index
        next_index = default_index;
    }

    return ap_switch_info[next_index].app_func_id;
}

/******************************************************************************/
/*!
 * \par  Description:
 *\ ap�л�����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/
void com_ap_switch_deal(app_result_e app_result)
{
    uint8 deal_ap_id;
    uint8 ap_param;
    uint8 cur_index; //ap_func_table[cur_index]��Ҫ�����APP_FUNC ��ID
    //(ֻ�����,cur_index�����ap_switch_info���±�)
    uint8 last_index; //��Ӧ��ap_func_table���±�
    uint8 default_index = 0; //��Ӧ��ap_func_table������±�
    uint8 i;
    uint32 reg_val;

    if (app_result == RESULT_SYSTEM_EXIT_S3)
    {
        //�ָ� app_info_state_t �� app_last_state_t
        restore_global_status();

        deal_ap_id = ap_switch_info[g_app_info_state.cur_func_index].ap_id;
        ap_param = ap_switch_info[g_app_info_state.cur_func_index].param;
        ap_param |= (uint8) PARAM_S3_EXIT;
        goto ap_switch_create_app;
    }

    //�õ�ȱʡAPP_FUNC��ap_switch_info�����е��±�
    default_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);

    //��ǰ��index ��Ϊ��һ�εı���
    last_index = get_cur_func_index();

    if (app_result == RESULT_ESD_RESTART)
    {
        //del  cur_index = (uint8) ((act_readl(RTC_BAK0) & (0xff << MY_RTC_FUNC_INDEX)) >> MY_RTC_FUNC_INDEX);
        cur_index = g_config_var.ap_id;
        if(ap_switch_info[cur_index].app_func_id == APP_FUNC_UDISK)
        {
            g_esd_cardreader_flag = 1;
        }
        goto ap_switch_create_app_esd_restart;
    }

    if (app_result == RESULT_BLUETEETH_CALL_BACKGROUND)
    {
        g_ap_switch_var.call_background_status = CALL_BACK_STATUS_IN;

#if 0        
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        g_bt_auto_connect_ctrl.need_auto_connect = 0;
        
        if(g_bt_stack_cur_info.dev_role == TWS_MASTER)
        {
            //���绰����������ȥ�Ͽ�������֮�������
            msg_apps_t msg;
            msg.type =  MSG_BTSTACK_TWS_FORCE_UNLINK_SYNC;
            tws_auto_info_t *cur_auto_info;
            msg.content.addr = cur_auto_info;

            //cur_auto_info Ҫ��0
            libc_memset(cur_auto_info, 0x00, sizeof(cur_auto_info));

            //����ͬ����Ϣ
            send_sync_msg_btmanager(NULL, &msg, NULL, 0);  
            g_ap_switch_var.tws_phon_come_flag=1;

            for ( i= 0; i < g_btmanager_gl_var.support_dev_num; i++)
            {    
                PRINT_DATA(cur_auto_info->bd_addr.bytes,6);
                if (libc_memcmp(&g_bt_stack_cur_info.rmt_dev[i].addr.bytes[3],
                    &cur_auto_info->bd_addr.bytes[3], 3) == 0)        
                {
                    //PRINT_DATA(&g_bt_stack_cur_info.rmt_dev[i].addr.bytes[3],3);

                    libc_memcpy(g_bt_auto_connect_ctrl.dev_info[i].remote_addr.bytes,
                                           cur_auto_info->bd_addr.bytes, 6);
                    g_bt_auto_connect_ctrl.dev_info[i].conn_flag=1;
                    g_bt_auto_connect_ctrl.dev_info[i].support_profile=cur_auto_info->support_profile;        
                    break;
                }                                                                                                              
            }    
        }
#endif
#endif
    }
    else
    {
        if (g_ap_switch_var.call_background_status == CALL_BACK_STATUS_IN)
        {
            g_ap_switch_var.call_background_status = CALL_BACK_STATUS_BACK;            
        }
        else
        {
            g_ap_switch_var.call_background_status = CALL_BACK_STATUS_NULL;
        }
    }

    //��һЩ��飬������У�cur_index�ͻ�����һ�����������ģ�
    //��cur_index��Ȼ��ap_switch_info����������±�
    cur_index = com_ap_switch_next_index(app_result);

    //for valid set
    if (cur_index >= AP_SWITCH_INFO_COUNT)
    {
        //check index
        cur_index = default_index;
        last_index = default_index;
    }

    if (ap_switch_info[cur_index].engine_do == TRUE)
    {
        //check��ǰ���棬�ر�
        if (get_engine_type() != ENGINE_NULL)
        {
            ap_param = 0;
            if (app_result == RESULT_SYSTEM_ENTER_S3)
            {
                ap_param = 1;
            }
            //ɱ������
            com_send_msg_to_manager(MSG_KILL_APP_SYNC, APP_ID_THEENGINE, ap_param, TRUE);
        }
    }

    if (app_result == RESULT_SYSTEM_ENTER_S3)
    {
        sd_sec_param_t sd_sec_param;

        //���� app_info_state_t �� app_last_state_t
        libc_memset(SRAM_S3BT_BUFFER, 0x0, 0x200);

        //if (g_ap_switch_var.s3bt_nor_erase_flag == FALSE)
        //{
        //    sd_sec_param.file_offset = NOR_S3BT_ERASE_WRITE;
        //    sd_sec_param.sram_addr = SRAM_S3BT_BUFFER;
        //    sd_sec_param.sec_num = 1;
        //    base_ext_vram_write(&sd_sec_param);

        //    g_ap_switch_var.s3bt_nor_erase_flag = TRUE;
        //}

        libc_memcpy(SRAM_S3BT_BUFFER, &g_app_info_state, sizeof(app_info_state_t));
        libc_memcpy(SRAM_S3BT_BUFFER + sizeof(app_info_state_t), &g_app_last_state, sizeof(app_last_state_t));

        //sd_sec_param.file_offset = NOR_S3BT_APP_GLOBAL;
        //sd_sec_param.sram_addr = SRAM_S3BT_BUFFER;
        //sd_sec_param.sec_num = 1;
        //base_ext_vram_write(&sd_sec_param);

        sys_vm_write(SRAM_S3BT_BUFFER, VM_S3BT_APP_GLOBAL, sizeof(app_info_state_t) + sizeof(app_last_state_t));
    }

    ap_switch_create_app_esd_restart:

    //���浱ǰ��index,��Ϊ��һ�ε����
    set_last_func_index(last_index);

    //save cur function index
    set_cur_func_index(cur_index);
    
    //reg_val = g_config_var.ap_id;
    //del  reg_val = (act_readl(RTC_BAK0) & (~(0xff << MY_RTC_FUNC_INDEX)));
    
    if(g_app_info_state.stub_tools_type == 0)
    {
        //del  reg_val |= ((uint32)(g_app_info_state.cur_func_index) << MY_RTC_FUNC_INDEX);
        reg_val = g_app_info_state.cur_func_index;
    }
    else
    {
        //del  reg_val |= (APP_FUNC_INVALID << MY_RTC_FUNC_INDEX);    
        reg_val = APP_FUNC_INVALID;
    }
    g_config_var.ap_id = (uint8)reg_val;
    sys_vm_write(&g_config_var,VM_AP_CONFIG,sizeof(g_config_var_t));
    //del  act_writel(reg_val, RTC_BAK0); 

    //rtc register
    //del  act_writel(0xA596, RTC_REGUPDATA);
    //del  while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//wait for register update
    }      
    
    //save cur function id
    set_cur_func_id(ap_switch_info[cur_index].app_func_id);

    PRINT_INFO_INT("ap swi fuc:", ap_switch_info[cur_index].app_func_id);

    //��manager����Ϣ����ǰ̨Ӧ��

    deal_ap_id = ap_switch_info[cur_index].ap_id;

    ap_param = ap_switch_info[cur_index].param;

    //ת�� ����Ӧ�� ��Ϣ��manager
    ap_switch_create_app:
    g_ap_switch_var.apk_alarm_flag = 0; //��APK�л����������ý���
    com_send_msg_to_manager(MSG_CREAT_APP, deal_ap_id, ap_param, FALSE);

    g_ap_switch_var.wake_from_s2 = FALSE;
    g_ap_switch_var.wake_ret_from_s2 = 0;
}


//app_func_id��ֵ���磺APP_FUNC_xxx
//���أ�app_func_id��ap_switch_info����������±�
//      ���û�ҵ����ͷ���0
static uint8 com_get_cfg_func_index(uint8 app_func_id)
{
    uint8 i;

    for (i = 0; i < AP_SWITCH_INFO_COUNT; i++)
    {
        if (ap_switch_info[i].app_func_id == app_func_id)
        {
            break;
        }
    }
    if (i >= AP_SWITCH_INFO_COUNT)
    {
        i = 0;
    }

    return i;
}

//���أ�app_func_id�ǡ�config.txt��SETTING_APP_SWITCH_SEQUENCE��еĵڼ���
static uint8 com_get_switch_seq_index(uint8 app_func_id)
{
    uint8 i;

    for (i = 0; i < MAX_FUNCTON_CYCLE; i++)
    {
        if (g_ap_switch_var.app_switch_seq[i] == app_func_id)
        {
            return i;
        }
    }

    return 0xff;
}

/******************************************************************************/
/*!
 * \par  Description:��ǰ������Ӳ���Ƿ�ok���ܷ����
 *\ check cur app function hardware is ready and can realy enter
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/

static bool com_check_function_in(uint8 func_id)
{
    //check if hard ware is not ready
    switch (func_id)
    {
        case APP_FUNC_PLAYUHOST:
        //u �̲�����
        if ((int8) sys_detect_disk(DRV_GROUP_STG_UHOST) == -1)
        {
            return FALSE;
        }
        break;

        case APP_FUNC_PLAYCARD:
        //��������
        if ((int8) sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            return FALSE;
        }
        break;

        case APP_FUNC_PLAYLINEIN:
        //linein����
#ifndef ENABLE_TRUE_WIRELESS_STEREO
        if (get_linein_state() != LINEIN_STATE_LINEIN_IN)
#else

#if (SUPPORT_AUX_DETECT == DETECT_NO_SUPPORT)
        g_tws_m_linein_flag = 1;
#endif
        if (g_bt_stack_cur_info.dev_role == NORMAL_DEV)
        {
            libc_print("c line flag",0,0);
            g_tws_m_linein_flag = 0;
        }
                        
        if ( ((get_linein_state() != LINEIN_STATE_LINEIN_IN) && (g_tws_m_linein_flag == 0))
            || ((get_linein_state() == LINEIN_STATE_LINEIN_IN)&& (g_tws_m_linein_flag == 0)
                &&(g_bt_stack_cur_info.dev_role == TWS_SLAVE)) )
#endif            
        {
            return FALSE;
        }
        break;

        case APP_FUNC_USOUND:
        //��ǰ��USB�߲���
        if (get_cable_state() != CABLE_STATE_CABLE_IN)
        {
            return FALSE;
        }

        if (g_app_info_state.stub_tools_type != 0)
        {
            return FALSE;
        }
        break;

        default:
        break;
    }

    return TRUE;
}

//����config.txt�л�����һ��APP_FUNC
//���أ���APP_FUNC��ap_switch_info����������±�
//last_index: ��ap_switch_info����������±�
static uint8 com_app_switch_next_function(uint8 last_index)
{
    uint8 last_switch_seq_index;
    uint8 tmp_func_id;

    if (last_index >= MAX_FUNCTON_CYCLE)
    {
        last_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);
    }

    //�ǡ�config.txt��SETTING_APP_SWITCH_SEQUENCE��еĵڼ���
    last_switch_seq_index = com_get_switch_seq_index(ap_switch_info[last_index].app_func_id);
    if (last_switch_seq_index == 0xff)
    {
        last_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);
        last_switch_seq_index = com_get_switch_seq_index(ap_switch_info[last_index].app_func_id);
        if (last_switch_seq_index == 0xff)
        {
            PRINT_ERR("ap swi er!");
            while (1)
            {
                ; //nothing for QAC
            }
        }
    }

    find_next_index:
    //����config.txt�е���һ��APP_FUNC
    last_switch_seq_index++;

    if (last_switch_seq_index >= MAX_FUNCTON_CYCLE)
    {
        last_switch_seq_index = 0;
    }

    tmp_func_id = g_ap_switch_var.app_switch_seq[last_switch_seq_index];
    if (((g_ap_switch_var.app_func_support & (uint32) (1 << tmp_func_id)) == 0)
            || (com_check_function_in(tmp_func_id) == FALSE))
    {
        goto find_next_index;
    }

    //���ظ�APP_FUNC��ap_switch_info����������±�
    return com_get_cfg_func_index(tmp_func_id);
}

//cur_index��ap_switch_info����������±�
//��������һЩ���
//����ֵ��Ȼ��ap_switch_info����������±�
static uint8 com_app_switch_any_function(uint8 cur_index)
{
    uint8 cur_switch_seq_index;

    if (cur_index >= AP_SWITCH_INFO_COUNT)
    {
        cur_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);
    }

    if ((g_ap_switch_var.app_func_support & (uint32) (1 << ap_switch_info[cur_index].app_func_id)) == 0)
    {
        cur_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);
    }

    if (cur_index < MAX_FUNCTON_CYCLE)
    {
        //cur_switch_seq_index��ʾconfig.txt����ĵڼ���APP_FUNC
        cur_switch_seq_index = com_get_switch_seq_index(ap_switch_info[cur_index].app_func_id);
        if (cur_switch_seq_index == 0xff)
        {
            cur_switch_seq_index = com_get_switch_seq_index(g_ap_switch_var.app_default_func_id);
            cur_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);
        }

        if(cur_switch_seq_index >= MAX_FUNCTON_CYCLE)
        {
            goto index_err;
        }
        
        if (((g_ap_switch_var.app_func_support & (uint32) (1 << ap_switch_info[cur_index].app_func_id)) == 0)
                || (com_check_function_in(g_ap_switch_var.app_switch_seq[cur_switch_seq_index]) == FALSE))
        {
            cur_switch_seq_index = com_app_switch_next_function(cur_index);
        }
        
        if(cur_switch_seq_index >= MAX_FUNCTON_CYCLE)
        {
            goto index_err;
        }
        
        //cur_index��ap_switch_info����������±�
        cur_index = com_get_cfg_func_index(g_ap_switch_var.app_switch_seq[cur_switch_seq_index]);
    }

    return cur_index;
    
    index_err:
    libc_print("index err:",cur_switch_seq_index,2);
    return cur_index;
}

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:������Ϣ��manager
 *\ check cur app function hardware is ready and can realy enter
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/
static void com_send_msg_to_manager(uint32 msg_type, uint8 param1, uint8 param2, bool synch_flag)
{
    msg_apps_t msg;

    //message type
    msg.type = msg_type;
    //param
    msg.content.data[0] = param1;

    msg.content.data[1] = param2;
    //whether sych message
    if (synch_flag == TRUE)
    {
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
    else
    {
        //msg.content.data[1] = param2;
        //�����첽��Ϣ
        send_async_msg(APP_ID_MANAGER, &msg);
    }
}

//���ؼ����л����ĸ�INDEX
static uint8 com_ap_switch_next_index(app_result_e app_result)
{
    uint8 cur_index; //ap_func_table[cur_index]��Ҫ�����APP_FUNC ��ID
    //(ֻ�����,cur_index�����ap_switch_info���±�)
    uint8 last_index; //��Ӧ��ap_func_table���±�
    uint8 default_index = 0; //��Ӧ��ap_func_table������±�

    //�õ�ȱʡAPP_FUNC��ap_switch_info�����е��±�
    default_index = com_get_cfg_func_index(g_ap_switch_var.app_default_func_id);

    //��ǰ��index ��Ϊ��һ�εı���
    last_index = get_cur_func_index();

    if (app_result == RESULT_ESD_RESTART)
    {
        //del  cur_index = (uint8) ((act_readl(RTC_BAK0) & (0xff << MY_RTC_FUNC_INDEX)) >> MY_RTC_FUNC_INDEX);
        cur_index = g_config_var.ap_id;
        return cur_index;
    }

    if (app_result == RESULT_LASTPLAY)
    {
        //��һ�ε�apindex
        cur_index = get_last_func_index();
        if (cur_index >= MAX_FUNCTON_CYCLE)
        {
            cur_index = com_get_switch_seq_index(g_ap_switch_var.app_default_func_id);
        }
    }
    else if (app_result == RESULT_NEXT_FUNCTION)
    {
        //������һ��Ӧ��
        //cur_index�ǰ���config.txt�л�APP_FUNC�����APP_FUNC��ap_switch_info����������±�
        cur_index = com_app_switch_next_function(last_index);
    }
    else
    {
        //���ݷ���ֵȷ������ֵ
        for (cur_index = 0; cur_index < AP_SWITCH_INFO_COUNT; cur_index++)
        {
            //check enter which one
            if (ap_switch_info[cur_index].result_val == app_result)
            {
                break;
            }
        }

        if (cur_index >= AP_SWITCH_INFO_COUNT)
        {
            cur_index = default_index;
        }
    }

    //��һЩ��飬������У�cur_index�ͻ�����һ�����������ģ�
    //��cur_index��Ȼ��ap_switch_info����������±�
    cur_index = com_app_switch_any_function(cur_index);

    return cur_index;
}

//�ָ� app_info_state_t �� app_last_state_t
static void restore_global_status(void)
{
    uint8 temp_data[512];
    
    app_info_state_t tmp_app_info_state;
    app_last_state_t tmp_app_last_state;

    sys_vm_read(temp_data, VM_S3BT_APP_GLOBAL, sizeof(app_info_state_t) + sizeof(app_last_state_t));

    libc_memcpy(&tmp_app_info_state, temp_data, sizeof(app_info_state_t));
    libc_memcpy(&tmp_app_last_state, temp_data + sizeof(app_info_state_t), sizeof(app_last_state_t));

    g_app_info_state.card_state = tmp_app_info_state.card_state;
    g_app_info_state.uhost_state = tmp_app_info_state.uhost_state;
    g_app_info_state.linein_state = tmp_app_info_state.linein_state;
    g_app_info_state.cable_state = tmp_app_info_state.cable_state;
    g_app_info_state.hp_state = tmp_app_info_state.hp_state;
    g_app_info_state.charge_state = tmp_app_info_state.charge_state;
    g_app_info_state.bat_value = tmp_app_info_state.bat_value;
    g_app_info_state.cur_func_index = tmp_app_info_state.cur_func_index;
    g_app_info_state.cur_func_id = tmp_app_info_state.cur_func_id;
    g_app_info_state.apk_status = tmp_app_info_state.apk_status;
    g_app_info_state.verification_status = tmp_app_info_state.verification_status;
    g_app_info_state.need_update_playlist = tmp_app_info_state.need_update_playlist;

    g_app_last_state.last_func_index = tmp_app_last_state.last_func_index;
}

uint32 com_check_ap_func_index_valid(void)
{
    uint32 cur_index;
    uint32 next_ap_id;
    
    //del  cur_index = ((act_readl(RTC_BAK0) & (0xff << MY_RTC_FUNC_INDEX)) >> MY_RTC_FUNC_INDEX);
    cur_index = g_config_var.ap_id;
    if(cur_index >= (sizeof(ap_switch_info) / sizeof(ap_switch_info_t)))
    {
        goto err_index;    
    }

    next_ap_id = ap_switch_info[cur_index].ap_id;

    libc_print("ESD next ap id: ", next_ap_id, 2);

    //����config ap��ESD��������
    if(next_ap_id == APP_ID_CONFIG)
    {
        goto err_index;
    }

    return TRUE;
    
    err_index:
    PRINT_INFO_INT("err index", cur_index);

    return FALSE;
}

