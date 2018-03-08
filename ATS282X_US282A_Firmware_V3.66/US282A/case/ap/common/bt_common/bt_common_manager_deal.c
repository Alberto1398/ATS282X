/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ģ��HANDLE����Ҫ����ǰ̨Ӧ�õĳ���ѭ����ִ�У�������ղ�����BT
 *       STACK���¼�������ѯ���BT STACK��״̬�仯����Ӧ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"
#include "common_btmanager.h"
#include "common_rcp.h"
#include "config_id.h"
#include "common_func.h"

#define Volume_Delay_Update_Time 4000
void com_set_ble_discoverable(bool mode) __FAR__;
//void com_set_discoverable(uint8 mode) __FAR__;
void com_set_active_dev(msg_apps_type_e msg_type, uint8 id) __FAR__;
void com_call_handle(void) __FAR__;
void com_bt_sniff_control(uint8 *bd, uint8 mode) __FAR__;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void __section__(".call_auto_conn") send_mod_flag_to_other() __FAR__;
#endif
void __section__(".call_auto_conn1") com_hfp_old_sco_change(uint8 id)__FAR__;

void __section__(".rcode") get_bt_stack_info(void)
{
    if (sys_share_query_read(APP_ID_BTSTACK, &g_bt_stack_cur_info) == -1)
    {
        PRINT_ERR("shqry err");
        while (1)
        {
            ; //nothing
        }
    }
}

void APP_AVRCP_Volume_Timer_Proc(void)
{
    uint8 volume,temp_vol;

    volume = sys_comval->volume_current;
    PRINT_INFO_INT("update vol:", volume);


    if (volume <= 8)
    {
        temp_vol = volume + 1;
    }
    else
    {
        temp_vol = volume - 1;
    }

    if (volume != VOLUME_VALUE_MAX)
    {
        com_btmanager_avrcp_update_volume_to_phone(temp_vol);
    }

    com_btmanager_avrcp_update_volume_to_phone(volume);
    g_btmanager_gl_var.avrcp_timer_id = -1;
}

void check_a2dp_status_ok(uint8 dev_index)
{
    com_set_active_dev(MSG_BTSTACK_SET_A2DP_ACTIVE_DEV_SYNC, dev_index);
    PRINT_DBG_INT("set a2dp id:", dev_index);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((g_btmanager_gl_var.avrcp_timer_id == -1)
            && (g_btmanager_gl_var.enable_avrcp_volume_sync == 1)
            && (g_bt_stack_cur_info.dev_role != TWS_SLAVE))
#else
    if ((g_btmanager_gl_var.avrcp_timer_id == -1)
            && (g_btmanager_gl_var.enable_avrcp_volume_sync == 1))
#endif
    {
        g_btmanager_gl_var.avrcp_timer_id = set_single_shot_app_timer_btmamager(APP_TIMER_ATTRB_COM_CONTROL \
        | (APP_TIMER_TAG_SYS << 8),Volume_Delay_Update_Time, APP_AVRCP_Volume_Timer_Proc);
    }
}

void check_a2dp_status_ok(uint8 dev_index) __FAR__;

void __section__(".rcode") check_a2dp_status(void)
{
    uint8 i;

    PRINT_DBG("check_a2dp_status");
    g_btmanager_gl_var.a2dp_timer_id = -1;

    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
/*
 1.����;NO_TWS_DEV;A2DP_STATUS_PLAY
 2.����;TWS_DEV;A2DP_STATUS_PLAY
 3.����;NO_TWS_DEV;A2DP_STATUS_PLAY
*/
        if ((g_bt_stack_cur_info.rmt_dev[i].a2dp_status == A2DP_STATUS_PLAY)
        	   &&(((g_bt_stack_cur_info.dev_role == TWS_MASTER)&&(g_bt_stack_cur_info.rmt_dev[i].dev_type == NO_TWS_DEV))
        	      || ((g_bt_stack_cur_info.dev_role == TWS_SLAVE)&&(g_bt_stack_cur_info.rmt_dev[i].dev_type == TWS_DEV))
        	      || ((g_bt_stack_cur_info.dev_role == NORMAL_DEV)&&(g_bt_stack_cur_info.rmt_dev[i].dev_type == NO_TWS_DEV))))       	 
#else
        if (g_bt_stack_cur_info.rmt_dev[i].a2dp_status == A2DP_STATUS_PLAY)
#endif
        {
            check_a2dp_status_ok(i);
            break;
        }
    }
}

