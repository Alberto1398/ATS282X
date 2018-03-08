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
 *    ����Ӧ��˽����Ϣ����Щ�����ڽ���ϵͳ��Ϣʱת���ģ���ר����̨����ʹ�õľ���汾��
 * \param[in]    none
 * \param[out]   private_msg�����ؽ��յ���Ӧ��˽����Ϣ
 * \return       bool
 * \retval           TRUE ����Ӧ��˽����Ϣ�ɹ���
 * \retval           FALSE ����Ӧ��˽����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1����̨����Ӧ��˽����Ϣ����
 private_msg_t private_msg;
 app_result_e result;
 bool ret;

 ret = get_app_msg(&private_msg);
 if(ret == TRUE)
 {
 result = msg_callback(&private_msg);
 if(result != RESULT_NULL)
 {
 �ǿշ���ֵ����
 }
 }
 * \endcode
 * \note
 *******************************************************************************/
bool get_app_msg(private_msg_t *private_msg)
{
    //��ʱ��ɨ��
    if (g_app_timer_count > 0)
    {
        handle_timers(g_app_timer_vector, g_app_timer_count);
    }

    //sys_dsp_print();

    //���� Ӧ��˽����Ϣ
    if (sys_mq_receive((uint32) (g_this_app_info->mq_id), (void *) private_msg) == 0)
    {
        return TRUE;
    }

    //��Ϣ�����ѿգ�����false
    return FALSE;
}

/*! \endcond */
