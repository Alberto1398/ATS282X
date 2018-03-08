/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������COMMON ʱ�����ӷ�����صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_TIME_ALARM_H
#define _COMMON_TIME_ALARM_H

#include "psp_includes.h"
#include "enhanced.h"
#include "common_func.h"
#include "vm_def.h"

#define RCP_ALARM_MAX                (4) //������Ŀ������������Ϊ10����time_alarm_var_t��512�ֽ����ڣ�
#define BUILDIN_RING_MAX             (1) //Ĭ��������Ŀ

#define RING_FOLDER_FILENUM_OFFSET   (4) //��offset��Ӧtime_alarm_var_t��uint8 ring_folder_filenum[]
#define RCP_RING_FOLDER_MAX          (4) //�Զ�����������Ŀ¼��Ŀ
#define RING_FOLDER_FILENAME_LEN     (32)//����Ŀ¼�ļ�������
#define RING_FOLDER_FILENUM_PER_SEC  (512/RING_FOLDER_FILENAME_LEN)//һ�������������ɵ�����Ŀ¼�ļ�������
#define RING_FOLDER_FILENUM_MAX      (64)//����Ŀ¼�ļ���������VRAM��С����

/*! ������������ */
typedef enum
{
    RING_TYPE_UHOST = 0, //U�̵�ǰ�б�
    RING_TYPE_SDCARD = 1, //SDCARD��ǰ�б�
    RING_TYPE_RESERVE = 2, //����
    RING_TYPE_BUILTIN = 3, //��������
    /*��Ŀ¼������Ŀ¼�µ�����Ŀ¼����˳����QUERY_ID_RING_FOLDER����ָ��Ŀ¼˳��һ��*/
    RING_TYPE_FOLDER1 = 4, //�Զ�������Ŀ¼
    RING_TYPE_FOLDER2 = 5, //�Զ�������Ŀ¼
    RING_TYPE_FOLDER3 = 6, //�Զ�������Ŀ¼
    RING_TYPE_FOLDER4 = 7, //�Զ�������Ŀ¼
} ring_type_e;

typedef struct
{
    /*! �������� */
    uint8 alarm_name[32];
    /*! ����ID����1~9Ϊ��ЧID */
    uint8 alarm_id;
    /*! ����ռ�ñ�־��1��ʾ��ռ�� */
    uint8 alarm_used;
    /*! ���ӿ��� */
    uint8 alarm_onoff;
    /*! �������ڣ�bit0 ~ bit6 �ֱ��ʾ���� ~ ���� */
    uint8 alarm_day;
    /*! ����ʱ�� {uint8 hour;uint8 minute;uint8 second;} */
    time_t alarm_time;
    /*! �������ͣ���� ring_type_e ���� */
    uint8 ring_type;
    /*! ������� */
    union
    {
        uint32 u_song_seq; /*RINT_TYPE_UHOST   = 0, U�̵�ǰ�б�*/
        uint32 sd_song_seq; /*RINT_TYPE_SDCARD  = 1, SDCARD��ǰ�б�*/
        uint32 ring_seq; /*RING_TYPE_BUILTIN = 3, �������� �� RING_TYPE_FOLDERX ����Ŀ¼*/
    } sound;
} alarm_alarm_t;

//��������״̬��
typedef enum
{
    /*! ������� */
    calendar_year,
    /*! �����·� */
    calendar_month,
    /*! �������� */
    calendar_day,
    /*! ����Сʱ */
    calendar_hour,
    /*! ���÷��� */
    calendar_minute,
    /*! ������� */
    calendar_finish
} calendar_state_e;

//��������״̬��
typedef enum
{
    /*! ���ÿ�/�� */
    alarm_onoff,
    /*! ����Сʱ */
    alarm_hour,
    /*! ���÷��� */
    alarm_minute,
    /*! ������� */
    alarm_finish,
} alarm_state_e;

typedef struct
{
    /*! ���Ӽ�¼ */
    alarm_alarm_t alarm;
    /*! ��ǰ����ʱ�� */
    time_t last_time;
    /*! ����snooze���� */
    uint8 snooze;
} alarm_record_t;

/*!
 * \brief
 *  time & alarm��VMȫ�ֱ���.
 */
typedef struct
{
    /*! VM ����ħ��*/
    uint16 magic;
    /*! ��ǰ���Ӽ�¼INDEX��0xff��ʾ��Ч */
    uint8 alarm_idx;
    uint8 reserve[1];
    /*! �Զ�����������Ŀ¼�ļ��� */
    uint8 ring_folder_filenum[RCP_RING_FOLDER_MAX];
    /*! ���Ӽ�¼�б� */
    alarm_record_t alarm_records[RCP_ALARM_MAX];
} time_alarm_var_t;

