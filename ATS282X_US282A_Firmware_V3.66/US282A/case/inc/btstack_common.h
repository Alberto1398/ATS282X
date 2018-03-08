/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Э��ջ��صĺ궨�塢ö�����͡��ṹ�嶨��ȣ���ͷ�ļ�Ҫ�����޸ģ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __BTSTACK_COMMON_H__
#define __BTSTACK_COMMON_H__

#include "psp_includes.h"
#include "case_independent.h"
#include "mmm_bp.h"
#include "case_type.h"
#include "app_msg.h"
#include "config_id.h"
#include "vm_def.h"
#include "applib.h"
#include "bt_controller_interface.h"

#define WORK_MODE_NORMLA_MASK    0x01
#define WORK_NORNAL              0x01
#define WORK_AUTOTEST            0x00

#define WORK_MEM_NORMAL_MASK     0x02
#define WORK_MEM_NORMAL          0x02
#define WORK_MEM_SMALL           0x00

// !!! �� Makefile ���� gcc �ĺ궨��
#ifndef OTA_VERSION
// !!! OTA�汾���ܴ�
#define ENABLE_TRUE_WIRELESS_STEREO
#endif

#define RMT_DEV_NUM 2

#define MGR_MAX_REMOTE_NAME_LEN         16
#define BD_ADDR_LEN                     6
#define PAIRED_LIST_MAX                 8

#define AUTO_CONNECT_TYPE_STARTUP       0
#define AUTO_CONNECT_TYPE_MODE          1
#define AUTO_CONNECT_TYPE_TIMEOUT       2

#define HFP_SUPPORTED           0x01
#define A2DP_SUPPORTED          0x02
#define AVRCP_SUPPORTED         0x04
#define HID_SUPPORTED           0x08
#define PROFILE_VALID           0x80
#define LINKKEY_VALID           0x40

//serv_conn
#define HFP_CONNECTED           0x01
#define A2DP_CONNECTED          0x02
#define AVRCP_CONNECTED_MY      0x04//��avrcp.h��t_AVRCP_STATE��������ΪAVRCP_CONNECTED_MY
#define SPP_CONNECTED           0x08
#define BLE_CONNECTED           0x10
#define HID_CONNECTED           0x20
#define HFP_DISCONNECTED        0xFE
#define A2DP_DISCONNECTED       0xFD
#define AVRCP_DISCONNECTED      0xFB
#define SPP_DISCONNECTED        0xF7
#define BLE_DISCONNECTED        0xEF
#define HID_DISCONNECTED        0xdf


#define ANCS_TYPE_NONE			0
#define ANCS_TYPE_NOTIFICATION	1
#define ANCS_TYPE_DATA			2

typedef enum
{
    THREE_WAY_HOLD_NOT_EXIST,
    THREE_WAY_NEW_CALL_IN,
    THREE_WAY_HOLD_EXIST,
}bt_3way_status;


typedef struct
{
    uint8 bytes[BD_ADDR_LEN];
} t_bdaddr;

//COMMON��������ģ������������Э��ջʱ���ݲ�����

typedef enum
{
    PB_RESULT_NULL = 0, 
    PB_RESULT_NAME,
    PB_RESULT_NUM,
} pb_result_e;

#define PB_MAX_NUM_LEN  16
#define PB_MAX_NAME_LEN 16
//phone information
typedef struct
{
    uint8 phonenum[PB_MAX_NUM_LEN];
    uint8 name[PB_MAX_NAME_LEN];
} pb_data_t; //32byte

#ifdef __SUPPORT_3_WAY_CALL_
/* Used By +CLCC */
struct HFP_CLCCInfo {
    uint8   idx;                /* The numbering (start with 1) of the call given by the sequence of setting up or receiving the calls */
    uint8   dir;                /* Direction, 0=outgoing, 1=incoming */
    uint8   status;             /* 0=active, 1=held, 2=dialling(outgoing), 3=alerting(outgoing), 4=incoming(incoming), 5=waiting(incoming) */
    uint8   mode;               /* 0=voice, 1=data, 2=fax */
    uint8   mpty;               /* 0=not multiparty, 1=multiparty */
    uint8   type;               /* The format of the phone number provided */
    uint8   num_len;            /* the length of the phone number provided, not including ending 0 */   
    uint8   number[1];          /* Endby 0, Phone number */
};
#endif  //__SUPPORT_3_WAY_CALL_

