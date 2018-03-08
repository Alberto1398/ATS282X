/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Զ�����Զ���豸����ģ�飬��������ͨ����ģ�鶨�ƣ���ģ����Ҫ����2Hz��
 *       ��ʱ�����С�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "common_func.h"

//����������Ҫʹ��USB��ⶨʱ�������ǻ�ֱ����ǰ̨����Ϣѭ���л�ȡbtstack״̬���������Ƿ���Ҫ����
//������ʱ�����Բ���0.5S��ʱ��������btplay�й�0.5S��ʱ�����л���

//com_connect_tick_cnt������ʱ�䵥λ ms
#define COM_AUTOCONNECT_TICK_INTEVAL  (500)

//�������ӵ�Ƶ��30���Ӻ�ͻὫƵ��ǿ�Ƶ���WORKING
#define LINKING_TO_WORKING_TIMER      (60)


void check_auto_con_flag()__FAR__;
//�����������
#ifdef ENABLE_TRUE_WIRELESS_STEREO
void com_exchange_vale(auto_conn_dev_info_t *p_dev_info,uint8 i,uint8 j)__FAR__;

extern uint8 check_id1;
extern uint8 check_id2;
extern uint8 once_auto_flag;

uint8 __section__(".text.auto_connect_bank") com_get_dev_conn_init(void)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;
    uint8 already_conn_init;
    temp_reply.content = &already_conn_init;
    
    msg.type = MSG_BTSTACK_GET_BTSTACK_CONN_INIT_SYNC;
    
    send_sync_msg_btmanager(g_bt_auto_connect_addr
        , &msg, &temp_reply, 0);
   
    return already_conn_init;
}

#endif

#ifndef ENABLE_TRUE_WIRELESS_STEREO
uint8 __section__(".text.auto_connect_bank") auto_connect_deal(void)
{
    uint8 i;

    PRINT_DBG("20s");

    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
        if (g_bt_auto_connect_ctrl.dev_info[i].conn_flag == 0)
        {
            continue;
        }

        PRINT_DBG_INT("i=", i);
        //������������
        //���ֻ���ģʽ
        if ((g_bt_auto_connect_ctrl.auto_connect_type == AUTO_CONNECT_TYPE_TIMEOUT)
                && (g_bt_auto_connect_ctrl.auto_connect_count_timeout != 0))
        {
            if ((g_bt_auto_connect_ctrl.dev_info[i].connect_cnt >= g_bt_auto_connect_ctrl.auto_connect_count_timeout))
            {
                PRINT_DBG_INT("cnt:", g_bt_auto_connect_ctrl.dev_info[i].connect_cnt);
                goto NEED_TO_STOP_AUTOCONNECT;
            }
        }
        else if ((g_bt_auto_connect_ctrl.auto_connect_type == AUTO_CONNECT_TYPE_MODE)
                && (g_bt_auto_connect_ctrl.auto_connect_count_mode != 0))
        { //�������޴�
            if ((g_bt_auto_connect_ctrl.dev_info[i].connect_cnt >= g_bt_auto_connect_ctrl.auto_connect_count_mode))
            {
                PRINT_DBG_INT("mode:", g_bt_auto_connect_ctrl.dev_info[i].connect_cnt);
                goto NEED_TO_STOP_AUTOCONNECT;
            }
        }
        else if ((g_bt_auto_connect_ctrl.auto_connect_type == AUTO_CONNECT_TYPE_STARTUP)
                && (g_bt_auto_connect_ctrl.auto_connect_count_startup != 0))
        {
            if ((g_bt_auto_connect_ctrl.dev_info[i].connect_cnt >= g_bt_auto_connect_ctrl.auto_connect_count_startup))
            {
                PRINT_DBG_INT("start:", g_bt_auto_connect_ctrl.dev_info[i].connect_cnt);
                goto NEED_TO_STOP_AUTOCONNECT;
            }
        }
        else
        {
            //do nothing for QAC
        }

        g_bt_auto_connect_ctrl.dev_info[i].connect_cnt++;

        if (((g_need_reset_controller_timeout == 1) && (g_bt_auto_connect_ctrl.dev_info[i].connect_cnt > 3))
                || (g_bt_auto_connect_ctrl.dev_info[i].connect_cnt >= 100))
        {
            //ת����COMMON����COMMON����װ��BT STACK������Ҫ�ָ�����
            msg_apps_t msg;

            if (g_this_app_info->app_id == APP_ID_BTPLAY)
            {
                msg.type = MSG_BTSTACK_ERR_HARDWARE_EXCEPTION;
            }
            else
            {
                msg.type = MSG_BTSTACK_ERR_HARDWARE;
            }        

            broadcast_msg(&msg, TRUE);

            goto NEED_TO_STOP_AUTOCONNECT;
        }

        libc_memcpy(g_bt_auto_connect_addr, g_bt_auto_connect_ctrl.dev_info[i].remote_addr.bytes, BD_ADDR_LEN);

        if (((g_bt_auto_connect_ctrl.dev_info[i].support_profile & (uint8) HFP_SUPPORTED) != 0)
        /*&& ((g_bt_stack_cur_info.serv_conn & HFP_CONNECTED) == 0)*/)
        {
            return AUTO_CONNECT_NEED_CONNECT_HFP; //ÿ��ֻ��һ���豸
        }
    #ifdef __SUPPORT_HID_
        else if ((g_bt_auto_connect_ctrl.dev_info[i].support_profile & (uint8) A2DP_SUPPORTED) != 0)
        {
            return AUTO_CONNECT_NEED_CONNECT_A2DP; //ÿ��ֻ��һ���豸 
        }
        else //if((g_bt_auto_connect_ctrl.dev_info[i].support_profile & (uint8) HID_SUPPORTED) != 0)
        {
            return AUTO_CONNECT_NEED_CONNECT_HID; //ÿ��ֻ��һ���豸 
        }
    #else
        else //if ((g_bt_auto_connect_ctrl.dev_info[i].support_profile & (uint8) A2DP_SUPPORTED) != 0)
        {
            return AUTO_CONNECT_NEED_CONNECT_A2DP; //ÿ��ֻ��һ���豸 
        }
    #endif

        NEED_TO_STOP_AUTOCONNECT:
        //ֹͣ�Զ�����
        g_bt_auto_connect_ctrl.dev_info[i].connect_cnt = 0;
        g_bt_auto_connect_ctrl.dev_info[i].conn_flag = 0;
        return 0;
    }
}
#else

