/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *        brief   ȫ�ֱ���
 *      Cailizhen          2015-1          creat
 *       Wekan        2015-3-30       review
 *******************************************************************************/

#include  "btcall_engine.h"

/******************************************************************************
 * \par  Description:   ��ƵԴ�������л��ֻ�
 * \param[in]     null0
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

void force_hfp_to_phone(void)
{
    msg_apps_t msg;
    int i;

    if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->hfp_active_id].hfp_attr.sco_link == 1)
    {
        //�Ͽ�SCO��·
        msg.type = MSG_BTSTACK_HFP_RELEASESYN_SYNC;
        //����ͬ����Ϣ
        send_sync_msg_btmanager(NULL, &msg, NULL, 0);

        //�ȴ���������л����ֻ���
        for (i = 0; i < 10; i++)
        {
            sys_os_time_dly(1);

            if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->hfp_active_id].hfp_attr.sco_link == 0)
            {
                break;
            }
        }
    }
}

/******************************************************************************
 * \par  Description:   �˳������߳�
 * \param[in]     null0
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

void force_quit_thread(void)
{
    btcall_gl_var.need_exit_thread = TRUE;

    force_hfp_to_phone();

    if (btcall_gl_var.g_sco_deal_flag != 0)
    {
        btcalleg_free_hfp_mmm();
    }
    //�ȴ����������߳��˳�
    while (g_callring_data.callring_thread_exist != 0)
    {
        sys_os_time_dly(1);
    }
}

/******************************************************************************
 * \par  Description:   �����绰��
 * \param[in]     null0
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

bool btcall_enter_pb_tts_play(void)
{
    PRINT_DBG("tts pb play\n");
    //��ֹ��������������δ����
    if (btcall_gl_var.callin_ring_mode != CALLIN_RING_MODE_ONLY_PB)
    {
        if (g_callring_data.callring_timer_id != -1)
        {
            kill_app_timer(g_callring_data.callring_timer_id);
            g_callring_data.callring_timer_id = -1;
        }

        if (g_callring_data.callring_thread_exist == 1)
        {
            btcall_gl_var.need_exit_thread = TRUE;
            while (g_callring_data.callring_thread_exist != 0)
            {
                sys_os_time_dly(1);
            }
        }
    }

    if (btcall_gl_var.g_sco_deal_flag == 1)
    { //�������ͨ�������г�CAGģʽ
      //���sco��·�Ѵ��ڣ���ɱ��sco�߳�
        btcalleg_free_hfp_mmm();
    }

    btcall_gl_var.tts_play_flag = 1;
    btcall_gl_var.tts_pb_play_flag = 1;

    return TRUE;
}

/******************************************************************************
 * \par  Description:   �˳��绰������
 * \param[in]     null0
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

bool btcall_exit_pb_tts_play(void)
{
    PRINT_DBG("tts pb finish\n");
    btcall_gl_var.tts_play_flag = 0;
    btcall_gl_var.tts_pb_play_flag = 0;
    btcall_gl_var.callin_pb_back = 1;
    if (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->hfp_active_id].hfp_status == HFP_STATUS_CALLING)
    {
        bt_engine_hfp_sco_connect_ind_handle();
    }

    return TRUE;
}

/******************************************************************************
 * \par  Description:   �ظ�ͬ����Ϣ
 * \param[in]    void  msg_ptr ��Ϣ�ṹ��
 * \param[in]    ret_vals ��һ�¼��Ĵ����� TRUE �ɹ� FALSE ʧ��
 * \param[out]   none
 * \return           null
 * \note  �ظ���Ϣʱ������ret_vals�Ľ��������Ӧ��ɹ���ʧ��
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

void btcall_reply_msg(void* msg_ptr, bool ret_vals)
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
}

/******************************************************************************
 * \par  Description:   ��Ϣ��ʼ��
 * \param[in]    void  msg_ptr ��Ϣ�ṹ��
 * \param[out]   none
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

app_result_e app_msg_deal_app_quit(void)
{
    force_quit_thread();

    return RESULT_APP_QUIT;
}

/******************************************************************************
 * \par  Description:  ������Ϣ�¼�
 * \param[in]    pri_msg  ˽����Ϣָ��
 * \param[out]   none
 * \return           ��Ҫ���㴦����¼�
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

app_result_e app_message_deal(private_msg_t *pri_msg)
{
    app_result_e app_ret = RESULT_NULL;
    bool ret_val;
    switch (pri_msg->msg.type)
    {
        case MSG_SWITCH_AOUT_START_SYNC: //׼���л���Ƶ���
        {
            ret_val = btcall_enter_pb_tts_play();
            btcall_gl_var.tts_play_flag = 1;
            btcall_reply_msg(pri_msg, ret_val);
        }
        break;

        case MSG_SWITCH_AOUT_STOP_SYNC: //����л���Ƶ���
        {
            ret_val = btcall_exit_pb_tts_play();
            btcall_gl_var.tts_play_flag = 0;
            btcall_reply_msg(pri_msg, ret_val);
        }
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        {
            bool pb_play_flag = pri_msg->msg.content.data[0];

            if (pb_play_flag == TRUE)
            {
                ret_val = btcall_enter_pb_tts_play();
            }
            else
            {
                PRINT_DBG("tts play\n");

                if ((g_p_btcall_info->status != BTCALL_IDLE)
                        && (g_p_btcall_info->status != BTCALL_STOP))
                {
                    //������TTS����
                    ret_val = FALSE;
                }
                else
                {
                    btcall_gl_var.tts_play_flag = 1;
                    ret_val = TRUE;
                }
            }

            btcall_reply_msg(pri_msg, ret_val);
        }
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        {
            bool pb_play_flag = pri_msg->msg.content.data[0];

            PRINT_DBG("tts finish\n");

            if (pb_play_flag == TRUE)
            {
                ret_val = btcall_exit_pb_tts_play();
            }
            else
            {
                btcall_gl_var.tts_play_flag = 0;
                ret_val = TRUE;
            }

            btcall_reply_msg(pri_msg, ret_val);
        }
        break;

        case MSG_ENGINE_STANDBY_SYNC:
        {
            os_event_t *standby_sem = (os_event_t *)(pri_msg->msg.content.addr);

            //�����ֳ���׼������˯��ģʽ
            PRINT_INFO("engine standby enter");

            //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
            btcall_reply_msg(pri_msg, TRUE);

            libc_sem_wait(standby_sem, 0);

            //��˯��ģʽ�������ָ��ֳ�
            PRINT_INFO("engine standby exit");
            standby_restart_all_app_timer(g_app_timer_vector, g_app_timer_count);
        }
        break;

        case MSG_APP_QUIT: //�˳�����
        PRINT_DBG("quit btcall engine\n");
        if ((pri_msg->msg.content.data[0] & 0x01) != 0)
        {
            PRINT_INFO("eg enter s3bt");
        }
        app_ret = app_msg_deal_app_quit();
        break;

        //ͨ��Զ�˾���
        case MSG_BTCALLEG_MIC_MUTE_SYNC:
        btcall_gl_var.g_mic_mute_flag = TRUE;
        g_dsp_init_parm.pcm_mode = 2;//�ϴ���������
        libc_print("btcall-set-mic-mute.",0,0);
        btcall_reply_msg(pri_msg, TRUE);
        break;

        //ͨ��Զ�˻ָ�����
        case MSG_BTCALLEG_MIC_RELEASEMUTE_SYNC:
        btcall_gl_var.g_mic_mute_flag = FALSE;
        g_dsp_init_parm.pcm_mode = 0;
        libc_print("btcall-set-mic-ok.",0,0);
        btcall_gl_var.g_actaec_reset_flag = TRUE;
        btcall_reply_msg(pri_msg, TRUE);
        break;


        case  MSG_BTCALLEG_GET_EG_VOLUM_SYNC:
        {
           uint32 *pu32 = (uint32*)pri_msg->reply->content;
           //*pu32 = g_asqt_parm.AsqtCfg.pa_volume;
           *pu32 = g_asqt_parm.asqt_volume;
           btcall_reply_msg(pri_msg, TRUE);
        }
        break;


        case MSG_BTCALLEG_GET_EG_PA_VOLUM_SYNC:
        {
           uint32 *pu32 = (uint32*)pri_msg->reply->content;
           *pu32 = g_asqt_parm.AsqtCfg.pa_volume;
           //*pu32 = g_asqt_parm.asqt_volume;
           btcall_reply_msg(pri_msg, TRUE);
        }
        break;

        default:
        btcall_reply_msg(pri_msg, TRUE);
        break;
    }

    return app_ret;
}