/*
 0.�رտɼ���
 1.�򿪿ɼ���
 2.�رտɼ���,��������
*/
void com_set_discoverable(uint8 mode)
{
    msg_apps_t msg;

    g_btmanager_gl_var.discoverable_flag = mode;

    msg.type = MSG_BTSTACK_SET_DISCOVERABLE_SYNC;
    msg.content.data[0] = mode;

    send_sync_msg_btmanager(NULL, &msg, NULL, 0);
}

void com_set_ble_discoverable(bool mode)
{
    msg_apps_t msg;

    msg.type = MSG_BTSTACK_SET_BLE_DISCOVERABLE_SYNC;
    msg.content.data[0] = mode;

    send_sync_msg_btmanager(NULL, &msg, NULL, 0);
}

void com_bt_sniff_control(uint8 *bd, uint8 mode)
{
    msg_apps_t msg;

    msg.type = MSG_BTSTACK_SNIFF_CONTROL_SYNC;
    msg.content.data[0] = mode;

    send_sync_msg_btmanager(bd, &msg, NULL, 0);
}

void com_set_active_dev(msg_apps_type_e msg_type, uint8 id)
{
    msg_apps_t msg;

    msg.type = msg_type;
    msg.content.data[0] = id;
    send_sync_msg_btmanager(NULL, &msg, NULL, 0);
    //���ϸ���active device
    get_bt_stack_info();
}

void com_call_handle(void)
{
    msg_apps_t msg;

    msg.type = MSG_BT_HFP_CALL;
    sys_mq_send(MQ_ID_SYS, &msg);

    if (g_btmanager_gl_var.a2dp_timer_id != -1)
    {
        kill_app_timer(g_btmanager_gl_var.a2dp_timer_id);
        g_btmanager_gl_var.a2dp_timer_id = -1;
    }
}

void __section__(".rcode") com_btmanager_discoverable_handle(void)
{
#ifndef ENABLE_TRUE_WIRELESS_STEREO
    if (g_bt_stack_cur_info.num_connected == g_btmanager_gl_var.support_dev_num)
    {
        if (g_btmanager_gl_var.discoverable_flag == 1)
        {
            com_set_discoverable(0);
        }
    }
    else
    {
        if (g_btmanager_gl_var.discoverable_flag == 0)
        {
            com_set_discoverable(1);
        }
    }
#else
    uint8 i;
    //�ֻ������ϼ��رտɼ���
    if (g_bt_stack_cur_info.num_connected != 0)
    {        
      if (g_bt_stack_cur_info.num_connected == g_btmanager_gl_var.support_dev_num)
      {
      	 //�豸��������,�رտ�������
      	 if (g_btmanager_gl_var.discoverable_flag != 2)
         {
            com_set_discoverable(2);
         }
      }
      else
      {
      	 for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
         {      	
         	if (g_btmanager_gl_var.discoverable_flag == 1)
         	{
         		//�ֻ������ϼ��رտɼ���
         	  if ((g_bt_stack_cur_info.rmt_dev[i].dev_type == NO_TWS_DEV)
         	  	   || (g_bt_stack_cur_info.dev_role == TWS_SLAVE))
         	  {
         	  	com_set_discoverable(0);
         	  	break;
         	  }
          }
         	else
         	{
         		if ((g_bt_stack_cur_info.dev_role == TWS_MASTER)
         			  && (g_bt_stack_cur_info.num_connected == 1)
         			  &&(g_btmanager_gl_var.discover_ctl == 0))
         		{
         			com_set_discoverable(1);
         			break;
         		}	        		
         	}		
         }          
      }		            	      
    }   
    else
    {
        if ((g_btmanager_gl_var.discoverable_flag == 0)
        	  || (g_btmanager_gl_var.discoverable_flag == 2))
        {
            com_set_discoverable(1);
        }
    }
#endif     
}