typedef struct
{
    uint32 enable_a2dp :1;
    uint32 enable_a2dp_cp :2;
    uint32 enable_avdtp_aac :1;
    uint32 enable_avrcp :1;
    uint32 enable_avrcp_14 :1;
    uint32 enable_avrcp_volume_sync :1;
    uint32 enable_hfp :1;
    uint32 enable_hfp_volume_sync :1;
    uint32 enable_hfp_bat_report :1;
    uint32 enable_hfp_phonebook :1;
    uint32 enable_spp :1;
    uint32 enable_ble :1;
    uint32 enable_hid :1;
    uint32 enable_3way_call :1;
    uint32 enable_ancs :1;
    uint32 enable_tip :1;
    uint32 enable_siri :1;
    uint32 enable_msbc : 1;
#ifdef __SUPPORT_3_WAY_CALL_    	
    struct HFP_CLCCInfo hfp_clcc_info[2]; //add by Ian,15.04.16, for three_way_status.
#endif     
} btstack_config_t;

typedef enum
{
    BT_ERR_NONE,
    BT_ERR_HW,
    BT_ERR_PAGE_TIMEOUT,
    BT_ERR_CONNECTION_TIMEOUT,
    BT_ERR_LINKEY_MISS, //�Է�ȡ�����
    BT_ERR_NOT_POPUP_WINDOW //�Է����뵯������    
} bt_err_e;

typedef enum
{
    CONN_STATUS_NONE,
    CONN_STATUS_WAIT_PAIR,
    CONN_STATUS_PAIRED,
    CONN_STATUS_LINKED,
    CONN_STATUS_ERROR,
} conn_status_e;

typedef enum
{
    A2DP_STATUS_NONE,
    A2DP_STATUS_LINKED,
    A2DP_STATUS_PLAY,
    A2DP_STATUS_PAUSE,
} a2dp_status_e;

typedef enum
{
    HFP_STATUS_NONE,
    HFP_STATUS_LINKED,
    HFP_STATUS_INCOMING,
    HFP_STATUS_OUTGOING,
    HFP_STATUS_CALLING,
    HFP_STATUS_PHONE,
	HFP_STATUS_SIRI,
} hfp_status_e;

typedef enum
{
	  DEV_TYPE_NONE,
	  NO_TWS_DEV,
	  TWS_DEV,	
} dev_type_e;


typedef enum
{
	  NORMAL_DEV,
	  TWS_MASTER,
	  TWS_SLAVE,	
} dev_role_e;

//����Э��ջ���⣬ֻ��Ҫ��֪������Ϣ��
//1����ǰ�豸���ӵķ���
//2��ÿ�ַ�����ҵ����ص����ԣ�
//3��ÿ�ַ���ǰ״̬�����ֻ����������״̬����ô�������������״̬��Ϣ��
typedef struct
{
    //uint8 conn_status;
    uint8 serv_conn;
    t_bdaddr addr;
    struct
    {
        uint8 media_type;
        uint8 play_status_flag :1; //1:֧��״̬�仯֪ͨ��0:��֧�֣���������IPhone�������ֻ�
        uint8 avrcp_pause_flag :1; //1:�յ�PAUSE����Ѵ�����Ҫ������������ͣ�����ҹ��˵�3�������ڵ�β��  
        uint8 avdtp_data_flag :1; //1:���200ms���յ�AVDTP��Ƶ���ݰ�
    } a2dp_attr;
    struct
    {
        uint8 sco_link :1; //0, δ����SCO��·��1,�ѽ���SCO��·
        uint8 batt_supported :1; //0, ��֧�ֵ�ص����ϱ���1,֧��
        uint8 pb_result :2; //��ǰ����绰���������ͣ��� pb_result_e ����
#ifdef __SUPPORT_3_WAY_CALL_
        bt_3way_status three_way_status : 2;   // 3��ͨ��״̬ 0��û��3��ͨ����1������3��ͨ��
#endif    
        uint8 old_sco_link :1; //�������SCO�ı仯
        uint16 air_pkt_len;	
    } hfp_attr;
    uint8 a2dp_status;
    uint8 hfp_status;
    uint8 sniff_status; //0��not sniff, 1:sniff
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    /*! ��Ҫ���������Ƕ����豸,NO_TWS_DEV��TWS_DEV
        ��ʵ�����ϲ��ȡֱ�ӻ�ȡ�������Ƚ�,
     *  ����Ҫ��Ϊʡrcode��rdata����������*/
    uint8 dev_type;
#endif    
} bt_stack_sub_info_t;

