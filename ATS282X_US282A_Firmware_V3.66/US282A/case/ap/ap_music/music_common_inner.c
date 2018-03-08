/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������music����ͼ�����Ĵ���
 * ���ߣ�Gary Wang
 ********************************************************************************/
#include "music.h"

uint8 g_seq_tts_sec_ids[10];

bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    //�����������ĵ�ַ
    msg.content.addr = input_param;

    //��Ż�ȡ�����ĵ�ַ
    reply.content = output_param;

    //��Ϣ����(����Ϣ����)
    msg.type = msg_type;

    //����ͬ����Ϣ
    if (send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0) == ERR_NO_ERR)
    {
        if (reply.type == MSG_REPLY_FAILED)
        {
            bret = FALSE;
        }
    }
    else
    {
        bret = FALSE;
    }

    return bret;
}

bool music_play(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);
    return bret;
}

bool music_pause(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PAUSE_SYNC);
    return bret;
}

bool music_resume(play_mode_e play_mode)
{
    bool bret;
    bret = music_send_msg(&play_mode, NULL, MSG_MENGINE_RESUME_SYNC);
    return bret;
}

bool music_stop(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_STOP_SYNC);
    return bret;
}

bool music_digital_song(uint16 song_num)
{
    bool bret;
    //����ͬ����Ϣ
    bret = music_send_msg(&song_num, NULL, MSG_MENGINE_DIGITAL_SONG_SYNC);
    return bret;
}

void music_switch_song(uint8 tts_id, play_mode_e play_mode)
{
    music_get_filepath(&g_file_path_info);

    //��������
    if (tts_id != TTS_INVALID)
    {
        if (tts_id == TTS_SONG_SEQ)
        {
            music_tts_song_seq(g_file_path_info.file_path.dirlocation.file_num, g_seq_tts_sec_ids);
            if (g_neednot_song_tts_play == FALSE)
            {
                com_tts_state_play(TTS_MODE_SECLIST, (void *) g_seq_tts_sec_ids);
            }
        }
        else
        {
            if (g_neednot_song_tts_play == FALSE)
            {
                com_tts_state_play(TTS_MODE_ONLYONE, (void *) ((uint32) tts_id));
            }
        }
    }

    if(play_mode == PLAY_RESUME)
    {
        music_resume(PLAY_RESUME);
    }
    else/* if(play_mode == PLAY_NORMAL)*/
    {
        music_play();
    }
}

bool music_get_filepath(file_path_info_t *filepath)
{
    bool bret;
    bret = music_send_msg(NULL, filepath, MSG_MENGINE_GET_FILEPATH_SYNC);
    return bret;
}

bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
{
    bool bret;
    bret = music_send_msg(NULL, fileinfo, MSG_MENGINE_GET_FILEINFO_SYNC);
  
    if (TRUE == g_music_restore_from_s3bt)
    {
        fileinfo->total_time = g_music_config.total_time;
        
        g_music_restore_from_s3bt = FALSE; 
    }
    else
    {
        g_music_config.total_time = fileinfo->total_time;
    }
     
    return bret;
}

//����ļ��л���־
bool music_clear_fileswitch(void)
{
    //���֮ǰ���ȱ����л���Ϣ
    music_get_filepath(&g_file_path_info);
    music_file_switch_save();
    music_get_fileinfo(&g_music_fileinfo);
    return music_send_msg(NULL, NULL, MSG_MENGINE_CLEAR_FILESWITCH_SYNC);
}

bool music_get_loop_mode(uint8* loop_mode)
{
    bool bret;
    bret = music_send_msg(NULL, loop_mode, MSG_MENGINE_GET_PLAYMODE_SYNC);
    return bret;
}

bool music_set_loop_mode(uint8 loop_mode)
{
    bool bret;
    bret = music_send_msg(&loop_mode, NULL, MSG_MENGINE_SET_PLAYMODE_SYNC);
    return bret;
}

uint8 music_tts_song_seq(uint16 value, uint8 *tts_sec_ids)
{
    uint8 section_cnt = 0;
    uint8 thousand, hundred, ten, one;
    bool number_bitmap = 0x00; //bit 3 ~ 0 �ֱ�Ϊǧ��ʮ��

    thousand = (uint8) (value / 1000);
    hundred = (uint8) ((value % 1000) / 100);
    ten = (uint8) ((value % 100) / 10);
    one = (uint8) (value % 10);

    //��
    tts_sec_ids[section_cnt] = TTS_SECTION_NO;
    section_cnt++;
    //ǧλ������
    if (thousand > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + thousand;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_THOUSAND;
        section_cnt++;
        number_bitmap |= 0x08;
    }

    //��λ������
    if (hundred > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + hundred;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_HUNDRED;
        section_cnt++;
        number_bitmap |= 0x04;
    }
    else if ((number_bitmap & 0x08) != 0) //ǧλ����Ϊ0
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0;
        section_cnt++;
    }
    else
    {
        //for QAC
    }

    //ʮλ������
    if (ten > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + ten;
        section_cnt++;
        tts_sec_ids[section_cnt] = TTS_SECTION_TEN;
        section_cnt++;
        number_bitmap |= 0x02;
    }
    else if ((number_bitmap & 0x04) != 0) //��λ����Ϊ0
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0;
        section_cnt++;
    }
    else
    {
        //for QAC
    }

    //��λ������
    if (one > 0)
    {
        tts_sec_ids[section_cnt] = TTS_SECTION_NUM_0 + one;
        section_cnt++;
    }

    //��
    tts_sec_ids[section_cnt] = TTS_SECTION_SONG;
    section_cnt++;

    tts_sec_ids[section_cnt] = 0xff;
    return section_cnt;
}

bool music_set_sortnumber(uint16 destnum)
{
    bool bret;
    bret = music_send_msg(&destnum, NULL, MSG_MENGINE_SET_SORTNUMBER_SYNC);
    return bret;
}

bool music_get_disknumber(uint16* disk_num)
{
    bool bret;
    bret = music_send_msg(NULL, disk_num, MSG_MENGINE_GET_DISKNUMBER_SYNC);
    return bret;
}

void music_create_song_num_view(view_type_e view_type, bool tts, play_mode_e play_mode)
{
    music_clear_fileswitch();
    create_view_song_num(view_type, tts, play_mode);
}

app_result_e music_song_num_tts(void *ev_param)
{
    //��������
    music_switch_song(TTS_SONG_SEQ, (uint32) ev_param);
    return RESULT_NULL;
}

void music_file_switch_save(void)
{
    if((g_rcp_file_status.file_switch == 0)
        ||
        (
        (g_rcp_file_status.file_switch == 1)
        &&
        (g_rcp_file_status.file_seq[0] != g_file_path_info.file_path.dirlocation.file_num)
        )
        )
    {
        g_rcp_file_status.file_seq[g_rcp_file_status.file_switch] =
            g_file_path_info.file_path.dirlocation.file_num;
        g_rcp_file_status.file_switch++;
    }
}
