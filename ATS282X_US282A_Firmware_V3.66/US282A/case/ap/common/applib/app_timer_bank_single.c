/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������BT MANAGER ר����ʱ������������ʱ���ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��������Ӧ�ü���ʱ����
 * \param[in]    attrb ��ʱ�����ԣ�������һ�ַ��࣬�� control �� ui ��
 * \param[in]    timeout ��ʱ�����ڣ��� 1ms Ϊ��λ
 * \param[in]    func_proc ��ʱ����������
 * \param[out]   none
 * \return       int8
 * \retval           id ����ɹ������ض�ʱ��ID����ΧΪ 0 ~ g_app_timer_count - 1��
 * \retval           -1 ����ʧ�ܣ���ʱ����Դ���꣬�� timeout Ϊ0���� proc ΪNULL��
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1������ 5s ��ʱ��ʱ������������ĳ���¼�
 //5s ��ʱ��ʱ�� ISR���ñ�־ g_timeout_flagd
 static void __timeout_handle(void)
 {
 g_timeout_flagd = TRUE;
 }

 bool g_timeout_flagd = FALSE;//ȫ�ֱ���
 int8 timeout_timer = -1;
 //���� control�� Ӧ�ü���ʱ��
 timeout_timer = set_single_shot_app_timer(APP_TIMER_ATTRB_UI, 5000, __timeout_handle);
 if(timeout_timer == -1)
 {
 ����ʧ�ܣ�ԭ��Ϊ��ʱ���ѱ�������
 }

 //ɾ����ЧӦ�ü���ʱ��������ܹ�ȷ��������ʱ����ִ�е�����ô�Ͳ���Ҫִ��ɾ��������
 if(timeout_timer != -1)
 {
 kill_app_timer(timeout_timer);//����ڴ�֮ǰ������ʱ��������ɾ�����᷵�ش��󣬵��ǲ������
 timeout_timer = -1;
 }
 * \endcode
 * \note
 * \li  ����Ӧ�ü���ʱ����Ψһһ�� ISR ִ��ʱ�ͱ��Զ����٣��û����Բ����Լ����١�
 * \li  �������ֶ�ʱ��Ҳ�����⣬�����û�������ȫ���ն�ʱ�����������ڣ�����ȷ����ʱ��
 *      �Ƿ������ˣ��������п����ظ�������ͬ�Ķ�ʱ����
 * \li  timeout����Ϊ0��func_proc����Ϊ�գ����򷵻�ʧ�ܡ�
 *******************************************************************************/
int8 set_single_shot_app_timer_btmamager(uint32 attrb_tagh, uint16 timeout, timer_proc func_proc)
{
    timer_attrb_e attrb = APP_TIMER_ATTRB_CONTROL;
    timer_type_e type = TIMER_TYPE_SINGLE_SHOT;
    uint8 tag = APP_TIMER_TAG_SYS;

    app_timer_t *tm;
    int8 timer_id = -1;
    int8 i;

    //�������������Ŀ�����ʱ��
    for (i = 0; i < COM_APP_TIMER_MAX; i++)
    {
        if (g_com_app_timer_vct[i].state == TIMER_STATE_NOUSED)
        {
            tm = g_com_app_timer_vct + i;
            //��4bit��Ϊ0100��ʾCOM��ʱ��
            timer_id = 0x40 | i;

            //��ʼ����ʱ��
            tm->timeout = timeout;
            tm->timeout_off = 0;
            tm->timeout_expires = sys_get_ab_timer() + timeout;
            tm->func_proc = func_proc;
            tm->state = TIMER_STATE_RUNNING;
            tm->attrb = attrb;
            tm->type = type;
            tm->twinkle = FALSE;
            tm->on_off = FALSE;
            tm->tag = tag;
            tm->count = 0;

            break;
        }
    }

    if (timer_id == -1)
    {
        PRINT_ERR("set app timer fail!!");
        while (1)
        {
            ; //nothing
        }
    }

    return timer_id;
}

/*! \endcond */
