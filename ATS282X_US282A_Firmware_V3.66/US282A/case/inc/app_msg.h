/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ��˽����Ϣ��ϵͳ��Ϣ��غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _app_msg_H_
#define _app_msg_H_

#include "psp_includes.h"
#include "case_type.h"

/*!
 * \brief
 *  msg_apps_type_e Ӧ����Ϣö������
 */
typedef enum
{
    /*! ����Ϣ */
    MSG_NULL = 0x0000,

    /*!���������Ƹ�*/
    EVENT_ENTER_BTPLAY,
    /*!����MUSIC����*/
    EVENT_ENTER_MUSIC,
    /*!����LINEIN����*/
    EVENT_ENTER_LINEIN,
    /*!����fm����*/
    EVENT_ENTER_RADIO,
    /*!�����������ã��������ֻ�APK�л�����*/
    EVENT_ENTER_ALARM_RCP,
    /*!����忨����*/
    EVENT_ENTER_MUSIC_CARD,
    /*!����U�̲���*/
    EVENT_ENTER_MUSIC_UDISK,
    /*!����card¼���ļ�����*/
    EVENT_RECORD_CPLAY_RCP,
    /*!����udisk¼���ļ�����*/
    EVENT_RECORD_UPLAY_RCP,
    /*!����mic¼��*/
    EVENT_ENTER_CRECORD_RCP,
    /*!����mic¼��*/
    EVENT_ENTER_URECORD_RCP,
    /*!����USB����*/
    EVENT_ENTER_USOUND,

    /******************************system message****************************/
    MSG_RTC2HZ = MSG_SYS_RTC2HZ, /*0x80*/
    MSG_BAT_V_CHG = MSG_SYS_BAT_V_CHG, /*0x81*/
    MSG_LOW_POWER = MSG_SYS_LOW_POWER, /*0x82*/
    MSG_RTCALARM = MSG_SYS_RTCALARM, /*0x83*/
    MSG_POWER_OFF = MSG_SYS_POWER_OFF, /*0x84*/
    MSG_RECLAIM = MSG_SYS_RECLAIM, /*0x85*/
    MSG_DSP_RESET = MSG_SYS_DSP_RESET, /*0x86*/
    MSG_MPU_ERROR = MSG_SYS_MPU_ERROR, /*0x87*/
    MSG_USB_STICK = MSG_SYS_USB_STICK, /*0x90*/
    MSG_USB_UNSTICK = MSG_SYS_USB_UNSTICK, /*0x91*/
    MSG_ADAPTOR_IN = MSG_SYS_ADAPTOR_IN, /*0xa0*/
    MSG_ADAPTOR_OUT = MSG_SYS_ADAPTOR_OUT, /*0xa1*/
    MSG_SD_IN = MSG_SYS_SD_IN, /*0xb0*/
    MSG_SD_OUT = MSG_SYS_SD_OUT, /*0xb1*/
    MSG_UH_IN = MSG_SYS_UH_IN, /*0xc0*/
    MSG_UH_OUT = MSG_SYS_UH_OUT, /*0xc1*/
    MSG_LINEIN_IN = MSG_SYS_LINEIN_IN, /*0xd0*/
    MSG_LINEIN_OUT = MSG_SYS_LINEIN_OUT, /*0xd1*/
    MSG_HP_IN = MSG_SYS_HP_IN, /*0xe0*/
    MSG_HP_OUT = MSG_SYS_HP_OUT, /*0xe1*/

    /*! ���������Ϣ */
    MSG_FULL_CHARGE = 0xf0,

    /*! ֪ͨӦ�ó����˳���Ϣ��Ӧ�ó�����ܵ�����Ϣ�󣬽����˳����̣�����Ϣֻ���� manager.app ���� */
    MSG_APP_QUIT,

    /*! ����TTS���� */
    MSG_TTS_START_SYNC,
    /*! TTS������� */
    MSG_TTS_STOP_SYNC,

    /*! ��ʼ�л���Ƶ��� */
    MSG_SWITCH_AOUT_START_SYNC,
    /*! ����л���Ƶ��� */
    MSG_SWITCH_AOUT_STOP_SYNC,

    /*! ����DAE���� */
    MSG_SET_DAE_CFG_SYNC,

    //����Ŀ��
    MSG_MUSIC_SONG_NUM_TTS,

    MSG_ASQT_TEST,//ASQT����

    /*! ϵͳ����S2�͹��� */
    MSG_SYSTEM_ENTER_S2,
    /*! �������STANDBY */
    MSG_ENGINE_STANDBY_SYNC,
    /*! ϵͳ����S3�͹��� */
    MSG_SYSTEM_ENTER_S3,
    /*! �������ģʽ */
    MSG_IDLE_MODE,

    /*! ���������Ϣ�� */
    /*! ��绰״̬�仯��Ϣ */
    MSG_BT_HFP_CALL = 0x140,
    /*! �����豸������Ϣ */
    MSG_BT_CONNECTED,
    /*! �����豸�Ͽ�������Ϣ */
    MSG_BT_DISCONNECT,

    /******************************manager message****************************/
    /*! process manager��Ϣռλ��*/
    MANAGER_BASE = 0x180,
    /*! �첽����Ӧ�ó�����Ϣ */
    MSG_CREAT_APP,
    /*! ͬ������Ӧ�ó�����Ϣ */
    MSG_CREAT_APP_SYNC,
    MSG_CREAT_APP_EXT_SYNC,
    /*! �첽ɱ��Ӧ�ó�����Ϣ */
    MSG_KILL_APP,
    /*! ͬ��ɱ��Ӧ�ó�����Ϣ */
    MSG_KILL_APP_SYNC,
    /*! ����stub���� */
    MSG_STUB_INSTALL_SYNC,

    /***************************Music engine message**************************/
    /*! Music engine��Ϣռλ�� */
    MENGINE_BASE = 0x200,
    /*! ��ȡ������Ϣ�����ֲ��Ž��涨ʱ��ȡ������Ϣ */
    MSG_MENGINE_GET_ENGINE_INFO_SYNC,
    /*! ��ȡ��ǰ�ļ�·�� */
    MSG_MENGINE_GET_FILEPATH_SYNC,
    /*! ���ò���ģʽ */
    MSG_MENGINE_SET_PLAYMODE_SYNC,
    /*! ������ʾģʽ */
    MSG_MENGINE_SET_DISPMODE_SYNC,
    /*! ���õ��뵭�� */
    MSG_MENGINE_SET_FADE_SYNC,
    /*! ���ò����ٶ� */
    MSG_MENGINE_SET_PLAYSPEED_SYNC,
    /*! ����AB����ģʽ: �Զ����ֶ� */
    MSG_MENGINE_SET_AB_AUTO_SYNC,
    /*! ����AB�������� */
    MSG_MENGINE_SET_AB_COUNT_SYNC,
    /*! ����AB����ʱ���� */
    MSG_MENGINE_SET_AB_TIME_SYNC,
    /*! ��ʼAB���� */
    MSG_MENGINE_START_AB_SYNC,
    /*! ȡ��AB���� */
    MSG_MENGINE_CLEAR_AB_SYNC,
    /*! ����AB������һ��: A��--> B�� */
    MSG_MENGINE_SET_AB_NEXT_SYNC,
    /*! ����AB������һ��: A��<-- B�� */
    MSG_MENGINE_SET_AB_PREV_SYNC,
    /*! �������� */
    MSG_MENGINE_PLAY_SYNC,
    /*! ֹͣ���� */
    MSG_MENGINE_STOP_SYNC,
    /*! ��ͣ���� */
    MSG_MENGINE_PAUSE_SYNC,
    /*! �ָ����� */
    MSG_MENGINE_RESUME_SYNC,
    /*! ����ָ��ʱ�䲥������ */
    MSG_MENGINE_SEEK_SYNC,
    /*! �������ֲ����ļ�(�ļ�ѡ������ʽ) */
    MSG_MENGINE_SET_FILEPATH_SYNC,
    /*! �������ֲ����ļ�(playlist��ʽ)  */
    MSG_MENGINE_SET_FILEINDEX_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_PLAY_NEXT_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_PLAY_PREV_SYNC,
    /*! ɾ���ļ� */
    MSG_MENGINE_DELETE_FILE_SYNC,
    /*! ��� */
    MSG_MENGINE_FFWD_SYNC,
    /*! ���� */
    MSG_MENGINE_FBWD_SYNC,
    /*! ȡ���������� */
    MSG_MENGINE_CANCEL_FFB_SYNC,
    /*! ���ÿ������˲��� */
    MSG_MENGINE_SET_FFBSTEP_SYNC,
    /*! ���ÿ������˲���ʱ�� */
    MSG_MENGINE_SET_FFBTIME_SYNC,
    /*! ��ȡ��ǰʱ�����ǩ��Ϣ */
    MSG_MENGINE_MAKE_BOOKMARK_SYNC,
    /*! �����ϴβ����ļ� */
    MSG_MENGINE_SET_LAST_FILE_SYNC,
    /*! �������״̬: UIӦ�ô������� */
    MSG_MENGINE_CLEAR_ERR_SYNC,
    /*! ǰ̨�ں���ʱ���������FILESWITCH��־ */
    MSG_MENGINE_CLEAR_FILESWITCH_SYNC,
    /*! ɾ���ղؼУ������ղؼ���ź����� */
    MSG_MENGINE_DEL_FAVORITE_SYNC,
    /*! ����ղؼУ������ղؼ����� */
    MSG_MENGINE_ADD_FAVORITE_SYNC,
    //for audible
    /*! �л�����һ�� */
    MSG_MENGINE_CAHPTER_NEXT_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_CHAPTER_PREV_SYNC,
    /*! ��ȡ��ǰ��ͼƬ��Ϣ */
    MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC,
    //===========music ui ������Ϣ ======================

    /*! ��ȡ��Ϣ */
    MSG_MENGINE_GET_ENGINE_CFG_SYNC,
    /*! ��ȡѭ��ģʽ+shuffle */
    MSG_MENGINE_GET_PLAYMODE_SYNC,
    /*! ��ȡ����ģʽ */
    MSG_MENGINE_GET_ENGINE_PLAYMODE_SYNC,
    /*! ��ȡ�ļ���ʽ */
    MSG_MENGINE_GET_FILEINFO_SYNC,
    /*! ��ȡ��ǰ����״̬ */
    MSG_MENGINE_GET_STATUS_SYNC,
    /*! ��ȡ��ǰ����ʱ��ͱ����� */
    MSG_MENGINE_GET_PLAYINFO_SYNC,
    /*! ��ȡ��ǰ����ʱ��ϵ� */
    MSG_MENGINE_GET_BKINFO_SYNC,
    /*! ��ȡ��ǰ�½����������� */
    MSG_MENGINE_GET_SECTION_INFO_SYNC,
    /*! ����audible�½�ģʽ */
    MSG_MENGINE_SET_SECTION_MODE_SYNC,
    /*! ����AB������� */
    MSG_MENGINE_SET_AB_GAP_SYNC,
    /*! ����AB����A�� */
    MSG_MENGINE_SET_APOINT_SYNC,
    /*! ����AB����B�� */
    MSG_MENGINE_SET_BPOINT_SYNC,
    /*! ������ǩ */
    MSG_MENGINE_PLAY_BOOKMARK_SYNC,
    /*! music���ų���ɾ���ļ� */
    MSG_MENGINE_DELFILE_MUSUI_SYNC,
    /*! music���ų���ɾ���ļ� */
    MSG_MENGINE_DELFILE_OTER_SYNC,
    /*! music�ı��ļ�����·�� */
    MSG_MENGINE_CHANGE_FILEPATH_SYNC,
    /*! music standby���� */
    MSG_MENGINE_STANDBY_PLAY_SYNC,
    /*! �л����̺���Ȳ� */
    MSG_MENGINE_SWITCH_DISK_SYNC,
    /*! ��list��ʽ�л����� */
    MSG_MENGINE_LIST_ACCESS_ALLOW_SYNC,
    /*! ���ֵ�� */
    MSG_MENGINE_DIGITAL_SONG_SYNC,
    /*! folder�л���һ�׸��� */
    MSG_MENGINE_FOLFER_NEXT_SYNC,
    /*! folder�л���һ�׸��� */
    MSG_MENGINE_FOLFER_PREV_SYNC,
    /*! ��ȡ��ǰ����ģʽ */ //0--music,1--sdfile,2--linein
    MSG_MENGINE_GET_PLAYTYPE_SYNC,

    /*! ��ȡid3��Ϣ */
    MSG_MENGINE_GET_ID3INFO_SYNC,
    /*! ��ȡĿ¼����Ϣ */
    MSG_MENGINE_GET_DIRNAME_SYNC,
    /*! ��ȡ�ļ����� */
    MSG_MENGINE_GET_FILENAME_SYNC,

    /*! ������������� */
    MSG_MENGINE_SET_SORTNUMBER_SYNC,

    /*! ��ȡ��ǰ��� */
    MSG_MENGINE_GET_DISKNUMBER_SYNC,
    //����ϵ���Ϣ
    MSG_MENGINE_SAVE_BKINFO_SYNC,
    /*! ����ϵ�����־ */
    MSG_MENGINE_CLR_BKERR_SYNC,
    //SD���ϵ����ϵ�
    MSG_MENGINE_SD_PW_RESET_SYNC,
    MSG_MENGINE_SET_FIO_SYNC,

    /****************************fm engine message****************************/
    /*! fm engine��Ϣռλ��*/
    FMENGINE_BASE = 0x300,

    /* ��ʼ����ж�����*/
    /* ��װFM drv ,  ִ��FM ������̺󼴰�װ*/
    MSG_FMENGINE_OPEN,
    /* FM ģ���*/
    MSG_FM_MODE_OPEN,
    /* FM ģ��ͨ����*/
    MSG_FM_AIN_OPEN,
    /* FM ����IC  ��ʼ������= MSG_FM_MODE_OPEN + MSG_FM_A_CHANNEL_OPEN*/
    MSG_FMENGINE_INIT,
    /* FM ����IC ����Standby ״̬*/
    MSG_FMENGINE_STANDBY,
    /* ���յ��������ж��FM ���������˳�����*/
    MSG_FMENGINE_CLOSE,

    /* �������*/
    /* ����Ƶ�㲥��( �첽) */
    MSG_FMENGINE_SETFREQ,
    /* ����Ƶ�㲥��( ͬ��)  */
    MSG_FMENGINE_SETFREQ_SYNC,
    /* ���þ�����ȡ������*/
    MSG_FMENGINE_SETMUTE,
    /* ���õ�̨����*/
    MSG_FMENGINE_SETBAND,
    /* ������̨����ֵ*/
    MSG_FMENGINE_SETTHRED,
    /* ���������̨*/
    MSG_FMENGINE_SET_SOFTSEEK,
    /* ����Ӳ����̨*/
    MSG_FMENGINE_SET_HARDSEEK,
    /* �˳�Ӳ����̨*/
    MSG_FMENGINE_SET_SEEKBREAK,

    /* ��ȡ��Ϣ���*/
    /* ��ȡӲ���Զ���̨�Ƿ���ɣ���ӦEngine_Status_t.STC_flag */
    MSG_FMENGINE_AUTO_SEEK_OVER,
    /* ��ȡȫ��״̬��Ϣ����ӦEngine_Status_t  �ṹ*/
    MSG_FMENGINE_GET_STATUS,
    /* ��ȡ��ǰ���Σ���ӦEngine_Status_t.FM_CurBand */
    MSG_FMENGINE_GET_BAND,
    /* ��ȡ��ǰƵ�ʣ���ӦEngine_Status_t.FM_CurrentFreq */
    MSG_FMENGINE_GET_FREQ,
    /* ��ȡ��̨�ź�ǿ�ȣ���ӦEngine_Status_t.FM_SignalStg */
    MSG_FMENGINE_GET_INTENSITY,
    /* ��ȡ����״̬( �����Ƿ����) */
    MSG_FMENGINE_GET_ANTENNA,
    /* ��ȡ������״̬��Ϣ����ӦEngine_Status_t.FM_Stereo_Status */
    MSG_FMENGINE_GET_STEREO,
    /* ��ȡ���沥��״̬��Ϣ����ӦEngine_Status_t.FM_VolStatus */
    MSG_FMENGINE_GET_PLAYSTATUS,
    /* ��ȡ��ǰģ��״̬*/
    MSG_FMENGINE_GET_MODUAL_STATUS,
    /* �ؽ�FMģ��ȫ����������*/
    MSG_FMENGINE_RESINTALL,
    /*FM play*/
    MSG_FMENGINE_PLAY,
    /*FM pause*/
    MSG_FMENGINE_PAUSE,
    //������
    MSG_FMENGINE_DEBUG,

    /****************************autotest message****************************/
    /*! auto test��Ϣռλ�� */
    AUTOTEST_BASE = 0x400,
    MSG_AUTOTEST_CREAT_APP_EXT_SYNC,
    MSG_AUTOTEST_SET_PAIRED_DEV_SYNC,
    MSG_AUTOTEST_GET_BTSTACK_STATUS_SYNC,
    MSG_AUTOTEST_CLEAR_BTSTACK_ERR_SYNC,
    MSG_AUTOTEST_HFP_CONNECT_SYNC,
    MSG_AUTOTEST_A2DP_CONNECT_SYNC,
    MSG_AUTOTEST_QUERY_BTSTACK_WORK_SYNC,
    MSG_AUTOTEST_SET_SHARE_DATA_SYNC,
    MSG_AUTOTEST_BTT_TEST_SYNC,
    MSG_AUTOTEST_SWITCH_APP,
    MSG_AUTOTEST_SET_PAIRED_DEV2_SYNC,
    MSG_AUTOTEST_GET_PAIRED_DEV2_SYNC,
    /****************************linein message****************************/
    //LINEIN�������Ϣ
    LINEIN_EG_BASE = 0x500,
    //�õ�״̬
    MSG_LINEIN_EG_GET_STATUS_SYNC,
    //����
    MSG_LINEIN_EG_PLAY_SYNC,
    //ֹͣ
    MSG_LINEIN_EG_STOP_SYNC,
     //salve now linein
    MSG_LINEIN_IN_STATUS_SYNC,

    /****************************** BTSTACK ******************************/
    /****************************** BTSTACK A2DP/AVRCP ******************************/
    /*ע�⣺Ϊ�˸��ý���bank���֣�Ҫ������A2DP/AVRCP��ص���Ϣ���������[0x600-0x6ff]*/
    MSG_BTSTACK_AVRCP_BASE = 0x600,
    MSG_BTSTACK_AVRCP_TX_OPID_SYNC = 0x601,
    MSG_BTSTACK_AVRCP_VOLUME_SYNC = 0x602,
    MSG_BTSTACK_A2DP_CONNECT_SYNC = 0x603,
    MSG_BTSTACK_A2DP_DISCONNECT_SYNC = 0x604,
    MSG_BTSTACK_AVRCP_CONNECT_SYNC = 0x605,
    MSG_BTSTACK_AVRCP_DISCONNECT_SYNC = 0x606,
#ifdef __BQB_TEST_PROFILE_
    MSG_BTSTACK_DELAY_REPORT_SYNC = 0x607,
#endif
    /****************************** BTSTACK HFP ******************************/
    /*ע�⣺Ϊ�˸��ý���bank���֣�Ҫ������HFP��ص���Ϣ���������[0x700-0x7ff]*/
    MSG_BTSTACK_HFP_BASE = 0x700,
    MSG_BTSTACK_HFP_CALL_REQ_SYNC = 0x701,
    MSG_BTSTACK_HFP_VOLUME_SYNC = 0x702,
    MSG_BTSTACK_HFP_BATTERY_SYNC = 0x703,
    MSG_BTSTACK_HFP_SETSYN_SYNC = 0x704,
    MSG_BTSTACK_HFP_RELEASESYN_SYNC = 0x705,
    MSG_BTSTACK_HFP_TX_DTMF_CODE_SYNC = 0x706,
    MSG_BTSTACK_HFP_DIAL_PHONENUM_SYNC = 0x707,
    MSG_BTSTACK_HFP_CALL_LAST_SYNC = 0x708,
    MSG_BTSTACK_HFP_CONNECT_SYNC = 0x709,
    MSG_BTSTACK_HFP_DISCONNECT_SYNC = 0x70a,
    MSG_BTSTACK_HFP_SET_SIRI_SYNC = 0x70b,
    MSG_BTSTACK_HFP_SCO_CHANGE_SYNC = 0x70c,

    /*3��ͨ������*/
    MSG_BTENGINE_3WAY_HANDLE_SYNC = 0x720,
    MSG_BTENGINE_3WAY_GET_STATUS_SYNC = 0x721,
	
	
    /****************************** BTSTACK OTHER ******************************/
    MSG_BTSTACK_OTHER_BASE = 0x800,
    MSG_BTSTACK_SET_DEVICE_NAME_SYNC = 0x801,
    MSG_BTSTACK_GET_DEVICE_INFO_SYNC = 0x802,
    MSG_BTSTACK_SET_DISCOVERABLE_SYNC = 0x803,
    MSG_BTSTACK_BQB_TEST_SYNC = 0x804,
    MSG_BTSTACK_GET_PAIRED_LIST_SYNC = 0x805,
    MSG_BTSTACK_FORCE_UNLINK_SYNC = 0x806,
    MSG_BTSTACK_RCP_SEND_SYNC = 0x807,
    MSG_BTSTACK_SET_A2DP_ACTIVE_DEV_SYNC = 0x808,
    MSG_BTSTACK_SET_HFP_ACTIVE_DEV_SYNC = 0x809,
    MSG_BTSTACK_SNIFF_CONTROL_SYNC = 0x80a,
    MSG_BTSTACK_SPP_DISCONNECT_SYNC = 0x80b,
    MSG_BTSTACK_BLE_DISCONNECT_SYNC = 0x80c,
    MSG_BTSTACK_GET_PB_CONTENT_SYNC = 0x80d,
    MSG_BTSTACK_PLAY_PIPE_CTRL_SYNC = 0x80e,
    MSG_BTSTACK_CALL_PIPE_CTRL_SYNC = 0x80f,
    MSG_BTSTACK_SET_BLE_DISCOVERABLE_SYNC = 0x810,
    MSG_BTSTACK_ENTER_BTT_TEST_SYNC = 0x811,
    MSG_BTSTACK_GET_BTSTACK_VER_SYNC = 0x812,
    MSG_BTSTACK_GET_BTSTACK_CONN_INIT_SYNC=0x813,
    MSG_BTSTACK_DECODE_QUIT_SYNC=0x814,
    MSG_BTSTACK_SET_PAGEABLE_SYNC = 0x815,
    MSG_BTSTACK_AG_UNLINK_SYNC = 0x816,
    //for test client 
    MSG_BTSTACK_SET_BLE_WORK_MODE = 0x820,
    //for profile qulification
    MSG_BTSTACK_SET_CTS_NOTIFY = 0x821,    
    //for test client disconect
    MSG_BTSTACK_SET_BLE_DISCONECT = 0x822,
     //for test client discovery
    MSG_BTSTACK_SET_BLE_AUTOCACHE = 0x823,
         //for test client discovery
    MSG_BTSTACK_SET_BLE_READVAL = 0x824,
    //for test tip security
    MSG_BTSTACK_SET_BLE_SECURITY = 0x825,
    //for test fms start alart
    MSG_BTSTACK_SET_BLE_ALART_1 = 0x826,
    //for test fms stop alart
    MSG_BTSTACK_SET_BLE_ALART_0 = 0x827,
    
    //for test hrs notify
    MSG_BTSTACK_SET_BLE_HRS_NOTI = 0x828,
    
    //for test hts notify
    MSG_BTSTACK_SET_BLE_HTS_NOTI = 0x829,
    
    //for hid test 
    MSG_BTSTACK_SET_LM_DISCOVERABLE_SYNC = 0x82a,
    
    //for fast scan br/edr
    MSG_BTSTACK_SET_SCAN_MODE = 0x82b,
    //for force unlink one 
    MSG_BTSTACK_FORCE_DISCONN = 0x82c,
    
    /****************************** BTSTACK TWS ******************************/
    MSG_BTSTACK_TWS_BASE = 0x8a0,
    MSG_BTSTACK_TWS_START_PLAY_SYNC = 0x8a1,
    MSG_BTSTACK_TWS_FORCE_UNLINK_SYNC = 0x8a2,
    MSG_BTSTACK_TWS_CLEAR_ACTIVE_ID=0x8a3,
    MSG_BTSTACK_TWS_SEND_MSG_SYNC=0x8a4,
    MSG_BTSTACK_TWS_SEND_PHONCON_SYNC=0x8a5,
    MSG_BTSTACK_TWS_SUB_MODE_FLAG=0x8a6,
    MSG_BTSTACK_TWS_POWER_OFF=0x8a7,
    MSG_BTSTACK_GET_TWS_PAIRED_LIST_SYNC = 0x8a8,
    MSG_BTSTACK_TWS_VOLUME_SYNC=0x8a9,
    MSG_BTSTACK_TWS_DEAL_MSG_SYNC = 0x8aa,
    MSG_BTSTACK_TWS_UPDATE_PARM = 0x8ab,
    MSG_BTSTACK_TWS_SYNC_CMD = 0x8ac,
    MSG_BTSTACK_CLEAR_TWSPAIR_LIST = 0x8ad,
    MSG_BTSTACK_TWS_FILTER_CTRL_SYNC = 0x8ae,
    MSG_BTSTACK_TWS_SET_START_TIME = 0x8af,
    MSG_BTSTACK_TWS_LINEIN_EXT = 0x8b0,
   
    /****************************** BTSTACK EVENT ******************************/
    MSG_BTSTACK_EVENT_BASE = 0x900,
    MSG_BTSTACK_AVRCP_RX_OPID = 0x901,
    MSG_BTSTACK_AVRCP_VOLUME_ABS = 0x902,
    MSG_BTSTACK_HFP_VOLUME_ABS = 0x903,
    MSG_BTSTACK_ERR_CONNECTION_TIMEOUT = 0x904,
    MSG_BTSTACK_SPPBLE_RCP_PKG = 0x905,
    MSG_BTSTACK_PATCH_DOWNLOAD_FLAG = 0x906,
    MSG_BTSTACK_ERR_HARDWARE_EXCEPTION = 0x907,
    
    MSG_BTSTACK_POWER_OFF=0x908,
    MSG_BTSTACK_ERR_HARDWARE = 0x909,
    MSG_BTSTACK_TWS_APSWITCH_SYNC = 0x90a,
    MSG_BTSTACK_PARSE_GENARAL_CMD=0x90b,
    

    /****************************** BTPLAY ENGINE ******************************/
    MSG_BTPLAYEG_BASE = 0xa00,

    /****************************** BTCALL ENGINE ******************************/
    MSG_BTCALLEG_BASE = 0xb00,
    MSG_BTCALLEG_MIC_MUTE_SYNC, //����Զ�˱�
    MSG_BTCALLEG_MIC_RELEASEMUTE_SYNC,//��Զ�˾���
    MSG_BTCALLEG_GET_EG_VOLUM_SYNC, //��ѯ��̨������-asqt����
    MSG_BTCALLEG_GET_EG_PA_VOLUM_SYNC, //��ѯ��̨��PA����-asqt����



    /****************************** USOUN ENGINE ******************************/
    MSG_UENGINE_BASE = 0xc00,
    /*!������ͣ*/
    MSG_UENGINE_PLAY_PAUSE_SYNC,
    /*!�л���һ��*/
    MSG_UENGINE_NEXT_SONG_SYNC,
    /*!�л���һ��*/
    MSG_UENGINE_PREV_SONG_SYNC,
    /*!��ȡ״̬*/
    MSG_UENGINE_GET_STATUS_SYNC,
    /*!������*/
    MSG_UENGINE_VOL_ADD_SYNC,
    /*!������*/
    MSG_UENGINE_VOL_SUB_SYNC,
    /*!����ֵͬ��*/
    MSG_UENGINE_VOL_SEND_SYNC,
    
    /****************************** HID ******************************/
    MSG_BTSTACK_HID_BASE                = 0xd00,
    MSG_BTSTACK_HID_CONNECT_SYNC        = 0xd01, 
    MSG_BTSTACK_HID_SEND_SYNC           = 0xd02,
    MSG_BTSTACK_HID_DISCONNECT_SYNC     = 0xd03,
	

} msg_apps_type_e;

