/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ϣ�㲥�ӿڣ�ֻ�������첽��Ϣ�㲥������ϵͳ��Ϣ����ͨ���㲥��ǰ̨Ӧ�ã�
 *       ����ǰ̨Ӧ�ô���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    �Թ㲥��ʽ��Ҳ�����첽���ͣ�����Ӧ��˽����Ϣ��
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[in]    only_front �����͸�ǰ̨Ӧ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �㲥��Ϣ�ɹ���
 * \retval           FALSE �㲥��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1���㲥ϵͳ��Ϣ���� get_sys_msg ʵ��
 * \endcode
 * \note
 * \li  �㲥Ӧ����Ϣ���ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
 * \li  �ýӿڲ����ϵͳ��Ϣ�㲥�� manager ap ˽����Ϣ�����С�
 *******************************************************************************/
bool broadcast_msg(msg_apps_t *msg, bool only_front)
{
    app_info_t *app;
    uint8 i;

    //�㲥����Ϣ������Ӧ��
    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if (app->used == 1)
        {
            if ((only_front == FALSE) || (app->app_type == APP_TYPE_GUI))
            {
                send_async_msg(app->app_id, msg);
            }
        }
    }

    return TRUE;
}

/*! \endcond */
