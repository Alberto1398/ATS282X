/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP ��صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_RCP_H
#define _COMMON_RCP_H

#include "psp_includes.h"
#include "case_independent.h"
#include "case_type.h"
#include "gui_msg.h"
#include "app_msg.h"

/*! Զ�̿������ݰ����޸�ʽ������д rcp */
typedef struct
{
    /*! ���ݰ����� */
    void * rcp_data;
    /*! ���ݰ����� */
    uint16 rcp_len;
    //uint8 reserve[2];
    /*! ���ַ��� */
    uint8 rcp_flag;
    /*! ����Զ���豸�� */
    uint8 rcp_remote;
} rmt_ctrl_pkg_t;

/*! rcp ������غ궨�� */
#define RCP_QUEUE_SIZE      8 //RCP ���ݰ�����
#define RCP_QUEUE_DEPTH     5 //RCP ������ȣ���Ӧ����Ϣ����һ��

//#define RCP_ACT_SERV_FLAG   0
//#define RCP_TIP_SERV_FLAG   1
//#define RCP_ANCS_SERV_FLAG  2
//for weixin service support
#define RCP_WEIXIN_MASK 0x10
#define RCP_WEIXIN_FLAG 0x10
//for client 
#define RCP_ROLE_CLIENT 0x80

/*! service typeö�� */
typedef enum
{
    RCP_ACT_SERV_FLAG = 0,//actions service
    RCP_TIP_SERV_FLAG,    //ble tip service
    RCP_ANCS_SERVN_FLAG,   //apple ancs service notify
    RCP_ANCS_SERVD_FLAG,   //apple ancs service data source
	
	  // ��������
	  RCP_ANCS_CTRL_POINT_FLAG,
    RCP_CTS_SERV_FLAG,   //cts service
    RCP_NDST_SERV_FLAG,   //ndst service
    RCP_BAS_SERV_FLAG,   //bas service 
    RCP_LLS_SERV_FLAG,// 
    RCP_IAS_SERV_FLAG,// 
    RCP_HRS_SERV_FLAG,// 
    RCP_HTS_SERV_FLAG,//     
    RCP_FMS_SERV_FLAG,// 
    RCP_HTS_SERV1_FLAG, 
    RCP_RTUS_SERV_FLAG,   
    RCP_SERVICE_MAX       //invalid service
}rcp_service_type_e;

/*! ������ö�� */
typedef enum
{
    RCP_CMD_NONE = 0, //QAC
    RCP_CMD_APSWITCH = 'P', //0x50
    RCP_CMD_CONTROL = 'C', //0x43
    RCP_CMD_SIMKEY = 'K', //0x4B
    RCP_CMD_QUERY = 'Q', //0x51
    RCP_CMD_ANSWER = 'A', //0x41
    RCP_CMD_SETTING = 'S', //0x53
    RCP_CMD_ACK = 'R', //0x52
} rcp_cmd_type_e;

/*Զ�̿���ͨ������*/
#define RCP_CHAN_MASK            (0x03 << 0)
#define RCP_CHAN_SPP             (0x00 << 0)
#define RCP_CHAN_I2C_SPI_UART    (0x01 << 0)
#define RCP_CHAN_GPIO            (0x02 << 0)
/*Զ�̿������ݴ���ֶα�־*/
#define RCP_SEGMENT_MASK    (0x03 << 2)
#define RCP_SEGMENT_NONE    (0x00 << 2)
#define RCP_SEGMENT_FIRST   (0x01 << 2)
#define RCP_SEGMENT_CONT    (0x02 << 2)
#define RCP_SEGMENT_LAST    (0x03 << 2)
/*Զ�̿������ݴ���ֶ�SEQ*/
#define RCP_SEQ_MASK        (0x0f << 0)

