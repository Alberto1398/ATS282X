/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������COMMON OTA������صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�lizijian
 ********************************************************************************/
#ifndef _COMMON_OTA_H
#define _COMMON_OTA_H

#include "psp_includes.h"
#include "enhanced.h"
#include "common_func.h"
#include "vm_def.h"
#include "sys_info.h"



#define RECEIVE_DARA_FAIL  (0)
#define RECEIVE_DARA_SUC   (1)
#define RECEIVE_DARA_COMLETE  (2)

#define BYTE_PER_SECTION   (512)

#define RCP_BUFF_ADDR      (g_rcp_long_buffer)


//��ѯ����ظ��Ľṹ��
typedef struct
{
    Fw_Ver_t cru_updata;    //��ǰ���صİ汾��
    uint16 cru_pack_count;// ��ǰ�յ����ǵڼ�����
    uint8  part_id;       //����ID
    uint8  rec_data_state;   //�����Ƿ��������� 0�쳣��1������ 2�Ѿ��յ�������ɵ�����(������������)
                               //    3 ����У�鲻ͨ��������������
    uint8 tws_connect_sta;//������ʾ�û���ǰ��TWS����״̬��
    uint8 reserved;
} enquiry_message_t;

typedef struct
{
    uint8 connect_state;
    uint8 reserved[15];
}connect_reply_t;


typedef struct
{
    uint8  connect_state;
    uint8  reservd;
    uint16 cru_pack_count;// ��ǰ�յ����ǵڼ�����
    uint32 random_upg_key; //������¼�����ֵ 
    uint8  part_id;       //����ID
    uint8  rec_data_state;   //�����Ƿ��������� 0�쳣��1������2�Ѿ��յ�������ɵ�����(������������)
    uint8  reservd2[6];
}ota_bkinfo_t;


typedef struct
{
    Fw_Ver_t cru_updata;    //��ǰ���صİ汾�� 8bytes
    uint32 random_upg_val; //����
    uint16 complete_send_num; //�Ѿ����˶��ٸ�����
    uint8  part_id;       //��Ҫ�����ķ���ID
    uint8  reserved[1];      //�����ֽ�
    ota_write_info_t wirte_info;
} connect_message_t;//�ܹ�516byte

typedef struct
{
    uint32 part_id;            //����ID
    uint32 part_size;        //������С���ֽ�Ϊ��λ
    uint32 part_offset;        //����ƫ�ƣ��ֽ�Ϊ��λ
    uint16 check_sum;        //����У��ͣ�16λ���ݵ��ۼӺ�
    uint16 part_max_size;   //��������С
}part_item_t;

typedef struct
{
    uint8 magic[4];         //'OTA'
    uint8 sdk_version[4];    //SDK�汾��
    uint8 case_version[4];  //case�汾��
    uint8 date[4];            //�̼���������
    uint8 part_total;        //�����ܴ�С�����16������
    uint8 reserved0[7];
    uint8 module_num[8];
    part_item_t part_item[16]; //���16��ϵͳ����
    uint8 reserved[224];
}part_head_t;
   
typedef struct
{   
   connect_reply_t g_connect_repy; 
   ota_bkinfo_t g_reply_bkinfo;
   enquiry_message_t g_cur_rec_state;//С�������ݵ�״̬
   uint16 g_backup_part_addr; //������¼��ǰ������NOR����ʼλ��
   uint16 g_cru_pack_count;//��¼��ǰ����Ҳ��дnor�ĵ�ַƫ��
   uint8 g_send_erro_count;//���������󷢴���
}OTA_malloc_param_t;


extern restore_breakpoint_t g_ota_restore_data;//OTA��¼��VRAM������
extern OTA_malloc_param_t *g_OTA_var;





extern app_result_e ota_receive_connect_data(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e ota_receive_data(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e ota_receive_vram_data(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e ota_restart_to_clean_data(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
extern app_result_e ota_restart_to_upgrade(uint32 data1, uint32 data2,  void *data, uint16 data_len) __FAR__;
extern app_result_e ota_send_complete(uint32 data1, uint32 data2,  void *data, uint16 data_len) __FAR__;
extern app_result_e ota_get_ver_num(uint32 data1, uint32 data2) __FAR__;
extern app_result_e ota_enquiry_message(uint32 data1, uint32 data2) __FAR__;
extern app_result_e ota_enquiry_connect(uint32 data1, uint32 data2) __FAR__;
extern uint32 fw_download_new(void);
extern uint32 get_ver_message(Fw_Ver_Module_t *ver_buf);
extern uint32 message_copy(void *data,uint8 message_type);
extern void reboot_system(void);
extern UINT16 CheckSum16(UINT16 *buf, UINT32 len);

extern UINT32 CheckSum32(UINT32 *buf, UINT32 len);




#endif