void start_avrcp_volume_update_timer(void)
{
    PRINT_INFO("#status change");
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((g_btmanager_gl_var.avrcp_timer_id == -1)
            && (g_btmanager_gl_var.enable_avrcp_volume_sync == 1)
            && (g_bt_stack_cur_info.dev_role != TWS_SLAVE))
#else
    if ((g_btmanager_gl_var.avrcp_timer_id == -1)
            && (g_btmanager_gl_var.enable_avrcp_volume_sync == 1))
#endif   
    {
        //PRINT_DBG("= -1");
        g_btmanager_gl_var.avrcp_timer_id = set_single_shot_app_timer_btmamager(APP_TIMER_ATTRB_COM_CONTROL \
        | (APP_TIMER_TAG_SYS << 8),Volume_Delay_Update_Time, APP_AVRCP_Volume_Timer_Proc);
    }
}

void check_a2dp_status(void) __FAR__;
void com_btmanager_a2dp_handle_kill_a2dp_timer(void)
{
    kill_app_timer(g_btmanager_gl_var.a2dp_timer_id);
    check_a2dp_status();
    g_btmanager_gl_var.a2dp_timer_id = -1;
}

void com_btmanager_a2dp_handle_kill_avrcp_timer(void)
{
    kill_app_timer(g_btmanager_gl_var.avrcp_timer_id);
    g_btmanager_gl_var.avrcp_timer_id = -1;
}

void start_avrcp_volume_update_timer(void) __FAR__;
void com_btmanager_a2dp_handle_kill_a2dp_timer(void) __FAR__;
void com_btmanager_a2dp_handle_kill_avrcp_timer(void) __FAR__;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void __section__(".call_auto_conn") send_mod_flag_to_other()
{
            libc_print("#b1",0,0);
           //tws_send_cmd(SUB_SYNC_QUIT_BL);
         send_cmd_to_other(1);
         send_once_flag++;
  }

void __section__(".rcode") com_btmanager_mod_change()
{
    if(g_this_app_info->app_id!=APP_ID_BTPLAY)
    {
         if(g_bt_stack_cur_info.dev_role==TWS_SLAVE)
         {
             //�������䲻�ܴ򿪶�����־
              if(send_once_flag==0)
              {
                send_mod_flag_to_other();
              }
          }
          else
          {
               send_once_flag=0;
          }
    
    }
}
#endif

void __section__(".rcode") com_btmanager_a2dp_handle(void)
{
    if (g_this_app_info->app_id == APP_ID_BTPLAY)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if((g_btmanager_gl_var.last_a2dp_status==A2DP_STATUS_NONE)&&(g_bt_stack_cur_info.dev_role==TWS_SLAVE))
        {
            //libc_print("updat stack vol ",0,0);
            com_btmanager_tws_send(VOLUME_VALUE_FLAG,sys_comval->volume_current,NULL,NULL,MSG_BTSTACK_TWS_VOLUME_SYNC);
        }
#endif    
        if (g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].a2dp_status != A2DP_STATUS_PLAY)
        {
            /*�ӳټ���Ƿ���Ҫ�л���Ƶ�������ǰ�豸���Ƹ裬Ȼ�����磬����״̬������ͣ������
             �жϵĻ����Ҷϵ绰����ܻᲥ����һ���豸�ĸ���*/
            if (g_btmanager_gl_var.a2dp_timer_id == -1)
            {
                g_btmanager_gl_var.a2dp_timer_id = set_single_shot_app_timer_btmamager(APP_TIMER_ATTRB_COM_CONTROL \
                | (APP_TIMER_TAG_SYS << 8),1000, check_a2dp_status);
            }

            if (g_btmanager_gl_var.avrcp_timer_id != -1)
            {
                com_btmanager_a2dp_handle_kill_avrcp_timer();
            }
        }
        else
        {
            if (g_btmanager_gl_var.a2dp_timer_id != -1)
            {
                com_btmanager_a2dp_handle_kill_a2dp_timer();
            }

            if ((g_btmanager_gl_var.last_a2dp_status == A2DP_STATUS_PAUSE) ||
                    (g_btmanager_gl_var.last_a2dp_status == A2DP_STATUS_LINKED) ||
                    (g_btmanager_gl_var.send_vol_syn == 1))
            {
                start_avrcp_volume_update_timer();
                g_btmanager_gl_var.send_vol_syn = 0;
            }
        }
    }
    else
    {
        if (g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].a2dp_status == A2DP_STATUS_PLAY)
        {
            g_btmanager_gl_var.send_vol_syn = 1;
        }
        else
        {
            g_btmanager_gl_var.send_vol_syn = 0;
        }
        if (g_btmanager_gl_var.avrcp_timer_id != -1)
        {
            com_btmanager_a2dp_handle_kill_avrcp_timer();
        }
    }
}

