/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ӿڣ����������������л�����ģʽ��������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "common_func.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰ����������
 * \param[in]    none
 * \param[out]   none
 * \return       uint8 ��ǰ����ֵ��0xff��ʾ��Ч
 * \note
 *******************************************************************************/
uint8 com_get_sound_volume(void)
{
    return sys_comval->volume_current;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���õ�ǰ����������
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���������ɹ�
 * \retval           FALSE ��������ʧ��
 * \note
 *******************************************************************************/
bool com_reset_sound_volume(uint8 set_mode)
{
    sys_comval->volume_current_phy = 0xff;

    return com_set_sound_volume(sys_comval->volume_current, set_mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��������������
 * \param[in]    set_vol ��������ֵ
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���������ɹ�
 * \retval           FALSE ��������ʧ��
 * \note
 *******************************************************************************/
bool com_set_sound_volume(uint8 set_vol, uint8 set_mode)
{
    bool bret = TRUE;
    
    if(set_vol > VOLUME_VALUE_MAX)
    {
        libc_print("vol err:",set_vol,2);
        set_vol = (uint8) com_get_config_default(SETTING_SOUND_DEFAULT_MAIN_VOLUME);
    }

    sys_comval->volume_current = set_vol;
#pragma __PRQA_IGNORE_START__
    if (sys_comval->volume_mode >= VOLUME_MODE_MAX)
    {
        PRINT_WARNING("Invalid volume mode!");
    }
    else
    {
        sys_comval->volume[sys_comval->volume_mode] = set_vol;
    }
#pragma __PRQA_IGNORE_END__

    if (get_mute_enable() == FALSE)
    {


        if ((g_app_info_vector[APP_TYPE_BTSTACK].used == 1)
                && ((set_mode & SET_VOLUME_SYNC_TO_REMOTE) != 0))
        {
#ifndef ENABLE_TRUE_WIRELESS_STEREO
            if (g_this_app_info->app_id == APP_ID_BTPLAY)
#else
            if ((g_this_app_info->app_id == APP_ID_BTPLAY)
                ||(g_this_app_info->app_id == APP_ID_LINEIN))
#endif                
            {
                com_btmanager_avrcp_update_volume_to_phone(set_vol);
            }
            else if (g_this_app_info->app_id == APP_ID_BTCALL)
            {
                com_btmanager_hfp_update_volume_to_phone(set_vol);
            }
            else
            {
                ; //nothing for QAC
            }
        }
        if (g_app_info_state.inner_pa_inited == TRUE)
        {
            com_set_phy_volume(set_vol);
        }       
    }

    return bret;
}

//�л�����ģʽ
void com_switch_volume_mode(volume_mode_e mode)
{
    if (mode >= VOLUME_MODE_MAX)
    {
        PRINT_WARNING("invalid volume mode!!");
        return;
    }

    sys_comval->volume_current_phy = 0xff;
    sys_comval->volume_mode = mode;
    sys_comval->volume_current = sys_comval->volume[mode];
}

//�������������棨˥����
void com_set_volume_gain(uint8 gain)
{
    if (gain > VOLUME_TABLE_MAX)
    {
        PRINT_WARNING("Invalid volume gain!!");
        gain = VOLUME_TABLE_MAX;
    }

    sys_comval->volume_gain = gain;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ������رվ�����
 * \param[in]    mute TRUE��ʾ������FALSE��ʾ�ͷž���
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���þ����ɹ���
 * \retval           FALSE ���þ���ʧ�ܡ�
 * \ingroup      common_func
 * \note
 *******************************************************************************/
bool com_set_mute(bool mute)
{
    sys_comval->volume_current_phy = 0xff;

    if (mute == TRUE)
    {
        if (get_mute_enable() == FALSE) //��ǰΪ�Ǿ���״̬
        {
            //�ȴ�����������������������������������
            keytone_play_deal_wait();
            //����PA����Ϊ0����Ϊ����״̬
            com_set_phy_volume(0); //ֱ�ӵ��� com_set_phy_volume ���ã��Է�ֹ�޸��� volume_current
            change_mute_enable(TRUE);
        }
    }
    else
    {
        if (get_mute_enable() == TRUE) //��ǰΪ����״̬
        {
            //��Ϊ�Ǿ���״̬��������PA����
            change_mute_enable(FALSE);
            com_reset_sound_volume(0);
        }
    }

    return TRUE;
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //����״̬ת��
    bool com_switch_mute(uint8 op1,uint8 op2,bool op3)
    {
        bool cur_mute_state=0;
        cur_mute_state=get_mute_enable();
        //tws mod:need to notify the other
        if(op1==1)
        {
            if(g_bt_stack_cur_info.dev_role!=0)
            {
                com_btmanager_tws_send(MUTE_STATE_FLAG,(uint8)(1- cur_mute_state)\
                    ,NULL,NULL,MSG_BTSTACK_TWS_SEND_MSG_SYNC);
            }
        }
        if(op2==2)
        {
            //tws accept set mute cmd
            com_set_mute(op3);
        }
        else
        {
            if (cur_mute_state == TRUE) //��ǰΪ����״̬
            {
                com_set_mute(FALSE);
            }
            else //��ǰΪ�Ǿ���״̬
            {
                com_set_mute(TRUE);
            }
        }
        return TRUE;
    }

#else
    //����״̬ת��
    bool com_switch_mute(void)
    {
        if (get_mute_enable() == TRUE) //��ǰΪ����״̬
        {
            com_set_mute(FALSE);
        }
        else //��ǰΪ�Ǿ���״̬
        {
            com_set_mute(TRUE);
        }

        return TRUE;
    }
#endif

