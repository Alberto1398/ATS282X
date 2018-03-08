#ifndef _KEY_INTERFACE_H            //��ֹ�ض���
#define _KEY_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"
#include "common_bat_charge.h"

#define PER_DETECT_PERIOD           200 //��Χ�豸�������
#define CARD_DETECT_TIMER           2   //���������Ϊ400ms = 200ms * 2
#define USBUHOST_DETECT_TIMER       2   //USB/UHOST�������Ϊ400ms����⣩ + 200ms���л���ȴ�200ms��ʹ�ܣ�

#define PER_DETECT_CARD             0x00000001 //Ҫ��⿨���ȽϺ�ʱ�����Լ�����ڻ᳤һ��
#define PRE_DETECT_CARD_FOR_IN      0x00000002 //��λ��ʾ��ǰ�����ڣ���⿨����
#define PER_DETECT_CARD_TEST        0x00000004 //���Կ��Ƿ����
#define PER_DETECT_CARD_FORCE       0x00000080 //ǿ�ƽ��п���⣬һ����ǰ̨Ӧ�ø��л�������ǿ�Ƽ��һ��
#define PER_DETECT_AUX              0x00000100 //Ҫ���AUX
#define PER_DETECT_AUX_FOR_IN       0x00000200 //��λ��ʾ��ǰAUX���ڣ����AUX����
#define PER_DETECT_AUX_TEST         0x00000400 //����AUX�Ƿ����
#define PER_DETECT_HP               0x00010000 //Ҫ���HPEAPHONE
#define PER_DETECT_HP_FOR_IN        0x00020000 //��λ��ʾ��ǰHPû���ţ����HP����
#define PER_DETECT_HP_TEST          0x00040000 //����HP�Ƿ����
#define PER_DETECT_USBUHOST_SWITCH  0x01000000 //Ҫ�л�USB/UHOST
#define PER_DETECT_UHOST_TEST       0x02000000 //����UHOST�Ƿ����
#define PER_DETECT_USB_FORCE        0x40000000 //ǿ���л���USB
#define PER_DETECT_UHOST_FORCE      0x80000000 //ǿ���л���UHOST

/*1.�����ṩ�ľ���ӿ���������ǰ�������ͳһ�ṩ�Ľӿ�*/

typedef enum
{
    TK_OP_ON = 0,
    TK_OP_OFF = 1,
    TK_OP_CHECK = 2,
} tk_op_e;

typedef enum
{
    KEY_BATTERY_CHARGE_OPEN = 0,
    KEY_BATTERY_CHARGE_STOP,
    KEY_BATTERY_GET_STATUS,
    KEY_ADJUST_FREQ_OP,
    KEY_PERIPHERAL_DETECT,
    KEY_REGISTER_SCAN_ISR_CBK,
    KEY_DISCRETE_LED_OP,
#if (SUPPORT_TK_DRIVER == TK_DRIVER_OUTER)
    KEY_EXTERN_TK_OP,
#endif
    SET_EFFECT_INFO,
    GET_FEATURE_INFO,
    SET_WAVES_EFFECT_INFO,
    
    KEY_ADDRESS_TRANSLATION,
    
    KEY_MAX,
} key_cmd_e;

extern void *key_op_entry(void *param1, void *param2, void *param3, key_cmd_e cmd)__FAR__;

#define key_battery_charge_open(current_ma, voltage_mv)   \
key_op_entry((void*)(uint32)(current_ma),(void*)(uint32)(voltage_mv),(void*)0, KEY_BATTERY_CHARGE_OPEN)
#define key_battery_charge_stop()    \
key_op_entry((void*)0,(void*)0,(void*)0, KEY_BATTERY_CHARGE_STOP)
#define key_battery_get_status(p_ad_val, p_vol_limit, app_id)    \
(battery_status_e)key_op_entry((void*)(p_ad_val),(void*)(p_vol_limit),(void*)app_id, KEY_BATTERY_GET_STATUS)

#if (SUPPORT_TK_DRIVER == TK_DRIVER_OUTER)
#define key_extern_tk_op(tk_op)    \
(uint8)key_op_entry((void*)0,(void*)0,(void*)(uint32)(tk_op), KEY_EXTERN_TK_OP)
#endif

/*������Ч����*/
/*int32 key_inner_set_effect_param(uint32 set_type, void *param_ptr, void *null3)*/
/*set_type:��������;param_ptr:��Ч����ָ��*/
#define set_effect_param(a,b) (int32)key_op_entry((void*)(uint32)(a), (void*)(b), (void*)(0), SET_EFFECT_INFO)

/*��ȡ��Ч������Ϣ*/
/*int32 key_inner_get_feature_info(uint32 set_type, void *info_ptr, void *null3)*/
/*set_type:��Ч����;info_ptr:��Ϣָ��*/
#define get_feature_info(a,b) (int32)key_op_entry((void*)(uint32)(a), (void*)(b), (void*)(0), GET_FEATURE_INFO)

/*����WAVES��Ч����*/
/*int32 key_inner_set_waves_effect_param(uint32 set_type, void *param_ptr, void *null3)*/
/*set_type:��������;param_ptr:��Ч����ָ��*/
#define waves_set_effect_param(a,b) (int32)key_op_entry((void*)(uint32)(a), (void*)(b), (void*)(0), SET_WAVES_EFFECT_INFO)

/*mips��dsp֮��ĵ�ַת��*/
/*int32 key_inner_address_translation(uint32 type, void *param_ptr, void *null3)*/
/*type:address_translation_e;param_ptr:��Ҫת����ַ�ĵ�ַ*/