void __section__(".call_auto_conn1") com_hfp_old_sco_change(uint8 id)
{
    msg_apps_t msg;
    msg.type = MSG_BTSTACK_HFP_SCO_CHANGE_SYNC;
    //����ͬ����Ϣ
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[id].addr.bytes, &msg, NULL, 0);
}

void __section__(".rcode") com_btmanager_hfp_handle(void)
{
    uint8 i;

    //����ͨ���У���Ҫ����Ƿ���������ͨ��
    if (g_this_app_info->app_id != APP_ID_BTCALL)
    {
        for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
        {
            if ((g_bt_stack_cur_info.rmt_dev[i].hfp_status > HFP_STATUS_LINKED)
                || ((g_bt_stack_cur_info.rmt_dev[i].hfp_attr.old_sco_link == 0)
                    && (g_bt_stack_cur_info.rmt_dev[i].hfp_attr.sco_link == 1)))
            {
                com_hfp_old_sco_change(i);
                com_set_active_dev(MSG_BTSTACK_SET_HFP_ACTIVE_DEV_SYNC, i);
                com_call_handle();
                //PRINT_DBG_INT("call handle:", i);
                break;
            }
        }
    }
    else
    {
        if ((g_btmanager_gl_var.last_hfp_status[0] == HFP_STATUS_INCOMING)
                && (g_bt_stack_cur_info.rmt_dev[0].hfp_status != HFP_STATUS_INCOMING))
        {
            //�������л�Ϊ״̬��֪ͨ��ǰ̨���Ա�COMMON�ܹ�����λ
            com_call_handle();
        }
    }
    //��ͨ���У�����û��ʹ�����ᣬ��Ҫ��������ֻ�������״̬
//    else
//    {
//        if (g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].hfp_status == HFP_STATUS_PHONE)
//        {
//            for(i=0; i<g_btmanager_gl_var.support_dev_num; i++)
//            {
////                if (i == active)
////                {
////                    continue;
////                }
//                if (g_bt_stack_cur_info.rmt_dev[i].hfp_status > HFP_STATUS_LINKED)
//                {
//                    com_set_active_dev(MSG_BTSTACK_SET_HFP_ACTIVE_DEV_SYNC, i);
//                    PRINT_DBG_INT("switch active:", i);
//                    break;
//                }
//            }
//        }
//    }
}

void start_battery_report_update_timer(void)
{
    if (g_btmanager_gl_var.bat_rpt_timer_id != -1)
    {
        kill_app_timer(g_btmanager_gl_var.bat_rpt_timer_id);
        g_btmanager_gl_var.bat_rpt_timer_id = -1;
    }
    g_btmanager_gl_var.bat_rpt_timer_id = set_single_shot_app_timer_btmamager(APP_TIMER_ATTRB_COM_CONTROL \
    | (APP_TIMER_TAG_SYS << 8),1000, com_btmanager_hfp_battery_report_update_handle);
}

void start_battery_report_update_timer(void) __FAR__;

