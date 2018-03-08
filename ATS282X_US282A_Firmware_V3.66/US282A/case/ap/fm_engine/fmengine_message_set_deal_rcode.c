#include "app_fmengine.h"
#include "mmm_mr.h"
#include <analog.h>

//����Ƶ��
app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 freq;
    int result;

    freq = *(uint32*) (data_ptr->msg.content.addr); //��λ:kHz
    result = fm_set_freq(freq);

    /* ����Ƶ��ɹ�*/ //0�ɹ� -1ʧ��
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}


//���������̨
app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    //uint16 now_freq;
    int result;
    //uint8 dir;
    seek_param_t soft_param;

    seek_param_t* param_ptr = (seek_param_t*) (data_ptr->msg.content.addr);

    //�����̨�ӿں�������
    soft_param.freq = param_ptr->freq; //�����̨��ǰ�ж�Ƶ��

    soft_param.dir = param_ptr->dir; //��̨����
    //for search too long
    change_engine_state(ENGINE_STATE_PLAYING);

    result = fm_soft_seek(soft_param.freq, soft_param.dir);
    //-1:û�ź�
    //0: ���ź�
    //1: ������

    g_fmengine_status.FM_CurrentFreq = soft_param.freq; //��������㵱ǰƵ��ֵ

    if (result != -1) //0�ɹ� -1ʧ��
    {
        //��Ч��̨
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        //��Ч��̨
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;

}

//����Ӳ����̨
app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    //uint16 now_freq;
    //uint8 dir;
    int result;
    seek_param_t hard_param;

    seek_param_t* param_ptr = (seek_param_t*) (data_ptr->msg.content.addr);

    //�����̨�ӿں�������
    hard_param.freq = param_ptr->freq; //�����̨��ǰ�ж�Ƶ��
    //dir = data_ptr->msg.content.data[2]; //��̨����
    hard_param.dir = param_ptr->dir; //��̨����

    //Ӳ����̨�ӿں�������
    //now_freq = *(uint16*) &(data_ptr->msg.content.data[0]); //Ӳ����̨��ʼƵ��
    //dir = data_ptr->msg.content.data[2]; //��̨����
    //for search too long
    change_engine_state(ENGINE_STATE_PLAYING);
    //����Ӳ����̨
    result = fm_hard_seek(hard_param.freq, hard_param.dir);

    if (result != -1) //0�ɹ� -1ʧ��
    {
        //�����ɹ�
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_status.STC_flag = HARDSEEK_DOING;
    }
    else
    {
        //����ʧ��
        data_ptr->reply->type = MSG_REPLY_FAILED;
        g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//�ж�Ӳ����̨
app_result_e fmengine_seek_break(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

    result = fm_set_breakseek();

    //�ɹ�ֹͣӲ����̨//0�ɹ� -1ʧ��
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        fm_get_freq(&g_fmengine_status.FM_CurrentFreq); //���µ�ǰƵ��
        g_fmengine_status.STC_flag = HARDSEEK_COMPLETE_NOT_VALID;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;

}