//��������޸Ĵ���
uint8 __section__(".text.auto_connect_bank") auto_connect_deal(void)
{
    uint8 i,j=0,ret=0;
    uint8 tmp_already_conn_init = 0;
    uint8 tmp_auto_connect_count_startup = g_bt_auto_connect_ctrl.auto_connect_count_startup;

  //  PRINT_DBG("20s");

    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {

        if(check_id1 >= g_btmanager_gl_var.support_dev_num)
        {
            check_id1 = 0;
        }
                            
        if (g_bt_auto_connect_ctrl.dev_info[check_id1].conn_flag != 1)
        {
            j = j + 1;     
            if(j == g_btmanager_gl_var.support_dev_num)
            {
                g_bt_auto_connect_ctrl.need_auto_connect = 0;
            }
            
            if(++check_id1 >= g_btmanager_gl_var.support_dev_num)
            {
                check_id1 = 0;
            }
            continue;
        }
        libc_print("check_id1 =",check_id1,2);

       // PRINT_DBG_INT("i=", i);
        //������������
        //���ֻ���ģʽ
        if ((g_bt_auto_connect_ctrl.auto_connect_type == AUTO_CONNECT_TYPE_TIMEOUT)
                && (g_bt_auto_connect_ctrl.auto_connect_count_timeout != 0))
        {
            if ((g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt >= g_bt_auto_connect_ctrl.auto_connect_count_timeout))
            {
                //PRINT_DBG_INT("cnt:", g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt);
                goto NEED_TO_STOP_AUTOCONNECT;
            }
        }
        else if ((g_bt_auto_connect_ctrl.auto_connect_type == AUTO_CONNECT_TYPE_MODE)
                && (g_bt_auto_connect_ctrl.auto_connect_count_mode != 0))
        { //�������޴�
            if ((g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt >= g_bt_auto_connect_ctrl.auto_connect_count_mode))
            {
              //  PRINT_DBG_INT("mode:", g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt);
                goto NEED_TO_STOP_AUTOCONNECT;
            }
        }
        else if ((g_bt_auto_connect_ctrl.auto_connect_type == AUTO_CONNECT_TYPE_STARTUP)
                && (g_bt_auto_connect_ctrl.auto_connect_count_startup != 0))
        {
            if ((g_bt_auto_connect_ctrl.dev_info[check_id1].dev_type == TWS_DEV)
                  && (g_bt_auto_connect_ctrl.tws_prior_role == TWS_MASTER))
            {
                 tmp_auto_connect_count_startup = g_bt_auto_connect_ctrl.auto_connect_count_startup + 3;
            }    
            if ((g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt >= tmp_auto_connect_count_startup))
            {
             //   PRINT_DBG_INT("start:", g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt);
                goto NEED_TO_STOP_AUTOCONNECT;
            }
        }
        else
        {
            //do nothing for QAC
        }

        g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt++;

/*        if (((g_need_reset_controller_timeout == 1) && (g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt > 3))
                || (g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt >= 100))
        {
            //ת����COMMON����COMMON����װ��BT STACK������Ҫ�ָ�����
            msg_apps_t msg;

            msg.type = MSG_BTSTACK_ERR_HARDWARE_EXCEPTION;
            broadcast_msg(&msg, TRUE);

            goto NEED_TO_STOP_AUTOCONNECT;
        }
*/
        libc_memcpy(g_bt_auto_connect_addr, g_bt_auto_connect_ctrl.dev_info[check_id1].remote_addr.bytes, BD_ADDR_LEN);

    //ȷ��һ�λ���û�ɹ�����ô�ͻ�����һ���豸  !!
        tmp_already_conn_init = com_get_dev_conn_init();
        if((tmp_already_conn_init == 1) && (g_bt_stack_cur_info.rmt_dev[check_id1].serv_conn == 0))
        {
            //��������䶼���ӹ��ֻ�����һֱcheck_id1 =0������
            //if(++check_id1 >= g_btmanager_gl_var.support_dev_num)
            if(check_id1 >= g_btmanager_gl_var.support_dev_num)
            {
                check_id1 = 0;
            }
            continue;
        }

        //if (((g_bt_auto_connect_ctrl.dev_info[check_id1].support_profile & (uint8) HFP_SUPPORTED) != 0)
        //if ((((g_bt_auto_connect_ctrl.dev_info[check_id1].support_profile & (uint8) HFP_SUPPORTED) != 0)&&(g_bt_auto_connect_ctrl.dev_info[check_id1].dev_type!=TWS_DEV))
        if (((g_bt_auto_connect_ctrl.dev_info[check_id1].support_profile & (uint8) HFP_SUPPORTED) != 0))
            // && (( g_bt_stack_cur_info.rmt_dev[check_id1].serv_conn & HFP_CONNECTED) == 0))
        {
            ret=AUTO_CONNECT_NEED_CONNECT_HFP; //ÿ��ֻ��һ���豸
        }    
        else //if ((g_bt_auto_connect_ctrl.dev_info[i].support_profile & (uint8) A2DP_SUPPORTED) != 0)
        {
            ret=AUTO_CONNECT_NEED_CONNECT_A2DP; //ÿ��ֻ��һ���豸 
        }
        //else //if((g_bt_auto_connect_ctrl.dev_info[i].support_profile & (uint8) HID_SUPPORTED) != 0)
        //{
        //    ret=AUTO_CONNECT_NEED_CONNECT_HID; //ÿ��ֻ��һ���豸 
        //}
        
        if(ret != 0)
        {
            if(++check_id1 >= g_btmanager_gl_var.support_dev_num)
            {
                check_id1 = 0;
            }
            return ret;
        }
        
        NEED_TO_STOP_AUTOCONNECT:
        //ֹͣ�Զ�����
        g_bt_auto_connect_ctrl.dev_info[check_id1].connect_cnt = 0;
        g_bt_auto_connect_ctrl.dev_info[check_id1].conn_flag = 0;
        if(++check_id1 >= g_btmanager_gl_var.support_dev_num)
        {
            check_id1 = 0;
        }
        return 0;

    }
}
#endif

