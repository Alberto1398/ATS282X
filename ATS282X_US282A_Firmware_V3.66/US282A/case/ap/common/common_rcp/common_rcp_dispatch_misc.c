/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP ����ַ�������Խӿڣ��ͻ��ɶ��ο�����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/* Ӧ���л������ӳ��ΪӦ���л���Ϣ */
const msg_apps_type_e rcp_cmd_apswitch_event[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    EVENT_ENTER_BTPLAY, /* 10 �����Ƹ�Ӧ�� */
    EVENT_ENTER_MUSIC, /* 11 musicӦ�� */
    EVENT_ENTER_LINEIN, /* 12 lineinӦ�� */
    EVENT_ENTER_RADIO, /* 13 fmӦ�� */
    EVENT_ENTER_ALARM_RCP, /* 14 ����Ӧ�� */
    EVENT_ENTER_MUSIC_CARD, /* 15 �忨���� */
    EVENT_ENTER_MUSIC_UDISK, /* 16 U�̲��� */
    EVENT_RECORD_CPLAY_RCP, /* 17 ��¼���ط�*/
    EVENT_RECORD_UPLAY_RCP, /* 18 U��¼���ط� */
    EVENT_ENTER_CRECORD_RCP, /* 19 ��¼�� */
    EVENT_ENTER_URECORD_RCP, /* 20 U��¼�� */
    EVENT_ENTER_USOUND, /* 21 USB���� */
};

/* COMMON set/get/control������ص�����ע��� */
const rcp_cmd_cb_t com_rcp_cmd_cb_tbl[] =
{
    { RCP_CMD_SETTING, SETTING_ID_SYSTEM_SETTING, (void *) com_set_system_setting }, //ϵͳ����
    { RCP_CMD_SETTING, SETTING_ID_SYSTEM_STATUS, (void *) com_set_system_status }, //APK״̬
    { RCP_CMD_SETTING, SETTING_ID_SYSTEM_DIALOG, (void *) com_set_system_dialog }, //�Ի�����ʾ
    { RCP_CMD_SETTING, SETTING_ID_DEVICE_NAME, (void *) com_set_device_name }, //�����豸����
    { RCP_CMD_SETTING, SETTING_ID_ALARM_ADD, (void *) time_alarm_set_alarm_add }, //��ӻ��������
    { RCP_CMD_SETTING, SETTING_ID_ALARM_DELETE, (void *) time_alarm_set_alarm_delete }, //ɾ������
    { RCP_CMD_SETTING, SETTING_ID_ALARM_PARAM, (void *) time_alarm_set_alarm_param }, //�������Ӳ���
    { RCP_CMD_SETTING, SETTING_ID_OTA_CONNECT_SETTING, (void *) ota_receive_connect_data }, //�������ֲ���
    { RCP_CMD_SETTING, SETTING_ID_OTA_RECIEVE_DATA, (void *) ota_receive_data }, //����FW����
    { RCP_CMD_SETTING, SETTING_ID_OTA_RECIEVE_VRAM_DATA, (void *) ota_receive_vram_data }, //����FW����
    { RCP_CMD_SETTING, SETTING_ID_SEND_COMPLETE, (void *) ota_send_complete }, //��ɷ�������
    { RCP_CMD_SETTING, SETTING_ID_OTA_RESTAT_UD, (void *) ota_restart_to_upgrade }, //������������        
    { RCP_CMD_SETTING, SETTING_ID_OTA_RESTAT_CD, (void *) ota_restart_to_clean_data }, //���������������

    { RCP_CMD_QUERY, QUERY_ID_GLOBAL_RCP_INFO, (void *) com_get_global_rcp_info }, //��ȡȫ��״̬
    { RCP_CMD_QUERY, QUERY_ID_SUPPORT_FEATURE, (void *) com_get_support_feature }, //��ȡ֧�������б�
    { RCP_CMD_QUERY, QUERY_ID_BT_DEVICE_INFO, (void *) com_get_bt_device_info }, //��ȡ�豸��Ϣ���������Ƶ�
    { RCP_CMD_QUERY, QUERY_ID_ALARM_LIST, (void *) time_alarm_get_alarm_list }, //��ȡ�����б�
    { RCP_CMD_QUERY, QUERY_ID_RING_LIST, (void *) time_alarm_get_ring_list }, //��ȡ���������б�
    { RCP_CMD_QUERY, QUERY_ID_RING_FOLDER, (void *) time_alarm_get_ring_folder }, //��ȡ���ش�������Ŀ¼�б�
    { RCP_CMD_QUERY, QUERY_ID_ALARM_PARAM, (void *) time_alarm_get_alarm_param }, //��ȡ���Ӳ���
    { RCP_CMD_QUERY, QUERY_ID_OTA_VER, (void *) ota_get_ver_num }, //��ȡС���汾��
    { RCP_CMD_QUERY, QUERY_ID_OTA_RECEIVE_STATE, (void *) ota_enquiry_message }, //��ȡС���������ݵ�״̬
    { RCP_CMD_QUERY, QUERY_ID_OTA_CONNECT, (void *) ota_enquiry_connect }, //��ȡ����״̬

    { RCP_CMD_CONTROL, CONTROL_KEY_MUTE, (void *) com_ctrl_switch_mute }, //��������
    { RCP_CMD_CONTROL, CONTROL_ALARM_SNOOZE, (void *) alarm_alarm_snooze }, //����SNOOZE
    { RCP_CMD_CONTROL, CONTROL_ALARM_SHUTOFF, (void *) alarm_alarm_close }, //���ӹر�

    /* ������ */
    { 0, 0, NULL },
};

