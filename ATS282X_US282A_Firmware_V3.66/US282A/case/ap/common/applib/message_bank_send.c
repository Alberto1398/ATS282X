/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ͬ����Ϣ���ͽӿڣ�����Ӧ��֮�䷢��ͬ����Ϣ��ʹ��ͬ����Ϣ���������Ҫ��֤
 *       ��Ϣ���շ��յ����ظ�������ǰ̨Ӧ�û��������״̬��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

static app_info_t *__app_running_get_app_info(uint8 app_id);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ͬ����ʽ����Ӧ��˽����Ϣ��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[in]    reply ָ��ͬ����ϢӦ��ṹ��
 * \param[in]    timeout ��ʱ����ʱ�䣬0 ��ʾһֱ�ȴ�����ʱ����
 * \param[out]   none
 * \return       int
 * \retval           0  ����ͬ����Ϣ�ɹ���
 * \retval           -1 Ŀ��Ӧ�ò����ڡ�
 * \retval           -2 ��Ϣ����������
 * \retval           -3 ����ͬ����Ϣ������
 * \retval           -4 ͬ����Ϣ��Ӧ��ʱ��
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1������ʽ����ͬ����Ϣ��ֱ���Է����� libc_sem_post Ӧ��
 msg_apps_t msg;
 msg_reply_t reply;
 int ret;

 msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
 reply.content = (void *)file_path;
 ret = send_sync_msg(APP_ID_MANAGER, &msg, &reply, 0);
 if (ret == ERR_NO_ERR)
 {
 if (reply.type == MSG_REPLY_SUCCESS)
 {
 ��ȷ���أ���ȡ�������ļ�·��
 }
 else
 {
 ���󷵻�
 }
 }
 else
 {
 ��������
 }
 * \endcode
 * \note
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã���Ϣ��������������ͬ����Ϣ���Լ���ͬ����Ϣ��Ӧ��ʱ��
 * \li  ע����Ϣ���ݻ����� msg.content.addr �� Ӧ�𻺳��� reply.content �Ŀռ䶼�ǿ�����Ϣ���ͷ��ġ�
 *      ���ڷ���ͬ����Ϣ���ȴ����أ�������� send_sync_msg �Ĵ������� control bank�Σ���ô�����
 *      ���������Կ���bank data�С�
 *******************************************************************************/
int send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    const app_info_t *target_app_info;
    msg_reply_t temp_reply;
    os_event_t *sem; //ͬ����Ϣͬ���ź���ָ��
    int retvalue = ERR_NO_ERR;

    PRINT_DBG_INT("sync msg:", msg->type);

    //����ʱ�ȴ��ź�����ͬʱֻ����1���̷߳���ͬ����Ϣ
    libc_sem_wait(thread_mutex, 0);

    //ͨ������IDת��Ϊ����APP ID
    if (app_id == APP_ID_THEENGINE)
    {
        app_id = get_engine_appid_by_type(get_engine_type());
        if (app_id == APP_ID_MAX)
        {
            goto app_noexist;
        }
    }

    //����Ŀ��Ӧ��
    target_app_info = __app_running_get_app_info(app_id);
    if (NULL == target_app_info)
    {
        app_noexist:
        libc_sem_post(thread_mutex);
        PRINT_ERR("sync msg ERR_TARGET_NOEXIST!!");
        return ERR_TARGET_NOEXIST;
    }

    //���ܷ���ͬ����Ϣ��Ӧ���Լ�
    if (target_app_info == g_this_app_info)
    {
        libc_sem_post(thread_mutex);
        PRINT_ERR("sync msg ERR_SYNC_ITSELF!!");
        return ERR_SYNC_ITSELF;
    }

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    if (reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //������Ϣ��������
    if (sys_mq_send((uint32) (target_app_info->mq_id), (void *) &cur_send_pmsg) < 0)
    {
        PRINT_ERR("sync msg ERR_MSGQUEUE_FULL!!");
        retvalue = ERR_MSGQUEUE_FULL;
    }
    else
    {
        //�ȴ�ͬ����Ϣ��Ӧ
        if (libc_sem_wait(sem, timeout) < 0)
        {
            PRINT_ERR("sync msg ERR_SYNC_TIMEOUT!!");
            retvalue = ERR_SYNC_TIMEOUT;
        }
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);

    PRINT_DBG_INT("sync msg ret:", msg->type);

    return retvalue;
}

/*! \endcond */

/*! \cond */

//����Ӧ��ID��ȡӦ����Ϣ
static app_info_t *__app_running_get_app_info(uint8 app_id)
{
    app_info_t *app;
    uint8 i;

    if (app_id >= APP_ID_MAX)
    {
        return NULL;
    }

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if ((app->used == 1) && (app->app_id == app_id))
        {
            return app;
        }
    }

    return NULL;
}

/*! \endcond */