//����Ƿ��з���û������
uint8 __section__(".text.auto_connect_bank") connect_remain_profile_bank(auto_conn_dev_info_t *p_dev_info,
        uint8 need_connect)
{
    if (need_connect == AUTO_CONNECT_NEED_CONNECT_A2DP)
    {
        //try to connect again, up to the reconnect another limit
        if ((g_bt_auto_connect_ctrl.reconnect_another_limit == 0) ||
                (p_dev_info->reconnect_another_cnt < g_bt_auto_connect_ctrl.reconnect_another_limit))
        {
            /* if phone is ongoing, wait.  */
            if (g_bt_stack_cur_info.conn_status == CONN_STATUS_LINKED)
            {
                p_dev_info->onlyone_disconnect_cnt = 0;
                p_dev_info->reconnect_another_cnt++;

                return AUTO_CONNECT_NEED_CONNECT_A2DP;
            }
        }
    }
    else if(need_connect == AUTO_CONNECT_NEED_CONNECT_HID)
    {
        //
                //try to connect again, up to the reconnect another limit
        if ((g_bt_auto_connect_ctrl.reconnect_another_limit == 0) ||
                (p_dev_info->reconnect_another_cnt < g_bt_auto_connect_ctrl.reconnect_another_limit))
        {
            p_dev_info->onlyone_disconnect_cnt = 0;
            p_dev_info->reconnect_another_cnt++;

            return AUTO_CONNECT_NEED_CONNECT_HID;
        }
    }
    else 
    {
        //try to connect again, up to the reconnect another limit
        if ((g_bt_auto_connect_ctrl.reconnect_another_limit == 0) ||
                (p_dev_info->reconnect_another_cnt < g_bt_auto_connect_ctrl.reconnect_another_limit))
        {
            p_dev_info->onlyone_disconnect_cnt = 0;
            p_dev_info->reconnect_another_cnt++;

            return AUTO_CONNECT_NEED_CONNECT_HFP;
        }
    }

    return 0;
}



