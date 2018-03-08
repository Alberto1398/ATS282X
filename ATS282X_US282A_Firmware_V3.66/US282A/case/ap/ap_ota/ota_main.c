/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������켰����Ŀ¼ɨ�蹦����ڣ�����Ӧ�ó�ʼ����Ӧ���˳��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "ap_ota.h"


//ϵͳȫ�ֱ���
comval_t g_comval;
otaval_t g_otaval;

bool display_flag = FALSE;
int8 display_timer_id = -1;
uint8 g_ota_status = OTA_STATUS_IDLE;
bool g_need_draw = FALSE;
uint8 g_tick = 0;

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     3

/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t ota_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

OS_STK *ptos = (OS_STK *) AP_OTA_STK_POS;
INT8U prio = AP_OTA_PRIO;



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

void _load_vram_ota(void)
{
    sys_vm_read(&g_otaval, VM_OTA_UPGRADE, sizeof(otaval_t));
}

void _save_vram_ota(void)
{
    sys_vm_write(&g_otaval, VM_OTA_UPGRADE, sizeof(otaval_t));
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
	_load_vram_ota();
	
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_OTA_UPGRADE, APP_TYPE_GUI);

    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(ota_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

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


void sys_reboot(void)
{
    sys_local_irq_save();
    ENABLE_WATCH_DOG(1);
    while(1)
    {
        ;
    }
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
    app_result_e retval = RESULT_NULL;
	PRINT_INFO(" OTA_MAIN ");

    //��ʼ��
    if (_app_init() == FALSE)
    {
        //��ʾ����(��ʾERRO)
        goto ota_exit;
    }

	ota_create_main_view();
	
	//����������̨
	com_btmanager_init(FALSE);
	
    retval = get_message_loop();
	
ota_exit:
	g_otaval.ota_upgrade_flag = 0;	// ���
	_save_vram_ota();
	
	PRINT_INFO(" OTA_EXIT ");
	sys_reboot();
    return retval;
}