#define RCP_COMMAND_HEAD_SIZE     16 //RCP����ṹ�峤��
#define RCP_COMMAND_MAGIC_OFFSET  0
#define RCP_COMMAND_FLAGS_OFFSET  2  //FLAGS��Ա��RCP����ṹ���е�ƫ�Ƶ�ַ
#define RCP_COMMAND_LEN_OFFSET    6  //����ȳ�Ա��RCP����ṹ���е�ƫ�Ƶ�ַ

//��ȡ�����
#define COMMAND_LENGTH_HIGH(buf)  ((uint16)(*((uint8*)(buf) + RCP_COMMAND_LEN_OFFSET + 1)) << 8)
#define COMMAND_LENGTH_LOW(buf)   (*((uint8*)(buf) + RCP_COMMAND_LEN_OFFSET))
#define COMMAND_LENGTH(buf)       (COMMAND_LENGTH_HIGH(buf) + COMMAND_LENGTH_LOW(buf))
//��ȡFLAGS
#define COMMAND_FLAGS(buf,n)      (*((uint8*)(buf) + RCP_COMMAND_FLAGS_OFFSET + (n)))
//��ȡMAGIC
#define COMMAND_MAGIC_SOH(buf)    (*((uint8*)(buf) + RCP_COMMAND_MAGIC_OFFSET))
#define COMMAND_MAGIC_SOH_R(buf)  (*((uint8*)(buf) + RCP_COMMAND_MAGIC_OFFSET + 1))

#define MAX_SEGMENT_LEN     0x20 //I2CӦ������������ݰ�����

#define PHY_REPLY_HEADER    0  //Ӧ������ͷ

#define PHY_REPLY_DATA      1  //Ӧ�����ݶ�

//�Ի�����ϢID
#define DIALOG_MSG_USB_STICK    0x00  //USB�߲���ʱѡ�����绹�ǽ���USB����

//APK�����̨ģʽ��ʱʱ��
#define APK_BACKGROND_OVERTIME  6     //3����û�յ�APK�������ʾAPK�����̨ģʽ
//APK/APP��֤��ʱʱ��
#define APK_APP_VERIFICATION_OVERTIME  10 //���ӳɹ���5����û���յ���֤�������Ϊ�ǷǷ�����

//����֧�����Ի���BITMAP
//low 32bit
#define SUPPORT_FEATURE_A2DP_PLAY     (1 << 0)  //A2DP�����Ƹ�
#define SUPPORT_FEATURE_SDCARD_PLAY   (1 << 1)  //���ز忨����
#define SUPPORT_FEATURE_UHOST_PLAY    (1 << 2)  //����U�̲���
#define SUPPORT_FEATURE_RECORD_PLAY   (1 << 3)  //¼���ط�
#define SUPPORT_FEATURE_FOLDER_PLAY   (1 << 4)  //�Զ���Ŀ¼��ݲ���
#define SUPPORT_FEATURE_FMRADIO       (1 << 5)  //FM������
#define SUPPORT_FEATURE_LINEIN_PLAY   (1 << 6)  //AUX����
#define SUPPORT_FEATURE_RECORD        (1 << 7)  //MIC¼��
#define SUPPORT_FEATURE_REC_FMRADIO   (1 << 8)  //FM������¼��
#define SUPPORT_FEATURE_REC_LINEIN    (1 << 9)  //AUX����¼��
#define SUPPORT_FEATURE_ALARM_CLOCK   (1 << 10) //����
#define SUPPORT_FEATURE_ALARM_BUILDIN_RING (1 << 11) //��������֧����������
#define SUPPORT_FEATURE_ALARM_RING_FOLDER  (1 << 12) //��������֧���Զ�������Ŀ¼
#define SUPPORT_FEATURE_ALARM_PLAYLIST     (1 << 13) //��������֧�ֲ����б�
#define SUPPORT_FEATURE_ALARM_FMRADIO      (1 << 14) //��ʱFM������
#define SUPPORT_FEATURE_ALARM_REC_FMRADIO  (1 << 15) //��ʱFM������¼��������֧��FM������¼��
#define SUPPORT_FEATURE_USBBOX_PLAY        (1 << 16) //USB���䲥��
#define SUPPORT_FEATURE_OTA        (1 << 17)         //OTA����


