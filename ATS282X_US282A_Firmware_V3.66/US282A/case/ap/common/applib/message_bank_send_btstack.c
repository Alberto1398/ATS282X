/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������BT STACKר���¼����ͽӿڣ�������BT MANAGER����BT�¼���BT�¼��������첽��ʽ
 *       ���͡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    bt stack �����¼��� bt manager��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       int
 * \retval           0  �����첽��Ϣ�ɹ���
 * \retval           -1 Ŀ��Ӧ�ò����ڡ�
 * \retval           -2 ��Ϣ����������
 * \note
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã���Ϣ����������
 *******************************************************************************/
int send_async_event(uint8 app_id, btstack_event_t *ev)
{
    uint8 mq_id = MQ_ID_RES;

    PRINT_DBG_INT("btstack event:", ev->msg.type);

    //if (app_id != APP_ID_BT_MANAGER)
    //{
    //    return ERR_TARGET_NOEXIST;
    //}

    //������Ϣ��������
    if (sys_mq_send((uint32) (mq_id), (void *) ev) < 0)
    {
        //PRINT_ERR("bt evt MSG FULL!!");
        return ERR_MSGQUEUE_FULL;
    }

    return ERR_NO_ERR;
}

/*! \endcond */
