#include "app_mengine.h"

typedef enum _SPECIAL_SWITCH_MODE
{
    SWITCH_BY_NO, //���ֵ��
    SWITCH_NEXT_DIR = 0x80, //��һ��Ŀ¼
    SWITCH_PRE_DIR = 0x80, //��һ��Ŀ¼
}
SPECIAL_SWITCH_MODE;

//�л���������
static app_result_e mengine_play_switch(void *msg_ptr, switch_mode_e switch_mode)
{
    bool bret;
    //play_mode_e play_mode;//�����Ƿ����ڲ���
    switch_result_e switch_ret; //�л��Ƿ�ɹ�

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL10, FREQ_NULL);

    if (g_eg_status_p->play_status != PlaySta) //���ڲ���
    {
        //����ϵ�
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
    }

    switch_ret = mengine_file_switch(STOP_NORMAL, switch_mode, PLAY_NO_PLAY);

    if (switch_ret == SWITCH_NO_ERR)
    {
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }

    mengine_reply_msg(msg_ptr, bret);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    return RESULT_IGNORE;

}

//�л�����һ��
app_result_e mengine_play_next(void* msg_ptr)
{
    if (g_mengine_enter_mode == ENTER_RECOD_PLAY)
    {
        return mengine_play_switch(msg_ptr, NORMAL_SWITCH_NEXT);
    }
    else
    {
        return mengine_play_switch(msg_ptr, FORCE_SWITCH_NEXT);
    }
}

//�л�����һ��
app_result_e mengine_play_prev(void* msg_ptr)
{
    play_status_e play_status = g_eg_status_p->play_status;
    bool bret = TRUE;

    if ((g_music_replay_by_prev_key == TRUE) && (g_eg_playinfo_p->cur_time > 5000))
    {
        g_eg_playinfo_p->cur_time = 0;

        //����ϵ�
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));

        if (play_status == PlaySta)
        {
            //ֹͣ����
            bret = stop(STOP_NORMAL);

            //��ͷ��ʼ����
            if (bret == TRUE)
            {
                //play(PLAY_NORMAL);
            }
            g_eg_playinfo_p->cur_file_switch = 1;
        }
        if (bret == TRUE)
        {
            mengine_reply_msg(msg_ptr, bret);
            return RESULT_IGNORE;
        }
    }

    //return mengine_play_switch(msg_ptr, FORCE_SWITCH_PREV);
    if (g_mengine_enter_mode == ENTER_RECOD_PLAY)
    {
        return mengine_play_switch(msg_ptr, NORMAL_SWITCH_PREV);
    }
    else
    {
        return mengine_play_switch(msg_ptr, FORCE_SWITCH_PREV);
    }
}

//ʵ���ļ��л�, �����ļ��š�����Ŀ¼�л�
bool mengine_special_switch(uint8 direct, uint16 dest_num)
{
    bool ret;

    //ֹͣ��ǰ��������
    ret = stop(STOP_NORMAL);
    if (ret == FALSE)
    {
        return ret;
    }

    //�������Ƿ����
    ret = mengine_check_disk_in();
    //����Ѿ��γ�����
    if (ret == FALSE)
    {
        //�������״̬
        g_eg_status_p->err_status = EG_ERR_DISK_OUT;

        return ret;
    }

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL10, FREQ_NULL);

    //�Ƿ����ѡ���б�
    if ((g_list_access_allow == TRUE) && (g_mengine_enter_mode != ENTER_RECOD_PLAY))
    {
        //ֻ����һ��
        fsel_set_seltype(FSEL_TYPE_LISTDIR, fsel_get_fileno());
        g_list_access_allow = FALSE;
    }

    //ǿ���л�
    fsel_set_mode(g_shuffle_flag | FSEL_MODE_LOOPALL);

    if (direct == SWITCH_BY_NO)
    {
        //���ֵ��
        ret = fsel_get_byno((void*) g_eg_cfg_p->location.dirlocation.file_info.file_extend_info.file_ext, dest_num);
    }
    else if (direct == SWITCH_NEXT_DIR)
    {
        //Ŀ¼���
        ret = fsel_get_nextdir((void*) g_eg_cfg_p->location.dirlocation.file_info.file_extend_info.file_ext);
    }
    else
    {
        //Ŀ¼��ǰ
        ret = fsel_get_prevdir((void*) g_eg_cfg_p->location.dirlocation.file_info.file_extend_info.file_ext);
    }

    //�ָ�ѭ��ģʽ
    fsel_set_mode(g_shuffle_flag | (uint8)g_repeat_mode);

    //��ȡ��ǰ������location
    if (ret == TRUE)
    {
        fsel_get_location(&(g_eg_cfg_p->location.dirlocation));

        if (direct != 0)
        {
            g_eg_playinfo_p->cur_file_switch |= 0x01;
        }
        //����ϵ���Ϣ
        libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

#ifdef MENGINE_ID3_SUPPORT
        mengine_getid3_done(&g_eg_cfg_p->location);
#endif
#ifdef DIR_NAME_SUPPORT
        fsel_get_dir_name(g_dir_name_buf, sizeof(g_dir_name_buf) / 2 - 2, &(g_eg_cfg_p->location.dirlocation));
#endif
        vfs_get_name(g_file_sys_id, g_eg_cfg_p->name_buf,
                sizeof(g_eg_cfg_p->name_buf) / 2 - 2);
#ifdef CHECK_BK_DIR
        fsel_get_dir_name(g_eg_cfg_p->dir_name, sizeof(g_eg_cfg_p->dir_name) / 2 - 2,
                &g_eg_cfg_p->location.dirlocation);
#endif
    }
    else
    {
        libc_memset(g_eg_cfg_p->name_buf, 0, sizeof(g_eg_cfg_p->name_buf));
#ifdef CHECK_BK_DIR
        libc_memset(g_eg_cfg_p->dir_name, 0, sizeof(g_eg_cfg_p->dir_name));
#endif
    }
    if (g_mengine_enter_mode != ENTER_RECOD_PLAY)
    {
        //�����л�Ҳ��Ҫ�ϵ����,���ڶϵ�ָ�
        mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
    }

    //��ͨ�����л�ʱ��ǰʱ��������
    g_eg_playinfo_p->cur_time = 0;

    //if (direct == 0)
    //{
    //  //ֻҪ�л������ͻῪʼ����
    //  ret = play(PLAY_RESUME);
    //}

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    return ret;
}

//���ֵ��
app_result_e mengine_play_digital_song(void* msg_ptr)
{
    bool ret_val;
    //������ֵ�ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint16 song_number = *(uint16*) (data_ptr->msg.content.addr);
    ret_val = mengine_special_switch(SWITCH_BY_NO, song_number);

    //���سɹ�
    mengine_reply_msg(msg_ptr, ret_val);

    return RESULT_IGNORE;
}

//Ŀ¼�л�
app_result_e mengine_play_folder_next(void* msg_ptr)
{
    bool ret_val;

    ret_val = mengine_special_switch(SWITCH_NEXT_DIR, 1);

    //���سɹ�
    mengine_reply_msg(msg_ptr, ret_val);

    return RESULT_IGNORE;
}

//Ŀ¼�л�
app_result_e mengine_play_folder_prev(void* msg_ptr)
{
    bool ret_val;

    ret_val = mengine_special_switch(SWITCH_PRE_DIR, 1);

    //���سɹ�
    mengine_reply_msg(msg_ptr, ret_val);

    return RESULT_IGNORE;
}

