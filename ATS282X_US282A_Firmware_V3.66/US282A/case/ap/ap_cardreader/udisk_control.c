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

#include "ap_udisk.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool udisk_send_msg(void *input_param, void *output_param, uint32 msg_type)
 * \�����淢����Ϣ
 * \param[in]    input_param �������ָ��
 * \param[in]    msg_type  ��Ϣ����
 * \param[out]   output_param �������ָ��
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      udisk_control.c
 * \note
 */
/*******************************************************************************/
bool udisk_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    int32 bret = TRUE;
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
    bret = (send_sync_msg(APP_ID_UENGINE, &msg, &reply, 0) == ERR_NO_ERR);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_get_status(mengine_status_t* play_status)
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
/*bool usound_get_status(mengine_status_t* play_status)
 {
 bool bret;
 bret = udisk_send_msg(NULL, play_status, MSG_UENGINE_GET_STATUS_SYNC);
 return bret;
 }*/

