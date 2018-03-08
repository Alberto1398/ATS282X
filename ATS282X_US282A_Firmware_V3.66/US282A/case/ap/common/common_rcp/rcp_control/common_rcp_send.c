/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP ����ظ����ͽӿڣ�������ѯ������Ӧ�����ACK/NACK����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"
#include "rcp_control.h"

/*���������һ�����ݰ�*/
void __section__(".rcode") com_rcp_send_pkg(uint32 data1, uint32 data2, void *data, uint16 data_len, uint16 data_total)
{
#if (SUPPORT_RCP_FUNC == 1)
    rcp_command_t tmp_rcp_answer;
    rcp_command_t *p_rcp_answer;
    uint8 *p_rcp_data;

    if (data != NULL)
    {
        p_rcp_answer = (rcp_command_t *) data;
    }
    else
    {
        p_rcp_answer = &tmp_rcp_answer;
    }

    //����ͬ����Ϣ��������̨
    p_rcp_answer->magic[0] = RCP_MAGIC_SOH;
    p_rcp_answer->magic[1] = RCP_MAGIC_SOH_R;
    p_rcp_answer->flags[0] = (g_rcp_cmd_flags[0] & RCP_CHAN_MASK);
    p_rcp_answer->flags[1] = 0x00;
    p_rcp_answer->cmd_type = RCP_CMD_ANSWER;
    p_rcp_answer->cmd_id = g_rcp_cmd_id;
    p_rcp_answer->cmd_len = data_total + RCP_COMMAND_HEAD_SIZE;
    p_rcp_answer->param1 = data1;
    p_rcp_answer->param2 = data2;

    p_rcp_data = (uint8 *) p_rcp_answer;
    
    com_btmanager_sppble_send(p_rcp_data, data_len + RCP_COMMAND_HEAD_SIZE);
#endif
}

/*���ͺ������ݰ�*/
void com_rcp_send_pkg_cont(void *data, uint16 data_len)
{
    com_btmanager_sppble_send(data, data_len);
}

/*����ACK���ݰ�*/
void com_rcp_send_ack_pkg(uint8 ack)
{
    rcp_command_t tmp_ack_pkg;

    //����ͬ����Ϣ��������̨
    tmp_ack_pkg.magic[0] = RCP_MAGIC_SOH;
    tmp_ack_pkg.magic[1] = RCP_MAGIC_SOH_R;
    tmp_ack_pkg.flags[0] = (g_rcp_cmd_flags[0] & RCP_CHAN_MASK);
    tmp_ack_pkg.flags[1] = 0x00;
    tmp_ack_pkg.cmd_type = RCP_CMD_ACK;
    tmp_ack_pkg.cmd_id = ack;
    tmp_ack_pkg.cmd_len = RCP_COMMAND_HEAD_SIZE;
    tmp_ack_pkg.param1 = 0;
    tmp_ack_pkg.param2 = 0;

    com_btmanager_sppble_send(&tmp_ack_pkg, tmp_ack_pkg.cmd_len);
}


/*����rcp�����*/
void com_rcp_send_cmd_pkg(uint8 cmd_id, uint8 cmd_type,uint8 dev_id)
{
    rcp_command_t tmp_cmd_pkg;
    
    //���͸��ĸ��豸
    g_rcp_remote_number = dev_id;
    //��ǰ��client
    g_rcp_remote_number |= (uint8)RCP_ROLE_CLIENT;

    //����ͬ����Ϣ��������̨
    tmp_cmd_pkg.magic[0] = RCP_MAGIC_SOH;
    tmp_cmd_pkg.magic[1] = RCP_MAGIC_SOH_R;
    tmp_cmd_pkg.flags[0] = 0;//(g_rcp_cmd_flags[0] & RCP_CHAN_MASK);
    tmp_cmd_pkg.flags[1] = 0x00;
    tmp_cmd_pkg.cmd_type = cmd_type;
    tmp_cmd_pkg.cmd_id = cmd_id;
    tmp_cmd_pkg.cmd_len = RCP_COMMAND_HEAD_SIZE;
    tmp_cmd_pkg.param1 = 0;
    tmp_cmd_pkg.param2 = 0;

    com_btmanager_sppble_send(&tmp_cmd_pkg, tmp_cmd_pkg.cmd_len);
}
