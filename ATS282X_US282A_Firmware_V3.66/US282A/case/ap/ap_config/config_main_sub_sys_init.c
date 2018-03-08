/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_main_sub.c
 * \brief    ���ػ��Ĵ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

void config_init_data_time(void)
{
    date_t init_data;
    time_t init_time;

    init_data.year = 2015;
    init_data.month = 1;
    init_data.day = 1;
    sys_set_date(&init_data);

    init_time.hour = 0;
    init_time.minute = 0;
    init_time.second = 0;
    sys_set_time(&init_time);
}

void config_sys_init_volume(void)
{
    uint8 reset_volume;
    uint8 min_volume;
    uint8 max_volume;
    uint8 cur_volume;

    sys_comval->volume_current_phy = 0xff;
    com_switch_volume_mode(VOLUME_MODE_NORMAL);
    com_set_volume_gain((uint8) com_get_config_default(SETTING_SOUND_MAIN_VOLUME_GAIN));
    g_comval.volume_limit = 0;

    if ((g_config_standby_exit == FALSE) && (g_config_esd_restart == FALSE))
    {
        reset_volume = (uint8) com_get_config_default(SETTING_SOUND_VOLUME_RESET_POWERON);
        if (reset_volume == 1)
        {
            g_comval.volume[VOLUME_MODE_NORMAL] = (uint8) com_get_config_default(SETTING_SOUND_DEFAULT_MAIN_VOLUME);
            g_comval.volume[VOLUME_MODE_CALL] = (uint8) com_get_config_default(SETTING_SOUND_DEFAULT_CALL_VOLUME);
            cur_volume = g_comval.volume[VOLUME_MODE_NORMAL];
        }
        else
        {
            min_volume = (uint8) com_get_config_default(SETTING_SOUND_MIN_VOLUME_POWERON);
            max_volume = (uint8) com_get_config_default(SETTING_SOUND_MAX_VOLUME_POWERON);
            cur_volume = com_get_sound_volume();
            if (cur_volume < min_volume)
            {
                cur_volume = min_volume;
            }
            if (cur_volume > max_volume)
            {
                cur_volume = max_volume;
            }
        }
    }
    else
    {
        cur_volume = com_get_sound_volume();
    }

    com_set_sound_volume(cur_volume, 0);
}

/******************************************************************************/
/*!
 * \par  Description:
 *  config��ʼ��������������غ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/

void config_sys_init_display(void)
{
    //��װ��ʾ����
#if (SUPPORT_LED_DRIVER == 1)
    if (g_comval.support_led_display == 1)
    {
        sys_drv_install(DRV_GROUP_LCD, 0, "drv_led.drv");
        led_clear_screen();
        led_display(0, NUM_ERROR, 1);
        led_display(1, NUM_ERROR, 1);
        led_display(2, NUM_ERROR, 1);
        led_display(3, NUM_ERROR, 1);
    }
#endif
    //��ʼ������LED��ģ��
    discrete_led_init();
}

void config_sys_init(void)
{
    //��������
    config_sys_init_volume();

    //���ð�����
    keytone_set_dac_chan(DAF0_EN); //Ĭ������ΪDACͨ��0
    keytone_set_dac_sample(KEYTONE_SAMPLE_RATE);
    keytone_set_on_off(FALSE); //�����׶���ʱ�رհ�����

    //�ж��Ƿ���磬���ǣ���ʼ������2015-1-1��ʱ��00:00:00
    if ((act_readl(RTC_CTL) & 0x0010) == 0)
    {
        config_init_data_time();
    }

    return;
}

int config_key_module(void)
{
    cfg_key_var_t cfg_key;
    uint8 i;

    //��ȡ��������
    com_get_config_struct(SETTING_INPUT_KEY_ARRAY, (uint8 *) &cfg_key, sizeof(cfg_key_var_t));
    for (i = 0; i < cfg_key.total; i++)
    {
        g_key_infor.key_val_map[i] = (uint8) cfg_key.keyID[i];
    }

    //��ȡ��������
    com_get_config_struct(SETTING_INPUT_TK_KEY_ARRAY, (uint8 *) &cfg_key, sizeof(cfg_key_var_t));
    for (i = 0; i < cfg_key.total; i++)
    {
        g_key_infor.tk_key_val_map[i] = (uint8) cfg_key.keyID[i];
    }

    return 0;
}
