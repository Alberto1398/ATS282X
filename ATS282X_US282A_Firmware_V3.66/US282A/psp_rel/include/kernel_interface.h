/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������kernel �ӿڶ���
 * ���ߣ�liminxian
 ********************************************************************************/


#ifndef __KERNEL_DEFINE_H__
#define __KERNEL_DEFINE_H__
#include <attr.h>
#include <typeext.h>
#include "sys_board_def.h"
#include <mem_manager.h>

typedef enum
{
    SYS_BASE_NOR,
    SYS_BASE_CARD,    
}base_type_stg;


typedef enum
{
    SYS_INFO_DISK_CONFIG,
    SYS_INFO_SDREAD_CALLBACK,
    SYS_INFO_PAGEMISS_CALLBACK,    
    SYS_HIGH_PW_MODE_SWITCH,
    SYS_PRINT_ONOFF,
    SYS_FREQ_LOCK_SWITCH,
    SYS_CPU_WORK_MONITOR,
    SYS_HOSC_PARAMETER,
    SYS_RESET_TIMER,
    SYS_PRINT_EXCEPTION_INFO,
    SYS_AVOID_LINEIN_NOISE,
    SYS_MEM_USE_INFO,
    SYS_SET_MEM_USE_INFO,
    SYS_CLEAR_TEMP_POOL,
    SYS_DISABLE_USE_TEMP_POOL,
    SYS_ENABLE_USE_TEMP_POOL,
    SYS_DISABLE_USE_SEND_BUFFER,
    SYS_READ_PAGEMISS_CALLBACK,
}sys_info_type_e;

typedef enum
{
    EXECE_AP = 0,
    LOAD_CODEC,
    LOAD_MMM,
    LOAD_DSP_CODEC,
    FREE_AP,
    FREE_CODEC,
    FREE_MMM,
    FREE_DSP_CODEC,
    BANK_FLUSH,
    DETECT_DISK,
    DRV_INSTALL,
    DRV_UNINSTALL,
    GET_DRV_INSTALL_INFO,
    
    SD_FOPEN,
    SD_FCLOSE,
    SD_FSEEK,
    SD_FTELL,
    SD_FREAD,
    
    VM_READ,
    VM_WRITE,
    
    REQUEST_IRQ,
    FREE_IRQ,
    
    MEM_MALLOC,
    MEM_FREE,
    
    MQ_SEND,
    MQ_RECEIVE,
    MQ_FLUSH,
    
    GET_FW_INFO,
    GET_AB_TIMER,
    
    MDELAY,
    UDELAY,
    ACTIONS_SLEEP,
    USLEEP,
    
    SET_IRQ_TIMER1,
    DEL_IRQ_TIMER1,
    
    
    SET_TIME,
    GET_TIME,
    SET_DATE,
    GET_DATE,
    SET_ALARM_TIME,
    GET_ALARM_TIME,
    
    MOUNT_FS,
    UNMOUNT_FS,
    GET_FAT_TYPE_AFTER_MOUNT,
    FORMAT_DISK,
    
    LOCAL_IRQ_SAVE,
    LOCAL_IRQ_RESTORE,
    
    OS_TASK_RESUME,
    OS_TASK_SUSPEND,
    OS_TIME_DLY,
    OS_TIME_DLY_RESUME,
    OS_SCHED_LOCK,
    OS_SCHED_UNLOCK,
    
    RANDOM,
    SET_SYS_INFO,
    GET_SYS_INFO,
    
    REQUEST_CLKADJUST,
    FREE_CLKADJUST,
    ADJUST_CLK,
    RESPOND_DSP_CMD,
    REQUEST_DSP_IRQ,
    SET_DRV_SETTING,
    
    BASE_SET_INFO,
    MQ_TRAVERSE,
    DSP_PRINT,
    IRQ_PRINT,
    RECORD_ABTIME_US,
    
    SHARE_QUERY_CREAT,
    SHARE_QUERY_DESTROY,
    SHARE_QUERY_READ,
    SHARE_QUERY_UPDATE,
    
    SHM_CTRL_CREAT,
    SHM_CTRL_DESTROY,
    SHM_CTRL_MOUNT,
    
    PTHREAD_CREATE_FIX,
    SYSTEM_CPU_PRINT,
    SYSTEM_DMA_PRINT,

    GET_TIME_OF_DAY,

    PRINT_EXCEPTION_INFO,
    MEM_POOL_CREATE,
    MEM_POOL_DESTROY,
    MEM_SET_LOCK_INFO,
    MEM_ENABLE_USE,
	MEM_DISABLE_USE,

    LOAD_DSP_FADE,
    FREE_DSP_FADE,
}kernel_cmd_e;

/*
KERNEL���ϲ��ṩͳһ�Ľӿڣ���ֻ�ṩһ��API������ID�ķ�ʽ����
*/
extern void* sys_op_entry(void *param1, void *param2, void *param3, kernel_cmd_e cmd_type) __FAR__;

typedef void* (*sys_op_i)(void*, void*, void*);

struct sys_operations {
    sys_op_i exece_ap;
    sys_op_i load_codec;
    sys_op_i load_mmm;
    sys_op_i load_dsp_codec;  
    sys_op_i free_ap;
    sys_op_i free_codec;
    sys_op_i free_mmm;
    sys_op_i free_dsp_codec;
    sys_op_i bank_flush;
    sys_op_i detect_disk;
    sys_op_i drv_install;
    sys_op_i drv_uninstall;
    sys_op_i get_drv_install_info;
    
    sys_op_i sd_fopen;
    sys_op_i sd_fclose;
    sys_op_i sd_fseek;
    sys_op_i sd_ftell;
    sys_op_i sd_fread;
    
    sys_op_i vm_read;
    sys_op_i vm_write;
    
    sys_op_i request_irq;
    sys_op_i free_irq;
    
    sys_op_i malloc;
    sys_op_i free;
    
    sys_op_i mq_send;
    sys_op_i mq_receive;
    sys_op_i mq_flush;
    
    sys_op_i get_fw_info;
    sys_op_i get_ab_timer;
    
