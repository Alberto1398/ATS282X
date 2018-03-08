/*******************************************************************************
 *                              US212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_msg_loop.c
 * \brief    ��ȡ��Ϣ����
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"

extern void dump_cache_info(void) __FAR__;
/******************************************************************************/
/*!
 * \par  Description:
 *  ѭ����ȡ��Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
app_result_e manager_message_loop(void)
{
    uint32 i = 0;
    private_msg_t pri_msg;
//    uint8 null_gui_app_count = 0;

    while (1)
    {
        CLEAR_WATCH_DOG;
        
        if ((int) sys_mq_receive(MQ_ID_MNG, &pri_msg) == 0)
        {
            manager_msg_callback(&pri_msg);
        }

        //����10ms�����������
        sys_os_time_dly(1);

        i++;

        if(i == 500)
        {
            i = 0;
            //dump_cache_info();
        }
    }
}
