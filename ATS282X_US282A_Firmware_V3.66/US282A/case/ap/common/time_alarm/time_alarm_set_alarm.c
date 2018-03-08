/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������������ù�����ģ�飬ʵ���û������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_time_alarm.h"

const key_event_map_t alarm_ke_maplist[];
void tm_alarm_set_alarm_view(view_update_e mode);
void tm_alarm_create_set_alarm_view(void)
{
    create_view_param_t create_view_param;

    create_view_param.type = VIEW_TYPE_SUB;
    create_view_param.unknown_key_deal = UNKNOWN_KEY_IGNORE;
    create_view_param.overtime = 8000;
    create_view_param.ke_maplist = alarm_ke_maplist;
    create_view_param.se_maplist = NULL;
    create_view_param.view_cb = tm_alarm_set_alarm_view;
    com_view_add(COM_VIEW_ID_SET_ALARM, &create_view_param);
}

static const uint8 default_alarm_name[] = "default";

app_result_e alarm_key_deal_switch_clock(void);
app_result_e alarm_key_deal_switch_menu(void);
app_result_e alarm_key_deal_add(void);
app_result_e alarm_key_deal_sub(void);
app_result_e alarm_key_deal_number_input(void);

void tm_alarm_set_alarm_view(view_update_e mode);
void alarm_handle(void);

const key_event_map_t __section__(".rodata.ke_maplist") alarm_ke_maplist[] =
{
    /*! ����ʱ�ӹ��� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, 0 }, alarm_key_deal_switch_clock },

    /*! �л����ò˵� */
    { { KEY_ABREC, 0, KEY_TYPE_SHORT_UP, 0 }, alarm_key_deal_switch_menu },
    { { KEY_CLK, 0, KEY_TYPE_SHORT_UP, 0 }, alarm_key_deal_switch_menu },

    /*! ���ڼ� */
    { { KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD, 0 }, alarm_key_deal_add },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD, 0 }, alarm_key_deal_add },

    /*! ���ڼ� */
    { { KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD, 0 }, alarm_key_deal_sub },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD, 0 }, alarm_key_deal_sub },

    /*! ���ְ������� */
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG_UP, 0 }, alarm_key_deal_number_input },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, 0 }, NULL },
};

app_result_e alarm_key_deal_switch_clock(void)
{
    //tm_alarm_create_clock_view();
    com_view_remove_top_view();

    return RESULT_NULL;
}

app_result_e alarm_key_deal_switch_menu(void)
{
    app_result_e result;

    result = set_alarm_switch_deal(g_p_set_alarm);
    if (result == RESULT_BACK_SCENE_DISPATCH)
    {
        com_view_remove_top_view();
        result = RESULT_NULL;
    }
    else
    {
        tm_alarm_set_alarm_view(VIEW_UPDATE_REDRAW);
        restart_app_timer(g_p_set_alarm->twinkling_timer_id);
        g_p_set_alarm->twinkling_flag = TRUE;
        g_p_set_calendar->digit_flag = FALSE;
    }

    return result;
}

app_result_e alarm_key_deal_add(void)
{
    set_alarm_add_deal(g_p_set_alarm);

    tm_alarm_set_alarm_view(VIEW_UPDATE_REDRAW);
    restart_app_timer(g_p_set_alarm->twinkling_timer_id);
    g_p_set_alarm->twinkling_flag = TRUE;

    g_p_set_calendar->digit_flag = FALSE;
    return RESULT_NULL;
}

app_result_e alarm_key_deal_sub(void)
{
    set_alarm_sub_deal(g_p_set_alarm);

    tm_alarm_set_alarm_view(VIEW_UPDATE_REDRAW);
    restart_app_timer(g_p_set_alarm->twinkling_timer_id);
    g_p_set_alarm->twinkling_flag = TRUE;

    g_p_set_calendar->digit_flag = FALSE;
    return RESULT_NULL;
}

