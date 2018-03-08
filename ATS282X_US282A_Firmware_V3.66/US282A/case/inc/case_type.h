/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE Ӧ�ú͹��ܼ���ĺ궨�壬ö�����ͣ��ṹ�嶨��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "psp_includes.h"

#ifndef _case_type_H_
#define _case_type_H_

#define FRAME_SIZE 1152

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
#define APP_ID_OTA_TEST     0x14
// #define APP_ID_USER4        0x15
#define APP_ID_OTA_UPGRADE  0x15
#define APP_ID_ALARM        0x16
#define APP_ID_BTSTACK_OTA  0x17
#define APP_ID_MAX          0x18


#define APP_ID_THEENGINE    0x80 //��ָ����
#define APP_ID_BT_MANAGER   0x81 //BT MANAGER��ռһ���¼�����

//app func id for config ���8��bit ֧��64������
#define APP_FUNC_BTPLAY      0

#define APP_FUNC_PLAYCARD    1

#define APP_FUNC_PLAYUHOST   2

#define APP_FUNC_PLAYLINEIN  3

#define APP_FUNC_RADIO       4

#define APP_FUNC_USOUND      5

#define APP_FUNC_CONFIG      6

#define APP_FUNC_UDISK       7

#define APP_FUNC_CLOCK       8

//����udisk¼���ļ� APP_FUNC_PLAYURECORD
#define APP_FUNC_PLAYURECORD 9
//���ſ�¼���ļ�
#define APP_FUNC_PLAYCRECORD 10

#define APP_FUNC_LINREC_TOC  11

#define APP_FUNC_FMREC_TOC   12

#define APP_FUNC_MICREC_TOC  13

#define APP_FUNC_BTCALL      14

#define APP_FUNC_UPGRADE     15

#define APP_FUNC_CHARGING    16

//17 no used
//18 no used

#define APP_FUNC_LINREC_TOU  19

#define APP_FUNC_FMREC_TOU   20

#define APP_FUNC_MICREC_TOU  21

#define APP_FUNC_IDLE_MODE   30 //�ⲿ��緽���Ŀ���ģʽ

#define APP_FUNC_OTA_TEST    31

//��Ч��APP_FUNC ID
#define APP_FUNC_INVALID    (63)

/*!
 * \brief
 *  Ӧ�ú�׺�� bitmap��bit<31> = 0h��
 * \note
 * \li  ���׺�������Ӧ��ϵ��bit[30] ��Ӧ��׺������ rom_ext_name ��Ԫ�أ��� MP3��
 */
#define MUSIC_BITMAP        0x76000c00

//#define MUSIC_BITMAP        0x30000000 //only wav & wma

#define AUDIBLE_BITMAP      0x00003000
#define VIDEO_BITMAP        0x01004000
#define PICTURE_BITMAP      0x00780000
#define PICTURE_BITMAP1     0x00600000//��̨���ֲ���ʱ��ͼƬ�ļ�������
#define TEXT_BITMAP         0x00800000
#define RECORD_BITMAP       0x50000000
#define ALLALL_BITMAP       0x00000004
#define ALLFILE_BITMAP      0x00000002
#define ALLDIR_BITMAP       0x00000001

#define RECORD_WAV_BITMAP   0x10000000
#define RECORD_MP3_BITMAP   0x40000000
//���� rom_ext_name �б����� fs_common_func.c �С�
//const char rom_ext_name[31][4] =
//{
//    "MP3",//��Ӧ bit[30]
//    "WMA",
//    "WAV",
//    "AAC",
//    "FLAC",
//    "APE",
//    "AVI",//��Ӧ bit[24]
//    "TXT",
//    "JPG",
//    "JPEG",
//    "BMP",
//    "GIF",
//    "LRC",
//    "ASF",
//    "OGG",//��Ӧ bit[16]
//    "WMV",
//    "AMV",
//    "AAX",
//    "AA",
//    "MP1",
//    "MP2",
//    "M4A",
//    "** ",//��Ӧ bit[8]
//    "** ",
//    "** ",
//    "** ",
//    "** ",
//    "** ",
//    "**",
//    "*f",
//    "*d",//��Ӧ bit[0]
//};

/*! �޷���32λ������ -1 ֵ�궨�� */
#define V_U32_INVALID   0xffffffff
/*! �޷���16λ������ -1 ֵ�궨�� */
#define V_U16_INVALID   0xffff
/*! �޷���8 λ������ -1 ֵ�궨�� */
#define V_U8_INVALID    0xff

/*! �ļ�ϵͳ������С */
#define FILE_SECTOR     512

//������������������
#define MAX_DIGITAL_COUNT 4

