#include "app_fmengine.h"
#include <analog.h>

//MMM_PP_PLAY�Ƿ񷢳���
static bool s_mmm_pp_played = FALSE;

//ADDAģʽʱ����: ���м������PLAY
bool play(void)
{
    int result;

    if (s_mmm_pp_played == TRUE)
    {
        return TRUE;
    }

    //���µ���Ƶ�ʣ��߳���TTS����
    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL5, FREQ_LEVEL2);

    adjust_freq_add_value(0, g_dae_cfg_shm->run_freq);

    com_set_sound_out(SOUND_OUT_RESUME);

    mmm_pp_cmd(&g_mmm_pp_handle, MMM_PP_OPEN, (unsigned int) NULL);
    /*
    if(1 == g_app_info_state_all.fix_sample_rate_flag)
    {
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_FIX_SAMPLE_RATE, (unsigned int) NULL);
    }
    */
    mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_AINOUT_OPEN, (unsigned int) &g_ainout_param);

    //�����������ѡ��
    //#define OUTPUT_CHANNEL_NORMAL      0 //�������м�����ʼ��Ϊ����
    //#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
    //#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
    //#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
    //#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0
    mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_SET_OUTPUT_CHANNEL, g_output_channel_config);

#ifdef WAVES_ASET_TOOLS
    g_waves.bin_number = g_app_info_state_all.bin_number;
    result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_PLAY,(unsigned int) &g_waves);
#else
    //���Ͳ�������
    result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_PLAY, (unsigned int) NULL);
#endif

    mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_GET_ENERGY_INFO, (unsigned int) &(g_app_info_state_all.p_energy_value));
    g_app_info_state_all.energy_available_flag = TRUE;

    s_mmm_pp_played = TRUE;

    return result;
}

//ADDAģʽʱ����: ���м������CLOSE
bool stop(void)
{
    bool bret = TRUE;
    int result = 0;

    if (s_mmm_pp_played != TRUE)
    {
        return bret;
    }

    g_app_info_state_all.energy_available_flag = FALSE;
    
#ifdef WAVES_ASET_TOOLS
    result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP, (unsigned int) &g_waves);
#else
    //ֹͣ����
    result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP, (unsigned int) NULL);
#endif

    if (result != 0)
    {
        bret = FALSE;
    }
    mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_AINOUT_CLOSE, (unsigned int) NULL);
    s_mmm_pp_played = FALSE;

    //�ر��м���豸
    mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);

    //�ر���Ƶ���
    com_set_sound_out(SOUND_OUT_PAUSE);

    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL2, FREQ_NULL);
    adjust_freq_add_value(0, 0);

    return bret;
}
//��װplay�ӿ�
app_result_e fmengine_play(private_msg_t* cur_msg)
{
    play();

    cur_msg->reply->type = MSG_REPLY_SUCCESS;
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}
//��װSTOP�ӿ�
app_result_e fmengine_stop(private_msg_t* cur_msg)
{
    stop();

    cur_msg->reply->type = MSG_REPLY_SUCCESS;
    fm_eg_reply_msg(cur_msg);
    return RESULT_IGNORE;
}

