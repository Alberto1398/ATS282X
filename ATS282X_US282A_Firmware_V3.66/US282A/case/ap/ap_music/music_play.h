/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������Ŵ���
 * ���ߣ�Gary Wang
 ********************************************************************************/
#ifndef __MUSIC_PLAY_H__
#define __MUSIC_PLAY_H__

#include "case_include.h"

//ˢ��
#define PLAY_REFRESH_ALL     ((uint8)-1)
#define PLAY_REFRESH_TIME    ((uint8)1 << 0)
#define PLAY_REFRESH_STATUS  ((uint8)1 << 1)
#define PLAY_REFRESH_DISK    ((uint8)1 << 2)

//ˢ�±�־
extern uint8 g_play_refresh_flag;

//�ļ�·����Դ��Ϣ���棬����ʱ����
extern file_path_info_t g_file_path_info;
//music��ʱ����Ϣ
extern mmm_mp_file_info_t g_music_fileinfo;
//����ʾ���Ƿ��˳�������
extern bool g_quit_from_hint;

//�������������
extern uint16 g_error_num;
//�ϴδ���ĸ�����
extern uint16 g_error_file_num;
//��ǰ��(�����һ���պô���Ҫ������һ��������һ��)
extern uint16 g_switch_prev;

extern bool g_neednot_song_tts_play;

void play_view_handle(view_update_e mode) __FAR__;

app_result_e music_scene_play(void) __FAR__;
bool play_get_status(mengine_info_t *p_music_info) __FAR__;

#endif //__MUSIC_PLAY_H__
