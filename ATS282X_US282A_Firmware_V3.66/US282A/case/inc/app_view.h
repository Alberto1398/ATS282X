/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ����ͼ��غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_VIEW_H
#define _COMMON_VIEW_H

#include "psp_includes.h"
#include "gui_msg.h"
#include "app_msg.h"
#include "case_type.h"

//APP TIMER TAG ID Ҳ��ʹ��ͬ������ֵ
#define APP_VIEW_ID_MAIN         0x00
#define COM_VIEW_ID_DISP_CLOCK   0x80
#define COM_VIEW_ID_SET_CALENDAR 0x81
#define COM_VIEW_ID_SET_ALARM    0x82
#define COM_VIEW_ID_SET_VOLUME   0x83
#define COM_VIEW_ID_SET_EQ       0x84

typedef enum
{
    VIEW_EVENT_KEY,
    VIEW_EVENT_SYS_EVENT,
    VIEW_EVENT_STATUS_CHANGE,
} view_event_e;

typedef struct
{
    view_event_e ev_type;
    /* ���� event_type ����
     * VIEW_EVENT_KEY -> key_event_t
     */
    void *ev_data;
} view_event_t;

typedef app_result_e (*key_event_cb)(void);
typedef app_result_e (*sys_event_cb)(void *ev_param);

typedef struct
{
    key_event_t ke;
    key_event_cb cb;
} key_event_map_t;

typedef struct
{
    sys_event_t se;
    sys_event_cb cb;
} sys_event_map_t;

typedef enum
{
    VIEW_UPDATE_CREATE, //com_view_add ʱ�Զ�����
    VIEW_UPDATE_DESTROY, //com_view_remove ʱ�Զ�����
    VIEW_UPDATE_REDRAW,
    VIEW_UPDATE_STATUS,
} view_update_e;

typedef void (*view_update_cb)(view_update_e mode);

typedef enum
{
    /*! ����ͼ */
    VIEW_TYPE_MAIN,
    /*! ��̬����ͼ������Ƕ����������ͼ�����Լ��İ����¼�����Ҳ������ʱһ��ʱ����˳� */
    VIEW_TYPE_SUB,
    /*! ��ʾ����ͼ��������ʾ����ʱһ��ʱ����˳���������Ƕ����������ͼ������ʾ����ͼֻ��ͬʱ����һ�� */
    VIEW_TYPE_MSG,
} view_type_e;

#define UNKNOWN_KEY_IGNORE       0
#define UNKNOWN_KEY_QUIT_FILTER  1
#define UNKNOWN_KEY_QUIT_REDEAL  2
#define UNKNOWN_KEY_QUIT_IFMAIN  3

typedef struct
{
    view_type_e type; //��ͼ����
    uint8 unknown_key_deal; //��̬����ͼ�£�����ʶ��İ�������ʽ��0��ʾ���ԣ�1��ʾ�˳������ˣ�
                            //2��ʾ�˳�������MAIN��ͼ��������
                            //3��ʾ���MAIN��ͼ�ܹ�ʶ����˳�������MAIN��ͼ��������
    uint16 overtime; //��̬����ͼ����ʾ����ͼ��ʱʱ�䣬
                     //������̬����ͼ��Ϊ0��ʾ�����ó�ʱ���ƣ�
                     //������ʾ����ͼ��Ϊ0��ʾ��Ч�����ᴴ������ͼ�������˳�
    const key_event_map_t *ke_maplist; //����ͼ����̬����ͼ�İ����¼�ӳ���
    const sys_event_map_t *se_maplist; //����ͼϵͳ��Ϣӳ���
    view_update_cb view_cb; //��ͼ���»ص�����
} create_view_param_t;

typedef struct
{
    uint8 id; //id��2�����ã���һ����ƥ����ͼ���ڶ�������Ϊapp_timer��tag��
    uint8 type; //��ͼ����
    uint8 unknown_key_deal; //��̬����ͼ�£�����ʶ��İ�������ʽ��0��ʾ���ԣ�1��ʾ�˳������ˣ�
                            //2��ʾ�˳�������MAIN��ͼ��������
                            //3��ʾ���MAIN��ͼ�ܹ�ʶ����˳�������MAIN��ͼ��������
    int8 timer_id; //VIEW_TYPE_SUB �� VIEW_TYPE_MSG ��ʱ��ʱ��
    const key_event_map_t *ke_maplist; //����ͼ����̬����ͼ�İ����¼�ӳ���
    const sys_event_map_t *se_maplist; //����ͼϵͳ��Ϣӳ���
    view_update_cb view_cb; //��ͼ���»ص�����
} view_infor_t;

typedef struct
{
    uint8 top;
    view_infor_t *p_key_view_infor; //��ǰ���ͼ��Ϣ�ṹ��
} view_manager_infor_t;

#define MVC_VIEW_DEEPTH   3
extern view_infor_t g_view_infor[MVC_VIEW_DEEPTH];
extern view_manager_infor_t g_view_manager_infor;
extern key_event_t * g_p_view_ke;

extern int com_view_manager_init(void) __FAR__;
extern int com_view_manager_exit(void) __FAR__;
extern int com_view_add(uint8 view_id, create_view_param_t *p_create_view_param) __FAR__;
extern int com_view_remove(uint8 view_id, bool redraw) __FAR__;
extern int com_view_update(uint8 view_id) __FAR__;
extern void com_view_remove_top_view(void) __FAR__;
extern void com_view_remove_type_view(view_type_e type) __FAR__;

extern int com_view_key_event(key_event_t *p_ke, const key_event_map_t *app_ke_maplist, key_event_cb *ret_cb,
        bool only_com) __FAR__;
extern void com_view_key_check_dck(const key_event_map_t *app_ke_maplist) __FAR__;

typedef struct
{
    sys_event_cb ret_cb;
    uint8 ret_option;
    uint8 reserve[3];
} sys_event_ret_t;
extern int com_view_sys_event(sys_event_t *p_se, const sys_event_map_t *app_se_maplist, sys_event_ret_t *p_ret,
        bool only_com) __FAR__;

extern app_result_e com_view_loop_key(input_gui_msg_t *p_gui_msg) __FAR__;
extern app_result_e com_view_loop_sys(private_msg_t *p_pri_msg, bool tts_flag) __FAR__;
extern app_result_e com_view_loop(void) __FAR__;

#endif
