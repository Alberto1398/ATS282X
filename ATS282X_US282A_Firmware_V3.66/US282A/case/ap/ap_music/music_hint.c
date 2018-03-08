//          ��ʾ��Ϣ����
//
//1.ģʽ���ڡ�
//2.������ܹر���,�����������Ϣ�����µĶ���ģʽ���ڡ����ա�
//  ����V+�����رձ����ڣ�ͬʱ�������������ô��ڡ�(ģʽ����)��
//  �´��ڻᴦ��V+��(������1��)��
//
//  ������¡��������(����е����Ŀ�����Ļ�)�����رձ����ڣ�
//  �����Ŵ��ڡ���Ϊ�µĶ���ģʽ���ڣ��´��ڻᴦ��������
#include "music.h"

MUS_STATIC eg_err_e s_hint_err;

MUS_STATIC void _hint_show(uint8 error_no)
{
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 0)
    {
        return;
    }

    led_display(NUMBER1, NUM_E, TRUE);
    led_display(NUMBER2, NUM_R, TRUE);
    led_display(NUMBER3, NUM_0, TRUE);
    led_display(NUMBER4, NUM_0 + error_no, TRUE);
#endif
}

//��ͼ�����ɡ����¡����ٴ���
MUS_STATIC void _hint_view_handle(view_update_e mode)
{
    switch (mode)
    {
        case VIEW_UPDATE_CREATE:
        g_quit_from_hint = FALSE;

#if(SUPPORT_LED_DRIVER == 1)
        if (g_comval.support_led_display == 1)
        {
            led_clear_screen();
        }
#endif
        switch (s_hint_err)
        {
            case EG_ERR_DISK_OUT:
            _hint_show(3);
            break;
            default:
            _hint_show(1);
            break;
        }
        break;

        case VIEW_UPDATE_DESTROY:
        g_quit_from_hint = TRUE;
        break;

        case VIEW_UPDATE_REDRAW:
        case VIEW_UPDATE_STATUS:
        default:
        break;
    }
}

//err: �����
app_result_e create_view_hint(eg_err_e err)
{
    //����hint����
    create_view_param_t param;

    s_hint_err = err;

    param.type = VIEW_TYPE_MSG;
    param.unknown_key_deal = UNKNOWN_KEY_QUIT_REDEAL;
    //��Ϊ������ʾ�����Ծ;����˳�
    param.overtime = 100;
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 1)
    {
        param.overtime = 3000;
    }
#endif
    param.ke_maplist = NULL;
    param.se_maplist = NULL;
    param.view_cb = _hint_view_handle;
    com_view_add(VIEW_ID_HINT, &param);

    return RESULT_NULL;
}
