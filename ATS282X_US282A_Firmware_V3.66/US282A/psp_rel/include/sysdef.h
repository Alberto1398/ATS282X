/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ�����ڴ涨��
 * ���ߣ�liminxian
 ********************************************************************************/

#ifndef _SYSDEF_H
#define _SYSDEF_H

#include <storage_device.h>
#include <typeext.h>
#include <sys_board_def.h>

//clear watch dog
#define CLEAR_WATCH_DOG         (act_writel(act_readl(WD_CTL) | 0x01, WD_CTL))
#define DISABLE_WATCH_DOG       (act_writel(0x69, WD_CTL))
#define ENABLE_WATCH_DOG(CLK)   (act_writel(0x51 | (((CLK)&0x07) << 1), WD_CTL))

/*************system*******************************************************************/
#define SYS_CHECK_BANKSWITCH_ON         //����Ŀ���bank�л�����
/*********************************************************************************************************************
 * ϵͳȫ�ֱ��� 32 �ֽ�(0x9fc09f60~0x9fc09f7f)
 �ṩ��card uhost��ȫ�ֶ��壬�̻�������õ��������޸�
 *********************************************************************************************************************/

/*
 g_card_type bitʹ�ã�
 bit0(card type):      0--1_line, 1--4_line;
 bit1(sdvccout power): 0--direct, 1--GPIO;
 bit2(DAT3 pull high): 0--hardware pull, 1--GPIO pull;
 bit3(write_fast):     0--not continuous, 1--continuous write;
 bit4(read fast):      0--not continuous,1--continuous read;
 */
#define     g_card_type     (*(uint8 *)(RDATA_ADDR_BASE + 0x09f70))

//g_card_para Ӧ����Ҫ��װ�ؿ�����֮ǰ�������������������������ʹ��ǰһ�����õ�ֵ
#define     g_card_para     (*(uint8 *)(RDATA_ADDR_BASE + 0x09f71))
#define     g_card_rca      (*(uint16 *)(RDATA_ADDR_BASE + 0x09f72))
//bit 0 for continuous read or write
//bit 1 for is already init
#define     g_card_state    (*(uint8 *)(RDATA_ADDR_BASE + 0x09f74))
#define     g_stub_setup    (*(uint8 *)(RDATA_ADDR_BASE + 0x09f78))

/*bitmap  for g_uhost_state  */
#define GL_DEVICE_IN                   (1 << 0) // bit0,USB�ж�������־�Ƿ���U�̲���
#define GL_CONTROLLER_ENABLED          (1 << 1) // bit1,������ʹ�ܱ��
#define GL_ENUMERATED                  (1 << 2) // bit2,
#define GL_SPEED_FULL                  (0)      // bit[3]: 0--U��ʹ��Ϊ���٣�1--U��ʹ��Ϊ����
#define GL_SPEED_HIGH                  (1 << 3) // bit[3]: 0--U��ʹ��Ϊ���٣�1--U��ʹ��Ϊ����
#define GL_DP_PULLHIGH                 (1 << 4) // bit4,Timer�ж�������־�Ƿ���U�̲���
#define GL_SPEED_HIGH_INTENDED         (1 << 5) // bit5,��ʼ��ʱ�ϲ���Ҫ֧�ָ���(��ʵ���ϲ�һ���ɹ�)
#define GL_PROCESS_INITIAL             (1 << 6) // bit6,uhost���������У�����ϵͳ�����
#define     g_uhost_state         (*(uint8 *)(RDATA_ADDR_BASE + 0x09f60))

//for some comptile cnfig
#define     g_uhost_cfg           (*(uint8 *)(RDATA_ADDR_BASE + 0x09f61))

// VFS GLOBAL
#define g_fs_scan_space           (*(uint32 *)(RDATA_ADDR_BASE + 0x09f6c))

/*********************************************************************************************************************
 * ����ϵͳ��ȫ�ֱ�����ַ
 *********************************************************************************************************************/

