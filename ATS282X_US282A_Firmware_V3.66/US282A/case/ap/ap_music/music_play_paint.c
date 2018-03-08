//      ���Ŵ��ڵ�paint
//
//  ����Ĵ����music_play.c����ͬһ��bank, ��������ʱ�Ͳ�����bank(�ڲ��������������쳣ʱ).
//      ��Ϊʲô����������һ���ļ��أ���Ϊ�кܶ෽����û��LED�ģ�
//      ��������£�ֻҪ������Ϳ���ʡ�ܶ����ռ䡣
//
#include "music.h"


//��ʾ����ʱ��, ����״̬(����/��ͣ), ���ŵ����ĸ���(U/��)
void play_view_handle(view_update_e mode)
{
#if(SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 0)
    {
        return;
    }

    switch (mode)
    {
        case VIEW_UPDATE_CREATE:
        led_clear_screen();
        //��ʾ4��0
        led_display(NUMBER1, NUM_0, TRUE);
        led_display(NUMBER2, NUM_0, TRUE);
        led_display(NUMBER3, NUM_0, TRUE);
        led_display(NUMBER4, NUM_0, TRUE);
        break;

        case VIEW_UPDATE_DESTROY:
        led_clear_screen();
        break;

        case VIEW_UPDATE_REDRAW:
        if (g_quit_from_hint == TRUE)
        {
            break;
        }
        g_play_refresh_flag = PLAY_REFRESH_ALL;
        //������case��ͬ�Ĵ���
        case VIEW_UPDATE_STATUS:
        //ˢ�²���ʱ�䣬����״̬, �̷�
        if ((g_play_refresh_flag & PLAY_REFRESH_TIME) != 0)
        {
            uint32 cur_time; //��λ��
            uint8 minute; //����

            //����ת��Ϊ��
            cur_time = g_music_info.eg_playinfo.cur_time / 1000;
            //ȥ��Сʱ
            cur_time %= 3600;

            minute = (uint8)(cur_time / 60);

            //���ӵ�ʮλ
            led_display(NUMBER1, NUM_0 + minute / 10, TRUE);
            //���ӵĸ�λ
            led_display(NUMBER2, NUM_0 + (minute % 10), TRUE);

            //:
            led_display(LCD_COL, 0xff, TRUE);

            //��
            cur_time %= 60;
            //���ʮλ
            led_display(NUMBER3, NUM_0 + cur_time / 10, TRUE);
            //��ĸ�λ
            led_display(NUMBER4, NUM_0 + (cur_time % 10), TRUE);
        }

        if ((g_play_refresh_flag & PLAY_REFRESH_STATUS) != 0)
        {
            //��ʾ����״̬
            if ((g_music_info.eg_status.play_status == StopSta) || (g_music_info.eg_status.play_status == PauseSta))
            {
                led_display(LCD_PAUSE, 0xff, TRUE);
                led_display(LCD_PLAY, 0xff, FALSE);
            }
            else
            {
                led_display(LCD_PLAY, 0xff, TRUE);
                led_display(LCD_PAUSE, 0xff, FALSE);
            }
        }

        if ((g_play_refresh_flag & PLAY_REFRESH_DISK) != 0)
        {
            //��ʾ�̷�
            music_get_filepath(&g_file_path_info); //��ȡ�ļ�·��
            if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
            {
                led_display(LCD_SD, 0xff, TRUE);
                led_display(LCD_USB, 0xff, FALSE);
            }
            else
            {
                led_display(LCD_USB, 0xff, TRUE);
                led_display(LCD_SD, 0xff, FALSE);
            }
        }
        g_play_refresh_flag = 0;
        break;

        default:
        break;
    }
#endif
}
