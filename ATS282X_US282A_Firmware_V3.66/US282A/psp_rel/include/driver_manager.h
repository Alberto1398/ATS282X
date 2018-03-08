/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������豸������Ϣ����
 * ���ߣ�liminxian
 ********************************************************************************/

#ifndef _DRIVER_H
#define _DRIVER_H

#include <typeext.h>
#include <attr.h>

#ifndef _ASSEMBLER_

/*5110�Ŀ�������������һ����������������λ�ÿ��ܻ᲻һ��*/
/*��Ϊ���ݿ���������ÿ���ļ�ϵͳҲ�п��ܳ����ڲ�ͬ�Ŀռ�*/
/*�������ִ洢�����������ļ�ϵͳ������������drv�ļ�*/
#define MODE_NORMAL 0
#define MODE_SPECIAL 1

typedef struct {
	unsigned char file_type;
	unsigned char drv_type;
	unsigned char Major_version;
	unsigned char minor_version;
	unsigned char magic[4];
	unsigned int text_offset;
	unsigned int text_length;
	unsigned int text_addr;
    unsigned int text1_offset;
    unsigned int text1_length;
    unsigned int text1_addr;
    unsigned int text2_offset;
    unsigned int text2_length;
    unsigned int text2_addr;
	unsigned int data_offset;
	unsigned int data_length;
	unsigned int data_addr;
	unsigned int bss_length;
	unsigned int bss_addr;
	unsigned int drv_init_entry;
	unsigned int drv_exit_entry;
	unsigned int drv_banka_file_offset;
	unsigned int drv_bankb_file_offset;
	unsigned int drv_bankc_file_offset;
	unsigned int drv_op_entry;
} drv_head_t;

typedef struct {
	uint32 file_startaddr;
    uint16 bank_a_offset;
	uint16 bank_b_offset;
	uint16 bank_c_offset;
    uint16 resever;
	uint32 exit_entry;
} drv_bank_info_t;

typedef struct {
    uint32 file_startaddr;
	uint16 bank_a_offset;
	uint16 bank_b_offset;
} sys_bank_info_t;

#define MAX_DRV_GROUP_TOTAL 15
#define SUPPORT_DRV_GROUP_TOTAL 14  //��ʱֻ��14�����,���ٿ���ʡdrv_bank_info_t��Ӧ�����ݿռ�
#define API_TOTAL_ENTRY (MAX_DRV_GROUP_TOTAL+1+1)//sys+libc   vfs������һ���ӿ�
typedef enum {
	DRV_GROUP_STG_BASE = 0,
	DRV_GROUP_STG_CARD,
	DRV_GROUP_STG_UHOST,
	DRV_GROUP_FAT,
	DRV_GROUP_EXFAT,
	DRV_GROUP_UD,
	DRV_GROUP_LCD,
	DRV_GROUP_FM,
	DRV_GROUP_KEY,//TOUCH&GSENSOR
	DRV_GROUP_CCD,//DRV_GROUP_I2C,
	DRV_GROUP_AUDIO_DEVICE,
	DRV_GROUP_TTS,
	DRV_GROUP_BT,
	DRV_RESERVER2,
	DRV_GROUP_SYS = 15,
} drv_type_t;

/*���¶���init �� exit �Ľӿڣ�����������*/
/*����ͨ��CMD������*/
/*void*  (*vfs_init)(vfs_mount_t* p_vfs_mount, void* null, void* null, void* null);*/
/*void* (*vfs_exit)(vfs_mount_t* p_vfs_mount, void* null1, void* null2, void* null3);*/

typedef int (*drv_entry_i)(void *param1, void *param2, void *param3);



#endif

#endif /*_DRIVER_H */

