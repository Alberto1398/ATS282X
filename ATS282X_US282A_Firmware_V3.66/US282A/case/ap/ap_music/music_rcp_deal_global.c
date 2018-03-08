#include "music.h"

app_result_e music_get_global_rcp_info(uint32 data1, uint32 data2)
{
    uint8 tmp_music_global_rcp_info[sizeof(music_global_rcp_info_t) + RCP_COMMAND_HEAD_SIZE];
    music_global_rcp_info_t *p_music_global_rcp_info = (music_global_rcp_info_t*) (tmp_music_global_rcp_info
            + RCP_COMMAND_HEAD_SIZE);
    uint8 tmp_update_flag = 0;

    if((g_rcp_file_status.file_switch == 0)
        && (g_rcp_file_status.file_seq[0] !=
        g_file_path_info.file_path.dirlocation.file_num))
    {
        g_rcp_file_status.file_seq[0] =
            g_file_path_info.file_path.dirlocation.file_num;
        g_rcp_file_status.file_switch = 1;
    }

    if ((eh_vfs_mount != -1) && (g_rcp_file_status.file_switch == 1))
    {
        //ͬ�����¸��״̬����Ŀ��
        g_rcp_file_status.lyric_exist = music_is_has_lyric();
    }

    libc_memset(p_music_global_rcp_info, 0, sizeof(music_global_rcp_info_t));

    //��ȡ������Ϣ
    com_get_com_info(&(p_music_global_rcp_info->com_info));
#if (SUPPORT_SLAVE_CONTROL == 0)
    //p_music_global_rcp_info->com_info.config_flag = g_scan_status;//�ļ��Ƿ�ɨ�����
    p_music_global_rcp_info->com_info.config_flag = g_rcp_file_status.scan_status;
#endif
    if (g_file_path_info.file_path.dirlocation.disk == DISK_U)
    {
        if ((g_app_info_state.need_update_playlist & UHOST_NEED_UPLOAD) != 0)
        {
            tmp_update_flag = 1;
        }
    }
    else
    {
        if ((g_app_info_state.need_update_playlist & CARD_NEED_UPLOAD) != 0)
        {
            tmp_update_flag = 1;
        }
    }
    //������U�̲����ڣ�����Ҫ����
    if((g_app_info_state.card_state==0) && (g_app_info_state.uhost_state==0))
    {
        tmp_update_flag = 0;
    }
    if(g_scan_flag == 0)
    {
        tmp_update_flag = 0;
//        libc_print("scan ing",0,0);
    }    
//    libc_print("update flag",tmp_update_flag,2);
    //0��ʾ����Ҫ���£�1��ʾǿ�Ƹ��£�2��ʾ¼�����£�3��ʾ�������
    p_music_global_rcp_info->com_info.app_argv = tmp_update_flag;

    //��ȡAPP��Ϣ
    p_music_global_rcp_info->app_info.repeat_mode = g_music_config.repeat_mode;
    p_music_global_rcp_info->app_info.play_status = (g_music_info.eg_status.play_status >= PlaySta ? PlaySta : PauseSta);
    p_music_global_rcp_info->app_info.ab_status = g_music_info.eg_status.ab_status;
    p_music_global_rcp_info->app_info.fast_status = g_music_info.eg_status.fast_status;
    p_music_global_rcp_info->app_info.err_status = g_music_info.eg_status.err_status;

    p_music_global_rcp_info->app_info.lyric = g_rcp_file_status.lyric_exist;
    p_music_global_rcp_info->app_info.cur_time = g_music_info.eg_playinfo.cur_time;
    p_music_global_rcp_info->app_info.total_time = (uint32) g_music_fileinfo.total_time;

    if(g_rcp_file_status.file_switch == 0)
    {
        p_music_global_rcp_info->app_info.file_switch = FALSE;
        //ֻ��С�����ڲ���״̬��ʱ�򣬲Ÿ�����Ŀ�ţ������и�APK��ʾ������ʱ�䲻��
        //if (PlaySta == g_music_info.eg_status.play_status)
        //{
        p_music_global_rcp_info->app_info.file_seq = g_file_path_info.file_path.dirlocation.file_num;
        //}
    }
    else
    {
        p_music_global_rcp_info->app_info.file_switch = TRUE;
        //ֻ��С�����ڲ���״̬��ʱ�򣬲Ÿ�����Ŀ�ţ������и�APK��ʾ������ʱ�䲻��
        //if (PlaySta == g_music_info.eg_status.play_status)
        //{
        p_music_global_rcp_info->app_info.file_seq = g_rcp_file_status.file_seq[0];
        //}
        if(g_rcp_file_status.file_switch == 2)
        {
            g_rcp_file_status.file_seq[0] = g_rcp_file_status.file_seq[1];
        }
        g_rcp_file_status.file_switch--;
    }

    p_music_global_rcp_info->app_info.file_total = g_vmfile_total;
//    libc_print("globe file num",g_vmfile_total,2);

    com_rcp_send_pkg(0, 0, tmp_music_global_rcp_info, sizeof(music_global_rcp_info_t),
        sizeof(music_global_rcp_info_t));
    //������ִ��ʱ�䣺HFP����ʱ��42ms, һ�������:16ms

    return RESULT_NULL;
}
