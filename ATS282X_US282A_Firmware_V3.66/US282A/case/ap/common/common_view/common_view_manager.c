/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ǰ̨Ӧ����ͼ��������������ʼ�����˳���������ͼ��������ͼ�����µ�ǰ��ͼ�Ƚӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"
#include "common_func.h"
#include "app_view.h"

//��ʼ����ͼ������
int com_view_manager_init(void)
{
    uint8 i;

    libc_memset(g_view_infor, 0x00, sizeof(g_view_infor));
    for (i = 0; i < MVC_VIEW_DEEPTH; i++)
    {
        g_view_infor[i].timer_id = -1;
    }

    g_view_manager_infor.top = -1;
    g_view_manager_infor.p_key_view_infor = NULL;

    return 0;
}

void com_view_remove_top_view(void)
{
    if (g_view_manager_infor.top >= MVC_VIEW_DEEPTH)
    {
        return;
    }

    com_view_remove(g_view_infor[g_view_manager_infor.top].id, TRUE);
}

void com_view_remove_type_view(view_type_e type)
{
    view_infor_t *p_tmp_view_infor;

    while (g_view_manager_infor.top < MVC_VIEW_DEEPTH)
    {
        p_tmp_view_infor = &g_view_infor[g_view_manager_infor.top];

        if (p_tmp_view_infor->type >= type)
        {
            com_view_remove(p_tmp_view_infor->id, FALSE);
        }
        else
        {
            break;
        }
    }
}

int com_view_manager_exit(void)
{
    com_view_remove_type_view(VIEW_TYPE_MAIN);

    //������ڲ��Ű�������Ҫ�ȴ��������������
    keytone_play_deal_wait();

    //������ڲ���TTS��Ҫ�ȴ�TTS�������
    com_tts_state_play_wait();

    return 0;
}

void com_view_update_key_view(void)
{
    view_infor_t *p_tmp_view_infor;
    uint8 i;

    if (g_view_manager_infor.top < MVC_VIEW_DEEPTH)
    {
        for (i = 0; i <= g_view_manager_infor.top; i++)
        {
            p_tmp_view_infor = &g_view_infor[g_view_manager_infor.top - i];

            if (p_tmp_view_infor->type != VIEW_TYPE_MSG)
            {
                g_view_manager_infor.p_key_view_infor = p_tmp_view_infor;
                return;
            }
        }
    }

    g_view_manager_infor.p_key_view_infor = NULL;

    return;
}

//װ����ͼ����ʱ��֧��Ƕ�ף���MAINװ��ǰ��Ҫ����գ�SUBװ��ǰ��Ҫ�����MSG��SUB��MSGװ��ǰ��Ҫ�����MSG
int com_view_add(uint8 view_id, create_view_param_t *p_create_view_param)
{
    view_infor_t *p_tmp_view_infor;

    if ((p_create_view_param->type == VIEW_TYPE_MSG) && (p_create_view_param->overtime == 0))
    {
        return -1;
    }

    com_view_remove_type_view(p_create_view_param->type);

    g_view_manager_infor.top++;
    
    if(g_view_manager_infor.top >= MVC_VIEW_DEEPTH)
    {
        return -1;
    }

    p_tmp_view_infor = &g_view_infor[g_view_manager_infor.top];
    p_tmp_view_infor->id = view_id;
    p_tmp_view_infor->type = p_create_view_param->type;
    p_tmp_view_infor->unknown_key_deal = p_create_view_param->unknown_key_deal;
    p_tmp_view_infor->ke_maplist = p_create_view_param->ke_maplist;
    p_tmp_view_infor->se_maplist = p_create_view_param->se_maplist;
    p_tmp_view_infor->view_cb = p_create_view_param->view_cb;

    com_view_update_key_view();
    if (g_view_manager_infor.p_key_view_infor->ke_maplist != NULL)
    {
        com_view_key_check_dck(g_view_manager_infor.p_key_view_infor->ke_maplist);
    }

    change_app_timer_tag(p_tmp_view_infor->id);

    (p_tmp_view_infor->view_cb)(VIEW_UPDATE_CREATE);

    if ((p_create_view_param->type == VIEW_TYPE_MAIN)
            || ((p_create_view_param->type == VIEW_TYPE_SUB) && (p_create_view_param->overtime == 0)))
    {
        return 0;
    }

    p_tmp_view_infor->timer_id = set_app_timer(APP_TIMER_ATTRB_COM_UI | ((uint32) p_tmp_view_infor->id << 8),
            p_create_view_param->overtime,
            com_view_remove_top_view);

    return 0;
}