void __section__(".rcode") com_btmanager_battery_handle(void)
{
    uint8 i;
    uint8 update_flag = 0;

    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
        if (g_bt_stack_cur_info.hfp_established != 0)
        {
            if (g_app_info_state_all.battery_report_enable < g_btmanager_gl_var.support_dev_num)
            {
                com_btmanager_hfp_battery_report_enable(g_bt_stack_cur_info.rmt_dev[i].addr.bytes);
                update_flag = 1;
                g_app_info_state_all.battery_report_enable ++;
            }

            if (g_app_info_state.bat_value != g_btmanager_gl_var.last_battery_value)
            {
                update_flag = 1;
            }
        }
        else if(g_app_info_state_all.battery_report_enable != 0)
        {
            g_app_info_state_all.battery_report_enable = 0;
        }
        else
        {
            ;//for QAC
        }
        g_btmanager_gl_var.last_hfp_status[i] = g_bt_stack_cur_info.rmt_dev[i].hfp_status;
    }

    if (update_flag == 1)
    {
        start_battery_report_update_timer();
    }

    g_btmanager_gl_var.last_battery_value = g_app_info_state.bat_value;
}

void check_sniff_status_send_cmd(uint8 idx)
{
    PRINT_INFO_INT("check sniff:", idx);
    com_bt_sniff_control(g_bt_stack_cur_info.rmt_dev[idx].addr.bytes, 1);
}

void check_sniff_status_send_cmd(uint8 i) __FAR__;

void __section__(".rcode") check_sniff_status(void)
{
    uint8 i;

//#ifdef ENABLE_TRUE_WIRELESS_STEREO
//    if (g_bt_stack_cur_info.controller_status == BT_CONTROLLER_STATUS_IDLE)
//#else
    if ((g_bt_stack_cur_info.controller_status == BT_CONTROLLER_STATUS_IDLE)
            && (g_this_app_info->app_id != APP_ID_BTCALL))
//#endif
    {
        for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if ((g_bt_stack_cur_info.rmt_dev[i].sniff_status == 0)
                    && ((g_bt_stack_cur_info.rmt_dev[i].a2dp_status != A2DP_STATUS_NONE)
                            || (g_bt_stack_cur_info.rmt_dev[i].hfp_status == HFP_STATUS_LINKED))
                    && ((g_bt_stack_cur_info.dev_role == 0)
                          /*|| (g_bt_stack_cur_info.rmt_dev[i].dev_type == NO_TWS_DEV)*/))
#else
            if ((g_bt_stack_cur_info.rmt_dev[i].sniff_status == 0)
                    && ((g_bt_stack_cur_info.rmt_dev[i].a2dp_status != A2DP_STATUS_NONE)
                            || (g_bt_stack_cur_info.rmt_dev[i].hfp_status == HFP_STATUS_LINKED)))
#endif
            {
//#ifdef ENABLE_TRUE_WIRELESS_STEREO
              //do no    
//#else
              check_sniff_status_send_cmd(i);
//#endif
            }
        }
    }
    g_btmanager_gl_var.sniff_timer_id = -1;
}

void __section__(".rcode") com_btmanager_sniff_handle(void)
{
    //5s
    if ((g_bt_stack_cur_info.conn_status == CONN_STATUS_LINKED)
            && (g_bt_stack_cur_info.controller_status == BT_CONTROLLER_STATUS_IDLE))
    {
        if (g_btmanager_gl_var.sniff_timer_id == -1)
        {
            g_btmanager_gl_var.sniff_timer_id = set_single_shot_app_timer_btmamager(APP_TIMER_ATTRB_COM_CONTROL \
            | (APP_TIMER_TAG_SYS << 8),2000, check_sniff_status);
        }
    }
}

