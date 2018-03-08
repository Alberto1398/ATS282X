/*******************************************************************************
 *                              US212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_msg_handle2.c
 * \brief    ��Ϣ����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"
#include  "common_btmanager.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ϣ�������ڿռ����ƽ�����2������
 * \param[in]    ˽����Ϣָ��
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void manager_msg_callback_sub(private_msg_t *pri_msg)
{
    int32 waitpid_ret;
    uint8 ap_ret;

    switch (pri_msg->msg.type)
    {
        //����bt stack
        case MSG_CREAT_APP_EXT_SYNC:
        {
            create_app_param_t *param = pri_msg->msg.content.addr;
            uint8 ap_name[12];
            void * ap_param;
            
            // OTA�汾��Ҫһ��OTA�汾��bt_stack
            if ( (g_app_info_vector[APP_TYPE_GUI].app_id == APP_ID_OTA_UPGRADE) && (param->app_id == APP_ID_BTSTACK) )
            {
                param->app_id = APP_ID_BTSTACK_OTA;
            }
            
            _get_app_name(ap_name, param->app_id, APP_TYPE_BTSTACK);
            ap_param = (void *)param->data;
            
            //wait for cur app exit
            libc_waitpid(0, 1);
            
            sys_free_ap(AP_BTSTACK_FILE);

            manager_mem_manager(param->app_id, TRUE);
            
            if (sys_exece_ap(ap_name, AP_BTSTACK_FILE, (int32) ap_param) >= 0)
            {
                pri_msg->reply->type = MSG_REPLY_SUCCESS;
            }
            else
            {
                pri_msg->reply->type = MSG_REPLY_FAILED;
            }
            libc_sem_post(pri_msg->sem);
        }
        break;

        case MSG_AUTOTEST_SET_SHARE_DATA_SYNC:
        {
            g_p_test_share_info = pri_msg->msg.content.addr;
            libc_sem_post(pri_msg->sem);
        }
        break;
        
        case MSG_STUB_INSTALL_SYNC:
        {
            *(uint8 *)pri_msg->reply->content = manager_open_stub(pri_msg->msg.content.data[0]);
            libc_sem_post(pri_msg->sem);
        }
        break;

        default:
        break;
    }

}
