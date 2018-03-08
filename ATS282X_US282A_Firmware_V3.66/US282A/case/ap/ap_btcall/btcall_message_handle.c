
/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *        brief   ��Ӧͨ���еĲ�����������Ϣ�¼�
 *      <author>       <time>
 *       Wekan   2015-3-27
 *******************************************************************************/

#include  "ap_btcall.h"

app_result_e hfp_accept_call(void);
app_result_e hfp_reject_call(void);
app_result_e hfp_hang_up_call(void);
app_result_e hfp_change_to_hfp(void);
app_result_e hfp_change_to_phone(void);
app_result_e hfp_outgo_call(void);

#if (SUPPORT_PHONE_KEY == 1)
app_result_e key_phone_deal(void);
app_result_e key_phone_long_deal(void);
#else
app_result_e key_play_deal(void);
app_result_e key_next_deal(void);
app_result_e key_prev_deal(void);
app_result_e key_eq_deal(void);
#endif
app_result_e key_phone_deal(void);
app_result_e key_phone_long_deal(void);

app_result_e key_EQ_deal(void);
app_result_e key_EQ_long_deal(void);
app_result_e key_switch_ap(void);

#ifdef __SUPPORT_3_WAY_CALL_
int com_get_hfp_3way_status(void);
app_result_e hfp_3way_handle(int8 mode);
app_result_e key_dbl_next_deal(void);
#endif //#ifdef __SUPPORT_3_WAY_CALL_
const key_event_map_t __section__(".rodata.ke_maplist") btcall_ke_maplist[] =
{
#if (SUPPORT_PHONE_KEY == 1)
    { { KEY_PHONE, 0, KEY_TYPE_SHORT_UP , 0}, key_phone_deal},
    { { KEY_PHONEPLAY, 0, KEY_TYPE_SHORT_UP , 0}, key_phone_deal},
    { { KEY_PHONE, 0, KEY_TYPE_LONG , KEY_DEAL_FILTER}, key_phone_long_deal},
    { { KEY_PHONEPLAY, 0, KEY_TYPE_LONG , KEY_DEAL_FILTER}, key_phone_long_deal},
#else

#ifdef __SUPPORT_3_WAY_CALL_

    { { KEY_NEXT, 0, KEY_TYPE_DBL_CLICK , 0}, key_dbl_next_deal},
#endif
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG_UP , 0}, key_next_deal},

    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG_UP , 0}, key_prev_deal},
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP , 0}, key_next_deal},
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP , 0}, key_prev_deal},
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP , 0}, key_play_deal},
    { { KEY_EQ, 0, KEY_TYPE_SHORT_UP , 0}, key_EQ_deal},
    { { KEY_EQ, 0, KEY_TYPE_LONG , KEY_DEAL_FILTER}, key_EQ_long_deal},
#endif
    /*! �л�Ӧ��ģʽ */
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP, 0}, key_switch_ap},

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL,0}, NULL},
};

#if (SUPPORT_PHONE_KEY == 0)

/******************************************************************************
 * \par  Description:   ��Ӧplay��������ͨ��ҵ绰
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e key_play_deal(void)
{
    PRINT_INFO("BT-CALL : key_play_deal\n");

    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        if (com_btmanager_avrcp_op(BTPLAY_AVRCP_PLAY) == 0)
        {
            sys_os_time_dly(10); //��Ҫʹ��sys_mdelay�����ֽӿ��ڵ����ȼ�����׼ȷ
        }
        return RESULT_NULL;
    }

    if ((g_btcall_cur_info.status == BTCALL_IDLE) || (g_btcall_cur_info.status == BTCALL_STOP))
    {
        return RESULT_NULL;
    }

#if (__SUPPORT_SIRI_ == 1)
    if (((com_get_config_default(BTMANAGER_ENABLE_SIRI)!=0))&&(g_btcall_cur_info.status == BTCALL_SIRI))
    {
        PRINT_INFO("hang up siri\n");
        com_btmanager_hfp_siri_handle();
        return RESULT_NULL;
    }
    else
    {
        ;//for qac
    }
#endif //#if (__SUPPORT_SIRI_ == 1)
    /* ����ͨ��״̬ע��:
     THREE_WAY_NEW_CALL_IN : phone A active, and new phone B call in,
     to handle, turn A to held, accept B.
     THREE_WAY_HOLD_EXIST :  phone A held, and phone B active,
     to hanle, turn A to active, B to held.
     */
