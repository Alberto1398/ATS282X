/*******************************************************************************
 *                              US211A
 *                            Module: music engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     fiona.yang     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
#include "app_uengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \int32 uegine_player_open(void)
 * \���м��
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      uengine_control.c
 * \note
 */
/*******************************************************************************/
int32 uegine_player_open(void)
{
    int open_ret = 0;
    if(0 == g_uspeaker_24bit)
    {
        //bool init_fsel_ret;
        //char *mmm_name =
        //{ "mmm_pp.al" };

        //�����м������
        //sys_load_mmm(mmm_name, TRUE);//������

        if (g_mmm_pp_handle != NULL)
        {
            return 0;
        }

        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL10, FREQ_LEVEL10);

        adjust_freq_add_value(0, g_dae_cfg_shm->run_freq);

        //��ʼ���м����
        open_ret = mmm_pp_cmd(&g_mmm_pp_handle, MMM_PP_OPEN, (unsigned int) NULL);

        if(1 == g_app_info_state_all.fix_sample_rate_flag)
        {
            mmm_pp_cmd(&g_mmm_pp_handle, MMM_PP_FIX_SAMPLE_RATE, (unsigned int) NULL);
        }

        if (open_ret != -1)
        {
            open_ret = mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_AINOUT_OPEN, (unsigned int) &g_ain_out_setting);
        }

        //�����������ѡ��
        //#define OUTPUT_CHANNEL_NORMAL      0 //�������м�����ʼ��Ϊ����
        //#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
        //#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
        //#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
        //#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_SET_OUTPUT_CHANNEL, (unsigned int) g_output_channel_config);

#ifdef WAVES_ASET_TOOLS
        g_waves.bin_number = g_app_info_state_all.bin_number;
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_PLAY, (unsigned int) &g_waves);
#else
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_PLAY, (unsigned int) NULL);
#endif

        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_GET_ENERGY_INFO, (unsigned int) &(g_app_info_state_all.p_energy_value));

        g_app_info_state_all.energy_available_flag = TRUE;

        //start get data timer
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_START_SEND, (unsigned int) NULL);

        //ud_set_cmd(SET_ADJUST_TIMER, 1);
    }
    else
    {
        ud_set_cmd(SET_USPEAKER_24BIT,USPEAKER24_SAMPLE_RATE);
        ud_set_cmd(SET_USPEAKER_24BIT,USPEAKER24_PLAY);
    }
    return open_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void uegine_player_close(void)
 * \�ر��м��
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      uengine_control.c
 * \note
 */
/*******************************************************************************/

void uegine_player_close(void)
{
    if(0 == g_uspeaker_24bit)
    {
        uint32 fade_out_use = 0;

        //ud_set_cmd(SET_ADJUST_TIMER, 0);

        if (g_mmm_pp_handle == NULL)
        {
            return;
        }

        g_app_info_state_all.energy_available_flag = FALSE;

        if (g_status_data.start_play == 0)
        {
            mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_FADE_OUT_TIME, (unsigned int) fade_out_use);
        }
#ifdef WAVES_ASET_TOOLS
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP, (unsigned int) &g_waves);
#else
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP, (unsigned int) NULL);
#endif
        //stop get data timer
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_STOP_SEND, (unsigned int) NULL);

        //�ر������豸
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_AINOUT_CLOSE, (unsigned int) NULL);

        //�ر��м���豸
        mmm_pp_cmd(g_mmm_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);

        g_mmm_pp_handle = NULL;

        adjust_freq_add_value(0, 0);

        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);

        //ж�ؽ�������
        //sys_free_mmm(TRUE);
    }
    else
    { 
        ud_set_cmd(SET_USPEAKER_24BIT,USPEAKER24_PAUSE);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void uengine_status_deal(void)
 * \����״̬����,����ǰ���ֲ���״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      uengine_control.c
 * \note
 */
/*******************************************************************************/
void uengine_status_deal(void)
{
    ;//do nothing
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void uegine_check_status(void)
 * \�������״̬��ʱ�����������ñ�־λ
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      uengine_control.c
 * \note
 */
/*******************************************************************************/
void uegine_check_status(void)
{
    g_check_status_flag = TRUE;
}