#define ASQT_STUB_TEST_EN  0/*ASQT���Ժ�,���ܸ�ASETͬʱ��*/
#define ASET_STUB_TEST_EN  0 /*ASET���Ժ겻�ܸ�ASQTͬʱ��*/

#define TK_STUB_TEST_EN  0   /*tk���Ժ꣬Ҫ�Ѻ�̨BT�ص�*/

/*!
 * \brief
 *  app_param_e ����Ӧ�ô��ݲ�������ö������
 */
//��ʱ����PARAM_S3_EXIT���һ���ֽڣ�
//����Ҫ������Ϻ󣬻��ܲ���֪�������ͱ�����ֵ�Ƕ��ٵ����⡣
//���Ա����ͱ�����ֵ�Ƕ��٣����н����ģ��������������ӡ�
typedef enum
{
    /*! �ղ��� */
    PARAM_NULL = 0x0,
    /*! �� udisk ap ���� */
    PARAM_FROM_UDISK,
    /*! ��alarm ap���뿨¼��*/
    PARAM_FROM_ALARM,
    /*! �� radio ap ���뿨¼�� */
    PARAM_FROM_RADIO_TOC,
    /*! �� radio ap ����u��¼�� */
    PARAM_FROM_RADIO_TOU,
    /*! ��linein���뿨¼��*/
    PARAM_FROM_LINEIN_TOC,
    /*! ��linein����u��¼��*/
    PARAM_FROM_LINEIN_TOU,
    /*����mic¼�����뿨¼��*/
    PARAM_FROM_MIC_TOC,
    /*����mic¼������u��¼��*/
    PARAM_FROM_MIC_TOU,
    /*!����uhost*/
    PARAM_MUSIC_UHOST,
    /*!����card*/
    PARAM_MUSIC_SDCARD,
    /*!����cdisk¼���ļ�����*/
    PARAM_RECORD_CPLAY,
    /*!����udisk¼���ļ�����*/
    PARAM_RECORD_UPLAY,

} app_param_e;

#define PARAM_S3_EXIT  0x80

/*!
 * \brief
 *  app_result_e Ӧ�ò�ӿڷ��ؽ��ö������
 */
