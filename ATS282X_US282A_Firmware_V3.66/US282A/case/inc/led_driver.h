/*******************************************************************************
 *                              us280A
 *                            Module: LED����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       johnsen    2012-12-31 14:16     1.0             build this file
 *******************************************************************************/
/*!
 * \file     led_driver.h
 * \brief    led��������ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   johnsen
 * \version 1.0
 * \date  2012-12-31 14:17
 *******************************************************************************/

#ifndef _LED_DRIVER_H
#define _LED_DRIVER_H

/*! \cond LED_DRIVER_API */
#include "psp_includes.h"
#include "case_independent.h"

/*!
 * \brief
 *  lcd_cmd_e��led������������ö������
 */
typedef enum
{
    /*!��ʾLED*/
    LED_DISPLAY = 0,
    /*!�����Ļ*/
    LED_CLEAR_SCREEN,
    /*!��˸��Ļ*/
    LED_FLASH_SCREEN,
    /*����LED��ʾ*/
    LED_WAKE_UP,
    /*��LED��ʾ*/
    LED_SLEEP,
} led_cmd_e;

//LED��ʾ��ַ������߼�������ʾ�����н����߼��������ӳ�䣩
#define S1 0        //��ַΪ0��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S2 1        //��ַΪ1��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S3 2        //��ַΪ2��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S4 3        //��ַΪ3��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S5 4        //��ַΪ4��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S6 5        //��ַΪ5��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S7 6        //��ַΪ6��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S8 7        //��ַΪ7��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S9 8        //��ַΪ8��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S10 9       //��ַΪ9��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S11 10      //��ַΪ10��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S12 11      //��ַΪ11��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S13 12      //��ַΪ12��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S14 13      //��ַΪ13��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S15 14      //��ַΪ14��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S16 15      //��ַΪ15��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S17 16      //��ַΪ16��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S18 17      //��ַΪ17��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S19 18      //��ַΪ18��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S20 19      //��ַΪ19��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S21 20      //��ַΪ20��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S22 21      //��ַΪ21��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S23 22      //��ַΪ22��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S24 23      //��ַΪ23��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S25 24      //��ַΪ24��ICON�ĵ�ַ������Ϊ0xff��ʾ��ICON��
#define S_NULL 25

#define NUMBER1 S1      //��һ�����ֵĵ�ַ����ICON��ַ��ͳһ����
#define NUMBER2 S2      //�ڶ������ֵĵ�ַ����ICON��ַ��ͳһ����
#define NUMBER3 S3      //���������ֵĵ�ַ����ICON��ַ��ͳһ����
#define NUMBER4 S4      //���ĸ����ֵĵ�ַ����ICON��ַ��ͳһ����

//for 04311
// #define LED_SD  S1
// #define LED_USB S2
// #define LED_AUX S6
// #define LCD_COL S7
// #define LED_FM  S8  ;FM��.Ϊͬһ��ICON��ֻ����"FM"ICON��"."ICONͬʱ����
// #define LED_PLAY S17
// #define LED_PAUSE S18

//�±��ICON ��ַ���伯���˶�������ܵĳ��ϣ�Ϊ�����ܾ����ܶ��֧�ֲ�ͬ
//����ܶ������޸Ļ����޸ġ�

#if (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2825)

#define LCD_SD    S18   //ICON "SD"
#define LCD_USB   S2   //ICON "USB"
#define LCD_P1    S_NULL   //ICON "."
#define LCD_TIMER S_NULL   //ICON "CLOCK"
#define LCD_TEMER S_NULL   //ICON "TEMPLETURE"
#define LCD_AUX   S13   //ICON "AUX"
#define LCD_COL   S7   //ICON ":"
#define LCD_FM    S17   //ICON "FM"
#define LCD_SOUND S_NULL   //ICON "sound"
#define LCD_BAT1  S_NULL  //ICON "bat1"
#define LCD_BAT2  S_NULL  //ICON "bat2"
#define LCD_BAT3  S_NULL  //ICON "bat3"
#define LCD_PMHZ  S_NULL  //ICON "PM"
#define LCD_NO1   S_NULL  //ICON "NO."
#define LCD_VOL   S_NULL  //ICON "VOL"
#define LCD_MUSIC S_NULL  //ICON "MUSIC"
#define LCD_PLAY  S1      //ICON "PLAY"
#define LCD_PAUSE S8      //ICON "PAUSE"
#define ICON_TOTAL 18

#else

