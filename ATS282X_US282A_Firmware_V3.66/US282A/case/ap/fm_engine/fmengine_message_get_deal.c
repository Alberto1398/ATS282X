#include "app_fmengine.h"


//��ȡ״̬��Ϣ
app_result_e fmengine_get_status(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    Engine_Status_t engine_status_tmp;
    uint8 mode;

    //��ȡ״̬�ڵ�:   0-- ��������ʱȡ   1-- Ӳ��seek ������ȡ
    mode = data_ptr->msg.content.data[0];
    result = fm_get_status(&engine_status_tmp, mode);

    //��ȡ״̬�ɹ�//0�ɹ� -1ʧ��
    if (result != -1)
    {
        engine_status_tmp.FM_VolStatus = g_fmengine_status.FM_VolStatus;
        libc_memcpy(&g_fmengine_status, &engine_status_tmp, sizeof(Engine_Status_t));

        //��ϢӦ��
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        //������״̬�ظ���RadioUI
        libc_memcpy(data_ptr->reply->content, &g_fmengine_status, sizeof(Engine_Status_t));
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//��ȡ����
app_result_e fmengine_get_band(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    result = fm_get_band(&g_fmengine_status.FM_CurBand);

    if (result != -1) //0�ɹ� -1ʧ��
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(radio_band_e*) data_ptr->reply->content = g_fmengine_status.FM_CurBand;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;

}


//��ȡ��ǰ�ź�ǿ��
app_result_e fmengine_get_intensity(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

    result = fm_get_intensity(&g_fmengine_status.FM_SignalStg);
    if (result != -1) //0�ɹ� -1ʧ��
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(uint16*) data_ptr->reply->content = g_fmengine_status.FM_SignalStg;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//��ȡ����״̬
app_result_e fmengine_get_antenna(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    uint8 antena_status;
    result = fm_get_antena(&antena_status);

    if (result != -1) //0�ɹ� -1ʧ��
    {
        //��ȡ״̬�ɹ�
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        //��ִ�������Ƿ����
        *(uint8*) data_ptr->reply->content = antena_status;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//��ȡ������״̬
app_result_e fmengine_get_stereo(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    result = fm_get_stereo(&g_fmengine_status.FM_Stereo_Status);

    if (result != -1) //0�ɹ� -1ʧ��
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(FM_Audio_e*) data_ptr->reply->content = g_fmengine_status.FM_Stereo_Status;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//��ȡ��ǰ����״̬(����or  ��ͣ)
app_result_e fmengine_get_playflag(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    *(FM_Playstatus_e*) data_ptr->reply->content = g_fmengine_status.FM_VolStatus;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

//��ȡFM ģ�鵱ǰ״̬(Work/Standby)
app_result_e fmengine_get_modual_status(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    *(Module_Status_e*) data_ptr->reply->content = g_module_status;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //�ظ�ͬ����Ϣ(�����ź���)
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