#define SUPPORT_FEATURE_PLIST_FILE    (1 << 24) //֧���ļ����б�
#define SUPPORT_FEATURE_PLIST_TITLE   (1 << 25) //֧�ֱ����б�
#define SUPPORT_FEATURE_PLIST_ARTIST  (1 << 26) //֧���ݳ����б�
#define SUPPORT_FEATURE_PLIST_ALBUM   (1 << 27) //֧��ר���б�
#define SUPPORT_FEATURE_PLIST_GENRE   (1 << 28) //֧�ַ���б�

/*!
 * \brief
 *  dialog_type_e �Ի�������ö������
 */
typedef enum
{
    /*! ��Ч�Ի������� */
    DIALOG_TYPE_NONE = 0x00,
    /*! ���а���ѡ���"ѯ�ʶԻ���" */
    DIALOG_BUTTON_INCLUDE = 0x01,
    /*! ����������"��ʾ��Ϣ�Ի���"�����������ⰴ���˳� */
    DIALOG_BUTTON_ANYKEY = 0x02,
    /*! ����������"��ʾ��Ϣ�Ի���"��������ȴ�2���Ӻ��˳� */
    DIALOG_INFOR_WAIT = 0x03,
    /*! ����������"��ʾ��Ϣ�Ի���"�������������˳� */
    DIALOG_INFOR_IMMEDIATE = 0x04,
    /*! ȡ����ǰ�Ի��� */
    DIALOG_TYPE_CANCEL = 0x05,
} dialog_type_e;

/*!
 * \brief
 *  button_type_e �Ի��򰴼��������ö������
 */
typedef enum
{
    /*! ȷ����ȡ�� */
    BUTTON_OKCANCEl = 0x00,
    /*! ��ֹ�����Ժͺ��� */
    BUTTON_ABORTRETRYIGNORE = 0x01,
    /*! �ǡ����ȡ�� */
    BUTTON_YESNOCANCEL = 0x02,
    /*! �Ǻͷ� */
    BUTTON_YESNO = 0x03,
    /*! ���Ժ�ȡ�� */
    BUTTON_RETRYCANCEL = 0x04,
    /*�������Զ��尴ť���*/
    /*! ���/USB���� */
    BUTTON_USBSOUND_CHARGE = 0x05,
    /*! û�а�ť */
    BUTTON_NO_BUTTON = 0x06,
} button_type_e;

/*! �Ի���״̬�� */
typedef enum
{
    /*! ���ڿ���״̬ */
    DIALOG_STATE_NONE,
    /*! С������Ի������󣬵ȴ�APKӦ�� */
    DIALOG_STATE_LAUNCH,
    /*! С���յ�APKӦ�𣬵ȴ�APK�Ի���ѡ�� */
    DIALOG_STATE_CHOOSING,
    /*! С���յ�APK�Ի���ѡ��ȷ��ѡ��� */
    DIALOG_STATE_CONFIRM,
    /*! APKû����ѡ���Զ��˳� */
    DIALOG_STATE_CANCEL_AUTO,
} dialog_state_e;

/*! ����������ص����� */
typedef app_result_e (*rcp_ctrl_func)(uint32 data1, uint32 data2);
/*! ��ѯ������ص����� */
typedef app_result_e (*rcp_get_func)(uint32 data1, uint32 data2);
/*! ����������ص����� */
typedef app_result_e (*rcp_set_func)(uint32 data1, uint32 data2, void *data, uint16 data_len);

/*! ��չ������ص����� */
typedef app_result_e (*rcp_ext_serv_func)(void *data, uint16 data_len, uint8 serv_flag);