#define SYSMSG_STOP_TTS_OPTION_MASK  (1 << 0)
#define SYSMSG_STOP_TTS_NO           (0 << 0)
#define SYSMSG_STOP_TTS_YES          (1 << 0)

typedef struct
{
    /*! ϵͳ��Ϣ���� */
    msg_apps_type_e msg_type;
    /*! ϵͳ��Ϣ����ѡ�� */
    uint8 msg_option;
} sys_event_t;

/*����Ӧ�ô��ݽṹ�壬������bt_stack*/
typedef struct
{
    uint8 app_id;
    uint8 byte_param[3];
    void *data;
} create_app_param_t;

/*!
 * \brief
 *  sys_msg_t ϵͳ��Ϣ�ṹ��
 */
typedef struct
{
    /*! Ӧ����Ϣ���� */
    msg_apps_type_e type;
} sys_msg_t;

/*!
 * \brief
 *  msg_apps_t Ӧ����Ϣ�ṹ��
 */
typedef struct
{
    /*! Ӧ����Ϣ���ͣ��μ� msg_apps_type_e ���� */
    uint32 type;
    /*! Ӧ����Ϣ���� */
    union
    {
        /*! ��Ϣ������ʵ���� */
        uint8 data[4];
        /*! ��Ϣ���ݻ�����ָ�룬ָ����Ϣ���ͷ��ĵ�ַ�ռ� */
        void *addr;
    } content;
} msg_apps_t;