    sys_op_i mdelay;
    sys_op_i udelay;
    sys_op_i sleep;
    sys_op_i usleep;
    
    sys_op_i set_irq_timer1;
    sys_op_i del_irq_timer1;
    
    
    sys_op_i set_time;
    sys_op_i get_time;
    sys_op_i set_date;
    sys_op_i get_date;
    sys_op_i set_alarm_time;
    sys_op_i get_alarm_time;
    
    sys_op_i mount_fs;
    sys_op_i unmount_fs;
    sys_op_i get_fat_type_after_mount;
    sys_op_i format_disk;
    
    sys_op_i local_irq_save;
    sys_op_i local_irq_restore;
    
    sys_op_i os_task_resume;
    sys_op_i os_task_suspend;
    sys_op_i os_time_dly;
    sys_op_i os_time_dly_resume;
    sys_op_i os_sched_lock;
    sys_op_i os_sched_unlock;
    
    sys_op_i random;
    sys_op_i set_sys_info;
    sys_op_i get_sys_info;
    
    sys_op_i request_clkadjust;
    sys_op_i free_clkadjust;
    sys_op_i adjust_clk;
    sys_op_i respond_dsp_cmd;
    sys_op_i request_dsp_irq;
    
    sys_op_i set_drv_setting;
    sys_op_i base_set_info;
    sys_op_i mq_traverse;
    sys_op_i dsp_print;
    sys_op_i irq_print;
    sys_op_i record_abtime_us;
    
    sys_op_i share_query_creat;
    sys_op_i share_query_destroy;
    sys_op_i share_query_read;
    sys_op_i share_query_update;
    
    sys_op_i shm_ctrl_creat;
    sys_op_i shm_ctrl_destroy;
    sys_op_i shm_ctrl_mount;
    
    sys_op_i pthread_create_fix;
    
    sys_op_i system_cpu_print;
    sys_op_i system_dma_print;
    sys_op_i get_time_of_day;
    sys_op_i print_exception_info;
    sys_op_i mem_pool_create;
    sys_op_i mem_pool_destroy;
    sys_op_i mem_set_lock_info;
    sys_op_i enable_mem_use;
    sys_op_i disable_mem_use;

    sys_op_i load_dsp_fade; 
    sys_op_i free_dsp_fade; 	
};

/*
int sys_exece_ap(char *name,uint32 type,void *argc)
�ýӿ����ڼ���APӦ��
name��Ӧ�õ��ļ���XXX.AP
type��Ӧ������
argc������Ӧ�õĲ������������Ӷ���
����ֵint��0-�ɹ�����0ʧ��
note:bank���룬��ֹ���жϵ���
*/
#define sys_exece_ap(name,type,argc)     \
(int)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)(argc), EXECE_AP) 

/*
int sys_load_codec(char *name, uint32 type)
�ýӿ����ڼ���Ԥ�����
name��Ԥ�������ļ���XXX.al
type��Ԥ���������
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_load_codec(name,type)     \
(int)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)0, LOAD_CODEC)

/*
int sys_load_mmm(char *name, uint32 type)
�ýӿ����ڼ����м��
name���м�����ļ���XXX.al
type���м������
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_load_mmm(name,type)      \
(int)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)0, LOAD_MMM)

/*
int sys_load_dsp_codec(char *name, uint32 type)
�ýӿ����ڼ���DSP��
name��DSP����ļ���XXX.dsp
type��DSP������

����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_load_dsp_codec(name,type)     \
(int)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)0, LOAD_DSP_CODEC)

/*
int sys_load_dsp_fade(char *name, uint32 type, uint32 address)
�ýӿ����ڼ���DSP��
name��DSP����ļ���XXX.dsp
type��DSP������
address:���dsp��text0_addr

����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_load_dsp_fade(name,type,address)     \
(int)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)(uint32)(address), LOAD_DSP_FADE)


/*
void sys_free_ap(uint32 type)
�ýӿ�����ж��Ӧ��
type��Ӧ������
����ֵvoid
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_ap(type)        \
(void)sys_op_entry((void*)(uint32)(type), (void*)0, (void*)0, FREE_AP)

/*
void sys_free_codec(uint32 type)
�ýӿ�����ж��Ԥ�����
type��Ԥ���������
����ֵvoid
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_codec(type)     \
(void)sys_op_entry((void*)(uint32)(type), (void*)0, (void*)0, FREE_CODEC)

/*
void sys_free_mmm(uint32 type)
�ýӿ�����ж���м��
type���м������
����ֵvoid
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_mmm(type)     \
(void)sys_op_entry((void*)(uint32)(type), (void*)0, (void*)0, FREE_MMM)

/*
void sys_free_dsp_codec(uint32 type)
�ýӿ�����ж��DSP��
type��DSP������
����ֵvoid
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_dsp_codec(name,type)    \
(void)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)0, FREE_DSP_CODEC)

/*
sys_free_dsp_fade(name,type) 
�ýӿ�����ж��DSP��
type��DSP������Ϊ1
����ֵvoid
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_dsp_fade(name,type)    \
(void)sys_op_entry((void*)(name), (void*)(uint32)(type), (void*)0, FREE_DSP_FADE)


/*
int sys_bank_flush(uint8 page_index)
�ýӿ��������pageҳ��TLB
page_index��pageҳ
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_bank_flush(page_index)  \
(int)sys_op_entry((void*)(uint32)(uint8)(page_index), (void*)0, (void*)0, BANK_FLUSH)

/*
int sys_detect_disk(uint8 drv_type)
�ýӿڼ���ⲿ�洢�豸 (card,u��)�Ƿ����
drv_type���豸����
����ֵint��0-���룬��0-û�в���
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_detect_disk(drv_type)   \
(int)sys_op_entry((void*)(uint32)(uint8)(drv_type), (void*)0, (void*)0, DETECT_DISK)

/*
int drv_install(uint8 drv_type, void *drv_para, char* drv_name)
�ýӿ����ڼ��������豸����
drv_type���豸����
drv_para���������Σ������߶���,��Ӧ����init��ڵĵ�3������
drv_name�������ļ�����xxx.drv
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_drv_install(drv_type,drv_para,drv_name)   \
(int)sys_op_entry((void*)(uint32)(uint8)(drv_type), (void*)(drv_para), (void*)(drv_name), DRV_INSTALL)

/*
int sys_drv_uninstall(uint8 drv_type)
�ýӿ�����ж�������豸����
drv_type���豸����
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_drv_uninstall(drv_type)   \
(int)sys_op_entry((void*)(uint32)(uint8)(drv_type), (void*)0, (void*)0, DRV_UNINSTALL)

/*
int sys_get_drv_install_info(uint8 drv_type)
�ýӿڻ�ȡ�豸������Ϣ
drv_type���豸����
����ֵint��-1-ʧ�ܣ�����ֵ--������Ϣ
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_get_drv_install_info(drv_type) \
(int)sys_op_entry((void*)(uint32)(uint8)(drv_type), (void*)0, (void*)0, GET_DRV_INSTALL_INFO)

/*
sd_file_t *sys_sd_fopen (char *name)
�ýӿڴ�sd���ļ�
name��sd���ļ�xxx.yyy
����ֵ�ļ������NULL--Ϊʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_sd_fopen(name)             \
(void*)sys_op_entry((void*)(name), (void*)0, (void*)0, SD_FOPEN)

/*
sd_file_t *sys_sd_fopen (char *name, uint32 part_id)
�ýӿڴ�sd���ļ�
name��sd���ļ�xxx.yyy
����ֵ�ļ������NULL--Ϊʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_sd_fopen_ext(name, part_id)             \
(void*)sys_op_entry((void*)(name), (void*)(part_id), (void*)0, SD_FOPEN)


/*
int sys_sd_fclose(sd_file_t *fp)
�ýӿ����ڹر��Ѵ򿪵�sd���ļ�
fp���ļ����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_sd_fclose(fp)            \
(int)sys_op_entry((void*)(fp), (void*)0, (void*)0, SD_FCLOSE)

/*
int sys_sd_fseek(sd_file_t *fp, uint8 type, int32 offset)
�ýӿ����ڶ�λ�ļ���λ��
fp���ļ����
type��seek���ͣ�SEEK_SET-���ļ�ͷ��SEEK_CUR--��ǰλ�ã�SEEK_END--�ļ�ĩβ
offset��ƫ��λ�ã���Ϊ����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_sd_fseek(fp,type,offset)   \
(int)sys_op_entry((void*)(fp), (void*)(uint32)(uint8)(type), (void*)(offset), SD_FSEEK)

/*
uint32 sys_sd_ftell(sd_file_t *fp)
�ýӿ����ڻ�ȡ�ļ���ǰ��λ��
fp���ļ����
����ֵuint32:�ļ���ָ��
*/
#define sys_sd_ftell(fp)           \
(uint32)sys_op_entry((void*)(fp), (void*)0, (void*)0, SD_FTELL) 