/*set/get/control������ص�����ע���*/
typedef struct
{
    rcp_cmd_type_e type;
    uint8 id;
    void * cb;
} rcp_cmd_cb_t;

//get������ID��
#define QUERY_ID_GLOBAL_RCP_INFO  0x00  //��ȡȫ��״̬
/* Ӧ�����Ϊ��0, 0, com_rcp_info_t + XXX_rcp_info_t ����ap�Զ��壬���Բ�Ҫ��*/
#define QUERY_ID_DISK_CAPACITY    0x01  //��ȡ���δ�������
/* Ӧ�����Ϊ��available capacity, total capacity, NULL */
#define QUERY_ID_SUPPORT_FEATURE  0x02  //��ȡ֧�����Ի���
/* Ӧ�����Ϊ��support feature bitmap low 32bit, support feature bitmap high 32bit, NULL */
#define QUERY_ID_MUSIC_FOLDER     0x03  //��ȡ���ش�������Ŀ¼�б�
/* Ӧ�����Ϊ��music folder count, 0, music_folder_infor_t * music folder count */
#define QUERY_ID_BT_DEVICE_INFO   0x80  //�û���չ���������ȡ�����豸��Ϣ���������Ƶ�
/* Ӧ�����Ϊ��0, 0, bt_device_info_t */

#define QUERY_ID_MUSIC_INFO       0x10  //��ȡ�������ֵ�ǰ�����ļ�����ID3��Ϣ
/* Ӧ�����Ϊ��file seq, 0, music_rcp_id3_info_t */
#define QUERY_ID_MUSIC_LYRIC      0x11  //��ȡ�������ֵ�ǰ��������ļ�
/* Ӧ�����Ϊ��file seq, 0, lyric file ���ֶδ��䣬ÿ��512�ֽڣ�*/
#define QUERY_ID_MUSIC_PLIST      0x12  //��ȡָ����ű������ֵĲ����б���Ϣ
/* �������Ϊ��file seq, file count, NULL */
/* Ӧ�����Ϊ��file count, 0, music_rcp_plist_info_t */
#define QUERY_ID_MUSIC_STATUS    0x13   //��ȡ�������ֵ�ǰĿ¼��״̬��Ϣ
/* Ӧ�����Ϊ��dir_status, 0, random */
#define QUERY_ID_MUSIC_DLIST       0x14  //��ȡָ����ű���Ŀ¼���б���Ϣ
/* �������Ϊ��dir seq, dir count, NULL */
/* Ӧ�����Ϊ��dir seq, dir count, music_rcp_dlist_info_t */

#define QUERY_ID_STATION_LIST     0x20  //��ȡ��������̨�б�
/* Ӧ�����Ϊ��freq count, 0, Ƶ��uint32 * freq count*/
#define QUERY_ID_STATION_NAME     0x21  //��ȡ��ǰ��̨����
/* Ӧ�����Ϊ��0, 0, 64�ֽ����� */

#define QUERY_ID_ALARM_LIST       0x30  //��ȡ���Ӽ�¼�б�
/* Ӧ�����Ϊ��alarm count, 0, alarm_alarm_t * alarm count */
#define QUERY_ID_RING_LIST        0x31  //��ȡ�����б�
/* �������Ϊ��ring type, 0, NULL ��ring type -1��ʾ��ȡȫ�����壬����ȫ��������Щ���������*/
/* Ӧ�����Ϊ��ring count, 0, ring_song_t * ring count */
#define QUERY_ID_RING_FOLDER      0x32  //��ȡ���ش�������Ŀ¼�����ͣ��б�
/* Ӧ�����Ϊ��ring folder count, 0, ring_folder_infor_t * ring folder count */
#define QUERY_ID_ALARM_PARAM      0x33  //��ȡ���Ӳ���������snoozeʱ��ʹ�������ʱʱ���
/* Ӧ�����Ϊ��0, 0, alarm_param_t */
#define QUERY_ID_OTA_VER      0x70  //��ȡFW�汾��
/* Ӧ�����Ϊ�� */
#define QUERY_ID_OTA_RECEIVE_STATE      0x71  //��ȡС�������ݵ�״̬
/* Ӧ�����Ϊ�� */
#define QUERY_ID_OTA_CONNECT      0x72  //��ȡС������



//set������ID��
//NOTE!!! �û������������䣺[0x80-0xbf] ��64��ID������ID�����������갸��չ
#define SETTING_ID_SYSTEM_SETTING 0x00  //ϵͳ����
#define SETTING_ID_SS_DATETIME    0x00  //����ʱ��
#define SETTING_ID_SS_VOLUME      0x01  //����
#define SETTING_ID_SS_EQ          0x02  //EQģʽ
#define SETTING_ID_SS_USREQ       0x03  //�û�EQ
#define SETTING_ID_SS_DAEMODE     0x04  //DAEģʽ
/* �������Ϊ��setting id, value, data
 ��1�� �������ں�ʱ�䣺0, 0, system_datetime_t
 ��2�� ����������1, volume, NULL
 ��3�� ����EQģʽ��2, eq type, NULL
 ��4�� �����û�EQ��3, 0, uint8 eq_param[7] + uint8 reserve[1]
 ��5�� ����DAEģʽ��4, 0, daemode_param_t
 */
#define SETTING_ID_SYSTEM_STATUS  0x01  //APK״̬
/* �������Ϊ��apk_status_e, 0, NULL */
#define SETTING_ID_SYSTEM_DIALOG  0x02  //�Ի�����ʾ����
#define SETTING_ID_SD_ANSWER      0x00  //��Ӧ����
#define SETTING_ID_SD_CONFIRM     0x01  //�Ի���ȷ��
#define SETTING_ID_SD_FAIL        0x02  //ȷ��ȡ��
/* �������Ϊ��control id, value data
 ��1����Ӧ����0, overtime/-1 for error, NULL
 ��2���Ի��򷵻أ�1, button id, NULL
 ��3��ȷ��ȡ����2, 0, NULL
 */
#define SETTING_ID_DEVICE_NAME    0x80  //�û���չ������������豸����
/* �������Ϊ��0, 0, bt_device_name_t */

#define SETTING_ID_MUSIC_SETTING  0x10  //���ֲ�������
/* �������Ϊ��setting id, value, data 
 ��1�� ����ѭ��ģʽ��0, loop mode, NULL
 */
#define SETTING_ID_SONG_SEQ       0x11  //��Ŀ�ŵ㲥
/* �������Ϊ��seq, 0, 0 */
#define SETTING_ID_MUSIC_PLIST    0x12  //�����б����ļ���ű�
/* �������Ϊ��0, 0, file index array NOTE!!! ���ȱ���4�ֽڶ��룬�粻����Ҫ������ */
#define SETTING_ID_MUSIC_DISK     0x13  //�л�����
/* �������Ϊ��1 for card/2 for udisk, 0, NULL */

#define SETTING_ID_STATION_BAND   0x20  //���õ�̨Ƶ��
/* �������Ϊ��band, 0, NULL */
#define SETTING_ID_STATION_FREQ   0x21  //���õ�̨Ƶ��
/* �������Ϊ��freq, 0, NULL */

#define SETTING_ID_ALARM_ADD      0x30  //��ӻ�������Ӽ�¼
/* �������Ϊ��0, 0, alarm_alarm_t*/
#define SETTING_ID_ALARM_DELETE   0x31  //ɾ�����Ӽ�¼
/* �������Ϊ��alarm id��-1��ʾ��գ�, 0, NULL */
#define SETTING_ID_ALARM_PARAM    0x32  //�������Ӳ���������snoozeʱ��ʹ�������ʱʱ���
/* �������Ϊ��0, 0, alarm_param_t */

#define SETTING_ID_AUX_SETTING    0x40  //LINEIN��������
/* �������Ϊ��setting id, value, data 
 �ݲ�ʹ��*/
