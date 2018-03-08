#ifndef __AP_MANAGER_TEST_H
#define __AP_MANAGER_TEST_H

#include <psp_includes.h>
#include "test_bt_stack_common.h"
#include "ap_autotest_mem_define.h"
#include "../../inc/app_msg.h"
#include "../../inc/stub_interface.h"

#include "ap_autotest_cmd.h"

#define  MSG_TARGET_ID_MANAGER   MQ_ID_MNG
#define  MSG_TARGET_ID_ENGINE    MQ_ID_EGN
#define  MSG_TARGET_ID_BTSTACK   MQ_ID_BT 
#define  MSG_TARGET_ID_FRONTAPP  MQ_ID_DESK

/*!
 * \brief
 *  ����Ӧ��ID�����֧��32��Ӧ�ã�ID��Ӧ app_name_ram �е����֡�
 */
#define APP_ID_MUSIC        0x00
#define APP_ID_LINEIN       0x01
#define APP_ID_BTPLAY       0x02
#define APP_ID_BTCALL       0x03
#define APP_ID_RADIO        0x04
#define APP_ID_USOUND       0x05
#define APP_ID_UDISK        0x06
#define APP_ID_RECORD       0x07
#define APP_ID_USER1        0x08
#define APP_ID_USER2        0x09
#define APP_ID_CONFIG       0x0a
#define APP_ID_MANAGER      0x0b
#define APP_ID_UPGRADE      0x0c
#define APP_ID_BTSTACK      0x0d
#define APP_ID_MENGINE      0x0e
#define APP_ID_LINEIN_EG    0x0f
#define APP_ID_BTPLAYEG     0x10
#define APP_ID_BTCALLEG     0x11
#define APP_ID_FMENGINE     0x12
#define APP_ID_UENGINE      0x13
#define APP_ID_USER3        0x14
#define APP_ID_USER4        0x15
#define APP_ID_ALARM        0x16

//ͨ�ò���ID,��Χ0x01-0xbf
#define TESTID_MODIFY_BTNAME        0x01
#define TESTID_MODIFY_BLENAME       0x02
#define TESTID_FM_TEST              0x05
#define TESTID_GPIO_TEST            0x06
#define TESTID_LINEIN_CH_TEST_ATS2825       0x07
#define TESTID_MIC_CH_TEST          0x08
#define TESTID_FM_CH_TEST           0x09
#define TESTID_SDCARD_TEST          0x0a
#define TESTID_UHOST_TEST           0x0b
#define TESTID_LINEIN_TEST          0x0c
#define TESTID_PREPARE_PRODUCT_TEST 0x0d
#define TESTID_PRODUCT_TEST         0x0e
#define TESTID_FLASHDUMP_TEST       0x0f
#define TESTID_BURN_IC              0x10
#define TESTID_CHECK_IC             0x11
#define TESTID_FLASHDOWNLOAD_TEST   0x12
#define TESTID_MEM_UPLOAD_TEST      0x13
#define TESTID_MEM_DOWNLOAD_TEST    0x14
#define TESTID_GPIO_TEST_ATS2823    0x15
#define TESTID_READ_BTNAME          0x16
#define TESTID_MONITOR              0x17
#define TESTID_FTMODE               0x18
#define TESTID_BQBMODE              0x19
#define TESTID_FLASHTEST            0x1a
#define TESTID_LRADC_TEST           0x1b
#define TESTID_LINEIN_CH_TEST_ATS2823  0x1c

//�������ID,��Ҫ�͹��߰󶨣������������IDֵ ��Χ0xc0-0xfe
#define TESTID_MODIFY_BTADDR  0xc0
#define TESTID_BT_TEST        0xc1
#define TESTID_MP_TEST        0xc2
#define TESTID_MP_READ_TEST   0xc3
#define TESTID_READ_BTADDR    0xc4
#define TESTID_BER_TEST       0xc5
#define TESTID_RSSI_TEST      0xc6
#define TESTID_MODIFY_BTADDR2 0xc7

