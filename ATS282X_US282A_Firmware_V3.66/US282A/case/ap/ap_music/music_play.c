//
//      ���Ŵ���
//
//1.����ͼ
//2.������/��/���/����/����/��ͣ/����/ģʽ
//  ��������/��������/Ŀ¼��/Ŀ¼��
//  (������/������/EQ��commonȥ����)
//  ����/POWER/¼����commonȥ��������Ӧ����ȫ�޹�
//3.����REPEAT��(����REPEATE���ڣ��������������ϢҲ���͸�����)
//4.����AB��(����AB���ڣ��������������ϢҲ���͸�����)
//5.�������ּ�(��������ѡ�贰��, �������������ϢҲ���͸�����)
//6.�����쳣��һ�׸��ʽ����/ȫ������/��U�γ�
//7.����������Ϣ
//
//��ע��1.
//      2.
#include "music.h"
#include "music_scan_cmd.h"

//���/���˵Ĳ���(��λ:����)
#define FF_FB_STEP      (5000)

//timer��ʱ����(��λ:����)
#define T1_INTERVAL     (250)

//�������������
uint16 g_error_num = 0;
//�ϴδ���ĸ�����
uint16 g_error_file_num = 0;
//��ǰ��(�����һ���պô���Ҫ������һ��������һ��)
uint16 g_switch_prev = 0;
bool g_neednot_song_tts_play;
//�и�֮ǰ��״̬
play_status_e g_last_play_status = StopSta;
//�ļ�·����Դ��Ϣ���棬����ʱ����
file_path_info_t g_file_path_info;
//��ǰ����״̬��Ϣ����ǰ����ʱ��ͱ�������Ϣ
mengine_info_t g_music_info;
//music��ʱ����Ϣ
mmm_mp_file_info_t g_music_fileinfo;
//music����������Ϣ
music_config_t g_music_config;
//����ʾ���Ƿ��˳�������
bool g_quit_from_hint = FALSE;
//ˢ�±�־
uint8 g_play_refresh_flag = PLAY_REFRESH_ALL;
static int8 music_sound_mode_timer_id = -1;
    
//ǰһ�μ�¼�Ĳ���ʱ��(��λ:��)
MUS_STATIC uint32 s_prev_time;
MUS_STATIC play_status_e s_status_old = StopSta;
//���¿��/���˼��󣬻�û��̧��
MUS_STATIC bool s_fast = FALSE;
//�����ERROR״̬
MUS_STATIC eg_err_e s_eg_err_status = EG_ERR_NONE;

//��Ϣ-�¼���

MUS_STATIC app_result_e _play_key_ff(void);
MUS_STATIC app_result_e _play_key_fb(void);
MUS_STATIC app_result_e _play_stop_ffb(void);
MUS_STATIC app_result_e _play_key_ab(void);
MUS_STATIC app_result_e _play_key_repeat(void);
MUS_STATIC app_result_e _play_key_mute(void) __FAR__;
MUS_STATIC app_result_e _play_key_folderadd(void) __FAR__;
MUS_STATIC app_result_e _play_key_foldersub(void) __FAR__;
MUS_STATIC app_result_e _play_key_number(void);
MUS_STATIC app_result_e _play_disk_out(void *ev_param) __FAR__;
MUS_STATIC app_result_e _play_card_out(void *ev_param) __FAR__;
MUS_STATIC app_result_e _play_uhost_out(void *ev_param) __FAR__;

MUS_STATIC app_result_e get_message_loop(void);
MUS_STATIC void _play_create_view(void);

MUS_STATIC  app_result_e _audiopp_update(void *ev_param);
MUS_STATIC __section__(".text.BANK20") void update_sound_mode(void);
void __section__(".text.BANK20")  update_waves_bin_para(void);

