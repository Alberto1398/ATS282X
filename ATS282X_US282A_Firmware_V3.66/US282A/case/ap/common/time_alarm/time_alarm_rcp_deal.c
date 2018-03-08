/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������APK/APP ���ӹ��ܽӿڣ����������б�������Ӳ����������ȡ�ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_time_alarm.h"

/*��ȡ���Ӽ�¼�б�*/
app_result_e time_alarm_get_alarm_list(uint32 data1, uint32 data2)
{
    alarm_record_t cur_alarm_record;
    uint8 alarm_cnt, alarm_index, i;
    uint8 tmp_ring_folder_type[RCP_RING_FOLDER_MAX];
    uint8 tmp_ring_foldernum[RCP_RING_FOLDER_MAX];

    //������Ѱγ�����ô��������Ŀ¼��Ŀ��0
#if (SUPPORT_RING_FOLDER == 1)
    if (get_card_state() == CARD_STATE_CARD_NULL)
    {
        libc_memset(tmp_ring_foldernum, 0, RCP_RING_FOLDER_MAX);
        time_alarm_record_list_operate(RECORD_LIST_WRITE_FOLDERNUM, tmp_ring_foldernum, 0);
    }
#endif

    alarm_cnt = (uint8) time_alarm_record_list_operate(RECORD_LIST_VALID_TOTAL, 0, 0);

    com_load_share_data(COM_SHARE_DATA_RING_TYPE, tmp_ring_folder_type, RCP_RING_FOLDER_MAX, 0);
    time_alarm_record_list_operate(RECORD_LIST_READ_FOLDERNUM, (void *) tmp_ring_foldernum, 0);

    com_rcp_send_pkg(alarm_cnt, 0, NULL, 0, sizeof(alarm_alarm_t) * alarm_cnt);
    for (alarm_index = 0; alarm_index < RCP_ALARM_MAX; alarm_index++)
    {
        time_alarm_record_list_operate(RECORD_LIST_READ_BYINDEX, &cur_alarm_record, (void *) (uint32) alarm_index);

        //���Ӽ�¼�Ϸ��Լ�飬������Ϸ����Զ���Ϊ���������һ��
        for (i = 0; i < RCP_RING_FOLDER_MAX; i++)
        {
            if (cur_alarm_record.alarm.ring_type == tmp_ring_folder_type[i])
            {
                if (cur_alarm_record.alarm.sound.ring_seq > tmp_ring_foldernum[i])
                {
                    cur_alarm_record.alarm.ring_type = RING_TYPE_BUILTIN;
                    cur_alarm_record.alarm.sound.ring_seq = 1;
                    time_alarm_record_list_operate(RECORD_LIST_ADD_BYINDEX, &cur_alarm_record, \
                            (void *) (uint32) alarm_index);
                }
                break;
            }
        }

        if (cur_alarm_record.alarm.alarm_used == 1)
        {
            com_rcp_send_pkg_cont(&(cur_alarm_record.alarm), sizeof(alarm_alarm_t));
        }
    }

    return RESULT_NULL;
}

/*���/�������Ӽ�¼*/
app_result_e time_alarm_set_alarm_add(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    alarm_record_t cur_alarm_record;
    alarm_alarm_t *p_alarm_info = (alarm_alarm_t *) data;
    int ret_index;

    //�ȼ����Ƿ����и����Ӽ�¼�������δ�иü�¼�������������ÿհ׼�¼
    ret_index = time_alarm_record_list_operate(RECORD_LIST_SEARCH_BYID, &cur_alarm_record, \
            (void *) (uint32) (p_alarm_info->alarm_id));
    if (ret_index < 0)
    {
        ret_index = time_alarm_record_list_operate(RECORD_LIST_SEARCH_NOUSED, 0, 0);
        if (ret_index < 0)
        {
            return RESULT_NULL;
        }
    }

    //��ӻ�������Ӽ�¼
    libc_memcpy(&(cur_alarm_record.alarm), p_alarm_info, sizeof(alarm_alarm_t));
    cur_alarm_record.alarm.alarm_used = 1;
    libc_memcpy(&(cur_alarm_record.last_time), &(cur_alarm_record.alarm.alarm_time), sizeof(time_t));
    cur_alarm_record.snooze = 0;
    time_alarm_record_list_operate(RECORD_LIST_ADD_BYINDEX, &cur_alarm_record, (void *) ret_index);

    //ˢ����������
    time_alarm_record_list_operate(RECORD_LIST_UPDATE_ALARM, UPDATE_ALARM_ADD, 0);

    return RESULT_NULL;
}

/*ɾ�����Ӽ�¼*/
app_result_e time_alarm_set_alarm_delete(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    alarm_record_t cur_alarm_record;
    int alarm_id = (int) data1;
    int ret_index;

    if (alarm_id == -1)
    { //ɾ��ȫ�����Ӽ�¼
        ret_index = -1;
    }
    else
    { //ɾ��ָ�����Ӽ�¼
        ret_index = time_alarm_record_list_operate(RECORD_LIST_SEARCH_BYID, &cur_alarm_record, (void *) alarm_id);
        if (ret_index < 0)
        {
            return RESULT_NULL;
        }
    }

    time_alarm_record_list_operate(RECORD_LIST_DELETE_BYINDEX, 0, (void *) ret_index);
    //ˢ����������
    time_alarm_record_list_operate(RECORD_LIST_UPDATE_ALARM, (void *) UPDATE_ALARM_DEL, 0);

    return RESULT_NULL;
}

//��ȡ���Ӳ���
app_result_e time_alarm_get_alarm_param(uint32 data1, uint32 data2)
{
    uint8 tmp_param_buf[RCP_COMMAND_HEAD_SIZE + sizeof(alarm_param_t)];
    alarm_param_t *p_tmp_alarm_param = (alarm_param_t *) (tmp_param_buf + RCP_COMMAND_HEAD_SIZE);

    p_tmp_alarm_param->snooze_time = sys_comval->snooze_timer;
    p_tmp_alarm_param->snooze_max_count = sys_comval->snooze_max;
    p_tmp_alarm_param->overtime = sys_comval->alarm_overtime;

    com_rcp_send_pkg(0, 0, tmp_param_buf, sizeof(tmp_param_buf), sizeof(tmp_param_buf));

    return RESULT_NULL;
}

//�������Ӳ���
app_result_e time_alarm_set_alarm_param(uint32 data1, uint32 data2, void *data, uint16 data_len)
{
    alarm_param_t *p_tmp_alarm_param = (alarm_param_t *) data;

    if (p_tmp_alarm_param->snooze_time <= 30)
    {
        sys_comval->snooze_timer = p_tmp_alarm_param->snooze_time;
    }
    if (p_tmp_alarm_param->snooze_time <= 10)
    {
        sys_comval->snooze_max = p_tmp_alarm_param->snooze_max_count;
    }
    if (p_tmp_alarm_param->snooze_time <= 30)
    {
        sys_comval->alarm_overtime = p_tmp_alarm_param->overtime;
    }

    return RESULT_NULL;
}