//��ȡ�ò���ID,С����Ҫһֱ�ȴ�PC�ظ���Ч��TESTID
#define TESTID_ID_WAIT        0xfffe

//��ȡ�ò���ID,С�����Խ�������
#define TESTID_ID_QUIT        0xffff

#define TEST_BTNAME_MAX_LEN    56 /*18*3 + 2*/
#define TEST_BTBLENAME_MAX_LEN 30 /*29 + null*/

#define DEBUG_PRINT_ATT

#ifdef DEBUG_PRINT_ATT
#define DEBUG_ATT_PRINT(a,b,c)    libc_print(a,b,c)
#else
#define DEBUG_ATT_PRINT(a,b,c) 
#endif

//�Ƿ��¼��ȡ��ͨ�����ݵ�����
//#define DEBUG_WRITE_CHANNEL_DATA

#define  VM_BTSTACK             0x00000000//0 �����豸��Ϣ /*�õ�ַ�̶������ˣ������޸�*/

/* AP VRAM ��������־ = 0x55AA + ID (0x00 - 0x1f) */
#define  VRAM_MAGIC(vram_addr)  (0x55AA + (short)((vram_addr) >> 16))

#define MGR_MAX_REMOTE_NAME_LEN         16
#define BD_ADDR_LEN                     6
#define BT_MAC_NAME_LEN_MAX    56 /*18*3 + 2*/
#define BT_BLE_NAME_LEN_MAX    30 /*29 + null*/


typedef enum
{
    TEST_MODE_CARD = 0,
    TEST_MODE_USB,
    TEST_MODE_UART,
    TEST_MODE_EXTEND
}test_mode_e;

typedef enum
{
    TEST_PASS = 0,
    TEST_BT_FAIL,
    TEST_MIC_FAIL,
    TEST_LINEIN_FAIL,
    TEST_UHOST_FAIL,
    TEST_FM_FAIL,
    TEST_IR_FAIL, 
    TEST_GPIOA_SHORT,
    TEST_GPIOA_SHORT_VCC,
    TEST_GPIOA_SHORT_GND,
    TEST_GPIOA_OPEN,
    TEST_GPIOB_SHORT,
    TEST_GPIOB_SHORT_VCC,
    TEST_GPIOB_SHORT_GND,
    TEST_GPIOB_OPEN,  
    TEST_GPIOSIO_SHORT,
    TEST_GPIOSIO_SHORT_VCC,
    TEST_GPIOSIO_SHORT_GND,
    TEST_GPIOSIO_OPEN,  	
    TEST_KEY_FAIL,
    TEST_SDCARD_FAIL,    
    TEST_FW_ERROR,   
}test_result_e;

typedef enum
{
    TEST_STATE_INIT = 0x0,
    TEST_STATE_START = 0x1,
    TEST_STATE_STOP = 0x2,
    TEST_STATE_PAUSE = 0x3,
}test_state_e;

typedef enum
{
    //DUTֱ�Ӻ�PC����
    DUT_CONNECT_MODE_DIRECT = 0,
    //DUTͨ��UDAת�Ӱ��PC����
    DUT_CONNECT_MODE_UDA,
}dut_connect_state_e;

typedef enum
{
    //DUT��������ģʽ,���Դ�������ֹͣ
    DUT_WORK_MODE_NORMAL = 0,
    //DUT���Դ���������һ�����
    DUT_WORK_MODE_SPECIAL = 1,
}dut_work_mode_e;

typedef struct
{
    /* ��Ϣ�ṹ��С:
     * ���Գ���ɼ��ô�С��ȷ����ǰ�̼��Ͳ��Գ����Ƿ����,
     * �������������Ҫ�����ӿ��������չ̼�;
     */
    uint8  info_size;
    
    /* ���Խ׶�:
     * 0: config Ӧ�ù���ȴ�ֱ�����Խ׶� 1;
     * 1: config Ӧ�ü������в�������������;
     */
    uint8  test_stage;
    
    uint8  base_type;
    uint8  stub_phy_type;
    uint32 ap_file_offset;
    uint32 cfg_file_offset;
    uint32 cfg_file_len;
}test_ap_info_t;

