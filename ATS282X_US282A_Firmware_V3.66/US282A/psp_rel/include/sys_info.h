   /********************************************************************************
	*							   USDK(GL5110)
	*							 Module: KERNEL
	*				  Copyright(c) 2003-2011 Actions Semiconductor,
	*							 All Rights Reserved.
	*
	* History:
	*	   <author> 	 <time> 		  <version >			 <desc>
	*	   wuyueqian	 2011-9-10 09:00	 1.0			 build this file
	********************************************************************************/
   /*!
	* \file 	sys_info.h
	* \brief	sys_info.bin
	* \author	wuyueqian
	* \version	1.0
	* \date  2011/9/10
	*******************************************************************************/
   
#ifndef __SYS_INFO_H__
#define __SYS_INFO_H__

#include <typeext.h>
#include <ucos/init.h>


#define DIR_COUNT      126

#define BREC_OFFSET (128)
#define BREC_SIZE   (128)

#define PARAM_OFFSET (BREC_OFFSET + BREC_SIZE)
#define PRRAM_SIZE   (128)

#define LFI_OFFSET  (PARAM_OFFSET + PRRAM_SIZE)
#define LFI_SIZE    (1024 * 2)

//OTA:LFI��С��2��:
//4Mnor  :LFI������СΪ0XE00��(flash_sector - s3bt_size - reserved_size - brec_sector - mbrec_size - param_size - vram_size)/2
//8M:LFI������СΪ0x1000��(����4M��LFI����С����2M)

#define NOR_8M_LFI_MAX_SIZE         (0x1000)//2M,��λΪ����
#define NOR_8M_LFI_MAX_HALF_SIZE    (0x800) 

#define NOR_4M_LFI_MAX_SIZE         (0xe80) 
#define NOR_4M_LFI_MAX_HALF_SIZE    (0x740) //����С��һ��

#define NOR_2M_LFI_MAX_SIZE         (0x680)
#define NOR_2M_LFI_MAX_HALF_SIZE    (0x340)

#define LFI_MAX_BYTE_SIZE    (2 * 1024 * 1024) //lFI���������СΪ2M

typedef struct
{
    uint8   filename[11];   /* �ļ���8.3 */
    uint8   type;           /* ��A��,��B��,��H��,��F��, ��S��, ��U��, ��I�� */
    uint32  DownloadAddr;   /* ���صĵ�ַ */
    uint32  offset;         /* ���ļ��е�ƫ�ƣ�byteΪ��λ */
    uint32  length;         /* ����byteΪ��λ��512ȡ����*/
    uint32  Subtype;        /* ������ */
    uint32  checksum;       /* DWORD�ۼ� */
}AFI_DIR_t;


typedef struct
{
    uint8   afi_flag[4];
    uint16  reserved0;
    uint16  reserved1;
    uint32  version;
    uint32  date;
    uint32  dLength;    //afi���ܳ��ȣ��ֽ�Ϊ��λ
    uint32  dVersion;   //afi�İ汾����CFG�ļ�ָ��
    uint8   reserved2[8];
    AFI_DIR_t afi_dirs[DIR_COUNT];
    uint8   reserved3[28];
    uint32  checksum;      /* afi headУ��ͣ�DWORD�ۼ� */
}AFI_Head_t;    /* 4kB */



typedef struct 
{
    uint32  SD_Capacity;
    uint32  VM_Capacity;
    uint32  MI_Capacity;
    uint32  UI_Capacity;
    uint32  UserDisk_Start_Addr;
    uint32  vm_disk_cap;
    uint32  udisk_cap;
    uint32  hide_disk_cap;
    uint32  auto_disk_cap;
    uint32  Reserved[3];
}CapInfo_t; 

