/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������첥����������ʵ���û�������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_alarm.h"

app_result_e alarm_alarm_snooze(uint32 data1, uint32 data2);
app_result_e alarm_alarm_close(uint32 data1, uint32 data2);

app_result_e alarm_key_deal_pause_alarm(void);
app_result_e alarm_key_deal_stop_alarm(void);
void alarm_ring_handle(void);

const key_event_map_t __section__(".rodata.ke_maplist") alarm_playing_ke_maplist[] =
{
    /*! �ص����� */
    { { KEY_MODE, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_stop_alarm },

    /*! ��ͣ���� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, 0 }, alarm_key_deal_pause_alarm }, //����ӳ�䵽��ݼ�����PLAY�ػ�
    { { KEY_PHONEPLAY, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_NEXT, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_PREV, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_VADD, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_VSUB, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_ABREC, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_MUTE, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_FOLDER_ADD, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },
    { { KEY_FOLDER_SUB, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, alarm_key_deal_pause_alarm },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, 0 }, NULL },
};

app_result_e alarm_key_deal_pause_alarm(void)
{
    return RESULT_ALARM_SNOOZE;
}

app_result_e alarm_key_deal_stop_alarm(void)
{
    return RESULT_ALARM_CLOSE;
}

const sys_event_map_t __section__(".rodata.se_maplist") alarm_playing_se_maplist[] =
{
    { { MSG_SD_IN, 0 }, NULL },
    { { MSG_UH_IN, 0 }, NULL },
    { { MSG_LINEIN_IN, 0 }, NULL },
    { { MSG_USB_STICK, 0 }, NULL },

    /*! ������־ */
    { { MSG_NULL, 0 }, NULL },
};

/* COMMON set/get/control������ص�����ע��� */
const rcp_cmd_cb_t __section__(".rodata.alarm_rcp") alarm_rcp_cmd_cb_tbl[] =
{
    //{ RCP_CMD_CONTROL, CONTROL_ALARM_SNOOZE, (void *) alarm_alarm_snooze }, //����SNOOZE
    //{ RCP_CMD_CONTROL, CONTROL_ALARM_SHUTOFF, (void *) alarm_alarm_close }, //���ӹر�

    /* ������ */
    { 0, 0, NULL },
};

/*app_result_e alarm_alarm_snooze(uint32 data1, uint32 data2)
{
    g_rcp_ctrl_quit_flag = TRUE;
    return RESULT_ALARM_SNOOZE;
}

app_result_e alarm_alarm_close(uint32 data1, uint32 data2)
{
    g_rcp_ctrl_quit_flag = TRUE;
    return RESULT_ALARM_CLOSE;
}*/

void alarm_rcp_var_init(void)
{
    com_rcp_set_callback(alarm_rcp_cmd_cb_tbl, NULL);
}

void alarm_rcp_var_exit(void)
{
    com_rcp_set_callback(NULL, NULL);
}



void alarm_playing_main_view(view_update_e mode)
{
    switch (mode)
    {
        case VIEW_UPDATE_CREATE:
        {
            //��ȡ���ں�ʱ��
            sys_get_date(&(g_p_alarm_ring->alarm_date));
            if (g_p_alarm_ring->alarm_date.year > 2099)
            {
                g_p_alarm_ring->alarm_date.year = 2000;
                sys_set_date(&(g_p_alarm_ring->alarm_date));
            }
            sys_get_time(&(g_p_alarm_ring->alarm_time));

            g_p_alarm_ring->twinkling_flag = TRUE;
            g_p_alarm_ring->twinkling_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 500, alarm_ring_handle);

            g_p_alarm_ring->overtime_flag = FALSE;

#if (SUPPORT_LED_DRIVER == 1)
            if (g_comval.support_led_display == 1)
            {
                alarm_ring_view_redraw();
            }
#endif
        }
        break;

        case VIEW_UPDATE_DESTROY:
        {
            if (g_p_alarm_ring->twinkling_timer_id != -1)
            {
                kill_app_timer(g_p_alarm_ring->twinkling_timer_id);
            }
        }
        break;

        case VIEW_UPDATE_REDRAW:
        case VIEW_UPDATE_STATUS:
        {
#if (SUPPORT_LED_DRIVER == 1)
            if (g_comval.support_led_display == 1)
            {
                alarm_ring_view_redraw();
            }
#endif
        }
        break;

        default:
        break;
    }
}