/*!
 * \brief
 *  msg_reply_type_e ͬ����Ϣ��ִö������
 */
typedef enum
{
    /*! ͬ����Ϣ�޻ظ�������Ӧ���˳�ʱӦ����δ�����ͬ����Ϣ */
    MSG_REPLY_NO_REPLY,
    /*! �ظ�ͬ����Ϣ�����ߣ�ͬ����ϢҪ���ܳɹ���� */
    MSG_REPLY_SUCCESS,
    /*! �ظ�ͬ����Ϣ�����ߣ�ͬ����ϢҪ�������ʧ�� */
    MSG_REPLY_FAILED,
} msg_reply_type_e;

/*!
 * \brief
 *  msg_reply_t ͬ����Ϣ��ִ�ṹ��
 */
typedef struct
{
    /*! ͬ����Ϣ��ִö�����ͣ��μ� msg_reply_type_e ���� */
    uint8 type;
    uint8 reserve[3];
    /*! ��ִ���ݻ�����ָ�룬ָ����Ϣ���ͷ��ĵ�ַ�ռ� */
    void *content;
} msg_reply_t;

/*!
 * \brief
 *  private_msg_t ��Ӧ�ü䣩˽����Ϣ�ṹ��
 */
typedef struct
{
    /*! ˽����Ϣ��Ϣ���� */
    msg_apps_t msg;
    /*! ͬ���ź��� */
    os_event_t *sem;
    /*! ͬ����Ϣ��ִָ�� */
    msg_reply_t *reply;
} private_msg_t;