typedef struct
{
    uint8 conn_status;
    bt_err_e err_status;
    uint8 controller_status;
    uint8 num_connected;
    bt_stack_sub_info_t rmt_dev[RMT_DEV_NUM];
    uint8 a2dp_active_id;
    uint8 hfp_active_id;
    uint8 hfp_established;//���ڱ�־HFP�������
    uint8 dev_discon_lock;//���ڸ����ӳɹ���һ���������һ��ʱ�䣬���ʱ���ڲ����������Ͽ�����
    uint8 spp_con_flag;//0 no con;1 spp con;
    uint8 ble_con_flag;//0 no con;1 ble con;    
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    uint8 dev_role; //������������,����
    uint8 pair;//���������Ƿ�������״̬
    uint8 phonetomastercon;//������־������ֻ�����ʱ��Ĳ���
    uint8 phonetomasterdis;//������־������ֻ��Ͽ����ӵĲ���
    uint8 tws_hfp_con_flag;
    uint8 tws_dev_discon;//����Ͽ�������·�ı�־   ��������tts���������Ͽ�
    uint8 play_and_pause;//��ɫת����־
    uint8 tws_master_phonecon;//���ڱ�ʶtws�豸�Ƿ���ֻ��Ѿ�������
    uint8 sd_lis_ov;
    uint8 sim_a_en;//pmu �������ģʽʹ��
    uint8 dev_disconnecting;
    uint8 tws_pair_status;
#endif
    uint8 esd_reset_fla;
    
    //for wsb support
    uint8 msbc_code_flag;
} bt_stack_info_t;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
//��ʱ���Թ�������

typedef struct
{
    uint8 cmd_pkg[6];
  //  struct Node *pNext;        //    ָ����һ���ڵ��ָ��
}Node;

#endif


extern bt_stack_info_t g_bt_stack_cur_info;

extern bt_stack_pipe_t *g_p_rcp_in_data_ctl;

/* Codec Type */
#define A2DP_CODEC_SBC              0x00/* now only support sbc codec. 2011.11.2 */
#define A2DP_CODEC_MPEG12           0x01
#define A2DP_CODEC_AAC              0x02
#define A2DP_CODEC_ATRAC            0x04
#define A2DP_CODEC_NONA2DP          0xFF

#define PIPE_OP_FILTER        0x01 //���ù��˱�־
#define PIPE_OP_UNFILTER      0x02 //������˱�־
#define PIPE_OP_CLEAR_PAUSE   0x40 //�������յ�AVRCP PAUSE�����3���ӣ������յ���Ҫ�����־
#define PIPE_OP_FLUSH         0x80 //���������
#define PIPE_OP_CLEAR_LIST    0x10 //���sendlist
#define PIPE_OP_PLAY 0x20 //�����ײ�̬Ϊplay
#define PIPE_OP_PAUSE 0x04 //�����ײ�״̬pause
#define PIPE_OP_MMM_FILTER 0x08 //��������м���м�Ķ������

//tws ͨ�������
#define VOLUME_VALUE_FLAG  0x00  //����ֵ
#define MUTE_STATE_FLAG         0x03  //mute״̬
#define DISCON_TWS_FLAG         0x04  //discon tws acl handle flag

#define TWS_START_PLAY_SYNC    1
#define TWS_STOP_PLAY_SYNC     2
#define TWS_SLAVE_ENTER_SYNC   3


#define BATTERY_REPORT_CMD_ID_ENABLE  0
#define BATTERY_REPORT_CMD_ID_UPDATE  1
#define BATTERY_REPORT_MAX    (9)
#define BATTERY_REPORT_MIN    (0)

/*!
 * \brief
 *  bt_dev_info_t �ṩ���ϲ��Զ�������豸��Ϣ
 */
typedef struct
{
    /*! ����������豸��ַ */
    t_bdaddr bd_addr;
    /*! ������������� */
    uint8 name[MGR_MAX_REMOTE_NAME_LEN];
    /*! ���ڱ�ʶ�豸��ʹ��Ƶ�ʣ�1��ʾ���һ��ʹ��
     * Ȼ����������*/
    uint8 mru_num;
    uint8 support_profile; //�������ӹ���profile������ʱʹ��
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    /*! ��Ҫ���������Ƕ����豸,NO_TWS_DEV��TWS_DEV
        ��ʵ�����ϲ��ȡֱ�ӻ�ȡ�������Ƚ�,
     *   ����Ҫ��Ϊʡrcode��rdata����������*/
    uint8 dev_type;
#endif    
} bt_dev_info_t;