/*
int32 sys_sd_fread(sd_file_t *fp, void *buf, uint32 len)
�ýӿ����ڶ�ȡ�ļ�����
fp���ļ����
buffer����ŵ�buffer
len����ȡ�ĳ���(�ֽڵ�λ)
����ֵint32����ʵ��ȡ�ĳ��ȣ�-1--ʧ��
*/
#define sys_sd_fread(fp,buf,len)         \
(int32)sys_op_entry((void*)(fp), (void*)(buf), (void*)(uint32)(len), SD_FREAD)


/*
int sys_vm_read(void *buf, uint32 offset, uint32 len)
�ýӿ����ڶ�ȡVRAM������
buffer����ŵ�buffer
offset����ȡ��λ��ƫ��
len����ȡ�ĳ���(�ֽڵ�λ)
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_vm_read(buf,offset,len)    \
(int)sys_op_entry((void*)(buf), (void*)(uint32)(offset), (void*)(uint32)(len), VM_READ)


/*
int sys_vm_write(void *buf, uint32 offset, uint32 len)
�ýӿ�����д��VRAM������
buffer��д�����ݵ�buffer
offset��д���λ��ƫ��
len��д��ĳ���(�ֽڵ�λ)
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_vm_write(buf,offset,len)         \
(int)sys_op_entry((void*)(buf), (void*)(uint32)(offset), (void*)(uint32)(len), VM_WRITE)


/*
int sys_request_irq(uint32 type, void *handle)
�ýӿ�����ע���жϷ������
type���ж�����
handle���жϷ���������
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_request_irq(type,handle)        \
(int)sys_op_entry((void*)(uint32)(type), (void*)(handle), (void*)0, REQUEST_IRQ)

/*
void sys_free_irq(uint32 type)
�ýӿ�����ע���ж�
type���ж�����
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_irq(type)             \
(void)sys_op_entry((void*)(uint32)(type), (void*)0, (void*)0, FREE_IRQ) 

/*
void* sys_malloc(uint32 size)
�ýӿ����������ڴ�
size�������ڴ��С(�ֽڵ�λ)
����ֵ �ڴ��ַ
*/
#define sys_malloc(size)        \
(void *) sys_op_entry((void *)(uint32)(size), (void *)(MEM_DATA), (void *)0, MEM_MALLOC)


#define sys_malloc_bt_data(size) \
(void *) sys_op_entry((void *)(uint32)(size), (void *)(MEM_BT_DATA), (void *)0, MEM_MALLOC) 


#define sys_malloc_bank_data(size) \
(void *) sys_op_entry((void *)(uint32)(size), (void *)(MEM_BANK_DATA), (void *)0, MEM_MALLOC)

#define sys_malloc_large_data(size) \
(void *) sys_op_entry((void *)(uint32)(size), (void *)(MEM_LDATA), (void *)0, MEM_MALLOC)

/*
void sys_free(void* addr)
�ýӿ������ͷ������ڴ�
addr���ڴ��ַ
����ֵint��0-�ɹ�����0ʧ��
*/    
#define sys_free(addr)             \
(int) sys_op_entry((void *)(addr), (void *)(MEM_DATA), (void *)0, MEM_FREE)

#define sys_free_bt_data(addr)             \
(int) sys_op_entry((void *)(addr), (void *)(MEM_BT_DATA), (void *)0, MEM_FREE)

