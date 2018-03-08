/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�����Ӧ��ҵ���߼���ѭ��ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "btplay_engine.h"

/******************************************************************************/
/*!
 * \par  Description:
 *      ����ǰӦ�õ�˽����Ϣ
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
app_result_e btplay_deal_app_msg(void)
{
    private_msg_t cur_msg;
    app_result_e result = RESULT_NULL;

    //���¹����ѯ
    g_p_btplay_info = sys_share_query_update(APP_ID_BTPLAYEG);

    if (get_app_msg(&cur_msg) == TRUE)
    {
        result = app_message_deal(&cur_msg);
    }

    return result;
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void Tws_clear_filter(void)
{
    if(btplay_gl_var.tts_play_flag == FALSE)
    {
        btplay_engine_pipe_control(PIPE_OP_UNFILTER );

    }
}
void Tws_mode_back(void)
{
    g_p_tws_info->pri_full_exit_flag=1;
}
#endif
void check_play_status(void)
{
    uint16 threshold;

    if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.play_status_flag == 1)
    {
        threshold = 1300;
    }
    else
    {
        threshold = 400;
    }

 //   if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status == A2DP_STATUS_PLAY)
   if((g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status == A2DP_STATUS_PLAY)&&(g_p_tws_info->pri_pause_flag==0))
    {
        if (btplay_gl_var.filter_after_pause > 0)
        {
            btplay_gl_var.filter_after_pause = 0;
            //libc_print("Q1",0,0);
            if (btplay_gl_var.tts_play_flag == FALSE)
            {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                if(g_p_bt_stack_cur_info->dev_role!=TWS_MASTER)
#endif
                 {
                     btplay_engine_pipe_control(PIPE_OP_UNFILTER);
                 }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                else
                 {
                   // if(g_p_tws_info->sub_quit_bl_flag != 1)
                   if((g_p_tws_info->sub_quit_bl_flag != 1)&&(g_p_tws_info->tts_start_flag==0))
                    {
                    	   libc_print("Q1",0,0);  
                        set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
                        300, Tws_clear_filter);                       	
                    }
                    else
                    {
                    	//libc_print("Q2",0,0);  
                    }		

                 }
#endif    
                  //btplay_engine_pipe_control(PIPE_OP_UNFILTER);
            }
        }

        if (change_flag == 0)
        {
            g_p_btplay_info->status = BTPLAY_PLAY;
        }

        //û�����ݹ���
        if (g_p_btplay_info->status == BTPLAY_PLAY)
        {
            if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.avdtp_data_flag == 0)
            {
                pause_cnt++;
                play_cnt = 0;
                if (pause_cnt >= 300)
                {
                    g_p_btplay_info->status = BTPLAY_PAUSE;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
#ifndef ENABLE_PLAY_PAUSE_TTS
                    //�ײ���ĳ�pause״̬
                    //��Ӱ�쳬ʱ�Ͽ������������Ļָ�����
                   // btplay_engine_pipe_control(PIPE_OP_PAUSE);
#endif
#endif
                    change_flag = 1;
                    pause_cnt = 0;
                    PRINT_INFO_INT("eng chg pau:", 300);
                }
            }
            else
            {
                pause_cnt = 0;
            }
        }
        else if (g_p_btplay_info->status == BTPLAY_PAUSE)
        {
            if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.avdtp_data_flag == 1)
            {
                g_p_btplay_info->status = BTPLAY_PLAY;
                change_flag = 0;
            }
        }
        else
        {
            ; //do nothing
        }
    }
  // else if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status == A2DP_STATUS_PAUSE)
   else if ((g_p_tws_info->pri_pause_flag==3)||(g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status == A2DP_STATUS_PAUSE))
    {
        //����AVRCP PAUSE���͹���1-3������
    //    if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.avrcp_pause_flag == 1)
        if ((g_p_tws_info->pri_pause_flag==3)||(g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.avrcp_pause_flag == 1))
        {
            if(g_p_btplay_info->status != BTPLAY_PAUSE)
            {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
#ifdef ENABLE_PLAY_PAUSE_TTS
                btplay_engine_pipe_control(PIPE_OP_FILTER | PIPE_OP_CLEAR_PAUSE);
                //ж��BTPLAY�м����DSP�����
                if (btplay_gl_var.codec_flag == TRUE)
                {
                    //libc_print("G3", 0, 0);
                    a2dp_decode_quit();
                }
                btplay_engine_pipe_control(PIPE_OP_FLUSH);
#else
                //libc_print("G3", 0, 0);
#endif
#else
                btplay_engine_pipe_control(PIPE_OP_FILTER | PIPE_OP_CLEAR_PAUSE);
                //ж��BTPLAY�м����DSP�����
                if (btplay_gl_var.codec_flag == TRUE)
                {
                    //libc_print("G3", 0, 0);
                    a2dp_decode_quit();
                }
                btplay_engine_pipe_control(PIPE_OP_FLUSH);

#endif
                g_p_btplay_info->status = BTPLAY_PAUSE;
                btplay_gl_var.filter_after_pause = threshold - 2; //���� TAG1 �����������
                play_cnt = 0;
            }
        }

        if (btplay_gl_var.filter_after_pause > 0)
        {
            btplay_gl_var.filter_after_pause--;
            if (btplay_gl_var.filter_after_pause == 0)
            {
              g_p_tws_info->pri_pause_flag=0;
                if (btplay_gl_var.tts_play_flag == FALSE)
                {
                       // if(g_p_tws_info->sub_quit_bl_flag==1)
                       if((g_p_tws_info->sub_quit_bl_flag==1)||(g_p_tws_info->tts_start_flag==1))
                        {
                               ////libc_print("U2",0,0);
                        }
                        else
                        {
                            btplay_engine_pipe_control(PIPE_OP_UNFILTER);
                        }
                }
            }
        }

        if (change_flag == 0)
        {
            g_p_btplay_info->status = BTPLAY_PAUSE;
        }

        //�����ݹ���
        if (g_p_btplay_info->status == BTPLAY_PAUSE)
        {
            if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.avdtp_data_flag == 1)
            {
                play_cnt++;
                pause_cnt = 0;
                if (play_cnt >= threshold) //TAG1
                {
                    g_p_btplay_info->status = BTPLAY_PLAY;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
#ifndef ENABLE_PLAY_PAUSE_TTS
                    //���ĵײ�״̬����Ҫ��Ϊ�˽��水����֤ǰ̨״̬�ܹ�����
                    btplay_engine_pipe_control(PIPE_OP_PLAY);
#endif
#endif
                    change_flag = 1;
                    play_cnt = 0;
                //    PRINT_INFO_INT("eng chg ply:", (uint32) threshold);
                }
            }
            else
            {
                play_cnt = 0;
            }
        }
        else if (g_p_btplay_info->status == BTPLAY_PLAY)
        {
            if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.avdtp_data_flag == 0)
            {
                g_p_btplay_info->status = BTPLAY_PAUSE;
                change_flag = 0;
            }
        }
        else
        {
            ; //do nothing
        }
    }
    else
    {
        ; //do nothing
    }
}

void __section__(".bank") btplay_eg_status_change_deal(a2dp_status_e *last_a2dp_status)
{
    switch (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status)
    {
        case A2DP_STATUS_NONE:
        g_p_btplay_info->status = BTPLAY_IDLE;
        change_flag = 0;
        break;
    
        default:
        if (*last_a2dp_status == A2DP_STATUS_NONE)
        {
            g_p_btplay_info->status = BTPLAY_STOP;
        }
        break;
    }
    
    *last_a2dp_status = g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status;
}
#ifdef ENABLE_TRUE_WIRELESS_STEREO
void __section__(".bank") tws_decode_end_deal(void)
{
    //libc_print("phc:",g_p_bt_stack_cur_info->tws_master_phonecon,2);
    btplay_engine_pipe_control(PIPE_OP_FILTER);
    a2dp_decode_quit();
    //libc_print("G1",0,0);
    if((g_p_bt_stack_cur_info->dev_role==1)&&((g_p_tws_info->pri_exit_mmm==1)||(g_p_tws_info->pri_full_exit_flag)
        ||(g_p_tws_info->pri_empty_exit_mmm==1)))
    {
        //   //libc_print("EE",0,0);
        //��֤�������ǰ�������Ѿ�ж����
        //1s����ȥ���������־
        clear_filter_timer_id = set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
                500, Tws_clear_filter);
        btplay_engine_pipe_control(PIPE_OP_FLUSH);

    }
    else if((g_p_bt_stack_cur_info->dev_role==1)&&(g_p_tws_info->tts_start_flag==1)) //�Է�tts���������������ı�־

    {
        btplay_engine_pipe_control(PIPE_OP_FLUSH);
        //g_p_tws_info->tts_start_flag=0;
    }
#ifndef ENABLE_PLAY_PAUSE_TTS
    else if((g_p_bt_stack_cur_info->dev_role==TWS_MASTER)&&(g_p_tws_info->sub_quit_bl_flag==1))//����ȥ����G3�ķ�֧����˴�����Ӧ�û���G1��֧
    {
        btplay_engine_pipe_control(PIPE_OP_FLUSH);
    }
#endif
    else
    {
        btplay_engine_pipe_control(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
    }
    //  second_empty_flag=0;

}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu Ӧ�õ���ں����ͳ���������ģ��
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
app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    a2dp_status_e last_a2dp_status = A2DP_STATUS_NONE;

    while (1)
    {
        if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status != last_a2dp_status)
        {
            btplay_eg_status_change_deal(&last_a2dp_status);
        }

        //����BTPLAY�м����DSP�����
#ifdef ENABLE_TRUE_WIRELESS_STEREO

        if(g_p_bt_stack_cur_info->dev_role==TWS_MASTER)
        { 
             if(g_p_tws_info->sub_quit_bl_flag==1)
              {
                 btplay_engine_pipe_control(PIPE_OP_FILTER);
               }
               else if(g_p_tws_info->sub_back_bl_flag==1)
                {
                    g_p_tws_info->sub_back_bl_flag=0;
                    if((btplay_gl_var.codec_flag == FALSE)&&(btplay_gl_var.tts_play_flag== FALSE))
                    {
                          btplay_engine_pipe_control(PIPE_OP_UNFILTER);
                    }               
                }
        }
        if(g_p_bt_stack_cur_info->dev_role==NORMAL_DEV)
        {  
              g_p_tws_info->sub_quit_bl_flag=0;
         }
         if((btplay_gl_var.codec_flag == FALSE)&&((g_p_bt_stack_cur_info->play_and_pause==1)||(g_p_bt_stack_cur_info->tws_master_phonecon==0)))
        {
            g_p_tws_info->tts_stop_flag=0;
            g_p_tws_info->tts_start_flag=0;
            ////libc_print("U3",0,0);
        }

        if ((btplay_gl_var.codec_flag == FALSE) && (btplay_gl_var.tts_play_flag == FALSE)
                && (g_p_play_pipe->length> 0)
                && (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status
                        != A2DP_STATUS_NONE)
                && (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.media_type
                        != A2DP_CODEC_NONA2DP)&&(g_p_bt_stack_cur_info->controller_status != BT_CONTROLLER_STATUS_IDLE))
#else
        if ((btplay_gl_var.codec_flag == FALSE) && (btplay_gl_var.tts_play_flag == FALSE)
                && (g_p_play_pipe->length > 0)
                && (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status
                        != A2DP_STATUS_NONE)
                && (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.media_type
                        != A2DP_CODEC_NONA2DP))
#endif
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (g_p_bt_stack_cur_info->dev_role == 1)
            {
                btplay_engine_pipe_control(PIPE_OP_FILTER | PIPE_OP_MMM_FILTER);
            }
#endif

            //������������м�����ֻ�ȥж�ص�
            if ((g_p_tws_info->pri_exit_mmm == 1) || (g_p_tws_info->sub_exit_mmm == 1))
            {
                g_p_tws_info->pri_exit_mmm = 0;
                g_p_tws_info->sub_exit_mmm = 0;

            }

            a2dp_decode_init(g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_attr.media_type);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (g_p_bt_stack_cur_info->dev_role == 1)
            {
                btplay_engine_pipe_control(PIPE_OP_UNFILTER);
            }
#endif
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO

        send_mmm_exit_cmd();
        //ж��BTPLAY�м����DSP�����
        if ((btplay_gl_var.codec_flag == TRUE)
                && ((g_p_bt_stack_cur_info->controller_status == BT_CONTROLLER_STATUS_IDLE)
                        || (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status
                                == A2DP_STATUS_NONE)
                        ||(g_p_tws_info->pri_exit_mmm==1)
                        ||(g_p_tws_info->sub_exit_mmm==1)
                        ||(second_empty_flag!=0)
                        ||(g_p_tws_info->tts_start_flag==1)
                        ||((tws_or_notws_flag != g_p_bt_stack_cur_info->dev_role)&&(g_app_info_state_all.g_no_support_tts_flag==1))
                        ||((g_p_bt_stack_cur_info->dev_role==2)&&(g_p_bt_stack_cur_info->tws_master_phonecon == 0))))

        {
            tws_decode_end_deal();
        }
        tws_or_notws_flag = g_p_bt_stack_cur_info->dev_role;
        second_empty_flag=0;

        //�Է�tts�������������־
        if((g_p_tws_info->tts_stop_flag==1)&&(g_p_tws_info->tts_start_flag==1))
        {
            //btplay_engine_pipe_control(PIPE_OP_UNFILTER);
            if(g_p_bt_stack_cur_info->dev_role==1)
            {
                 set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,
                        500, Tws_clear_filter);
            }
            else
            {
                 btplay_engine_pipe_control(PIPE_OP_UNFILTER );
            }
            g_p_tws_info->tts_stop_flag=0;
            g_p_tws_info->tts_start_flag=0;
        }
#else
        if ((btplay_gl_var.codec_flag == TRUE) && ((g_p_bt_stack_cur_info->controller_status
                == BT_CONTROLLER_STATUS_IDLE)
                || (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->a2dp_active_id].a2dp_status
                        == A2DP_STATUS_NONE)))
        {
            btplay_engine_pipe_control(PIPE_OP_FILTER);
            a2dp_decode_quit();
            btplay_engine_pipe_control(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);

        }
#endif

        //����ǰӦ�õ�˽����Ϣ
        result = btplay_deal_app_msg();
        if (result != RESULT_NULL)
        {
            break;
        }
        check_play_status();

        /*when wait_paired, and no connect_request, and not bt_front, release more os_time.*/
        if (g_p_btplay_info->status == BTPLAY_PLAY)
        {
            change_engine_state(ENGINE_STATE_PLAYING);
        }
        else
        {
            change_engine_state(ENGINE_STATE_PAUSE);
        }
        //����10ms�����������
        sys_os_time_dly(1);
    }

    return result;
}
