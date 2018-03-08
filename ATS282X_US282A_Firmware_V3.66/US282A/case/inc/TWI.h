/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TWI GPIOģ���ģ�����ģ��궨�壬�ṹ�嶨��ȡ�
 *       NOTE!!! �ӿ������������ģ���Լ�������
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _TWI_H
#define _TWI_H

#include "psp_includes.h"
#include "case_independent.h"

#if ((CASE_BOARD_TYPE == CASE_BOARD_EVB) || (CASE_BOARD_TYPE == CASE_BOARD_ATS2823) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823))

/* ģ��TWI ����GPIO ���ú�*/
#define  TWI_SCL_BIT        31
#define  TWI_SDA_BIT        15
#define  GPIO_SCL_BIT    (0x00000001<<TWI_SCL_BIT)
#define  GPIO_SDA_BIT    (0x00000001<<TWI_SDA_BIT)

#endif

#if (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2825)

/* ģ��TWI ����GPIO ���ú�*/
#define  TWI_SCL_BIT        0
#define  TWI_SDA_BIT        1
#define  GPIO_SCL_BIT    (0x00000001<<TWI_SCL_BIT)
#define  GPIO_SDA_BIT    (0x00000001<<TWI_SDA_BIT)

#endif

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)

/* ģ��TWI ����GPIO ���ú�*/
#define  TWI_SCL_BIT        15
#define  TWI_SDA_BIT        31
#define  GPIO_SCL_BIT    (0x00000001<<TWI_SCL_BIT)
#define  GPIO_SDA_BIT    (0x00000001<<TWI_SDA_BIT)

#endif

/*TWI GPIO ����ṹ */
typedef struct
{
    uint32 reg_in; //����ʹ�ܼĴ���
    uint32 reg_out; //���ʹ�ܼĴ���
    uint32 reg_data; //���ݼĴ���
    uint32 reg_bit; //bit λ
} gpio_init_cfg_t;

#endif /*_TWI_H */

