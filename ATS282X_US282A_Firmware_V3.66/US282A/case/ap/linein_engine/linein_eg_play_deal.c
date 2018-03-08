#include "app_linein_eg.h"

static void get_noise_reduction_para(noise_reduction_t* p_noise_reduction)
{
    noise_reduction_config_t noise_reduction_temp;
    
    com_get_config_struct((uint16) NOISE_AUX_REDUCTION, (uint8 *) &noise_reduction_temp, sizeof(noise_reduction_config_t));
    libc_memcpy(p_noise_reduction, &noise_reduction_temp.noise_reduction, sizeof(noise_reduction_t));    
}

//����ͨ��ʱ�Ĳ���
bool play(void)
{
    int result;

    bool bret = TRUE;
    noise_reduction_t noise_reduction;
       
    //��TTS�����̣߳�������������Ƶ��
    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL5, FREQ_LEVEL2);

    adjust_freq_add_value(0, g_dae_cfg_shm->run_freq);
    
    //ֹͣ or ��ͣʱ ���Ͳ�������
    if (g_eg_status_p->play_status == linein_play_status_stop)
    {
        //��ʼ���м����
        mmm_pp_cmd(&g_mmm_pp_handle, MMM_PP_OPEN, (unsigned int) NULL);

        /*
        if(1 == g_app_info_state_all.fix_sample_rate_flag)
        {
            mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_FIX_SAMPLE_RATE, (unsigned int) NULL);
        }
        */
        //set sound open
        com_set_sound_out(SOUND_OUT_RESUME);

        g_eg_status_p->play_status = linein_play_status_play; //���ò���״̬

        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_AINOUT_OPEN, (unsigned int) &g_ainout_param);

	    //�����������ѡ��
	    //#define OUTPUT_CHANNEL_NORMAL      0 //�������м�����ʼ��Ϊ����
	    //#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
	    //#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
	    //#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
        //#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0
        
	    mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_SET_OUTPUT_CHANNEL, g_output_channel_config);
		
#ifdef WAVES_ASET_TOOLS
        //���Ͳ�������
        g_waves.bin_number = g_app_info_state_all.bin_number;
        result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_PLAY, (unsigned int) &g_waves);
#else
        get_noise_reduction_para(&noise_reduction);
        result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_SET_NOISE_REDUCTION_PARA, &noise_reduction);
        //���Ͳ�������
        result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_PLAY, (unsigned int) NULL);     
#endif

        if (result != 0)
        {
            bret = FALSE;
            _error_handle();
        }
        else
        {
            mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_GET_ENERGY_INFO, (unsigned int) &(g_app_info_state_all.p_energy_value));
            g_app_info_state_all.energy_available_flag = TRUE;

            bret = TRUE;
        }
    }

    return bret;
}

//����ͨ��ʱ��ֹͣ
bool stop(void)
{
    bool bret = TRUE;
    int result = 0;
    
    //mmm_pp_status_t play_status;

    g_app_info_state_all.energy_available_flag = FALSE;
    
    /*mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_GET_STATUS, (unsigned int) &play_status); //��ȡ����ʱû��

    if(play_status.status == MMM_PP_ENGINE_STOPPED)
    {
    	 //return bret;
    	 g_eg_status_p->play_status = linein_play_status_stop;
    }*/

    //�������ŲŻ�ֹͣ
    if (g_eg_status_p->play_status == linein_play_status_play)
    {
#ifdef WAVES_ASET_TOOLS
        //ֹͣ����
        result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP, (unsigned int) &g_waves);     
#else
        //ֹͣ����
        result = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP, (unsigned int) NULL);
#endif
        if (result != 0)
        {
            bret = FALSE;
        }
        else
        {           
            bret = TRUE;
        }

        /*if (bret == FALSE)
        {
        }*/ 
        //����״̬
        g_eg_status_p->play_status = linein_play_status_stop;
        //�ر������豸
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_AINOUT_CLOSE, (unsigned int) NULL);
        //�ر��м���豸
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);

        //�ر���Ƶ���
        com_set_sound_out(SOUND_OUT_PAUSE);
    }

    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL2, FREQ_NULL);
    adjust_freq_add_value(0, 0);

    return bret;
}
