/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ�ý���ϵͳ��Ϣ��˽����Ϣ�ӿ�ʵ�֣�ϵͳ��Ϣ����ת��Ϊ˽����Ϣ����ǰ̨
 *       Ӧ���н��ܲ���������ӿڽ���ǰ̨Ӧ���е��á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ϵͳ��Ϣ�����㲥������Ӧ�á�
 * \param[in]    none
 * \param[out]   sys_msg�����ؽ��յ���ϵͳ��Ϣ
 * \return       bool
 * \retval           TRUE ����ϵͳ��Ϣ�ɹ���
 * \retval           FALSE ����ϵͳ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note
 * \li  ��ȡϵͳ��Ϣ�󣬻�㲥������Ӧ�á�
 * \li  �ýӿ�ֻ��ǰ̨AP�� get_app_msg �е��ã�Ҳ����˵��ϵͳ��Ϣ�ӷ�����������
 *      ���ܻᱻ delay һЩʱ�䣬ֱ����ǰ̨AP������Ϣѭ����
 *******************************************************************************/
bool get_sys_msg(sys_msg_t *sys_msg)
{
    msg_apps_t msg;

    //���type��0����Ϊϵͳ��Ϣ����Ϊ2���ֽڣ�sys_mq_receive �����Ḳ�ǵ�type��4�ֽڣ���2�ֽڣ��ᵼ��type����
    sys_msg->type = MSG_NULL;
    if (sys_mq_receive(MQ_ID_SYS, (void *) sys_msg) == 0)
    {
        msg.type = sys_msg->type;
        return broadcast_msg(&msg, TRUE);
    }

    //��Ϣ�����ѿգ�����false
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ӧ��˽����Ϣ����Щ�����ڽ���ϵͳ��Ϣʱת���ģ���
 * \param[in]    none
 * \param[out]   private_msg�����ؽ��յ���Ӧ��˽����Ϣ
 * \return       bool
 * \retval           TRUE ����Ӧ��˽����Ϣ�ɹ���
 * \retval           FALSE ����Ӧ��˽����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1��Ӧ��˽����Ϣ����
 private_msg_t private_msg;
 app_result_e result;
 bool ret;

 ret = get_app_msg(&private_msg);
 if(ret == TRUE)
 {
 result = msg_callback(&private_msg);
 if(result == RESULT_REDRAW)
 {
 �����ػ�
 }
 else
 {
 ��������ֵ����
 }
 }
 * \endcode
 * \note
 *******************************************************************************/
bool get_app_msg(private_msg_t *private_msg)
{
    if (g_this_app_info->app_type == APP_TYPE_GUI) //ǰ̨Ӧ��
    {
        if (g_buffer_app_msg.msg.type != MSG_NULL) //������Ϣ��ֱ�ӿ���
        {
            libc_memcpy(private_msg, &g_buffer_app_msg, sizeof(private_msg_t));
            g_buffer_app_msg.msg.type = MSG_NULL;
            return TRUE;
        }
        else
        {
            sys_msg_t sys_msg;
            //��ȡϵͳ��Ϣ���㲥
            get_sys_msg(&sys_msg);
        }
    }
    //���� Ӧ��˽����Ϣ
    if (sys_mq_receive((uint32) (g_this_app_info->mq_id), (void *) private_msg) == 0)
    {
        g_this_app_msg_hook(private_msg);
        return TRUE;
    }

    //��Ϣ�����ѿգ�����false
    return FALSE;
}

/*! \endcond */
