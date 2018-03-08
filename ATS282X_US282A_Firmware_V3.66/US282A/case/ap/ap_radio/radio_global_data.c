#include "app_radio.h"

/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t radio_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

// for alarm_fm
// radio����ģʽ
uint8 g_radio_from_alarm = 0;
//alarm_fm�Ľ���ʱ��
time_t alarmfm_end_time;
// alarm_fm����
uint8 alarmfm_volume;

/*****************************************************
 * ֻ����ȫ�ֱ���
 ******************************************************/

//���������Ƶ��
const uint32 max_freq[] =
{ MAX_US, MAX_JP, MAX_EU };

//��������СƵ��(��λkHz)
const uint32 min_freq[] =
{ MIN_US, MIN_JP, MIN_EU };

/*****************************************************
 * ��������ر���
 ******************************************************/

//ϵͳȫ�ֱ���
comval_t g_comval;

//radio ui ����������Ϣ
radio_config_t g_radio_config;

//����״̬
Engine_Status_t g_engine_status;

/*****************************************************
 * ȫ��buffer
 ******************************************************/

//�Զ���̨�����ڱ�����Ч��̨����ʱbuffer
uint32 Auto_tab[MAX_STATION_COUNT];

/*****************************************************
 * ���̿�����ؼ�����
 ******************************************************/

/* ָ��ǰ�����ŵĵ�̨�б����*/
uint32 *pfreq;

//��ʶ��ǰ����Ӧ�ó���
radio_scene_e g_radio_scene;

//���ݽ��а��Զ���̨ǰ��Ƶ��
uint32 startfreq;

/*****************************************************
 * ��������ѡ̨
 ******************************************************/

//�洢���������buffer
uint8 g_digital_buf[MAX_DIGITAL_COUNT];

//��ǰ���ֽ��ĸ���
uint8 g_digital_count;

//��̨����ֵ FMENGINE_AP_ID_SEEKTH
uint8 g_search_throd;

//�Ƿ��¼������
app_param_e g_enter_param;

//KEY_NEXT_VOLADD �� KEY_PREV_VOLSUB ������������־
bool g_next_prev_flag = FALSE;
//for RCP
//�Ƿ�������̨��
bool g_searching_flag = FALSE;
//FM�Ƿ��ڲ���
bool g_playing_flag = FALSE;

uint8 g_freq_tts_sec_ids[10];

uint32 g_search_current_freq;
bool g_digit_input_flag;
bool g_first_enter_play;

bool g_neednot_freq_tts_play;

//��ʼ���׶Σ����Թ��˵�һЩ�¼�����
bool g_radio_init_flag;

//for set release mute timer
int8 g_mute_set_timer = -1;
