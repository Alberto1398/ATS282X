/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������Ӽ�¼����ģ�飬���Ӽ�¼�洢��VRAM�У���Ҫͨ���ӿڽ��м�Ӵ���������ȡ��
 *       �������޸ģ�ɾ�������ӵȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_time_alarm.h"
#include "vm_def.h"

time_alarm_var_t g_time_alarm_var _BANK_DATA_ATTR_;

//�ر�����
static void sys_set_alarm_disable(void)
{
    uint32 i;

    act_writel((act_readl(RTC_CTL) & (~0x02)), RTC_CTL); //disable ALIE

    //flush_rtc
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ; //�ȴ�״̬
    }
    for (i = 0; i < 256 * 4; i++)
    {
        act_writel((act_readl(WD_CTL) | 0x01), WD_CTL);
    }
}

//0��0-N ���ز����ɹ�
//-1     ���ز�������
//-2     �����Ҳ�����Ч����
int time_alarm_record_list_operate(uint8 op, void *param1, void *param2)
{
    bool need_write_vram = FALSE;
    int ret = 0;

    sys_os_sched_lock();

    //��ȡtime & alarm ���Ӽ�¼ VM����������Ϣ
    sys_vm_read(&g_time_alarm_var, VM_AP_TIME_ALARM, sizeof(g_time_alarm_var));

    //��һ�ν���AP����ʼ��VM����
    if (g_time_alarm_var.magic != VRAM_MAGIC(VM_AP_TIME_ALARM))
    {
        g_time_alarm_var.magic = VRAM_MAGIC(VM_AP_TIME_ALARM);
        g_time_alarm_var.alarm_idx = 0xff;
        libc_memset(g_time_alarm_var.ring_folder_filenum, 0x00, sizeof(g_time_alarm_var.ring_folder_filenum));
        libc_memset(g_time_alarm_var.alarm_records, 0x00, sizeof(g_time_alarm_var.alarm_records));

        sys_vm_write(&g_time_alarm_var, VM_AP_TIME_ALARM, sizeof(time_alarm_var_t));
    }

    switch (op)
    {
        case RECORD_LIST_READ_BYINDEX:
        {
            alarm_record_t *p_alarm_record = (alarm_record_t *) param1;
            uint8 index = (uint8) (uint32) param2;

            if (index >= RCP_ALARM_MAX)
            {
                ret = -1;
                break;
            }

            libc_memcpy(p_alarm_record, &(g_time_alarm_var.alarm_records[index]), sizeof(alarm_record_t));
        }
        break;

        case RECORD_LIST_SEARCH_BYID:
        {
            alarm_record_t *p_alarm_record = (alarm_record_t *) param1;
            alarm_record_t *p_tmp_alarm_record;
            uint8 id = (uint8) (uint32) param2;
            uint8 index;

            for (index = 0; index < RCP_ALARM_MAX; index++)
            {
                p_tmp_alarm_record = &(g_time_alarm_var.alarm_records[index]);
                if (p_tmp_alarm_record->alarm.alarm_used == 1)
                {
                    if (p_tmp_alarm_record->alarm.alarm_id == id)
                    {
                        libc_memcpy(p_alarm_record, p_tmp_alarm_record, sizeof(alarm_record_t));
                        ret = index;
                        break;
                    }
                }
            }

            if (index >= RCP_ALARM_MAX)
            {
                ret = -2;
            }
        }
        break;

        case RECORD_LIST_READ_CUR:
        {
            alarm_record_t *p_alarm_record = (alarm_record_t *) param1;
            uint8 idx = (uint8) g_time_alarm_var.alarm_idx;

            if (idx >= RCP_ALARM_MAX)
            {
                ret = -2;
                break;
            }
            else
            {
                ret = idx;
            }

            libc_memcpy(p_alarm_record, &(g_time_alarm_var.alarm_records[idx]), sizeof(alarm_record_t));
        }
        break;

        case RECORD_LIST_UPDATE_ALARM:
        {
            uint8 mode = (uint8) (uint32) param1;

            time_alarm_update_alarm(mode);

            need_write_vram = TRUE;
        }
        break;

        case RECORD_LIST_GET_NEXT_ALARM:
        {
            time_t *p_tmp_next_time = (time_t *) param1;
            uint8 alarm_id;

            alarm_id = time_alarm_update_alarm(UPDATE_ALARM_GET_NEXT);
            if ((alarm_id == 0xff) || (alarm_id >= RCP_ALARM_MAX))
            {
                ret = -1;
            }
            else
            {
                libc_memcpy(p_tmp_next_time, &(g_time_alarm_var.alarm_records[alarm_id].last_time), sizeof(time_t));
                ret = alarm_id;
            }
        }
        break;

        case RECORD_LIST_SET_NEXT_ALARM:
        {
            time_alarm_update_alarm(UPDATE_ALARM_JUST_CLOSE);

            g_time_alarm_var.alarm_idx = (uint8) (uint32) param1;

            need_write_vram = TRUE;
        }
        break;

        case RECORD_LIST_ADD_BYINDEX:
        {
            alarm_record_t *p_alarm_record = (alarm_record_t *) param1;
            uint8 index = (uint8) (uint32) param2;

            if (index >= RCP_ALARM_MAX)
            {
                ret = -1;
                break;
            }

            libc_memcpy(&(g_time_alarm_var.alarm_records[index]), p_alarm_record, sizeof(alarm_record_t));

            need_write_vram = TRUE;
        }
        break;

        case RECORD_LIST_DELETE_BYINDEX:
        {
            uint8 index = (uint8) (uint32) param2;

            if (index >= RCP_ALARM_MAX)
            {
                ret = -1;
                break;
            }

            g_time_alarm_var.alarm_records[index].alarm.alarm_used = 0;

            need_write_vram = TRUE;
        }
        break;

        case RECORD_LIST_VALID_TOTAL:
        {
            uint8 alarm_cnt = 0;
            uint8 alarm_index = 0;

            for (alarm_index = 0; alarm_index < RCP_ALARM_MAX; alarm_index++)
            {
                if (g_time_alarm_var.alarm_records[alarm_index].alarm.alarm_used == 1)
                {
                    alarm_cnt++;
                }
            }

            ret = alarm_cnt;
        }
        break;

        case RECORD_LIST_SEARCH_NOUSED:
        {
            alarm_record_t *p_tmp_alarm_record;
            uint8 index;

            for (index = 0; index < RCP_ALARM_MAX; index++)
            {
                p_tmp_alarm_record = &(g_time_alarm_var.alarm_records[index]);
                if (p_tmp_alarm_record->alarm.alarm_used == 0)
                {
                    ret = index;
                    break;
                }
            }

            if (index >= RCP_ALARM_MAX)
            {
                ret = -2;
            }
        }
        break;

        case RECORD_LIST_READ_FOLDERNUM:
        {
            uint8 *p_tmp_ring_foldernum = (uint8 *) param1;

            libc_memcpy(p_tmp_ring_foldernum, g_time_alarm_var.ring_folder_filenum, RCP_RING_FOLDER_MAX);
        }
        break;

        case RECORD_LIST_WRITE_FOLDERNUM:
        {
            uint8 *p_tmp_ring_foldernum = (uint8 *) param1;

            libc_memcpy(g_time_alarm_var.ring_folder_filenum, p_tmp_ring_foldernum, RCP_RING_FOLDER_MAX);

            need_write_vram = TRUE;
        }
        break;

        default:
        break;
    }

    if (need_write_vram == TRUE)
    {
        sys_vm_write(&g_time_alarm_var, VM_AP_TIME_ALARM, sizeof(time_alarm_var_t));
    }

    sys_os_sched_unlock();

    return ret;
}

