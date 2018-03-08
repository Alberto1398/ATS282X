#include "app_mengine.h"

//bank��������ʱ��һ��
static mengine_config_t s_cfg _BANK_DATA_ATTR_;

void mengine_vram_write(uint8 save_disk)
{
    //дvram
    sys_vm_write(g_eg_cfg_p,
            (save_disk == DISK_H) ? VM_CARD_BKINFO : VM_UHOST_BKINFO,
            sizeof(*g_eg_cfg_p));
}

//ʼ���ļ�ѡ��������λ���ϴ��˳����ļ�
bool switch_disk_file_init(void)
{
    //�ļ�ѡ������ʼ������
    fsel_param_t switch_init_param;
    //��׺��buffer
    uint8 exname[4];
    //�ļ�ѡ������
    //fsel_type_e file_source;
    //·����Ϣָ��
    file_location_t* loact_ptr = &g_eg_cfg_p->location.dirlocation;
    //��ʼ���ļ�ѡ�������
    bool init_result = FALSE;

    //���ʴ��ڱ�־
    int8 uhost_exist = -1;
    int8 card_exist = -1;

    g_file_sys_id = -1;
    //�����ڷ�
    //��ֹ�����б�
    g_list_access_allow = FALSE;

    if (g_card_support == 1)
    {
        //�����ڷ�
        card_exist = (int8) sys_detect_disk(DRV_GROUP_STG_CARD);
    }
    if (g_uhost_support == 1)
    {
        //u�̴��ڷ�
        uhost_exist = (int8) sys_detect_disk(DRV_GROUP_STG_UHOST);
    }

    //����u�̶�������
    if ((card_exist == -1) && (uhost_exist == -1))
    {
        goto switch_init_over;
    }
    if ((loact_ptr->disk == DISK_U) && (uhost_exist == -1))
    {
        //�ο�����,��u�̴���,���л���u�̲���
        goto switch_init_over;
    }
    if ((loact_ptr->disk == DISK_H) && (card_exist == -1))
    {
        //�ο�����,��u�̴���,���л���������
        goto switch_init_over;
    }

    //�ļ�ѡ������ʼ���ӿڵĲ�������

    //�̷���Ϣ
    switch_init_param.disk = loact_ptr->disk;

    switch_init_param.fsel_type = g_fsel_type;

    //ѭ��ģʽ
    switch_init_param.fsel_mode = FSEL_MODE_LOOPALL;

    //bitmap ����
    switch_init_param.file_type_bit = MUSIC_BITMAP;

    //�ļ�ѡ������ʼ��
    g_file_sys_id = fsel_init(&switch_init_param, 0);
    //��ʼ���ɹ�
    if (g_file_sys_id != -1)
    {
        //�����ϴ��˳����ļ�
        init_result = fsel_set_location(loact_ptr);
        if (init_result == TRUE)
        {
            //�ȽϺ�׺��
            vfs_get_name(g_file_sys_id, exname, 0);
            if (libc_strncmp(exname, loact_ptr->file_info.file_extend_info.file_ext, sizeof(exname)) != 0)
            {
                init_result = FALSE;
            }
        }

        //����ʧ��
        if (init_result == FALSE)
        {
            //����ϵ���Ϣ
            libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

            //��ȡ��ǰģʽ�µĵ�һ���ɲ����ļ�
            init_result = fsel_get_nextfile(loact_ptr->file_info.file_extend_info.file_ext);
            //�ҵ��ɲ����ļ�
            if (init_result == TRUE)
            {
                //��ȡ��ǰ�ļ���location��Ϣ
                init_result = fsel_get_location(loact_ptr);
            }
        }
#ifdef MENGINE_ID3_SUPPORT
        if(init_result == TRUE)
        {
            mengine_getid3_done(&g_eg_cfg_p->location);
        }
#endif
#ifdef DIR_NAME_SUPPORT
        if(init_result == TRUE)
        {
            fsel_get_dir_name(g_dir_name_buf, sizeof(g_dir_name_buf) / 2 - 2, &(g_eg_cfg_p->location.dirlocation));
        }
#endif
    }

    switch_init_over:
    if (init_result == FALSE)
    {
        //û�пɲ����ļ�
        //loact_ptr->file_total = 0;

        //����״̬Ϊ�������ļ�
        g_eg_status_p->err_status = EG_ERR_NO_FILE;
    }
    return init_result;
}