typedef enum
{
    /*! û���κ���Ҫ֪ͨ�����ߵ�������أ������߲���Ҫ���κ����⴦�� */
    RESULT_NULL = 0x00,
    RESULT_IGNORE = 0x01,
    /*! û���κ��¼������� com_view_loop ���أ���ʾû�а�����Ϣ��ϵͳ��Ϣ��BT�¼��� */
    RESULT_NONE_EVENT = 0x02,
    
    RESULT_RCP_LC_WAIT = 0x03,

    /*! �޹ؽ�Ҫ�Ľ����һ�㲻�����𳡾���Ӧ���л� */
    RESULT_COMMON_RESERVE = 0x20,

    /*! �������󷵻� */
    RESULT_ERROR = 0x80,
    /*! �˳���ǰ�������ص������ַ�ѭ�� */
    RESULT_BACK_SCENE_DISPATCH = 0x81,
    /*! ���������˳� */
    RESULT_SCENE_EXIT = 0x82,
    /*! �����¼���COMMON���´��� */
    RESULT_KEY_EVENT_REDEAL_BY_COMMON = 0x83,
    /*! ϵͳ��Ϣ��COMMON���´��� */
    RESULT_SYS_MSG_REDEAL_BY_COMMON = 0x84,
    /*! ǿ����ֹTTS */
    RESULT_FORCE_TTS_STOP = 0x85,
    
    /*! OTA���� */
    RESULT_ENTER_OTA_UPGRADE = 0xa1,
    
    /*! �˳��Զ������� */
    RESULT_TESTAP_QUIT = 0xff, //NOTE!!! ���ɸ���

    /*! Ӧ���������˳� */
    RESULT_APP_QUIT = 0x100, //NOTE!!! ���ɸ���

    /*! �����ļ����� */
    RESULT_MUSIC_PLAY = 0x101,
    /*! �����ļ����ſ��ļ� */
    RESULT_MUSIC_CPLAY = 0x102,
    /*! �����ļ�����u���ļ�*/
    RESULT_MUSIC_UPLAY = 0x103,
    /*! ����line in */
    RESULT_ENTER_LINEIN = 0x104,
    /*!RADIO������Ϣ*/
    RESULT_RADIO = 0x105,
    /*! ������������ */
    RESULT_BLUETEETH_SOUND = 0x106,
    /*! ������������*/
    RESULT_BLUETEETH_CALL = 0x107,
    /*! ��̨��ʽ������������ */
    RESULT_BLUETEETH_CALL_BACKGROUND = 0x108,
    /*! ����usb���� */
    RESULT_USB_SOUND = 0x109,
    /*! �ػ� */
    RESULT_POWER_OFF = 0x10a,
    /*! ������ */
    RESULT_LOW_POWER = 0x10b,
    /*! ������������� */
    RESULT_USB_TRANS = 0x10c,
    /*! ����¼���ļ����� */
    RESULT_RECORD_CPLAY = 0x10d,
    /*! ����¼���ļ����� */
    RESULT_RECORD_UPLAY = 0x10e,
    /*!¼��������Ϣ¼�Ƶ�����*/
    RESULT_RECORD_LINEIN_TOC = 0x10f,
    /*!¼��������Ϣ¼�Ƶ�����*/
    RESULT_RECORD_FM_TOC = 0x110,
    /*!¼��������Ϣ¼�Ƶ�����*/
    RESULT_RECORD_MIC_TOC = 0x111,
    /*!¼��������Ϣ¼�Ƶ�U��*/
    RESULT_RECORD_LINEIN_TOU = 0x112,
    /*!¼��������Ϣ¼�Ƶ�U��*/
    RESULT_RECORD_FM_TOU = 0x113,
    /*!¼��������Ϣ¼�Ƶ�U��*/
    RESULT_RECORD_MIC_TOU = 0x114,
    /*!��һ�β���Ӧ�� */
    RESULT_LASTPLAY = 0x115,
    /*!��һ������ѡ��*/
    RESULT_NEXT_FUNCTION = 0x116,
    /*! ����upgrade*/
    RESULT_ENTER_UPGRADE = 0x117,
    /*! �ص�Ĭ��Ӧ�� */
    RESULT_MAIN_APP = 0x118,
    /*! ������ */
    RESULT_ENTER_ALARM = 0x119,
    /*! ������SNOOZEֹͣ */
    RESULT_ALARM_SNOOZE = 0x11a,
    /*! ������ر�ֹͣ */
    RESULT_ALARM_CLOSE = 0x11b,
    /*! ��������Ŀ¼ɨ�� */
    RESULT_ALARM_RING_FOLDER_SCAN = 0x11c,
    /*! FM�Զ���̨ */
    RESULT_AUTO_SEARCH = 0x11d,
    /*! FM�����ֶ���̨ */
    RESULT_MANUAL_SEARCH_UP = 0x11e,
    /*! FM�ݼ��ֶ���̨ */
    RESULT_MANUAL_SEARCH_DOWN = 0x11f,
    /*! FMֹͣ��̨ */
    RESULT_STOP_SEARCH = 0x120,
    /*! ����CONFIG ������ */
    RESULT_CONFIG_CHARGE = 0x121,
    /*! �˳�CONFIG ������ */
    RESULT_CONFIG_CHARGE_QUIT = 0x122,
    /*! ASQT������� */
    RESULT_ENTER_ASQT_HF_TEST = 0x123,
    /*! USER1Ӧ�� */
    RESULT_ENTER_USER1 = 0x124,
    /*! ���볬�͹���ģʽS3 */
    RESULT_SYSTEM_ENTER_S3 = 0x125,
    /*! �˳����͹���ģʽS3������CONFIGʹ�� */
    RESULT_SYSTEM_EXIT_S3 = 0x126,
    /*! ����stub����ģʽ */
    RESULT_ENTER_STUB_DEBUG = 0x127,
    /*! ESD ���� */
    RESULT_ESD_RESTART = 0x128,
    /*! ����ģʽ�����ⲿ��緽������ */
    RESULT_IDLE_MODE,
    /*!����OTA�������� */
    RESULT_ENTER_OTA_SONG_TEST,
    
    RESULT_PLAY_SDD_FID16,//����SDD��չ��16���ļ�OTA����ʹ��

    RESULT_PLAY_SDD_FID17,//����SDD��չ��17���ļ�

    RESULT_PLAY_SDD_FID18,

    RESULT_PLAY_SDD_FID19,
        
    RESULT_PLAY_SDD_FID20,

    RESULT_PLAY_SDD_FID21,
    
    RESULT_PLAY_SDD_FID22,

    RESULT_PLAY_SDD_FID23,
    
    RESULT_PLAY_SDD_FID24,

    RESULT_PLAY_SDD_FID25,

    
    /*! APK/APP ��ť����ֵ���� */
    RESULT_DIALOG_OK = 0x200,
    RESULT_DIALOG_CANCEL = 0x201,
    RESULT_DIALOG_ABORT = 0x202,
    RESULT_DIALOG_RETRY = 0x203,
    RESULT_DIALOG_IGNORE = 0x204,
    RESULT_DIALOG_YES = 0x205,
    RESULT_DIALOG_NO = 0x206,
    RESULT_DIALOG_CANCEL_AUTO = 0x207, //�Զ�ȡ��
    RESULT_DIALOG_USB_SOUND = 0x208, //����USB����
    RESULT_DIALOG_ONLY_CHARGE = 0x209, //�����

    RESULT_HID_SEND_KEY_EVENT = 0x20a,//���ռ�ֵ�����¼�

} app_result_e;