/* check map, must not change address of following variables, if add new variables after them.  */
#define GLOBAL_AP_BANK_HANDLE_HANDLER     (*(uint32 *)(RDATA_ADDR_BASE + 0x1a8d0)) //ap_bank_handle_handler
#define GLOBAL_CARD_BANK_SWITCH_HANDLER   (*(uint32 *)(RDATA_ADDR_BASE + 0x201e8))

/********system back_door*******/

#define BACKDOOR_EXCEPTION_NOTRESET       (1 << 1) /*if exception, 0--will reset; 1-not reset*/

#define BACKDOOR_DISUSB_DETECT            (1 << 6)
#define BACKDOOR_DISUHOST_DETECT          (1 << 7)
#define BACKDOOR_USB_PLUGIN               (1 << 8)
#define BACKDOOR_UHOST_PLUGIN             (1 << 9)
#define BACKDOOR_USB_STATUS_CHANGED       (1 << 10)
#define BACKDOOR_SYSCLK_LOCKED            (1 << 11)
#define BACKDOOR_DISCARD_DETECT           (1 << 12)
#define BACKDOOR_NOVRAM_WRITE             (1 << 13)

#define BACKDOOR_PRINT_AP_BANKSWITCH             (1 << 16)
#define BACKDOOR_PRINT_SYS_BANKSWITCH            (1 << 17)
#define BACKDOOR_PRINT_DSP_BANKSWITCH            (1 << 18)

#define BACKDOOR_ANTIPOP_START					(1 << 28)
#define BACKDOOR_AUDIO_POW_SAVING               (1 << 29)

#define GLOBAL_KERNEL_BACKDOOR_FLAG     (*(uint32 *)(RDATA_ADDR_BASE + 0x1aa5c))//backdoor_flag
/***************/

#define  SAVE_BT_NONE (0)
#define  SAVE_BT_ALL  (1)
#define  SAVE_BT_NAME (2)
#define  SAVE_BT_ADDR (3)

/*Ƶƫд��efuse��*/
#define  RW_TRIM_CAP_EFUSE  (0)
/*Ƶƫд��norflash��*/
#define  RW_TRIM_CAP_SNOR   (1)

typedef enum
{
    WK_TYPE_BATIN = 0,
    WK_TYPE_REMOTE,
    WK_TYPE_NFC, WK_TYPE_ONOFF,//PLAY KEY
    WK_TYPE_HDSWON,
    WK_TYPE_HDSWOFF,
    WK_TYPE_S3BT_TIMERON,
    WK_TYPE_BT_ACTIVE,
} sys_wake_type_e;

typedef void(*print_func_t)(uint8 *str, uint8 data);
typedef struct
{
    uint32 wake_up;
    uint8 *cfg_buffer;
    print_func_t print;
} welcome_argument_t;

typedef struct
{
    uint8 logical_index;
    uint8 rw_len;
    uint8 *rw_buffer;
} nvram_param_rw_t;

typedef struct
{
    uint8 cap_base_value;
    uint8 cap_trim_value;
} trim_cap_value_t;

typedef enum
{
    TRIM_CAP_WRITE_NO_ERROR,
    TRIM_CAP_WRITE_ERR_HW,
    TRIM_CAP_WRITE_ERR_NO_RESOURSE
} trim_cap_write_result_e;

typedef struct
{
    uint32 fw_cap_size;                 //�������õĹ̼����ܴ�С
    uint32 spi_cap_size;                //flash�ܴ�С
    uint32 need_write_protect_size;     //��Ҫ���õ�д���������С
    uint32 set_write_protect_size;      //���õ�д���������С
}wp_zone_info_t;


typedef enum
{
    /*��ȡƵƫ��ȷ*/
    TRIM_CAP_READ_NO_ERROR,
    /*��ȡƵƫ����ֵ����������Ƶƫֵ����Ҫ��ƫ��*/
    TRIM_CAP_READ_ADJUST_VALUE,
    /*��ȡƵƫ����*/
    TRIM_CAP_READ_ERR_HW,
    /*δдƵƫֵ*/
    TRIM_CAP_READ_ERR_NO_WRITE,
    /*Ƶƫֵ�Ƿ�*/
    TRIM_CAP_ERAD_ERR_VALUE
} trim_cap_read_result_e;

#endif  /*_SYSDEF_H*/