app_result_e alarm_key_deal_number_input(void)
{
    uint8 number = (uint8) (g_p_view_ke->val - KEY_NUM0);

    if (g_p_set_alarm->digit_flag == FALSE)
    {
        g_p_set_alarm->now_digits = 0;
        switch (g_p_set_alarm->setting_digits)
        {
            case 2:
            *(g_p_set_alarm->setting_num8) = 0;
            break;

            case 4:
            *(g_p_set_alarm->setting_num16) = 0;
            break;

            default:
            break;
        }

        g_p_set_calendar->digit_flag = TRUE;
    }

    switch (g_p_set_alarm->setting_digits)
    {
        case 2:
        if (g_p_set_alarm->now_digits >= 2)
        {
            *(g_p_set_alarm->setting_num8) = number;
            g_p_set_alarm->now_digits = 1;
        }
        else
        {
            *(g_p_set_alarm->setting_num8) = (*(g_p_set_alarm->setting_num8) * 10) + number;
            g_p_set_alarm->now_digits++;
        }
        break;

        case 4:
        if (g_p_set_alarm->now_digits >= 4)
        {
            *(g_p_set_alarm->setting_num16) = number;
            g_p_set_alarm->now_digits = 1;
        }
        else
        {
            *(g_p_set_alarm->setting_num16) = (*(g_p_set_alarm->setting_num16) * 10) + number;
            g_p_set_alarm->now_digits++;
        }
        break;

        default:
        break;
    }

    tm_alarm_set_alarm_view(VIEW_UPDATE_REDRAW);
    restart_app_timer(g_p_set_alarm->twinkling_timer_id);
    g_p_set_alarm->twinkling_flag = TRUE;

    return RESULT_NULL;
}

static void __tm_alarm_set_alarm_redraw(void)
{
    uint16 hour, minute;
    bool hour_twinkling = FALSE;
    bool minute_twinkling = FALSE;

    hour = g_p_set_alarm->time.hour;
    minute = g_p_set_alarm->time.minute;

    switch (g_p_set_alarm->alarm_state)
    {
        /*! ���ÿ�/�� */
        case alarm_onoff:
        break;

        /*! ����Сʱ */
        case alarm_hour:
        hour_twinkling = g_p_set_alarm->twinkling_flag;
        minute_twinkling = TRUE;
        break;

        /*! ���÷��� */
        case alarm_minute:
        hour_twinkling = TRUE;
        minute_twinkling = g_p_set_alarm->twinkling_flag;
        break;

        default:
        break;
    }

    led_display(LCD_COL, 0xff, 0); //���:��
    if (g_p_set_alarm->alarm_state == alarm_onoff)
    {
        if (g_p_set_alarm->cur_alarm_record.alarm.alarm_onoff == TRUE)
        {
            led_display(0, NUM_O, g_p_set_alarm->twinkling_flag);
            led_display(1, NUM_N, g_p_set_alarm->twinkling_flag);
            led_display(2, NUM_0, 0);
            led_display(3, NUM_0, 0);
        }
        else
        {
            led_display(0, NUM_O, g_p_set_alarm->twinkling_flag);
            led_display(1, NUM_F, g_p_set_alarm->twinkling_flag);
            led_display(2, NUM_F, g_p_set_alarm->twinkling_flag);
            led_display(3, NUM_0, 0);
        }
    }
    else
    {
        led_display(0, (hour / 10) + NUM_0, hour_twinkling);
        led_display(1, (hour % 10) + NUM_0, hour_twinkling);
        led_display(2, (minute / 10) + NUM_0, minute_twinkling);
        led_display(3, (minute % 10) + NUM_0, minute_twinkling);
    }

    if (g_p_set_alarm->twinkling_flag == TRUE)
    {
        g_p_set_alarm->twinkling_flag = FALSE;
    }
    else
    {
        g_p_set_alarm->twinkling_flag = TRUE;
    }
}