#define BD_ADDR_LEN  6

/*!
 * \brief
 *  btstack_msg_t BT MANAGER����BT STACK����Ϣ�ṹ��
 */
typedef struct
{
    /*! ˽����Ϣ��Ϣ���� */
    msg_apps_t msg;
    /*! ͬ���ź��� */
    os_event_t *sem;
    /*! ͬ����Ϣ��ִָ�� */
    msg_reply_t *reply;
    /*! �����ֽ� */
    uint8 reserve[2];
    /*! ���������豸��ַ���������������Ҫ�豸��ַ */
    uint8 bd_addr[BD_ADDR_LEN];
} btstack_msg_t;

typedef struct
{
    /*! �¼����� */
    msg_apps_t msg;
    /*! �����ֽ� */
    uint8 reserve[2];
    /*! �¼�������豸��ַ�����������¼������豸��ַ */
    uint8 bd_addr[BD_ADDR_LEN];
} btstack_event_t;

/*!
 * \brief
 *  app_msg_hook ǰ̨Ӧ��˽����Ϣ������ָ��
 */
typedef void (*app_msg_hook)(private_msg_t *private_msg);

/*!
 *  \brief
 *  g_this_app_msg_hook ������Ϣ���⴦������
 */
extern app_msg_hook g_this_app_msg_hook;