#define LCD_SD    S18   //ICON "SD"
#define LCD_USB   S13   //ICON "USB"
#define LCD_P1    S_NULL   //ICON "."
#define LCD_TIMER S_NULL   //ICON "CLOCK"
#define LCD_TEMER S_NULL   //ICON "TEMPLETURE"
#define LCD_AUX   S1   //ICON "AUX"
#define LCD_COL   S7   //ICON ":"
#define LCD_FM    S17   //ICON "FM"
#define LCD_SOUND S_NULL   //ICON "sound"
#define LCD_BAT1  S_NULL  //ICON "bat1"
#define LCD_BAT2  S_NULL  //ICON "bat2"
#define LCD_BAT3  S_NULL  //ICON "bat3"
#define LCD_PMHZ  S_NULL  //ICON "PM"
#define LCD_NO1   S_NULL  //ICON "NO."
#define LCD_VOL   S_NULL  //ICON "VOL"
#define LCD_MUSIC S_NULL  //ICON "MUSIC"
#define LCD_PLAY  S8  //ICON "PLAY"
#define LCD_PAUSE S2  //ICON "PAUSE"
#define ICON_TOTAL 18

#endif




//��������ȡֵ�����
#define NUM_0  0
#define NUM_1  1
#define NUM_2  2
#define NUM_3  3
#define NUM_4  4
#define NUM_5  5
#define NUM_6  6
#define NUM_7  7
#define NUM_8  8
#define NUM_9  9
#define NUM_A  10
#define NUM_B  11
#define NUM_C  12
#define NUM_D  13
#define NUM_E  14
#define NUM_F  15
#define NUM_G  16
#define NUM_H  17
#define NUM_I  18
#define NUM_J  19
#define NUM_K  20
#define NUM_L  21
#define NUM_M  22   //can't display
#define NUM_N  23
#define NUM_O  24
#define NUM_P  25
#define NUM_Q  26
#define NUM_R  27
#define NUM_S  28
#define NUM_t  29
#define NUM_T  29   //the same as 't'
#define NUM_U  30
#define NUM_V  31   //the same as U
#define NUM_W  32   //can't display
#define NUM_X  33   //can't display
#define NUM_y  34
#define NUM_Z  35
#define NUM_b  36
#define NUM_ERROR  37   //'-',���������ֵ��������ֵ���˴���
#define NUMBER_TOTAL 38

#define ICON_NUM 7
#define ADDR_NUM 5
#define NUMBERADDR 4

#define reg_writeb(val, reg)  *(volatile unsigned char *)(reg) = (val)
#define reg_writew(val, reg)  *(volatile unsigned short *)(reg) = (val)
#define reg_writel(val, reg)  *(volatile unsigned int *)(reg) = (val)
#define reg_readb(port)      (*(volatile unsigned char *)(port))
#define reg_readw(port)      (*(volatile unsigned short *)(port))
#define reg_readl(port)      (*(volatile unsigned int *)(port))

///���ϲ����ͳһ�ӿ�����
extern void *lcd_op_entry(void *param1,
        void *param2, void *param3, led_cmd_e cmd)__FAR__;

//for welcome module
extern void sdrv_led_display(uint8 addr, uint8 content, uint8 type)__FAR__; //��ʾLED
extern void sdrv_clear_screen(void *null1, void *null2, void *null3)__FAR__; //�����Ļ
extern void sdrv_flash_screen(uint8 onoff, void *null2, void *null3)__FAR__; //��˸��Ļ
extern uint8 sdrv_wake_up_LED_display(uint8, uint8, uint8)__FAR__; //����LED��ʾ
extern void sdrv_sleep_LED_display(uint8, uint8, uint8)__FAR__; //0.5��ִ��һ�μ�ʱ����

#if (SUPPORT_LED_DRIVER == 1)

/*!
 * \brief��ʾLED
 * input: addr--��ַ��ȡֵ��ICON����S1��S25����
 *              ���֡���ĸ����NUMBER1-NUMBER4��
 *        content--�������ݣ�0xff��ʾICON��
 *              �����ʾ���ֻ���ĸ,ȡֵ��NUM_0-NUM_Z
 *        type--��ʾ����ʾ��0--����ʾ����0--��ʾ
 */

#define led_display(uint8_addr,uint8_content,uint8_type) \
lcd_op_entry((void *)(uint32)(uint8_addr), (void *)(uint32)(uint8_content), (void *)(uint32)(uint8_type), LED_DISPLAY)

/*!
 * \brief
 *   �����Ļ
 */
#define led_clear_screen() \
lcd_op_entry((void *)(0), (void *)(0), (void *)(0), LED_CLEAR_SCREEN)

/*!
 * \brief
 *   ��˸��Ļ
 *input: onoff--��˸���أ�ȡֵ��1��ʾ������ʾԭ���ݣ�0��ʾ�أ�������ݣ�
 */
#define led_flash_screen(onoff) \
lcd_op_entry((void *)(uint32)(onoff), (void *)(0), (void *)(0), LED_FLASH_SCREEN)

#define led_wake_up_display()  \
((uint8)(uint32)lcd_op_entry((void *)(0), (void *)(0), (void *)(0), LED_WAKE_UP))

#define led_sleep_display() \
lcd_op_entry((void *)(0), (void *)(0), (void *)(0),LED_SLEEP)

#else

#define led_display(addr,content,type)
#define led_clear_screen()
#define led_flash_screen(onoff)
#define led_wake_up_display()
#define led_sleep_display()

#endif

/*! \endcond */

#endif //_LED_DRIVER_H