void __section__(".auto_connect_bank1") check_auto_con_flag()
{
    uint8 i,j=0;
    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
        if (libc_memcmp(g_bt_auto_connect_ctrl.dev_info[i].remote_addr.bytes,
                g_bt_stack_cur_info.rmt_dev[i].addr.bytes, 6) == 0)
        {
            if (((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HFP_CONNECTED) != 0)
                    || ((g_bt_stack_cur_info.rmt_dev[i].serv_conn & A2DP_CONNECTED) != 0))
            {
                //����Ѿ����ϣ����һ�£������Ͳ���Ҫ�ٻ�����
                g_bt_auto_connect_ctrl.dev_info[i].connect_cnt = 0;
                g_bt_auto_connect_ctrl.dev_info[i].conn_flag = 0;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                if ((g_bt_auto_connect_ctrl.dev_info[1-i].conn_flag == 1)
                      && (g_bt_auto_connect_ctrl.need_auto_connect == 1)
                      && (once_auto_flag == 0))
                {
                    g_bt_auto_connect_ctrl.connect_tick_cnt = g_bt_auto_connect_ctrl.auto_connect_interval * 2;
                    once_auto_flag = 1;
                   // PRINT_INFO("hgg");
                }    
#endif    
            }
        }
    }  
}


bool __section__(".text.auto_connect_profile_bank") stop_auto_con(uint8 i)
{
    uint8 h;
    bool ret=TRUE;
    if(g_btmanager_gl_var.support_dev_num==1)
    {
         //�ҵ��ͺ�Զ���豸��ͬ��ַ�Ļ����豸id
        for(h=0;h<2;h++)
        {
            g_bt_auto_connect_ctrl.dev_info[h].connect_cnt = 0;
            g_bt_auto_connect_ctrl.dev_info[h].conn_flag = 0;
        }  
    }
    else
    {
        //�ҵ��ͺ�Զ���豸��ͬ��ַ�Ļ����豸id
        for(h=0;h<2;h++)
        {
              if (libc_memcmp(g_bt_auto_connect_ctrl.dev_info[h].remote_addr.bytes,
                g_bt_stack_cur_info.rmt_dev[i].addr.bytes, 6) == 0)
              {
                 break;
              } 
        }
        if(h>=2)
        {
            ret=FALSE;
            return ret;
        }
        g_bt_auto_connect_ctrl.dev_info[h].connect_cnt = 0;
        g_bt_auto_connect_ctrl.dev_info[h].conn_flag = 0;
    }
    return ret;
}