#define sys_free_bank_data(addr)             \
(int) sys_op_entry((void *)(addr), (void *)(MEM_BANK_DATA), (void *)0, MEM_FREE)

#define sys_free_large_data(addr)             \
(int) sys_op_entry((void *)(addr), (void *)(MEM_LDATA), (void *)0, MEM_FREE)

/*
int mq_send(uint8 queue_id, void *msg)
�ýӿ����ڷ���ϵͳ��Ϣ
queue_id����Ϣ����id
msg�������Ϣ����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_mq_send(queue_id,msg)            \
(int)sys_op_entry((void*)(uint32)(uint8)(queue_id), (void*)(msg), (void*)0, MQ_SEND)

/*
int sys_mq_receive(uint8 queue_id, void *msg)
�ýӿ����ڽ���ϵͳ��Ϣ
queue_id����Ϣ����id
msg�������Ϣ����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_mq_receive(queue_id,msg)         \
(int)sys_op_entry((void*)(uint32)(uint8)(queue_id), (void*)(msg), (void*)0, MQ_RECEIVE) 

/*
int sys_mq_flush(uint8 queue_id)
�ýӿ��������ϵͳ��Ϣ��
queue_id����Ϣ����id
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_mq_flush(queue_id)             \
(int)sys_op_entry((void*)(uint32)(uint8)(queue_id), (void*)0, (void*)0, MQ_FLUSH)

/*
int sys_mq_traverse(uint8 queue_id, void* msg, uint32 msg_index)
�ýӿ����� ��ѯ/��ȡ ϵͳ��Ϣ
queue_id����Ϣ����id
msg�������Ϣ���ݣ���ΪNULL��ֻ��ѯ��Ϣ����������ȡ��Ϣ����
msg_index����ȡָ������Ϣ���
����ֵint����Ϣ����Ϣ��������Ϊ-1 - ����ʧ��
*/
#define sys_mq_traverse(queue_id,msg,msg_index)      \
(int)sys_op_entry((void*)(uint32)(uint8)(queue_id), (void*)(msg), (void*)(uint32)(msg_index), MQ_TRAVERSE)

/*
int get_fw_info (uint8 *buf, uint32 info_addr, uint32 info_len)
�ýӿ����ڻ�ȡSD��ָ��λ������
ptr_fw_info�������Ϣ����
info_addr��sdλ��
info_len�����ݳ���(�ֽ�)
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_get_fw_info(buf,info_addr,info_len)      \
(int)sys_op_entry((void*)(buf), (void*)(uint32)(info_addr), (void*)(uint32)(info_len), GET_FW_INFO)

/*
uint32 sys_get_ab_timer(void)
�ýӿ�����ϵͳ�������ʱ�䣬����ʱ���ͳ��
����ֵ ʱ�䣬��λms
*/
#define sys_get_ab_timer()      \
(uint32)sys_op_entry((void*)0, (void*)0, (void*)0, GET_AB_TIMER)

/*
void sys_mdelay(uint32 ms)
�ýӿ����ں��뼶��ʱ
ms����ʱʱ��ms
����ֵNULL
*/
#define sys_mdelay(ms)      \
(void)sys_op_entry((void*)(uint32)(ms), (void*)(0), (void*)(0), MDELAY)

/*
void sys_udelay(uint32 us)
�ýӿ�����΢�뼶��ʱ
us����ʱʱ��us
����ֵNULL
*/
#define sys_udelay(us)      \
(void)sys_op_entry((void*)(uint32)(us), (void*)0, (void*)(0), UDELAY) 

/*
void sys_sleep(uint32 s)
�ýӿ����ڹ����߳�˯�ߣ��뼶��ʱ
s��˯��ʱ��s
����ֵNULL
*/
#define sys_sleep(s)       \
(void)sys_op_entry((void*)(uint32)(s), (void*)(0), (void*)0, ACTIONS_SLEEP)

/*
void sys_usleep(uint32 us)
�ýӿ����ڹ����߳�˯�ߣ�΢����ʱ
us��˯��ʱ��us
����ֵNULL
*/
#define sys_usleep(us)    \
(void)sys_op_entry((void*)(uint32)(us), (void*)(0), (void*)(0), USLEEP)

/*
int8 sys_set_irq_timer1(void* time_handle, uint32 ms_count)
�ýӿ�����ע����뼶�Ķ�ʱ��
time_handle����ʱ����������
ms_count����ʱʱ��(ms)
����ֵint����ʱ��ID��-1Ϊע��ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_irq_timer1(time_handle,ms_count)     \
(int8)(int)sys_op_entry((void*)(time_handle), (void*)(uint32)(ms_count), (void*)0, SET_IRQ_TIMER1)

/*
int sys_del_irq_timer1(int8 timer_id)
�ýӿ�����ע����ʱ��
timer_id����ʱ��ID
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_del_irq_timer1(timer_id)       \
(int)sys_op_entry((void*)(uint32)(timer_id), (void*)(0), (void*)(0), DEL_IRQ_TIMER1)

/*
void sys_set_time(time_t *time)
�ýӿ���������ϵͳʱ��
time������ʱ��ֵ
����ֵNULL
*/
#define sys_set_time(time)      \
(void)sys_op_entry((void*)(time), (void*)(0), (void*)(0), SET_TIME)

/*
void sys_get_time(time_t *time)
�ýӿ����ڻ�ȡϵͳʱ��
time�����ʱ��ֵ
����ֵNULL
*/
#define sys_get_time(time)      \
(void)sys_op_entry((void*)(time), (void*)(0), (void*)(0), GET_TIME)

