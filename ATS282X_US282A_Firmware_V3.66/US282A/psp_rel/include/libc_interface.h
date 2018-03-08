/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������libc �ӿڶ���
 * ���ߣ�liminxian
 ********************************************************************************/

#ifndef __LIBC_DEFINE_H__
#define __LIBC_DEFINE_H__

#include <attr.h>
#include <typeext.h>

typedef enum
{
	MEMCPY = 0,
	MEMSET,
	MEMCMP,
	STRLEN,
	STRNCAT,
	STRNCMP,
	STRNCPY,
	STRLENUNI,
	STRNCPYUNI,
	STRNCATUNI,
	ITOA,
	PRINT,

	PTHREAD_EXIT,
	PTHREAD_CREATE,
	EXIT,
	WAITPID,
	GET_PROCESS_STRUCT,
	FREE_PROCESS_STRUCT,
	SEM_INIT,
	SEM_DESTROY,
	SEM_WAIT,
	SEM_TRYWAIT,
	SEM_POST,
	PTHREAD_MUTEX_INIT,
	PTHREAD_MUTEX_DESTROY,
	PTHREAD_MUTEX_LOCK,
	PTHREAD_MUTEX_UNLOCK,
	PTHREAD_MUTEX_TRYLOCK,
	PTHREAD_COND_INIT,
	PTHREAD_COND_DESTROY,
	PTHREAD_COND_WAIT,
	PTHREAD_COND_SIGNAL,
	DMA_PRINT,
	
	C_RESERVE_FUN1,
	C_RESERVE_FUN2,

} libc_cmd_e;

/*
 KERNEL���ϲ��ṩͳһ�Ľӿڣ���ֻ�ṩһ��API������ID�ķ�ʽ����
 */
// dma print define 
typedef struct
{
    union
    {
        uint32 value;
        uint8  *buf;    
    }data;    
    uint32 len; 
    uint8  dma_channel;       
}dma_print_info_t; 

#define DMA_PRINT_VAL              0x01
//PRINT BUF
#define DMA_PRINT_BUF1             0x02
#define DMA_PRINT_BUF2             0x03



extern void* libc_op_entry(void *param1, void *param2, void *param3, libc_cmd_e cmd_type) __FAR__;

typedef void* (*libc_op_i)(void*, void*, void*);

struct libc_operations {
	libc_op_i memcpy;
	libc_op_i memset;
	libc_op_i memcmp;
	libc_op_i strlen;
	libc_op_i strncat;
	libc_op_i strncmp;
	libc_op_i strncpy;
	libc_op_i strlenuni;
	libc_op_i strncpyuni;
	libc_op_i strncatuni;
	libc_op_i itoa;
	libc_op_i print;

	libc_op_i pthread_exit;
	libc_op_i pthread_create;
	libc_op_i exit;
	libc_op_i waitpid;
	libc_op_i get_process_struct;
	libc_op_i free_process_struct;
	libc_op_i sem_init;
	libc_op_i sem_destroy;
	libc_op_i sem_wait;
	libc_op_i sem_trywait;
	libc_op_i sem_post;
	libc_op_i pthread_mutex_init;
	libc_op_i pthread_mutex_destroy;
	libc_op_i pthread_mutex_lock;
	libc_op_i pthread_mutex_unlock;
	libc_op_i pthread_mutex_trylock;
	libc_op_i pthread_cond_init;
	libc_op_i pthread_cond_destroy;
	libc_op_i pthread_cond_wait;
	libc_op_i pthread_cond_signal;
    libc_op_i dma_print;
	
	libc_op_i c_reserve1;
	libc_op_i c_reserve2;

};

/*
void* libc_memcpy(void* dest, void* src, uint32 count)
�ýӿ������ڴ濽��
dest:Ŀ���ַ
src:Դ��ַ
count:�����ֽڸ���
����ֵ��0
*/
#define libc_memcpy(dest,src,count)          \
(void*)libc_op_entry((void*)(dest), (void*)(src), (void*)(uint32)(count), MEMCPY)