#ifdef ENABLE_TRUE_WIRELESS_STEREO

//����Ƿ��з���û������
void __section__(".auto_connect_bank1") com_exchange_vale(auto_conn_dev_info_t *p_dev_info,uint8 i,uint8 j)
{
    #if 1
     libc_print("\nexchange i=:",i,2);
    libc_memcpy(p_dev_info, &g_bt_auto_connect_ctrl.dev_info[i], sizeof(auto_conn_dev_info_t));
    libc_memcpy(p_dev_info->remote_addr.bytes,g_bt_auto_connect_ctrl.dev_info[i].remote_addr.bytes, 6);

    libc_memcpy(&g_bt_auto_connect_ctrl.dev_info[i], &g_bt_auto_connect_ctrl.dev_info[j], sizeof(auto_conn_dev_info_t));
    libc_memcpy(g_bt_auto_connect_ctrl.dev_info[i].remote_addr.bytes, g_bt_auto_connect_ctrl.dev_info[j].remote_addr.bytes, 6);

    libc_memcpy(&g_bt_auto_connect_ctrl.dev_info[j], p_dev_info, sizeof(auto_conn_dev_info_t));
    libc_memcpy(g_bt_auto_connect_ctrl.dev_info[j].remote_addr.bytes,  p_dev_info->remote_addr.bytes, 6);
    #endif
}

