/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP �����������ӿڣ��ͻ����ɶ��ο�����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"
#include "rcp_control.h"

app_result_e com_rcp_cmd_dispatch_ap_switch(uint8 cmd_id) __FAR__;
app_result_e com_rcp_cmd_dispatch_bank(rcp_command_t *p_rcp_cmd, void *p_cur_param) __FAR__;

/*���RCP�Ϸ���*/
rcp_state_e __section__(".rcode") com_check_valid_rcp(uint8 *rcp_head, uint16 rcp_len)
{
#if (SUPPORT_RCP_FUNC == 1)
    uint16 tmp_cmd_len = COMMAND_LENGTH(rcp_head);
    uint8 magic_soh = COMMAND_MAGIC_SOH(rcp_head);
    uint8 magic_soh_r = COMMAND_MAGIC_SOH_R(rcp_head);

    if (((magic_soh != RCP_MAGIC_SOH) || (magic_soh_r != RCP_MAGIC_SOH_R)) || (tmp_cmd_len > g_rcp_pkg_max_len))
    {
        return RCP_STATE_INVALID;
    }

    if (tmp_cmd_len > rcp_len)
    {
        return RCP_STATE_HEAD;
    }
#endif
    return RCP_STATE_OK;
}

app_result_e com_rcp_cmd_dispatch_ap_switch(uint8 cmd_id)
{
    //�����л�Ӧ���¼���ǰ̨Ӧ��
    msg_apps_t rcp_msg;

    /*��������ӹ��Ӻ��������л�����AP�յ��л�Ӧ����Ϣ֮ǰ֪����APKҪ�л�Ӧ���ˣ�
     ���������﷢��ĳ�����ⰴ����Ϣ������ȡ��TTS������TTSֻ��ͨ������ȡ����*/
    com_apswitch_hook();

    rcp_msg.type = rcp_cmd_apswitch_event[cmd_id];
    broadcast_msg(&rcp_msg, TRUE);

    return RESULT_NULL;
}

/*RCP����ַ�����*/
app_result_e __section__(".rcode") com_rcp_cmd_dispatch(rmt_ctrl_pkg_t *p_rcp)
{
#if (SUPPORT_RCP_FUNC == 1)
    rcp_command_t rcp_cmd;
    void *p_cur_param;
    app_result_e result = RESULT_NULL;

    /* ����ַ�֮ǰ���Ӻ��� */
    com_rcp_cmd_dispatch_before_hook();

    libc_memcpy(&rcp_cmd, p_rcp->rcp_data, sizeof(rcp_command_t));
    if (p_rcp->rcp_len > sizeof(rcp_command_t))
    {
        p_cur_param = (uint8 *) p_rcp->rcp_data + sizeof(rcp_command_t);
    }
    else
    {
        p_cur_param = NULL;
    }

    g_rcp_cmd_flags[0] = rcp_cmd.flags[0];
    g_rcp_cmd_flags[1] = rcp_cmd.flags[1];
    g_rcp_cmd_id = rcp_cmd.cmd_id;

    PRINT_DBG_INT("com rcp:", (rcp_cmd.cmd_type << 8) + rcp_cmd.cmd_id);

    if ((rcp_cmd.cmd_type == RCP_CMD_QUERY) && (rcp_cmd.cmd_id == QUERY_ID_GLOBAL_RCP_INFO))
    {
        g_rcp_seg_flag = FALSE; //����ֶα�־

        if (g_app_get_global_info != NULL)
        {
            result = g_app_get_global_info(rcp_cmd.param1, rcp_cmd.param2);
        }
        else
        {
            result = com_get_global_rcp_info(rcp_cmd.param1, rcp_cmd.param2);
        }
    }
    else
    {
        result = com_rcp_cmd_dispatch_bank(&rcp_cmd, p_cur_param);
    }

    /* ����ַ�֮���Ӻ��� */
    //com_rcp_cmd_dispatch_after_hook();

    return result;
#else
    return RESULT_NULL;
#endif
}

