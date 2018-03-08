/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������ص�����صĺ궨�壬ö�����ͣ��ṹ�嶨��ȣ���ͷ�ļ�ΪKEY������
 *       COMMON ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __KEY_BAT_CHARGE__H
#define __KEY_BAT_CHARGE__H

#include  "psp_includes.h"

/*! ��ص�ѹ�ȼ�����0 ~ 9��0��ʾ��ʾ�͵�ػ���1��ʾ�͵�����ʾ */
#define BATTERY_GRADE_MAX      9

/* ������ */
typedef enum
{
    Chg_current_25mA = 0,
    Chg_current_50mA,
    Chg_current_100mA,
    Chg_current_200mA,
    Chg_current_300mA,
    Chg_current_400mA,
    Chg_current_500mA,
    Chg_current_600mA,
} chg_current_e;

/* ��ѹ����ѹ */
typedef enum
{
    Chg_voltage_4200mV = 0,
    Chg_voltage_4230mV,
    Chg_voltage_4260mV,
    Chg_voltage_4290mV,
    Chg_voltage_4320mV,
    Chg_voltage_4350mV,
    Chg_voltage_4380mV,
    Chg_voltage_4410mV
} chg_voltage_e;

/* �������ѹ */
typedef enum
{
    Chg_FullVol_4160mV = 0,
    Chg_FullVol_4180mV,
    Chg_FullVol_4320mV,
    Chg_FullVol_4340mV
} chg_FullVol_e;

typedef enum
{
    BAT_NORMAL = 0, //��ع����Ҳ��ڳ��״̬
    BAT_CHARGING, //���ڳ��
    BAT_FULL, //����
    BAT_NO_EXIST, //û�е��
    BAT_CHECKING, //���ڼ������
} battery_status_e;



#define BAT_LOW_WARNING_CYCLE  120  //Լ60��,��ֵҪ����10��
#define BAT_FULL_WARNING_CYCLE  120  //Լ60�룬��ֵҪ����10��


/*! ��ص�ѹ�������ݼ�¼���� */
#define BAT_ADC_DATA_NUM    50

#define bat_check_open()       act_writel((act_readl(CHG_CTL) | (1 << CHG_CTL_ENBATDT)), CHG_CTL)
#define bat_check_close()      act_writel((act_readl(CHG_CTL) & (~(1 << CHG_CTL_ENBATDT))), CHG_CTL)

#define bat_check_end()        (act_readl(CHG_DET)&(1 << CHG_DET_DTOVER))
#define bat_check_status()     (act_readl(CHG_DET)&(1 << CHG_DET_BATEXT))

#define bat_charge_open()      act_writel(act_readl(CHG_CTL) | (1 << CHG_CTL_CHGEN), CHG_CTL)
#define bat_charge_close()     act_writel(act_readl(CHG_CTL) & (~(1 << CHG_CTL_CHGEN)), CHG_CTL)

#define bat_charge_trickle()   (act_readl(CHG_DET) & (1 << CHG_DET_CHG_STA0))//������С��7.5%
#define bat_charge_full()      (act_readl(CHG_DET) & (1 << CHG_DET_CHGEND))//����

#define get_bat_charge_open()  (act_readl(CHG_CTL) & (1 << CHG_CTL_CHGEN))

#endif

