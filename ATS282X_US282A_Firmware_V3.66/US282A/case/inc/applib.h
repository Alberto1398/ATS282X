/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������APPLIB ģ��궨�壬�ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _applib_H_
#define _applib_H_

#include "psp_includes.h"
#include "case_independent.h"
#include "case_type.h"
#include "led_driver.h"
#include "key_interface.h"

#include "gui_msg.h"
#include "app_msg.h"
#include "app_timer.h"
#include "common_config.h"

/*! �������֧�� 4 ��Ӧ��ͬʱ���� */
#define MAX_APP_COUNT           4

/*! ��֤APK/APP״̬*/
#define VERIFICATION_NULL       0x00
#define VERIFICATION_START      0x01
#define VERIFICATION_PASS       0x02
#define VERIFICATION_FAIL       0x03

//��Ҫ����ɨ��U������������ļ�
#define UHOST_NEED_SCAN     (1 << 4)
//��Ҫ�ϱ�U��������ļ��б��app/apk
#define UHOST_NEED_UPLOAD   (1 << 5)
#define UHOST_NEED_MASK     (UHOST_NEED_SCAN | UHOST_NEED_UPLOAD)

//��Ҫ����ɨ�迨����������ļ�
#define CARD_NEED_SCAN     (1 << 0)
//��Ҫ�ϱ���������ļ��б��app/apk
#define CARD_NEED_UPLOAD   (1 << 1)
#define CARD_NEED_MASK     (CARD_NEED_SCAN | CARD_NEED_UPLOAD)

/*! ϵͳ����״̬�����ݸ���״̬�ܽ������ֻ�� sys_counter ��ʹ�� */
typedef enum
{
    SYS_STATUS_IDLE      = 0,
    SYS_STATUS_WAIT_LINK = 1,
    SYS_STATUS_WORKING   = 2,
} sys_status_e;

#define UI_IDLE_TIMER    14 //����״̬����7���Ӿ͵���Ƶ��
/*! ǰ̨�û�����״̬ */
typedef enum
{
    UI_STATUS_IDLE         = 0, //����״̬����7���Ӻ�͵���Ƶ��
    UI_STATUS_IDLE_LOWFREQ = 1, //�������ѵ���Ƶ��״̬
    UI_STATUS_BUSY         = 2, //��ʾ���ڽ��г�ʼ�����и�ȶ�����Ҫ���ٵ�����
} ui_status_e;

/*!
 * \brief
 *  app_type_e Ӧ�ã����̣�����ö�����ͣ���Ӧ g_app_info_vector ����
 */
typedef enum
{
    /*! ���̹����� */
    APP_TYPE_PMNG = 0,
    /*! ǰ̨Ӧ�ã�gui���̣� */
    APP_TYPE_GUI = 1,
    /*! ��̨Ӧ�ã�console���̣� */
    APP_TYPE_CONSOLE = 2,
    /*! ����Э��ջ */
    APP_TYPE_BTSTACK = 3,
} app_type_e;

/*!
 * \brief
 *  app_info_t Ӧ����Ϣ�ṹ��
 */
typedef struct
{
    /*! �ṹ��ʹ�ñ�־��1��ʾ�ѱ�ʹ�ã�0��ʾδ��ʹ�� */
    uint8 used;
    /*! ����ID�ţ��� case_type.h �ж�����б� */
    uint8 app_id;
    /*! Ӧ�����ͣ���app_type_e ���� */
    uint8 app_type;
    /*! ����˽����Ϣ����ID����mq_id_e ���� */
    uint8 mq_id;
} app_info_t;

/*!
 * \brief
 *  engine_type_e ��������ö�����ͣ�����Ӧ���û������̨��������
 */
typedef enum
{
    /*! û������ */
    ENGINE_NULL = 0,
    /*! music���� */
    ENGINE_MUSIC = 1,
    /*! fm���� */
    ENGINE_RADIO = 2,
    /*! LINEIN���� */
    ENGINE_LINEIN = 3,
    /*! �����Ƹ����� */
    ENGINE_BTPLAY = 4,
    /*! ������绰���� */
    ENGINE_BTCALL = 5,
    /*! usb�������� */
    ENGINE_USOUND = 6,
} engine_type_e;

/*!
 * \brief
 *  engine_state_e ����״̬ö������
 */
typedef enum
{
    /*! û������ */
    ENGINE_STATE_NULL = 0,
    /*! �������ڲ��� */
    ENGINE_STATE_PLAYING = 1,
    /*! ���洦����ͣ״̬ */
    ENGINE_STATE_PAUSE = 2,
} engine_state_e;