MUS_STATIC const key_event_map_t __section__(".rodata.ke_maplist") music_play_key_map_list[] =
{
    /*! �̰���next�л���һ�� */
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, play_key_next },
    /*! �̰���prev�л���һ�� */
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, play_key_prev },
    /*! ����next��� */
    { { KEY_NEXT, 0, KEY_TYPE_LONG, KEY_DEAL_NULL }, _play_key_ff },
    { { KEY_NEXT, 0, KEY_TYPE_LONG_UP, KEY_DEAL_NULL }, _play_stop_ffb },
    /*! ����prev���� */
    { { KEY_PREV, 0, KEY_TYPE_LONG, KEY_DEAL_NULL }, _play_key_fb },
    { { KEY_PREV, 0, KEY_TYPE_LONG_UP, KEY_DEAL_NULL }, _play_stop_ffb },
    /*! �̰���next�л���һ�� */
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, play_key_next },
    /*! �̰���prev�л���һ�� */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, play_key_prev },
    /*!�̰�play����/��ͣ�¼� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, play_key_play },
    { { KEY_PHONEPLAY, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, play_key_play },
    #ifdef AB_SUPPORT
    { { KEY_ABREC, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, _play_key_ab },
    #endif
    /*! ѭ��ģʽ�����¼� */
    { { KEY_REPEAT, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, _play_key_repeat },
    /*! mute�����¼� */
    { { KEY_MUTE, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, _play_key_mute },
    /*! forder���¼� */
    { { KEY_FOLDER_ADD, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, _play_key_folderadd },
    /*! forder���¼� */
    { { KEY_FOLDER_SUB, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, _play_key_foldersub },
    /*! ���ֵ���¼� */
    { { KEY_NUMBER, 0, KEY_TYPE_SHORT_UP, KEY_DEAL_NULL }, _play_key_number },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL, KEY_DEAL_NULL }, NULL },
};

MUS_STATIC const sys_event_map_t __section__(".rodata.se_maplist") music_play_se_map_list[] =
{
    { { MSG_SD_OUT, SYSMSG_STOP_TTS_YES }, _play_card_out, },
    { { MSG_UH_OUT, SYSMSG_STOP_TTS_YES }, _play_uhost_out, },

    { { MSG_MUSIC_SONG_NUM_TTS, SYSMSG_STOP_TTS_YES }, music_song_num_tts, },

    /*! ������־ */
    { { MSG_NULL, 0 }, NULL },
};

bool __section__(".rcode") play_get_status(mengine_info_t *p_music_info)
{
    if (sys_share_query_read(APP_ID_MENGINE, p_music_info) == -1)
    {
        PRINT_ERR("music share query not exist!\n");
        return FALSE;
    }

    return TRUE;
}

MUS_STATIC bool __section__(".text.BANK2") _play_set_ffb_step(void)
{
    bool bret;
    uint32 step_len = FF_FB_STEP;
    bret = music_send_msg((void *) &step_len, NULL, MSG_MENGINE_SET_FFBSTEP_SYNC);
    return bret;
}

app_result_e music_scene_play(void)
{
    app_result_e ret_vals = RESULT_NULL;
    uint8 disk_in_play;

    _play_create_view();
    //�������������
    g_error_num = 0;
    g_error_file_num = 0;
    g_manul_switch_song_seq = 0;

    ret_vals = play_init();
    if (ret_vals > RESULT_COMMON_RESERVE)
    {
        //���ó����˳�����
        play_exit();
        return ret_vals;
    }

    //��������,û�д���
    if (g_music_info.eg_status.err_status == EG_ERR_NONE)
    {
        music_create_song_num_view(VIEW_TYPE_MSG, FALSE, PLAY_RESUME);
    }

    disk_in_play = g_file_path_info.file_path.dirlocation.disk;

    //����ɨ���߳�
    music_open_scan(disk_in_play);

    //�޸Ĳ����̵�״̬,commonʹ��
    if (disk_in_play == DISK_H)
    {
        set_play_disk_type(PLAY_DISK_CARD);
    }
    else
    {
        set_play_disk_type(PLAY_DISK_UHOST);
    }

    //����ѭ��
    ret_vals = get_message_loop();

    //�ر�ɨ���߳�
    music_close_scan();

    //���ó����˳�����
    play_exit();
    return ret_vals;
}

MUS_STATIC void _play_check_status(void)
{
    app_result_e sta_ret = RESULT_NULL;
    static bool play_ok_falg = FALSE;//�Ƿ��Ѿ��������ţ����ֿտ��ͳ����ʶ��Ϊ�տ�
    play_get_status(&g_music_info); //��ȡ״̬��Ϣ

    if (1 == sys_comval->dae_cfg.audiopp_type_update_flag)
    {
        update_sound_mode();
    }

    if (1 == g_app_info_state_all.waves_dae_para_update_flag)
    {
        update_waves_bin_para();
    }
    
    //��������,û�д���
    if (g_music_info.eg_status.err_status == EG_ERR_NONE)
    {
        play_ok_falg = TRUE;
        if ((g_music_info.eg_status.play_status != StopSta) && (g_music_info.eg_playinfo.cur_time > 0))
        {
            //�������������
            g_error_num = 0;
            g_error_file_num = 0;
            if (g_music_info.eg_status.play_status != PlayFast)
            {
                //�����ǰ�л���ʶ
                g_switch_prev = 0;
            }
        }
        s_eg_err_status = EG_ERR_NONE;

        //�Ƿ��и�
        if ((g_music_info.eg_playinfo.cur_file_switch & 0x01) != 0)
        {
            //����ļ��л���־
            if (g_file_path_info.file_path.dirlocation.file_num != g_manul_switch_song_seq)
            {
                music_create_song_num_view((s_fast == TRUE) ? VIEW_TYPE_MSG : VIEW_TYPE_SUB, FALSE, PLAY_NORMAL);
            }
            else
            {
                music_clear_fileswitch();
            }
            g_manul_switch_song_seq = 0;
        }

        if (s_prev_time != (g_music_info.eg_playinfo.cur_time / 1000))
        {
            s_prev_time = g_music_info.eg_playinfo.cur_time / 1000;
            g_play_refresh_flag |= PLAY_REFRESH_TIME;
            com_view_update(APP_VIEW_ID_MAIN);
        }
    }
    else if (s_eg_err_status != g_music_info.eg_status.err_status)
    {
        //������
        s_eg_err_status = g_music_info.eg_status.err_status;
        if((play_ok_falg == FALSE) && (s_eg_err_status == EG_ERR_NO_FILE))
        {
            sta_ret = RESULT_NEXT_FUNCTION;//�տ�
        }
        else if(s_eg_err_status == EG_ERR_RAED_CARD_TIMEOUT)
        {
            sta_ret = RESULT_NEXT_FUNCTION;//������ʱ
        }
        else
        {
            sta_ret = play_err_deal(g_music_info.eg_status.err_status);
        }
        
        if (sta_ret == RESULT_NEXT_FUNCTION)
        {
            //ֹͣ���ţ���Ȼ���������ʾER03ʱ�ֿ��ٲ��룬��������������
            //����һС��ʱ�䣬Ȼ���˳�music
            music_stop();
            create_view_hint(g_music_info.eg_status.err_status);
        }
        else if (sta_ret == RESULT_NULL)
        {
            s_eg_err_status = EG_ERR_NONE;
            music_create_song_num_view(VIEW_TYPE_MSG, FALSE, PLAY_NORMAL);
        }
        else
        {
        }
    }
    else
    {
        //�����Ѿ��õ�������
    }

    if (g_music_info.eg_status.play_status != s_status_old)
    {
        s_status_old = g_music_info.eg_status.play_status;
        g_play_refresh_flag |= PLAY_REFRESH_STATUS;
        com_view_update(APP_VIEW_ID_MAIN);
    }
}

MUS_STATIC app_result_e get_message_loop(void)
{
    uint8 scan_flag = 0;
    app_result_e ret_vals = RESULT_NULL;
    while (1)
    {
        if (g_quit_from_hint == TRUE)
        {
            ret_vals = RESULT_NEXT_FUNCTION;
            break;
        }

        _play_check_status(); //�������ȡ��ǰ״̬��Ϣ

        //����Ƿ�ɨ�����
        if (g_scan_handle != NULL)
        {
            music_scan_cmd(g_scan_handle, MUSIC_SCAN_GET_STATUS, (unsigned int) &g_scan_flag);
            //�ر�ɨ���߳�
            if ((g_scan_flag & 0x81) != 0)
            {
                //�������������
                music_close_scan();
            }
            //��ȷɨ�����
            if ((g_scan_flag & 0x81) == 1)
            {
                PRINT_DBG("scan over!");
                update_dir_list_random();//���������
                if((g_scan_flag & 0x02) != 0)
                {
                    //�ļ�ϵͳ�иĶ�,ˢ��apk�����б�
                    music_upload_music_plist();
                }
                //������Ϣ������
                music_send_msg(NULL, NULL, MSG_MENGINE_LIST_ACCESS_ALLOW_SYNC);
            }
            else if((g_scan_flag & 0x80) == 0x80)
            {
                //������
                PRINT_ERR("scan err!");
            }
            else
            {
                ;//QAC
            }
        }

        ret_vals = com_view_loop();
        if (ret_vals> RESULT_COMMON_RESERVE)
        {
            break;
        }

#ifdef SUPPORT_ASET_TEST
        if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) 
            || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
        {
            aset_test_loop_deal();
        }
#endif
        sys_os_time_dly(1);
    } //end of while

    return ret_vals;
}

