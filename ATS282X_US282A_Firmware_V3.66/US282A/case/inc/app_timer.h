/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������ʱ����غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _app_timer_H_
#define _app_timer_H_

#include "psp_includes.h"
#include "case_type.h"

/*! ������ʱ����Ŀ������ϵͳ���ܶ�ʱ��USB����л� */
#define COMMON_TIMER_COUNT      4

/*! 
 * \brief
 *  Ӧ�ü���ʱ���������� tag id�����ڰѶ�ʱ��������ĳ�����������õع���Ӧ�������ж�ʱ����
 */
/*! AP ������ tag */
#define APP_TIMER_TAG_MAIN          0x00
/*! ϵͳ���� tag�����ⳡ�����κγ����¶���ִ�� */
#define APP_TIMER_TAG_SYS           0xff

/*! 
 * \brief
 *  timer_type_e ��ʱ������ö�ٽṹ
 */
typedef enum
{
    /*! ��ͨģʽ�Ķ�ʱ�����Զ����ڷ��� */
    TIMER_TYPE_NORMAL,
    /*! ������ʱ����ֻ��ʱһ�κ����TIMER_STATE_STOPED ״̬ */
    TIMER_TYPE_SINGLE_SHOT,
} timer_type_e;

/*! 
 * \brief
 *  timer_state_e ��ʱ��״̬ö�ٽṹ
 */
typedef enum
{
    /*! ��־����״̬��������������ʱ�� */
    TIMER_STATE_NOUSED,
    /*! ������ʱ״̬ */
    TIMER_STATE_RUNNING,
    /*! ֹͣ��ʱ״̬��ֻ��ͨ��restart���¿�ʼ��ʱ */
    TIMER_STATE_STOPED,
} timer_state_e;

/*! 
 * \brief
 *  timer_attrb_e ��ʱ������ö�ٽṹ����������ʱ����Դ���г�ʼ����
 */
typedef enum
{
    /*! UI ����ʱ�����ڱ���Ϩ�����ִ�� ISR */
    APP_TIMER_ATTRB_UI,
    /*! control ����ʱ�����κ�ʱ�򶼻�ִ�� ISR */
    APP_TIMER_ATTRB_CONTROL,
    /*! UI ����ʱ�����ڱ���Ϩ�����ִ�� ISR */
    APP_TIMER_ATTRB_COM_UI,
    /*! control ����ʱ�����κ�ʱ�򶼻�ִ�� ISR */
    APP_TIMER_ATTRB_COM_CONTROL,
} timer_attrb_e;

/*! 
 * \brief
 *  timer_proc ��ʱ���������� 
 */
typedef void (*timer_proc)(void);

/*! 
 * \brief
 *  app_timer_t ��ʱ������ṹ��
 */
typedef struct
{
    /*! ��ʱ��״̬����timer_state_e ���� */
    uint8 state :2;
    /*! ��ʱ�����ԣ���timer_attrb_e ���� */
    uint8 attrb :2;
    /*! ��ʱ�����ͣ���timer_type_e ���� */
    uint8 type :1;
    /*! twinkle ��־��twinkle�ඨʱ��typeΪTIMER_TYPE_NORMAL */
    uint8 twinkle :1;
    /*! ��ʱ��������־ tag */
    /*! twinkle ״̬��1��ʾ�����ڣ�0��ʾ������ */
    uint8 on_off :1;
    uint8 tag;
    /*! ֻ��twinkle�ඨʱ����Ч��ѭ��������Ŀ��0��ʾѭ��ֹͣ��-1��ʾһֱѭ��ֱ���������� */
    uint8 count;
    uint8 reserve;
    /*! ��ʱ�� ��ʱ���ڣ���λΪ 1ms */
    /*! ��ʱ�䣬��˸ʱ��Ч����λΪ1ms */
    uint16 timeout;
    /*! ��ʱ�䣬��˸ʱ��Ч����λΪ1ms */
    uint16 timeout_off;
    /*! ��ʱ�� ��һ�γ�ʱ����ʱ��㣬��λΪ 1ms */
    uint32 timeout_expires;
    /*! ��ʱ�� �������� */
    timer_proc func_proc;
} app_timer_t;

typedef struct
{
    /*! ѭ��������Ŀ��0��ʾѭ��ֹͣ��-1��ʾһֱѭ��ֱ���������� */
    uint8 count;
    /*! ��ʼ����״̬ */
    uint8 on_off;
    /*! ��ʱ�䣬��˸ʱ��Ч����λΪ1ms */
    uint16 timeout_on;
    /*! ��ʱ�䣬��˸ʱ��Ч����λΪ1ms */
    uint16 timeout_off;
    /*! ��ʱ�� �������� */
    timer_proc func_proc;
} twinkle_app_timer_param_t;

/*! 
 *  \brief
 *  g_app_timer_vector Ӧ�ü���ʱ������ָ�룬ָ��Ӧ�ÿռ�Ķ�ʱ������
 */
extern app_timer_t *g_app_timer_vector;

/*! 
 *  \brief
 *  g_app_timer_count Ӧ�ü���ʱ����Ŀ
 */
extern uint8 g_app_timer_count;
extern uint8 g_app_timer_id;

/*! 
 *  \brief
 *  g_this_app_timer_tag ��ʱ��������־��AP����Ϊ0
 */
extern uint8 g_this_app_timer_tag;

#define COM_APP_TIMER_MAX  (8)
extern app_timer_t g_com_app_timer_vct[COM_APP_TIMER_MAX];


//��ʱ������ģ��ӿ�����
extern int8 set_app_timer(uint32 attrb_tagh, uint16 timeout, timer_proc func_proc) __FAR__;
extern int8 set_single_shot_app_timer(uint32 attrb_tagh, uint16 timeout, timer_proc func_proc) __FAR__;
extern int8 set_single_shot_app_timer_btmamager(uint32 attrb_tagh, uint16 timeout, timer_proc func_proc) __FAR__;
extern int8 set_twinkle_app_timer(uint32 attrb_tagh, twinkle_app_timer_param_t *twinkle_param) __FAR__;
extern bool modify_app_timer(int8 timer_id, uint16 timeout) __FAR__;
extern bool stop_app_timer(int8 timer_id) __FAR__;
extern bool restart_app_timer(int8 timer_id) __FAR__;
extern bool kill_app_timer(int8 timer_id) __FAR__;
extern bool init_app_timers(app_timer_t *timers, uint8 count) __FAR__;
extern void standby_restart_all_app_timer(app_timer_t *timers, uint8 count) __FAR__;
extern void handle_timers(app_timer_t *timers, uint8 count) __FAR__;
extern bool kill_app_timer_safe(int8 timer_id, timer_proc time_handle)__FAR__;

#endif