//�л�����
app_result_e mengine_switch_disk(void* msg_ptr)
{
    //���Ŀ������̷�������ָ��
    private_msg_t* data_ptr = (private_msg_t *) msg_ptr;
    uint8 dest_disk;
    bool ret_val;
    uint8 temp_buf[4];

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL10, FREQ_NULL);

    temp_buf[0] = 'W';
    temp_buf[1] = 'A';
    temp_buf[2] = 'V';
    temp_buf[3] = 0;

    g_eg_status_p->err_status = EG_ERR_NONE;

    //ֹͣ��ǰ��������
    stop(STOP_NORMAL);
    if (g_file_sys_id != -1)
    {
        vfs_get_name(g_file_sys_id, g_eg_cfg_p->name_buf,
                sizeof(g_eg_cfg_p->name_buf) / 2 - 2);
#ifdef CHECK_BK_DIR
        fsel_get_dir_name(g_eg_cfg_p->dir_name, sizeof(g_eg_cfg_p->dir_name) / 2 - 2,
                &g_eg_cfg_p->location.dirlocation);
#endif

        //�ر��ļ�ѡ����
        fsel_exit();
    }

    //��ֹ�����б�
    //g_list_access_allow = FALSE;

    //��ȡĿ������̷�
    dest_disk = *(uint8*) (data_ptr->msg.content.addr);
    //����ϵ�
    mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
    sys_vm_read(&s_cfg,
            (dest_disk == DISK_H) ? VM_CARD_BKINFO : VM_UHOST_BKINFO,
            sizeof(s_cfg));
    //vram�ϵ�ֵ
    libc_memcpy(&g_eg_cfg_p->location, &s_cfg.location, sizeof(mengine_bkinfo_t));

    g_fsel_type = FSEL_TYPE_COMMONDIR;

    g_eg_cfg_p->location.dirlocation.disk = dest_disk;

    ret_val = switch_disk_file_init();

    if (ret_val == TRUE)
    {
        if (g_eg_cfg_p->bk_infor.bp_time_offset != 0)
        {
            //�޸Ĳ�δ���ʱwav���ļ�λ����Ϣ�����¶ϵ��޷����ŵ�����
            if (libc_strncmp(g_eg_cfg_p->location.dirlocation.file_info.file_extend_info.file_ext, temp_buf, 4) == 0)
            {
                //����ļ�λ����Ϣ
                g_eg_cfg_p->bk_infor.bp_file_offset = 0;
            }
            ret_val = play(PLAY_RESUME); //����(play�л��Ƚ���setfile)
        }
        else
        {
            ret_val = play(PLAY_NORMAL); //����(play�л��Ƚ���setfile)
        }
        //���ų���
        if (ret_val == FALSE)
        {
            error_handle();
        }
    }
    //����
    mengine_reply_msg(msg_ptr, ret_val);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    return RESULT_IGNORE;
}

//�����б��ش�
app_result_e mengine_list_access_allow(void* msg_ptr)
{
    g_list_access_allow = TRUE;
    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);
    return RESULT_IGNORE;
}

