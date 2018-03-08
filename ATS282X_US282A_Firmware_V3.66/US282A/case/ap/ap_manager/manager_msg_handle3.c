/*******************************************************************************
 *                              us212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 20:38     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_message_send.c
 * \brief    applib ��Ϣͨ�Ż��� �����ࣨbank�� �ӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ϣͨ�Ż��ƣ���Ҫ������Ϣ���ͻ��ơ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ���� libc �� psp �Ľӿڣ��Լ�applib ���̹���ģ��ӿ�ʵ����Ϣ���ơ�
 * \version 1.0
 * \date  2011-9-27
 *******************************************************************************/

#include "applib.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  �ȴ�ǰ����̨ap�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music
 * \note
 *******************************************************************************/
void wait_ap_exit(void)
{
    uint8 ap_ret;
    int32 waitpid_ret;

    while (1)
    {
        waitpid_ret = libc_waitpid(&ap_ret, 0);
        if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
        {
            break;
        }
    }

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ɱ��ap����
 * \param[in]    ap_id ap���
 * \param[out]   none
 * \return       none
 * \ingroup      music
 * \note
 *******************************************************************************/
void kill_app(uint8 ap_id, uint8 ap_param)
{
    msg_apps_t async_msg;

    async_msg.type = MSG_APP_QUIT;
    async_msg.content.data[0] = ap_param;
    send_async_msg(ap_id, &async_msg);

    wait_ap_exit();

    return;
}
