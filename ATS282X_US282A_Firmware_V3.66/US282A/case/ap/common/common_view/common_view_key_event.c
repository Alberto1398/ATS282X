/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ�ð����¼��ַ���ڣ�Ӧ���Զ��尴���¼����ȷַ���Ȼ����COMMON��ݼ�
 *       �¼��ַ���Ҳ֧�ֽ�COMMON��ݼ��¼��ַ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"
#include "common_func.h"
#include "app_view.h"

extern const key_event_map_t com_ke_maplist[];
// OTA �汾����Ҫ common �еİ���
#ifdef OTA_VERSION
extern const key_event_map_t com_ke_maplist_null[];
#endif

static bool is_key_val_match(key_value_e tb_val, key_value_e in_val)
{
    if (tb_val == in_val)
    {
        return TRUE;
    }

    if ((in_val >= KEY_NUM0) && (in_val <= KEY_NUM9) && (tb_val == KEY_NUMBER))
    {
        return TRUE;
    }

    return FALSE;
}

int com_view_key_event(key_event_t *p_ke, const key_event_map_t *app_ke_maplist, key_event_cb *ret_cb, bool only_com)
{
    const key_event_map_t *this_ke_map;

    if ((g_view_infor[g_view_manager_infor.top].type == VIEW_TYPE_SUB)
            && (g_view_infor[g_view_manager_infor.top].timer_id != -1))
    {
        restart_app_timer(g_view_infor[g_view_manager_infor.top].timer_id);
    }

    if ((app_ke_maplist != NULL) && (only_com == FALSE))
    {
        this_ke_map = app_ke_maplist;
        for (;; this_ke_map++)
        {
            /* �ﵽ�û����һ��ӳ�䣬��������ӳ�� */
            if (KEY_NULL == this_ke_map->ke.val)
            {
                break;
            }

            /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
            if ((is_key_val_match(this_ke_map->ke.val, p_ke->val) == TRUE)
                    && ((this_ke_map->ke.type & p_ke->type) != 0))
            {
                *ret_cb = this_ke_map->cb;
                if (this_ke_map->ke.deal == KEY_DEAL_FILTER)
                {
                    com_filter_key_hold();
                }
                else if (this_ke_map->ke.deal == KEY_DEAL_FILTER_UP)
                {
                    com_filter_key_up();
                }
                else if (this_ke_map->ke.deal == KEY_DEAL_ITSELF)
                {
                    com_filter_key_itself();
                }
                else
                {
                    ; //nothing for QAC
                }
                return 0;
            }
        }
    }

// OTA �汾����Ҫ common �еİ���
#ifdef OTA_VERSION
	this_ke_map = com_ke_maplist_null;
#else
    this_ke_map = com_ke_maplist;
#endif
    for (;; this_ke_map++)
    {
        /* �ﵽ�û����һ��ӳ�䣬��������ӳ�� */
        if (KEY_NULL == this_ke_map->ke.val)
        {
            break;
        }

        /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
        if ((is_key_val_match(this_ke_map->ke.val, p_ke->val) == TRUE)
                && ((this_ke_map->ke.type & p_ke->type) != 0))
        {
            *ret_cb = this_ke_map->cb;
            if (this_ke_map->ke.deal == KEY_DEAL_FILTER)
            {
                com_filter_key_hold();
            }
            else if (this_ke_map->ke.deal == KEY_DEAL_FILTER_UP)
            {
                com_filter_key_up();
            }
            else if (this_ke_map->ke.deal == KEY_DEAL_ITSELF)
            {
                com_filter_key_itself();
            }
            else
            {
                ; //nothing for QAC
            }
            return 0;
        }
    }

    return -1;
}

void com_view_key_check_dck(const key_event_map_t *app_ke_maplist)
{
    const key_event_map_t *this_ke_map;

    g_key_infor.dck_map = 0;
    if (app_ke_maplist != NULL)
    {
        this_ke_map = app_ke_maplist;
        for (;; this_ke_map++)
        {
            /* �ﵽ�û����һ��ӳ�䣬��������ӳ�� */
            if (KEY_NULL == this_ke_map->ke.val)
            {
                break;
            }

            /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
            if ((this_ke_map->ke.type & KEY_TYPE_DBL_CLICK) != 0)
            {
                g_key_infor.dck_map |= (uint32) (1 << this_ke_map->ke.val);
            }
        }
    }

// OTA �汾����Ҫ common �еİ���
#ifdef OTA_VERSION
	this_ke_map = com_ke_maplist_null;
#else
    this_ke_map = com_ke_maplist;
#endif
    for (;; this_ke_map++)
    {
        /* �ﵽ�û����һ��ӳ�䣬��������ӳ�� */
        if (KEY_NULL == this_ke_map->ke.val)
        {
            break;
        }

        /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
        if ((this_ke_map->ke.type & KEY_TYPE_DBL_CLICK) != 0)
        {
            g_key_infor.dck_map |= (uint32) (1 << this_ke_map->ke.val);
        }
    }
}
