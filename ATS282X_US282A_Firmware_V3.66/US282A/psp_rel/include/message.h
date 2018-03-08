/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ϣ���Ͷ���
 * ���ߣ�liminxian
 ********************************************************************************/


#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define MSG_SYS_RTC2HZ               0x0080
#define MSG_SYS_BAT_V_CHG            0x0081
#define MSG_SYS_LOW_POWER            0x0082
#define MSG_SYS_RTCALARM             0x0083
#define MSG_SYS_POWER_OFF            0x0084  /*! �ػ���Ϣ, �ȴ�ǰ̨Ӧ�úͺ�̨Ӧ���˳������� config Ӧ�ý���ػ����� */
#define MSG_SYS_RECLAIM              0x0085
#define MSG_SYS_DSP_RESET            0x0086
#define MSG_SYS_MPU_ERROR            0x0087

#define MSG_SYS_USB_STICK            0x0090
#define MSG_SYS_USB_UNSTICK          0x0091

#define MSG_SYS_ADAPTOR_IN           0x00a0  /*! ����� ADAPTOR ������Ϣ */
#define MSG_SYS_ADAPTOR_OUT          0x00a1  /*! ����� ADAPTOR �γ���Ϣ */

#define MSG_SYS_SD_IN                0x00b0  /*�忨״̬*/
#define MSG_SYS_SD_OUT               0x00b1  /*����״̬*/

#define MSG_SYS_UH_IN				 0x00c0	 /*U�̲���*/
#define MSG_SYS_UH_OUT				 0x00c1	 /*U�̰γ�*/

#define MSG_SYS_LINEIN_IN            0x00d0  /*LineIn����*/
#define MSG_SYS_LINEIN_OUT           0x00d1  /*LineIn�γ�*/

#define MSG_SYS_HP_IN                0x00e0  /*Headphone����*/
#define MSG_SYS_HP_OUT               0x00e1  /*Headphone�γ�*/

#endif