//����Ƿ��з���û������
uint8 connect_remain_profile(void)
{
    
    uint8 i;
    uint8 need_connect = 0;
    uint8 tmp_already_conn_init = 0;

    if(check_id2 >= g_btmanager_gl_var.support_dev_num)
    {
        check_id2 = 0;
    }

    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
        libc_memcpy(g_bt_auto_connect_addr, g_bt_stack_cur_info.rmt_dev[check_id2].addr.bytes, BD_ADDR_LEN);

    //ȷ�����λ���û�ɹ���������һ���豸�ķ���
       // tmp_already_conn_init = com_get_dev_conn_init();
       // if((tmp_already_conn_init == 1) && (g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn == 0))
       if(g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn == 0)
        {
            if(++check_id2 >= g_btmanager_gl_var.support_dev_num)
            {
                check_id2 = 0;
            }
            continue;
        }
    //    libc_print("check_id2 =",check_id2,2);

        //������ߴ��䲻���tws�豸����
        if (!( ((g_bt_stack_cur_info.dev_role == 1) || (g_bt_stack_cur_info.dev_role == 2))
              &&(g_bt_stack_cur_info.rmt_dev[check_id2].dev_type == TWS_DEV)))
        {
        if ((g_bt_auto_connect_ctrl.enable_a2dp == 1)
                && ((g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn & A2DP_CONNECTED) == 0)
                && ((g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn & HFP_CONNECTED) != 0))
        {
            if (g_bt_auto_connect_ctrl.dev_info[check_id2].onlyone_disconnect_cnt > A2DP_CONNECT_TIME_AFTER_HFP_CONNECT)
            {
                need_connect = connect_remain_profile_bank(&g_bt_auto_connect_ctrl.dev_info[check_id2],
                        AUTO_CONNECT_NEED_CONNECT_A2DP);
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[check_id2].onlyone_disconnect_cnt++;
            }
        }
        else if ((g_bt_auto_connect_ctrl.enable_hfp == 1)
                && ((g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn & HFP_CONNECTED) == 0)
                && ((g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn & A2DP_CONNECTED) != 0))
        {
            if (g_bt_auto_connect_ctrl.dev_info[check_id2].onlyone_disconnect_cnt > HFP_CONNECT_TIME_AFTER_A2DP_CONNECT)
            {
                need_connect = connect_remain_profile_bank(&g_bt_auto_connect_ctrl.dev_info[check_id2],
                        AUTO_CONNECT_NEED_CONNECT_HFP);
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[check_id2].onlyone_disconnect_cnt++;
            }
        }
        else
        {
            g_bt_auto_connect_ctrl.dev_info[check_id2].reconnect_another_cnt = 0;
            g_bt_auto_connect_ctrl.dev_info[check_id2].onlyone_disconnect_cnt = 0;
        }
        }
        else
        {
               g_bt_auto_connect_ctrl.dev_info[check_id2].reconnect_another_cnt = 0;
               g_bt_auto_connect_ctrl.dev_info[check_id2].onlyone_disconnect_cnt = 0;
        }

        //���䲻���������
        if (!( (g_bt_stack_cur_info.dev_role == 1)
              &&(g_bt_stack_cur_info.rmt_dev[check_id2].dev_type == TWS_DEV) ))    
        {      
        //����Ƿ���Ҫ����AVRCP
        if (((g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn & AVRCP_CONNECTED_MY) == 0)
                && ((g_bt_stack_cur_info.rmt_dev[check_id2].serv_conn & A2DP_CONNECTED) != 0))
        //����A2DP��������AVRCPδ����
        {
            if (g_bt_auto_connect_ctrl.dev_info[check_id2].avrcp_disconnect_cnt > CHECK_AVRCP_CONNECT_TIME)
            {
                need_connect = AUTO_CONNECT_NEED_CONNECT_AVRCP;

                g_bt_auto_connect_ctrl.dev_info[check_id2].avrcp_disconnect_cnt = 0;
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[check_id2].avrcp_disconnect_cnt++;
            }
        }
        else
        {
            g_bt_auto_connect_ctrl.dev_info[check_id2].avrcp_disconnect_cnt = 0;
        }

        if (need_connect != 0)
        {
            break; //ÿ��ֻ��һ���豸
        }
        }
        else
        {
             g_bt_auto_connect_ctrl.dev_info[check_id2].avrcp_disconnect_cnt = 0;
        }    
        if(++check_id2 >= g_btmanager_gl_var.support_dev_num)
        {
            check_id2 = 0;
        }

    }
    return need_connect;
    
}
#else
//����Ƿ��з���û������
uint8 connect_remain_profile(void)
{
    uint8 i;
    uint8 need_connect = 0;

    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
        libc_memcpy(g_bt_auto_connect_addr, g_bt_stack_cur_info.rmt_dev[i].addr.bytes, BD_ADDR_LEN);

        if ((g_bt_auto_connect_ctrl.enable_a2dp == 1)
                && ((g_bt_stack_cur_info.rmt_dev[i].serv_conn & A2DP_CONNECTED) == 0)
                && (((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HFP_CONNECTED) != 0)
                    ||((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HID_CONNECTED)!=0)))
        {
            if (g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt > A2DP_CONNECT_TIME_AFTER_HFP_CONNECT)
            {
                need_connect = connect_remain_profile_bank(&g_bt_auto_connect_ctrl.dev_info[i],
                        AUTO_CONNECT_NEED_CONNECT_A2DP);
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt++;
            }
        }
        else if ((g_bt_auto_connect_ctrl.enable_hfp == 1)
                && ((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HFP_CONNECTED) == 0)
                && (((g_bt_stack_cur_info.rmt_dev[i].serv_conn & A2DP_CONNECTED) != 0)
                     ||((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HID_CONNECTED)!=0)))
        {
            if (g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt > HFP_CONNECT_TIME_AFTER_A2DP_CONNECT)
            {
                need_connect = connect_remain_profile_bank(&g_bt_auto_connect_ctrl.dev_info[i],
                        AUTO_CONNECT_NEED_CONNECT_HFP);
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt++;
            }
        }
        else if ((g_bt_auto_connect_ctrl.enable_hid == 1)
                && ((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HID_CONNECTED) == 0)
                && (((g_bt_stack_cur_info.rmt_dev[i].serv_conn & A2DP_CONNECTED) != 0)
                     ||((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HFP_CONNECTED)!=0)))
        {
            if (g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt > HID_CONNECT_TIME_AFTER_OTHER_CONNECT)
            {
                need_connect = connect_remain_profile_bank(&g_bt_auto_connect_ctrl.dev_info[i],
                        AUTO_CONNECT_NEED_CONNECT_HID);
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt++;
            }
        }
        else
        {
            g_bt_auto_connect_ctrl.dev_info[i].reconnect_another_cnt = 0;
            g_bt_auto_connect_ctrl.dev_info[i].onlyone_disconnect_cnt = 0;
        }

        //����Ƿ���Ҫ����AVRCP
        if (((g_bt_stack_cur_info.rmt_dev[i].serv_conn & AVRCP_CONNECTED_MY) == 0)
                && ((g_bt_stack_cur_info.rmt_dev[i].serv_conn & A2DP_CONNECTED) != 0))
        //����A2DP��������AVRCPδ����
        {
            if (g_bt_auto_connect_ctrl.dev_info[i].avrcp_disconnect_cnt > CHECK_AVRCP_CONNECT_TIME)
            {
                need_connect = AUTO_CONNECT_NEED_CONNECT_AVRCP;

                g_bt_auto_connect_ctrl.dev_info[i].avrcp_disconnect_cnt = 0;
            }
            else
            {
                g_bt_auto_connect_ctrl.dev_info[i].avrcp_disconnect_cnt++;
            }
        }
        else
        {
            g_bt_auto_connect_ctrl.dev_info[i].avrcp_disconnect_cnt = 0;
        }

        if (need_connect != 0)
        {
            break; //ÿ��ֻ��һ���豸
        }
    }

    return need_connect;
}
#endif

