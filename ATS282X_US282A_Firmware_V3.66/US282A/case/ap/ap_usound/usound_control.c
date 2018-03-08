/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-22          1.0              create this file
 *******************************************************************************/

#include "ap_usound.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_send_msg(void *input_param, void *output_param, uint32 msg_type)
 * \�����淢����Ϣ
 * \param[in]    input_param �������ָ��
 * \param[in]    msg_type  ��Ϣ����
 * \param[out]   output_param �������ָ��
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_send_msg(void *input_param, void *output_param, uint32 msg_type)
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
    bret = (uint8) (send_sync_msg(APP_ID_UENGINE, &msg, &reply, 0) == ERR_NO_ERR);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_volume_add(void)
 * \�Ӵ�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_volume_add(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_UENGINE_VOL_ADD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_volume_sub(void)
 * \��С����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_volume_sub(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_UENGINE_VOL_SUB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_pause(void)
 * \������ͣ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_play_pause(void)
{
    bool bret;
    bret = usound_send_msg(NULL, NULL, MSG_UENGINE_PLAY_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_next(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_play_next(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_UENGINE_NEXT_SONG_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_prev(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_play_prev(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_UENGINE_PREV_SONG_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_tts_start(void)
 * \��ʼ����tts
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_tts_start(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_TTS_START_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_tts_stop(void)
 * \tts�������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_tts_stop(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_TTS_STOP_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_get_status(uint32* play_status)
 * \��ȡ��ǰ״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool __section__(".rcode") usound_get_status(uengine_status_t* play_status)
{
    if (sys_share_query_read(APP_ID_UENGINE, play_status) == -1)
    {
        PRINT_ERR("usound share query not exist!\n");
        return FALSE;
    }

    return TRUE;
}

