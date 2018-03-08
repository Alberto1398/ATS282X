/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *        brief   ������Ϣ
 *      Cailizhen          2015-1          creat
 *       Wekan        2015-3-30       review
 *******************************************************************************/

#include  "btcall_engine.h"

/******************************************************************************
 * \  par  Description:   ��ƵԴ�������л��ֻ�
 * \  param[in]     null0
 * \  param[out]   null
 * \  return           null
 * \  note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/
app_result_e btcall_deal_app_msg(void)
{
    private_msg_t cur_msg;
    app_result_e result = RESULT_NULL;

    //���¹����ѯ
    g_p_btcall_info = sys_share_query_update(APP_ID_BTCALLEG);

    if (get_app_msg(&cur_msg) == TRUE)
    {
        result = app_message_deal(&cur_msg);
    }

    return result;
}


/******************************************************************************
 * \  par  Description:   ����callinʱ�������绰�����������˳��¼�
 * \  param[in]     null0
 * \  param[out]   null
 * \  return           null
 * \  note
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-6-17      split
 *******************************************************************************/

void deal_callin(void)
{
    if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->hfp_active_id].hfp_status == HFP_STATUS_INCOMING)
    {
        if ((btcall_gl_var.g_sco_deal_flag == 0)
            && (btcall_gl_var.callin_ring_mode != CALLIN_RING_MODE_ONLY_PB)
            && (g_callring_data.callring_timer_id == -1))
        {
            uint16 callring_start_delay;
            
            if (btcall_gl_var.callin_ring_first == 1)
            {
                callring_start_delay = 2000;
                btcall_gl_var.callin_ring_first = 0;
            }
            else
            {
                callring_start_delay = 200;
            }
            
            g_callring_data.callring_timer_id = set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
            callring_start_delay, (timer_proc) callring_start);
        }
    }
    else
    {
        if (btcall_gl_var.callin_ring_mode != CALLIN_RING_MODE_ONLY_PB)
        {
            if (g_callring_data.callring_timer_id != -1)
            {
                kill_app_timer(g_callring_data.callring_timer_id);
                g_callring_data.callring_timer_id = -1;
            }
            
            if (g_callring_data.callring_thread_exist != 0)
            {
                //�ȴ����������߳��˳�
                btcall_gl_var.need_exit_thread = TRUE;
            }
            while (g_callring_data.callring_thread_exist != 0)
            {
                sys_os_time_dly(1);
            }
        }
    }
    
}


btcall_status_e bt_stack_status(const bt_stack_sub_info_t*cur_dev)
{
    btcall_status_e ret = BTCALL_IDLE;
    static uint32 sco_link_out_tick = 0;
    static uint8 hfp_sta = 0xff;
    static uint8 sco_link = 0xff;
    switch (cur_dev->hfp_status)
    {
        case HFP_STATUS_LINKED:
            ret = BTCALL_STOP;
        break;
        
        case HFP_STATUS_INCOMING:
            ret = BTCALL_CALLIN;
        break;
        
        case HFP_STATUS_OUTGOING:
            ret = BTCALL_CALLOUT;
        break;
        
        case HFP_STATUS_CALLING:
            ret = BTCALL_HFP;
        break;
        
        case HFP_STATUS_PHONE:
            ret = BTCALL_PHONE;
        break;
		
		case HFP_STATUS_SIRI:
        	ret = BTCALL_SIRI;
        break;
        
        case HFP_STATUS_NONE:
        default:
            ret = BTCALL_IDLE;
        break;
    }
    
    if(sco_link != (cur_dev->hfp_attr.sco_link))
    {
        libc_print("SCO-link(old-new):",(sco_link << 8) + (cur_dev->hfp_attr.sco_link), 2);
        //sco_link = cur_dev->hfp_attr.sco_link;
    }
    
    
    if(hfp_sta != cur_dev->hfp_status)
    {
        libc_print("call-hfp(old-new):",(hfp_sta << 8) + cur_dev->hfp_status, 2);
        hfp_sta = cur_dev->hfp_status;
    }
    
    if ((sco_link == 1) && (cur_dev->hfp_attr.sco_link == 0))
    {
        if(cur_dev->hfp_status != HFP_STATUS_PHONE)
        {
            libc_print("force-stop.",0, 0);
            ret = BTCALL_STOP;
        }
    }
    sco_link = cur_dev->hfp_attr.sco_link;
    
    return ret;
}

