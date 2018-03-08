/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������line in tws�Ƹ�����Ӧ��ҵ���߼���Ϣ��Ӧʵ�֡�
 * ���ߣ�
 ********************************************************************************/

#include  "linein_engine.h"

void btplay_engine_pipe_control(uint8 op)
{
  if ((g_p_bt_stack_cur_info->dev_role == TWS_SLAVE)
  	   &&(sample_start_flag == 1))
  {
  	libc_print("fd:",0,0);
  	sample_sound_exit();
  }	   
  
  if ((g_p_bt_stack_cur_info->dev_role != TWS_SLAVE)
  	  &&(op == PIPE_OP_UNFILTER))
  {
  	sample_sound_init();
    linein_tws_playpause_flag = 0;
    g_p_btplay_info->status = BTPLAY_PLAY;
    tws_sync_cmd_send(1);
  }
  else if	((g_p_bt_stack_cur_info->dev_role != TWS_SLAVE)
  	  &&(op == PIPE_OP_FILTER))
  {
  	 sample_sound_exit(); 
  	 libc_print("fd1:",0,0); 	
  }
  else
  {
  	 if ((op == PIPE_OP_FLUSH)
  	 	   &&(g_p_tws_info->tts_start_flag == 1))
  	 {
  	 	  g_p_tws_info->tts_start_flag = 0;
  	 }	   
  	 
  	 msg_apps_t msg;
     msg.content.data[0] = op;
     //��Ϣ����(����Ϣ����)
     msg.type = MSG_BTSTACK_PLAY_PIPE_CTRL_SYNC;

     //����ͬ����Ϣ
     send_sync_msg_btmanager(NULL, &msg, NULL, 0);  	
  }		    		     
}

void update_btstack_parm_control(uint8 flag)
{
	 msg_apps_t msg;
	 uint32 used_len;
	 
	 uint8 data[6];
	 
	 used_len = (ADC_SAMPLE_BUFFER_SIZE + 0x200);
	 
	 data[0] = flag;
	 data[1] = used_len &0xff;
	 data[2] = (used_len >> 8) &0xff;
	 data[3] = (used_len >> 16)&0xff;
	 data[4] = (used_len >> 24);
	 //data[5] = len & 0xff;
	 //data[6] = (len>>8) & 0xff;
	 
     //��Ϣ����(����Ϣ����)
     msg.type = MSG_BTSTACK_TWS_UPDATE_PARM;
     //msg.content.addr = &used_len;
     msg.content.addr = data;
     //����ͬ����Ϣ
     send_sync_msg_btmanager(NULL, &msg, NULL, 0); 
     libc_print("@lll:",*(uint32*)( msg.content.addr),2);
     update_pipe = *(uint32*)( msg.content.addr);
	
}

/*
 ���������tws�豸֮���ͨѶͨ��.
 ���͵ļ�ֵ��ֱ����g_p_tws_info->tws_sync_cmd
 ������,ʹ����Ϻ���Ҫ�����tws_sync_cmd��ֵ
 ����:
 1:������
 2:������ͣ
*/
void tws_sync_cmd_send(uint8 val)
{
	 if (g_p_bt_stack_cur_info->dev_role != 0)
	 {
	 	  msg_apps_t msg;
      msg.type = MSG_BTSTACK_TWS_SYNC_CMD;
      msg.content.data[0] = val;
      send_sync_msg_btmanager(NULL, &msg, NULL, 0);	 	
	 }		
}


void tws_filter_ctrl_send(uint8 val)
{
	 msg_apps_t msg;
   msg.type = MSG_BTSTACK_TWS_FILTER_CTRL_SYNC;
   msg.content.data[0] = val;
   send_sync_msg_btmanager(NULL, &msg, NULL, 0);	 		
}

void tws_sync_cmd_deal_sub(void)
{

    switch(g_p_tws_info->tws_sync_cmd)
    {
    	case TWS_START_PLAY_SYNC:
    	if (g_p_bt_stack_cur_info->dev_role == TWS_MASTER)
		{		   	
            sample_sound_init();
            tws_filter_ctrl_send(0);
		}
		
		linein_tws_playpause_flag = 0;
        g_p_btplay_info->status = BTPLAY_PLAY;
		g_p_tws_info->tws_sync_cmd = 0;	
		
    	break;
    	case TWS_STOP_PLAY_SYNC:
    	if (g_p_bt_stack_cur_info->dev_role == TWS_MASTER)
		{
			sample_sound_exit();
			tws_filter_ctrl_send(1);
		}
		linein_tws_playpause_flag = 1;
        g_p_btplay_info->status = BTPLAY_PAUSE;
		g_p_tws_info->tws_sync_cmd = 0;		
    	break;
    	case TWS_SLAVE_ENTER_SYNC:
    	//start adc
        libc_print("sync",g_p_tws_info->tws_sync_cmd,2);
        g_need_adc_flag = 1;
        g_p_tws_info->tws_sync_cmd = 0;	
    	break;

    	default:
    	break;
    }
	
}

#if 0
void deal_linein_usb_noise_sub(void)
{        
    g_check_count = 0;
    
    if (act_readb(BATADC_DATA) > BAT_HIGH_VAL)
    {
        act_writel(act_readl(CHG_CTL) & (~CHG_CTL_CHG_CURRENT_MASK), CHG_CTL);
        act_writel(act_readl(CHG_CTL) & (~(1 << CHG_CTL_CHGEN)), CHG_CTL);
        
        g_cha_need_restore = 1;
    }
    else if (act_readb(BATADC_DATA) < BAT_LOW_VAL)
    {
        act_writel((act_readl(CHG_CTL) & (~CHG_CTL_CHG_CURRENT_MASK)) | g_cha_current, CHG_CTL);  
    
        act_writel(act_readl(CHG_CTL) | (1 << CHG_CTL_CHGEN), CHG_CTL);

        g_cha_need_restore = 1;
    }	
}
#endif