MUS_STATIC app_result_e __section__(".text.BANK2") _play_key_ff(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    if (g_music_info.eg_status.play_status != PlayFast)
    {
        //���ó�ʼ����
        _play_set_ffb_step();
        if(g_music_info.eg_status.play_status == PauseSta)
        {
            music_resume(PLAY_FFON);
        }
        else
        {
            music_send_msg(NULL, NULL, MSG_MENGINE_FFWD_SYNC);
        }
        s_fast = TRUE;
        g_switch_prev = 0;
    }

    return RESULT_NULL;
}

MUS_STATIC app_result_e __section__(".text.BANK2") _play_key_fb(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    if (g_music_info.eg_status.play_status != PlayFast)
    {
        //���ó�ʼ����
        _play_set_ffb_step();
        if(g_music_info.eg_status.play_status == PauseSta)
        {
            music_resume(PLAY_FBON);
        }
        else
        {
            music_send_msg(NULL, NULL, MSG_MENGINE_FBWD_SYNC);
        }
        s_fast = TRUE;
        g_switch_prev = 1;
    }

    return RESULT_NULL;
}

MUS_STATIC app_result_e __section__(".text.BANK2") _play_stop_ffb(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    //����Ϣ���У����������İ�����Ϣ
    s_fast = FALSE;
    sys_mq_flush(MQ_ID_GUI);

    g_switch_prev = 0;

    //�����ʱȡ�������
    if (g_music_info.eg_status.play_status == PlayFast)
    {
        music_send_msg(NULL, NULL, MSG_MENGINE_CANCEL_FFB_SYNC);
        //ִ�е�����ʱ��������ܻ�û���ü�����file_switch��־�����������������һ�±�־
        music_clear_fileswitch();
        g_play_refresh_flag |= PLAY_REFRESH_STATUS;
        com_view_update(APP_VIEW_ID_MAIN);
    }

    return RESULT_NULL;
}
MUS_STATIC app_result_e __section__(".text.BANK2") _play_key_number(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    //ToDo
    //create_view_digit_song(g_p_view_ke);
    return RESULT_NULL;
}

