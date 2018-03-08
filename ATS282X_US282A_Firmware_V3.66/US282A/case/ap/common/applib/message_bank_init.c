/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ����Ϣ��ʼ���ӿڣ������GUI��Ϣ���к�˽����Ϣ���С�
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
extern bool com_gui_msg_hook(input_gui_msg_t *input_msg) __FAR__;
extern void com_app_msg_hook(private_msg_t *private_msg) __FAR__;
bool applib_message_init(void)
{
    //��� Ӧ����Ϣ����
    sys_mq_flush(g_this_app_info->mq_id);

    g_this_gui_msg_hook = com_gui_msg_hook;
    g_this_app_msg_hook = com_app_msg_hook;

    return TRUE;
}

/*! \endcond */
