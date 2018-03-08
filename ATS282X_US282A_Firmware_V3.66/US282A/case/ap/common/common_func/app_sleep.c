/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ��˯�߽ӿڣ�˯�߹����п�ͨ��������ǰ��ֹ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ�ò����ߣ����԰����жϣ����Ҹð�����Ϣ���˳� app_sleep �󱻽��յ���
 * \param[in]    sleep_timer ˯��ʱ�䣬��msΪ��λ
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_NULL ˯��ʱ�䵽ʱ���أ���Ч
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
 ����1���ȴ�2���ӣ������ⰴ���˳�
 app_result_e result;
 result = com_app_sleep(2000);

 * \endcode
 * \note
 *******************************************************************************/
app_result_e com_app_sleep(uint32 sleep_timer)
{
    input_gui_msg_t input_msg;
    uint32 sleep_100ms_timer;

    //���� 100ms ��ʱ��
    sleep_100ms_timer = sleep_timer / 100;
    if (sleep_100ms_timer == 0)
    {
        return RESULT_NULL;
    }
    while (1)
    {
        //��ȡgui��Ϣ
        while (get_gui_msg(&input_msg) == TRUE) //��gui��Ϣ
        {
            //�����ⰴ������
            libc_memcpy(&g_buffer_gui_msg, &input_msg, sizeof(input_gui_msg_t)); //���ݻ�����Ϣ
            return RESULT_NULL;
        }

        //����100ms�����������
        sys_os_time_dly(10);

        //˯��ʱ���ʱ���ݼ�
        sleep_100ms_timer--;
        //˯��ʱ���ѵ����˳�
        if (sleep_100ms_timer == 0)
        {
            return RESULT_NULL;
        }
    }
}

/*! \endcond */