#define RECORD_LIST_READ_BYINDEX     0 //��ȡָ��INDEX���Ӽ�¼��param1 �������Ӽ�¼ָ�룬param2 ����INDEX
#define RECORD_LIST_SEARCH_BYID      1 //��ȡָ��ID���Ӽ�¼��param1 �������Ӽ�¼ָ�룬param2 ����ID
#define RECORD_LIST_READ_CUR         2 //��ȡ��ǰ�ID���Ӽ�¼��param1 �������Ӽ�¼ָ�룬param2 0
#define RECORD_LIST_UPDATE_ALARM     3 //��������ʱ�䣬param1 ����ģʽ��param2 0
#define RECORD_LIST_GET_NEXT_ALARM   4 //��ȡ�������ʱ�䣬�ų���ǰ�����ֵ����ӣ�param1 ��������ʱ�䣬param2 0
#define RECORD_LIST_SET_NEXT_ALARM   5 //������һ�����ӣ�������ǰ���ӹص���param1 ��һ������INDEX��param2 0
#define RECORD_LIST_ADD_BYINDEX      6 //��ӻ�������Ӽ�¼��param1 �������Ӽ�¼ָ�룬param2 ����INDEX
#define RECORD_LIST_DELETE_BYINDEX   7 //ɾ�����Ӽ�¼��param1 0��param2 ����INDEX
#define RECORD_LIST_VALID_TOTAL      8 //��ȡ��Ч������Ŀ��param1 0��param2 0
#define RECORD_LIST_SEARCH_NOUSED    9 //�����հ׼�¼��param1 0��param2 0
#define RECORD_LIST_READ_FOLDERNUM   10//��ȡ����Ŀ¼�¸�����Ŀ¼�µ��ļ���Ŀ
#define RECORD_LIST_WRITE_FOLDERNUM  11//��������Ŀ¼�¸�����Ŀ¼�µ��ļ���Ŀ
extern int time_alarm_record_list_operate(uint8 op, void *param1, void *param2) __FAR__;

typedef enum
{
    ALARM_RING_ENTER,
    ALARM_RING_CLOSE,
    ALARM_RING_SNOOZE,
    ALARM_RING_ENTER_CLOSE_AUTO, //�ڲ����������ӵĳ�����ʹ�ã��Ὣ���ӹر�Ȼ�������˳�
} alarm_ring_mode_e;

extern app_result_e tm_alarm_alarm_ring_deal(alarm_ring_mode_e alarm_ring_mode) __FAR__;

typedef struct
{
    date_t date;
    time_t time;
    bool twinkling_flag;
    int8 twinkling_timer_id;
} clock_show_var_t;

typedef struct
{
    date_t date;
    time_t time;
    uint16 *setting_num16;
    uint8 *setting_num8;
    uint8 setting_digits;
    uint8 now_digits;
    bool digit_flag;
    bool write_date_falg;
    bool write_time_falg;
    calendar_state_e calendar_state;
    bool twinkling_flag;
    int8 twinkling_timer_id;
} set_calendar_var_t;

extern app_result_e set_calendar_switch_deal(set_calendar_var_t *set_calendar_var) __FAR__;
extern app_result_e set_calendar_sub_deal(set_calendar_var_t *set_calendar_var) __FAR__;
extern app_result_e set_calendar_add_deal(set_calendar_var_t *set_calendar_var) __FAR__;
extern void sys_write_rtc(set_calendar_var_t *set_calendar_var) __FAR__;

typedef struct
{
    time_t time;
    uint16 *setting_num16;
    uint8 *setting_num8;
    uint8 setting_digits;
    uint8 now_digits;
    bool digit_flag;
    alarm_state_e alarm_state;
    bool twinkling_flag;
    int8 twinkling_timer_id;
    alarm_record_t cur_alarm_record;
    uint8 alarm_onoff_bk;
} set_alarm_var_t;

extern app_result_e set_alarm_switch_deal(set_alarm_var_t *set_alarm_var) __FAR__;
extern app_result_e set_alarm_sub_deal(set_alarm_var_t *set_alarm_var) __FAR__;
extern app_result_e set_alarm_add_deal(set_alarm_var_t *set_alarm_var) __FAR__;

typedef struct
{
    /*! �������ͣ���� ring_type_e ���� */
    uint8 ring_type;
    uint8 reserve[2];
    /*! �ļ����Ʊ������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 code_type;
    /*! ���������ļ����� */
    uint8 song_name[60];
} ring_song_t;

/*����Ŀ¼��Ϣ�ṹ��*/
typedef struct
{
    /*! ����Ŀ¼�����ͣ�ID */
    uint8 ring_type;
    /*! ����Ŀ¼���ֱ������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 code_type;
    /*! ����Ŀ¼���� */
    uint8 folder_name[30];
} ring_folder_infor_t;

/*���Ӳ���*/
typedef struct
{
    /*! snoozeʱ�䣬1-30���ӣ�0��ʾ������snooze���� */
    uint8 snooze_time;
    /*! snooze������1-10�Σ�0��ʾ���޴� */
    uint8 snooze_max_count;
    /*! ��ʱʱ�䣬1-30���ӣ�0��ʾ����ʱһֱ��*/
    uint8 overtime;
    uint8 reserve;
} alarm_param_t;

#define UPDATE_ALARM_ADD        0 //��ӻ��޸����Ӽ�¼
#define UPDATE_ALARM_DEL        1 //ɾ�����Ӽ�¼
#define UPDATE_ALARM_SNOOZE     2 //��ʱ
#define UPDATE_ALARM_CLOSE      3 //�Զ��ر�
#define UPDATE_ALARM_GET_NEXT   4 //��ȡ�������ʱ�䣬�ų�����ǰ�����ֵ�����
#define UPDATE_ALARM_JUST_CLOSE 5 //�����رյ�ǰ����
#define UPDATE_ALARM_CLOSE_SAME 6 //�ر�������ͬʱ�������
extern uint8 time_alarm_update_alarm(uint8 mode) __FAR__;

extern clock_show_var_t *g_p_clock_show;
extern void tm_alarm_create_clock_view(void) __FAR__;

extern set_calendar_var_t *g_p_set_calendar;
extern void tm_alarm_create_set_calendar_view(void) __FAR__;

extern set_alarm_var_t *g_p_set_alarm;
extern void tm_alarm_create_set_alarm_view(void) __FAR__;

#endif