/*!
 * \brief
 *  app_state_e ǰ̨Ӧ�ù���״̬ö������
 */
typedef enum
{
    /*! ǰ̨AP���ڷǲ���״̬ */
    APP_STATE_NO_PLAY = 0,
    /*! ǰ̨AP���ڲ���״̬ */
    APP_STATE_PLAYING = 1,
    /*! ǰ̨AP���ڲ���״̬����ϣ�����ⳣ�� */
    APP_STATE_PLAYING_ALWAYS = 2,
} app_state_e;

/*!
 * \brief
 *  card_state_e ��״̬ö������
 */
typedef enum
{
    /*! û�� */
    CARD_STATE_CARD_NULL = 0,
    /*! �п���û���� */
    CARD_STATE_CARD_IN = 1,
    /*! ���ڲ���UHOST����ʱ��ʹ�� */
    CARD_STATE_CARD_PLAY = 2,
} card_state_e;

/*!
 * \brief
 *  uhost_state_e uhost״̬ö������
 */
typedef enum
{
    /*! ûUHOST */
    UHOST_STATE_UHOST_NULL = 0,
    /*! ��UHOST��û���� */
    UHOST_STATE_UHOST_IN = 1,
    /*! ���ڲ���UHOST����ʱ��ʹ�� */
    UHOST_STATE_UHOST_PLAY = 2,
} uhost_state_e;

/*!
 * \brief
 *  linein_state_e linein״̬ö������
 */
typedef enum
{
    /*! ûlinein */
    LINEIN_STATE_LINEIN_NULL = 0,
    /*! ��linein��û���� */
    LINEIN_STATE_LINEIN_IN = 1,
    /*! ���ڲ���linein����ʱ��ʹ�� */
    LINEIN_STATE_LINEIN_PLAY = 2,
} linein_state_e;

/*!
 * \brief
 *  play_disk_type_e ���Ŵ�������ö������
 */
typedef enum
{
    /*! ���� */
    PLAY_DISK_NULL = 0,
    /*! UHOST���� */
    PLAY_DISK_UHOST = 1,
    /*! CARD���� */
    PLAY_DISK_CARD = 2,
    /*! SD������ */
    PLAY_DISK_SDFILE = 3,
    /*! LINEIN���� */
    PLAY_DISK_LINEIN = 4,
    /*! ¼������ */
    PLAY_RECORD_FILE = 5,
} play_disk_type_e;

/*!
 * \brief
 *  cable_state_e ����״̬ö������
 */
typedef enum
{
    /*! û�� USB �� */
    CABLE_STATE_CABLE_NULL = 0,
    /*! �в� USB �� */
    CABLE_STATE_CABLE_IN = 1,
    /*! �в� ADAPTOR �� */
    CABLE_STATE_ADAPTOR_IN = 2,
} cable_state_e;

typedef enum
{
    /*! û����������ߣ� */
    HEADPHONE_STATE_NULL = 0,
    /*! �в���������ߣ� */
    HEADPHONE_STATE_IN = 1,
} headphone_state_e;

/*!
 * \brief
 *  backlight_state_e ����״̬ö������
 */
typedef enum
{
    /*! ��������״̬������ */
    BACKLIGHT_STATE_NORMAL = 0,
    /*! ����䰵״̬������ */
    BACKLIGHT_STATE_DARK = 1,
    /*! ����ر�״̬������ */
    BACKLIGHT_STATE_OFF = 2,
} backlight_state_e;

/*!
 * \brief
 *  soundout_state_e �������״̬ö������
 */
typedef enum
{
    /*! ֹͣ����������ص�pa */
    SOUND_OUT_STOP = 0,
    /*! ��ʼ�������������pa */
    SOUND_OUT_START = 1,
    /*! ��ͣ�������������pa */
    SOUND_OUT_PAUSE = 2,
    /*! �ָ��������������pa */
    SOUND_OUT_RESUME = 3,
} soundout_state_e;

/*!
 * \brief
 *  apk_status_e APK״̬ö������
 */
typedef enum
{
    /*APK��δ���ӣ�APK�˳�ʱ����*/
    APK_UNLINK = 0,
    /*APK���ڲ�����5���ӷ���һ�Σ�BUSY��һ�������FRONTGROUND��������֮���ΪFRONTGROUND*/
    APK_BUSY = 1,
    /*APK�Ӻ�̨�л���ǰ̨*/
    APK_FRONTGROUND = 2,
    /*APK��ǰ̨�л�����̨*/
    APK_BACKGROUND = 3,
} apk_status_e;

/*!
 * \brief
 *  app_info_state_all_t ϵͳ��ǰ״̬�ṹ�壬����Ӧ�ó�����Ҫ����
 */