/* ����get/set/control������ص����� */
void * com_rcp_cmd_find_cb(uint8 cmd_type, uint8 cmd_id)
{
    const rcp_cmd_cb_t *this_rcp_cmd_cb;

    if (g_app_rcp_cmd_cb_tbl != NULL)
    {
        this_rcp_cmd_cb = g_app_rcp_cmd_cb_tbl;
        for (;; this_rcp_cmd_cb++)
        {
            if (this_rcp_cmd_cb->type == 0)
            {
                break;
            }

            if ((this_rcp_cmd_cb->type == cmd_type) && (this_rcp_cmd_cb->id == cmd_id))
            {
                return this_rcp_cmd_cb->cb;
            }
        }
    }

    this_rcp_cmd_cb = com_rcp_cmd_cb_tbl;
    for (;; this_rcp_cmd_cb++)
    {
        if (this_rcp_cmd_cb->type == 0)
        {
            break;
        }

        if ((this_rcp_cmd_cb->type == cmd_type) && (this_rcp_cmd_cb->id == cmd_id))
        {
            return this_rcp_cmd_cb->cb;
        }
    }

    return NULL;
}

/* �л�Ӧ������ص�����������Ӧ������֮ǰ���� */
void com_apswitch_hook(void)
{
    if (g_ap_switch_var.apk_alarm_flag == 1)
    {
        set_cur_func_id(g_ap_switch_var.apk_alarm_last_func);
        com_rcp_set_callback((const rcp_cmd_cb_t *) g_ap_switch_var.apk_alarm_backup_cb, 
                (rcp_get_func) g_ap_switch_var.apk_alarm_backup_global_cb);
        g_ap_switch_var.apk_alarm_flag = 0;
    }
}

/* ����ַ�֮ǰ���Ӻ���������RCP����ַ�����ʱ���� */
void __section__(".rcode") com_rcp_cmd_dispatch_before_hook(void)
{
    //�յ�APK�����ʾAPK available
    g_app_info_state.apk_status = APK_FRONTGROUND;
    g_sys_counter.apk_counter = 0;
}

/* ����ַ�֮���Ӻ��������˳�RCP����ַ�����ʱ���� */
#if 0
void __section__(".rcode") com_rcp_cmd_dispatch_after_hook(void)
{

}
#endif