typedef enum
{
    VDD_MODE_LDO = 0, //��Դ����ΪLDOģʽ
    VDD_MODE_DCDC = 1, //��Դ����ΪDCDC
    VDD_MODE_UVLO = 2, //DCDC/LDO�Զ��л�
    VDD_MODE_DC2VOV = 3
//��ѹ�Զ��л�
} vdd_mode_e;

/*!
 * \brief
 *  sys_counter_t ϵͳ���ܼ�ʱ�����ݽṹ������ʱ������0.5 sΪ��λ
 */
typedef struct
{
    /*! ʡ��ػ���ʱ�� */
    uint16 poweroff_counter;
    /*! ����䰵��رռ�ʱ�� */
    uint16 light_counter;
    /*! �����ÿ���Ӳ���һ�γ������ */
    uint16 charge_full_cnt;
    /*! ÿ3s����UI�����ı�һ�� */
    uint16 bat_change_cnt;
    /*! ������ÿ���Ӳ���һ�ε����������� */
    uint16 bat_low_cnt;
    /*! �ȴ�APK/APP��֤��ʱ�� */
    uint8 apk_app_counter;
    /*! �ж�APK avialable����ʱ�� */
    uint8 apk_counter;
    /*! ǰ̨UI���м�ʱ�� */
    uint8 ui_idle_counter;
} sys_counter_t;

typedef struct
{
    uint8 ap_id;
    uint8 param;
    bool engine_do;
    app_result_e result_val;
    uint8 app_func_id;
} ap_switch_info_t;


typedef struct
{
    uint8 freq_mode; // 0 = 32.768k, 1 = 13M
    uint8 gpio_level; //���13M��������0~7
    uint16 rev;
} FM_init_parm_t;

/*!
 *  \brief
 *  g_sys_counter ��ȫ�֣�ϵͳ���ܼ�ʱ���������ϵͳȫ�ֿռ�
 */
extern sys_counter_t g_sys_counter;

/************************state for customer**************************/
/*Ԥ��������g_customer_state��ÿ��bit�Ķ��壬��Բ�ͬ�ͻ��޸�*/

#define CUSTOMER_ESD_WAKEUP                    (1 << 6)//esd����

/************************end**************************/
/*****************************************************

Note: ���¼Ĵ�����Ҫ��ʹ��֮��ˢ����!!!
PMU:
Offset	Register Name	��ѹ��
0x4c	WKEN_CTL	RTCVDD��Ҫˢ����

GPIO
Offset	Register Name	��ѹ��
0x3C	GPIOB0_CTL	RTCVDD��Ҫˢ����

CMU_ANALOG
Offset	Register Name	��ѹ��
0x00	HOSC_CTL	RTCVDD��Ҫˢ����
0x04	HCL_CTL	RTCVDD��Ҫˢ����

RTC
Offset	Register Name	��ѹ��
0x00	RTC_CTL	RTCVDD��Ҫˢ����
0x08	RTC_DHMSALM	RTCVDD��Ҫˢ����
0x0C	RTC_DHMS	RTCVDD��Ҫˢ����
0x10	RTC_YMD	RTCVDD��Ҫˢ����
0x18	RTC_ACCESS	RTCVDD��Ҫˢ����

Offset	Register Name	��ѹ��
0x30	RTC_BAK0	RTCVDD��Ҫˢ����
0x34	RTC_BAK1	RTCVDD��Ҫˢ����
0x38	RTC_BAK2	RTCVDD��Ҫˢ����
0x3C	RTC_BAK3	RTCVDD��Ҫˢ����

BT
Offset	Register Name	��ѹ��
0x00	BT_CFG	RTCVDD��Ҫˢ����
*************************************************/

typedef enum
{
    APP_ATT_FUNC_ID_BTPLAY,
    APP_ATT_FUNC_ID_BTCALL,
    APP_ATT_FUNC_ID_CARDPLAY,
    APP_ATT_FUNC_ID_LINEIN,
    APP_ATT_FUNC_ID_RADIO
}att_func_id_e;

#endif