typedef struct
{
    /*! ��ǰ���е��������ͣ��� engine_type_e ���� */
    uint8 engine_type;
    /*! ���浱ǰ״̬�����ڲ��Ż�����ͣ���� engine_state_e ���� */
    uint8 engine_state;
    /*! ǰ̨Ӧ�ù���״̬�������źͲ��ţ��� app_state_e ���� */
    uint8 app_state;
    /*! ǰ̨UI����״̬����� ui_status_e */
    uint8 ui_status;

    /*-- ����4����ԱBT STACK��APPLIB����Ҫ���ʣ��������޸� --*/

    /*!��tts������ж������*/
    uint8 g_no_support_tts_flag;
    uint8 g_send_exit_mmm_tts;
    uint8 power_on_flag:4;
    uint8 stub_mode_flag:4;

    /*! ��Ƶ���״̬���� soundout_state_e */
    uint8 sound_state;
    
    uint8 fix_sample_rate_flag;
    
    /*! ��������ͬ��ȫ�ֱ��� */
    uint8 battery_report_enable;

    /*! ����������ȫ�ֱ��� */
    bool energy_available_flag;
    short *p_energy_value;
    
#if (SUPPORT_MULTI_FREQ_MULTI_BAND == 1)    
    short *p_energy_value_bands;
    short *p_energy_value_freqs;
#endif
    //�ñ�����;:ʹ��WAVES��Чʱ��Ϊ�����м��֪����ǰ�ǵ���ģʽ����normalģʽ���ӵ�
    uint8 stub_pc_tools_type;
    uint8 stub_phy_type;  //0:USB 1:UART

    uint8 bin_number:3;   //WAVES��Чnormalģʽ��ʹ�õڼ�����Ч����  
    uint8 waves_dae_para_update_flag:1;///WAVES��Чnormalģʽ�£�������Ч�������־
    uint8 reserve:4; 
    
} app_info_state_all_t;

/*!
 * \brief
 *  app_info_state_t ϵͳ��ǰ״̬�ṹ�壬ֻ��ǰ̨Ӧ����Ҫ����
 */
typedef struct
{
    /*! ����״̬ */
    bool mute_state;
    /*! ��/�ⲿPA */
    bool pa_mode;    
    /*! ��Ƶ�����ʽ */
    uint8 aout_mode;
    /*! ����״̬�����������͹أ��� backlight_state_e ���� */
    uint8 backlight_state;
    /*! ��״̬��û�����п����п��Ҳ��ţ��� card_state_e ���� */
    uint8 card_state;
    /*! UHOST״̬��ûUHOST����UHOST����UHOST�Ҳ��ţ��� uhost_state_e ���� */
    uint8 uhost_state;
    /*! linein״̬��ûlinein����linein����linein�Ҳ��ţ��� linein_state_e ���� */
    uint8 linein_state;
    /*! ����״̬��û�� USB �ߣ��в� USB �ߣ��� cable_state_e ���� */
    uint8 cable_state;
    /*! ���������ߣ�״̬ */
    bool hp_state;
    /*! ���״̬���� battery_status_e ���� */
    uint8 charge_state;
    /*! ��ص�����ȡֵ 0 ~ 9 */
    uint8 bat_value;
    /*! �Ƿ����������У�����ǽ����Բ���ϵͳ��Ϣ */
    bool ring_alarm;
    /*! stub�������� */
    uint8 stub_tools_type:7;
    uint8 stub_phy_type:1;  //0:USB 1:UART
    /*! auto test mode */
    uint8 autotest_mode;

    /*! BT patch download flag */
    bool patch_download_flag;
    /*! BT CONTROLLER POWER ON/OFF */
    bool bt_power_on;
    /*! �ڲ�PA IC��ʼ���Ƿ���ɣ���ɺ���ܲ������� */
    bool inner_pa_inited;
    /*! �ⲿPA ON/OFF ״̬ */
    bool outer_pa_on;
    /*!��ǰӦ�ù������,����ap_switch_info�����е��±�*/
    uint8 cur_func_index;
    /*! ��ǰӦ�ù���ʵ�����*/
    uint8 cur_func_id;
    /*! ��ǰ�ֻ�APK״̬���� apk_status_e ���� */
    uint8 apk_status;
    /*! ��֤APK/APP״̬�� */
    uint8 verification_status;
    /*! ǿ��APK���²����б�. �ο�CARD_NEED_SCAN�ȵ�˵�� */
    uint8 need_update_playlist;
    /*! �ⲿ��緽���ĵ�ؿ��ر�־ */
    uint8 extern_bat_in;
} app_info_state_t;

