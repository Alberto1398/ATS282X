/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������BT MANAGERר��ͬ���¼����ͽӿڣ�������BT STACK����BT������¼�Ӧ�����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ͬ����ʽ����Ӧ��˽����Ϣ��
 * \param[in]    bd_addr ���������豸��ַ
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
 * \note
 * \li  ���ش���ԭ���������Ϣ����������ͬ����Ϣ��Ӧ��ʱ��
 * \li  ע����Ϣ���ݻ����� msg.content.addr �� Ӧ�𻺳��� reply.content �Ŀռ䶼�ǿ�����Ϣ���ͷ��ġ�
 *      ���ڷ���ͬ����Ϣ���ȴ����أ�������� send_sync_msg �Ĵ������� control bank�Σ���ô�����
 *      ���������Կ���bank data�С�
 *******************************************************************************/
int send_sync_msg_btmanager(uint8 *bd_addr, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    btstack_msg_t cur_msg;
    msg_reply_t temp_reply;
    os_event_t *sem; //ͬ����Ϣͬ���ź���ָ��
    int retvalue = ERR_NO_ERR;
    uint8 mq_id = MQ_ID_BT;

    PRINT_DBG_INT("sync msg:", msg->type);

    if (g_app_info_vector[APP_TYPE_BTSTACK].used == 0)
    {
        return 0;
    }

    //����ʱ�ȴ��ź�����ͬʱֻ����1���̷߳���ͬ����Ϣ
    libc_sem_wait(thread_mutex, 0);

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_msg.msg), msg, sizeof(msg_apps_t));
    if (bd_addr != NULL)
    {
        libc_memcpy(cur_msg.bd_addr, bd_addr, BD_ADDR_LEN);
    }
    else
    {
        libc_memset(cur_msg.bd_addr, 0x0, BD_ADDR_LEN);
    }

    libc_sem_init(&sem, 0);
    cur_msg.sem = sem;
    if (reply == NULL)
    {
        cur_msg.reply = &temp_reply;
    }
    else
    {
        cur_msg.reply = reply;
    }

    //������Ϣ��������
    if (sys_mq_send((uint32) (mq_id), (void *) &cur_msg) < 0)
    {
        //PRINT_ERR("sync msg ERR_MSGQUEUE_FULL!!");
        retvalue = ERR_MSGQUEUE_FULL;
    }
    else
    {
        //�ȴ�ͬ����Ϣ��Ӧ
        if (libc_sem_wait(sem, timeout) < 0)
        {
            //PRINT_ERR("sync msg ERR_SYNC_TIMEOUT!!");
            retvalue = ERR_SYNC_TIMEOUT;
        }
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);

    PRINT_DBG_INT("sync msg ret:", msg->type);

    return retvalue;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���첽��ʽ����Ӧ��˽����Ϣ��
 * \param[in]    bd_addr ���������豸��ַ
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       int
 * \retval           0  �����첽��Ϣ�ɹ���
 * \retval           -1 Ŀ��Ӧ�ò����ڡ�
 * \retval           -2 ��Ϣ����������
 * \note
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã���Ϣ����������
 * \li  ˽����Ϣ�첽���ͷ�ʽ�����ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
 *******************************************************************************/
int __section__(".bank") send_async_msg_btmanager(uint8 *bd_addr, msg_apps_t *msg)
{
    btstack_msg_t cur_msg;
    uint8 mq_id = MQ_ID_BT;

    PRINT_INFO_INT("async msg:", msg->type);

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_msg.msg), msg, sizeof(msg_apps_t));
    if (bd_addr != NULL)
    {
        libc_memcpy(cur_msg.bd_addr, bd_addr, BD_ADDR_LEN);
    }
    else
    {
        libc_memset(cur_msg.bd_addr, 0x0, BD_ADDR_LEN);
    }

    //������Ϣ��������
    if (sys_mq_send((uint32) (mq_id), (void *) &cur_msg) < 0)
    {
        PRINT_ERR("async msg ERR_MSGQUEUE_FULL!!");
        return ERR_MSGQUEUE_FULL;
    }

    return ERR_NO_ERR;
}

/*! \endcond */
