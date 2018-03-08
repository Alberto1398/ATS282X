/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������̹�����
 * ���ߣ�liminxian
 ********************************************************************************/

#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#include <typeext.h>
#include <ucos/os.h>

#define CREATE_NOT_MAIN_THREAD 0xff
#define WAITPID_ONLY_PROCESS_MANAGER -1
#define WAITPID_NO_PROCESS_EXIT 0xf

/*!
 *     �߳��ڲ��ṹ:���ڱ����̵߳���ڼ�������
 */
struct pthread_start_args {
	/* function to run */
	void * (*start_routine)(void *);
	/* its argument */
	void * arg;
};

/*!
 *     �߳��ڲ��ṹ:�߳�������ָ��Ľṹ
 */
typedef struct _pthread_descr_struct {
	/* ����������ṹָ�� */
	struct _pthread_descr_struct *next;
	/* �̴߳����Ĳ��� */
	struct pthread_start_args p_start_args;
	/* �����Ľ��̽ṹ���� */
	uint8 process_descr_index;
	/* ��pthread_exit()��ֹ,��Ϊtrue */
	int8 p_terminated;
	/* ���ȼ� */
	uint8 prio;
} pthread_descr_t;

/*!
 *  \brief
 *     pthread_once��������
 */
typedef int pthread_once_t;

/* �������ݽṹ */
typedef struct {
	pthread_descr_t *descr_head; /* �߳�����������ͷ */
	pthread_once_t exit_once; /* ȷ��exitֻ������һ�� */
	int8 visible_thread_num; /*��ǰ��Ծ�߳������ǽ����߳�*/
	int8 is_used;
	int8 exit_val; /* �����˳�����ֵ */
} process_struct_t;

#define PROCESS_STRUCT_NUM 5
#define WNOHANG 0xff

typedef struct {
	void *(*start_rtn)(void*);
	void *arg;
	OS_STK *ptos;
} pthread_param_t;

typedef struct
{
    pthread_param_t pthread_param;
    uint16   stk_size;
} pthread_ext_param_t;

extern void pthread_exit(void* null0, void* null1, void* null2);
extern int pthread_create(pthread_param_t *pthread_pram, INT8U prio,
		uint8 process_descr_index);
extern int pthread_create_fix(pthread_ext_param_t *pthread_pram, INT8U prio,
		uint8 process_descr_index);		
extern void exit(int8 exitval, void* null1, void* null2);
extern int waitpid(int8 *stat_loc, int options, void* null2);
extern int8 get_process_struct(void* null0, void* null1, void* null2);
extern void free_process_struct(int8 index, void* null1, void* null2);

extern int sem_init(os_event_t ** sem, unsigned int value, void* null2);
extern int sem_destroy(os_event_t ** sem, void* null1, void* null2);
extern int sem_wait(os_event_t *sem, unsigned short timeout, void* null2);
extern int sem_trywait(os_event_t *sem, void* null1, void* null2);
extern int sem_post(os_event_t *sem, void* null1, void* null2);

extern int pthread_mutex_init(os_event_t ** mutex, void* null1, void* null2);
extern int pthread_mutex_destroy(os_event_t ** mutex, void* null1, void* null2);
extern int pthread_mutex_lock(os_event_t *mutex, void* null1, void* null2);
extern int pthread_mutex_unlock(os_event_t *mutex, void* null1, void* null2);
extern int pthread_mutex_trylock(os_event_t *mutex, void* null1, void* null2);

extern int pthread_cond_init(os_event_t ** cond, uint32 init_value, void* null2);
extern int pthread_cond_destroy(os_event_t ** cond, void* null1, void* null2);
extern int pthread_cond_wait(os_event_t *cond, os_event_t *mutex,
		unsigned short timeout);
extern int pthread_cond_signal(os_event_t *cond, void* null1, void* null2);

extern struct _pthread_descr_struct *__thread_special(uint8 prio);
extern int _thread_kill(uint8 prio);
extern struct _pthread_descr_struct *__thread_self(void);
extern int
		pthread_once(pthread_once_t *once_control, void(*init_routine)(void));
extern int thread_remove_from_process(pthread_descr_t *desrc,
		uint8 process_descr_index);
extern void pthread_init(void);

#endif

