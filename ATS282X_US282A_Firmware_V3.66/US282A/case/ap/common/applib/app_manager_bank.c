/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù���ģ�飬����Ӧ�ó�ʼ����Ӧ��ע���ӿڣ����� ap_manager �⣬����Ӧ��
 *       ��������Ӧ�ý���ʱ����Ӧ�ó�ʼ������Ӧ���˳�ʱ����Ӧ��ע����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info);

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵ�ע��ͳ�ʼ����
 * \param[in]    app_id Ӧ��ID���� APP_ID_MUSIC��APP_ID_MANAGER��APP_ID_MENGINE ��
 * \param[in]    type Ӧ�����ͣ��μ� app_type_e ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʾע��ͳ�ʼ���ɹ���
 * \retval           FALSE ��ʾע��ͳ�ʼ��ʧ�ܡ�
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
 ����1��ǰ̨Ӧ�� music AP ���� applib ģ��ע��ͳ�ʼ��
 applib_init(APP_ID_MUSIC, APP_TYPE_GUI);

 ����2����̨����Ӧ�� music engine AP ���� applib ģ��ע��ͳ�ʼ��
 applib_init(APP_ID_MENGINE, APP_TYPE_CONSOLE);

 ����3�����̹���Ӧ�� manager AP ���� applib ģ��ע��ͳ�ʼ��
 applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);
 * \endcode
 * \note
 * \li  ��Ӧ�� main ������ڴ����ã��ڴ�֮ǰ������������� applib �ӿڡ�
 *******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //����Ƶ��
    if (type == APP_TYPE_GUI)
    {
        adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL7, FREQ_NULL);
        g_app_info_state_all.ui_status = UI_STATUS_IDLE;
    }
    else if (type == APP_TYPE_CONSOLE)
    {
        adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);
    }
    else
    {
        //nothing
    }

    //��ȡ app_info �ṹ��ָ��
    cur_app_info = &g_app_info_vector[type];

    //��������ֹ�������
    sys_os_sched_lock();

    //��ʼ��Ӧ����Ϣ
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1; //��־�ѱ�����ʹ��
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    //cur_app_info->soft_watchdog_timeout_expires = 0;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }
    else if (type == APP_TYPE_BTSTACK)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_BT;
    }
    else if (type == APP_TYPE_GUI)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_DESK;
        g_app_info_state_all.app_state = APP_STATE_NO_PLAY; //ǰ̨APע�ᣬ��ʼ��Ϊ NO_PLAY ״̬
    }
    else
    {
        //message queue
        cur_app_info->mq_id = (uint8) MQ_ID_EGN;

        g_app_info_state_all.engine_type = get_engine_type_by_appid(app_id);
    }

    //�����������������
    sys_os_sched_unlock();

    g_this_app_info = cur_app_info;

    //��ʼ���̻߳����ź���
    libc_sem_init(&thread_mutex, 1);

    PRINT_INFO_INT("app init:", app_id);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵ�ע������
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʾע���ɹ���
 * \retval           FALSE ��ʾע��ʧ�ܡ�
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
 ����1��Ӧ�ý��� applib ģ��ע��
 applib_quit();
 * \endcode
 * \note
 * \li  ��Ӧ�� main �����˳������ã��ڴ�֮�󣬲���������� applib �ӿڡ�
 *******************************************************************************/
bool applib_quit(void)
{
    app_info_t cur_app_info;

    PRINT_INFO_INT("app quit:", g_this_app_info->app_id);

    //�����̻߳����ź���
    libc_sem_destroy(&thread_mutex);

    if (g_this_app_info->app_type == APP_TYPE_CONSOLE)
    {
        //�޸ĺ�̨��������Ϊ NONE
        g_app_info_state_all.engine_type = ENGINE_NULL;
        g_app_info_state_all.engine_state = ENGINE_STATE_NULL;
    }

    //�޸� app_info �ṹ��Ϊδ��ʹ��
    libc_memcpy(&cur_app_info, g_this_app_info, sizeof(app_info_t));
    cur_app_info.used = 0;
    cur_app_info.app_id = APP_ID_MAX;
    //cur_app_info.soft_watchdog_timeout_expires = 0;
    __app_running_set_app_info(g_this_app_info->app_id, &cur_app_info);

    //����Ƶ��
    if (g_this_app_info->app_type == APP_TYPE_GUI)
    {
        adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_NULL, FREQ_NULL);
    }
    else if (g_this_app_info->app_type == APP_TYPE_CONSOLE)
    {
        adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_NULL, FREQ_NULL);
    }
    else
    {
        //nothing
    }

    return TRUE;
}

/*! \endcond */

/*! \cond */

//����Ӧ��ID����Ӧ����Ϣ
static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info)
{
    app_info_t *app;
    uint8 i;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //���� app_id ��Ӧ app_info_t
    app = NULL;
    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if ((app->used == 1) && (app->app_id == app_id))
        {
            break;
        }
    }

    if (NULL == app)
    {
        return FALSE;
    }

    sys_os_sched_lock();
    libc_memcpy(app, app_info, sizeof(app_info_t));
    sys_os_sched_unlock();

    return TRUE;
}

/*! \endcond */
