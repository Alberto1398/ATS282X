/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������жϷ�����������еĶ�ʱ��
 * ���ߣ�liminxian
 ********************************************************************************/
 

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <ucos/init.h>
#ifndef _ASSEMBLER_

#define MAX_IRQ_TIMERS 6

/*!
 * \brief
 *      �ж϶�ʱ�����ݽṹ
 */
typedef struct
{
    /* ������ */
    void (*handler)(void);
    /* ʱ���� */
    uint16 period;
    /* ��ǰ����ֵ */
    uint16 cur_value;
}irq_timer_t;




#endif /*_ASSEMBLER_*/
#endif /*__IRQ_TIMER_H__*/
