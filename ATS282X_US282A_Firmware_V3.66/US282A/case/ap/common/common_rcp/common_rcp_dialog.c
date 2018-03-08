/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP �Ի�����ʾ����ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

const app_result_e button_result[][3] =
{
    { RESULT_DIALOG_OK, RESULT_DIALOG_CANCEL, RESULT_NULL },
    { RESULT_DIALOG_ABORT, RESULT_DIALOG_RETRY, RESULT_DIALOG_IGNORE },
    { RESULT_DIALOG_YES, RESULT_DIALOG_NO, RESULT_DIALOG_CANCEL },
    { RESULT_DIALOG_YES, RESULT_DIALOG_NO, RESULT_NULL },
    { RESULT_DIALOG_RETRY, RESULT_DIALOG_CANCEL, RESULT_NULL },
    /*�������Զ��尴ť�б�*/
    { RESULT_DIALOG_USB_SOUND, RESULT_DIALOG_ONLY_CHARGE, RESULT_NULL },
    { RESULT_NULL, RESULT_NULL, RESULT_NULL },
};

void dialog_overtime(void)
{
    g_dialog_overtime_flag = TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��APK���ʵ�ֶԻ���ѯ�ʺ���Ϣ��ʾ��
 * \param[in]    type �Ի������ͣ��� dialog_type_e ����
 * \param[in]    button_type ����������ͣ����������ڽ�������Ϻ��Զ��尴����ϣ��� button_type_e ����
 * \param[in]    desc_id �����ַ���ID����APKԼ��Ҫ��ʾʲô�����������Ϣ
 * \param[in]    active_default Ĭ�ϰ������
 * \param[in]    check_cb ���Ի����Ƿ�Ϸ����������FALSE�򷵻� RESULT_DIALOG_CANCEL_AUTO
 * \param[out]   none 
 * \return       app_result_e  �������� RESULT_DIALOG_XXX ��
 * \ingroup      common_func
 * \note   
 * \li  �����ⰴ��ȡ�����أ�����ֵΪĬ�ϰ�����
 *******************************************************************************/
app_result_e com_rcp_dialog(uint8 type, uint8 button_type, uint8 desc_id, uint8 active_default,
        dialog_check_callback check_cb)
{
    input_gui_msg_t ui_msg;
    app_result_e result = RESULT_NULL;
    int8 dialog_timer_id;
    uint8 last_dialog_state;

    g_dialog_type = type;
    g_dialog_button_type = button_type;
    g_dialog_desc_id = desc_id;
    g_dialog_active_default = active_default;
    last_dialog_state = g_dialog_state = DIALOG_STATE_LAUNCH;

    g_dialog_overtime_flag = FALSE;
    dialog_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL | (APP_TIMER_TAG_SYS << 8), 2000, dialog_overtime);

    while (1)
    {
        if (g_dialog_overtime_flag == TRUE)
        {
            g_dialog_type = DIALOG_TYPE_CANCEL;
            result = button_result[button_type][active_default];
            break;
        }

        if ((last_dialog_state == DIALOG_STATE_LAUNCH) && (g_dialog_state == DIALOG_STATE_CHOOSING))
        {
            if (g_dialog_overtime == (uint8) (-1))
            {
                result = button_result[button_type][active_default];
                break;
            }
            else
            {
                uint16 dialog_select_overtime;

                kill_app_timer(dialog_timer_id);

                dialog_select_overtime = (uint16) g_dialog_overtime * 1000 + 2000;
                dialog_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL | (APP_TIMER_TAG_SYS << 8),
                        dialog_select_overtime, dialog_overtime);
            }
        }
        else if ((last_dialog_state == DIALOG_STATE_CHOOSING) && (g_dialog_state == DIALOG_STATE_CONFIRM))
        {
            result = button_result[button_type][g_dialog_choosed];
            break;
        }
        else
        {
            //for QAC
        }

        last_dialog_state = g_dialog_state;

        //����Ի���Ƿ����Զ�ȡ���Ի���
        if ((check_cb != NULL) && (check_cb() == FALSE))
        {
            g_dialog_type = DIALOG_TYPE_CANCEL;
            result = RESULT_DIALOG_CANCEL_AUTO;
            break;
        }

        //��ȡgui��Ϣ
        if (get_gui_msg(&ui_msg) == TRUE) //��gui��Ϣ
        { //�����ⰴ��ȡ������
            g_dialog_type = DIALOG_TYPE_CANCEL;
            libc_memcpy(&g_buffer_gui_msg, &ui_msg, sizeof(input_gui_msg_t)); //���ݻ�����Ϣ
            result = button_result[button_type][active_default];
            break;
        }
        else
        {
            result = com_view_loop();
            if (result > RESULT_COMMON_RESERVE)
            {
                g_dialog_type = DIALOG_TYPE_CANCEL;
                break;
            }
        }

        //����10ms�����������
        sys_os_time_dly(1);
    }

    kill_app_timer(dialog_timer_id);
    g_dialog_state = DIALOG_STATE_NONE;
    return result;
}

//���USB�߲���Ի����Ƿ�Ϸ�
bool com_dialog_check_usbdialog(void)
{
    return (get_cable_state() == CABLE_STATE_CABLE_IN);
}
