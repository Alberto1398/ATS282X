/*******************************************************************************
 *                              us211A
 *                            Module: case
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-10-24 16:20     1.0             build this file
 *******************************************************************************/
/*!
 * \file     vm_def.h
 * \brief    ��������Ӧ����ռVM�ռ�궨��
 * \author   zhangxs
 * \version 1.0
 * \date  2011-10-24
 *******************************************************************************/
#ifndef _VM_FWSP_DEF_H
#define _VM_FWSP_DEF_H

//vram�߼�index����,�û����������vram����
#define VRAM_MAX_USE_SECTORS        (0x45)

//ASET ����ģʽ��PLIST�����ã�VM INDEX�ſ��Ը���
#define VM_SPK_COMP_DAT_BASE    0x00200000//0x20 ~ 0x27 speaker compensation �˲���������

//��¼����Ŀ¼����Ϣ
#define VM_PLIST_INFO_START0    0x00200000//0x20
#define VM_PLIST_INFO_START1    0x00210000//0x21
#define VM_PLIST_INFO_START2    0x00220000//0x22
#define VM_PLIST_INFO_START3    0x00230000//0x23
#define VM_PLIST_INFO_START4    0x00240000//0x24
#define VM_PLIST_INFO_START5    0x00250000//0x25
#define VM_PLIST_INFO_START6    0x00260000//0x26
#define VM_PLIST_INFO_START7    0x00270000//0x27

//#define VM_PLIST_LEN           0x1000 //4k 
//��Ŀ��
#define VM_PLIST_INDEX_START0   0x00280000//0x28
#define VM_PLIST_INDEX_START1   0x00290000//0x29
#define VM_PLIST_INDEX_START2   0x002a0000//0x2a
#define VM_PLIST_INDEX_START3   0x002b0000//0x2b
#define VM_PLIST_INDEX_START4   0x002c0000//0x2c
#define VM_PLIST_INDEX_START5   0x002d0000//0x2d
#define VM_PLIST_INDEX_START6   0x002e0000//0x2e
#define VM_PLIST_INDEX_START7   0x002f0000//0x2f
//#define VM_PLIST_INDEX_LEN     0x1000 //4k 

//#define VM_FWSP_START           0x00300000 //0x30

#define FS_VM                   0x00300000 //0x30
/**!!!!!!!ע��ʹ��waves�㷨ʱ��Ϊ����װ��DSP��ʱ��Ч����ʧЧ���������ݱ��ݵ�VRAM****** 
��ʹ�ø��㷨ʱ���䲻����
ID��:0x00310000 - 0x003e0000

*************************************************************************************/

//#define FS_VM_LEN              0x2000  //0x10
//#define UDISK_VM (FS_VM+FS_VM_LEN)
#define UDISK_VM                0x003f0000//0x38
//#define UDISK_VM_LEN           0x200

//VRAM����¼���ݱ��
#define VM_LARGE_DATA_TAG      0x5AA5

//�ڴ��б�����߼��������ַӳ���
typedef struct
{
    //vram����д���byteƫ�Ƶ�ַ,64KByteѰַ��Χ
    UINT16 phy_byte_addr;

    //vram��Ч���ݳ��ȣ�byteΪ��λ,���32KByte
    UINT16 valid_byte_len:15;

    //vram����block���: 0:block 0; 1:block 1;
    UINT16 vram_block_index:1;
}vram_data_map_t;


//��Ҫ�����������߼�ӳ���
#define  PARAM_BT_ADDR                      (0)
#define  PARAM_BT_ADDR_LEN                  (6)
#define  PARAM_BT_NAME                      (1)
#define  PARAM_BT_NAME_LEN                  (56)
#define  PARAM_BT_BLENAME                   (2)
#define  PARAM_BT_BLENAME_LEN               (30)
#define  PARAM_BT_FREQ_COMPENSATION         (3)
#define  PARAM_BT_FREQ_COMPENSTATION_LEN    (2)
#define  PARAM_PRODUCT_FLAG                 (4)
#define  PARAM_PRODUCT_FLAG_LEN             (1)
#define  PARAM_PART_TABLE                   (5)
#define  PARAM_PART_TABLE_MAX_LEN           (512)

#endif
