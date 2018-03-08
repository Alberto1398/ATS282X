/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ�ý��հ�����Ϣ�ӿ�ʵ�֣��ýӿڽ���ǰ̨Ӧ���е��á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ���� gui ��Ϣ������������Ϣ��
 * \param[in]    none
 * \param[out]   input_msg�����ؽ��յ��� gui ��Ϣ
 * \return       bool
 * \retval           TRUE ���� gui ��Ϣ�ɹ���
 * \retval           FALSE ���� gui ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1��gui ��Ϣ���¼���ѭ��
 input_gui_msg_t input_msg;
 msg_apps_type_e gui_event;
 bool ret;

 ret = get_gui_msg(&input_msg);
 if(ret == TRUE)
 {
 if(com_key_mapping(&input_msg, &gui_event, key_map_list) == TRUE)
 {
 switch(gui_event)
 {
 case EVENT_DO_SOMETHING:
 break;

 default:
 result = com_message_box(gui_event);
 break;
 }
 }
 }
 else
 {
 gui ��Ϣ���¼���������ϣ����������Դ���Ӧ��˽����Ϣ��
 }
 * \endcode
 * \note
 *******************************************************************************/
bool get_gui_msg(input_gui_msg_t *input_msg)
{
    uint32 tmp_cur_ab_timer = sys_get_ab_timer();
    int key_cnt;
    bool ret;

    if (g_buffer_gui_msg.data.kmsg.val != KEY_NULL)
    {
        //������Ϣ��ֱ�ӿ���
        libc_memcpy(input_msg, &g_buffer_gui_msg, sizeof(input_gui_msg_t));
        g_buffer_gui_msg.data.kmsg.val = KEY_NULL;
        return TRUE;
    }

    key_cnt = sys_mq_traverse(MQ_ID_GUI, NULL, 0);
    if (key_cnt <= 0)
    {
        if ((uint16) ((uint16) tmp_cur_ab_timer - g_key_infor.last_detect_timestamp) > 500)
        {
            g_key_infor.status = KEY_STATUS_NULL;
            g_key_infor.dck_flag = FALSE;
            g_key_infor.dck_fail = FALSE;
        }
        return FALSE;
    }
    else
    {
        g_key_infor.last_detect_timestamp = (uint16) tmp_cur_ab_timer;
    }

    if (g_key_infor.dck_flag == TRUE)
    {
        if (com_need_cont_dck_deal((uint8) key_cnt) == FALSE)
        {
            return FALSE;
        }
    }

    if (com_phy_key_fsm(input_msg) == FALSE)
    {
        return FALSE;
    }

    //�յ�gui��Ϣ����ʾ���û�����������һЩϵͳ�������磺
    //0.������������˵���������
    //1.����رռ�ʱ����Ļ������ʱ��ʡ��ػ���ʱ���������ڲ��ż�ʱ ��0
    //2.����������
    //3.����������
    ret = g_this_gui_msg_hook(input_msg);

    return ret;
}

/*! \endcond */
