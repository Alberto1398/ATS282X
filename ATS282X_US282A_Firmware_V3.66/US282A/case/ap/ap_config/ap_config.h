/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     ap_config.h
 * \brief    configӦ�õ�ͷ�ļ�
 * \author   zhangxs
 * \version 1.0
 * \date  2011/09/04
 *******************************************************************************/
#ifndef _AP_CONFIG_H
#define _AP_CONFIG_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "card.h"

#define SWITCH_ON           0 //����
#define SWITCH_OFF          1 //�ػ�
#define SWITCH_LOWPOWER     2 //�͵�ػ�
#define SWITCH_STANDBY      3 //���͹���ģʽ
#define SWITCH_IDLE_MODE    4 //�ⲿ��緽���Ŀ���ģʽ
#define POWER_ON            5 //ap_manager���Σ��ϵ翪��

typedef struct
{
    uint16 active; //�˲���û�õ���ֻ�Ǳ�����config.bin�е����ݽṹһ��
    uint16 total; //��������
    uint16 keyID[16]; //������������
} cfg_key_var_t;

typedef struct
{
    uint16 active_item;//�AP�˵�����
    uint16 total_item; //AP�˵�����
    uint16 ap_funcs[MAX_FUNCTON_CYCLE]; //֧��Ӧ�ñ�ŵ�����
} cfg_loop_func_var_t;

/*wav¼�������д�ļ�ͷ�ṹ��*/
typedef struct
{
    /*����*/
    uint8 rec_filename[28];
    /*�ļ�ƫ����Ϣ*/
    pfile_offset_t ptr_file_offset;
    /*��¼�ļ���С*/
    uint32 file_bytes;
    /*¼��������*/
    uint32 bitrate;
    /*Ŀ¼��Ϣ,73byte,��Ҫ������*/
    pdir_layer_t ptr_layer;
    /*�̷���Ϣ*/
    uint8 disk_no;
    /*¼����ʽ��0-wav, 1-mp3*/
    uint8 rec_format;
    /*��Ҫ��д��־*/
    bool rewrite_need;
} record_writehead_t;

extern record_writehead_t g_writehead;
extern comval_t g_comval;
extern bool usb_in_flag;
extern bool uhost_in_flag;
extern bool adaptor_in_flag;
extern bool sd_in_flag;
extern bool linein_in_flag;
extern bool rtcalarm_flag;
extern bool need_lowpower_tts_flag;
extern bool g_config_standby_exit;
extern bool g_config_bt_flag;
extern bool g_config_esd_restart;
extern bool g_config_volume_init;

extern uint8 g_uhost_delay;

extern app_result_e config_charging(int param) __FAR__;
extern app_result_e check_enter_test_mode(void) __FAR__;
extern void en_play_wake_up(bool standby_enter) __FAR__;
extern app_result_e sys_power_off(bool standby_enter) __FAR__;
extern app_result_e config_poweron_option(void) __FAR__;
extern void config_flush_rtc_reg(void) __FAR__;

extern app_result_e config_idle_mode_loop(void) __FAR__;

extern bool detect_power_on(void) __FAR__;
extern void config_record_writehead(void) __FAR__;
extern app_result_e open_stub(void) __FAR__;

extern void card_set_drv_power(void) __FAR__;
extern int config_key_module(void) __FAR__;
extern void config_sys_init(void) __FAR__;
extern void config_sys_init_display(void) __FAR__;

extern app_result_e load_att_code(int32 file_sys_id, uint8 stg_base) __FAR__;
extern OTA_malloc_param_t *g_OTA_var __FAR__;

extern void config_power_on_dispatch(app_result_e result) __FAR__;
extern g_config_var_t g_config_var;


#endif

