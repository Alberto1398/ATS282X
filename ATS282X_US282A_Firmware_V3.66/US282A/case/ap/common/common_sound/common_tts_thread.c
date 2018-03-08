/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TTS �������߳�LOOP���Լ�TTS���غ�ж�ؽӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_tts.h"
#include "common_keytone.h"
#include "common_func.h"
/******************************************************************************/
/*!
 * \par  Description:
 * \bool com_tts_state_sync(uint16 tts_mode, bool start_or_stop)
 *    ״̬��������
 * \param[in]    tts_mode TTS����ģʽ
 * \param[in]    start_or_stop 1��ʾ��ʼ������Ϊ0��ʾ�������
 * \param[in]    value ״ֵ̬
 * \param[out]   none
 * \return       ����TRUE��ʾ���Բ���������FALSE��ʾ���ܲ���
 * \ingroup      misc_func
 * \note
 * \li
 *******************************************************************************/
bool com_tts_state_sync(uint16 tts_mode, bool start_or_stop)
{
    msg_apps_t msg;
    msg_reply_t reply;
    bool bret = TRUE;

    if ((tts_mode & TTS_MODE_PB_OPTION_MASK) == TTS_MODE_PB_PLAY)
    {
        msg.content.data[0] = TRUE;
    }
    else
    {
        msg.content.data[0] = FALSE;
    }

    if (start_or_stop == TRUE)
    {
        msg.type = MSG_TTS_START_SYNC;
    }
    else
    {
        msg.type = MSG_TTS_STOP_SYNC;
    }

    //����ͬ����Ϣ
    if (get_engine_type() != ENGINE_NULL)
    {
        bret = (bool) (send_sync_msg(APP_ID_THEENGINE, &msg, &reply, 0) == ERR_NO_ERR);
    }
    else
    {
        return bret;
    }

    if (bret == TRUE)
    {
        if ((start_or_stop == TRUE) && (reply.type != MSG_REPLY_SUCCESS))
        {
            bret = FALSE;
        }
    }

    return bret;
}

bool tts_play_init(uint16 tts_mode, void* tts_ptr)
{
    bool ret = TRUE;
    uint8 section_cnt;
    int install_ret;
    uint8 dac_chan;
    uint8 system_vol = 0;
    
    //TTS��ʼ���������ز�������
    if (com_tts_state_sync(tts_mode, TRUE) == FALSE)
    {
        ret = FALSE;
        goto tts_ret;
    }

    //install tts driver
    sys_os_sched_lock();
    
    //�����밴������ͻ�����������밴��������
    dac_chan = (uint8)((g_keytone_infor.dac_chan == DAF0_EN)? DAF1_EN : DAF0_EN); 
    
    install_ret = sys_drv_install(DRV_GROUP_TTS, dac_chan, "tts_play.drv");
    sys_os_sched_unlock();
    if (install_ret == -1)
    {
        ret = FALSE;
        goto tts_ret;
    }

    if ((tts_mode & TTS_MODE_TYPE_MASK) == TTS_MODE_STRING)
    {
        section_cnt = 1;
    }
    else
    {
        section_cnt = 0;
        while (1)
        {
            if (g_tts_play_info.section_ids[section_cnt] == 0xff)
            {
                break;
            }
            if ((section_cnt + 1) >= TTS_SECTIONS_MAX)
            {
                break;
            }

            g_tts_play_info.section_ids[section_cnt] = g_tts_play_info.section_ids[section_cnt] * TTS_LANGUAGE_TOTAL
                    + sys_comval->tts_language;
            section_cnt++;
        }
    }

    if (section_cnt == 0)
    {
        PRINT_WARNING("tts secti zero");
        ret = FALSE;
        goto tts_ret;
    }

    if (tts_status_play_init(g_tts_play_info.section_ids, section_cnt, tts_ptr) == -1)
    {
        //PRINT_WARNING("tts play fail");
        ret = FALSE;
        goto tts_ret;
    }

    g_tts_play_info.status = TTS_STATUS_PLAYING;

    tts_ret:
    
    if (ret == TRUE)
    {
        //DAEģʽ�л�ʱ���뱣֤�������Ѳ���
        //keytone_play_deal_wait();

        //com_set_dae_onoff(FALSE);

        system_vol = sys_comval->volume_current;
        if (system_vol < TTS_MIN_VOL)
        {
            g_keytone_infor.kt_mute = 1;
            system_vol = TTS_MIN_VOL;
            com_set_phy_volume(system_vol); 
        }
    }
    
    return ret;
}

void tts_play_exit(uint16 tts_mode)
{
    //��������ж��dsp�����Դ������TTS�����������룬�����һ��TTS����������
    //�ڶ���TTS��ʼ���ţ���ô�ڶ���Ҫ�ȴ���һ��������ܲ��ţ���һ���ᱻ�ڶ���
    //������һ��tts_play_exit,��ô�ȵڶ��������꣬��һ���ص��˳��ĵط��ֻ���
    //��һ�Σ�������Դ���ͷ�������⣬�����Դ�ͷŷŵ�TTS���߳��д���
    //tts_status_play_exit();
    uint8 system_vol = 0;
    
    g_tts_play_info.status = TTS_STATUS_IDLE;

    g_tts_play_info.option &= (~FORCE_STOP_TTS); //��ʾ��ֹtts
    
    //tts exit close dsp
    sys_drv_uninstall(DRV_GROUP_TTS);
    
    //DAEģʽ�л�ʱ���뱣֤�������Ѳ���
    keytone_play_deal_wait();

	sys_comval->dae_cfg.enable = 1;
	com_update_dae_config(&sys_comval->dae_cfg);
    __set_dae_config(&sys_comval->dae_cfg);


    system_vol = sys_comval->volume_current; 

    if (system_vol < TTS_MIN_VOL)
    {
        com_set_phy_volume(system_vol);
        g_keytone_infor.kt_mute = 0;
    }
 
    //TTS�������
    com_tts_state_sync(tts_mode, FALSE);
}

void tts_play_loop(void)
{
    uint8 delay_time;
    uint8 dac_chan = (uint8)((g_keytone_infor.dac_chan == DAF0_EN)? DAF1_EN : DAF0_EN);

    while (1)
    {
        if ((g_tts_play_info.option & FORCE_STOP_TTS) != 0)
        { //ǿ����ֹtts���˳�
            break;
        }

        if (tts_status_play_frame() == -1)
        {
            break;
        }

        sys_os_time_dly(1);
    }

    //�ȴ�TTS���Ž���
    delay_time = 0;
    while (1)
    {
        if (dac_chan == DAF0_EN)
        {
            if ((act_readl(ASRC_OUT0_IP) & (1 << ASRC_OUT0_IP_PCMFIFOEmpty)) != 0)
            {
                break;
            }
        }
        else
        {
            if ((act_readl(ASRC_OUT1_IP) & (1 << ASRC_OUT1_IP_PCMFIFOEmpty)) != 0)
            {
                break;
            }
        }

        sys_os_time_dly(1);

        delay_time++;
        if (delay_time > 20)
        {
            break;
        }
    }

    tts_status_play_exit();

    g_tts_play_info.status = TTS_STATUS_PLAYOVER;

    libc_pthread_exit();
}
