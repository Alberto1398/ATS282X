/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Ӧ�ú�BT STACK��Ϣ��ʼ���ӿڣ������˽����Ϣ���У������BT STACK����Ҫ
 *       ��BT MANAGER���¼�����Ҳ��ա�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʼ�� applib ��Ϣͨ�Ź���ģ�顣
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʼ����Ϣͨ�Ź���ģ��ɹ���
 * \retval           FALSE ��ʼ����Ϣͨ�Ź���ģ��ʧ��
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 applib_message_init();
 * \endcode
 * \note
 *******************************************************************************/
bool applib_message_init(void)
{
    //��� Ӧ����Ϣ����
    sys_mq_flush(g_this_app_info->mq_id);

    return TRUE;
}

/*! \endcond */