void deal_linein_clear_filter(void)
{
	 if (clear_filter_timer_id != -1)
	 {
		  kill_app_timer(clear_filter_timer_id);
	 }	
	 clear_filter_timer_id = set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
                   500, Tws_clear_filter);	
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e btplay_reply_msg(void* msg_ptr, bool ret_vals)
 * \�ظ�ͬ����Ϣ
 * \param[in]    void  msg_ptr ��Ϣ�ṹ��
 * \param[in]    bool  ret_vals ����¼������� TRUE �ɹ� FALSE ʧ��
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR ����
 * \ingroup      mengine_control.c
 * \note  �ظ���Ϣʱ������ret_vals�Ľ��������Ӧ��ɹ���ʧ��
 */
/*******************************************************************************/
void btplay_reply_msg(void* msg_ptr, bool ret_vals)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    if (ret_vals == TRUE)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    libc_sem_post(data_ptr->sem);

    return;
}

app_result_e app_msg_deal_app_quit(void)
{
    libc_print("G3",0,0);
    btplay_engine_pipe_control(PIPE_OP_FILTER);

    if (btplay_gl_var.codec_flag == TRUE)
    {
        a2dp_decode_quit();
        g_p_btplay_info->status = BTPLAY_STOP;
    }
    btplay_engine_pipe_control(PIPE_OP_FLUSH);

    return RESULT_APP_QUIT;
}

//ʵ�ʵ�AVRCP������ǰ̨�������治����Ҫ����
app_result_e __section__(".text.bank4")app_message_deal(private_msg_t *pri_msg)
{
    app_result_e app_ret;
    bool ret_val;

    app_ret = RESULT_NULL;

    switch (pri_msg->msg.type)
    {

        case MSG_SET_DAE_CFG_SYNC: //����DAE
        if (mp_handle != NULL)
        {
            adjust_freq_add_value(0, g_dae_cfg_shm->run_freq);

            mmm_bp_cmd(mp_handle, MMM_BP_SET_EQ, (unsigned int) NULL);
        }
        btplay_reply_msg(pri_msg, TRUE);
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        case MSG_SWITCH_AOUT_START_SYNC: //׼���л���Ƶ���
        //��ʱֹͣ���裬����Ϣ��ͬ��STOP��Ϣ
        {

            btplay_engine_pipe_control(PIPE_OP_FILTER);
            if (btplay_gl_var.codec_flag == TRUE)
            {

                a2dp_decode_quit();
                ret_val = TRUE;
            }
            else
            {
                ret_val = TRUE;
            }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            send_tts_start_cmd();
#endif
            btplay_engine_pipe_control(PIPE_OP_FLUSH);

            btplay_gl_var.tts_play_flag = TRUE;
            btplay_reply_msg(pri_msg, ret_val);
        }
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        case MSG_SWITCH_AOUT_STOP_SYNC: //����л���Ƶ���
        //�ָ����裬����Ϣ��ͬ��PLAY��Ϣ
        {
            btplay_gl_var.tts_play_flag = FALSE;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (g_app_info_state_all.g_send_exit_mmm_tts == 1)
            {
              if (linein_tws_playpause_flag == 0)
              {
              	enable_unfilter_flag = 1;	
              }	              
            }	
            send_tts_stop_cmd();
#endif

            libc_print("@tts_stop:",btplay_gl_var.codec_flag,2);
         
            //��ͣģʽ��,��Ҫȥ��un_filter
            //������ͣģʽ��,�Ͽ���Ժ�,��ʼ����
            if (linein_tws_playpause_flag == 0)
            {
//               set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
//                                500, Tws_clear_filter);    
              deal_linein_clear_filter();        	
            }	

            btplay_reply_msg(pri_msg, TRUE);
        }
        break;

        case MSG_ENGINE_STANDBY_SYNC:
        {
            os_event_t *standby_sem = (os_event_t *) (pri_msg->msg.content.addr);

            //�����ֳ���׼������˯��ģʽ
       //     PRINT_INFO("eng stdby en");

            //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
            btplay_reply_msg(pri_msg, TRUE);

            libc_sem_wait(standby_sem, 0);

            //��˯��ģʽ�������ָ��ֳ�
           // PRINT_INFO("eng stdby exit");
            standby_restart_all_app_timer(g_app_timer_vector, g_app_timer_count);
        }
        break;

        case MSG_APP_QUIT: //�˳�����
        PRINT_INFO("linein tws quit");
        if ((pri_msg->msg.content.data[0] & 0x01) != 0)
        {
          //  PRINT_INFO("eg enter s3bt");
        }
        app_ret = app_msg_deal_app_quit();
        break;
        
        /*�������*/
        case MSG_LINEIN_EG_PLAY_SYNC: //��ʼ����
        
        tws_sync_cmd_send(1);
        linein_tws_playpause_flag = 0;
        g_p_btplay_info->status = BTPLAY_PLAY;
        if (g_p_bt_stack_cur_info->dev_role != 2)
        {
           sample_sound_init();
        }
        tws_filter_ctrl_send(0);
        btplay_reply_msg(pri_msg, TRUE);
        break;

        case MSG_LINEIN_EG_STOP_SYNC: //ֹͣ����
        
        tws_sync_cmd_send(2);
        sample_sound_exit();
        linein_tws_playpause_flag = 1;
        g_p_btplay_info->status = BTPLAY_PAUSE;
        tws_filter_ctrl_send(1);
        btplay_reply_msg(pri_msg, TRUE);
        break;

        default:
        btplay_reply_msg(pri_msg, TRUE);
        break;
    }

    return app_ret;
}
