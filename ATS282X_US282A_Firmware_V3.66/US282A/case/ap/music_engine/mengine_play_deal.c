#include "app_mengine.h"

bool __section__(".BANK_CONTROL_1_8")get_music_decrypt_status(mmm_mp_fs_para_t* p_fs_para);

//���Ϊ���̲����ļ�����⿨�Ƿ����,�������u�̼��u���Ƿ����
bool mengine_check_disk_in(void)
{
    bool bret = TRUE;
    if (g_eg_cfg_p->location.dirlocation.disk == DISK_H)
    {
        if (g_card_support == 1)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
            {
                bret = FALSE;
            }
        }
        else
        {
            bret = FALSE;
        }
    }
    if (g_eg_cfg_p->location.dirlocation.disk == DISK_U)
    {
        if (g_uhost_support == 1)
        {
            if (sys_detect_disk(DRV_GROUP_STG_UHOST) == -1)
            {
                bret = FALSE;
            }
        }
        else
        {
            bret = FALSE;
        }
    }
    return bret;
}

#if(_DEBUG == 1)
static void _print_file_name(void)
{
    uint8 name_buf[32];

    if (g_open_disk == DISK_SD)
    {
        return;
    }

    PRINT_INFO_INT("�ļ�:", (uint32 )g_eg_cfg_p->location.dirlocation.file_num);
    vfs_get_name(g_file_sys_id, name_buf, sizeof(name_buf) / 2 - 2);
    PRINT_DATA(name_buf, sizeof(name_buf));
}
#endif

//���ò����ļ���������
bool set_file(void)
{
    int result, m_type, level_a;
    mmm_mp_fs_para_t set_file_param;
    bool music_decrypt_status  = FALSE;
    
    if (g_fsel_type != FSEL_TYPE_SDFILE)
    {
        //���ȼ������Ƿ����
        if (mengine_check_disk_in() == FALSE)
        {
            //�������״̬
            g_eg_status_p->err_status = EG_ERR_DISK_OUT;
            return FALSE;
        }
    }

    //�ļ�ϵͳ����id
    if (g_fsel_type == FSEL_TYPE_SDFILE)
    {
        //sd file
        set_file_param.fs_type = FILE_SOURCE_SD;
        set_file_param.file_name = (char *) g_file_name;
    }
    else
    {
        //normal file
        set_file_param.fs_type = FILE_SOURCE_NORMAL;
        set_file_param.file_name = NULL;
        //��������
        if (g_file_sys_id < 0)
        {
            return FALSE;
        }
    }

    //�̷�
    set_file_param.file_mount_id = (void *) g_file_sys_id;

#if(_DEBUG == 1)
    _print_file_name();
#endif
    if(g_fsel_type != FSEL_TYPE_SDFILE)
    {
        music_decrypt_status = get_music_decrypt_status(&set_file_param);
    }
    
    //�Ƿ�֧�������ļ����� ֧��ΪTRUE ��֧��ΪFALSE
    mmm_mp_cmd(g_mp_handle, MMM_MP_DECRYPT_STATUS, music_decrypt_status);
    //�����ܺ����ĵ�ַ���ݵ��м��
    mmm_mp_cmd(g_mp_handle, MMM_MP_SET_DECRYPT_FUNC, m_decrypt_func);
    
    //����set file����
    result = mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FILE, (unsigned int) &set_file_param);

    if (result != 0)
    {
        mengine_save_errno_no();

        if (g_file_sys_id != -1)

        {
            //��ȡ�ļ�����(��֤δɨ�����ʱ��ȡ����ȷ���ļ�����)
            g_eg_cfg_p->location.dirlocation.file_total = fsel_get_total();
        }

        return FALSE;
    }
    //��ȡ�ļ���ʽ����ʱ�䡢�����ʵ���Ϣ
    result = mmm_mp_cmd(g_mp_handle, MMM_MP_MEDIA_INFO, (unsigned int) &g_eg_file_info);
    if (result != 0)
    {
        //need stop
        error_handle();
        return FALSE;
    }

    m_type = g_eg_file_info.music_type;
    PRINT_DBG_INT("music_type:", m_type);

    //their add dae check
    if (m_type == MP3_TYPE)
    {
        m_type = FREQ_LEVEL2; //
        level_a = FREQ_LEVEL8;
    }
    else if (m_type == WAV_TYPE)
    {
        m_type = FREQ_LEVEL6; //
        level_a = FREQ_LEVEL7;
    }
    else if ((m_type == APE_TYPE) || (m_type == FLAC_TYPE))
    {
        m_type = FREQ_LEVEL10; //
        level_a = FREQ_LEVEL9;
    }
    else if (m_type == WMA_TYPE)
    {
        m_type = FREQ_LEVEL6; //
        level_a = FREQ_LEVEL7;
    }
    else
    {
        m_type = FREQ_LEVEL10; //
        level_a = FREQ_LEVEL9;
    }

    adjust_freq_set_level(AP_BACK_HIGH_PRIO, level_a, m_type);

    return TRUE;

}