/*
int sys_set_date(date_t *date)
�ýӿ���������ϵͳ����
time����������ֵ
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_set_date(date)       \
(int)sys_op_entry((void*)(date), (void*)(0), (void*)(0), SET_DATE)

/*
int sys_get_date(date_t *date)
�ýӿ���������ϵͳ����
time���������ֵ
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_get_date(date)      \
(int)sys_op_entry((void*)(date), (void*)(0), (void*)(0), GET_DATE)

/*
void set_alarm_time(time_t *time)
�ýӿ�������������ʱ��
time������ʱ��ֵ
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_alarm_time(time)   \
(void)sys_op_entry((void*)(time), (void*)(0), (void*)(0), SET_ALARM_TIME)

/*
void sys_get_alarm_time(time_t *time)
�ýӿ����ڻ�ȡ����ʱ��
time�����ʱ��ֵ
����ֵNULL
*/
#define sys_get_alarm_time(time)     \
(void)sys_op_entry((void*)(time), (void*)(0), (void*)(0), GET_ALARM_TIME)

/*
int8 sys_mount_fs(uint8 drv_type, uint8 disk, uint8 work_mode)
�ýӿ����ڹ����豸���ļ�ϵͳ
drv_type���豸����
disk���豸�̷�
work_mode������ģʽ��Ĭ��Ϊ0
����ֵint���ļ�ϵͳID��Ϊ-1��ʾʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_mount_fs(drv_type,disk,work_mode)     \
(int8)(int)sys_op_entry((void*)(uint32)(uint8)(drv_type), (void*)(uint32)(disk), (void*)(uint32)(uint8)(work_mode), MOUNT_FS)

/*
int sys_unmount_fs(int8 id)
�ýӿ�����ж���ѹ��ص��ļ�ϵͳ
id���ļ�ϵͳID
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_unmount_fs(id)           \
(int)sys_op_entry((void*)(uint32)(id), (void*)0, (void*)0, UNMOUNT_FS)

/*
uint8 sys_get_fat_type_after_mount(uint32 id)
�ýӿ����ڻ�ȡ���غ���ļ�ϵͳ������ 
id���ļ�ϵͳID
����ֵuint8���ļ�ϵͳ���� FAT16/FAT32/EXFAT
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_get_fat_type_after_mount(id) \
(uint8)(uint32)sys_op_entry((void*)(uint32)(id), (void*)0, (void*)0, GET_FAT_TYPE_AFTER_MOUNT)

/*
int sys_format_disk(uint8 dry_type, uint8 reserver, uint8 fat_type)
�ýӿ����ڸ�ʽ��ָ���豸���Լ��ļ�ϵͳ����
dry_type���豸����
reserver�����ã�Ĭ��Ϊ0
fat_type���ļ�ϵͳ���� FAT16/FAT32/EXFAT
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_format_disk(dry_type,reserver,fat_type)      \
(int)sys_op_entry((void*)(uint32)(dry_type), (void*)(uint32)(reserver), (void*)(uint32)(fat_type), FORMAT_DISK)

/*
uint32 sys_local_irq_save(void)
�ýӿ����ڹر�ϵͳ�жϣ��������ж�����
����ֵuint32�������ж�����
*/
#define sys_local_irq_save()        \
(uint32)sys_op_entry((void*)0, (void*)0, (void*)0, LOCAL_IRQ_SAVE)

/*
void sys_local_irq_restore(uint32 irq_save)
�ýӿ����ڻָ��ж�����
irq_save���ж�����
����ֵNULL
*/
#define sys_local_irq_restore(irq_save)    \
(void) sys_op_entry((void*)(uint32)(irq_save), (void*)0, (void*)0, LOCAL_IRQ_RESTORE)

/*
uint8 sys_os_task_resume(uint8 prio)
�ýӿ�����ɾ������
prio���������ȼ�
����ֵuint8��0-�ɹ�����0ʧ��
*/
#define sys_os_task_resume(prio)       \
(uint8)(uint32)sys_op_entry((void*)(uint32)(prio), (void*)0, (void*)0, OS_TASK_RESUME)

/*
uint8 sys_os_task_suspend(uint8 prio)
�ýӿ����ڹ�������
prio���������ȼ�
����ֵuint8��0-�ɹ�����0ʧ��
*/
#define sys_os_task_suspend(prio)      \
(uint8)(uint32)sys_op_entry((void*)(uint32)(prio), (void*)0, (void*)0, OS_TASK_SUSPEND)

/*
void sys_os_time_dly(uint16 ticks)
�ýӿ����������ͷŵ���ʱ��Ƭ
ticks��ʱ�ӽ���(10msΪ��λ)
����ֵNULL
*/
#define sys_os_time_dly(ticks)   \
(void)sys_op_entry((void*)(uint32)(ticks), (void*)0, (void*)0, OS_TIME_DLY)

/*
uint8 sys_os_time_dly_resume(uint8 prio)
�ýӿ����ڹ������񣬲���ʱ
prio���������ȼ�
����ֵuint8��0-�ɹ�����0ʧ��
*/
#define sys_os_time_dly_resume(prio)   \
(uint8)(uint32)sys_op_entry((void*)(uint32)(prio), (void*)0, (void*)0, OS_TIME_DLY_RESUME)

/*
void sys_os_sched_lock(void)
�ýӿ�����ϵͳ��������
����ֵNULL
*/
#define sys_os_sched_lock()    \
(void) sys_op_entry((void*)0, (void*)0, (void*)0, OS_SCHED_LOCK)

/*
void sys_os_sched_unlock(void)
�ýӿ�����ϵͳ���Ƚ���
����ֵNULL
*/
#define sys_os_sched_unlock()   \
(void) sys_op_entry((void*)0, (void*)0, (void*)0, OS_SCHED_UNLOCK)


/*
uint32 sys_random(void)
�ýӿ����ڻ�ȡϵͳ���������
����ֵuint32������� 0~0xFFFFFFFF
*/
#define sys_random()      \
(uint32)sys_op_entry((void*)0, (void*)0, (void*)0, RANDOM)


