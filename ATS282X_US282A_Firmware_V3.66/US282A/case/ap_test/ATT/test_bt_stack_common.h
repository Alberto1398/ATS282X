#ifndef _TEST_BTSTACK_COMMON_H
#define _TEST_BTSTACK_COMMON_H

#define HFP_SUPPORTED           0x01
#define A2DP_SUPPORTED          0x02
#define AVRCP_SUPPORTED         0x04
#define PROFILE_VALID           0x80
#define LINKKEY_VALID           0x40

#if 0
typedef struct
{
    uint8 bytes[BD_ADDR_LEN];
} t_bdaddr;

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
    uint8 support_profile;//�������ӹ���profile������ʱʹ��
} bt_dev_info_t;

typedef struct
{
    uint32 enable_a2dp              : 1;
    uint32 enable_a2dp_cp           : 2;
    uint32 enable_avdtp_aac         : 1;
    uint32 enable_avrcp             : 1;
    uint32 enable_avrcp_14          : 1;
    uint32 enable_avrcp_volume_sync : 1;
    uint32 enable_hfp               : 1;
    uint32 enable_hfp_volume_sync   : 1;
    uint32 enable_hfp_bat_report    : 1;
    //HFP_PB_ALL,HFP_PB_DISABLE,HFP_PB_ONLY_NUM, 0:��ֹ��1:�����汾 2:ֻ��������벥��
    uint32 enable_hfp_phonebook     : 2;
    uint32 enable_spp               : 1;
    uint32 enable_ble               : 1;
    uint32 enable_hid               : 1;
} btstack_config_t;

typedef struct
{
    uint8 work_mode : 2;
    uint8 need_patch_download : 1;
    uint8 save_addr : 1;
    uint8 save_name : 1;
    uint8 max_paired_num;
    uint8 volume;/*Զ���豸��������ǰ���������Խ���ǰ������BT STACKҲά���ã���������ֵתΪAVRCP VOLUME 0 ~ 127*/
    uint16 uart_idle_timer;
    uint32 random_lap;
    btstack_config_t cfg;

    t_bdaddr rmt_bd_addr;
    bt_dev_info_t *cfg_dev_info;
} bt_stack_arg_t;
#endif
typedef struct
{
	uint8    remote_addr[6];
	uint8    support_profile;		
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

#endif
