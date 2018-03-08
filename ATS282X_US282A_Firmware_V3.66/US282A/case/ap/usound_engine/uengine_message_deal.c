/*******************************************************************************
 *                              US211A
 *                            Module: music engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     fiona.yang     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
#include "app_uengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e uengine_message_deal(private_msg_t* cur_msg)
 * \������Ϣ����,����������յ���˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       RESULT_APP_QUIT �˳�app
 * \note
 */
/*******************************************************************************/
app_result_e uengine_message_deal(private_msg_t* cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    //��Ϣ
    switch (cur_msg->msg.type)
    {

        case MSG_APP_QUIT: //�˳�app
        ret_value = RESULT_APP_QUIT;
        break;

        /* ����eq*/
        //case MSG_UENGINE_SET_EQ_SYNC:
        case MSG_SET_DAE_CFG_SYNC:
        if(0 == g_uspeaker_24bit)
        {
            uengine_set_eq(cur_msg);
        }
        uengine_reply_msg(cur_msg, TRUE);
        break;

        /* ��ȡ��Ϣ���*/
        case MSG_UENGINE_GET_STATUS_SYNC:
        uengine_reply_msg(cur_msg, TRUE);
        break;
        
        /* ������ͣ*/
        case MSG_UENGINE_PLAY_PAUSE_SYNC:
        uengine_play_pause();
        uengine_reply_msg(cur_msg, TRUE);
        break;

        /* �л���һ��*/
        case MSG_UENGINE_NEXT_SONG_SYNC:
        uengine_next_song();
        uengine_reply_msg(cur_msg, TRUE);
        break;
        
        /* �л���һ��*/
        case MSG_UENGINE_PREV_SONG_SYNC:
        uengine_prev_song();
        uengine_reply_msg(cur_msg, TRUE);
        break;
        
        /* ������*/
        case MSG_UENGINE_VOL_ADD_SYNC:
        uengine_vol_add();
        uengine_reply_msg(cur_msg, TRUE);
        break;
        
        /* ������*/
        case MSG_UENGINE_VOL_SUB_SYNC:
        uengine_vol_sub();
        uengine_reply_msg(cur_msg, TRUE);
        break;

        //׼������TTS����
        case MSG_TTS_START_SYNC:
        //׼���л���Ƶ���
        case MSG_SWITCH_AOUT_START_SYNC:
        uengine_tts_start();
        uengine_reply_msg(cur_msg, TRUE);
        break;

        //TTS�������
        case MSG_TTS_STOP_SYNC:
        case MSG_SWITCH_AOUT_STOP_SYNC: //����л���Ƶ���
        uengine_tts_stop();
        uengine_reply_msg(cur_msg, TRUE);
        break;

        case MSG_UENGINE_VOL_SEND_SYNC:
        g_current_vol = *((uint8 *)cur_msg->msg.content.addr);
        uengine_vol_sync((uint8)g_current_vol);
        uengine_reply_msg(cur_msg, TRUE);
        break;

        default:
        uengine_reply_msg(cur_msg, TRUE);
        break;
    }

    return ret_value;

}