//��ʼ��������
bool play(play_mode_e play_mode)
{
    int result;
    bool bret = TRUE;

    //ֹͣ or ��ͣʱ ���Ͳ�������
    if ((g_eg_status_p->play_status != StopSta) && (g_eg_status_p->play_status != PauseSta))
    {
        return bret;
    }

    bret = set_file();
    if (bret == FALSE)
    {
        return bret;
    }
    
#ifdef WAVES_ASET_TOOLS
    g_waves.bin_number = g_app_info_state_all.bin_number;
#endif

    //set sound open
    com_set_sound_out(SOUND_OUT_RESUME);

    adjust_freq_add_value(0, g_dae_cfg_shm->run_freq);

    //�����������ѡ��
    //#define OUTPUT_CHANNEL_NORMAL      0 //�������м�����ʼ��Ϊ����
    //#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
    //#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
    //#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
    //#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0
    
    mmm_mp_cmd(g_mp_handle, MMM_MP_SET_OUTPUT_CHANNEL, g_output_channel_config);

    if (play_mode == PLAY_FFON)
    {
        if (g_eg_cfg_p->bk_infor.bp_time_offset != 0)
        {
            mmm_mp_cmd(g_mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int) &g_eg_cfg_p->bk_infor);
        }
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) 4);
        //�������
#ifdef WAVES_ASET_TOOLS
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) &g_waves);
#else
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) NULL);
#endif
        

        g_eg_status_p->play_status = PlayFast; //���ò���״̬
        g_eg_status_p->fast_status = FFPlaySta;
    }
    else if (play_mode == PLAY_FBON)
    {
        //���˲���
        if (g_eg_cfg_p->bk_infor.bp_time_offset == 0)
        {
            //mmm_mp_cmd(g_mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int) &g_eg_cfg_p->bk_infor);
            //}
            //else
            //{
            g_eg_cfg_p->bk_infor.bp_time_offset = -5000;
            g_eg_cfg_p->bk_infor.bp_file_offset = 0;
            g_eg_cfg_p->bk_infor.bp_info_ext = 0;
        }
        mmm_mp_cmd(g_mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int) &g_eg_cfg_p->bk_infor);

        result = mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) -4);
#ifdef WAVES_ASET_TOOLS
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) &g_waves);
#else
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) NULL);
#endif

        g_eg_status_p->play_status = PlayFast; //���ò���״̬
        g_eg_status_p->fast_status = FBPlaySta;
    }
    else if (play_mode == PLAY_RESUME)
    {
        //�ϵ�����,���öϵ�
        if (g_eg_cfg_p->bk_infor.bp_time_offset != 0)
        {
            mmm_mp_cmd(g_mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int) &g_eg_cfg_p->bk_infor);
        }
#ifdef WAVES_ASET_TOOLS
            result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) &g_waves);
#else
            result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) NULL);
#endif

        g_eg_status_p->play_status = PlaySta; //���ò���״̬
    }
    else
    {
        g_eg_status_p->play_status = PlaySta; //���ò���״̬
#ifdef WAVES_ASET_TOOLS
            result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) &g_waves);
#else
            result = mmm_mp_cmd(g_mp_handle, MMM_MP_PLAY, (unsigned int) NULL);
#endif

    }

    if (result != 0)
    {
        bret = FALSE;
        //mengine_save_errno_no();
        error_handle();
    }
    else
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_GET_ENERGY_INFO, (unsigned int) &(g_app_info_state_all.p_energy_value));
        g_app_info_state_all.energy_available_flag = TRUE;

        bret = TRUE;
    }

    return bret;
}