/******************************************************************************
 * \  par  Description:   ��ȡ��Ϣ�Ĵ���
 * \  param[in]     null0
 * \  param[out]   null
 * \  return           ��Ҫ���㴦����¼�
 * \  note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/
app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    uint8 last_hfp_status = HFP_STATUS_NONE;
    uint8 hfp_sta_tmp;
    uint8 hfp_active_id;
    btcall_gl_var.callin_ring_first = 1;

    while (1)
    {
        
        hfp_active_id = g_p_bt_stack_cur_info->hfp_active_id;
        hfp_sta_tmp = g_p_bt_stack_cur_info->rmt_dev[hfp_active_id].hfp_status;
        g_p_btcall_info->status = bt_stack_status(&g_p_bt_stack_cur_info->rmt_dev[hfp_active_id]); 
        if (btcall_gl_var.tts_play_flag == 0)
        {
            if ((btcall_gl_var.g_sco_deal_flag == 0)
                    && (g_p_bt_stack_cur_info->rmt_dev[hfp_active_id].hfp_attr.sco_link == 1))
            {
                if ((btcall_gl_var.ignore_uncall_sco == 1)
                        && (hfp_sta_tmp != HFP_STATUS_CALLING)
                        && (hfp_sta_tmp != HFP_STATUS_INCOMING))
                {
                    ; //���˷Ǵ�绰��SCO����
                }
                else
                {
                    if ((hfp_sta_tmp != HFP_STATUS_INCOMING)
                            || (btcall_gl_var.callin_ring_mode != CALLIN_RING_MODE_ONLY_PB))
                    { //���������������Ҳ��õ绰������ȡ��������������⣬
                      //������������Դ��������߳�
                        bt_engine_hfp_sco_connect_ind_handle();
                        //libc_print("CORE_PLL_CTL=", act_readl(CORE_PLL_CTL), 2);
                    }
                }
            }         
            else if ((btcall_gl_var.g_sco_deal_flag == 1))
            {
                if(g_stub_tools_type == STUB_PC_TOOL_ATT_MODE)
                {
                    //�Զ�������ģʽʹ�õķ�֧����
                    if(g_p_bt_stack_cur_info->rmt_dev[hfp_active_id].hfp_attr.sco_link == 0)
                    {
                        bt_engine_hfp_sco_disconnect_ind_handle();
                        sys_os_time_dly(5);                       
                    }                    
                }
                else
                {
                    if((g_p_btcall_info->status <= BTCALL_STOP) || (g_p_btcall_info->status == BTCALL_PHONE))
                    {
                        bt_engine_hfp_sco_disconnect_ind_handle();
                        sys_os_time_dly(5);//����ʱ��asqt����
                    }
                }    
            }
            else
            {
                ;//nothing
            }
			
#if 0
			PRINT_INFO_INT("hfp_sta_tmp", hfp_sta_tmp);
			PRINT_INFO_INT("last_hfp_status", last_hfp_status);
			PRINT_INFO_INT("callin_pb_back", btcall_gl_var.callin_pb_back);
#endif	

			if(hfp_sta_tmp == HFP_STATUS_CALLING)
        	{
				g_dsp_init_parm.hfp_connect_mod = 1;
        	}
        	else
        	{
				g_dsp_init_parm.hfp_connect_mod = 0;
        	}
			
			g_p_share_mem->pAEC_parm = (sv_prms_t*)ADDR_M4K2DSP(&g_dsp_init_parm);//ת��dspֱ�ӷ���

			//PRINT_INFO_INT("hfp_connect_mod", g_dsp_init_parm.hfp_connect_mod);
			
			if ((hfp_sta_tmp != last_hfp_status)
                    || (btcall_gl_var.callin_pb_back == 1))
            {	
                deal_callin();
                btcall_gl_var.callin_pb_back = 0;
                last_hfp_status = hfp_sta_tmp;
            }
            
        }

        if ((g_p_btcall_info->status == BTCALL_IDLE) || (g_p_btcall_info->status == BTCALL_STOP))
        {
            change_engine_state(ENGINE_STATE_PAUSE);
            btcall_gl_var.callin_ring_first = 1;
        }
        else
        {
            change_engine_state(ENGINE_STATE_PLAYING);
        }

        //����ǰӦ�õ�˽����Ϣ
        result = btcall_deal_app_msg();
        if (result != RESULT_NULL)
        {
            break;
        }

        //����10ms�����������
        sys_os_time_dly(1);
    }

    //���ٹ����ѯ
    if (sys_share_query_destroy(APP_ID_BTCALLEG) == -1)
    {
        PRINT_ERR("btcall share query destroy fail!\n");
        while (1)
        {
            ;//nothing
        }
    }

    return result;
}
