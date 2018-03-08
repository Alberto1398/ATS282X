/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TWI GPIOģ���ģ�����ģ���ⲿ�ӿ���������Щ�ӿ�����ֻ�����ڲ�ʹ�ã��ⲿ
 *       ģ��Ҫ������Щ�����������Լ������������BANK����������Щ���������뽫����
 *       ��Ϊ __FAR__ �ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _TWI_INNER_H
#define _TWI_INNER_H

#include "psp_includes.h"

extern uint8 TWI_Trans_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
extern uint8 TWI_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
extern void TWI_Start(gpio_init_cfg_t* gpio, uint8* delay);
extern void TWI_Stop(gpio_init_cfg_t* gpio, uint8* delay);
extern void TWI_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay);
extern uint8 TWI_ReadByte(gpio_init_cfg_t* gpio, uint8* delay);
extern void TWI_Init(gpio_init_cfg_t* gpio, uint8* delay);
extern void TWI_Exit(gpio_init_cfg_t* gpio);
extern void TWI_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay);
extern uint8 TWI_GetAck(gpio_init_cfg_t* gpio, uint8* delay);

#endif /*_TWI_INNER_H */