//ֹͣ��������
bool stop(stop_mode_e stop_mode)
{
    bool bret = TRUE;
    int result = 0;

    g_app_info_state_all.energy_available_flag = FALSE;

    //ȡ�������
    //    if (g_eg_status_p->play_status == PlayFast)
    //    {
    //        mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);
    //        g_eg_status_p->play_status = PlaySta;
    //    }
#ifdef AB_SUPPORT
    //    else if (g_eg_status_p->play_status == PlayAB)
    //    {
    //        if (g_eg_status_p->ab_status == PlayAB_Do)
    //        {
    //            mmm_mp_cmd(g_mp_handle, MMM_MP_CLEAR_AB, (unsigned int) NULL);
    //        }
    //        g_eg_status_p->play_status = PlaySta;
    //        g_eg_status_p->ab_status = PlayAB_Null;
    //    }
    //    else
    //    {
    //        ; //for qac
    //    }
    //ȡ��ab repeat
    if (g_eg_status_p->play_status == PlayAB)
    {
        if (g_eg_status_p->ab_status == PlayAB_Do)
        {
            mmm_mp_cmd(g_mp_handle, MMM_MP_CLEAR_AB, (unsigned int) NULL);
        }
        g_eg_status_p->play_status = PlaySta;
        g_eg_status_p->ab_status = PlayAB_Null;
    }

#endif
    //�������ŲŻ�ֹͣ
    if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
    {
        //����ϵ�
        mmm_mp_cmd(g_mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int) &(g_eg_cfg_p->bk_infor));
        //if (g_save_freq != 0)
        //{
        //}
        //ֹͣ����
#ifdef WAVES_ASET_TOOLS
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_STOP, (unsigned int) &g_waves);
#else      
        result = mmm_mp_cmd(g_mp_handle, MMM_MP_STOP, (unsigned int) NULL);
#endif
       
        if (g_eg_status_p->play_status == PlayFast)
        {
            mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);
        }
        if (result != 0)
        {
            bret = FALSE;
        }
        else
        {
            //��ͣ����
            if (stop_mode == STOP_PAUSE)
            {
                g_eg_status_p->play_status = PauseSta;
            }
            else
            {
                //����ֹͣ
                g_eg_status_p->play_status = StopSta;
            }
            bret = TRUE;
        }

        if (bret == FALSE)
        {
            mengine_save_errno_no();
        }

        //for dae adjust freq
        adjust_freq_add_value(0, 0);

        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL2, FREQ_NULL);

        //�ر���Ƶ���
        com_set_sound_out(SOUND_OUT_PAUSE);
    }

    //ֹͣ�ر��ļ�
    mmm_mp_cmd(g_mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);

    fsel_set_mode(g_shuffle_flag | (uint8) g_repeat_mode); //����ѭ��ģʽ
    return bret;
}

//�л�������
bool next_or_prev(switch_mode_e switch_mode)
{
    bool bret = TRUE;
    uint8 direct = (uint8) (switch_mode & 0x01);
    uint8 force_mod = (uint8) ((switch_mode & 0x02) >> 1);

    g_eg_status_p->err_status = EG_ERR_NONE;

    if (force_mod == TRUE)
    {
        //ǿ���л�
        //��ѭ��ģʽ����ΪFSEL_MODE_LOOPALL
        fsel_set_mode(g_shuffle_flag | FSEL_MODE_LOOPALL);
    }

    if (g_fsel_type == FSEL_TYPE_LISTDIR)
    {
        //check if need to set just only one not the same
        if (fsel_get_fileno() != g_eg_cfg_p->location.dirlocation.file_num)
        {
            fsel_set_fileno(g_eg_cfg_p->location.dirlocation.file_num);
        }
    }

    //�Ƿ����ѡ���б�
    if (g_list_access_allow == TRUE)
    {
        if (fsel_set_seltype(FSEL_TYPE_LISTDIR, fsel_get_fileno()) == TRUE)
        {
            //ֻ����һ��
            g_fsel_type = FSEL_TYPE_LISTDIR;
            g_list_access_allow = FALSE;
        }
    }

    if (direct == NORMAL_SWITCH_NEXT)
    {
        //��ȡ��һ��
        bret = fsel_get_nextfile((void*) g_eg_cfg_p->location.dirlocation.file_info.file_extend_info.file_ext);
    }
    else
    {
        //��ȡ��һ��
        bret = fsel_get_prevfile((void*) g_eg_cfg_p->location.dirlocation.file_info.file_extend_info.file_ext);
    }

    //��ȡ��ǰ������location
    if (bret == TRUE)
    {
        fsel_get_location(&(g_eg_cfg_p->location.dirlocation));

        g_eg_playinfo_p->cur_file_switch |= 0x01;

        //��ͨ�����л�ʱ��ǰʱ��������
        g_eg_playinfo_p->cur_time = 0;
#ifdef MENGINE_ID3_SUPPORT
        mengine_getid3_done(&g_eg_cfg_p->location);
#endif
#ifdef DIR_NAME_SUPPORT
        fsel_get_dir_name(g_dir_name_buf, sizeof(g_dir_name_buf) / 2 - 2, &(g_eg_cfg_p->location.dirlocation));
#endif
    }

    if (force_mod == TRUE)
    {
        //�ָ�ѭ��ģʽ
        fsel_set_mode(g_shuffle_flag | (uint8) g_repeat_mode);
    }

    //����ϵ���Ϣ
    libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

    return bret;

}

