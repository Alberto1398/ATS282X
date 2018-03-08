/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 14:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_applib_funs.c
 * \brief    applib ���̹���ģ�飨bank���֣�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻��� ���̹��� �ӿڡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ���� libc �� psp �Ľӿ�ʵ�֡�
 * \version 1.0
 * \date  2011-9-28
 *******************************************************************************/

#include "applib.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵĳ�ʼ��������Ӧ�õ� main ������ڴ�����
 * \param[in]    app_id��Ӧ��ID
 * \param[in]    type��Ӧ�����ͣ��μ�app_type_e ����
 * \param[out]   none
 * \return       bool
 * \retval           �����Ƿ��ʼ���ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \note
 *******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    //��ȡ app_info �ṹ��ָ��
    cur_app_info = &g_app_info_vector[type];

    //��ʼ��Ӧ����Ϣ
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1;//��־�ѱ�����ʹ��
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    cur_app_info->mq_id = (uint8) MQ_ID_MNG;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���첽��ʽ����Ӧ��˽����Ϣ
 * \param[in]    app_id��ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg��ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           ���ط����첽��Ϣ�Ƿ�ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \par          exmaple code
 * \code
 *               msg.type = MSG_KILL_APP;
 *               msg.content.data[0] = APP_ID_MUSIC;
 *               result = send_async_msg(APP_ID_MANAGER, &msg);
 *               if(result == TRUE)
 *               {}
 * \endcode
 * \note
 *******************************************************************************/
int send_async_msg(uint8 app_id, msg_apps_t *msg)
{
    btstack_msg_t cur_msg;
    mq_id_e mq_id;

    cur_msg.sem = NULL;
    cur_msg.reply = NULL;
    libc_memset(cur_msg.bd_addr, 0x0, BD_ADDR_LEN);

    //����Ŀ��Ӧ��
    if (get_engine_type_by_appid(app_id) != ENGINE_NULL)
    {
        mq_id = MQ_ID_EGN;
    }
    else if (app_id == APP_ID_BTSTACK)
    {
        mq_id = MQ_ID_BT;
    }
    else
    {
        mq_id = MQ_ID_DESK;
    }

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_msg.msg), msg, sizeof(msg_apps_t));

    //������Ϣ��������
    if ((int) sys_mq_send((uint32) (mq_id), (void *) &cur_msg) < 0)
    {
        return ERR_MSGQUEUE_FULL;
    }

    return ERR_NO_ERR;
}