/*
int sys_set_sys_info(void *info, uint32 sys_info_type)
�ýӿ���������ϵͳ��Ϣ
info����Ϣ����
sys_info_type����Ϣ����
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_sys_info(info,sys_info_type)	     \
(int)sys_op_entry((void*)(uint32)(info), (void*)(uint32)(sys_info_type), (void*)0, SET_SYS_INFO)			

/*
int sys_get_sys_info(void *info, uint32 sys_info_type)
�ýӿ����ڻ�ȡϵͳ��Ϣ
info�������Ϣ����
sys_info_type����Ϣ����
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_get_sys_info(info,sys_info_type)	     \
(int)sys_op_entry((void*)(uint32)(info), (void*)(uint32)(sys_info_type), (void*)0, GET_SYS_INFO)			

/*
int8 sys_request_clkadjust(void *call_back, uint32 pll_range)
�ýӿ�����ע���Ƶ�ص�����
call_back���ص��������
pll_range������PLLƵ�ʷ�Χ����16λΪ��Сֵ����16λΪ���ֵ����ֵΪ0��ʾ������PLLƵ��
����ֵint���ص�id�ţ�-1Ϊʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_request_clkadjust(call_back,pll_range)	\
(int8)(int)sys_op_entry((void*)(call_back), (void*)(uint32)(pll_range), (void*)0, REQUEST_CLKADJUST)

/*
int sys_free_clkadjust(int8 id)
�ýӿ�����ע����Ƶ�ص�����
id���ص�id��
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_free_clkadjust(id)	    \
(int)sys_op_entry((void*)(uint32)(id), (void*)0, (void*)0, FREE_CLKADJUST)			

/*
uint32 sys_adjust_clk(uint32 freq, uint32 type)
�ýӿ����ڵ���MISP �� dspƵ��
freq����8λΪMISPƵ�ʣ���8λΪDSPƵ��
type�����������,��δʹ�ã�0-ǰ̨��1-��̨���棬2-����
����ֵuint32��֮ǰ��Ƶ�ʣ���8λΪMISPƵ�ʣ���8λΪDSPƵ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_adjust_clk(freq, type)	    \
(uint32)sys_op_entry((void*)(uint32)(freq), (void*)(uint32)(type), (void*)0, ADJUST_CLK)

/*
uint32 sys_get_adjustclk(uint32 *lock_sw)
�ýӿ����ڻ�ȡMISP �� dspƵ��
lock_sw: ��ȡ��ǰƵ������״̬�� 1--lock��0--unlock
����ֵuint32����ǰ��Ƶ�ʣ���8λΪMISPƵ�ʣ���8λΪDSPƵ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_get_adjustclk(lock_sw)	        \
(uint32)sys_op_entry((void*)(uint32)(0), (void*)(uint32)(4), (void*)lock_sw, ADJUST_CLK)

/*
uint32 sys_adjust_asrc_clk(uint32 freq)
�ýӿ����ڵ���ASRCƵ��
freq��ASRCƵ��
����ֵuint32��֮ǰ��Ƶ�ʣ�ASRCƵ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_adjust_asrc_clk(freq)	    \
(uint32)sys_op_entry((void*)(uint32)(freq), (void*)(uint32)(3), (void*)0, ADJUST_CLK)


/*
int32 sys_respond_dsp_cmd(void)
�ýӿ����ڻ�ӦDSP�������󣬱�ʾ��������ɣ�DSP���Կ�ʼ����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_respond_dsp_cmd()      \
(int32)sys_op_entry((void*)0, (void*)0, (void*)0, RESPOND_DSP_CMD)

/*
int32 sys_request_dsp_irq(uint8 in_use, uint8 *cmd_context)
�ýӿ���������DSP�ж�
in_use�������ж�������Ϣ
cmd_context���������ݣ�ΪNULL--��Ϊ����Ҫ������Ϣ
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_request_dsp_irq(in_use,cmd_context)   \
(int32)sys_op_entry((void*)(uint32)(in_use), (void*)(cmd_context), (void*)0, REQUEST_DSP_IRQ)

/*
int32 sys_set_mpu_irq(sys_mpu_param_t *param)
�ýӿ�����ע��MPU�ж�
param��ע���������
����ֵint���ж�ע��ID��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_mpu_irq(param)        \
(int32)sys_op_entry((void*)(uint32)(IRQ_MPU), (void*)(param), (void*)(0), REQUEST_IRQ)

/*
void sys_del_mpu_irq(id)
�ýӿ�����ע��MPU�ж�
id���ж�ע��ID��
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_del_mpu_irq(id)         \
(void)sys_op_entry((void*)(IRQ_MPU), (void*)(uint32)(id), (void*)0, FREE_IRQ) 

/*
int sys_set_drv_setting(void *set_info)
�ýӿ����������豸�������ã���card����������
set_info��������Ϣ���� card_pm_cfg_t
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_drv_setting(set_info)   \
(int)sys_op_entry((void*)(uint32)(set_info), (void*)0, (void*)0, SET_DRV_SETTING)

/*
int sys_set_drv_ops(uint8 drv_type, uint32 ops_addr)
�ýӿ������ض�λ������ops��ַ
drv_type: ��������
ops_addr��������ַ��
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_drv_ops(drv_type, ops_addr)   \
(int)sys_op_entry((void*)(uint32)(drv_type), (void*)(uint32)(ops_addr), (void*)1, SET_DRV_SETTING)

/*
int sys_get_delay_val(uint32 delay_ms)
�ýӿ����ڻ�ȡ��ʱ����ֵ
delay_ms: ��ʱʱ��
����ֵint����ʱ����ֵ
*/
#define sys_get_delay_val(delay_ms, div_val)   \
(int)sys_op_entry((void*)(uint32)(delay_ms), (void*)(uint32)(div_val), (void*)2, SET_DRV_SETTING)