/*!
 * \brief
 *  app_last_state_t ϵͳ���״̬�ṹ�壬�����Զ����״̬�仯������
 */
typedef struct
{
    /*! ������Ŵ������ͣ��� play_disk_type_e ���� */
    uint8 last_disk;
    /*! ���Ӧ�ù������*/
    uint8 last_func_index;
    /*! ���ϵͳ����״̬���� sys_status_e ���� */
    uint8 last_sys_status;
    /*! ���ǰ̨UI����״̬���� ui_status_e ���� */
    uint8 last_ui_status;
} app_last_state_t;

/*!
 *  \brief
 *  g_app_info_vector ȫ��Ӧ����Ϣ�ṹ�����飬�����ϵͳȫ�ֿռ�
 *  NOTE!!! ����ȷ�� app_info_t �� g_app_info_vector ���������ı�
 */
extern app_info_t g_app_info_vector[MAX_APP_COUNT];

/*!
 *  \brief
 *  config_fp ��ȫ�֣�Ӧ�����ýű��ļ�����������ϵͳȫ�ֿռ�
 */
extern sd_handle config_fp;

/*!
 *  \brief
 *  g_app_info_state_all ȫ��ϵͳ��ǰ״̬�ṹ�壬�����ϵͳȫ�ֿռ�
 *  NOTE!!! ����ȷ�� app_info_state_all_t ǰ�沿�ֳ�Ա����
 */
extern app_info_state_all_t g_app_info_state_all;

/*!
 *  \brief
 *  g_app_last_state ȫ��ϵͳ���״̬�ṹ��
 */
extern app_last_state_t g_app_last_state;

/*!
 *  \brief
 *  g_app_info_state ȫ��ϵͳ��ǰ״̬�ṹ��
 */
extern app_info_state_t g_app_info_state;

/*!
 *  \brief
 *  g_this_app_info ��ǰӦ����Ϣ�ṹ��ָ��
 */
extern const app_info_t *g_this_app_info;

/*!
 *  \brief
 *  thread_mutex �̻߳����ź���ָ��
 */
extern os_event_t *thread_mutex;

extern uint32 g_customer_state;

///��ȡ��̨��������
static inline engine_type_e get_engine_type(void)
{
    return g_app_info_state_all.engine_type;
}

///�޸ĺ�̨��������
static inline void change_engine_type(engine_type_e engine_type)
{
    g_app_info_state_all.engine_type = engine_type;
}

///��ȡ��̨����״̬
static inline engine_state_e get_engine_state(void)
{
    return g_app_info_state_all.engine_state;
}

///�޸ĺ�̨����״̬
static inline void change_engine_state(engine_state_e engine_state)
{
    g_app_info_state_all.engine_state = engine_state;
}

///��ȡ��Ƶ���״̬����soundout_state_e
static inline soundout_state_e get_soundout_state(void)
{
    return g_app_info_state_all.sound_state;
}

///�޸���Ƶ���״̬����soundout_state_e
static inline void change_soundout_state(soundout_state_e sound_state)
{
    g_app_info_state_all.sound_state = sound_state;
}

///��ȡ�������TRUE��ʾ������FALSE��ʾ�Ǿ���
static inline bool get_mute_enable(void)
{
    return g_app_info_state.mute_state;
}

///�޸ľ������TRUE��ʾ������FALSE��ʾ�Ǿ���
static inline void change_mute_enable(bool mute)
{
    g_app_info_state.mute_state = mute;
}

///�޸�ǰ̨UI����״̬
static inline void change_ui_status(ui_status_e ui_status)
{
    g_app_info_state_all.ui_status = ui_status;
}

///��ȡ����״̬��TRUE��ʾ��������FALSE��ʾ������
static inline backlight_state_e get_backlight_state(void)
{
    return g_app_info_state.backlight_state;
}

///�޸ı���״̬
static inline void change_backlight_state(backlight_state_e state)
{
    g_app_info_state.backlight_state = state;
}

///��ȡǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI
static inline app_state_e get_app_state(void)
{
    return g_app_info_state_all.app_state;
}

///�޸�ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI
static inline void change_app_state(app_state_e state)
{
    g_app_info_state_all.app_state = state;
}

///��ȡ��ʱ��������־
static inline uint8 get_app_timer_tag(void)
{
    return g_this_app_timer_tag;
}

///�޸���ʱ��������־
static inline void change_app_timer_tag(uint8 tag)
{
    g_this_app_timer_tag = tag;
}

///��ȡϵͳ��״̬
static inline card_state_e get_card_state(void)
{
    return g_app_info_state.card_state;
}