void check_connect_status(void)
{
    uint8 i, j = 0;
    auto_conn_dev_info_t tmp_dev;
    //PRINT_DATA(g_bt_auto_connect_ctrl.dev_info[0].remote_addr.bytes,6);
    //PRINT_DATA(g_bt_auto_connect_ctrl.dev_info[1].remote_addr.bytes,6);
    if (g_bt_stack_cur_info.num_connected == g_btmanager_gl_var.support_dev_num)
    {
        for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
        {
            if (((g_bt_stack_cur_info.rmt_dev[i].serv_conn & HFP_CONNECTED) != 0)
                    || ((g_bt_stack_cur_info.rmt_dev[i].serv_conn & A2DP_CONNECTED) != 0))
            {

                //��bank
                stop_auto_con(i);
                j++;
                //PRINT_INFO("clear flag");            
                //g_bt_auto_connect_ctrl.dev_info[i].connect_cnt = 0;
                //g_bt_auto_connect_ctrl.dev_info[i].conn_flag = 0;

            }
        }

        if ((j == g_btmanager_gl_var.support_dev_num)
                && (g_bt_auto_connect_ctrl.need_auto_connect == 1))
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
           once_auto_flag = 0;
#endif
            g_bt_auto_connect_ctrl.need_auto_connect = 0x0;
            PRINT_INFO("stop auto-conn1");
        }
        return;
    }

    if (g_bt_stack_cur_info.num_connected != 0)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        //������� �������������������ʱ����ô�Ͳ���ȥ�����ֻ�
        if((g_bt_stack_cur_info.dev_role == TWS_SLAVE)&&( g_bt_auto_connect_ctrl.need_auto_connect!=0))
        {
             g_bt_auto_connect_ctrl.need_auto_connect = 0x0;
             PRINT_INFO("stop auto-conn2");
        }
        //�����豸����˳��
        if(g_bt_stack_cur_info.dev_role==TWS_MASTER)
        {
            for (i = 0; i < g_bt_stack_cur_info.num_connected ; i++)
            {
                for(j = 0;j < g_btmanager_gl_var.support_dev_num;j++)
                {
                    //���������豸˳��
                    if ((libc_memcmp(g_bt_auto_connect_ctrl.dev_info[j].remote_addr.bytes,
                         g_bt_stack_cur_info.rmt_dev[i].addr.bytes, 6) == 0) && (i != j))
                    {
                        com_exchange_vale(&tmp_dev,i,j);
                        break;                        
                    }    
                }
             }
        }
