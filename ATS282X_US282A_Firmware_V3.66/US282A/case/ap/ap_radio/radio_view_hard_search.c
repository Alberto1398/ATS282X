//��̨��ͼ
#include "app_radio.h"

app_result_e __section__(".bank") search_key_deal_cancel_search(void)
{
    return RESULT_STOP_SEARCH;
}

const key_event_map_t __section__(".rodata.ke_maplist") radio_search_ke_maplist[] =
{
    /*! ȡ����̨ */
    { { KEY_NEXT, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_PREV, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_PLAY, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_PHONEPLAY, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_VADD, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },
    { { KEY_VSUB, 0, KEY_TYPE_ALL, KEY_DEAL_FILTER }, search_key_deal_cancel_search },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, KEY_DEAL_NULL }, NULL },
};

static void show_freq(uint32 freq)
{
#if(SUPPORT_LED_DRIVER == 1)
    uint8 dis_num;

    if (g_comval.support_led_display == 0)
    {
        return;
    }

    led_clear_screen();

    //��λ��mhz
    freq = freq / 100;

    led_display(LCD_FM, 0xff, 1);

    //���λ
    dis_num = (uint8) (freq / 1000);
    if (dis_num != 0)
    {
        led_display(NUMBER1, (uint32 )dis_num, 1);
    }
    else
    {
        led_display(NUMBER1, 0, 0);
    }

    freq = freq % 1000;
    //�ڶ�λ
    dis_num = (uint8) (freq / 100);

    led_display(NUMBER2, (uint32 )dis_num, 1);

    //����λ
    freq = freq % 100;
    dis_num = (uint8) (freq / 10);

    led_display(NUMBER3, (uint32 )dis_num, 1);

    //��
    led_display(LCD_NO1, 0xff, 1);

    //����λ
    dis_num = (uint8) (freq % 10);
    led_display(NUMBER4, (uint32 )dis_num, 1);
#endif
}

static void __radio_search_view_redraw(void)
{
    show_freq(g_search_current_freq);
}

void radio_hard_search_main_view(view_update_e mode)
{
    switch (mode)
    {
        case VIEW_UPDATE_CREATE:
        {
            __radio_search_view_redraw();
        }
        break;

        case VIEW_UPDATE_DESTROY:
        {

        }
        break;

        case VIEW_UPDATE_REDRAW:
        {
            __radio_search_view_redraw();
        }
        break;

        case VIEW_UPDATE_STATUS:
        {
            __radio_search_view_redraw();
        }
        break;

        default:
        break;
    }
}

//��������ͼ--��̨��ͼ
void __section__(".bank") radio_hard_search_create_main_view(void)
{
    create_view_param_t create_view_param;

    create_view_param.type = VIEW_TYPE_MAIN;
    create_view_param.unknown_key_deal = UNKNOWN_KEY_IGNORE;
    create_view_param.overtime = 0;
    create_view_param.ke_maplist = radio_search_ke_maplist;
    create_view_param.se_maplist = NULL;
    create_view_param.view_cb = radio_hard_search_main_view;
    com_view_add(RADIO_MAIN_VIEW_HARD_SEARCH, &create_view_param);
}
