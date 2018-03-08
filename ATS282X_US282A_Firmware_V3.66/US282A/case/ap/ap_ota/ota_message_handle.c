/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�ǰ̨Ӧ���û�����ʵ�֣����������¼���ϵͳ�¼�����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_ota.h"

app_result_e key_mode_deal(void);

const sys_event_map_t __section__(".rodata.se_maplist") ota_se_maplist[] =
{
    //{{   MSG_BTSTACK_ERR_HARDWARE_EXCEPTION, SYSMSG_STOP_TTS_YES}, deal_btstack_hd_err},
    /*! ������־ */
    {{   MSG_NULL, 0}, NULL},
};

const key_event_map_t __section__(".rodata.ke_maplist") ota_ke_maplist[] =
{
    { {   KEY_MODE, 0, KEY_TYPE_SHORT_UP, 0}, key_mode_deal },
	
    /*! ������־ */
    { {   KEY_NULL, 0, KEY_TYPE_NULL, 0}, NULL},
};


app_result_e key_mode_deal(void)
{
	PRINT_INFO(" key_mode_deal ");
    return RESULT_APP_QUIT;
}

