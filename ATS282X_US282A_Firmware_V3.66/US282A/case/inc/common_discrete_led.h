/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������LED����صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __COMMON_DISCRETE_LED_H__
#define __COMMON_DISCRETE_LED_H__

#include "psp_includes.h"
#include "case_independent.h"

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)

#define LED_BL_LINK_PORT     LED_PORT_GPIOA
#define LED_BL_LINK_PIN      (12)

#define LED_MUSIC_PLAY_PORT  LED_PORT_GPIOA
#define LED_MUSIC_PLAY_PIN   (14)

#define LED_AUX_PLAY_PORT    LED_PORT_GPIOA
#define LED_AUX_PLAY_PIN     (13)

#define LED_FM_PLAY_PORT     LED_PORT_GPIOA
#define LED_FM_PLAY_PIN      (22)

#define LED_POWER_PORT       LED_PORT_GPIOA
#define LED_POWER_PIN        (23)

#else

#define LED_BL_LINK_PORT     LED_PORT_GPIOA
#define LED_BL_LINK_PIN      (4)

#endif

//���������;��LED��
typedef enum
{
    LED_ID_BT_LINK = 0, //��������ָʾ��
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    LED_ID_MUSIC_PLAY = 1, //���ֲ���ָʾ��
    LED_ID_AUX_PLAY = 2, //LINEIN����ָʾ��
    LED_ID_FM_PLAY = 3, //����������ָʾ��
    LED_ID_POWER = 4, //��Դָʾ��
#endif
    LED_ID_MAX
} led_id_e;

typedef enum
{
    LED_ALWAYS_OFF = 0,
    LED_ALWAYS_ON = 1,
    LED_TWINKLE = 2,
} led_mode_e;

typedef struct
{
    /*! ѭ��������Ŀ��0��ʾѭ��ֹͣ��-1��ʾһֱѭ��ֱ���������� */
    uint8 cycle_count;
    /*! LED��ʱ�䣬��˸ʱ��Ч����λΪ1ms */
    uint16 duty_on_time;
    /*! LED��ʱ�䣬��˸ʱ��Ч����λΪ1ms */
    uint16 duty_off_time;
} led_duty_t;

//������˸��������led��
typedef struct
{
    /*�˿ڵ�ַ����� led_port_e */
    uint16 led_port : 3;
    /*! GPIO PIN �ţ�0��ʾGPIO0 */
    uint16 led_pin : 5;
    /*! LED�����ȣ�8���ɵ� */
    uint16 lightness : 3;
    /*! ����ģʽ��LED_MODE���ͣ�0��ʾ����1��ʾ������2��ʾ��˸����� led_mode_e */
    uint16 work_mode : 2;
    /*! LED����״̬��0��ʾ��1��ʾ�� */
    uint16 on_off : 1;
    //��ʱ��ID
    int8  timer_id;
    uint8 reserve;
} discrete_led_info_t;

typedef struct
{
    led_port_e port;
    uint8 pin;
} discrete_led_config_t;

extern discrete_led_info_t g_discrete_led_info[LED_ID_MAX];
extern bool g_discrete_led_a23_available;

extern void discrete_led_set(led_id_e led_id, led_mode_e mode, led_duty_t *duty_param) __FAR__;
extern void discrete_led_init(void) __FAR__;
extern void discrete_led_low_power(void) __FAR__;
extern void discrete_led_restore(void) __FAR__;

#endif

