/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������Ϣ�����Ӻ������ڰ�����Ϣ��Ӧ֮ǰִ��һЩ���������������˵�����
 *       ��Ϣ�����������������������ϵͳ��ʱ�����㣬����UI��ʾ�ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    �����������⴦����Ϊ gui ��Ϣ����Ļص�������
 * \param[in]    input_msg ��ȡ���� gui��Ϣָ��
 * \param[out]   input_msg ���ܻ��input_msg�����޸ģ����罫val�޸�ΪKEY_HOLD���ⰴ��
 * \return       bool
 * \retval           TRUE ��ȡ���İ�����Ч��û�б����˵���
 * \retval           FALSE ��ȡ���İ�����Ч�������˵��ˡ�
 * \ingroup      common_func
 * \note
 *******************************************************************************/
bool com_gui_msg_hook(input_gui_msg_t *input_msg)
{
    bool ret;
    key_value_e key_val = input_msg->data.kmsg.val;
    key_type_e key_type = input_msg->data.kmsg.type;

    //1.��������������˵�����������������⵽����̧�𣬻��������������£���ð����������ڣ����������ɣ�
    if (key_val == g_key_infor.filter_key_itself)
    {
        if ((key_type == KEY_TYPE_DOWN) //�յ��ð����������ڣ��������
            || (key_type == KEY_TYPE_DBL_CLICK))//˫���ð���Ҳ�������ڣ�������� //�ڶ��γ�����Ҳ��������
        {
            g_key_infor.filter_key_itself = KEY_NULL;
        }
        else
        {
           //����LONG��HOLD����
            if (key_type == KEY_TYPE_LONG_10S)
            {
                return FALSE;
            }
            if ((key_type != KEY_TYPE_SHORT_UP) && (key_type != KEY_TYPE_LONG_UP))                    
            {
                return FALSE;
            }
            else //������ɣ������ǲ���ӦSHORT_UP����
            {
                this_filter_key_value = KEY_NULL;
                g_key_infor.filter_key_itself = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //�յ��°������������
    {
        g_key_infor.filter_key_itself = KEY_NULL;
    }

    //1.������������˵�������������⵽����̧�𣬻��������������£���ð����������ڣ����������ɣ�
    if (key_val == g_key_infor.filter_key)
    {
        //���˻�û��ɾ��յ�DOWN��Ϣ������SHORTUP��Ϣ���л�Ӧ��ʱ��������ˣ�
        //��ôDOWN��Ϣ�������µİ������ڵ���Ϣ����Ӧ�ñ�����
        if ((key_type == KEY_TYPE_DOWN) //�յ��ð����������ڣ��������
                || (key_type == KEY_TYPE_DBL_CLICK)) //˫���ð���Ҳ�������ڣ��������
        {
            g_key_infor.filter_key = KEY_NULL;
        }
        else
        {
            if(key_type == KEY_TYPE_LONG_10S)
            {
                g_key_infor.filter_key = KEY_NULL;
                this_filter_key_value = KEY_NULL;
                goto LONG10S_DEAL;
            }
            //����LONG��HOLD����
            if ((key_type != KEY_TYPE_SHORT_UP)
                    && (key_type != KEY_TYPE_LONG_UP))
            {
                return FALSE;
            }
            else //������ɣ������ǲ���ӦSHORT_UP����
            {
                this_filter_key_value = KEY_NULL;
                g_key_infor.filter_key = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //�յ��°������������
    {
        g_key_infor.filter_key = KEY_NULL;
    }

    //2.������������˵�����̧�𰴼�����⵽����̧�𣬻��������������£���ð����������ڣ����������ɣ�
    if (key_val == g_key_infor.filter_key_up)
    {
        //���˻�û��ɾ��յ�DOWN��Ϣ������SHORTUP��Ϣ���л�Ӧ��ʱ��������ˣ�
        //��ôDOWN��Ϣ�������µİ������ڵ���Ϣ����Ӧ�ñ�����
        if ((key_type == KEY_TYPE_DOWN) //�յ��ð����������ڣ��������
                || (key_type == KEY_TYPE_DBL_CLICK)) //˫���ð���Ҳ�������ڣ��������
        {
            g_key_infor.filter_key_up = KEY_NULL;
        }
        else
        {
            //������̧�𰴼�
            if ((key_type == KEY_TYPE_SHORT_UP)
                    || (key_type == KEY_TYPE_LONG_UP))
            {
                this_filter_key_value = KEY_NULL;
                g_key_infor.filter_key_up = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //�յ��°������������
    {
        g_key_infor.filter_key_up = KEY_NULL;
    }

    LONG10S_DEAL:

    //���µ�ǰ����ֵ
    if ((key_type == KEY_TYPE_SHORT_UP) //��������Ѿ�̧����ô���ù��˰�����Ч
    || (key_type == KEY_TYPE_LONG_UP))
    {
        this_filter_key_value = KEY_NULL;
    }
    else
    {
        this_filter_key_value = key_val;
    }

    //3.�а�������Ҫ�������
    g_ignore_switch_mute = FALSE;
    if (get_mute_enable() == TRUE)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        com_switch_mute(1,0,NULL);
#else
        change_mute_enable(FALSE);
        g_ignore_switch_mute = TRUE;
#endif
        com_reset_sound_volume(0);
        
    }

    //4.�������������ڰ�������ʱ����������
    if ((key_type == KEY_TYPE_DOWN) && (input_msg->data.kmsg.option == 0))
    {
        //û��Ƶ������Ǿ���״̬���Ұ�����ʹ�ܣ��ŷ���������
        com_start_key_tone(KEYTONE_NOBLOCK);
    }

    ret = TRUE;

    //5.ʡ��ػ�,������ʱ��0
    g_sys_counter.poweroff_counter = 0;
    g_sys_counter.light_counter = 0;

    if (get_backlight_state() == BACKLIGHT_STATE_OFF)
    {
        com_set_display_onoff(TRUE);
    }

    //6.����Ƶ��
    g_sys_counter.ui_idle_counter = 0;
    if (g_app_info_state_all.ui_status == UI_STATUS_IDLE_LOWFREQ)
    {
        adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL7, FREQ_NULL);
        g_app_info_state_all.ui_status = UI_STATUS_IDLE;
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���˵���ǰ������������������ֵͬ������Ϣ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 * \li  �ýӿ����ڹ��˵�ǰ����ֵ�ĺ���������������˵�����ָ����������ֱ�ӵ���
 *      change_filter_key ���á�
 * \li  �ڼ�⵽��ǰ����̧����Ϣ��������������Ϣ���� com_gui_msg_hook �Զ����ý����
 * \li  �����ʱû�а������£��ýӿڵ��ò�����������İ�������Ч����
 *******************************************************************************/
void com_filter_key_hold(void)
{
    g_key_infor.filter_key = this_filter_key_value;
}

void com_filter_key_up(void)
{
    g_key_infor.filter_key_up = this_filter_key_value;
}

void com_filter_key_itself(void)
{
    //libc_print("this_filter_key_value",this_filter_key_value,2);
    g_key_infor.filter_key_itself = this_filter_key_value;
}

/*! \endcond */
