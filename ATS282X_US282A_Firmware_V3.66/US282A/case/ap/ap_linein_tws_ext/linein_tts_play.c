/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������line in tws�Ƹ�ǰ̨Ӧ��TTS��ؽӿ�ʵ�֡�
 * ���ߣ�
 ********************************************************************************/

#include  "ap_linein.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO
//���ӵ����ɸ��������øøú������𲥱�tts
void linein_tws_tts_play(void)
{
    //tws��绰��ʱ�������ϲ�������Գɹ�
    if((g_bt_stack_cur_info.pair==1)&&(flag2==1))
    {
        //g_app_info_state_all.g_send_exit_mmm_tts = 1;
        //������Գɹ�
        com_tts_state_play(TTS_MODE_ONLYONE, (void *) TTS_PAIR_SUCCEED);
        flag2=0;

    }
    else if(g_bt_stack_cur_info.tws_dev_discon == 1)
    {
        //�������������Ͽ�&�ȴ�����
        com_tts_state_play(TTS_MODE_ONLYONE, (void *) TTS_BT_DISCON);
        com_tts_state_play(TTS_MODE_ONLYONE, (void *) TTS_WAIT_BT_CON);
        flag_con_ok_one=0;
        PRINT_INFO("dis2 ");
    }
    else
    {
        //do nothing
    }
}

#endif