///�޸�ϵͳ��״̬
static inline void change_card_state(card_state_e card_state)
{
    g_app_info_state.card_state = card_state;
}

///��ȡϵͳUHOST״̬
static inline uhost_state_e get_uhost_state(void)
{
    return g_app_info_state.uhost_state;
}

///�޸�ϵͳUHOST״̬
static inline void change_uhost_state(uhost_state_e uhost_state)
{
    g_app_info_state.uhost_state = uhost_state;
}

///��ȡϵͳLINEIN״̬
static inline linein_state_e get_linein_state(void)
{
    return g_app_info_state.linein_state;
}

///�޸�ϵͳLINEIN״̬
static inline void change_linein_state(linein_state_e linein_state)
{
    g_app_info_state.linein_state = linein_state;
}

///��ȡϵͳ USB ����״̬
static inline cable_state_e get_cable_state(void)
{
    return g_app_info_state.cable_state;
}

///�޸�ϵͳ USB ����״̬
static inline void change_cable_state(cable_state_e cable_state)
{
    g_app_info_state.cable_state = cable_state;
}

///��ȡ���������ߣ�״̬
static inline headphone_state_e get_headphone_state(void)
{
    return g_app_info_state.hp_state;
}

///�޸Ķ��������ߣ�״̬
static inline void change_headphone_state(headphone_state_e hp_state)
{
    g_app_info_state.hp_state = hp_state;
}

///��ȡ���״̬
static inline battery_status_e get_charge_state(void)
{
    static uint8 lsat_state = 0xff;
    static uint8 ret_state = 0xff; 
    static uint32 last_time_ab = 0;
    uint32 os_cur_time_ab = sys_get_ab_timer();
    if(ret_state == 0xff)//��ʼ��
    {
       ret_state = g_app_info_state.charge_state;
    }
    
    if(g_app_info_state.charge_state == lsat_state)
    {
        if((last_time_ab + (1000*5)) < os_cur_time_ab)//����5��
        {
            ret_state = g_app_info_state.charge_state;
        }
    }
    else
    {
        last_time_ab = os_cur_time_ab;
        lsat_state = g_app_info_state.charge_state;
    }
    return ret_state;
}

///��ȡ��ص���
static inline uint8 get_bat_value(void)
{
    return g_app_info_state.bat_value;
}

///�޸ĵ�ص���
static inline void change_bat_value(uint8 value)
{
    g_app_info_state.bat_value = value;
}

///��ȡ������Ŵ�������
static inline play_disk_type_e get_play_disk_type(void)
{
    return g_app_last_state.last_disk;
}

///����������Ŵ�������
static inline void set_play_disk_type(play_disk_type_e play_disk)
{
    g_app_last_state.last_disk = play_disk;
}

//��ȡ��һ��ǰ̨AP fuction index
static inline uint8 get_last_func_index(void)
{
    return g_app_last_state.last_func_index;
}

///������һ��ǰ̨AP fuction index�� ����ap_switch_info�����е��±�
static inline void set_last_func_index(uint8 index_num)
{
    g_app_last_state.last_func_index = index_num;
}

///��ȡ��ǰǰ̨AP fuction index�� ����ap_switch_info�����е��±�
static inline uint8 get_cur_func_index(void)
{
    return g_app_info_state.cur_func_index;
}

///���õ�ǰǰ̨AP fuction index
static inline void set_cur_func_index(uint8 index_num)
{
    g_app_info_state.cur_func_index = index_num;
}

///��ȡ��ǰǰ̨AP funciton id
static inline uint8 get_cur_func_id(void)
{
    return g_app_info_state.cur_func_id;
}

///���õ�ǰǰ̨AP function id
static inline void set_cur_func_id(uint8 func_id)
{
    g_app_info_state.cur_func_id = func_id;
}

///��ȡ��ǰAPK״̬
static inline apk_status_e get_apk_status(void)
{
    return g_app_info_state.apk_status;
}
static inline bool get_apk_available(void)
{
    return ((g_app_info_state.apk_status == APK_FRONTGROUND) || (g_app_info_state.apk_status == APK_BUSY));
}

///���õ�ǰAPK״̬
static inline void set_apk_status(apk_status_e status)
{
    g_app_info_state.apk_status = status;
}

//Ӧ�ã����̣�����ģ��ӿ���������ϸ˵�����ӿڶ��壩
extern bool applib_init(uint8 app_id, app_type_e type) __FAR__;
extern bool applib_quit(void) __FAR__;

extern engine_type_e get_engine_type_by_appid(uint8 app_id) __FAR__;
extern uint8 get_engine_appid_by_type(engine_type_e engine_type) __FAR__;

#endif