#ifdef __SUPPORT_3_WAY_CALL_
    {
        uint8 hfp_status;
        hfp_status = (uint8)com_get_hfp_3way_status();

        if ( (hfp_status == THREE_WAY_NEW_CALL_IN)
                ||(hfp_status == THREE_WAY_HOLD_EXIST))
        {
            //ͨ�������н���������
            libc_print("ap:key accept 3-way  new call in.\n", 0, 0);
            return hfp_3way_handle(THREE_WAY_HOLD_EXIST);
        }
    }
#endif  //__SUPPORT_3_WAY_CALL_
    if (g_btcall_cur_info.status == BTCALL_CALLIN)
    {
        return hfp_accept_call();
    }
    else
    {
        return hfp_hang_up_call();
    }
}
/******************************************************************************
 * \par  Description:   ��Ӧnext�������ܽӡ��Ҷϡ��л���Դ
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/
app_result_e key_next_deal(void)
{
    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        if (com_btmanager_avrcp_op(BTPLAY_AVRCP_NEXT) == 0)
        {
            sys_os_time_dly(10); //��Ҫʹ��sys_mdelay�����ֽӿ��ڵ����ȼ�����׼ȷ
        }

    }
    else
    {
        if (g_btcall_cur_info.status == BTCALL_CALLIN)
        {
            return hfp_reject_call();//�ܽ� hfp_reject_call
        }

        if (g_chf_cag_exch_enable == 1)//֧���л�
        {
            if (g_btcall_cur_info.status == BTCALL_HFP)
            {
                return hfp_change_to_phone();//��Ƶ�е��ֻ�
            }
            else if (g_btcall_cur_info.status == BTCALL_PHONE)
            {
                return hfp_change_to_hfp();//��Ƶ�е�С��
            }
            else
            {
                ;//nothing
            }
        }
    }
    return RESULT_NULL;

}

/******************************************************************************
 * \par  Description:   ��Ӧprve��������nextһ�����ܽӡ��Ҷϡ��л���Դ
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e key_prev_deal(void)
{
    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        if (com_btmanager_avrcp_op(BTPLAY_AVRCP_PREV) == 0)
        {
            sys_os_time_dly(10); //��Ҫʹ��sys_mdelay�����ֽӿ��ڵ����ȼ�����׼ȷ
        }

    }
    else
    {
        if (g_btcall_cur_info.status == BTCALL_CALLIN)
        {
            return hfp_reject_call();//�ܽ� hfp_reject_call
        }

        if ((uint8) com_get_config_default(BTCALL_SET_MIC_MUTE_ENABLE) != 0)
        {
            if (g_btcall_cur_info.status == BTCALL_HFP)
            {
                if (g_set_mic_mute == FALSE)
                {
                    btcall_send_sync_msg(NULL, NULL, MSG_BTCALLEG_MIC_MUTE_SYNC);
                    g_set_mic_mute = TRUE;
                }
                else
                {
                    btcall_send_sync_msg(NULL, NULL, MSG_BTCALLEG_MIC_RELEASEMUTE_SYNC);
                    g_set_mic_mute = FALSE;
                }
                g_need_draw = TRUE;
            }
        }
    }
    return RESULT_NULL;
}

#endif

/******************************************************************************
 * \par  Description:   ��Ӧphone��������ͨ��ҵ绰
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/
app_result_e key_phone_deal(void)
{
#ifdef __SUPPORT_3_WAY_CALL_
    uint8 hfp_status;
    hfp_status = (uint8)com_get_hfp_3way_status();
#endif

    if ((g_btcall_cur_info.status == BTCALL_IDLE) || (g_btcall_cur_info.status == BTCALL_STOP))
    {
        return RESULT_NULL;
    }

#ifdef __SUPPORT_3_WAY_CALL_

    if (hfp_status == THREE_WAY_HOLD_EXIST)
    {
        // ����ͨ��״̬���л�����һ��
        return hfp_3way_handle(THREE_WAY_HOLD_EXIST);
    }
    else if (hfp_status == THREE_WAY_NEW_CALL_IN)
    {
        // ͨ���У�����������
        //return hfp_accept_call();
        return hfp_3way_handle(THREE_WAY_HOLD_EXIST);
    }
    else
#endif

#if (__SUPPORT_SIRI_ == 1)
    if (((com_get_config_default(BTMANAGER_ENABLE_SIRI)!=0))&&(g_btcall_cur_info.status == BTCALL_SIRI))
    {
        PRINT_INFO("hang up siri\n");
        com_btmanager_hfp_siri_handle();
        return RESULT_NULL;
    }
    else
    {
        ;//for qac
    }
#endif //#if (__SUPPORT_SIRI_ == 1)
    if (g_btcall_cur_info.status == BTCALL_CALLIN)
    {
        return hfp_accept_call();
    }

    else
    {
        ;//nothing
    }

    return hfp_hang_up_call();
}

/******************************************************************************
 * \par  Description:   ����phone�������ܽӵ绰
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/
app_result_e key_phone_long_deal(void)
{
#ifdef __SUPPORT_3_WAY_CALL_

    uint8 hfp_status = (uint8)com_get_hfp_3way_status();

    if (hfp_status == THREE_WAY_NEW_CALL_IN)
    {
        // �ҶϺ�̨ͨ�����ܽ�������
        return hfp_3way_handle(0);
    }
    else if (hfp_status == THREE_WAY_HOLD_EXIST)
    {
        // �Ҷϵ�ǰͨ�����л�����һ��������������
        return hfp_3way_handle(1);
    }
    else
#endif

    if (g_btcall_cur_info.status == BTCALL_CALLIN)
    {
        return hfp_reject_call();//�ܽ�
    }
    else
    {
        ;//for qac
    }
    return RESULT_NULL;
}

/******************************************************************************
 * \par  Description:   �̰�eq������֧��3��ͨ��ʱ������ͬphone��
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2016-3-10
 *******************************************************************************/

