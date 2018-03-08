#include "app_mengine.h"

void mengine_status_deal(void)
{
    mmm_mp_status_t mmm_status;
    switch_result_e switch_retval = SWITCH_NO_ERR;
    //��ȡ��ǰ����״̬
    mmm_mp_cmd(g_mp_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status);

    //��ŵ�ǰ�Ĳ���ʱ��ͱ�����
    if ((g_eg_status_p->play_status != PauseSta) && (g_eg_status_p->play_status != StopSta))
    {
        g_eg_playinfo_p->cur_time = (uint32) mmm_status.cur_time;
    }

    switch (mmm_status.status)
    {
        case MMM_MP_ENGINE_PLAYING: //��������
        if (g_eg_status_p->fast_status != FBPlay_Null) //�����ȡ��
        {
            //�Ѿ��ָ���������
            if (g_eg_status_p->play_status == PlaySta)
            {
                mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);
                g_eg_status_p->fast_status = FBPlay_Null;
            }
        }
#ifdef AB_SUPPORT
        else if (g_eg_status_p->play_status == PlayAB)/*ab ����״̬*/
        {
            //��������,�ص���������״̬
            if ((g_eg_status_p->ab_status == PlayAB_Do) && (mmm_status.ab_play_flag == AB_PLAY_CLEAR))
            {
                g_eg_status_p->ab_status = PlayAB_A;
            }
        }
        else
        {
            ; //����˲�����������
        }
#endif //AB_SUPPORT
        break;

        case MMM_MP_ENGINE_STOPPED: //ֹͣ
        //sd �ļ�����ֻ��Ҫֹͣ
        if ((g_fsel_type == FSEL_TYPE_SDFILE) || (g_mengine_enter_mode == ENTER_ALARM))
        {
            stop(STOP_NORMAL); //ֹͣ����,���ļ��ر�
            break;
        }

        //�������ŵ�β or �����β or ���˵�ͷ
#ifdef AB_SUPPORT
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayAB))
#else
        if (g_eg_status_p->play_status == PlaySta)
#endif
        {
            //����β�ˣ�������û�����ĸ裬��Ϊ���д���ĸ�
            if (g_eg_playinfo_p->cur_time == 0)
            {
                g_eg_status_p->err_status = EG_ERR_DECODER_ERROR;
                error_handle2();
                break;
            }
            //�������ŵ�β
            switch_retval = mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);

            if (switch_retval == SWITCH_ERR_READ_CARD_TIMEOUT)
            {
                g_eg_status_p->err_status = EG_ERR_RAED_CARD_TIMEOUT;
            }
        }
        else if (g_eg_status_p->play_status == PlayFast)
        {
            if (g_eg_status_p->fast_status == FFPlaySta)
            {
                //�����β��һ���������
                switch_retval = mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_FFON);
            }
            else
            {
                //���˵�ͷ��һ����������
                switch_retval = mengine_file_switch(STOP_NORMAL, FORCE_SWITCH_PREV, PLAY_FBON);
            }
        }
        else
        {
            ; //do nothing
        }
        break;

        case MMM_MP_ENGINE_ERROR: //����
        //ֹͣ���ֱ������״̬
        error_handle();
        break;

        default:
        break;
    }

    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        change_engine_state(ENGINE_STATE_PAUSE);
    }
    else
    {
        change_engine_state(ENGINE_STATE_PLAYING);
    }
}

void megine_check_status(void)
{
    g_check_status_flag = TRUE;
}

void me_update_share(void)
{
    g_mengine_info_p = sys_share_query_update(APP_ID_MENGINE);
    g_eg_status_p = &(g_mengine_info_p->eg_status);
    g_eg_playinfo_p = &(g_mengine_info_p->eg_playinfo);

}

//����ϵ���Ϣ��rtc_ram��reset������0
void save_bk_info_to_rtc_ram(void)
{
    mmm_mp_bp_info_t bk_info;
    mmm_mp_cmd(g_mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int)&bk_info);
    act_writel((uint32)bk_info.bp_file_offset, RTC_BAK1);
    act_writel((uint32)bk_info.bp_time_offset, RTC_BAK2);
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//nothing
    }
}

app_result_e mengine_control_block(void)
{
    private_msg_t cur_msg_val;
    app_result_e mengine_retval = RESULT_NULL;
    uint8 save_bk_info_cnt = 0;
    //250ms��ѯһ���м��״̬
    g_status_timer = set_app_timer(APP_TIMER_ATTRB_CONTROL, 250, megine_check_status);
    while (1)
    {
        //���¹����ѯ
        me_update_share();

        //��ѯ��ǰ����״̬
        if (g_check_status_flag == TRUE)
        {
            g_check_status_flag = FALSE;
            if (g_eg_status_p->err_status == EG_ERR_NONE)
            {
                mengine_status_deal();
            }
            save_bk_info_cnt++;
            if(save_bk_info_cnt >= 20)//250*20 = 5000ms
            {
                save_bk_info_to_rtc_ram();
                save_bk_info_cnt = 0;
            }
        }

        //��ȡ��������ϸ
        if (get_app_msg(&cur_msg_val) == TRUE)
        {
            mengine_retval = mengine_message_done_bank(&cur_msg_val);
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            //�˳�Ӧ�ùرմ���
            stop(STOP_NORMAL);
            break;
        }

        //����g_delay_time * 10ms �����񽻻�
        sys_os_time_dly(g_delay_time);
    }

    kill_app_timer(g_status_timer);

    return mengine_retval;
}

void mengine_reply_msg(void* msg_ptr, bool ret_vals)
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

    me_update_share();

    //�ظ�ͬ����Ϣ(�����ź���)
    libc_sem_post(data_ptr->sem);
}

void m_decrypt_func(void *buf, int offset, int size)
{
    int i;
    uint8 *p = buf;
    
    for (i = 0; i<size; i++)
    {
        p[i] ^= 0x5a;
    }
}

