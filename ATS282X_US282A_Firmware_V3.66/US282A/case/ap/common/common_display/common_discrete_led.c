/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������LED�ƹ���ģ�飬������ʼ�������ã��Լ���˸��ʱ���ص�����������LED��
 *       ֧�ֺ�����ģʽ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

const discrete_led_config_t discrete_led_cfg[LED_ID_MAX] =
{
    { LED_BL_LINK_PORT, LED_BL_LINK_PIN },
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    { LED_MUSIC_PLAY_PORT, LED_MUSIC_PLAY_PIN },
    { LED_AUX_PLAY_PORT, LED_AUX_PLAY_PIN },
    { LED_FM_PLAY_PORT, LED_FM_PLAY_PIN },
    { LED_POWER_PORT, LED_POWER_PIN },
#endif
};

void discrete_led_twinkle(void);

//��ʼ������LED��
void discrete_led_init(void)
{
    uint8 n;

    g_discrete_led_a23_available = ((uint8) com_get_config_default(SETTING_UART_PRINT_ENABLE) == 0);

    for (n = 0; n < LED_ID_MAX; n++)
    {
        //��timer_id������Ϊ-1
        //��Ϊ�����磺���ε���discrete_led_set, ��2��Ӧ�ðѵ�һ���õĶ�ʱ���ص���
        //����һ�ζ�ʱ���Ƿ����ã����и����(-1��ʾû�����ã�������ʾ����)
        g_discrete_led_info[n].timer_id = -1;

        g_discrete_led_info[n].led_port = discrete_led_cfg[n].port;
        g_discrete_led_info[n].led_pin = discrete_led_cfg[n].pin;
        g_discrete_led_info[n].lightness = 0; //��ʱ����
        g_discrete_led_info[n].work_mode = LED_ALWAYS_OFF;
        g_discrete_led_info[n].on_off = FALSE;

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
        if ((g_discrete_led_a23_available == FALSE) && (n == LED_ID_POWER))
        { //���A23��UART TXռ�ã��Ͳ�Ҫ��ʼ��֮
            continue;
        }
#endif
        key_discrete_led_init(discrete_led_cfg[n].port, discrete_led_cfg[n].pin);
    }
}

//���÷���LED�ƣ���/��/��˸Ч��
void discrete_led_set(led_id_e id, led_mode_e mode, led_duty_t *duty_param)
{
    if (id >= LED_ID_MAX)
    {
        return;
    }

    if ((mode == LED_TWINKLE) && (duty_param == NULL))
    {
        mode = LED_ALWAYS_ON;
    }

    g_discrete_led_info[id].work_mode = mode;

    if (g_discrete_led_info[id].timer_id != -1)
    {
        kill_app_timer_safe(g_discrete_led_info[id].timer_id, discrete_led_twinkle);
        g_discrete_led_info[id].timer_id = -1;
    }


    if (mode == LED_ALWAYS_OFF)
    {
        if (g_discrete_led_info[id].on_off == TRUE)
        {
            //���LED��
            g_discrete_led_info[id].on_off = FALSE;
            key_discrete_led_off(g_discrete_led_info[id].led_port, g_discrete_led_info[id].led_pin);
        }
    }
    else
    {
        if (g_discrete_led_info[id].on_off == FALSE)
        {
            //����LED��
            g_discrete_led_info[id].on_off = TRUE;
            key_discrete_led_on(g_discrete_led_info[id].led_port, g_discrete_led_info[id].led_pin);
        }

        if (mode == LED_TWINKLE)
        {
            twinkle_app_timer_param_t twinkle_param;

            twinkle_param.count = duty_param->cycle_count;
            twinkle_param.on_off = TRUE;
            twinkle_param.timeout_on = duty_param->duty_on_time;
            twinkle_param.timeout_off = duty_param->duty_off_time;
            twinkle_param.func_proc = discrete_led_twinkle;

            g_discrete_led_info[id].timer_id = set_twinkle_app_timer(
                    APP_TIMER_ATTRB_COM_CONTROL | (APP_TIMER_TAG_SYS << 8),
                    &twinkle_param);
        }
    }
}

//���͹����������LED��
void discrete_led_low_power(void)
{
    uint8 n;

    g_discrete_led_a23_available = ((uint8) com_get_config_default(SETTING_UART_PRINT_ENABLE) == 0);

    for (n = 0; n < LED_ID_MAX; n++)
    {
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
        if ((g_discrete_led_a23_available == FALSE) && (n == LED_ID_POWER))
        { //���A23��UART TXռ�ã��Ͳ�Ҫ��ʼ��֮
            continue;
        }
#endif

        if (g_discrete_led_info[n].on_off == TRUE)
        {
            key_discrete_led_off(g_discrete_led_info[n].led_port, g_discrete_led_info[n].led_pin);
        }
    }
}

//�˳��͹��Ļָ�LED��
void discrete_led_restore(void)
{
    uint8 n;

    g_discrete_led_a23_available = ((uint8) com_get_config_default(SETTING_UART_PRINT_ENABLE) == 0);

    for (n = 0; n < LED_ID_MAX; n++)
    {
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
        if ((g_discrete_led_a23_available == FALSE) && (n == LED_ID_POWER))
        { //���A23��UART TXռ�ã��Ͳ�Ҫ��ʼ��֮
            continue;
        }
#endif

        if (g_discrete_led_info[n].on_off == TRUE)
        {
            key_discrete_led_on(g_discrete_led_info[n].led_port, g_discrete_led_info[n].led_pin);
        }
    }
}

//����led����˸Ч��
void __section__(".text.led_twinkle") discrete_led_twinkle(void)
{
    uint8 i;

    for (i = 0; i < LED_ID_MAX; i++)
    {
        if ((g_discrete_led_info[i].timer_id & 0x3f) == g_app_timer_id) //COMMON��ʱ��IDֻ�е�6bit����
        {
            break;
        }
    }

    if (i == LED_ID_MAX)
    {
        //���������Ӧ�÷���
        return;
    }

    if (g_discrete_led_info[i].on_off == TRUE)
    {
        //���LED��
        g_discrete_led_info[i].on_off = FALSE;
        key_discrete_led_off(g_discrete_led_info[i].led_port, g_discrete_led_info[i].led_pin);
    }
    else
    {
        //��LED��
        g_discrete_led_info[i].on_off = TRUE;
        key_discrete_led_on(g_discrete_led_info[i].led_port, g_discrete_led_info[i].led_pin);
    }
}