app_result_e com_rcp_cmd_dispatch_bank(rcp_command_t *p_rcp_cmd, void *p_cur_param)
{
    app_result_e result = RESULT_NULL;

    /*SEQ��֤*/
    if ((g_rcp_cmd_flags[0] & RCP_SEGMENT_MASK) != RCP_SEGMENT_NONE)
    {
        if (g_rcp_seg_flag == FALSE)
        { //��δ��ʼ�ֶΣ������ǵ�һ����
            if ((g_rcp_cmd_flags[0] & RCP_SEGMENT_MASK) == RCP_SEGMENT_FIRST)
            {
                g_rcp_next_seq = 0;
                g_rcp_seg_flag = TRUE;
            }
            else
            {
                g_rcp_next_seq = 0xff; //����ֶα�־������ЧSEQ��
            }
        }

        if ((g_rcp_cmd_flags[1] & RCP_SEQ_MASK) != g_rcp_next_seq)
        {
            goto seq_check_fail;
        }
        else
        {
            g_rcp_next_seq++;
            g_rcp_next_seq %= 16;
        }
    }

    if (((g_rcp_cmd_flags[0] & RCP_SEGMENT_MASK) == RCP_SEGMENT_LAST)
            || ((g_rcp_cmd_flags[0] & RCP_SEGMENT_MASK) == RCP_SEGMENT_NONE))
    {
        g_rcp_seg_flag = FALSE;
    }

    switch (p_rcp_cmd->cmd_type)
    {
        /*��1�� 'P'��Ӧ���л���*/
        case RCP_CMD_APSWITCH:
        {
            result = com_rcp_cmd_dispatch_ap_switch(p_rcp_cmd->cmd_id);
            com_start_key_tone(KEYTONE_NOBLOCK);
        }
        break;

        /*��2�� 'C'��������*/
        case RCP_CMD_CONTROL:
        {
            rcp_ctrl_func this_rcp_ctrl_func = (rcp_ctrl_func) com_rcp_cmd_find_cb(RCP_CMD_CONTROL, p_rcp_cmd->cmd_id);

            if (this_rcp_ctrl_func != NULL)
            {
#ifndef ENABLE_TRUE_WIRELESS_STEREO
                if (get_mute_enable() == TRUE)
                {
                    change_mute_enable(FALSE);
                    com_reset_sound_volume(0);
                    g_ignore_switch_mute = TRUE;
                }
#endif
                com_start_key_tone(KEYTONE_NOBLOCK);
                result = this_rcp_ctrl_func(p_rcp_cmd->param1, p_rcp_cmd->param2);
            }
        }
        break;

        /*��4�� 'Q'����ѯ��*/
        case RCP_CMD_QUERY:
        {
            rcp_get_func this_rcp_get_func = (rcp_get_func) com_rcp_cmd_find_cb(RCP_CMD_QUERY, p_rcp_cmd->cmd_id);

            if (this_rcp_get_func != NULL)
            {
                result = this_rcp_get_func(p_rcp_cmd->param1, p_rcp_cmd->param2);
            }
        }
        break;

        /*��6�� 'S'������������*/
        case RCP_CMD_SETTING:
        { //���ò���
            rcp_set_func this_rcp_set_func = (rcp_set_func) com_rcp_cmd_find_cb(RCP_CMD_SETTING, p_rcp_cmd->cmd_id);

            if (this_rcp_set_func != NULL)
            {
                result = this_rcp_set_func(p_rcp_cmd->param1, p_rcp_cmd->param2, p_cur_param,
                        p_rcp_cmd->cmd_len - RCP_COMMAND_HEAD_SIZE);
            }
        }
        break;

        default:
        break;
    }

    seq_check_fail:
    //��ҪACK���Ҳ��ǲ�ѯ������
    if (((p_rcp_cmd->flags[0] & RCP_ACKFLAG_MASK) == RCP_ACKFLAG_ACK) && (p_rcp_cmd->cmd_type != RCP_CMD_QUERY))
    {
        com_rcp_send_ack_pkg(RCP_ACK_ID_ACK);
    }

    return result;
}