void change_btmanager_status(btmanager_status_e status)
{
    if (status == BTMANAGER_STATUS_IDLE)
    {
        adjust_freq_set_level(AP_BT_HIGH_PRIO, FREQ_LEVEL1, FREQ_NULL);
        g_btmanager_gl_var.btmanager_status = BTMANAGER_STATUS_IDLE;
        PRINT_INFO("BT MANAGER IDLE");
    }
    else if (status == BTMANAGER_STATUS_WORKING)
    {
        adjust_freq_set_level(AP_BT_HIGH_PRIO, FREQ_LEVEL7, FREQ_NULL);
        g_btmanager_gl_var.btmanager_status = BTMANAGER_STATUS_WORKING;
        PRINT_INFO("BT MANAGER WORKING");
    }
    else if (status == BTMANAGER_STATUS_LINKING)
    {
        if (g_btmanager_gl_var.btmanager_status != BTMANAGER_STATUS_LINKING)
        {
            adjust_freq_set_level(AP_BT_HIGH_PRIO, FREQ_LEVEL10, FREQ_NULL);
            g_btmanager_gl_var.btmanager_status = BTMANAGER_STATUS_LINKING;
            PRINT_INFO("BT MANAGER LINKING");
            g_btmanager_gl_var.linking_timer = 0;
        }
    }
    else
    {
        adjust_freq_set_level(AP_BT_HIGH_PRIO, FREQ_LEVEL11, FREQ_NULL);
        g_btmanager_gl_var.btmanager_status = BTMANAGER_STATUS_POWERON;
        PRINT_INFO("BT MANAGER POWERON");
    }
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO

void __section__(".bank") com_btmanager_ble_deal(void)
{
	 if(g_bt_stack_cur_info.dev_role==TWS_SLAVE)
	 {
	     if(g_bt_stack_cur_info.ble_con_flag==0)
	     {
	     	   //disable advertise
	     	   com_set_ble_discoverable(0);
	     	   g_btmanager_gl_var.last_role = g_bt_stack_cur_info.dev_role;
	     }
	     else 
	     {
	     	  //disconect ble
	     	  com_btmanager_unlink_spp_ble_profile(1);
	     }
	 }
	 else 
	 {
	     if(g_btmanager_gl_var.last_role == TWS_SLAVE)
	     {
	         //disable advertise
	         com_set_ble_discoverable(1);
	     }
	     g_btmanager_gl_var.last_role = g_bt_stack_cur_info.dev_role;
	 }
	 
}

void com_btmanager_ble_deal(void) __FAR__;

#endif //#ifdef ENABLE_TRUE_WIRELESS_STEREO

app_result_e __section__(".rcode") com_btmanager_loop(bool tts_flag)
{
    btstack_event_t btstack_ev;
    app_result_e result = RESULT_NONE_EVENT;
    uint8 i;

    if (g_app_info_vector[APP_TYPE_BTSTACK].used == 0)
    {
        return RESULT_NONE_EVENT;
    }

    get_bt_stack_info();

    if (g_bt_stack_cur_info.conn_status == CONN_STATUS_NONE)
    {
        return RESULT_NONE_EVENT;
    }

    if (g_bt_stack_cur_info.controller_status == BT_CONTROLLER_STATUS_IDLE)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
		for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
		{
		        // if ((g_bt_stack_cur_info.conn_status != CONN_STATUS_LINKED))
	            //    || (g_bt_stack_cur_info.rmt_dev[i].sniff_status == 1)||(g_bt_stack_cur_info.dev_role==2)
	            //    || (g_bt_stack_cur_info.rmt_dev[i].sniff_status == 1)||(g_bt_stack_cur_info.dev_role!=0)
	            //    ||(g_bt_stack_cur_info.tws_master_phonecon==1)) //TODO ���ֻ�����֧��
		        {
		            //����BT MANAGER״̬Ϊ�����С�
		            if (g_btmanager_gl_var.btmanager_status != BTMANAGER_STATUS_IDLE)
		            {
		                change_btmanager_status(BTMANAGER_STATUS_IDLE);
		            }
		        }
		}
#else
        if ((g_bt_stack_cur_info.conn_status != CONN_STATUS_LINKED)
            || (g_bt_stack_cur_info.rmt_dev[0].sniff_status == 1)) //TODO ���ֻ�����֧��
        {
            //����BT MANAGER״̬Ϊ�����С�
            if (g_btmanager_gl_var.btmanager_status != BTMANAGER_STATUS_IDLE)
            {
                change_btmanager_status(BTMANAGER_STATUS_IDLE);
            }
        }

#endif
	
    }
    else
    {
        //����BT MANAGER״̬Ϊ��������
        if (g_btmanager_gl_var.btmanager_status == BTMANAGER_STATUS_IDLE)
        {
            change_btmanager_status(BTMANAGER_STATUS_WORKING);
        }
    }

    if (g_bt_stack_cur_info.num_connected > g_btmanager_gl_var.last_num_connected)
    {
        //����BT MANAGER״̬Ϊ�����ӡ�
        change_btmanager_status(BTMANAGER_STATUS_LINKING);
    }
    g_btmanager_gl_var.last_num_connected = g_bt_stack_cur_info.num_connected;

    if (g_bt_stack_cur_info.conn_status != g_btmanager_gl_var.last_conn_status)
    {
      //  libc_print("--Last_con--", g_btmanager_gl_var.last_conn_status,2);
        com_btmanager_conn_status_change_handle(FALSE);
        result = RESULT_NULL; //��ʾ���¼����������ǲ���Ҫ�������������������ģ��ҲӦ�ò������Ʒ�ʽ��ʾ���¼�����
    }
    g_btmanager_gl_var.last_conn_status = g_bt_stack_cur_info.conn_status;

    com_btmanager_discoverable_handle();
    com_btmanager_a2dp_handle();
    com_btmanager_hfp_handle();
    com_btmanager_battery_handle();
    com_btmanager_sniff_handle();

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    com_btmanager_mod_change();
    if(g_btmanager_gl_var.last_role != g_bt_stack_cur_info.dev_role)
    {
        com_btmanager_ble_deal();
    }    
#endif

    //��������Э��ջ�¼�������RCP�¼�
    if (tts_flag == TRUE)
    {
        if (sys_mq_traverse(MQ_ID_RES, (void *) &btstack_ev, 0) > 0)
        {
            result = com_btmanager_event_check_for_tts(&btstack_ev);
            if (result == RESULT_FORCE_TTS_STOP)
            {
                goto bt_manager_loop_exit;
            }
        }
    }

    if (sys_mq_receive((uint32) (MQ_ID_RES), (void *) &btstack_ev) == 0)
    {
        result = com_btmanager_event_dispatch(&btstack_ev, tts_flag);
    }

    bt_manager_loop_exit:
    g_btmanager_gl_var.last_a2dp_status = g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].a2dp_status;

    return result;
}