//������2Hz��ʱ����
void check_alarm_overtime(void)
{
    time_t *p_start_alarm = &(g_p_alarm_ring->cur_alarm_record.last_time);
    uint32 start_second, cur_second;

    if (g_has_next_alarm == TRUE)
    {
        if ((g_next_alarm_time.hour == g_p_alarm_ring->alarm_time.hour)
                && (g_next_alarm_time.minute == g_p_alarm_ring->alarm_time.minute)
                && (g_next_alarm_time.second == g_p_alarm_ring->alarm_time.second))
        {
            g_next_alarm_already_ring = TRUE;
            return;
        }
    }

    start_second = p_start_alarm->minute * 60;
    start_second += p_start_alarm->second;

    sys_get_time(&(g_p_alarm_ring->alarm_time));

    cur_second = g_p_alarm_ring->alarm_time.minute * 60;
    cur_second += g_p_alarm_ring->alarm_time.second;

    if (cur_second < start_second)
    {
        cur_second += 3600; //Խ��Сʱ
    }

    if ((cur_second - start_second) >= (g_comval.alarm_overtime * 60))
    {
        g_p_alarm_ring->overtime_flag = TRUE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    TOOLS Ӧ�����Ӳ��ų���������
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e ���� RESULT_APP_QUIT �� RESULT_NULL ��
 * \ingroup
 * \note
 *******************************************************************************/
app_result_e alarm_ring_entry(void)
{
    app_result_e result = RESULT_NULL;
    bool alarm_play_flag = TRUE;
    uint32 temp_flag;
    int ret;
    bool first_alarm_play_init_flag = TRUE;
    uint8 alarm_idx = 0;

    //��������ͼ
    {
        create_view_param_t create_view_param;

        create_view_param.type = VIEW_TYPE_MAIN;
        create_view_param.unknown_key_deal = UNKNOWN_KEY_IGNORE;
        create_view_param.overtime = 0;
        create_view_param.ke_maplist = alarm_playing_ke_maplist;
        create_view_param.se_maplist = alarm_playing_se_maplist;
        create_view_param.view_cb = alarm_playing_main_view;
        com_view_add(APP_VIEW_ID_MAIN, &create_view_param);
    }

    alarm_rcp_var_init();

    alarm_next_again:

    //��ȡ��ǰ���Ӽ�¼
    temp_flag = sys_local_irq_save();
    ret = time_alarm_record_list_operate(RECORD_LIST_READ_CUR, &(g_p_alarm_ring->cur_alarm_record), 0);
    sys_local_irq_restore(temp_flag);
    if (ret < 0)
    {
        PRINT_DBG("���ӣ�û�м�¼");
        //û�е�ǰ��Ч���Ӽ�¼
        return RESULT_NULL;
    }

    alarm_idx = ret;
    //�ر�������ͬʱ�������
    time_alarm_record_list_operate(RECORD_LIST_UPDATE_ALARM, (void *) UPDATE_ALARM_CLOSE_SAME, 0);

    //��ʼ�����Ӳ���
    g_app_info_state.ring_alarm = TRUE;

    alarm_play_init_again:

    alarm_play_flag = alarm_play_init(g_p_alarm_ring);
    if (alarm_play_flag == FALSE)
    {
        result = RESULT_ALARM_CLOSE;
        goto init_fail;
    }
    PRINT_DBG("���ӣ���ʼ����");

    if (TRUE == first_alarm_play_init_flag)
    {
        //�鿴�Ƿ���Ҫ�����ӹ����д�����һ������
        temp_flag = sys_local_irq_save();
        g_next_alarm_index = (uint8) time_alarm_record_list_operate(RECORD_LIST_GET_NEXT_ALARM, &g_next_alarm_time, 0);
        sys_local_irq_restore(temp_flag);
        if (g_next_alarm_index != 0xff)
        {
            g_has_next_alarm = TRUE;
        }
        else
        {
            g_has_next_alarm = FALSE;
        }
    }

    while (1)
    {
        if (g_p_alarm_ring->overtime_flag == TRUE)
        {
            //�رձ������ӣ����ô������ӣ�����ԭ״̬
            result = RESULT_ALARM_CLOSE;
            break;
        }

        if (g_next_alarm_already_ring == TRUE)
        {
            //������ǰ���ӣ��˳���Ȼ�����½�������
            time_alarm_record_list_operate(RECORD_LIST_SET_NEXT_ALARM, (void *) (uint32) g_next_alarm_index, 0);
            break;
        }

        //������Ӳ���״̬
        if (alarm_play_flag == TRUE)
        {
            if (alarm_check_status() == FALSE) //����֮���˳�����
            {
                if (TRUE == first_alarm_play_init_flag)
                {
                    first_alarm_play_init_flag = FALSE;

                    if (RING_TYPE_BUILTIN == g_p_alarm_ring->cur_alarm_record.alarm.ring_type)
                    {
                        result = RESULT_LASTPLAY;
                        return result;
                    }
                    else
                    {
                        //���̲����ڻ��������Ҳ������������������һ��
                        g_p_alarm_ring->cur_alarm_record.alarm.ring_type= RING_TYPE_BUILTIN;
                        g_p_alarm_ring->cur_alarm_record.alarm.sound.ring_seq= 1;
                        time_alarm_record_list_operate(RECORD_LIST_ADD_BYINDEX, &g_p_alarm_ring->cur_alarm_record, \
                                                        (void *) (uint32) alarm_idx);

                        goto alarm_play_init_again;
                    }
                }
                else
                {
                  //�����´�����ʱ�䣬�����ֻ��һ�����ӣ���ô����ر�
                    result = RESULT_ALARM_CLOSE;
                    break;
                }
            }
        }

        result = com_view_loop();
        if (result > RESULT_COMMON_RESERVE)
        {
            break;
        }

        //����10ms�����������
        sys_os_time_dly(1);
    }

    //�˳����Ӳ���
    if ((alarm_play_flag == TRUE) && (result != RESULT_APP_QUIT))
    {
        alarm_play_deinit();
    }

    if (g_next_alarm_already_ring == TRUE)
    {
        g_next_alarm_already_ring = FALSE;

        goto alarm_next_again;
    }

    init_fail:

    g_app_info_state.ring_alarm = FALSE;

    if (result == RESULT_POWER_OFF)
    {
        tm_alarm_alarm_ring_deal(ALARM_RING_CLOSE);
    }
    else if (result != RESULT_ALARM_SNOOZE)
    {
        if (tm_alarm_alarm_ring_deal(ALARM_RING_CLOSE) == RESULT_POWER_OFF)
        {
            result = RESULT_POWER_OFF;
        }
        else if (result == RESULT_ALARM_CLOSE)
        {
            result = RESULT_LASTPLAY;
        }
        else
        {
            ; //nothing for QAC
        }
    }
    else
    {
        PRINT_DBG("���ӣ��Ӻ�");
        if (tm_alarm_alarm_ring_deal(ALARM_RING_SNOOZE) == RESULT_POWER_OFF)
        {
            result = RESULT_POWER_OFF;
        }
        else
        {
            result = RESULT_LASTPLAY;
        }
    }

    alarm_rcp_var_exit();

    //ͨ��APK/APP��ֹ���ӻص�FM����Ҫ����Ƶ��
    if ((g_rcp_ctrl_quit_flag == TRUE) && (result == RESULT_LASTPLAY))
    {
        if (com_ap_switch_ask_next_func(RESULT_LASTPLAY) == APP_FUNC_RADIO)
        {
            g_ap_switch_var.switch_to_radio_by_apkapp = TRUE;
        }
    }

    return result;
}

//������2Hz��ʱ����
void alarm_ring_handle(void)
{
    if (g_comval.alarm_overtime != 0)
    {
        check_alarm_overtime();
    }

#if (SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 1)
    {
        alarm_playing_main_view(VIEW_UPDATE_REDRAW);
    }
#endif
}
