#include "app_fmengine.h"

//����Ӳ����̨�����У���ȡӲ����̨�Ƿ������ʶ
app_result_e fmengine_get_seekflag(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    uint8 over_flag;

    result = fm_get_seekflag(&over_flag);

    //�ɹ���ȡ����ʶ
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;

        //Ӳ����̨����
        if ((over_flag & 0x01) == 1) //bit0  Ϊ1
        {
            if ((over_flag & 0x02) != 0) //bit1 Ϊ1
            {
                //��Ч̨
                *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_COMPLETE;
                g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
            }
            else
            {
                //δ�ҵ���Ч̨
                *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_COMPLETE_NOT_VALID;
                g_fmengine_status.STC_flag = HARDSEEK_COMPLETE_NOT_VALID;
            }
        }
        else if ((over_flag & 0x04) != 0)
        {
            *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_FALSE;
            g_fmengine_status.STC_flag = HARDSEEK_FALSE;
        }
        else
        {
            *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_DOING;
            g_fmengine_status.STC_flag = HARDSEEK_DOING;
        }
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//��ȡ��ǰƵ��
app_result_e fmengine_get_freq(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    result = fm_get_freq(&g_fmengine_status.FM_CurrentFreq);

    if (result != -1) //0�ɹ� -1ʧ��
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(uint32*) data_ptr->reply->content = g_fmengine_status.FM_CurrentFreq;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