static void __time_alarm_update_alarm_close(alarm_record_t *p_alarm_record)
{
    if (p_alarm_record->alarm.alarm_day == 0)
    {
        p_alarm_record->alarm.alarm_onoff = FALSE;
    }
    libc_memcpy(&(p_alarm_record->last_time), &(p_alarm_record->alarm.alarm_time), sizeof(time_t));
    p_alarm_record->snooze = 0;
}

/* �����´�����ʱ�䣬���û�п������ӣ�����ȫֹͣ����
 #define UPDATE_ALARM_ADD        0 //��ӻ��޸����Ӽ�¼
 #define UPDATE_ALARM_DEL        1 //ɾ�����Ӽ�¼
 #define UPDATE_ALARM_SNOOZE     2 //��ʱ��ǰ����
 #define UPDATE_ALARM_CLOSE      3 //�رյ�ǰ���ӣ�ֻ�ر�ֻ��һ�ε�����
 #define UPDATE_ALARM_GET_NEXT   4 //��ȡ�������ʱ�䣬�ų�����ǰ�����ֵ�����
 #define UPDATE_ALARM_JUST_CLOSE 5 //�����رյ�ǰ����
 #define UPDATE_ALARM_CLOSE_SAME 6 //�ر�������ͬʱ�������
 */
