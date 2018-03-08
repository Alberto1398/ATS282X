#include "music_scan_file.h"

music_scan_param_t g_scan_param;

//����Ƶ�ļ�ɨ��,�����߳�
void * music_scan_open(void* param)
{
    music_scan_param_t *p_scan_param = &g_scan_param;
    pthread_ext_param_t scan_thread_param;
    
    libc_memset(&g_scan_param, 0, sizeof(music_scan_param_t));

    /* ������ֻʹ�û����� */
    libc_pthread_mutex_init(&p_scan_param->mutex);

    //��ʼ�������̵߳Ĳ���
    scan_thread_param.pthread_param.start_rtn = music_scan_file;
    scan_thread_param.pthread_param.arg = param;
    //scan_thread_param.pthread_param.ptos = (void *) AP_FRONT_HIGH_STK_POS;
    g_scan_thread_task_addr = sys_malloc_large_data(AP_FRONT_HIGH_STK_SIZE);
    scan_thread_param.pthread_param.ptos = (void *)(g_scan_thread_task_addr + AP_FRONT_HIGH_STK_SIZE);
    scan_thread_param.stk_size = AP_FRONT_HIGH_STK_SIZE;

    libc_print("scan thread addr:", g_scan_thread_task_addr, 2);
    //��ֹ����
    sys_os_sched_lock();

    //����ɨ������̲߳���������
    if (libc_pthread_create(&scan_thread_param, AP_FRONT_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        libc_free_process_struct(p_scan_param->process_struct_index);
        sys_os_sched_unlock();
        return NULL;
    }

    PRINT_INFO_STR("scan begin");

    sys_os_sched_unlock();

    return p_scan_param;
}

//�ر���Ƶ�ļ�ɨ��(ɱ���߳�)
int32 music_scan_close(music_scan_param_t *p_music_scan)
{
    //ȷ�ϴ����ɹ�
    if (p_music_scan != NULL)
    {
        p_music_scan->thread_end_flag = TRUE;

        while (p_music_scan->thread_end_flag == TRUE)
        {
            /* ���ӻ������Ա�֤�õ�����Ϣͬ����scan�߳� */
            libc_pthread_mutex_unlock(p_music_scan->mutex);

            sys_os_time_dly(1);

            /* ���ӻ������Ա�֤�õ�����Ϣͬ����scan�߳� */
            libc_pthread_mutex_lock(p_music_scan->mutex);
        }
        libc_pthread_mutex_unlock(p_music_scan->mutex);
        libc_pthread_mutex_destroy(&p_music_scan->mutex);
    }

    sys_free_large_data(g_scan_thread_task_addr);
    return 0;
}
