/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������ʱ��ִ�нӿڣ���Ҫ��פ�ڴ棻������Ӧ�ú�BT STACKʹ�á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ�ü���ʱ�� handle ������Ӧ����Ϣѭ����ѭ�����ã�ѭ��ɨ�����ж�ʱ����ִ��
 *    ��ʱ����Ķ�ʱ���ķ������� proc�������ݶ�ʱ�����ͽ������á�
 * \param[in]    timers Ӧ����ʱ�����飬����commonȫ����ʱ������
 * \param[in]    count ��ʱ������
 * \param[out]   none
 * \return       none
 * \ingroup      app_timer
 * \note
 *******************************************************************************/
void handle_timers(app_timer_t *timers, uint8 count)
{
    app_timer_t *tm;
    uint32 cur_time_ms = sys_get_ab_timer(); //���û��ִ����ʱ���������̣���Ϊ��ǰʱ�䲻��
    uint32 next_time_ms;
    uint16 next_timeout;
    uint8 i;
    bool need_proc;

    for (i = 0; i < count; i++)
    {
        tm = timers + i;

        //��ʱ���������в���Ҫִ��
        if (tm->state != TIMER_STATE_RUNNING)
        {
            continue;
        }

        //TAG�������
        if ((tm->tag != g_this_app_timer_tag) && (tm->tag != APP_TIMER_TAG_SYS))
        {
            continue;
        }

        g_app_timer_id = i;
        next_timeout = tm->timeout;
        if ((tm->twinkle == TRUE) && (tm->on_off == FALSE))
        {
            next_timeout = tm->timeout_off;
        }
        next_time_ms = cur_time_ms + next_timeout;
        need_proc = FALSE;
        if (cur_time_ms >= tm->timeout_expires) //��ʱʱ���ѵ�
        {
            //������ʱ��һ��ִ����� kill ��
            if (tm->type == TIMER_TYPE_SINGLE_SHOT)
            {
                tm->state = TIMER_STATE_NOUSED;
            }
            need_proc = TRUE;

            //������ʱ����ʱʱ��
            if (tm->timeout > (cur_time_ms - tm->timeout_expires))
            {
                //��ǰʱ��>=���expiresʱ��<�´�expiresʱ��
                tm->timeout_expires += next_timeout; //ֱ����expires�ϼӶ�ʱ���ڣ���ȷ����ζ�ʱʱ��׼ȷ
            }
            else
            {
                //��ǰʱ��>=�´�expiresʱ��
                //�Ѿ������һ��expires(˵��timeout̫����)
                //���´�expiresʱ�����õ���ǰʱ���timeout֮��, ��Ȼ�´�timeout�����ͻᵽ��
                tm->timeout_expires = next_time_ms;
            }

            if (tm->twinkle == TRUE)
            {
                if (tm->on_off == TRUE)
                {
                    tm->on_off = FALSE;
                }
                else
                {
                    tm->on_off = TRUE;

                    if (tm->count != 0xff)
                    {
                        tm->count--;
                    }
                    if (tm->count == 0)
                    {
                        tm->state = TIMER_STATE_NOUSED;
                        need_proc = FALSE;
                    }
                }
            }
        }

        if (next_time_ms <= tm->timeout_expires)
        {
            //���ϵͳʱ����ĳ��ԭ�����ڳ�ʱʱ�����棬��ô�����޸���ʱ���
            tm->timeout_expires = next_time_ms;
        }

        //������ʱ����������
        if ((need_proc == TRUE) && (tm->func_proc != NULL))
        {
            tm->func_proc();
        }

        //ִ�й���ʱ���������̣����¼��ص�ǰʱ�䣬����ʱ���Ϊ׼ȷ
        cur_time_ms = sys_get_ab_timer();
    }
}

/*! \endcond */