uint8 time_alarm_update_alarm(uint8 mode)
{
    uint32 tmp_cur_time, tmp_alarm_time;
    uint32 tmp_diff_time, tmp_diff_time_min;
    uint8 alarm_index, alarm_available;
    time_t alarm_time;
    alarm_record_t *p_cur_alarm_record = &(g_time_alarm_var.alarm_records[g_time_alarm_var.alarm_idx]);

    sys_get_time(&alarm_time);

    if (g_time_alarm_var.alarm_idx != 0xff)
    {
        //̰˯��ʱ
        if (mode == UPDATE_ALARM_SNOOZE)
        {
            p_cur_alarm_record->last_time.second = alarm_time.second;
            p_cur_alarm_record->last_time.minute = (alarm_time.minute + sys_comval->snooze_timer) % 60;
            p_cur_alarm_record->last_time.hour = alarm_time.hour;
            if ((alarm_time.minute + sys_comval->snooze_timer) >= 60)
            {
                p_cur_alarm_record->last_time.hour = (p_cur_alarm_record->last_time.hour + 1) % 24;
            }
            p_cur_alarm_record->snooze++;
        }
        //�ر�ֻ��һ������
        else if ((mode == UPDATE_ALARM_CLOSE) || (mode == UPDATE_ALARM_JUST_CLOSE))
        {
            __time_alarm_update_alarm_close(p_cur_alarm_record);
        }
        else
        {
            ; //for QAC
        }
    }

    if (mode == UPDATE_ALARM_JUST_CLOSE)
    { //�رյ�ǰ���Ӻ󼴿ɷ���
        return 0;
    }

    //����������˵�����
    tmp_cur_time = (uint32) (alarm_time.hour * 3600);
    tmp_cur_time += alarm_time.minute * 60;
    tmp_cur_time += alarm_time.second;
    tmp_diff_time_min = 0xffffffff;
    alarm_available = 0xff;
    for (alarm_index = 0; alarm_index < RCP_ALARM_MAX; alarm_index++)
    {
        if ((mode == UPDATE_ALARM_GET_NEXT) && (alarm_index == g_time_alarm_var.alarm_idx))
        { //������ӣ�����������ǰ�����ֵ�����
            continue;
        }

        if ((mode == UPDATE_ALARM_CLOSE_SAME) && (alarm_index == g_time_alarm_var.alarm_idx))
        { //ֻ�ر���������
            continue;
        }

        p_cur_alarm_record = &(g_time_alarm_var.alarm_records[alarm_index]);

        if (p_cur_alarm_record->alarm.alarm_used == 1)
        { //��֧��ANSI����
            if (p_cur_alarm_record->alarm.alarm_onoff == 1)
            {
                tmp_alarm_time = (uint32) (p_cur_alarm_record->last_time.hour * 3600);
                tmp_alarm_time += p_cur_alarm_record->last_time.minute * 60;
                tmp_alarm_time += p_cur_alarm_record->last_time.second;

                //�ر�������ͬʱ�������
                if (mode == UPDATE_ALARM_CLOSE_SAME)
                {
                    if (tmp_alarm_time == tmp_cur_time)
                    {
                        __time_alarm_update_alarm_close(p_cur_alarm_record);
                        continue;
                    }
                }

                if (tmp_alarm_time > tmp_cur_time)
                {
                    tmp_diff_time = tmp_alarm_time - tmp_cur_time;
                }
                else
                {
                    tmp_diff_time = (tmp_alarm_time + 24 * 3600) - tmp_cur_time;
                }

                if (tmp_diff_time < tmp_diff_time_min)
                {
                    tmp_diff_time_min = tmp_diff_time;
                    alarm_available = alarm_index;
                }

                //�ر�������ͬʱ�������
                if (mode == UPDATE_ALARM_CLOSE_SAME)
                {
                    if (tmp_diff_time <= 5)
                    {
                        __time_alarm_update_alarm_close(p_cur_alarm_record);
                    }
                }
            }
        }
    }

    if (mode == UPDATE_ALARM_CLOSE_SAME)
    {
        return 0;
    }

    if (mode != UPDATE_ALARM_GET_NEXT)
    {
        if (alarm_available != 0xff)
        { //�д��˵����ӣ���������ʱ��
            p_cur_alarm_record = &(g_time_alarm_var.alarm_records[alarm_available]);
            sys_set_alarm_time(&(p_cur_alarm_record->last_time));
            g_time_alarm_var.alarm_idx = alarm_available;
        }
        else
        { //û�д��˵����ӣ��ر�����
            sys_set_alarm_disable();
            g_time_alarm_var.alarm_idx = 0xff;
        }
    }
    
    return alarm_available;
}