void com_btmanager_set_scan_mode(uint8 mode)
{
    msg_apps_t msg;

    //g_btmanager_gl_var.discoverable_flag = mode;
    //0 is default mode 1 is fast mode 

    msg.type = MSG_BTSTACK_SET_SCAN_MODE;
    msg.content.data[0] = mode;

    send_sync_msg_btmanager(NULL, &msg, NULL, 0);
}


void __section__(".bank") com_btmanager_fast_discover_timer_proc(void)
{
	  g_fast_discoverable_cnt++;
	  if(g_fast_discoverable_cnt>=50)
	  {
	  	  kill_app_timer(g_fast_discoverable_timer);
        g_fast_discoverable_timer = -1;
        //send message to bt stack to change back san time
        com_btmanager_set_scan_mode(0);
	  } 
}


void __section__(".bank") com_btmanager_fast_discover_deal(uint8 type)
{
	   //init 
	   if(type == 1)
	   {
	       g_fast_discoverable_cnt = 0;
	       g_fast_discoverable_timer = set_app_timer(APP_TIMER_ATTRB_COM_CONTROL, 1000, com_btmanager_fast_discover_timer_proc);
	       //send message to bt stack to change back san time
	       com_btmanager_set_scan_mode(1);
	   }
	   else 
	   {
	   	  kill_app_timer(g_fast_discoverable_timer);
        g_fast_discoverable_timer = -1;
        //send message to bt stack to change back san time
        com_btmanager_set_scan_mode(0);
	   }
}

