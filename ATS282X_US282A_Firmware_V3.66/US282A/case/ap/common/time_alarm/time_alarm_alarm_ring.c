/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������������ִ��������Ƿ���Ӧ���֣��������ֽ������޸�����ʱ��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_time_alarm.h"
#include "vm_def.h"

//��������
static uint8 get_weekofday(uint16 y, uint8 m, uint8 d)
{
    int8 wk; //����
    uint8 ic; //��ݵ�ǰ��λ
    uint8 ir; //��ݵĺ���λ

    if (m < 3)
    {
        m += 12;
        y--;
    }
    ic = (uint8) (y / 100);
    ir = (uint8) (y % 100);
    //Taylor's formula
    wk = (int8) ((ic / 4) + ir + (ir / 4) + (26 * (m + 1)) / 10 - 2 * ic + d - 1); // ���⸺��
    while (wk >= 7) //�������
    {
        wk -= 7;
    }
    while (wk < 0)
    {
        wk += 7;
    }
    return wk;
}

/*��������Ƿ�Ϸ�����ƥ������*/
static bool check_alarm_day(uint8 alarm_day, date_t *alarm_date)
{
    uint8 weekofday = get_weekofday(alarm_date->year, alarm_date->month, alarm_date->day);

    if (alarm_day != 0)
    { //ѭ��
        if ((alarm_day & (1 << weekofday)) != 0)
        { //ƥ��
            return TRUE;
        }
        else
        { //��ƥ��
            return FALSE;
        }
    }
    else
    { //ֻ��һ�����ӣ�����ƥ������
        return TRUE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    TOOLS Ӧ�����Ӳ��Ŵ���
 * \param[in]    alarm_ring_mode ALARM_RING_ENTER, ALARM_RING_CLOSE, ALARM_RING_SNOOZE
 * \param[out]   none
 * \return       app_result_e ���� RESULT_APP_QUIT �� RESULT_NULL ��
 * \ingroup
 * \note
 *******************************************************************************/
app_result_e tm_alarm_alarm_ring_deal(alarm_ring_mode_e alarm_ring_mode)
{
    alarm_record_t cur_alarm_record;
    app_result_e result = RESULT_NULL;
    date_t alarm_date;

    //��ȡ��ǰ���Ӽ�¼
    if (time_alarm_record_list_operate(RECORD_LIST_READ_CUR, &cur_alarm_record, 0) < 0)
    {
        //û�е�ǰ��Ч���Ӽ�¼
        return RESULT_NULL;
    }

    //��ȡ���ں�ʱ��
    sys_get_date(&alarm_date);
    if (alarm_date.year > 2099)
    {
        alarm_date.year = 2000;
        sys_set_date(&alarm_date);
    }

    //�ս��룬��������Ƿ�Ϸ���
    if (alarm_ring_mode == ALARM_RING_ENTER)
    {
        PRINT_INFO("alarm enter");

        if ((cur_alarm_record.snooze == 0) //ֻ�г������ֲ���Ҫƥ������
        && (check_alarm_day(cur_alarm_record.alarm.alarm_day, &alarm_date) == FALSE))
        { //���ڲ�ƥ�䣬��������ʱ��������˳�
          //�����´�����ʱ�䣬�����ֻ��һ�����ӣ���ô����ر�
            alarm_ring_mode = ALARM_RING_CLOSE;
            result = RESULT_NULL;
        }
    }
    else if (alarm_ring_mode == ALARM_RING_ENTER_CLOSE_AUTO)
    {
        PRINT_INFO("alarm enter, but close auto!");
        alarm_ring_mode = ALARM_RING_CLOSE;
        result = RESULT_NULL;
    }
    else
    {
        PRINT_INFO("alarm stop");
    }

    //�رջ�����ʱ���ӣ���������ʱ��
    if ((alarm_ring_mode == ALARM_RING_CLOSE) || (alarm_ring_mode == ALARM_RING_SNOOZE))
    {
        if (alarm_ring_mode == ALARM_RING_SNOOZE)
        {
            if ((sys_comval->snooze_max != 0)
                    && (cur_alarm_record.snooze >= sys_comval->snooze_max))
            {
                alarm_ring_mode = ALARM_RING_CLOSE;
            }
        }

        if (alarm_ring_mode == ALARM_RING_CLOSE)
        {
            time_alarm_record_list_operate(RECORD_LIST_UPDATE_ALARM, (void *) UPDATE_ALARM_CLOSE, 0);
        }
        else
        {
            time_alarm_record_list_operate(RECORD_LIST_UPDATE_ALARM, (void *) UPDATE_ALARM_SNOOZE, 0);
        }

        if (g_ap_switch_var.rtcalarm_poweron == TRUE)
        {
            g_ap_switch_var.rtcalarm_poweron = FALSE;
            g_ap_switch_var.rtcalarm_poweroff = TRUE;
            result = RESULT_POWER_OFF;
        }
    }
    else
    {
        result = RESULT_ENTER_ALARM;
    }

    PRINT_INFO_INT("alarm go to:", result);

    return result;
}
