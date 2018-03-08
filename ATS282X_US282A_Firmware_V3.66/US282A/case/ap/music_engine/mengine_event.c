#include "app_mengine.h"

app_result_e mengine_get_filepath(void* msg_ptr)
{
    //uint8 location_size;
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //����·����ָ��
    file_path_info_t* locat_info = (file_path_info_t*) (data_ptr->reply->content);

    //��Ŀ¼ģʽ
    //location_size = sizeof(file_location_t);

    libc_memcpy(&(locat_info->file_path), &(g_eg_cfg_p->location), sizeof(file_location_t));

    locat_info->file_source = g_fsel_type; //��ǰ�ļ�ģʽ

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

app_result_e mengine_get_playmode(void* msg_ptr)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    if (g_shuffle_flag == SHUFFLE)
    {
        *(uint8 *) (data_ptr->reply->content) = FSEL_MODE_RANOM;
    }
    else
    {
        *(uint8 *) (data_ptr->reply->content) = g_repeat_mode;
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