//����ѭ��ģʽ
app_result_e mengine_set_playmode(void* msg_ptr)
{
    //����ѭ��ģʽ��ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint8 cur_mode;

    //��ȡ��ǰѭ��ģʽ
    cur_mode = *(uint8*) (data_ptr->msg.content.addr);

    if (cur_mode == FSEL_MODE_RANOM)
    {
        g_shuffle_flag = SHUFFLE;
        g_repeat_mode = FSEL_MODE_LOOPALL;
    }
    else
    {
        g_shuffle_flag = SHUFFLE_NONE;
        g_repeat_mode = cur_mode;
    }

    //��ͣ����ֹͣʱ����ѭ��ģʽ
    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        fsel_set_mode(cur_mode); //����ѭ��ģʽ
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//���ò����ļ�·����Ϣ
app_result_e mengine_set_filepath(void* msg_ptr)
{
    bool ret_vals;
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //·����Ϣָ��
    file_path_info_t *locat_info = (file_path_info_t*) data_ptr->msg.content.addr;
    //�̷�
    uint8 disk_old = g_eg_cfg_p->location.dirlocation.disk;
    uint8 disk_new = locat_info->file_path.dirlocation.disk;

    fsel_type_e cur_type = locat_info->file_source;
    ret_vals = stop(STOP_NORMAL); //ֹͣ����

    if (cur_type == FSEL_TYPE_SDFILE)
    {
        libc_memcpy(g_file_name, locat_info->file_path.dirlocation.file_info.file_extend_info.file_ext, 12);
        ret_vals = TRUE;
        g_mengine_enter_mode = ENTER_ALARM;
    }
    else
    {
        //¼���ļ�����ʱ�����������֧
        //(�ղ��Ǵ��̲��ţ���ʱ�ǻ���)
        if ((disk_new != disk_old) && (g_mengine_enter_mode == ENTER_MUSIC_START))
        {
            mengine_vram_write(disk_old);
        }
        //alarm ���������˴��̲���
        if (g_mengine_enter_mode == ENTER_ALARM)
        {
            libc_memcpy(&g_eg_cfg_p->location, &locat_info->file_path, sizeof(file_location_t));
        }
        else
        {
            sys_vm_read(&s_cfg,
                    (disk_new == DISK_H) ? VM_CARD_BKINFO : VM_UHOST_BKINFO,
                    sizeof(s_cfg));

            //�����ļ�ѡ��
            //��תΪmusic����,���ֲ���ʱ����תΪalarm
            g_mengine_enter_mode = ENTER_MUSIC_START;
            //{
            //vram�ϵ�ֵ
            libc_memcpy(&g_eg_cfg_p->location, &s_cfg.location, sizeof(mengine_bkinfo_t));
        }

        g_fsel_type = cur_type; //�������汣����ļ�ѡ������
        g_eg_cfg_p->location.dirlocation.disk = disk_new;
        //�ر��ļ�ѡ����
        fsel_exit();

        //���³�ʼ���ļ�ѡ����
        ret_vals = switch_disk_file_init();
        if (ret_vals == TRUE)
        {
            g_eg_playinfo_p->cur_file_switch |= 0x01;
        }
    }
    g_fsel_type = cur_type; //�������汣����ļ�ѡ������
    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//��ȡ���Ž���
app_result_e mengine_get_playtype(void* msg_ptr)
{
    //���ò������͵�ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint8 cur_type;

    if (g_mengine_enter_mode == ENTER_RECOD_PLAY)
    {
        cur_type = PLAY_RECORD_FILE;
    }
    else
    {
        //��ͨ���ź�sd����
        cur_type = (0 << 7); //�������ͷ��� bit[7]
        if (g_eg_cfg_p->location.dirlocation.disk == DISK_H)
        {
            //cur_type |= (2 << 5);//SDCARD���ʣ�bit[5]
            cur_type = PLAY_DISK_CARD;
        }
        else
        {
            //cur_type |= (1 << 5);//UHOST���ʣ�bit[5]
            cur_type = PLAY_DISK_UHOST;
        }
    }

    //��ȡ��ǰ��������
    *(uint8*) (data_ptr->reply->content) = cur_type;

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

app_result_e mengine_clear_fileswitch(void* msg_ptr)
{
    g_eg_playinfo_p->cur_file_switch &= (uint8) (0xfe);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//��ȡ�ļ���Ϣ�������ļ���ʱ�䣬�����ʣ������ʵ�
app_result_e mengine_get_fileinfo(void* msg_ptr)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //copy��ǰ�ļ���Ϣ����Ϣָ��ָ���������
    libc_memcpy((data_ptr->reply->content), &g_eg_file_info, sizeof(mmm_mp_file_info_t));

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//�����ļ����
app_result_e mengine_set_sortnumber(void *msg_ptr)
{
    //��Ϣָ��
    private_msg_t *data_ptr = (private_msg_t *) msg_ptr;

    //param
    uint16 set_num;

    set_num = *(uint16 *) (data_ptr->msg.content.addr);

    //change cur number
    g_eg_cfg_p->location.dirlocation.file_num = set_num;

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//��ȡ��ǰ�ļ����
app_result_e mengine_get_disknumber(void *msg_ptr)
{
    //��Ϣָ��
    private_msg_t *data_ptr = (private_msg_t *) msg_ptr;

    *(uint16 *) (data_ptr->reply->content) = fsel_get_fileno();

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