#define SETTING_ID_OTA_CONNECT_SETTING    0x70  //OTA����(��������)
/* �������Ϊ��  */
#define SETTING_ID_OTA_RECIEVE_DATA    0x71  //OTA FW���ݽ���
/* �������Ϊ��  */
#define SETTING_ID_OTA_RESTAT_UD    0x72  //������������
/* �������Ϊ��  */
#define SETTING_ID_OTA_RESTAT_CD    0x73  //������ձ�����
/* �������Ϊ��  */
#define SETTING_ID_SEND_COMPLETE          0x74  //��ɷ�������
/* �������Ϊ��  */
#define SETTING_ID_OTA_RECIEVE_VRAM_DATA  0x75  //OTA VRAM���ݽ���


/*ACK������ID*/
#define RCP_ACK_ID_ACK            0x00  //ACK Ӧ��Ĭ�������OK���Զ� ACK Ӧ��
#define RCP_ACK_ID_NACK           0x01  //NACKӦ��

#define CONTROL_KEY_PLAY        (0x0b)
#define CONTROL_KEY_PAUSE       (0x0c)
#define CONTROL_KEY_NEXT        (0x0d)
#define CONTROL_KEY_PREV        (0x0e)
#define CONTROL_KEY_DISMUTE     (0x0f)
#define CONTROL_KEY_VADD        (0x10)
#define CONTROL_KEY_VSUB        (0x11)
#define CONTROL_KEY_MUTE        (0x12)
#define CONTROL_KEY_STOP        (0x18)
#define CONTROL_FM_SEARCH       (0x23)
#define CONTROL_FM_STOP_SCH     (0x24)
#define CONTROL_FM_CHADD        (0x25)
#define CONTROL_FM_CHSUB        (0x26)
#define CONTROL_FM_MUTE         (0x27)
#define CONTROL_BT_DISCON       (0x28)
#define CONTROL_BT_ACCEPT       (0x29)
#define CONTROL_BT_HANG_UP      (0x2a)
#define CONTROL_BT_CALL_LAST    (0x2b)
#define CONTROL_ALARM_SNOOZE    (0x2c)
#define CONTROL_ALARM_SHUTOFF   (0x2d)

#if (SUPPORT_SLAVE_CONTROL == 1)
/*COMMONȫ��״̬�ṹ�壬Ԥ����4�ֽ�*/
typedef struct
{
    /*! RESERVE */
    uint8 reserve_bytes[4];
}com_rcp_info_t;
#else //SUPPORT_SLAVE_CONTROL
/*COMMONȫ��״̬�ṹ�壬Ԥ����24�ֽ�*/
typedef struct
{
    /*! ��ǰӦ�����ͣ���ʱ��APP ID���� */
    uint8 app_type;
    /*! ������� */
    uint8 max_volume;
    /*! ��С���� */
    uint8 min_volume;
    /*! ��ǰ���� */
    uint8 cur_volume;
    /*! ��ǰEQģʽ*/
    uint8 cur_eqtype;
    /*! �Ƿ���״̬ */
    uint8 mute_flag;
    /*! ��ǰ���״̬���޳�磬����У������磬�޵�� */
    uint8 battery_state;
    /*! ��ǰ��ص�����0-4����� */
    uint8 battery_value;
    /*! ��ǰSD��״̬���޲忨���忨�������� */
    uint8 sdcard_in;
    /*! ��ǰUHOST״̬����UHOST����UHOST��UHOST���� */
    uint8 uhost_in;
    /*! �Ƿ�����USB���£�û�����ӣ�����USB�ߣ����ӳ�������� */
    uint8 usb_in;
    /*! �Ƿ�����LINEIN�� */
    uint8 linein_in;
    /*! �Ƿ����Ӷ�����*/
    uint8 hp_in;
    /*! �Ƿ�������ã�����Ӧ�û��Ƚ������ã����֮��ű�֤Ӧ��״̬��ȷ�ԣ�
     ����SDCARD���裬Ҫ�Ƚ���Ŀ¼ɨ��� */
    uint8 config_flag;
    /*! �Ƿ����������� */
    uint8 alarm_clock;
    /*! ��Ӧ���������ʱ�����ݸ�APK��Ӧ����ز�������������Ӧ�ÿ����ɸò��������Ƿ���Ҫ�����б� */
    uint8 app_argv;
    /*! �Ի������ͣ��μ� dialog_type_e ���� */
    uint8 dialog_type;
    /*! �Ի��򰴼�������ͣ��μ� button_type_e ���� */
    uint8 dialog_button_type;
    /*! �Ի���������ϢID */
    uint8 dialog_desc_id;
    /*! Ĭ�ϼ������ */
    uint8 dialog_active_default;
    /*! ��ǰ������Чģʽ��0��ʾ��������Ч��1��ʾEQ��Ч��2��ʾDAE��Ч */
    uint8 dae_mode;
    /*! DAE��Чѡ�bit 0��ʾVBASS���أ�bit 1��ʾtreble���أ�bit 2��ʾ���⻷�ƿ��� */
    uint8 dae_option;
    /*! RESERVE */
    uint8 reserve_bytes[2];
} com_rcp_info_t;
#endif //SUPPORT_SLAVE_CONTROL