app_result_e key_EQ_deal(void)
{
    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        return RESULT_BLUETEETH_SOUND;
    }
#ifdef __SUPPORT_3_WAY_CALL_
    return key_phone_deal();
#else
    return RESULT_NULL;
#endif
}

/******************************************************************************
 * \par  Description:   ����eq������֧��3��ͨ��ʱ������ͬphone��
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2016-3-10
 *******************************************************************************/

app_result_e key_EQ_long_deal(void)
{
    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        return RESULT_BLUETEETH_SOUND;
    }

#ifdef __SUPPORT_3_WAY_CALL_
    return key_phone_long_deal();
#else
    return RESULT_NULL;
#endif
}

/******************************************************************************
 * \par  Description:   mode����Ӧ�¼�
 * \param[in]     null0
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2016-3-10
 *******************************************************************************/

app_result_e key_switch_ap(void)
{
    if (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        return RESULT_BLUETEETH_SOUND;
    }

    return com_key_deal_switch_app();
}

/******************************************************************************
 * \par  Description:   ��ͨ�绰
 * \param[in]     null
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e hfp_accept_call(void)
{
    com_btmanager_hfp_call_req(BTCALL_HFP_ANSWER_CALL);

    return RESULT_NULL;
}

/******************************************************************************
 * \par  Description:   �ܽӵ绰
 * \param[in]     null
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e hfp_reject_call(void)
{
    com_btmanager_hfp_call_req(BTCALL_HFP_REJECT_CALL);

    return RESULT_NULL;
}

/******************************************************************************
 * \par  Description:   �Ҷϵ绰
 * \param[in]     null
 * \param[out]   none
 * \return          ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e hfp_hang_up_call(void)
{
    com_btmanager_hfp_call_req(BTCALL_HFP_HANGUP_CALL);

    return RESULT_NULL;
}
/******************************************************************************
 * \par  Description:   ��Դ�е�����
 * \param[in]     null
 * \param[out]   none
 * \return          ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e hfp_change_to_hfp(void)
{
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTSTACK_HFP_SETSYN_SYNC;
    //����ͬ����Ϣ
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].addr.bytes, &msg, NULL, 0);

    return RESULT_NULL;
}
/******************************************************************************
 * \par  Description:   ��Դ�е��ֻ�
 * \param[in]     null
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e hfp_change_to_phone(void)
{
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTSTACK_HFP_RELEASESYN_SYNC;
    //����ͬ����Ϣ
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].addr.bytes, &msg, NULL, 0);

    return RESULT_NULL;
}

app_result_e btcall_sys_deal_ignore_when_call(void *ev_param);
app_result_e btcall_sys_deal_rtcalarm_when_call(void *ev_param);
const sys_event_map_t __section__(".rodata.se_maplist") btcall_se_maplist[] =
{
    { { MSG_UH_IN, 0}, btcall_sys_deal_ignore_when_call},
    { { MSG_UH_OUT, 0}, btcall_sys_deal_ignore_when_call},
    { { MSG_SD_IN, 0}, btcall_sys_deal_ignore_when_call},
    { { MSG_SD_OUT, 0}, btcall_sys_deal_ignore_when_call},
    { { MSG_LINEIN_IN, 0}, btcall_sys_deal_ignore_when_call},
    { { MSG_LINEIN_OUT, 0}, btcall_sys_deal_ignore_when_call},
    { { MSG_RTCALARM, 0}, btcall_sys_deal_rtcalarm_when_call},
    { { MSG_USB_STICK, 0}, btcall_sys_deal_ignore_when_call},

    /*! ������־ */
    { { MSG_NULL, 0}, NULL},
};