/*
void* libc_memset(void * dest, uint8 value, uint32 count)
�ýӿ�����д��ָ��ֵ���ڴ�
dest:Ŀ���ַ
value:��ֵַ
count:д���ֽڸ���
����ֵ��0
*/
#define libc_memset(dest,value,count)          \
(void*)libc_op_entry((void*)(dest), (void*)(uint32)(uint8)(value), (void*)(uint32)(count), MEMSET)

/*
int libc_memcmp(void* src1, void* src2, uint32 count)
�ýӿ����ڱȽ�ָ�����������Ƿ���ͬ
src1:����1��ַ
src2:����2��ַ
count:�Ƚ��ֽڸ���
����ֵint:0-��ͬ����0-����ͬ
*/
#define libc_memcmp(src1,src2,count)          \
(int)libc_op_entry((void*)(src1), (void*)(src2), (void*)(uint32)(count), MEMCMP)

/*
uint32 libc_strlen(char *str)
�ýӿ�����ͳ���ַ����ĳ���,���Ȳ�����������
str:�ַ���
����ֵuint32:�ַ�������
*/
#define libc_strlen(str)              \
(uint32)libc_op_entry((void*)(str), (void*)0, (void*)0, STRLEN)

/*
char* libc_strncat(char *str1, char *str2, uint32 count)
�ýӿ����ںϲ������ַ�������str2ָ�����ȵ��ַ����ϲ���str1�У�ĩβ�Զ����������
str1:�ַ���1
str2:�ַ���2
count:�ϲ�str2�еĸ���
����ֵ(char*):�ϲ�����ַ�����ַ����str1
*/
#define libc_strncat(str1,str2,count)         \
(char *)libc_op_entry((void*)(str1), (void*)(str2), (void*)(uint32)(count), STRNCAT)

/*
int libc_strncmp(char *str1, char *str2, uint32 count)
�ýӿ����ڱȽ������ַ�����ָ�����ȵĴ�С
str1:�ַ���1
str2:�ַ���2
count:�Ƚ��ַ�������
����ֵint:          > 0:  str1 > str2
                    = 0:  str1 = str2
                    < 0:  str1 < str2
*/
#define libc_strncmp(str1,str2,count)         \
(int)libc_op_entry((void*)(str1), (void*)(str2), (void*)(uint32)(count), STRNCMP)

/*
char* libc_strncpy(char *dest_str, char *src_str, uint32 count)
�ýӿ����ڴ�src_str�п���ָ�������ַ���dest_str�ַ����У�ĩβ�Զ����������
dest_str:Ŀ���ַ�����ַ
src_str:Դ�ַ�����ַ
count:����str2�е��ַ�����
����ֵ(char*):��������ַ�����ַ����str1
*/
#define libc_strncpy(dest_str,src_str,count)         \
(char *)libc_op_entry((void*)(dest_str), (void*)(src_str), (void*)(uint32)(count), STRNCPY)

/*
uint32 libc_strlenuni(uint8 *unicode)
�ýӿ����ڼ���unicode�ַ��ĳ��ȣ�������������
unicode:���unicode����
����ֵuint32:���ȣ��ֽڵ�λ
*/
#define libc_strlenuni(unicode)           \
(uint32)libc_op_entry((void*)(unicode), (void*)0, (void*)0, STRLENUNI)

/*
uint8* libc_strncpyuni(uint8 *dest_uni, uint8 *src_uni,uint32 count_uni)
�ýӿ�����unicode�Ŀ�����ĩβ�Զ����������
dest_uni:Ŀ��unicode
src_uni:Դunicode
count_uni:����unicode����(2�ֽ�Ϊ��λ)
����ֵ(uint8*):�������unicode��ַ����dest_uni
*/
#define libc_strncpyuni(dest_uni,src_uni,count_uni)      \
(uint8*)libc_op_entry((void*)(dest_uni), (void*)(src_uni), (void*)(uint32)(count_uni), STRNCPYUNI)

/*
uint8* libc_strncatuni(uint8 *dest_uni, uint8 *src_uni,uint32 count_uni)
�ýӿ�����unicode�ĺϲ�,�ϲ�ָ��������unicode��ĩβ�Զ����������
dest_uni:Ŀ��unicode
src_uni:Դunicode
count_uni:����unicode����(2�ֽ�Ϊ��λ)
����ֵ(uint8*):�������unicode��ַ����dest_uni
*/
#define libc_strncatuni(dest_uni,src_uni,count_uni)		\
(uint8*)libc_op_entry((void*)(dest_uni), (void*)(src_uni), (void*)(uint32)(count_uni), STRNCATUNI)