/*
int sys_base_set_info(void *info, uint32 type)
�ýӿ�����sd��������Ϣ
info��������Ϣ
type����������
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_base_set_info(info,type)     \
(int)sys_op_entry((void*)(info), (void*)(uint32)(type), (void*)0, BASE_SET_INFO)

//��־����S3BT��������ʱ��Ҫ��VRAM�����⴦��������β��index
#define sys_base_set_enter_s3bt_scene()  (base_enter_s3bt_scene())

//��־�˳�S3BT��������ʱ����VRAM�����⴦������β��index
#define sys_base_set_exit_S3bt_scene()   (base_exit_s3bt_scene())
/*
void sys_dsp_print(void)
�ýӿ����ڴ�ӡdsp��������
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_dsp_print()            \
(void)sys_op_entry((void*)(0), (void*)(0), (void*)(0), DSP_PRINT)

/*
void sys_irq_print(char *str, uint32 data,uint8 mode)
�ýӿ�����ϵͳ�жϴ�ӡ
str:�ַ���
data:����
mode:��ӡģʽ��0-ֻ��ӡ�ַ�����1-ֻ��ӡ���ݣ�2-��ӡ�ַ���+����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_irq_print(str,data,mode)       \
(void)sys_op_entry((void*)(str), (void*)(uint32)(data), (void*)(uint32)(mode), IRQ_PRINT)


/*
uint32 sys_read_c0count(void)
�ýӿ����ڻ�ȡϵͳ��cpu������ֵ
����ֵuint32����Χ 0~0xFFFFFFFF
*/
#define sys_read_c0count()         \
(uint32)sys_op_entry((void*)0, (void*)0, (void*)0, RANDOM)

#if 0
/*
void sys_us_timer_start(void)
�ýӿ����ڳ�ʼ΢���ʱ��
*/
#define sys_us_timer_start()    \
    (void)sys_op_entry((void*)(0), (void*)(0), (void*)(0), RECORD_ABTIME_US)

/*
uint32 sys_us_timer_break(void)
�ýӿ����ڻ�ȡ΢���ʱ��ֵ
����ֵuint32����Χ 0~0xFFFFFFFF
!!!NOTE����sys_us_timer_start���ʹ�ã��м䲻�����е�Ƶ�����������ʱ��׼
*/
#define sys_us_timer_break()    \
    (uint32)sys_op_entry((void*)(1), (void*)(0), (void*)(0), RECORD_ABTIME_US)
#endif
/*
void* sys_share_query_creat(int8 query_id, uint8 *mem_addr, uint16 size)
�ýӿ����ڴ��������ڴ��ѯ������У������ؿɱ�д����ڴ��ַ
query_id������ID
mem_addr���ڴ��ַ
size���ڴ��С
����ֵvoid*����д����ڴ��ַ��NULL-Ϊ����ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_share_query_creat(query_id,mem_addr,size)       \
(void*)sys_op_entry((void*)(int32)(query_id), (void*)(mem_addr), (void*)(uint32)(size), SHARE_QUERY_CREAT)

/*
int sys_share_query_destroy(int8 query_id)
�ýӿ�����ɾ���Ѵ����Ķ���
query_id������ID
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_share_query_destroy(query_id)         \
(int)sys_op_entry((void*)(int32)(query_id), (void*)0, (void*)0, SHARE_QUERY_DESTROY)

/*
int sys_share_query_read(int8 query_id, uint8 *read_addr)
�ýӿ����ڻ�ȡ�����ڴ���Ϣ����
query_id������ID
read_addr��������Ϣ����
����ֵint��0-�ɹ�����0ʧ��
*/
#define sys_share_query_read(query_id,read_addr)   \
(int)sys_op_entry((void*)(int32)(query_id), (void*)(read_addr), (void*)0, SHARE_QUERY_READ)

/*
void* sys_share_query_update(int8 query_id)
�ýӿ����ڸ�����д�빲���ڴ���Ϣ���ݣ������ؿɱ�д����ڴ��ַ
query_id������ID
����ֵvoid*���ɱ�д����ڴ��ַ��NULL-Ϊ����ʧ��
*/
#define sys_share_query_update(query_id)          \
(void*)sys_op_entry((void*)(int32)(query_id), (void*)0, (void*)0, SHARE_QUERY_UPDATE)

