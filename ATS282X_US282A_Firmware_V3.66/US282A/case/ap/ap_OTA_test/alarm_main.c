/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������켰����Ŀ¼ɨ�蹦����ڣ�����Ӧ�ó�ʼ����Ӧ���˳��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_alarm.h"


//ϵͳȫ�ֱ���
comval_t g_comval;

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     3

/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t alarm_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

int32 g_fs_mount_id = -1;
uint32 g_dir_file_total;
file_path_info_t g_alarm_path;

time_t g_next_alarm_time;
uint8 g_next_alarm_index;
bool g_has_next_alarm;
bool g_next_alarm_already_ring;

alarm_ring_var_t g_alarm_ring_var;
alarm_ring_var_t *g_p_alarm_ring;

OS_STK *ptos = (OS_STK *) AP_ALARM_STK_POS;
INT8U prio = AP_ALARM_PRIO;
sdd_open_arg_t g_sdd_open_parm;
uint32 g_display_sec = 0;


/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    //ϵͳ������ȡ
    com_setting_comval_init(&g_comval);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    sys_vm_write(&g_comval, VM_AP_SETTING, sizeof(comval_t));
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 * \li  ��ʼ���ļ�ѡ����ʱ����Ĭ���̷�Ϊ���̣��ļ�����ΪCOMMONDIR
 * \li  �����̨��music���Ż��browser�Ƚ��룬ͬ��location
 * \li  music uiĬ��û���̷����䣬��playlist������˳����̷�д��browser VM����
 */
/*******************************************************************************/
bool _app_init(void)
{
    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_ALARM, APP_TYPE_GUI);

    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(alarm_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //������init_app_timers �������
    sys_timer_init();

    com_rcp_init();//������xxx_rcp_var_init֮ǰ

    com_view_manager_init();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ��� ����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    com_view_manager_exit();

    //����ϵͳ��ʱ��
    sys_timer_exit();

    //ִ��applib���ע������
    applib_quit();

    //save config дvram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e retval;

    g_standby_exit_flag = (bool) ((argc & PARAM_S3_EXIT) != 0);
    argc = argc & (~PARAM_S3_EXIT);

    //g_p_alarm_ring = &g_alarm_ring_var;
    //g_p_alarm_ring->cur_alarm_record.alarm.ring_type = RING_TYPE_BUILTIN;

    //��ʼ��
    if (_app_init() == FALSE)
    {
        //��ʾ����(��ʾERRO)
        retval = RESULT_NEXT_FUNCTION;
        goto alarm_exit;
    }
    retval = alarm_ring_entry();
    alarm_exit:

    if (retval == RESULT_SYSTEM_ENTER_S3)
    {
        PRINT_INFO("ap enter s3bt");
    }
    retval = RESULT_NEXT_FUNCTION;
    com_ap_switch_deal(retval);

    //�˳�
    _app_deinit();
    return retval;
}
