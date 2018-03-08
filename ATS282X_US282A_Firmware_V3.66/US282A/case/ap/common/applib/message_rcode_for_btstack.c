/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Ӧ��˽����Ϣ���սӿ�ʵ�֣�������Ӧ�ú�BT STACK�е��ã��ýӿڻ�ִ��
 *       ��ʱ�������һ����DSP��ӡ���жϴ�ӡ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ӧ��˽����Ϣ����Щ�����ڽ���ϵͳ��Ϣʱת���ģ���ר��BTSTACKʹ�õľ���汾��
 * \param[in]    none
 * \param[out]   btstack_msg ���ؽ��յ���Ӧ��˽����Ϣ
 * \return       bool
 * \retval           TRUE ����Ӧ��˽����Ϣ�ɹ���
 * \retval           FALSE ����Ӧ��˽����Ϣʧ�ܡ�
 * \note
 *******************************************************************************/
bool get_app_msg_btstack(btstack_msg_t *btstack_msg)
{
    //��ʱ��ɨ��
    if (g_app_timer_count > 0)
    {
        handle_timers(g_app_timer_vector, g_app_timer_count);
    }

    //���� Ӧ��˽����Ϣ
    if (sys_mq_receive(MQ_ID_BT, (void *) btstack_msg) == 0)
    {
        return TRUE;
    }

    //��Ϣ�����ѿգ�����false
    return FALSE;
}

/*! \endcond */