typedef struct
{
    uint8 base_type;
    uint8 file_sys_id;
    uint16 ap_file_offset;
    uint16 cfg_file_offset;
    uint16 cfg_file_len;
}test_ap_old_info_t;


typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint16 offsetl;
    uint16 offseth;
    uint16 lengthl;
    uint16 lengthh;
}att_fread_arg_t;

typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint8 dut_connect_mode;
    uint8 dut_work_mode;
    uint8 timeout;
    uint8 reserved;
    uint8 bdaddr[6];
    uint8 reserved2[22];
}att_start_arg_t;

typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint8 reboot_timeout;
    uint8 reserved[31];
}att_swtich_fw_arg_t;

typedef struct
{
    uint16 last_test_id;
    uint8 reserved[30];
}pc_test_info_t;

typedef struct
{
    stub_ext_cmd_t ext_cmd;
    pc_test_info_t pc_test_info;
}att_pc_test_info_t;


typedef struct
{
    uint8 filename[11];
    uint8 reserved1[5];
    uint32 offset;
    uint32 length;
    uint8 reserved2[8];
} atf_dir_t;


typedef struct
{
    uint8 magic[8];
    uint8 sdk_version[4];
    uint8 reserved[20];
    atf_dir_t atf_dir[15];
} atf_head_t;


typedef test_result_e (*test_func_t)(void *arg);

typedef int32 (*read_arg_func_t)(uint16 *line_buffer, uint8 *arg_buffer, uint32 arg_len);

typedef struct
{
    uint16 test_id;
    uint16  support_card_test:1;       //�Ƿ�֧�ֿ����Է���
    uint16  support_att_ap_test:1;     //�Ƿ�֧��ATT����Ӧ�ò���
    uint16  support_att_card_test:1;   //�Ƿ�֧��ATT����������
    uint16  reserved:13;       
    test_func_t test_func; 
}att_task_stru_t;

typedef struct
{
    uint16 test_id;
    read_arg_func_t read_arg_func;
}att_task_read_arg_t;

typedef struct
{
    uint8  test_state;
    uint8 reserved;
    uint16 test_id;
    uint16 arg_len;
}autotest_test_info_t;

typedef struct
{
    uint8 stub_head[6];
    /*�䳤����*/
    uint8 arg_data[0];
}att_pc_arg_data_t;

typedef struct
{
    uint8 bt_name[TEST_BTNAME_MAX_LEN];
}bt_name_arg_t;


typedef struct
{
    uint8 bt_ble_name[TEST_BTBLENAME_MAX_LEN];
}ble_name_arg_t;

typedef struct
{
    uint8 bt_addr[6];
    uint8 bt_addr_add_mode;
    uint8 bt_burn_mode;
    uint8 reserved;
}bt_addr_arg_t;

typedef struct
{
    uint8 bt_transmitter_addr[6];
    uint8 bt_test_mode;
    uint8 bt_fast_mode;
}btplay_test_arg_t;

typedef struct
{
    uint32 freq;
}fmplay_test_arg_t;

typedef struct
{
    uint32 gpioA_value;
    uint32 gpioB_value;
	uint32 gpioSIO_value;
}gpio_test_arg_t;

typedef struct
{
    uint8 cfo_channel_low;          //���ŵ���
    uint8 cfo_channel_mid;          //���ŵ���
    uint8 cfo_channel_high;         //���ŵ���
    uint8 cfo_test;                 //�Ƿ����Ƶƫ����
    uint8 cfo_index_low;            //��¼��������Сֵ
    uint8 cfo_index_high;           //��¼���������ֵ
    uint8 cfo_index_changed;        //���Խ����Ƿ��޸�����
    int8 cfo_threshold_low;         //Ƶƫ��С��ֵ
    int8 cfo_threshold_high;        //Ƶƫ�����ֵ
    uint8 cfo_write_mode;           //Ƶƫ���д��ģʽ
    uint8 reserved[2];
    int32  cfo_upt_offset;          //UPT��ʼƵ��ƫ��
    uint8 pwr_test;
    int8 pwr_threshold_low;
    int8 pwr_threshold_high;
}mp_test_arg_t;