/*!
 * \brief
 *  bt_dev_info_t �ṩ���ϲ��������б�
 */
typedef struct
{
    /*! �����豸��Ϣ */
    bt_dev_info_t dev[PAIRED_LIST_MAX];
    /*! �����豸���� */
    uint8 dev_num;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    /*!
     * ���ڽ綨local dev ΪTWS_SLAVE/TWS_MASTER*/
    uint8 dev_role;
#endif    
} bt_paired_info_t;

/* �豸���ƽṹ�� */
typedef struct
{
    /*�豸���ֱ������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 code_type;
    /*�豸���� 18*3 + 2 */
    uint8 name[56];
} bt_device_name_t;

/* �豸��Ϣ�ṹ�� */
typedef struct
{
    /*�豸�汾�ţ�ֻ���ַ�����[0]��ʾ���汾�ţ�[1]Ϊ.�ţ�[2]��ʾ�ΰ汾�ţ�[3]��ʾС�汾�ţ�����1.01 */
    uint8 device_ver[4 + 1];
    uint8 reserve[2];
    /*�豸���� 57B*/
    bt_device_name_t device_name;
} bt_device_info_t;

#define BT_UART_IDLE_TIMER  8000 //8S����IDLEģʽ����λΪ1ms����������rt 6s lps

typedef struct
{
    uint8 work_mode :2;
    uint8 standby_exit :1;
    uint8 need_patch_download :1;
    uint8 need_power_on :1;
    uint8 save_addr :1;
    uint8 save_name :1;
    uint8 btt_test_mode:1;    
    uint8 max_paired_num;
    uint8 volume;/*Զ���豸��������ǰ���������Խ���ǰ������BT STACKҲά���ã���������ֵתΪAVRCP VOLUME 0 ~ 127*/
    uint16 uart_idle_timer;
    uint32 random_lap;
    btstack_config_t cfg;
    bool *p_btstack_standby;
} bt_stack_arg_t;

typedef struct
{
    uint8 remote_addr[6];
    uint8 support_profile;
} bt_paired_dev_info_t;

typedef struct
{
	uint8    remote_addr[6];
	uint8    support_profile;	
	uint8    linkkey_type;
	uint8    linkkey[16];
	uint8    local_addr[6];
    uint8    remote_name[16];
	uint8    reserved[6];
} bt_paired_dev_info2_t;


typedef enum
{
    TEST_CONN_STATUS_NONE,
    TEST_CONN_STATUS_WAIT_PAIR,
    TEST_CONN_STATUS_PAIRED,
    TEST_CONN_STATUS_LINKED,
    TEST_CONN_STATUS_ERROR		
} test_conn_status_e;

typedef enum
{
    TEST_BT_ERR_NONE,
    TEST_BT_ERR_HW,
    TEST_BT_ERR_PAGE_TIMEOUT,
    TEST_BT_ERR_CONNECTION_TIMEOUT,
    TEST_BT_ERR_LINKEY_MISS,  //�Է�ȡ�����
    TEST_BT_ERR_NOT_POPUP_WINDOW //�Է����뵯������ 	
} test_bt_err_status_e;

typedef struct
{
    uint8 conn_status;
    uint8 err_status;
    uint8 a2dp_status;
    uint8 hfp_status;	
    uint8 num_connected; 
    uint8 support_profile;	       	
} test_btstack_status_t;

/*!
 * \brief
 *  tws_auto_info_t �ṩ���ϲ�Ļ�������
 */
typedef struct
{
    /*! ����������豸��ַ */
    t_bdaddr bd_addr;
    uint8 support_profile; //�������ӹ���profile������ʱʹ��
} tws_auto_info_t;

/*!
 * \brief
 *  adv_data_t ble�㲥�����ø�ʽ 
 *  The AD type data formats and meanings are defined in [Core Specification Supplement], Part A.
 */
typedef struct
{
    uint8 len;
    uint8 adv_type;//The AD type identifier values are defined in the Assigned Numbers document.    
    uint8 content[31]; //countent have len-1 byte 
} adv_data_t; //32byte

#endif
