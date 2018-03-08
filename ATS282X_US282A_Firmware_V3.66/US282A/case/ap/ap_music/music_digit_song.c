//          ����ѡ�贰��
//
//1.ģʽ���ڡ�
//2.��������ּ��ܹر���,�����������Ϣ�����µĶ���ģʽ���ڡ����ա�
//  ����V+�����رձ����ڣ�ͬʱ�������������ô��ڡ�(ģʽ����)��
//  �´��ڻᴦ��V+��(������1��)��
//
//  ������¡��������(����е����Ŀ�����Ļ�)�����رձ����ڣ�
//  �����Ŵ��ڡ���Ϊ�µĶ���ģʽ���ڣ��´��ڻᴦ��������

#include "music.h"

//��༸������
#define DIGIT_COUNT     (MAX_DIGITAL_COUNT)

MUS_STATIC app_result_e _dig_key_number(void);

MUS_STATIC const key_event_map_t __section__(".rodata.ke_maplist")
music_digit_song_key_map_list[] =
{
    { { KEY_NUMBER, 0, KEY_TYPE_DOWN, KEY_DEAL_FILTER }, _dig_key_number },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, KEY_DEAL_NULL }, NULL },
};

//�洢���������(��λ�ڵ��±�)
MUS_STATIC uint8 s_digit_buf[DIGIT_COUNT];
//��ǰ���ֵĸ���
MUS_STATIC uint8 s_digit_count;

MUS_STATIC void _dig_show(void)
{
#if(SUPPORT_LED_DRIVER == 1)
    uint8 n;

    if (g_comval.support_led_display == 0)
    {
        return;
    }

    for (n = 0; n < s_digit_count; n++)
    {
        led_display(NUMBER1 + (4 - s_digit_count),
                NUM_0 + s_digit_buf[n], TRUE);
    }
#endif
}

MUS_STATIC app_result_e _dig_key_number(void)
{
    s_digit_buf[s_digit_count] = (uint8)(g_p_view_ke->val - KEY_NUM0);
    s_digit_count++;
    com_view_update(VIEW_ID_DIGIT_SONG);
    if (s_digit_count == DIGIT_COUNT)
    {
        com_view_remove(VIEW_ID_DIGIT_SONG, TRUE);
    }

    return RESULT_NULL;
}

MUS_STATIC void _dig_select_song(void)
{
    uint16 song_num = 0;
    uint8 n;
    play_mode_e play_mode;
    if((g_music_info.eg_status.play_status != StopSta)
        && (g_music_info.eg_status.play_status != PauseSta))
    {
        play_mode = PLAY_NORMAL;
    }
    else
    {
        play_mode = PLAY_NO_PLAY;
    }

    for (n = 0; n < s_digit_count; n++)
    {
        song_num *= 10;
        song_num += s_digit_buf[n];
    }
    music_get_filepath(&g_file_path_info);
    if (song_num != g_file_path_info.file_path.dirlocation.file_num)
    {
        music_digital_song(song_num);
        music_switch_song(TTS_SONG_SEQ, play_mode);
    }
}

//��ͼ�����ɡ����¡����ٴ���
MUS_STATIC void _dig_view_handle(view_update_e mode)
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
        break;

        case VIEW_UPDATE_DESTROY:
        //�˳�����ͼ��������ԭ��
        //1.��������˳�
        //2.��ʱ�˳�
        //3.������4������(������)�����˳�
        //4.ǿ���˳�
        //  4.1����ͬ����ͼ�ļ���(���磺���γ������µ���hint���ڡ�
        //      ������hint����ʱ���������л��ƿ��ܻᵼ�±����ڱ��ر�)
        //  4.2���磬apk�ϰ�PLAY����apk�ı����ǲ����������ϵ�PLAY,
        //      ���ԣ�������Ҫ�˳���
        //��1,4�֣�ֱ���˳�����ѡ��
        //��2,3�֣�Ҫѡ��
        //ע��:ǿ���˳�(���磺���γ�����POWER���˳�ʱ��������õ�����)

        _dig_select_song();
        break;

        case VIEW_UPDATE_REDRAW:
        case VIEW_UPDATE_STATUS:
        _dig_show();
        break;
        default:
        break;
    }
}

app_result_e create_view_digit_song(key_event_t *p_ke)
{
    create_view_param_t param;

    s_digit_count = 0;
    //���ﲻ�������ļ��(�ͻ����Ѿ���֤��)
    s_digit_buf[s_digit_count] = (uint8) (p_ke->val - KEY_NUM0);
    s_digit_count++;

    param.type = VIEW_TYPE_SUB;
    param.unknown_key_deal = UNKNOWN_KEY_QUIT_REDEAL;
    param.overtime = 3000;
    param.ke_maplist = music_digit_song_key_map_list;
    param.se_maplist = NULL;
    param.view_cb = _dig_view_handle;
    com_view_add(VIEW_ID_DIGIT_SONG, &param);

    return RESULT_NULL;
}