void tm_alarm_set_alarm_view(view_update_e mode)
{
    switch (mode)
    {
        case VIEW_UPDATE_CREATE:
        {
            alarm_alarm_t *p_alarm_alarm;

            g_p_set_alarm = sys_malloc(sizeof(set_alarm_var_t));
            if (g_p_set_alarm == NULL)
            {
                PRINT_ERR("set alarm view malloc fail!!");
                while (1)
                {
                    ; //nothing for QAC
                }
            }

            //����ֻ�Ե�һ�����ӽ�������
            time_alarm_record_list_operate(RECORD_LIST_READ_BYINDEX, &(g_p_set_alarm->cur_alarm_record), 0);
            p_alarm_alarm = &(g_p_set_alarm->cur_alarm_record.alarm);
            g_p_set_alarm->alarm_onoff_bk = p_alarm_alarm->alarm_onoff;

            //��ʼ������ʱ��
            if (p_alarm_alarm->alarm_onoff == TRUE)
            {
                libc_memcpy(&(g_p_set_alarm->time), &(p_alarm_alarm->alarm_time), sizeof(time_t));
            }
            else
            { //��ʼ��Ϊ��ǰʱ�䣬����0
                sys_get_time(&(g_p_set_alarm->time));
                g_p_set_alarm->time.second = 0;
            }
            g_p_set_alarm->alarm_state = alarm_onoff;
            g_p_set_alarm->setting_digits = 0;
            g_p_set_alarm->digit_flag = FALSE;

            g_p_set_alarm->twinkling_flag = TRUE;
            g_p_set_alarm->twinkling_timer_id = set_app_timer(APP_TIMER_ATTRB_UI | (COM_VIEW_ID_SET_ALARM << 8), 500,
                    alarm_handle);

            __tm_alarm_set_alarm_redraw();
        }
        break;

        case VIEW_UPDATE_DESTROY:
        {
            alarm_record_t *p_alarm_record = &(g_p_set_alarm->cur_alarm_record);
            alarm_alarm_t *p_alarm_alarm = &(p_alarm_record->alarm);

            //ֻҪ��off��on������on״̬�¸�����ʱ�䣬�Ͱ��ա�������������ӡ���ĳЩ���Իָ�Ϊ��������ֵ
            if (((g_p_set_alarm->alarm_onoff_bk == FALSE) && (p_alarm_alarm->alarm_onoff == TRUE))
                    || ((p_alarm_alarm->alarm_onoff == TRUE) && (g_p_set_alarm->alarm_state == alarm_finish)))
            { //Ĭ��IDΪ1������Ϊdefault��ÿ���ظ�������Ϊ���������һ�׸���
                libc_memcpy(&(p_alarm_alarm->alarm_time), &(g_p_set_alarm->time), sizeof(time_t));
                if (p_alarm_alarm->alarm_id == 0)
                { //ID��Ч
                    p_alarm_alarm->alarm_id = 1;
                }
                if (p_alarm_alarm->alarm_name[0] == 0x00)
                { //������Ч
                    sys_os_sched_lock();
                    libc_memcpy(p_alarm_alarm->alarm_name, default_alarm_name, sizeof(default_alarm_name));
                    sys_os_sched_unlock();
                }
                p_alarm_alarm->alarm_day = 0x7f;
                p_alarm_alarm->ring_type = RING_TYPE_BUILTIN;
                p_alarm_alarm->sound.ring_seq = 1;
                p_alarm_alarm->alarm_used = 1;
                libc_memcpy(&(p_alarm_record->last_time), &(p_alarm_alarm->alarm_time), sizeof(time_t));
                p_alarm_record->snooze = 0;
            }

            time_alarm_record_list_operate(RECORD_LIST_ADD_BYINDEX, &(g_p_set_alarm->cur_alarm_record), 0);
            //ˢ�����Ӷ�ʱ����
            time_alarm_record_list_operate(RECORD_LIST_UPDATE_ALARM, UPDATE_ALARM_ADD, 0);
            kill_app_timer(g_p_set_alarm->twinkling_timer_id);

            sys_free(g_p_set_alarm);
            g_p_set_alarm = NULL;
        }
        break;

        case VIEW_UPDATE_REDRAW:
        {
            __tm_alarm_set_alarm_redraw();
        }
        break;

        case VIEW_UPDATE_STATUS:
        {
            __tm_alarm_set_alarm_redraw();
        }
        break;

        default:
        break;
    }
}

//��������2Hz��ʱ����
void alarm_handle(void)
{
    tm_alarm_set_alarm_view(VIEW_UPDATE_REDRAW);
}

