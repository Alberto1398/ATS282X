//          ��Ŀ�Ŵ���
//
//1.ģʽ���ڡ�
//2.������ܹر���,�����������Ϣ�����µĶ���ģʽ���ڡ����ա�
//  ����V+�����رա���Ŀ�š����ڣ�ͬʱ�������������ô��ڡ�(ģʽ����)��
//  �´��ڻᴦ��V+��(������1��)��
//
//  ������¡��������(����е����Ŀ�����Ļ�)�����رա���Ŀ�š����ڣ�
//  �����Ŵ��ڡ���Ϊ�µĶ���ģʽ���ڣ��´��ڻᴦ��������
#include "music.h"

static play_mode_e s_play_mode = PLAY_NO_PLAY;

MUS_STATIC app_result_e _song_key_prev(void);
MUS_STATIC app_result_e _song_key_next(void);
MUS_STATIC app_result_e _song_tts(void);

MUS_STATIC const key_event_map_t __section__(".rodata.ke_maplist")
music_song_number_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG_UP, KEY_DEAL_NULL }, _song_tts },
    { { KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD | KEY_TYPE_LONG_10S, KEY_DEAL_NULL }, _song_key_next },

    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG_UP, KEY_DEAL_NULL }, _song_tts },
    { { KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD |KEY_TYPE_LONG_10S, KEY_DEAL_NULL }, _song_key_prev },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, KEY_DEAL_NULL }, NULL },
};

MUS_STATIC app_result_e _song_tts(void)
{
    //������������Ӧ�Ķ̰�����
    com_filter_keymsg_in_queue(g_p_view_ke->val, KEY_TYPE_SHORT_UP);

    //��������(��Ҫ�����ֲ���֮ǰ������Ŀ��)
    return music_song_num_tts((void *)s_play_mode);
}

MUS_STATIC app_result_e _song_key_prev(void)
{
    g_switch_prev = 1;
    play_prev();
    music_clear_fileswitch();
    com_view_update(VIEW_ID_SONG_NUMBER);
    return RESULT_NULL;
}

MUS_STATIC app_result_e _song_key_next(void)
{
    g_switch_prev = 0;
    play_next();
    music_clear_fileswitch();
    com_view_update(VIEW_ID_SONG_NUMBER);
    return RESULT_NULL;
}

MUS_STATIC void _song_show(void)
{
#if(SUPPORT_LED_DRIVER == 1)
    uint32 file_no;
    uint8 n;

    if (g_comval.support_led_display == 0)
    {
        return;
    }

    file_no = g_file_path_info.file_path.dirlocation.file_num;
    for (n = 0; n < 3; n++)
    {
        led_display(NUMBER4 - n, NUM_0 + (file_no % 10), TRUE);
        file_no /= 10;

    }
    if (file_no > 9)
    {
        //����9999�׸�����ʾF
        led_display(NUMBER1, NUM_F, TRUE);
    }
    else
    {
        led_display(NUMBER1, NUM_0 + file_no, TRUE);
    }
#endif
}

//��ͼ�����ɡ����¡����ٴ���
MUS_STATIC void _song_view_handle(view_update_e mode)
{
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 0)
    {
        return;
    }
    switch (mode)
    {
        case VIEW_UPDATE_DESTROY:
        led_clear_screen();
        break;
        case VIEW_UPDATE_CREATE:
        led_clear_screen();
        case VIEW_UPDATE_REDRAW:
        case VIEW_UPDATE_STATUS:
        _song_show();
        break;
        default:
        break;
    }
#endif
}

app_result_e create_view_song_num(view_type_e view_type, bool tts, play_mode_e play_mode)
{
    //������Ŀ�Ŵ���
    create_view_param_t param;
    bool bExists;

    if( (g_view_infor[g_view_manager_infor.top].id == VIEW_ID_SONG_NUMBER)
        && (g_view_infor[g_view_manager_infor.top].type == view_type) )
    {
        //ǰ���Ѿ�������һ����view
        bExists = TRUE;
    }
    else
    {
        com_view_remove(VIEW_ID_SONG_NUMBER, FALSE);
        bExists = FALSE;
    }

    music_get_filepath(&g_file_path_info);
    //����ʾ
    if(!bExists)
    {
        for (;;)
        {
#if(SUPPORT_LED_DRIVER == 1)
            if (g_comval.support_led_display == 1)
            {
                if (view_type == VIEW_TYPE_MSG)
                {
                    param.overtime = 500;
                }
                else
                {
                    param.overtime = 2000;
                }
                break;
            }
#endif
            view_type = VIEW_TYPE_MSG;
            param.overtime = 100;
            break;
        }
        param.unknown_key_deal = UNKNOWN_KEY_QUIT_REDEAL;
        param.type = view_type;
        param.ke_maplist = music_song_number_key_map_list;
        param.se_maplist = NULL;
        param.view_cb = _song_view_handle;
        com_view_add(VIEW_ID_SONG_NUMBER, &param);
    }
    else
    {
        com_view_update(VIEW_ID_SONG_NUMBER);
    }
    //��
    if (tts == TRUE)
    {
        if (g_neednot_song_tts_play == FALSE)
        {
            //���������ֱ�ӱ���Ŀ�ţ��Ʊػ�����ջ�����
            //���ԣ����÷�����Ϣ�ķ�ʽ
            msg_apps_t msg;

            msg.type = MSG_MUSIC_SONG_NUM_TTS;
            msg.content.data[0] = play_mode;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
        else
        {
            music_switch_song(TTS_INVALID, play_mode);
        }
        g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;
    }
    s_play_mode = play_mode;

    return RESULT_NULL;
}
