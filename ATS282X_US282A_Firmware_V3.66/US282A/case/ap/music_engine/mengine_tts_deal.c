#include "app_mengine.h"

//�ر��м��,�����м���߳�
int32 close_mmm(void)
{
    if(g_mmm_opened == TRUE)
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);
        g_mmm_opened = FALSE;
    }
    return 0;
}

int32 open_mmm(void)
{
    int32 open_ret;
    if(g_mmm_opened == TRUE)
    {
        return 0;
    }
    open_ret = mmm_mp_cmd(&g_mp_handle, MMM_MP_OPEN, (unsigned int) NULL);
    if((open_ret != -1)&&(1 == g_app_info_state_all.fix_sample_rate_flag))
    {
        mmm_mp_cmd(&g_mp_handle, MMM_MP_FIX_SAMPLE_RATE, NULL);
    }
    g_mmm_opened = TRUE;
    
    return open_ret;
}

app_result_e mengine_tts_start(void* msg_ptr)
{
    //���ò������͵�ָ��
    //private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    bool ret_vals = TRUE;

    if ((g_eg_status_p->play_status == PlayFast) 
        || (g_eg_status_p->play_status == PlaySta)
        || (g_eg_status_p->play_status == PlayAB))
    {
        //����ͣ����
        ret_vals = stop(STOP_PAUSE); //��ͣ����
        if (ret_vals == TRUE)
        {
            g_stop_by_tts_flag = TRUE;
        }
    }
    close_mmm();

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

app_result_e mengine_tts_stop(void* msg_ptr)
{
    //���ò������͵�ָ��
    //private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    bool ret_vals = TRUE;

    open_mmm();

    if (g_stop_by_tts_flag == TRUE)
    {

        if (g_eg_status_p->err_status == EG_ERR_NONE)
        {
            ret_vals = play(PLAY_RESUME); //����
        }
        g_stop_by_tts_flag = FALSE;
    }

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//STANDBY������Ӧ
app_result_e mengine_standby(void* msg_ptr)
{
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    os_event_t *standby_sem = (os_event_t *)(data_ptr->msg.content.addr);

    //�����ֳ���׼������˯��ģʽ
    PRINT_INFO("engine standby enter");

    //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
    mengine_reply_msg(data_ptr, TRUE);

    libc_sem_wait(standby_sem, 0);

    //��˯��ģʽ�������ָ��ֳ�
    PRINT_INFO("engine standby exit");
    standby_restart_all_app_timer(g_app_timer_vector, g_app_timer_count);

    return RESULT_IGNORE;
}