/*
int sys_share_query_creat(int8 shm_id, uint8 *shm_addr, uint16 shm_size)
�ýӿ����ڴ��������ڴ�ռ�
shm_id�������ڴ��ʶID
shm_addr���ڴ��ַ
shm_size���ڴ��С
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_shm_creat(shm_id,shm_addr,shm_size)       \
(int)sys_op_entry((void*)(int32)(shm_id), (void*)(shm_addr), (void*)(uint32)(shm_size), SHM_CTRL_CREAT)

/*
int sys_shm_destroy(int8 shm_id)
�ýӿ�����ɾ�������ڴ�ռ�
shm_id�������ڴ��ʶID
����ֵint��0-�ɹ�����0ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_shm_destroy(shm_id)         \
(int)sys_op_entry((void*)(int32)(shm_id), (void*)0, (void*)0, SHM_CTRL_DESTROY)

/*
uint8* sys_shm_mount(int8 shm_id)
�ýӿ������������ڴ��ַ
shm_id�������ڴ��ʶID
����ֵuint8*�������ڴ��ַ��NULL-Ϊ����ʧ��
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_shm_mount(shm_id)           \
(uint8*)sys_op_entry((void*)(int32)(shm_id), (void*)0, (void*)0, SHM_CTRL_MOUNT)

/*
void sys_enter_high_powered(int up_type)
�ýӿ���������ϵͳ�������� ��������ģʽ
up_type --��ʾ���ͣ�1--VDD, 2--VCC, 3--(VCC + VDD)
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_enter_high_powered(up_type)	 \
(void)sys_op_entry((void*)(uint32)1, (void*)(uint32)SYS_HIGH_PW_MODE_SWITCH, (void*)(uint32)(up_type), SET_SYS_INFO)			

/*
void sys_exit_high_powered(void)
�ýӿ������˳�ϵͳ�������� ��������ģʽ
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_exit_high_powered()	     \
(void)sys_op_entry((void*)(uint32)0, (void*)(uint32)SYS_HIGH_PW_MODE_SWITCH, (void*)0, SET_SYS_INFO)			
	

/*
void sys_lock_adjust_freq(uint32 lock_freq)
�ýӿ���������ϵͳ��ǰ��MISP&DSPƵ�ʣ������� sys_adjust_clk Ƶ�ʵĵ���
lock_freq����8λΪMISPƵ�ʣ���8λΪDSPƵ�ʣ�
           ��lock_freq = 0,������Ϊ��ǰ�ܵ�Ƶ��
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_lock_adjust_freq(lock_freq)	       \
(void)sys_op_entry((void*)(uint32)1, (void*)(uint32)SYS_FREQ_LOCK_SWITCH, (void*)(uint32)(lock_freq), SET_SYS_INFO)			

/*
void sys_unlock_adjust_freq(void)
�ýӿ����ڽ���ϵͳ��MISP&DSPƵ�ʵ���,����sys_adjust_clk Ƶ�ʵĵ���
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_unlock_adjust_freq()	     \
(void)sys_op_entry((void*)(uint32)0, (void*)(uint32)SYS_FREQ_LOCK_SWITCH, (void*)0, SET_SYS_INFO)			


/*
void sys_cpu_monitor_start(void)
�ýӿ���������cpuʹ��ռ�ձȼ��
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_cpu_monitor_start()	       \
(void)sys_op_entry((void*)(uint32)1, (void*)(uint32)SYS_CPU_WORK_MONITOR, (void*)0, SET_SYS_INFO)			

/*
void sys_cpu_monitor_end(void)
�ýӿ����ڽ���cpuʹ��ռ�ձȼ�أ��鿴���
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_cpu_monitor_end()	     \
(void)sys_op_entry((void*)(uint32)0, (void*)(uint32)SYS_CPU_WORK_MONITOR, (void*)0, SET_SYS_INFO)			


/*
void sys_set_hosc_param(uint16 param)
�ýӿ��������ø�Ƶ���ݲ���ֵ
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_hosc_param(param)	     \
(void)sys_op_entry((void*)(uint32)(uint16)param, (void*)(uint32)SYS_HOSC_PARAMETER, (void*)0, SET_SYS_INFO)			

/*
int sys_read_hosc_param(void)
�ýӿ����ڻ�ȡ��Ƶ���ݲ���ֵ
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_read_hosc_param()	     \
(int)sys_op_entry((void*)0, (void*)(uint32)SYS_HOSC_PARAMETER, (void*)1, SET_SYS_INFO)			

/*
void sys_reset_timer(void)
�ýӿ���������ϵͳ�Ķ�ʱ������
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_reset_timer()	     \
(void)sys_op_entry((void*)0, (void*)(uint32)SYS_RESET_TIMER, (void*)0, SET_SYS_INFO)

/*
void sys_avoid_linein_noise(channel_num)
�ýӿ����ڿ�������linein����
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_avoid_linein_noise(a)	     \
(void)sys_op_entry((void*)(a), (void*)(uint32)SYS_AVOID_LINEIN_NOISE, (void*)0, SET_SYS_INFO)


/*
void sys_print_exception_info(exc_code)
�ýӿ�������������ϵͳ�쳣����ӡ�쳣��Ϣ
����ֵNULL
!!!exc_code ������룬�ο�exception.hͷ�ļ�����
*/
#define sys_print_exception_info(a)	     \
(void)sys_op_entry((void*)(a), (void*)0, (void*)0, PRINT_EXCEPTION_INFO)			

/*
void sys_get_time_of_day(struct timeval* tm)
�ýӿ����ڶ�ȡϵͳ�Կ���֮��ľ�ȷʱ�䣬����΢��
tm��ʱ��
!!!��פ���룬���жϵ���
*/							
#define sys_get_time_of_day(a)	     \
(void)sys_op_entry((void*)(a), (void*)0, (void*)0, GET_TIME_OF_DAY)			

/*
void sys_read_mem_use_info(void)
�ýӿ����ڶ�ȡ�ڴ�ʹ����Ϣ�ṹ���ַ
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_read_mem_use_info()	     \
(int)sys_op_entry((void*)0, (void*)(uint32)SYS_MEM_USE_INFO, (void*)0, SET_SYS_INFO)	

/*
void sys_set_cache_info(void)
�ýӿ���������ĳ��ap�����¸��ӳ���cacheʹ�����
����ֵNULL
!!!NOTE:bank���룬��ֹ���жϵ���
*/
#define sys_set_mem_use_info(a)	     \
(int)sys_op_entry((void*)(a), (void*)(uint32)SYS_SET_MEM_USE_INFO, (void*)0, SET_SYS_INFO)	

#define sys_clear_temp_pool()	     \
(int)sys_op_entry((void*)0, (void*)(uint32)SYS_CLEAR_TEMP_POOL, (void*)0, SET_SYS_INFO)	

#define sys_disable_use_temp_pool()	     \
    (int)sys_op_entry((void*)0, (void*)(uint32)SYS_DISABLE_USE_TEMP_POOL, (void*)0, SET_SYS_INFO) 

#define sys_enable_use_temp_pool()	     \
(int)sys_op_entry((void*)0, (void*)(uint32)SYS_ENABLE_USE_TEMP_POOL, (void*)0, SET_SYS_INFO)

#define sys_disable_use_send_buffer()	     \
(int)sys_op_entry((void*)0, (void*)(uint32)SYS_DISABLE_USE_SEND_BUFFER, (void*)0, SET_SYS_INFO)	

#define sys_read_pagemiss_cbk()	     \
(int)sys_op_entry((void*)0, (void*)(uint32)SYS_READ_PAGEMISS_CALLBACK, (void*)0, SET_SYS_INFO)	

#define sys_mem_pool_create(a)  \
(void)sys_op_entry((void*)(a), (void*)0, (void*)0, MEM_POOL_CREATE)

#define sys_mem_pool_clear(a, b)  \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)(MEM_POOL_OP_CLEAR), MEM_POOL_DESTROY)

#define sys_mem_pool_destroy(a, b)  \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)(MEM_POOL_OP_DESTROY), MEM_POOL_DESTROY)

#define sys_mem_pool_force_destroy(a, b)  \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)(MEM_POOL_OP_FORCE_DESTROY), MEM_POOL_DESTROY)

#define sys_set_mem_lock(a, b)  \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)SET_MEM_LOCK, MEM_SET_LOCK_INFO)

#define sys_clr_mem_lock(a, b)  \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)CLR_MEM_LOCK, MEM_SET_LOCK_INFO)

#define sys_enable_mem_use(a,b) \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)(0), MEM_ENABLE_USE)

#define sys_disable_mem_use(a,b) \
(int)sys_op_entry((void*)(a), (void*)(b), (void*)(0), MEM_DISABLE_USE)

#endif




