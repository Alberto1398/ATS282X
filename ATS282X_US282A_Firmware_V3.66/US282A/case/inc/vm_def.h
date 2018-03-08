/*******************************************************************************
 *                              us211A
 *                            Module: case
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       cailizhen  2014-12-19 16:18     1.0             build this file 
 *******************************************************************************/
/*!
 * \file     vm_def.h
 * \brief    ��������Ӧ����ռVM�ռ�궨��
 * \author   zhangxs
 * \version  1.0
 * \date     2014-12-19
 *******************************************************************************/
#ifndef _VM_DEF_H
#define _VM_DEF_H

/* AP VRAM ���ݣ�0x00000000 - 0x001f0000��֧��32��VARM ID��ÿ��VRAM�ռ��Сû�й涨 */
#define  VM_BTSTACK             0x00000000//0 �����豸��Ϣ /*�õ�ַ�̶������ˣ������޸�*/
#define  VM_BTSTACK_PAIREDLIST  0x00010000//1 ����б���Ϣ
#define  VM_AP_SETTING          0x00020000//2 ϵͳ������Ϣ
#define  VM_AP_TIME_ALARM       0x00030000//3 ʱ�����Ӽ�¼
#define  VM_AP_CONFIG           0x00040000//4 ���ػ�������Ϣ����ʱû��
#define  VM_AP_MUSIC            0x00050000//5 MUSIC������Ϣ
#define  VM_AP_LINEIN           0x00060000//6 LINEIN������Ϣ
#define  VM_AP_RADIO            0x00070000//7 ������������Ϣ
#define  VM_AP_RECORD           0x00080000//8 ¼��������Ϣ
#define  VM_UHOST_BKINFO        0x00090000//9 host �ϵ㱣��
#define  VM_CARD_BKINFO         0x000a0000//a card �ϵ㱣��
#define  VM_AP_FMENGINE         0x000b0000//b FM����������Ϣ
#define  VM_AP_RECORD_WAV       0x000c0000//c ¼���ϵ�ָ������ݼ�¼��ַ
#define  VM_AP_USOUND           0x000d0000//d usound �����ϴβ��ŵĲ�������Ϣ
#define  VM_ASET_PARM_BASE      0x000e0000//e ASET���Բ�������
#define  VM_ASQT_PARM_BASE      0x000f0000//f ASQT���Բ�������

//BTMANAGER_ENABLE_HFP_PHONEBOOK ֻ�и�������򿪲Ż�ʹ�ø�VRAM��
#define  VM_BTPHONEBOOK         0x00100000//0x10 �����绰����Ϣ���ù���VRAM���������⣬�´θ��� 2015-9-18

//�����ⲿ��VRAM���������index����֤���ݲ��ᶪʧ
//!!!ע���ⲿ��index���ܺ�����ģ�鸴��,�绰��������ʱ�ر�
#define VM_S3BT_APP_GLOBAL      0x00100000   //28bytes
#define VM_S3BT_BT_MANAGER      0x00110000   //78bytes
#define VM_S3BT_APP_STATUS      0x00120000   //3bytes
//�����ⲿ��VRAM���ݣ���VRAM����merge��ʱ�����ⲿ������
#define VM_S3BT_BT_STACK        0x00400000   //0x6000

//SUPPORT_RING_FOLDER ֻ�иú�����Ϊ1�Ż�ʹ�ø�VRAM��
#define  VM_RING_FOLDER_BASE    0x001c0000//0x1c ~ 0x1f ÿ���ļ�����32�ֽڣ����Ա���64���ļ�����2KB����4��INDEX���
#define  VM_DEW_PARAM_BASE      0x00130000//13 ~ 14 ��Ч��ǿ�ͼ���������
#define  VM_DEW_PARAM_BASE_REV  0x00140000//13 ~ 14 ��Ч��ǿ�ͼ���������

//OTA����ϵ㱣��
#define  VM_OTA  				0x00150000
//OTA������Ϣ����
#define  VM_OTA_PART_INFO  	    0x00160000
//OTA������Ϣ����
#define  VM_OTA_CONNECT  	    0x00170000

#define  VM_OTA_UPGRADE  	    0x00180000

//FT mode / bqb mode��־
#define  VM_API_FLAG           0x00190000

#define VM_DIR_LIST             0x001a0000

#define  ATT_MAGIC              0xABCD


#define VM_S3BT_BT_STACK_LEN   0x6000


/* AP VRAM ��������־ = 0x55AA + ID (0x00 - 0x1f) */
#define  VRAM_MAGIC(vram_addr)  (0x55AA + (short)((vram_addr) >> 16))

//S3BT���ݺͻָ�NOR��ַ����
#define  NOR_S3BT_ERASE_WRITE   (0) //size = 0.5KB����������д��д��һ��������д
#define  NOR_S3BT_APP_GLOBAL    (1) //size = 1KB
#define  NOR_S3BT_BT_MANAGER    (3) //size = 1KB
#define  NOR_S3BT_APP_STATUS    (5) //size = 1.5KB
#define  NOR_S3BT_BT_STACK      (8) //size = 24KB

//S3BT���ݺͻָ�SRAM��ַ
#define  SRAM_S3BT_BUFFER       (0x9fc35800) //0x8000 = 28KB

#define  SRAM_S3BT_SIZE         (0x6000)

//RTC BAK0 ����
#define MY_RTC_S3BT         0  //bit0��ʾ����S3BT
#define MY_RTC_BT_FLAG      1  //bit1��ʾ�Ƿ�����������ʱ����S3BT
#define MY_RTC_ESD_FLAG     2  //bit2��ʾESD RESTART
#define MY_RTC_FUNC_INDEX   8  //bit[8-15]��ʾESDҪ��ȥ��FUNC INDEX
#define MY_RTC_BTPLAY_FLAG  16 //bit[16-18]��ʾ�����Ƿ��ڲ���״̬
#endif
