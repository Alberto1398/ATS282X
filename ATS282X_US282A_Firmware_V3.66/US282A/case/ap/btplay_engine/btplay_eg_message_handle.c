/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�����Ӧ��ҵ���߼���Ϣ��Ӧʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "btplay_engine.h"

void btplay_engine_pipe_control(uint8 op)
{
    msg_apps_t msg;
    msg.content.data[0] = op;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTSTACK_PLAY_PIPE_CTRL_SYNC;

    //����ͬ����Ϣ
    send_sync_msg_btmanager(NULL, &msg, NULL, 0);
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
    // libc_print("G3",0,0);
    btplay_engine_pipe_control(PIPE_OP_FILTER | PIPE_OP_CLEAR_PAUSE);

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
            send_tts_stop_cmd();
#endif
            if (btplay_gl_var.filter_after_pause == 0)
            {
#ifdef ENABLE_TRUE_WIRELESS_STEREO            
              if((g_p_bt_stack_cur_info->dev_role==1)&&(g_p_bt_stack_cur_info->tws_master_phonecon==0))
                {
                    //tts ������ɺ�Ҫ��������
                }
                else
                {
                    if(g_p_tws_info->sub_quit_bl_flag==1)
                    {
                        //do no
                    }
                    else
                    {
                        if((g_p_bt_stack_cur_info->dev_role==1)&&(g_p_bt_stack_cur_info->tws_master_phonecon==1))
                        {
                            if(clear_tts_filter_timer_id!=-1)
                            {
                                kill_app_timer(clear_tts_filter_timer_id);
                            }
                            clear_tts_filter_timer_id=set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
                                500, Tws_clear_filter);
                        }
                        else
                        {
                            btplay_engine_pipe_control(PIPE_OP_UNFILTER);
                        }                        
                    }

                    
                }
#else
                btplay_engine_pipe_control(PIPE_OP_UNFILTER);

#endif

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
        PRINT_INFO("btplay quit");
        if ((pri_msg->msg.content.data[0] & 0x01) != 0)
        {
          //  PRINT_INFO("eg enter s3bt");
        }
        app_ret = app_msg_deal_app_quit();
        break;

        default:
        btplay_reply_msg(pri_msg, TRUE);
        break;
    }

    return app_ret;
}