#ifdef AB_SUPPORT
MUS_STATIC app_result_e __section__(".text.BANK2") _play_key_ab(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    com_filter_key_hold();
    return create_view_ab();
}
#endif

MUS_STATIC app_result_e __section__(".text.BANK2") _play_key_repeat(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    return create_view_loop_mode();
}

MUS_STATIC void __section__(".text.BANK2") _play_create_view(void)
{
    create_view_param_t param;

    param.type = VIEW_TYPE_MAIN;
    param.unknown_key_deal = UNKNOWN_KEY_IGNORE;
    param.overtime = 0;
    param.ke_maplist = music_play_key_map_list;
    param.se_maplist = music_play_se_map_list;
    param.view_cb = play_view_handle;
    com_view_add(APP_VIEW_ID_MAIN, &param);
}

__section__(".text.BANK2") MUS_STATIC app_result_e _play_key_mute(void)
{
    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    if (get_mute_enable() == FALSE)
    {
        //ֻ������������ʱ��������mute
        if (g_music_info.eg_status.play_status == PlaySta)
        {
            com_set_mute(TRUE);
        }
    }
    else
    {
        com_set_mute(FALSE);
    }

    return RESULT_NULL;

}

__section__(".text.BANK2") MUS_STATIC app_result_e _play_key_folderadd(void)
{
    play_mode_e play_mode;

    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    if((g_music_info.eg_status.play_status != StopSta)
            && (g_music_info.eg_status.play_status != PauseSta))
    {
        play_mode = PLAY_NORMAL;
    }
    else
    {
        play_mode = PLAY_NO_PLAY;
    }

    //�л��ļ���
    //����ͬ����Ϣ
    music_send_msg(NULL, NULL, MSG_MENGINE_FOLFER_NEXT_SYNC);
    g_switch_prev = 0;

    //��������
    music_switch_song(TTS_SONG_SEQ, play_mode);

    return RESULT_NULL;
}