typedef struct
{
    uint8 cmp_btname_flag;
    uint8 cmp_btname[TEST_BTNAME_MAX_LEN];
    uint8 cmp_blename_flag;
    uint8 cmp_blename[TEST_BTBLENAME_MAX_LEN];
}read_btname_test_arg_t;

typedef struct
{
    uint8  stub_head[6];
    uint16 test_id;
    uint8  test_result;
    uint8  timeout;
    /*�䳤����*/
    uint16  return_arg[0]; 
}return_result_t;

typedef struct
{
    uint8 front_ap_id;
    uint8 ap_switch_flag;
}test_share_info_t;

typedef struct
{
    uint32 record_cnt;
    uint32 succeed_cnt;
    uint32 failed_cnt;    
}log_test_info_t;

typedef struct
{
    uint32 magicl;
    uint32 magich;
    uint8 reserved;
    uint8 addr[3];
    uint32 record_cnt;
    uint32 succeed_cnt;
    uint32 failed_cnt;
    bt_paired_dev_info2_t bt_paired_dev_info;
    uint32 checksum;
}btaddr_log_file_t;

typedef struct
{
    stub_ext_cmd_t  cmd;
    uint8 log_data[0];
}print_log_t;

typedef struct
{
    uint16 magic;
    uint8 bt_addr[6];
    uint8 burn_lock_flag; //������ַ�Ƿ���д��־λ
    uint8 remote_name[MGR_MAX_REMOTE_NAME_LEN];
    uint8 device_name[BT_MAC_NAME_LEN_MAX]; //�豸����  
    uint8 device_fix_pin_code[16];
    uint8 ble_device_name[BT_BLE_NAME_LEN_MAX]; //�豸����    
}bt_addr_vram_t;

/** ATF�ļ�����Ĳ���
*/
typedef struct
{
    uint8 ber_channel_low;          //���ŵ���
    uint8 ber_channel_mid;          //���ŵ���
    uint8 ber_channel_high;         //���ŵ���
    int8  ber_thr_low;
    int8  ber_thr_high;
    int8  rssi_thr_low;
    int8  rssi_thr_high;    
}ber_test_arg_t;

typedef struct
{
    uint8 lradc1_test;
    uint8 lradc1_thr_low;
    uint8 lradc1_thr_high;
    uint8 lradc2_test;
    uint8 lradc2_thr_low;
    uint8 lradc2_thr_high;
    uint8 lradc4_test;
    uint8 lradc4_thr_low;
    uint8 lradc4_thr_high;    
}lradc_test_arg_t;

typedef struct
{
    uint8 test_left_ch;
    uint8 test_right_ch;
    uint8 test_left_ch_SNR;
    uint8 test_right_ch_SNR;
    uint32 left_ch_power_threadshold;
    uint32 right_ch_power_threadshold;
    uint32 left_ch_SNR_threadshold;
    uint32 right_ch_SNR_threadshold;
    uint16 left_ch_max_sig_point;
    uint16 right_ch_max_sig_point;
}channel_test_arg_t;

typedef enum
{
    //ֻ���򿪣�����ļ������ڣ������ļ�����Ϊ0
    FILE_OPEN_MODE_RB,
    //ֻд�򿪣�����ļ����ڣ����ļ�������0
    FILE_OPEN_MODE_WB,
}att_file_open_mode_e;

typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint8 open_mode[8];     //���ļ�ģʽ��4�ֽ�unicode�ַ�
    uint8 file_name[0];     //�ļ�����unicode�ַ�
}att_fopen_arg_t;

typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint16 offsetl;         //�ļ�д��ƫ��
    uint16 offseth;
    uint16 lengthl;          //�ļ�д�볤��
    uint16 lengthh;
}att_fwrite_arg_t;