/******************************************************************************
 * \par  Description:    ����ͨ��ʱ�������¼�
 * \param[in]     ev_param �¼�
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e btcall_sys_deal_ignore_when_call(void *ev_param)
{
    if (g_state_call_phone == TRUE)
    {
        return RESULT_NULL;
    }
    else
    {
        return RESULT_SYS_MSG_REDEAL_BY_COMMON;
    }
}

/******************************************************************************
 * \par  Description:   ����ͨ��ʱ�������¼�
 * \param[in]     ev_param �¼�
 * \param[out]   none
 * \return           ��app��Ӧ���¼�
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-27
 *******************************************************************************/

app_result_e btcall_sys_deal_rtcalarm_when_call(void *ev_param)
{
    if (g_state_call_phone == TRUE)
    {
        return tm_alarm_alarm_ring_deal(ALARM_RING_ENTER_CLOSE_AUTO);
    }
    else
    {
        return RESULT_SYS_MSG_REDEAL_BY_COMMON;
    }
}

#ifdef __SUPPORT_3_WAY_CALL_

app_result_e key_dbl_next_deal(void)
{
    uint8 hfp_status = (uint8)com_get_hfp_3way_status();

    if ((hfp_status == THREE_WAY_NEW_CALL_IN)||(hfp_status == THREE_WAY_HOLD_EXIST))
    {

        /** hand up the current call, and accept the new call.,
         and the 3-way status not cancel.
         */
        return hfp_3way_handle(1);

    }
    return RESULT_NULL;
}

int com_get_hfp_3way_status(void)
{
    msg_apps_t msg;
    uint8 hfp_status = 0;

    msg.content.addr = &hfp_status;
    msg.type = MSG_BTENGINE_3WAY_GET_STATUS_SYNC;
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].addr.bytes,
            &msg, NULL, 0);

    // PRINT_INFO_INT("com_get_hfp_3way_status ", hfp_status);

    return hfp_status;
}

/*
 mode:0--release the held call
 1--release the current call
 2--held/active call swap
 3--transform held to active
 */
app_result_e hfp_3way_handle(int8 mode)
{
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_BTENGINE_3WAY_HANDLE_SYNC;
    msg.content.data[0] = (uint8)mode;
    //����ͬ����Ϣ
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.hfp_active_id].addr.bytes, &msg, NULL, 0);

    return RESULT_NULL;
}
#endif //#ifdef __SUPPORT_3_WAY_CALL_