/*
uint8 libc_itoa(uint32 data, uint8 *str, uint8 str_count)
�ýӿ����ڰ�����ת����ָ��λ���ĵ��ַ������ַ���ĩβ�Զ����������
data:��ת��������
str:���ת������ַ���������9byte����
str_count:ָ��λ�����ӵ͵��� 1~8
����ֵuint8:ʵ��ת�����ַ��ĸ���
*/
#define libc_itoa(data,str,str_count)            \
(uint8)(uint32)libc_op_entry((void*)(uint32)(data), (void*)(str), (void*)(uint32)(uint8)(str_count), ITOA)

/*
void libc_print(char *str, uint32 data,uint8 mode)
�ýӿ����ڷ��ж�ģʽ�µ��Դ�ӡ
str:�ַ���
data:����
mode:��ӡģʽ: 0-ֻ��ӡ�ַ�����1-ֻ��ӡ���ݣ�2-��ӡ�ַ���+����
                3-��ӡ���棬strΪ�����ַ��dataΪ��ӡ�ֽڳ���
����ֵvoid
*/
#define libc_print(str,data,mode)           \
(void)sys_op_entry((void*)(str), (void*)(uint32)(data), (void*)(uint32)(uint8)(mode), SYSTEM_CPU_PRINT)//PRINT

/*
void libc_pthread_exit(void)
�ýӿ�������ֹ�߳�����
����ֵvoid
*/
#define libc_pthread_exit()         \
(void)libc_op_entry((void*)0, (void*)0, (void*)0, PTHREAD_EXIT)

/*
int libc_pthread_create(pthread_ext_param_t *pthread_param, uint8 prio, uint8 process_descr_index)
�ýӿ����ڴ����߳�����
pthread_param:�߳���Ϣ
prio:���ȼ�
process_descr_index���߳�id
����ֵint:0-�ɹ�����0ʧ��
*/

#define libc_pthread_create(pthread_param,prio,process_descr_index)  \
(int)sys_op_entry((void*)(pthread_param), (void*)(uint32)(prio), (void*)(uint32)(process_descr_index), PTHREAD_CREATE_FIX)

/*
void libc_exit(int8 no)
�ýӿ����ڽ����˳�����
no:�˳���ʶ
����ֵvoid
*/
#define libc_exit(no)                \
(void)libc_op_entry((void*)(int32)(no), (void*)0, (void*)0, EXIT)

/*
int libc_waitpid(int8 *stat_loc, int options)
�ýӿ����ڵȴ����̽����Ƿ�
stat_loc:��������˳�����ֵ
options:�ȴ�������0--���ȣ���0--�Ȳ����˳�
����ֵint:-1-û�еȴ��������̣�0~3��Ӧ��������ID
*/
#define libc_waitpid(stat_loc,options)           \
(int)libc_op_entry((void*)(stat_loc), (void*)(int)(options), (void*)0, WAITPID)

/*
int8 libc_get_process_struct(void)
�ýӿ�����������н���ID
����ֵint8:-1-û�еȴ��������̣�0~3��Ӧ��������ID
*/
#define libc_get_process_struct()   \
(int8)(int32)libc_op_entry((void*)0, (void*)0, (void*)0, GET_PROCESS_STRUCT)

/*
void libc_free_process_struct(int8 id)
�ýӿ������ͷŽ���
id:���̱�ʶ
����ֵvoid
*/
#define libc_free_process_struct(id) \
(void)libc_op_entry((void*)(int32)(id), (void*)0, (void*)0, FREE_PROCESS_STRUCT)