MUS_STATIC __section__(".text.BANK2") app_result_e _play_key_foldersub(void)
{
    play_mode_e play_mode;

    if (g_musicplay_init_flag == TRUE)
    {
        return RESULT_NULL;
    }

    if((g_music_info.eg_status.play_status != StopSta)
            && (g_music_info.eg_status.play_status != PauseSta))
    {
        play_mode = PLAY_NORMAL;
    }
    else
    {
        play_mode = PLAY_NO_PLAY;
    }

    //�л��ļ���
    music_send_msg(NULL, NULL, MSG_MENGINE_FOLFER_PREV_SYNC);
    //Ϊ������ʹ��
    g_switch_prev = 1;

    //��������
    music_switch_song(TTS_SONG_SEQ, play_mode);

    return RESULT_NULL;
}

MUS_STATIC __section__(".text.BANK2") app_result_e _play_card_out(void *ev_param)
{
    //app_result_e ret;
    //U��ģʽ�°ο�����������
    if (g_file_path_info.file_path.dirlocation.disk == DISK_U)
    {
        return RESULT_NULL;
    }
	
    if ((g_musicplay_init_flag == TRUE)&&(g_file_path_info.file_path.dirlocation.disk == DISK_H))
    {
        return RESULT_NEXT_FUNCTION;
    }

    _play_disk_out(ev_param);

    return RESULT_NULL;
}

MUS_STATIC __section__(".text.BANK2") app_result_e _play_uhost_out(void *ev_param)
{
    //app_result_e ret;
    //��ģʽ�°�U�̣���������
    if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
    {
        return RESULT_NULL;
    }
	
    if ((g_musicplay_init_flag == TRUE)&&(g_file_path_info.file_path.dirlocation.disk == DISK_U))
    {
        return RESULT_NEXT_FUNCTION;
    }

    _play_disk_out(ev_param);

    return RESULT_NULL;
}

MUS_STATIC __section__(".text.BANK2") app_result_e _play_disk_out(void *ev_param)
{
    app_result_e ret;

    /*if ((g_music_info.eg_status.play_status != StopSta)
            && (g_music_info.eg_status.play_status != PauseSta))
    {
        //���ڲ��ţ�_play_check_status�ᴦ�����ﲻ����
        return RESULT_NULL;
    }*/

    ret = play_err_deal(EG_ERR_DISK_OUT);
    if (ret == RESULT_NEXT_FUNCTION)
    {
        //�Ѿ��Ȳ�������

        //����ֹͣ���š���Ȼ�������ʱ�ٲ��룬
        //�����ֿ�ʼ����Ȼ���˳�music,Ȼ���ֽ���music���������á�
        music_stop();
        //����Ĺ��̽���ʱ�����˳�music
        create_view_hint(EG_ERR_DISK_OUT);
    }
    else if (ret == RESULT_NULL)
    {
        music_create_song_num_view(VIEW_TYPE_MSG, FALSE, PLAY_RESUME);
    }
    else
    {
        ; //QAC
    }

    return RESULT_NULL;
}


MUS_STATIC __section__(".text.BANK20") void sound_mode_handle(void)
{
    g_neednot_song_tts_play = TRUE;
    play_key_play();
    g_neednot_song_tts_play = FALSE; 

    kill_app_timer(music_sound_mode_timer_id);
}



MUS_STATIC __section__(".text.BANK20") void update_sound_mode(void)
{
    sys_comval->dae_cfg.audiopp_type_update_flag = 0;
    sound_mode_handle();
    music_sound_mode_timer_id = set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,1000, sound_mode_handle);
}

void __section__(".text.BANK20")  update_waves_bin_para(void)
{
    g_app_info_state_all.waves_dae_para_update_flag = 0;
    sound_mode_handle();

    music_sound_mode_timer_id = set_single_shot_app_timer(APP_TIMER_ATTRB_CONTROL,1000, sound_mode_handle);
}

