#include "app_linein_eg.h"

//����״̬����
void linein_eg_status_deal(void)
{
    if (g_linein_channel_sel == LINEIN_CH_A)
    {
    }
    else
    {
        mmm_pp_status_t play_status;
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_GET_STATUS, (unsigned int) &play_status); //��ȡ����ʱû��
    }

    if (g_eg_status_p->play_status == linein_play_status_stop)
    {
        change_engine_state(ENGINE_STATE_PAUSE);
    }
    else
    {
        change_engine_state(ENGINE_STATE_PLAYING);
    }
}

//�������Ϣ����ѭ��
app_result_e linein_eg_control_block(void)
{
    private_msg_t cur_msg_val;

    app_result_e linein_eg_retval = RESULT_NULL;

    //uint32 msg_type;

    //g_linein_eg_result = RESULT_NULL;

    while (1)
    {
        //���¹����ѯ
        g_eg_status_p = sys_share_query_update(APP_ID_LINEIN_EG);

        //��ȡ�м����ǰ����״̬
        linein_eg_status_deal();

        //��ȡ��������ϸ
        if (get_app_msg(&cur_msg_val) == TRUE)
        {
            linein_eg_retval = linein_eg_message_done_bank(&cur_msg_val);
        }

        if (linein_eg_retval == RESULT_APP_QUIT)
        {
            if (g_linein_channel_sel == LINEIN_CH_A)
            {
                linein_eg_aa_stop();
            }
            else
            {
                //�˳�Ӧ�ùرմ���
                stop();
            }
            break;
        }

        //����20ms �����񽻻�
        sys_os_time_dly(2);
    }

    return linein_eg_retval;
}

void linein_eg_reply_msg(void* msg_ptr, bool ret_vals)
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