#define key_address_translation(a,b) (int32)key_op_entry((void*)(uint32)(a), (void*)(b), (void*)(0), KEY_ADDRESS_TRANSLATION)


/***************************************/

typedef enum
{
    FREQ_NULL = 0,  //������MIPS��DSPƵ�ʣ���Ƶ��ֵΪ0
    FREQ_LEVEL1,    //�����MIPS->1MM, �����DSP->35M
    FREQ_LEVEL2,    //�����MIPS->2M,  �����DSP->40M
    FREQ_LEVEL3,    //�����MIPS->4M,  �����DSP->55M
    FREQ_LEVEL4,    //�����MIPS->6M,  �����DSP->60M
    FREQ_LEVEL5,    //�����MIPS->8M,  �����DSP->70M
    FREQ_LEVEL6,    //�����MIPS->10M, �����DSP->80M
    FREQ_LEVEL7,    //�����MIPS->20M, �����DSP->100M
    FREQ_LEVEL8,    //�����MIPS->30M, �����DSP->120M
    FREQ_LEVEL9,    //�����MIPS->40M, �����DSP->140M
    FREQ_LEVEL10,   //�����MIPS->60M, �����DSP->160M
    FREQ_LEVEL11,   //�����MIPS->85M, �����DSP->180M
    FREQ_LEVEL_MAX
} adjust_freq_level_e;

//��Ƶ�ӿ�
typedef enum
{
    ADJUST_FQ_SET_LEVEL,
    ADJUST_FQ_ADD_VALUE,
    ADJUST_FQ_CMD_MAX,
} adjust_freq_cmd_e;


/*
 int adjust_freq_set_level(uint8 prio,uint8 level_a,uint8 level_b)
 �ýӿ����ڵ���misp �� dsp Ƶ�ʵ�λ
 prio���̶߳�Ӧ�����ȼ�
 level_a��misp��Ƶ��λ��adjust_freq_level_e,��Ϊ FREQ_NULL ��ʾ����Ƶ��
 level_b��dsp��Ƶ��λ��adjust_freq_level_e����Ϊ FREQ_NULL ��ʾ����Ƶ��
 ����ֵint��0-�ɹ�����0ʧ��
 !!!NOTE:bank���룬��ֹ���жϵ���
 */
#define adjust_freq_set_level(prio,level_a,level_b)  \
(int)key_op_entry((void*)(uint32)(prio),(void*)(((uint32)(level_b) << 8) | ((uint32)(level_a))),(void*)ADJUST_FQ_SET_LEVEL, KEY_ADJUST_FREQ_OP)


/*
 int adjust_freq_add_value(int16 freq_a,int16 freq_b)
 �ýӿ�����΢�� misp �� dsp Ƶ��ֵ,�ɵ��߻����
 freq_a������misp��Ƶ�ʣ�ֵ��Ϊ����
 freq_b������dsp��Ƶ�ʣ�ֵ��Ϊ����
 ����ֵint��0-�ɹ�����0ʧ��
 !!!NOTE:bank���룬��ֹ���жϵ���
 */
#define adjust_freq_add_value(freq_a,freq_b)  \
(int)key_op_entry((void*)(int)(int16)(freq_a),(void*)(int)(int16)(freq_b),(void*)ADJUST_FQ_ADD_VALUE, KEY_ADJUST_FREQ_OP)


/*
 int key_peripheral_detect_handle(uint32 detect_mode)
 �ýӿ��������ڽ����ⲿ�豸��⣬�������Ϊ200ms���������Լ������������Ϊ����200ms
 detect_mode����ʾҪ�����Щ���裬��� PER_DETECT_XXX �궨��
 ����ģʽ����ʱ������1��ʾ������ţ�����0��ʾ����û����
 */
#define key_peripheral_detect_handle(detect_mode)  \
(int)key_op_entry((void*)detect_mode,(void*)0,(void*)0, KEY_PERIPHERAL_DETECT)

#define key_register_scan_isr_cbk(cbk)  \
(int)key_op_entry((void*)(cbk),(void*)0,(void*)0, KEY_REGISTER_SCAN_ISR_CBK)


typedef enum
{
    LED_PORT_GPIOA = 0,
    LED_PORT_GPIOB = 1,
    LED_PORT_PWM0 = 2,
    LED_PORT_PWM1 = 3,
    LED_PORT_PWM2 = 4,
    LED_PORT_PWM3 = 5,
} led_port_e;

#define LED_ON_GPIO_HIGH  0
#define LED_ON_GPIO_LOW   1
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
#define LED_ON_GPIO_SEL   LED_ON_GPIO_LOW
#else
#define LED_ON_GPIO_SEL   LED_ON_GPIO_HIGH
#endif

//����LED�ƽӿ�
typedef enum
{
    DISCRETE_LED_INIT,
    DISCRETE_LED_ON,
    DISCRETE_LED_OFF,
} discrete_led_op_e;


#define key_discrete_led_init(port,pin)  \
(int)key_op_entry((void*)(uint32)(port),(void*)(uint32)(pin),(void*)DISCRETE_LED_INIT, KEY_DISCRETE_LED_OP)

#define key_discrete_led_on(port,pin)  \
(int)key_op_entry((void*)(uint32)(port),(void*)(uint32)(pin),(void*)DISCRETE_LED_ON, KEY_DISCRETE_LED_OP)

#define key_discrete_led_off(port,pin)  \
(int)key_op_entry((void*)(uint32)(port),(void*)(uint32)(pin),(void*)DISCRETE_LED_OFF, KEY_DISCRETE_LED_OP)

#endif