#endif
        if(g_bt_auto_connect_ctrl.need_auto_connect!=0)
        {
            //��bank,��bank�ռ���� 
            check_auto_con_flag(); 
        }      
    }

    //���ÿ���豸������Ҫ��������ֹͣ����
    for (i = 0; i < g_btmanager_gl_var.support_dev_num; i++)
    {
        if (g_bt_auto_connect_ctrl.dev_info[i].conn_flag == 1)
        {
            break;
        }
    }
    if ((i == g_btmanager_gl_var.support_dev_num)
            && (g_bt_auto_connect_ctrl.need_auto_connect == 1))
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
       once_auto_flag = 0;
#endif
        g_bt_auto_connect_ctrl.need_auto_connect = 0x0;
#ifdef __ESD_MODE_
        g_end_autoconnt = 1;
#endif
        PRINT_INFO("stop auto-conn");
    }
}

void __section__(".text.auto_connect_bank") auto_connect_profile(uint8 need_connect)
{
    msg_apps_t conn_msg;

    g_btmanager_gl_var.autoconnect_link_succ_flag = 1;

    if ((need_connect & AUTO_CONNECT_NEED_CONNECT_HFP) != 0)
    {
        conn_msg.type = MSG_BTSTACK_HFP_CONNECT_SYNC;
    }
    else if ((need_connect & AUTO_CONNECT_NEED_CONNECT_A2DP) != 0)
    {
        conn_msg.type = MSG_BTSTACK_A2DP_CONNECT_SYNC;
    }
    else if((need_connect & AUTO_CONNECT_NEED_CONNECT_HID) != 0)
    {
        conn_msg.type = MSG_BTSTACK_HID_CONNECT_SYNC;
    }
    else //if ((need_connect & AUTO_CONNECT_NEED_CONNECT_A2DP) != 0)
    {
        conn_msg.type = MSG_BTSTACK_AVRCP_CONNECT_SYNC;
    }

    //����BT MANAGER״̬Ϊ�����ӡ�
    if (g_btmanager_gl_var.btmanager_status != BTMANAGER_STATUS_LINKING)
    {
        change_btmanager_status(BTMANAGER_STATUS_LINKING);
    }

    send_sync_msg_btmanager(g_bt_auto_connect_addr, &conn_msg, NULL, 0);
}

/*COMMON ��̨������ʱ�Ͽ������ͳһ����*/
void com_btmanager_autoconnect(void)
{
    uint8 need_connect = 0;

    //uint8 weixin_flag;

    //btstackû�г�ʼ����ɵĻ�,��ʱ�������,���ɹ�,��Ҫ���
    //20s�ŷ�����һ�λ���.�����˷�20s,�ӿ�����ɹ�ʱ��
    if (g_bt_stack_cur_info.conn_status == CONN_STATUS_NONE)
    {
        return;
    }

    check_connect_status();

    if (g_bt_auto_connect_ctrl.need_auto_connect == 1)
    {
        g_bt_auto_connect_ctrl.connect_tick_cnt++;
        if (g_bt_auto_connect_ctrl.connect_tick_cnt >= (g_bt_auto_connect_ctrl.auto_connect_interval * 2))
        {
            need_connect = auto_connect_deal();
            g_bt_auto_connect_ctrl.connect_tick_cnt = 0;
            g_ap_switch_var.tws_phon_come_flag=0;
        }
    }
    else
    {
        need_connect = connect_remain_profile();
        g_bt_auto_connect_ctrl.connect_tick_cnt = 0;
    }

    if (need_connect != 0)
    { //����ֻ��һ������Ҫ��������
        auto_connect_profile(need_connect);
    }

    if ((g_btmanager_gl_var.btmanager_status == BTMANAGER_STATUS_POWERON)   
      ||(g_btmanager_gl_var.btmanager_status == BTMANAGER_STATUS_LINKING))
    {
        g_btmanager_gl_var.linking_timer++;
        if (g_btmanager_gl_var.linking_timer > LINKING_TO_WORKING_TIMER)
        {
            change_btmanager_status(BTMANAGER_STATUS_WORKING);
        }
    }

    //APK/APPУ��
#ifndef SUPPORT_SPPBLE_TEST
    //weixin_flag = (uint8) com_get_config_default(BTSTACK_ENABLE_WEIXIN);
    //#if 0
    //if (weixin_flag == 0)
    //{
    com_btmanager_apk_app_verification_handle();
    //}
#endif
}
