/********************************************************************************
 *                              USDK(GL5116)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      LMX         2014-7-22 10:50     1.0             build this file
 ********************************************************************************/
/*!
 * \file     sys_board_def.h
 * \brief    
 * \author   
 * \version  1.0
 * \date  2014/7/22
 *******************************************************************************/
#ifndef _SYS_BOARD_DEF_H__
#define _SYS_BOARD_DEF_H__


#define FOR_FPGA          0   //TEST  FPGA�汾
#define FOR_TESTCHIP_EVB  1   //TEST  EVB�汾
#define FOR_TESTCHIP_DEMO 2   //TEST  DEMO�汾
#define FOR_IC_EVB        3   // IC EVB�汾
#define FOR_IC_DEMO       4   // IC DEMO�汾


#define SYS_BOARD_TYPE  FOR_IC_EVB
//#define SYS_BOARD_TYPE  FOR_IC_EVB

//#if(SYS_BOARD_TYPE < FOR_IC_EVB)//��ʱ���壬����֮ǰ��FPGA_DEBUG ����Ĵ��� 
//#define FPGA_DEBUG
//#endif

//���ϵͳʱ��Ƭ������
//#define SYS_CHECK_THREAD_USEAGE

//���ϵͳջʹ��
#define SYS_CHECK_STK_SIZE

//���ϵͳջ������
//#define SYS_CHECK_STK_USAGE


#endif  /*_SYS_BOARD_DEF_H__*/