typedef struct
{
    uint32  LFIFlag;
    uint8   sdk_version[4];
    uint8   version[4];
    uint16  VID;
    uint16  PID;
    uint32  DIRItemCheckSum;
    
    //20�ֽ�ƫ��
    CapInfo_t CapInfo;  //48byte

    //68�ֽ� lfi���ݳ���
    uint32   lfi_length; 

    //72�ֽ�
    uint8   save_bt_data;

    //73�ֽ�ƫ��
    uint8   Reserve0[6];
    
    //79�ֽ�ƫ��
    uint8   udisk_setting;

    //80�ֽ�ƫ��, usb sn
    uint8   UsbSetupInfo[48];
    
    //128�ֽ�ƫ��
    uint8   sdk_description[336];
    
    //464�ֽ�ƫ��
    uint8   module_num[8];
    
    uint8   ota_mode;
        
    uint8   Reserve3[37];
    
    uint16  Headchecksum;
}LFIHead_t;


typedef struct
{
    uint8  bLength;              //�˽ṹ�ĳ��ȣ�����Ϊ��λ��ĿǰΪ0x1
    uint8  bType;               //���ݽṹ���ͣ�ĿǰΪ0x7
    uint8  Reserved1[14];
    uint32 dFwuChecksum;        // fwu��У��ͣ�������������
    uint32 dFlag;               //�ṹ����ֵ��־��ĿǰΪ0x55aa55aa
    uint8  pbDescriptor[8];    //�ṹ���ַ�����־��ĿǰΪ��FwuTail��
    uint8  pbFwuCrcChecksum[32];         //fwu��crcУ��ͣ�������������
    uint8  Reserved2[444];
    uint32 dFwuTailChecksum;            //�˽ṹ��У��ͣ�ĿǰΪǰ508bytes��У���
}FwuTail_t;

//�̶�������������buffer���������μ�INF_PARSE_CONFIG_INDEX_FOR_APP�Ķ��� 
typedef enum
{
    CONFIG_ID_80_CHARGE_MODE = 0,
    CONFIG_ID_110_CYRSTAL_CAP = 1,
    CONFIG_ID_121_DEBUG_MODE = 2,
    CONFIG_ID_124_UART_PRINT_ENABLE = 3,
}config_id_for_app_e;


#define FW_INFO_VERSION_ADDR           4
#define FW_INFO_VERSION_LEN            8

#define FW_INFO_SDK_VERSION_ADDR       4
#define FW_INFO_SDK_VERSION_LEN        4

#define FW_INFO_CASE_VERSION_ADDR      8
#define FW_INFO_CASE_VERSION_LEN       4

#define FW_INFO_MODULE_NUM_ADDR       464
#define FW_INFO_MODULE_NUM_LEN        8

#define FW_INFO_OTA_MODE_ADDR         472
#define FW_INFO_OTA_MODE_LEN          1

#define FW_INFO_USB_VIDPID_ADDR        12
#define FW_INFO_USB_VIDPID_LEN         8
#define FW_INFO_CAPINFO_ADDR           40
#define FW_INFO_CAPINFO_LEN            16
//#define FW_INFO_USB_VENDOR_ADDR        256
//#define FW_INFO_USB_VENDOR_LEN         8
#define FW_INFO_USB_ID_ADDR            80
#define FW_INFO_USB_ID_LEN             48
#define FW_INFO_USB_SN_TYPE_ADDR       79
#define FW_INFO_USB_SN_TYPE_LEN        1
#define FW_INFO_BT_SAVE_DATA           72
#define FW_INFO_BT_SAVE_DATA_LEN       1

extern int get_fw_info (uint8 *ptr_fw_info, unsigned int info_addr, uint32 info_len)__FAR__;

//#define sys_get_fw_sdk_version((a)) sys_get_fw_info((a), FW_INFO_SDK_VERSION_ADDR, FW_INFO_SDK_VERSION_LEN);

//#define sys_get_fw_case_version((a)) sys_get_fw_info((a), FW_INFO_CASE_VERSION_ADDR, FW_INFO_CASE_VERSION_LEN);


#endif


