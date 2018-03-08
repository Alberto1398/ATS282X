/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     vfs_interface.h
 * \brief    �����ļ�ϵͳ�ӿ�
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __SYS_OTA_INTERFACE_H__
#define __SYS_OTA_INTERFACE_H__

#define FORCE_READ_PART_MODE    (0x80)

#define OTA_FW_ZONE_CONTINUE        (0)

#define OTA_FW_ZONE_NOT_CONTINUE    (1)  

typedef enum
{
    //����״̬������Ĭ��ֵ
    OTA_CONNECT_INIT = 0,
    //�������ݴ��󣬲���ͨ������
    OTA_CONNECT_DATA_ERROR,
    //���ӳɹ�
    OTA_CONNECT_SUCCESS,
    //�̼�̫��������
    OTA_FW_DATA_TOO_BIG,
    //2�������̼���һ��
    OTA_FW_DIFFERENT,
    //�ϵ�����
    OTA_BK_CONTINUE,
}ota_connect_msg_e;


typedef enum
{
    //�÷���Ϊ���з�����Ҳ�����������ݷ���
    OTA_PART_AGE_NORMAL = 0,
    //�÷���Ϊ���з��������������ݿ����ڱ��ݵ�ʱ�򱻱�ķ����ƻ�
    OTA_PART_AGE_NORMAL_ERROR,
    //�÷���Ϊĳ���̼������ı���������
    OTA_PART_AGE_BACKUP,
    //�÷���Ϊ�����ӵ�������
    OTA_PART_AGE_ADD
}ota_part_age_e;

typedef enum
{
    OTA_DOWNLOAD_STATE_NULL = 0,
    OTA_DOWNLOAD_STATE_START,
    OTA_DOWNLOAD_STATE_END,
    OTA_DOWNLOAD_STATE_CLEAR
}ota_download_state_e;

typedef enum
{
    OTA_UPDATE_STATE_NULL = 0,
    OTA_UPDATE_STATE_UNCLEAR_BKINFO = 1,
    OTA_UPDATE_STATE_END
}ota_update_state_e;


typedef struct
{
	uint8 part_id;  
	uint8 part_age;   //�����¾ɱ�־����ֵԽ�󣬷�������Խ��
	uint16 part_offset; //����λ�ã�����Ϊ��λ
	uint16 part_size; //������С������Ϊ��λ
	uint16 part_max_size; //������С������Ϊ��λ
	uint16 part_checksum; //����У���	
}fw_part_item_t;

typedef struct
{
    uint8 part_total; //�ܷ�������
    uint8 part_type_total; //�������͸���
    fw_part_item_t part_item[0];
}fw_part_table_t;

typedef struct
{
    uint8 write_mode; //0:����д 1:����д
    uint8 part_id;    //����ID
    uint16 part_size; //��ʵ�ʴ�С
    uint16 part_max_size; //������С
    uint16 part_offset;
    uint16 part_checksum;
}ota_write_item_t;

typedef struct
{
    uint8 part_total;
    //uint8 reserved;
    uint8 fw_mode;    //0 ����֮����Ԥ�� 1 ����֮����Ԥ��
    ota_write_item_t part_info[0]; //�䳤����
}ota_write_info_t;


typedef struct
{
    uint8 part_total;
    uint8 part_id[0]; //�䳤����
}ota_part_info_t;

typedef struct
{
	unsigned short PID; //�豸pid
	unsigned short VID; //�豸vid
	char Version[4]; //�汾��xx.x, no use now
}Fw_Ver_t;

typedef struct
{
    Fw_Ver_t fw_ver;
    char module[8]; 
}Fw_Ver_Module_t;


//дVM�ṹ��;
typedef struct
{
	uint16 magic;
	uint8  dowmload_statue; //���ص�״̬    0  :  ������1:������   2:�������
	uint8  updata_statue; //���µ�״̬       0  : δ����	1:����ɸ��ƣ���û��� 
	uint16 pack_count;	//ÿN��block��¼һ�����صİ���
	uint16 complete_send_num; //�Ѿ����˶��ٸ�����
    uint8  part_id ;//��ǰ���صķ���
    uint8  ud_att;//�������������� 0:����ϵͳ����1:��ϵͳ����  ���ֻ�����������������󲻲�VRAM
    uint8  reboot_flg;//��������ȷ��0:brec����ҪOTA ������1:��ҪOTA����
    uint8  reserved[1];
	Fw_Ver_t cru_updata;	//��ǰ���صİ汾��Ϣ

} restore_breakpoint_t;


typedef struct
{
    uint32 part_id;                 //����ID
    uint8 *data_buffer;            //����buffer
    uint32 data_len;                //���ݳ���
    uint16 part_base_offset;        //����ƫ��λ�� 
    uint16 part_write_offset;       //����дλ��ƫ��
}ota_write_param_t;




#endif