/*!
 *  \brief
 *  g_buffer_app_msg ����ϵͳ��Ϣ
 */
extern private_msg_t g_buffer_app_msg;

//��Ϣͨ�Ź���ģ��ӿ�����
//������Ϣ�ӿ�
//bool get_sys_msg(sys_msg_t *sys_msg);���ڲ��ӿڣ���get_app_msg���ã�
bool get_app_msg(private_msg_t *private_msg) __FAR__;
bool get_app_msg_btstack(btstack_msg_t *btstack_msg) __FAR__;

//������Ϣ�ӿ�
#define ERR_NO_ERR         0
#define ERR_TARGET_NOEXIST -1
#define ERR_MSGQUEUE_FULL  -2
#define ERR_SYNC_ITSELF    -3
#define ERR_SYNC_TIMEOUT   -4
int send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout) __FAR__;
int send_async_msg(uint8 app_id, msg_apps_t *msg) __FAR__;
bool broadcast_msg(msg_apps_t *msg, bool only_front) __FAR__;

//BTר��
int send_sync_msg_btmanager(uint8 *bd_addr, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout) __FAR__;
int send_async_msg_btmanager(uint8 *bd_addr, msg_apps_t *msg) __FAR__;
int send_async_event(uint8 app_id, btstack_event_t *ev) __FAR__;

//��Ϣͨ�Ź���ģ���ʼ���ӿ�
bool applib_message_init(void) __FAR__;

#endif
