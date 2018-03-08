

#include "app_alarm.h"



//��������ʾ
void alarm_ring_view_redraw(void)
{
    uint16 hour, minute;
    hour = g_p_alarm_ring->alarm_time.hour;
    minute = g_p_alarm_ring->alarm_time.minute;

    led_display(LCD_COL, 0xff, 0); //���:��
    led_display(LCD_FM, 0xff, 0); //���.�ż�FM���
    led_display(LCD_NO1, 0xff, 0); //���.�ż�FM���

    led_display(0, (hour / 10) + NUM_0, g_p_alarm_ring->twinkling_flag);
    led_display(1, (hour % 10) + NUM_0, g_p_alarm_ring->twinkling_flag);
    led_display(LCD_COL, 0xff, g_p_alarm_ring->twinkling_flag);
    led_display(2, (minute / 10) + NUM_0, g_p_alarm_ring->twinkling_flag);
    led_display(3, (minute % 10) + NUM_0, g_p_alarm_ring->twinkling_flag);

    if (g_p_alarm_ring->twinkling_flag == TRUE)
    {
        g_p_alarm_ring->twinkling_flag = FALSE;
    }
    else
    {
        g_p_alarm_ring->twinkling_flag = TRUE;
    }
    
}