/*
int libc_sem_init(os_event_t ** sem, uint32 value)
�ýӿ����ڴ����ź���
*sem:����ź���
value:��ʼֵ
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_sem_init(sem,value)          \
(int)libc_op_entry((void*)(sem), (void*)(uint32)(value), (void*)0, SEM_INIT)

/*
int libc_sem_destroy(os_event_t ** sem)
�ýӿ����������ź���
*sem:����ź���
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_sem_destroy(sem)         \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, SEM_DESTROY)

/*
int libc_sem_wait(os_event_t *sem, uint16 timeout)
�ýӿ����ڵȴ��ź���
sem:�ź�������
timeout:��ʱʱ�䣬0-Ϊ���ȣ�>0 Ϊʱ�ӽ��ĸ���
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_sem_wait(sem,timeout)          \
(int)libc_op_entry((void*)(sem), (void*)(uint32)(timeout), (void*)0, SEM_WAIT)

/*
int libc_sem_trywait(os_event_t *sem)
�ýӿ������޵ȴ�������һ���ź���
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_sem_trywait(sem)         \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, SEM_TRYWAIT)

/*
int libc_sem_post(os_event_t *sem)
�ýӿ����ڷ���һ���ź���
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_sem_post(sem)            \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, SEM_POST)

/*
int libc_pthread_mutex_init(os_event_t ** sem)
�ýӿ����ڴ������ʻ����ź���
*sem:����ź���
value:��ʼֵ
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_mutex_init(sem)      \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_MUTEX_INIT)

/*
int libc_pthread_mutex_destroy(os_event_t ** sem)
�ýӿ��������ٷ��ʻ����ź���
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_mutex_destroy(sem)   \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_MUTEX_DESTROY)

/*
int libc_pthread_mutex_lock(os_event_t ** sem)
�ýӿ����ڷ��ʻ����ź�������
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_mutex_lock(sem)      \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_MUTEX_LOCK)

/*
int libc_pthread_mutex_unlock(os_event_t ** sem)
�ýӿ����ڷ��ʻ����ź�������
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_mutex_unlock(sem)    \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_MUTEX_UNLOCK)

/*
int libc_pthread_mutex_trylock(os_event_t *sem)
�ýӿ������޵ȴ�������һ�������ź���
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_mutex_trylock(sem)   \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_MUTEX_TRYLOCK)

/*
int libc_pthread_cond_init(os_event_t ** sem��uint32 value)
�ýӿ����ڵȴ������ź�������
sem:�ź�������
value����ʼֵ
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_cond_init(sem,value)       \
(int)libc_op_entry((void*)(sem), (void*)(value), (void*)0, PTHREAD_COND_INIT)

/*
int libc_pthread_cond_destroy(os_event_t ** sem)
�ýӿ��������ٷ��ʻ����ź���
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_cond_destroy(sem)    \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_COND_DESTROY)

/*
int libc_sem_wait(os_event_t *sem, os_event_t *mutex, uint16 timeout)
�ýӿ����ڵȴ��ź���
sem:�ź�������
mutex�������ź���
timeout:��ʱʱ�䣬0-Ϊ���ȣ�>0 Ϊʱ�ӽ��ĸ���
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_cond_wait(sem,mutex,timeout)   \
(int)libc_op_entry((void*)(sem), (void*)(mutex), (void*)(uint32)(timeout), PTHREAD_COND_WAIT)

/*
int libc_pthread_cond_signal(os_event_t *sem)
�ýӿ����ڷ���һ���ȴ��ź���
sem:�ź�������
����ֵint:0-�ɹ�����0ʧ��
*/
#define libc_pthread_cond_signal(sem)     \
(int)libc_op_entry((void*)(sem), (void*)0, (void*)0, PTHREAD_COND_SIGNAL)

/*
void libc_dma_print(uint8 *str_buf, dma_print_info_t *data_info, uint8 mode)
�ýӿ����ڻ�ȡϵͳ��Ϣ
str_buf:��ӡ�ַ�����
data_info:��ӡ����
mode����ӡģʽ,DMA_PRINT_VAL/DMA_PRINT_BUF1/DMA_PRINT_BUF2
����ֵvoid
*/
#define libc_dma_print(str_buf,data_info,mode)           \
(void)sys_op_entry((void*)(str_buf), (void*)(data_info), (void*)(uint32)(mode), SYSTEM_DMA_PRINT)//DMA_PRINT



#include <debug_interface.h>
#endif

