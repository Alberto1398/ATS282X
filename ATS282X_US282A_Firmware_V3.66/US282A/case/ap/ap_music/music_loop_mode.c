//          ѭ��ģʽ��ͼ
//
//1.ģʽ���ڡ�
//2.������ܹر���,�����������Ϣ�����µĶ���ģʽ���ڡ����ա�
//  ����V+�����رձ����ڣ�ͬʱ�������������ô��ڡ�(ģʽ����)��
//  �´��ڻᴦ��V+��(������1��)��
//
//  ������¡��������(����е����Ŀ�����Ļ�)�����رձ����ڣ�
//  �����Ŵ��ڡ���Ϊ�µĶ���ģʽ���ڣ��´��ڻᴦ��������
#include "music.h"

MUS_STATIC app_result_e _loop_key_next(void);
MUS_STATIC app_result_e _loop_key_prev(void);

MUS_STATIC const key_event_map_t __section__(".rodata.ke_maplist")
music_loop_mode_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_DOWN, KEY_DEAL_FILTER }, _loop_key_next },
    { { KEY_PREV, 0, KEY_TYPE_DOWN, KEY_DEAL_FILTER }, _loop_key_prev },
    { { KEY_REPEAT, 0, KEY_TYPE_DOWN, KEY_DEAL_FILTER }, _loop_key_next },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, KEY_DEAL_NULL }, NULL },
};

MUS_STATIC app_result_e _loop_key_next(void)
{
    music_get_loop_mode(&g_music_config.repeat_mode);

    //�л�ѭ��ģʽ
    g_music_config.repeat_mode++;
    //�л�����󷵻ص���һ��
    if (g_music_config.repeat_mode > FSEL_MODE_RANOM)
    {
        g_music_config.repeat_mode = MIN_LOOP_MODE;
    }

    music_set_loop_mode(g_music_config.repeat_mode);
    com_view_update(VIEW_ID_LOOP_MODE);

    return RESULT_NULL;
}

MUS_STATIC app_result_e _loop_key_prev(void)
{
    music_get_loop_mode(&g_music_config.repeat_mode);

    if (g_music_config.repeat_mode > MIN_LOOP_MODE)
    {
        //�л�ѭ��ģʽ
        g_music_config.repeat_mode--;
    }
    else
    {
        //�л�����һ�����ص����
        g_music_config.repeat_mode = FSEL_MODE_RANOM;
    }

    music_set_loop_mode(g_music_config.repeat_mode);
    com_view_update(VIEW_ID_LOOP_MODE);

    return RESULT_NULL;
}

MUS_STATIC void _loop_show(void)
{
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 0)
    {
        return;
    }

    led_display(NUMBER2, NUM_L, TRUE);
    led_display(NUMBER3, NUM_P, TRUE);
    led_display(NUMBER4, NUM_0 + g_music_config.repeat_mode + 1, TRUE);
#endif
}

//��ͼ�����ɡ����¡����ٴ���
MUS_STATIC void _loop_view_handle(view_update_e mode)
{
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 1)
    {
        led_clear_screen();
    }
#endif

    switch (mode)
    {
        case VIEW_UPDATE_CREATE:
        case VIEW_UPDATE_REDRAW:
        case VIEW_UPDATE_STATUS:
        _loop_show();
        break;
        case VIEW_UPDATE_DESTROY:
        break;
        default:
        break;
    }
}

app_result_e create_view_loop_mode(void)
{
    create_view_param_t param;

    param.type = VIEW_TYPE_SUB;
    param.unknown_key_deal = UNKNOWN_KEY_QUIT_REDEAL;
    param.overtime = 100;
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 1)
    {
        param.overtime = 3000;
    }
#endif
    param.ke_maplist = music_loop_mode_key_map_list;
    param.se_maplist = NULL;
    param.view_cb = _loop_view_handle;
    com_view_add(VIEW_ID_LOOP_MODE, &param);

    return RESULT_NULL;
}