//��ʲô��Ҫϵͳ����Ŀ���ͨ������ϵͳ��Ϣ��ʵ��
void com_btmanager_conn_status_change_handle(bool init_flag)
{
    msg_apps_t msg;
    libc_print("--conn--",g_bt_stack_cur_info.conn_status,2);
    switch (g_bt_stack_cur_info.conn_status)
    {
        case CONN_STATUS_NONE:
        g_btmanager_gl_var.autoconnect_link_succ_flag = 0;
        g_need_reset_controller_timeout = 0;

        //�ر�����ָʾ��
        discrete_led_set(LED_ID_BT_LINK, LED_ALWAYS_OFF, NULL);
        break;

        case CONN_STATUS_WAIT_PAIR:
        g_btmanager_gl_var.autoconnect_link_succ_flag = 0;
        g_need_reset_controller_timeout = 0;

        {
            led_duty_t bt_link_duty;

            bt_link_duty.cycle_count = -1;
            bt_link_duty.duty_on_time = 500;
            bt_link_duty.duty_off_time = 500;
            discrete_led_set(LED_ID_BT_LINK, LED_TWINKLE, &bt_link_duty);
        }

        if (init_flag == FALSE)
        {
            if (g_btmanager_gl_var.last_conn_status == CONN_STATUS_NONE)
            {
                //���������ȴ�����
                #if (SUPPORT_BT_COMMON_TTS == 1)
                com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_USEFIFO, (void *) TTS_WAIT_BT_CON);
                #endif
            }
            else
            {
                //�������������Ͽ�&�ȴ�����
                #if (SUPPORT_BT_COMMON_TTS == 1)
                com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_USEFIFO, (void *) TTS_BT_DISCON);
                com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_USEFIFO, (void *) TTS_WAIT_BT_CON);
                #endif
                #if (SUPPORT_BT_COMMON_KT == 1)
                keytone_play(KEYTONE_BTUNLINK, KEYTONE_NOBLOCK);
                #endif

                msg.type = MSG_BT_DISCONNECT;
                sys_mq_send(MQ_ID_SYS, &msg);
            }
        }
        //for set fast timer
        if(g_fast_discoverable_timer == -1)
        {
            //g_fast_discoverable_timer = set_app_timer(APP_TIMER_ATTRB_COM_CONTROL, 1000, com_btmanager_autoconnect);
            com_btmanager_fast_discover_deal(1);
        } 
        break;

        case CONN_STATUS_LINKED:
        g_btmanager_gl_var.autoconnect_link_succ_flag = 0;
        g_need_reset_controller_timeout = 0;

        //��������ָʾ��
        discrete_led_set(LED_ID_BT_LINK, LED_ALWAYS_ON, NULL);

        if (init_flag == FALSE)
        {
            //�������ӳɹ�
            #if (SUPPORT_BT_COMMON_TTS == 1)
            com_tts_state_play(TTS_MODE_ONLYONE | TTS_MODE_USEFIFO, (void *) TTS_BT_CON_OK);
            #endif
            #if (SUPPORT_BT_COMMON_KT == 1)
            keytone_play(KEYTONE_BTLINK, KEYTONE_NOBLOCK);
            #endif

            msg.type = MSG_BT_CONNECTED;
            sys_mq_send(MQ_ID_SYS, &msg);
        }
        
        if(g_fast_discoverable_timer != -1)
        {
        	  com_btmanager_fast_discover_deal(0);
        }
        
        break;

        case CONN_STATUS_ERROR:
        g_btmanager_gl_var.autoconnect_link_succ_flag = 0;
        g_need_reset_controller_timeout = 0;

        if (init_flag == FALSE)
        {
            com_btstack_error_deal();
        }
        break;

        default:
        break;
    }
}

//��ʲô��Ҫϵͳ����Ŀ���ͨ������ϵͳ��Ϣ��ʵ��
void com_btstack_error_deal(void)
{
    //Ӳ�����������˳��������ò���ʧ��
    if (g_bt_stack_cur_info.err_status == BT_ERR_HW)
    {
        //quit_bt_app(); //ɱ��bt stack��������ͳһ���
    }
    else if (g_bt_stack_cur_info.err_status == BT_ERR_PAGE_TIMEOUT)
    {
        //bt_clear_error(); //error������ǰ̨�����Ҫ��Э��ջ�Լ�����

        //�ȴ�20s����

    }
    else
    {
        ; //nothing for QAC
    }
}
