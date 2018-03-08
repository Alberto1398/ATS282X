/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ����������صĺ궨�壬ö�����ͣ��ṹ�嶨��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _SETTING_COMMON_H
#define _SETTING_COMMON_H

#include "psp_includes.h"
#include "case_type.h"
#include "mmm_dae_def.h"

typedef enum
{
    VOLUME_MODE_NORMAL = 0,
    VOLUME_MODE_CALL = 1,
    VOLUME_MODE_MAX = 2,
} volume_mode_e;

typedef enum
{
    LOW_POWER_MODE_NONE = 0,
    LOW_POWER_MODE_S2 = 1,
    LOW_POWER_MODE_S3 = 2,
} low_power_mode_e;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��Чֵ���飬�ɱ䳤���� */
    peq_band_t eq_data;
} peq_config_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �˲������� */
    short filter_order;
    /*! �������˲���Qֵ */
    short filter_QvalLeft;
    /*! �������˲���Qֵ */
    short filter_QvalRight;
} spk_compensation_config_t;

typedef struct
{
    short threshold;
    short attack_time;
    short release_time;
    short crossover_freq;
} mdrc_band_config_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��Чֵ���飬�ɱ䳤���� */
    mdrc_band_config_t mdrc_data;
} mdrc_config_t;

typedef struct
{
    /*! ��ֹƵ�ʣ���λHz */
    uint16 cutoff;
    /*! �������� */
    short  reserve_param;
    /*! ��ʼ��������λΪ 0.1db */
    short  rel_vol_start;
    /*! ������������λΪ 0.1db */
    short  rel_vol_keep;
    /*! ������棬��λΪ 0.1db */
    short  gain;
} dew_band_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��Ч��ǿ�������� */
    dew_band_t dew_band_setting;
} dew_config_t;

typedef struct
{
    //ͳ������ʱ������1msΪ��λ
    short period;
    //��Ч������
    short period_count;
    //�źŴ�С���ͽ϶������Ԥ�����ƣ���0.1dbΪ��λ
    short preadjust_db;
    //��ʼԤ�������źŴ�С����������Ȼ��ÿ�����ڽ�����Ӧ����
    short preadjust_count;
    //�źŴ�С��Ч��Сֵ����0.1dbΪ��λ
    short signal_energy_min;
} my_signal_config_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �źż����� */
    my_signal_config_t signal_cfg;
} enhanced_signal_config_t;

/*********Ϊ���ݱ�׼��Чģ�����ӵ�DAE����**********/
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �źż����� */
    dae_compressor_standard_mode_t compressor;
} dae_compressor_temp_standard_mode_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �źż����� */
    int16 vol_adjust;
    mdrc_extend_para_standard_mode_t mdrc;
} mdrc_extend_para_temp_standard_mode_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �źż����� */
    dae_mdrc_band_standard_mode_t mdrc_band;
} mdrc_para_temp_standard_mode_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �źż����� */
    mdrc_peak_detect_standard_mode_t mdrc_peak_detect;
} mdrc_peak_detect_temp_standard_mode_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! �źż����� */
    vol_dae_para_sm_t vol_dae_para_sm;
} vol_dae_para_temp_standard_mode_t;

/**************************************************/

typedef struct
{
    short enable;				            //default: 200ms,step 0.01ms, set 200*100 = 20000    
    short DownExp_attack_time;				//default: 200ms,step 0.01ms, set 200*100 = 20000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;		    //default: 1ms, step 0.01ms, set 100;
	short DownExp_threshold;			    //default: -80dB(min), step 0.1dB, set -800
	short DownExp_ratio;				    //default: 3(greater than 1)  
}noise_reduction_t;

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;

    noise_reduction_t noise_reduction;
}noise_reduction_config_t;

/*!
 * \brief
 *  comval_t ϵͳ����������Ͻṹ��
 */
