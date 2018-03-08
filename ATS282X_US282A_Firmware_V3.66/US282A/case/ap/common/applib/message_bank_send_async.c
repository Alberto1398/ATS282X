/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������첽��Ϣ���ͽӿڣ�����Ӧ��֮�䷢���첽��Ϣ��Ҳ������ǰ̨COMMON��������
 *       �첽��Ϣ���Խ���COMMON��Ӧ��֮�������ԡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

static app_info_t *__app_running_get_app_info(uint8 app_id);

/******************************************************************************/
/*!
 * \par  Description:
 *    ���첽��ʽ����Ӧ��˽����Ϣ��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       int
 * \retval           0  �����첽��Ϣ�ɹ���
 * \retval           -1 Ŀ��Ӧ�ò����ڡ�
 * \retval           -2 ��Ϣ����������
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1���첽������Ϣ������ȴ���������
 msg_apps_t msg;
 int ret;

 msg.type = MSG_KILL_APP;
 msg.content.data[0] = APP_ID_MUSIC;
 ret = send_async_msg(APP_ID_MANAGER, &msg);
 if (int != ERR_NO_ERR)
 {
 ��������
 }
 * \endcode
 * \note
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã���Ϣ����������
 * \li  ˽����Ϣ�첽���ͷ�ʽ�����ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
 *******************************************************************************/
int send_async_msg(uint8 app_id, msg_apps_t *msg)
{
    const app_info_t *target_app_info;
    private_msg_t cur_send_pmsg;
    uint8 mq_id;

    PRINT_INFO_INT("async msg:", msg->type);

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
    if (target_app_info == NULL)
    {
        app_noexist:
        PRINT_ERR("async msg ERR_TARGET_NOEXIST!!");
        return ERR_TARGET_NOEXIST;
    }
    mq_id = target_app_info->mq_id;

    send_msg:
    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //������Ϣ��������
    if (sys_mq_send((uint32) (mq_id), (void *) &cur_send_pmsg) < 0)
    {
        PRINT_ERR("async msg ERR_MSGQUEUE_FULL!!");
        return ERR_MSGQUEUE_FULL;
    }

    return ERR_NO_ERR;
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
