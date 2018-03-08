#include "app_radio.h"

//��13Mʱ��91��104��13�ı�Ƶ������
const uint32 freq_passed[] =
{ 76000, 85500, 91000, 96000, 104000, 108000 };

//�Ƿ�Ӧ�ñ�������Ƶ��
bool should_be_passed(uint32 freq)
{
    //��λkHz(�Ѹ������ζ�ֵ�����������)
    //������Щ�Ƿ��������ĸ���Ƶ�ʣ���ЩʱFMоƬ���ܴ����Ƶ��
    uint8 n;

    for (n = 0; n < ARR_COUNT(freq_passed); n++)
    {
        if (freq_passed[n] == freq)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//�ж�ָ��Ƶ���Ƿ��ڵ�ǰ�ı����б���
//freq: Ƶ��
//p_freq: ��ǰ���ζ�Ӧ��Ԥ���̨������
//����ֵ: ��Ƶ����������ʱ�������±ꣻ���򷵻�0xff
uint8 whether_in_list(uint32 freq, uint32 *p_freq)
{
    uint8 ret;
    uint32 *ptr;
    uint8 num;
    uint32 tmp;

    /* ��ǰ���ε�̨�б�*/
    ptr = p_freq;

    for (num = 0; num < MAX_STATION_COUNT; num++)
    {
        tmp = *(ptr + num);
        if (tmp == freq)
        {
            //���б������ҵ�
            break;
        }
    }

    //���б���δ�ҵ�
    if (num == MAX_STATION_COUNT)
    {
        ret = 0xff;
    }
    else
    {
        ret = num + 1;
    }
    return ret;
}

int radio_send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    //msg_reply_t temp_reply;
    os_event_t *sem; //ͬ����Ϣͬ���ź���ָ��
    int retvalue = ERR_NO_ERR;
    uint8 mq_id = MQ_ID_EGN;

    PRINT_DBG_INT("sync msg:", msg->type);

    //����ʱ�ȴ��ź�����ͬʱֻ����1���̷߳���ͬ����Ϣ
    libc_sem_wait(thread_mutex, 0);

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    cur_send_pmsg.reply = reply;

    //������Ϣ��������
    if (sys_mq_send((uint32) (mq_id), (void *) &cur_send_pmsg) < 0)
    {
        //PRINT_ERR("sync msg ERR_MSGQUEUE_FULL!!");
        retvalue = ERR_MSGQUEUE_FULL;
    }
    else
    {
        //�ȴ�ͬ����Ϣ��Ӧ
        if (libc_sem_wait(sem, timeout) < 0)
        {
            //PRINT_ERR("sync msg ERR_SYNC_TIMEOUT!!");
            retvalue = ERR_SYNC_TIMEOUT;
        }
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);

    PRINT_DBG_INT("sync msg ret:", msg->type);

    return retvalue;
}

bool radio_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    //�����������ĵ�ַ
    msg.content.addr = input_param;

    //��Ż�ȡ�����ĵ�ַ
    reply.content = output_param;

    //��Ϣ����(����Ϣ����)
    msg.type = msg_type;

    //����ͬ����Ϣ
    bret = (bool) (radio_send_sync_msg(APP_ID_FMENGINE, &msg, &reply, 0) == ERR_NO_ERR);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

//��ȡ��ǰƵ��
bool radio_get_freq(void)
{
    return radio_send_msg(NULL, &g_engine_status.FM_CurrentFreq, MSG_FMENGINE_GET_FREQ);
}

//�������
void release_mute_timer(void)
{
    //�������
    radio_set_mute(releaseMUTE);
    g_mute_set_timer = -1;
}

