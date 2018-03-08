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
 * \file     config_main_sub_globe_data_init.c
 * \brief    ���ػ��Ĵ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

//���ز���ʼ��Ӧ�ù���������
void config_load_app_config(void)
{
    cfg_loop_func_var_t cfg_loop_func_var;
    uint8 i;

    com_get_config_struct(SETTING_APP_SWITCH_SEQUENCE, (uint8 *) &cfg_loop_func_var, sizeof(cfg_loop_func_var_t));

    for (i = 0; (i < cfg_loop_func_var.total_item) && (i < MAX_FUNCTON_CYCLE); i++)
    {
        g_ap_switch_var.app_switch_seq[i] = (uint8) cfg_loop_func_var.ap_funcs[i];
    }
    g_ap_switch_var.app_default_func_id = (uint8) cfg_loop_func_var.active_item;

    g_ap_switch_var.app_func_support = 0xffffffff;

    //������֧��
    if ((g_comval.hard_support_card == 0) || (g_comval.support_cardplay == 0))
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYCARD));
    }
    //U�̲���֧��
    if ((g_comval.hard_support_uhost == 0) || (g_comval.support_uhostplay == 0))
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYUHOST));
    }
    //LINEIN����֧��
    if (g_comval.support_linein == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYLINEIN));
    }
    //FM ������֧��
    if (g_comval.support_fmradio == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_RADIO));
    }
    //��������֧��
    if (g_comval.support_btplay == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_BTPLAY));
    }
    //������绰֧��
    if (g_comval.support_btcall == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_BTCALL));
    }
    //USB����֧��
    if (g_comval.support_usbbox == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_USOUND));
    }
    //����������֧��
    if ((g_comval.hard_support_card == 0) || (g_comval.support_usbmsc == 0))
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_UDISK));
    }
    //OTA��������֧��
    if ( (g_comval.support_ota_song_test == 0))
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_OTA_TEST));
    }
}

//��ʼ������ǰ̨Ӧ�ù����ȫ�ֱ�������Щȫ�ֱ���������ֻ�ܳ�ʼ��һ�Σ�
void config_globe_data_init(void)
{
    uint8 i;
    ota_part_info_t *p_part_info;
    libc_memset(&g_app_last_state, 0x00, sizeof(g_app_last_state));
    libc_memset(&g_sys_counter, 0x00, sizeof(sys_counter_t));
    libc_memset(&g_buffer_gui_msg, 0x00, sizeof(input_gui_msg_t));
    libc_memset(&g_buffer_app_msg, 0x00, sizeof(private_msg_t));

    for (i = 0; i < COM_APP_TIMER_MAX; i++)
    {
        libc_memset(&(g_com_app_timer_vct[i]), 0x00, sizeof(app_timer_t));
        g_com_app_timer_vct[i].state = TIMER_STATE_NOUSED;
    }

    //key module
    libc_memset(&g_key_infor, 0x00, sizeof(key_infor_t));

    libc_memset(&g_keytone_infor, 0x00, sizeof(keytone_infor_t));
    libc_memset(g_keytone_infor.kt_fifo, 0xff, KEYTONE_FIFO_DEPTH);
    g_keytone_infor.dac_chan = -1;//��������Ч

    libc_memset(&g_tts_play_info, 0x00, sizeof(tts_play_info_t));

    libc_memset(&g_ap_switch_var, 0x00, sizeof(ap_switch_var_t));
    //���ز���ʼ��Ӧ�ù���������
    config_load_app_config();

    //RCP �Ի���
    g_dialog_type = DIALOG_TYPE_NONE;
    g_dialog_state = DIALOG_STATE_NONE;

    //ASETȫ�ֱ�����ʼ��
#ifdef SUPPORT_ASET_TEST
    g_aset_sync_volume = 0;
    g_aset_neednot_take_effect = 0;
    g_aset_main_switch_flag = 1;
#endif


    g_OTA_var = sys_malloc(sizeof(OTA_malloc_param_t) );//����OTA����
    if(g_OTA_var==NULL)
    {
        libc_print("malloc g_OTA_var fail",0,0);
        while(1)
        {
            ;//nothing
        }
    }
    g_OTA_var->g_connect_repy.connect_state = 0;//��������OTA���ֱ�־
    p_part_info = base_ota_read_part_info();
    if(p_part_info != NULL)
    {
        PRINTD_DATA(p_part_info,p_part_info->part_total +1);
    }
    
}


