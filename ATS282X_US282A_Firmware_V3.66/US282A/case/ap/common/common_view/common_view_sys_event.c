/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ��ϵͳ�¼��ַ���ڣ�Ӧ���Զ���ϵͳ�¼����ȷַ���Ȼ����COMMONϵͳ
 *       �¼��ַ���Ҳ֧�ֽ�COMMONϵͳ�¼��ַ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"
#include "common_func.h"
#include "app_view.h"

extern const sys_event_map_t com_se_maplist[];

int com_view_sys_event(sys_event_t *p_se, const sys_event_map_t *app_se_maplist, sys_event_ret_t *p_ret, bool only_com)
{
    const sys_event_map_t *this_se_map;

    if ((app_se_maplist != NULL) && (only_com == FALSE))
    {
        this_se_map = app_se_maplist;
        for (;; this_se_map++)
        {
            if (MSG_NULL == this_se_map->se.msg_type)
            {
                break;
            }

            if (this_se_map->se.msg_type == p_se->msg_type)
            {
                p_ret->ret_cb = this_se_map->cb;
                p_ret->ret_option = this_se_map->se.msg_option;
                return 0;
            }
        }
    }

    this_se_map = com_se_maplist;
    for (;; this_se_map++)
    {
        if (MSG_NULL == this_se_map->se.msg_type)
        {
            break;
        }

        if (this_se_map->se.msg_type == p_se->msg_type)
        {
            p_ret->ret_cb = this_se_map->cb;
            p_ret->ret_option = this_se_map->se.msg_option;
            return 0;
        }
    }

    return -1;
}