/*��ǰϵͳ����ʱ��*/
typedef struct
{
    /*! ��ǰϵͳ���� */
    date_t date;
    /*! ��ǰϵͳʱ�� */
    time_t time;
    uint8 reserve;
} system_datetime_t;

/*DAE mode�����ṹ��*/
typedef struct
{
    /*! ��ǰ������Чģʽ */
    uint8 dae_mode;
    /*! ���� */
    union
    {
        /*! DAE��Чѡ�� */
        uint8 dae_option;
        /*! EQ��Ч���� */
        uint8 cur_eqtype;
    } param;
    /*! �����GEQ����ô����ҪGEQ������ */
    uint8 eq_para[7];
} daemode_param_t;

//RCP��������ӿ�
typedef enum
{
    /*!��ЧRCP��*/
    RCP_STATE_INVALID,
    /*!��ǰRCP���Ѿ���������������*/
    RCP_STATE_OK,
    /*!��ǰRCP������������ͷ��*/
    RCP_STATE_HEAD,
} rcp_state_e;
extern rcp_state_e com_check_valid_rcp(uint8 *rcp_head, uint16 rcp_len) __FAR__;
extern app_result_e com_rcp_dispatch(rmt_ctrl_pkg_t *p_rcp) __FAR__;
extern void com_rcp_state_init(void) __FAR__;
extern void com_rcp_init(void) __FAR__;
extern void com_rcp_set_callback(const rcp_cmd_cb_t * app_rcp_cmd_cb_tbl, rcp_get_func app_get_global_info_cb) __FAR__;
extern bool com_rcp_check_for_tts(rmt_ctrl_pkg_t *p_rcp) __FAR__;
extern app_result_e com_rcp_cmd_dispatch(rmt_ctrl_pkg_t *p_rcp) __FAR__;
extern void com_rcp_send_pkg(uint32 data1, uint32 data2, void *data, uint16 data_len, uint16 data_total) __FAR__;
extern void com_rcp_send_pkg_cont(void *data, uint16 data_len) __FAR__;
extern void com_rcp_send_ack_pkg(uint8 ack) __FAR__;
extern void com_rcp_send_cmd_pkg(uint8 cmd_id, uint8 cmd_type,uint8 dev_id) __FAR__;
extern void com_get_com_info(com_rcp_info_t *p_com_rcp_info) __FAR__;
extern app_result_e com_get_global_rcp_info(uint32 data1, uint32 data2) __FAR__;
extern app_result_e com_get_bt_device_info(uint32 data1, uint32 data2) __FAR__;
extern app_result_e com_get_support_feature(uint32 data1, uint32 data2) __FAR__;
extern app_result_e com_set_system_setting(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e com_set_system_status(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e com_set_system_dialog(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
typedef bool (*dialog_check_callback)(void);
extern app_result_e com_rcp_dialog(uint8 type, uint8 button_type, uint8 desc_id, uint8 active_default,
        dialog_check_callback check_cb) __FAR__;
extern bool com_dialog_check_usbdialog(void) __FAR__;
extern app_result_e com_set_device_name(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;

extern app_result_e com_ctrl_switch_mute(uint32 data1, uint32 data2) __FAR__;

extern app_result_e time_alarm_get_alarm_list(uint32 data1, uint32 data2) __FAR__;
extern app_result_e time_alarm_get_ring_list(uint32 data1, uint32 data2) __FAR__;
extern app_result_e time_alarm_get_ring_folder(uint32 data1, uint32 data2) __FAR__;
extern app_result_e time_alarm_get_alarm_param(uint32 data1, uint32 data2) __FAR__;
extern app_result_e time_alarm_set_alarm_add(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e time_alarm_set_alarm_delete(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e time_alarm_set_alarm_param(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e alarm_alarm_snooze(uint32 data1, uint32 data2) __FAR__;
extern app_result_e alarm_alarm_close(uint32 data1, uint32 data2)__FAR__;
extern bool g_rcp_ctrl_quit_flag;

//rcp dialog
extern uint8 g_dialog_state;
extern uint8 g_dialog_overtime_flag;
extern uint8 g_dialog_overtime;
extern uint8 g_dialog_choosed;
extern uint8 g_dialog_type;
extern uint8 g_dialog_button_type;
extern uint8 g_dialog_desc_id;
extern uint8 g_dialog_active_default;
//����rcp�������ͱ���
extern uint8 g_rcp_service_type;
//����rcpԶ���豸�ű���
extern uint8 g_rcp_remote_number;

extern const rcp_cmd_cb_t *g_app_rcp_cmd_cb_tbl;
extern rcp_get_func g_app_get_global_info;

extern rcp_ext_serv_func g_app_ext_serv_cb[]; //��չ������Ӧ����

extern const msg_apps_type_e rcp_cmd_apswitch_event[];
extern const key_value_e rcp_cmd_simkey_key[];
extern void* com_rcp_cmd_find_cb(uint8 cmd_type, uint8 cmd_id) __FAR__;
extern void com_apswitch_hook(void) __FAR__;
extern void com_rcp_cmd_dispatch_before_hook(void) __FAR__;
extern void com_rcp_cmd_dispatch_after_hook(void) __FAR__;

extern uint8 g_rcp_cmd_flags[2]; //SPP�����־��
extern uint8 g_rcp_cmd_id; //��ǰ���ڴ�������ID
extern uint8 g_rcp_next_seq; //��һ�ηֶκϷ�SEQ
extern bool g_rcp_seg_flag; //RCP�ֶα�־
extern uint8 *g_rcp_long_buffer; //����RCP������������256�ֽڶ����ڳ���RCP
extern uint16 g_rcp_long_length; //����RCP����
extern uint16 g_rcp_long_index; //����RCP������INDEX
extern bool g_rcp_long_buffer_flag; //���ڻ�������RCP��־
extern uint16 g_rcp_pkg_max_len; //RCP��󳤶ȣ������ڻ�������С

#define RCP_PKG_MAX_PAYLOAD          (520) //RCP�����Ϊ520�ֽڣ�������data���д��512���ݼ�8���ֽڵ�����
#define RCP_LONG_COMMAND_BUFFER_LEN  (RCP_PKG_MAX_PAYLOAD + RCP_COMMAND_HEAD_SIZE) //���Ϊ 0x220 �ֽ�
extern uint8 *g_rcp_default_long_buffer;

#endif
