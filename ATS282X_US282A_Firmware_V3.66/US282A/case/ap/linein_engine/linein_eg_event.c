#include "app_linein_eg.h"

app_result_e linein_eg_get_status(void* msg_ptr)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //copy��ǰ����״̬����Ϣָ��ָ���������
    libc_memcpy(data_ptr->reply->content, g_eg_status_p, sizeof(linein_eg_status_t));

    linein_eg_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