typedef struct
{
    /*! magic��ֵΪ 0x55AA ʱ��Ч */
    uint16 magic;

    /*! ֧�ֿ�ѡ��  0:��֧��  1:֧�� */
    uint8 support_cardplay;
    /*!֧��u�̲���ѡ�� 0:��֧�� 1:֧��*/
    uint8 support_uhostplay;
    /*!֧��lineinѡ�� 0:��֧�� 1:֧��*/
    uint8 support_linein;
    /*! ֧��FMѡ��  0:��֧��  1:֧�� */
    uint8 support_fmradio;
    /*! ֧�ֺ�̨���� 0:��֧��  1:֧�� */
    uint8 support_bt_inback;
    /*! ֧�������Ƹ� 0:��֧��  1:֧�� */
    uint8 support_btplay;
    /*! ֧���������� 0:��֧��  1:֧�� */
    uint8 support_btcall;
    /*! ֧��USB���� 0:��֧��  1:֧�� */
    uint8 support_usbbox;
    /*! ֧�ִ��������� 0:��֧��  1:֧�� */
    uint8 support_usbmsc;
    /*! ֧��OTA 0:��֧��  1:֧�� */
    uint8 support_ota;    
    /*! ֧��OTA�������� 0:��֧��  1:֧�� */
    uint8 support_ota_song_test;    
    /*! �͹���ģʽ */
    low_power_mode_e lowpower_mode;
    /*! ����͹���ʱ�� */
    uint8 lowpower_timer;
    /*! ʡ��ػ�ʱ�� ��0.5��Ϊ��λ��0��ʾ��ʡ��ػ� */
    uint8 poweroff_timer;
    /*! ����ʡ��ػ�ʱ�䣬�Է���Ϊ��λ�����������ڡ�ʡ��ػ�ʱ�䡱��Ϊ0ʱ��Ч */
    uint8 poweroff_bt_waitpair_timer;

    /*! ˫����������*/
    uint8 double_click_enable;
    /*! ˫�������ӳ�ʱ��*/
    uint16 double_click_delay;

    /*! ����������*/
    uint8 key_tone;
    /*! ���������ӿ���*/
    uint8 key_tone_overlay;
    /*! ״̬������������*/
    uint8 tts_enable;
    /*! ��ǰTTS��������*/
    uint8 tts_language;
    /*! ��������TTS��С������0��ʾ������Ϊ0ʱû�а�������TTS*/
    uint8 kt_tts_min_volume;
    /*! ��������TTS������� */
    uint8 kt_tts_max_volume;
    /*! ��������TTS�ź�˥�����ٷֱ� */
    uint8 kt_tts_percent;

    /*! ��ǰ�߼�����ֵ��0~31 */
    uint8 volume_current;
    /*! ��ǰ�Ѿ����õ�PA������ֵ��0~31��0xff��ʾ��Ч */
    uint8 volume_current_phy;
    /*! ��ǰ����ģʽ */
    volume_mode_e volume_mode;
    /*! ��������ģʽ������ */
    uint8 volume[VOLUME_MODE_MAX];
    /*! ��ǰ���������棨˥������0 ~ 12 ��ʾ 0db ~ -12db */
    uint8 volume_gain;
    /*! ��ǰ������������������ƫ��ֵ����λΪ 0.1db��ȡֵ<=0 */
    int16 volume_relative;
    /*! ��ǰ�������ƣ���������������˥��ֵ����λΪ 0.1db��ȡֵ<=0 */
    int8 volume_limit;

    /*! �Ƿ�֧��LED�������ʾ */
    uint8 support_led_display;
    /*! LED����ܱ���ʱ�� ��10��Ϊ��λ��0��ʾ���� */
    uint8 light_timer;

    /*! ��س�緽ʽ */
    uint8 bat_charge_mode;
    /*! ������Ƿ�TTS��ʾ */
    uint8 bat_charge_full_tts;

    /*! ���������� */
    uint8 snooze_timer;
    uint8 snooze_max;
    uint8 alarm_overtime;

    /*! Ӳ���������� */
    uint8 hard_support_card;
    uint8 hard_support_uhost;
    uint8 hard_support_headphone;

    /*! ���Ե���ѡ�� */
    uint8 stub_enable;
    uint8 stub_print;

    /*! CASE ���Զ��� Graphics EQ ����*/
    uint8 eq_enable;
    uint8 eq_type;
    /*! MDRC����΢�� */
    int8 mdrc_vol_adjust;
    /*! mdrc��������������źŽ���˥�����ٷֱ� */
    uint8 mdrc_percent;
    /*! DAE���κ�Ƶ���������ֻ�ܵ��÷�ֵ���������ֻᱻ�ص� */
    uint8 dae_mdrc_peak_gain;
    /*! DAE���û����ѳ�ʼ���õı�־ */
    bool dae_inited_flag;
    bool default_vbass_enable; //Ĭ���������ʹ�ܿ���
    bool set_dae_by_app_flag;  //ͨ��APP������Ч��־
    bool w_input_enable;
    /*! ��Ч��ǿЧ������ */
    uint8 dae_enhanced_enable;
    /*! ��Ч����Ч������ */
    uint8 dae_weaken_enable;
    /*! ��ͨ�˲�����ֹƵ�ʣ�0��ʾû�и�ͨ�˲��� */
    uint8 highpass_cutoff;
    /*! ��Ч��ǿ�����ʣ���Ҫ�ۺϿ�����ǿPEQ���໥Ӱ��̶ȣ��Լ�������ǿ�͸�����ǿ����λΪ0.1 */
    uint8 dae_enhanced_peak_ratio;

    bool aux_flag; //LINEIN������־
    bool signal_variable_mode; //�źſɱ�ģʽ������AUXͨ������֧������ͬ���������Ƹ��
    bool dae_cfg_variable; //DAE�����Ƿ�ɱ䣬�����mdrc������Ч��ǿ��������ô���ǿɱ��

    /*! �������Ƶ�ź��������ʹ�� */
    uint8 signal_energy_enable;
    short signal_energy_used; //�Ѿ�ʹ���˵���Ч�ź����������ֵΪ0db����λΪ0.1db
    short signal_energy; //��ǰ��Ч�ź�����
    short signal_energy_min; //��Ч�ź�������Сֵ

   /*********Ϊ���ݱ�׼��Чģ�����ӵ�DAE����**********/
    //bool audiopp_type;                      //��Ч������
    int8 mdrc_vol_adjust_standard_mode;     //����΢��
    
   /**************************************************/

    /*! DAE������ */
    dae_config_t dae_cfg;
} comval_t;

#define OTA_UPGRADE_ENABLE		(0x7c8a)
/*!
 * \brief
 *  otaval_t OTA������Ͻṹ��
 */
typedef struct
{
    /*! magic��ֵΪ 0x55AA ʱ��Ч */
    uint16 magic;
	
    /*! ������־��ֵΪ OTA_UPGRADE_ENABLE(0x7c8a) ʱ��Ч */
	uint16 ota_upgrade_flag;
} otaval_t;

/*!
 * \brief
 *  testval_t test mode�����־
 */
typedef struct
{
    /*! magic��ֵΪ 0x55AA ʱ��Ч */
    uint16 magic;
	
    /*! test mode��־,0x01:FT mode,0x02:bqbmode */
	uint8 api_flag;

	uint16 usb_cardreader_magic;

	uint16 usb_sound_magic;

	uint32 usb_cardreader_low;

	uint32 usb_cardreader_high;
	
	uint32 usb_sound_low;

	uint32 usb_sound_high;

} testval_t;


/*!
 *  \brief
 *  sys_comval ϵͳ����ָ�룬ͨ�� com_set_sys_comval �ӿڴ��ݸ� common ��������ȫ��ָ��
 */
extern comval_t *sys_comval;

#endif //_SETTING_COMMON_H