extern int32 g_file_sys_id;
extern os_event_t *thread_mutex;
extern uint32 tick_cnt;
extern uint8 g_test_mode;
extern uint8 att_cmd_temp_buffer[];
extern uint8 g_read_code_mode;
extern test_share_info_t g_test_share_info;
extern uint32 g_test_file_handle;
extern uint8 g_cur_line_num;
extern uint32 g_write_file_len;
extern uint8 g_skip_product_test;
extern uint8 g_att_version;
extern uint32 g_app_func_id;
extern test_ap_info_t* g_test_ap_info;
extern autotest_test_info_t g_test_info;
extern uint32 g_epc_addr[];
extern const uint8 g_ap_name[];
extern test_ap_info_t *g_p_test_ap_info_bak;
extern uint8 g_support_norflash_wp;
extern uint32 g_test_base_time;
extern int32 _config_fs_init(uint8 disk_type) __FAR__;

extern int send_sync_msg(uint8 target_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout) __FAR__;

extern void test_dispatch(void) __FAR__;

extern uint32 tick_ISR_install(void) __FAR__;

extern void tick_ISR_uninstall(uint32 timer_id) __FAR__;

extern test_result_e act_test_gpio_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_gpio_test_ATS2823(void *arg_buffer) __FAR__;

extern test_result_e act_test_linein_channel_test_ATS2825(void *arg_buffer) __FAR__;

extern test_result_e act_test_linein_channel_test_ATS2823(void *arg_buffer) __FAR__;

extern test_result_e act_test_fm_channel_test(void *arg_buffer) __FAR__; 

extern test_result_e act_test_mic_channel_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_sdcard_play_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_uhost_play_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_linein_play_test(void *arg_buffer) __FAR__;

extern test_result_e att_mp_test(void *arg_buffer)__FAR__;

extern test_result_e att_mp_read_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_bt_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_btt_test(void *arg_buffer) __FAR__;

extern int32 act_test_read_test_info(uint8 read_line, uint8 *test_id, uint8 *arg_buffer, uint32 arg_len) __FAR__;

extern test_result_e act_test_modify_bt_addr(void *arg_buffer) __FAR__;

extern int32 act_test_read_bt_addr_from_log(uint8 *byte_buffer) __FAR__;

extern test_result_e act_test_modify_bt_name(void *arg_buffer) __FAR__;

extern test_result_e act_test_modify_bt_ble_name(void *arg_buffer) __FAR__;

extern int32 act_test_report_result(uint8 *write_data, uint16 total_len) __FAR__;

extern uint32 read_att_test_count(void) __FAR__;

extern void att_write_test_info(uint8 *test_info, uint32 test_value, uint32 write_data) __FAR__;

extern uint32 att_test_fread(uint8 *buffer, uint32 len, uint32 offset) __FAR__;

extern uint32 att_write_data(uint16 cmd, uint32 payload_len, uint32 data_addr) __FAR__;

extern uint32 att_read_data(uint16 cmd, uint32 payload_len, uint32 data_addr) __FAR__;

extern int print_log(const char* format, ...) __FAR__;

extern int act_test_read_bt_addr(void *arg_buffer) __FAR__;

extern test_result_e act_test_product_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_prepare_product(void *arg_buffer) __FAR__;

extern int32 act_test_start_deal(void) __FAR__;

extern void att_power_off(void) __FAR__;

extern void write_log_file(uint32 mode) __FAR__;

extern int act_test_read_bt_name(void *arg_buffer) __FAR__;

extern test_result_e act_test_enter_ft_mode(void *arg_buffer) __FAR__;

extern test_result_e act_test_enter_BQB_mode(void *arg_buffer) __FAR__;

extern void act_test_change_test_timeout(uint16 timeout) __FAR__;

extern void act_test_flashtest(void *arg_buffer) __FAR__;

extern test_result_e act_test_ber_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_lradc_test(void *arg_buffer) __FAR__;

extern test_result_e act_test_rssi_test(void *arg_buffer) __FAR__;

#endif
