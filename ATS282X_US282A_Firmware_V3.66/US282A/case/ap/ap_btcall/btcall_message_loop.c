/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *        brief   ��Ӧͨ���еĲ�����������Ϣ�¼�
 *      <author>       <time>
 *       Wekan   2015-3-27
 *******************************************************************************/

#include  "ap_btcall.h"

/******************************************************************************
 * \par  Description:   ��ȡ������̨�����״̬
 * \param[in]    none
 * \param[out]   bg_info ����״ָ̬��
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

bool get_engine_status(void)
{
    if (sys_share_query_read(APP_ID_BTCALLEG, &g_btcall_cur_info) == -1)
    {
        // rcode �ռ䲻�㣬 �رմ�ӡ
        //PRINT_ERR("btcall share query not exist!\n");
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************
 * \par  Description: ���ݺ�̨������״̬���д���
 * \param[in]    none
 * \param[out]   none
 * \return       ��app��Ӧ���¼�
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/
app_result_e bt_status_deal(void)
{
    app_result_e result = RESULT_NULL;

    get_engine_status();

    if (g_callin_ring_mode != CALLIN_RING_MODE_NORMAL)
    {
        if (g_btcall_cur_info.status == BTCALL_CALLIN)
        {
            if (g_call_phone_num == FALSE)
            {
                g_pb_result = g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].hfp_attr.pb_result;
                if (g_pb_result != PB_RESULT_NULL)
                {
                    g_call_phone_num = TRUE;

                    //����绰����������
                    if (btcall_pb_tts_init() == FALSE)
                    {
                        g_pb_result = PB_RESULT_NULL;
                    }
                }
            }
        }
        else
        {
            g_call_phone_num = FALSE;
        }
    }

    if ((g_btcall_cur_info.status == BTCALL_HFP) || (g_btcall_cur_info.status == BTCALL_CALLIN)
            || (g_btcall_cur_info.status == BTCALL_CALLOUT) || (g_btcall_cur_info.status == BTCALL_PHONE)
            || (g_btcall_cur_info.status == BTCALL_SIRI))
    {
        g_state_call_phone = TRUE;
    }
    else
    {
        g_state_call_phone = FALSE;
    }

    return result;
}

/******************************************************************************
 * \par  Description: app�̴߳�����Ϣ�¼�
 * \param[in]    none
 * \param[out]   none
 * \return       ��app��Ӧ���¼�
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    int pb_play_cnt = (int) g_callin_ring_space_time * 100;//��ת��Ϊ10ms
    bool last_state_call_phone = FALSE;
    conn_status_e last_conn_status = CONN_STATUS_NONE;

    //�ȴ�BTCALL ENGINE״̬����
    while (get_engine_status() == FALSE)
    {
        sys_os_time_dly(1);
    }

    g_last_btcall_status = g_btcall_cur_info.status;

    //����տ�ʼ��δ���ӣ�ǿ�Ʋ���һ�εȴ�����
    if (g_btcall_cur_info.status == BTCALL_IDLE)
    {
        g_need_tts_play = TRUE;
    }

    //�����ǰ�Ѿ�����IDLE״̬��ǿ��ˢ��һ��
    if (g_btcall_cur_info.status != BTCALL_IDLE)
    {
        g_need_draw = TRUE;
    }

    last_conn_status = g_bt_stack_cur_info.conn_status;

    while (1)
    {
        result = bt_status_deal();
        if (result > RESULT_COMMON_RESERVE)
        {
            break;
        }
        if ((g_btcall_cur_info.status != g_last_btcall_status) && (g_state_call_phone == TRUE))
        {
            // PRINT_INFO_INT("g_btcall_cur_info", g_btcall_cur_info.status);
            //  PRINT_INFO_INT("hfp_status:", g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].hfp_status);
            g_need_draw = TRUE;
            g_need_tts_play = TRUE;
        }

        if (g_need_draw == TRUE)
        {
            com_view_update(APP_VIEW_ID_MAIN);

            g_need_draw = FALSE;
        }

        if ((last_conn_status == CONN_STATUS_NONE) && (g_bt_stack_cur_info.conn_status == CONN_STATUS_WAIT_PAIR))
        {
            g_need_tts_play = TRUE;
        }

        if (g_need_tts_play == TRUE)
        {
            //��̨��ʽ���������Ͽ������ӵ�tts
            // if (g_background_call_flag == FALSE)
            {
                btcall_tts_play();
            }

            g_need_tts_play = FALSE;
        }

        g_last_btcall_status = g_btcall_cur_info.status;
        last_conn_status = g_bt_stack_cur_info.conn_status;

        //ѭ�������绰����Ϣ��ÿ��1s����һ��
        if (g_callin_ring_mode != CALLIN_RING_MODE_NORMAL)
        {
            if ((g_call_phone_num == TRUE) && (g_pb_result != PB_RESULT_NULL))
            {
                if (pb_play_cnt == ((int) g_callin_ring_space_time * 100)) //��ת��Ϊ10ms
                {
                    btcall_pb_tts_play();
                    pb_play_cnt = 0;
                }
                else
                {
                    pb_play_cnt++;
                }
            }
        }

        if (g_background_call_flag == TRUE)
        {
            if ((g_btcall_cur_info.status == BTCALL_IDLE) || (g_btcall_cur_info.status == BTCALL_STOP))
            {
                result = RESULT_LASTPLAY;
                break;
            }
        }
        else
        {
            if ((last_state_call_phone == FALSE) && (g_state_call_phone == TRUE))
            {
                volume_cfg(TRUE);
            }
            else if ((last_state_call_phone == TRUE) && (g_state_call_phone == FALSE))
            {
                volume_cfg(FALSE);
            }
            else
            {
                ;//nothing
            }
            get_eg_asqt_vol();
            get_eg_asqt_PA();
            last_state_call_phone = g_state_call_phone;
        }

        result = com_view_loop();

        //�绰״̬��ֻ��Ӧ�˳�\�ػ�\�͵��������Ϣ
        if (g_state_call_phone == TRUE)
        {
            if ((result != RESULT_APP_QUIT) //�˳�
                    && (result != RESULT_POWER_OFF) //�ػ�
                    && (result != RESULT_LOW_POWER) //�͵�
            )
            {
                if (result > RESULT_COMMON_RESERVE)
                {
                    PRINT_INFO_INT("btcall ignore retult:", result);
                }
                result = RESULT_NULL;
            }
        }

        if (result > RESULT_COMMON_RESERVE)
        {
            break;
        }

        //����10ms�����������
        sys_os_time_dly(1);
        g_standby_exit_flag = FALSE;
    }

    return result;
}