switch_result_e mengine_file_switch(stop_mode_e stop_mode, switch_mode_e switch_mode, play_mode_e play_mode)
{
    bool ret;

    uint8 read_err;
    switch_result_e switch_result = SWITCH_NO_ERR;

    //ֹͣ��ǰ��������
    ret = stop(stop_mode);
    if (ret == FALSE)
    {
        switch_result = SWITCH_ERR_STOP;
        goto switch_end;
    }

    if (g_file_sys_id < 0)
    {
        return SWITCH_NO_ERR;
    }
    //�������Ƿ����
    ret = mengine_check_disk_in();
    //�Ƿ��Ƕ��ļ�����
    read_err = fsel_get_error();

    if ((ret == FALSE) || (read_err == 2))
    {
        PRINT_DBG_INT("file_switch:", (((uint32) ret) << 8) | read_err);
        switch_result = SWITCH_ERR_DISK_OUT;
        //�������״̬
        g_eg_status_p->err_status = EG_ERR_DISK_OUT;
        goto switch_end;
    }

    //�л�������
    ret = next_or_prev(switch_mode);
    if ((ret == FALSE) && (g_mengine_enter_mode != ENTER_RECOD_PLAY))
    {
        if (play_mode == PLAY_FBON)
        {
            //ǿ���л�����һ�׸���
            ret = next_or_prev(FORCE_SWITCH_PREV);
        }
        else
        {
            //ǿ���л�����һ�׸���
            ret = next_or_prev(FORCE_SWITCH_NEXT);
        }
    }

    if (FALSE == ret)
    {
         //���ٲ�ο���ʱ��next_or_prev�����л���Ҫ�����ļ�ϵͳ�ٴζ�����ʱ����Ϊ������ʱ����������ݶ����ش���
        switch_result = SWITCH_ERR_READ_CARD_TIMEOUT;
        goto switch_end;
    }
    
    if (g_mengine_enter_mode != ENTER_RECOD_PLAY)
    {
        vfs_get_name(g_file_sys_id, g_eg_cfg_p->name_buf, sizeof(g_eg_cfg_p->name_buf) / 2 - 2);
#ifdef CHECK_BK_DIR
        fsel_get_dir_name(g_eg_cfg_p->dir_name, sizeof(g_eg_cfg_p->dir_name) / 2 - 2,
                &g_eg_cfg_p->location.dirlocation);
#endif

        //�����л�Ҳ��Ҫ�ϵ����,���ڶϵ�ָ�
        mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
    }

    if ((play_mode != PLAY_NO_PLAY) && (ret == TRUE))
    {
        //�л�������ʼ����
        ret = play(play_mode);
    }

    if (ret == FALSE)
    {
        switch_result = SWITCH_ERR_PLAY;
    }
    switch_end:

    return switch_result;
}

//�ж������ļ�����demo�����������и��������ļ�β��4���ֽ������Ƿ�Ϊ0x7f���жϵ�
//�ͻ�������Լ��ж������ļ��Ƿ���ܣ������޸ĸú�������
//����ֵΪTRUE��ʾ�����ļ��Ǽ��ܵ�;FALSE��ʾ�����ļ��ǷǼ��ܵ�
bool __section__(".BANK_CONTROL_1_8")get_music_decrypt_status(mmm_mp_fs_para_t* p_fs_para)
{
    uint32 cur_file_handle;
    uint8 encryption_data[4];
    uint32 file_len = 0;
    bool music_decrypt_status  = FALSE;
    
    cur_file_handle = (void *) vfs_file_open(g_file_sys_id, p_fs_para->file_name, R_NORMAL_SEEK);
    vfs_file_get_size(g_file_sys_id, &file_len,p_fs_para->file_name,1);
    
    vfs_file_seek(g_file_sys_id,  file_len - 4,SEEK_SET, cur_file_handle);
    vfs_file_read(g_file_sys_id,encryption_data,4,cur_file_handle);

    vfs_file_close(g_file_sys_id, cur_file_handle);

    if ((0x7f == encryption_data[0])\
        &&(0x7f == encryption_data[1])\
        &&(0x7f == encryption_data[2])\
        &&(0x7f == encryption_data[3]))
    {
        music_decrypt_status = TRUE;
    }
     
    return music_decrypt_status;
}
