/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������music��ؽӿ�
 * ���ߣ�Gary Wang
 ********************************************************************************/
#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "psp_includes.h"
#include "case_include.h"
#include "music_common.h"
#include "enhanced.h"
#include "music_ab.h"
#include "music_common_inner.h"
#include "music_digit_song.h"
#include "music_hint.h"
#include "music_loop_mode.h"
#include "music_play.h"
#include "music_play2.h"
#include "music_rcp.h"
#include "music_song_number.h"

/*!��������Ӧ�õ�����ջ�����߳����ȼ�*/
#define AP_MUSIC_STK_POS    (AP_FRONT_LOW_STK_POS)
#define AP_MUSIC_PRIO       (AP_FRONT_LOW_PRIO)

//ע�⣺����궨�岻�ܸ�common_func.h�����TTS_xxx(��TTS_ENTER_BT)�ظ�
#define TTS_SONG_SEQ        (0x80)//��Ŀ��
#define TTS_INVALID         (0xff)

//����о���map�����㣬�����¶���
#define MUS_STATIC
//#define MUS_STATIC          static

#define MIN(a, b)           (((a) > (b)) ? (b) : (a))
#define MAX(a, b)           (((a) < (b)) ? (b) : (a))

//��view��ID��(ע�ⲻҪ��APP_VIEW_ID_MAIN��common�õ�ID���ظ�)
typedef enum
{
    VIEW_ID_SONG_NUMBER = 1,
    VIEW_ID_LOOP_MODE,
    VIEW_ID_DIGIT_SONG,
    #ifdef AB_SUPPORT
    VIEW_ID_AB,
    #endif
    VIEW_ID_HINT,
}
music_view_id_e;

typedef struct
{
    /*! ħ�� */
    uint16 magic;
    /*! ѭ��ģʽ */
    fsel_mode_e repeat_mode;
    /*! shuffle ���� ȡ��4bit*/
    uint8 shuffle_flag;
   /*! ��ǰ������ʱ��*/
    int total_time;
} music_config_t;

//����music�ķ�ʽ
extern app_param_e g_enter_mode;

//��ʼ���׶Σ����Թ��˵�һЩ�¼�����
extern bool g_musicplay_init_flag;

//�����и赽�ڼ���
extern uint16 g_manul_switch_song_seq;

//��ǰ����״̬��Ϣ����ǰ����ʱ��ͱ�������Ϣ
extern mengine_info_t g_music_info;

//music ui����������Ϣ
extern music_config_t g_music_config;

//ϵͳȫ�ֱ���
extern comval_t g_comval;

//����ɨ���߳̾��
extern void *g_scan_handle;

//ɨ����
extern uint8 g_scan_flag;

//rcp save some file switch info
extern rcp_file_status_t g_rcp_file_status;

#ifdef PRINT_BANK_INFO
extern int print_label;
#endif

extern uint8 g_seq_tts_sec_ids[10];

//rcpͨѶ�еõ������б�ʱ�Ķ�ʱ��
extern int8 g_timer_id_rcp_getplist;

extern app_result_e music_get_dir_list_status(uint32 data1, uint32 data2) __FAR__;
extern app_result_e music_get_music_dir_list(uint32 data1, uint32 data2) __FAR__;
extern bool get_dir_by_dirno(uint16 dir_no, list_dir_info_t *p_dir) __FAR__;
extern void update_dir_list_random(void) __FAR__;

extern bool g_music_restore_from_s3bt;

#endif //__MUSIC_H__