//������ͼ
int com_view_remove(uint8 view_id, bool redraw)
{
    view_infor_t *p_tmp_view_infor;
    uint8 i, tmp_top, uninstall_count;
    bool find_flag = FALSE;

    if (g_view_manager_infor.top >= MVC_VIEW_DEEPTH)
    {
        return -1;
    }

    for (i = 0; i <= g_view_manager_infor.top; i++)
    {
        tmp_top = g_view_manager_infor.top - i;
        p_tmp_view_infor = &g_view_infor[tmp_top];

        if (p_tmp_view_infor->id == view_id)
        {
            find_flag = TRUE;
            break;
        }
    }

    if (find_flag == FALSE)
    {
        return -1;
    }

    uninstall_count = g_view_manager_infor.top - tmp_top + 1;
    for (i = 0; i < uninstall_count; i++)
    {
        if (g_view_manager_infor.top >= MVC_VIEW_DEEPTH)
        {
            g_view_manager_infor.top = 0;
            PRINT_INFO("MVC VIEW DEEPTH ERROR");
            return -1;
        }
        p_tmp_view_infor = &g_view_infor[g_view_manager_infor.top];

        (p_tmp_view_infor->view_cb)(VIEW_UPDATE_DESTROY);

        if (p_tmp_view_infor->timer_id != -1)
        {
            kill_app_timer(p_tmp_view_infor->timer_id);
        }

        libc_memset(p_tmp_view_infor, 0x00, sizeof(view_infor_t));
        p_tmp_view_infor->timer_id = -1;
        if (g_view_manager_infor.top > 0)
        {
            g_view_manager_infor.top--;
        }
        else
        {
            g_view_manager_infor.top = -1;
        }
        com_view_update_key_view();
        if ((g_view_manager_infor.p_key_view_infor != NULL)
                && (g_view_manager_infor.p_key_view_infor->ke_maplist != NULL))
        {
            com_view_key_check_dck(g_view_manager_infor.p_key_view_infor->ke_maplist);
        }
    }

    if (redraw == TRUE)
    {
        if (g_view_manager_infor.top < MVC_VIEW_DEEPTH)
        {
            p_tmp_view_infor = &g_view_infor[g_view_manager_infor.top];

            change_app_timer_tag(p_tmp_view_infor->id);
            (p_tmp_view_infor->view_cb)(VIEW_UPDATE_REDRAW);
            if (p_tmp_view_infor->timer_id != -1)
            {
                restart_app_timer(p_tmp_view_infor->timer_id);
            }
        }
    }

    return 0;
}

//ֻ�е�ǰ��ͼ���²���Ч
int __section__(".rcode") com_view_update(uint8 view_id)
{
    view_infor_t *p_tmp_view_infor;

    if (g_view_manager_infor.top >= MVC_VIEW_DEEPTH)
    {
        return -1;
    }

    p_tmp_view_infor = &g_view_infor[g_view_manager_infor.top];
    if ((p_tmp_view_infor->id != view_id) && (p_tmp_view_infor->type == VIEW_TYPE_MSG))
    {
        return -1;
    }

    (p_tmp_view_infor->view_cb)(VIEW_UPDATE_STATUS);

    return 0;
}
