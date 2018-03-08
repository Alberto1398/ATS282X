/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������첥�����̹��ܽӿ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_alarm.h"

//����������
bool alarm_open_engine(uint8 disk_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t temp_reply;
    //����Ϣ����
    msg.content.data[0] = APP_ID_MENGINE;
    if (disk_type == DISK_H)
    {
        msg.content.data[1] = (uint8) (ENTER_ALARM | ENTER_MUSIC_DISK_H);
    }
    else if (disk_type == DISK_SD)
    {
        msg.content.data[1] = (uint8) (ENTER_ALARM | ENTER_MUSIC_DISK_C);
    }
    else
    {
        msg.content.data[1] = (uint8) (ENTER_ALARM | ENTER_MUSIC_DISK_U);
    }
    //��Ϣ����(ͬ������Ӧ�ó�����Ϣ)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    bret = (bool) (send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0) == ERR_NO_ERR);
    return bret;
}

//�ر���������
bool alarm_close_engine(engine_type_e engine_type)
{
    msg_apps_t msg;
    bool bret = TRUE;

    if (engine_type != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;
        msg.content.data[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        bret = (bool) (send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0) == ERR_NO_ERR);
    }

    return bret;
}

//��������������Դ
bool alarm_set_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //���ò����ļ���·����Ϣ
    msg.content.addr = filepath;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_SET_FILEPATH_SYNC;
    //����ͬ����Ϣ
    bret = (bool) (send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0) == ERR_NO_ERR);
    return bret;
}

//���ò���ģʽ
bool alarm_set_playmode(uint8 playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //����ѭ��ģʽ&shuffleģʽ
    msg.content.addr = &playmode;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_SET_PLAYMODE_SYNC;
    //����ͬ����Ϣ
    bret = (bool) (send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0) == ERR_NO_ERR);
    return bret;
}

//��ʼ��������
bool alarm_play(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //��Ϣ����(����Ϣ����)��ʼ��������
    msg.type = MSG_MENGINE_PLAY_SYNC;
    //����ͬ����Ϣ
    bret = (bool) (send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0) == ERR_NO_ERR);
    return bret;
}

//ֹͣ���ֲ���
bool alarm_stop(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)ֹͣ��������
    msg.type = MSG_MENGINE_STOP_SYNC;
    //����ͬ����Ϣ
    bret = (bool) (send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0) == ERR_NO_ERR);

    return bret;
}

//��ȡ���ֲ���״̬
bool alarm_get_status(mengine_info_t* p_mengine_info)
{
    if (sys_share_query_read(APP_ID_MENGINE, p_mengine_info) == -1)
    {
        PRINT_ERR("music share query not exist!");
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    TOOLS Ӧ�����Ӳ���ѭ����鲥��״̬
 * \param[in]    none
 * \param[out]   none
 * \return       bool TRUE��ʾ����������FALSE��ʾ���ŷ�������
 * \ingroup
 * \note
 *******************************************************************************/
bool alarm_check_status(void)
{
    mengine_info_t tmp_music_info;

    //�ȴ�MUSIC״̬����
    while (alarm_get_status(&tmp_music_info) == FALSE)
    {
        sys_os_time_dly(1);
    }
    if (tmp_music_info.eg_status.err_status != EG_ERR_NONE)
    {
        return FALSE;
    }
    if (tmp_music_info.eg_status.play_status == StopSta) //ѭ������
    {
        alarm_play();
    }
    return TRUE;
}

void set_SDD_file_IO(void)
{
    storage_io_t io; 
    msg_apps_t msg;
    g_sdd_open_parm.file_name = g_alarm_path.file_path.dirlocation.file_info.file_name_info.file_name;
    //Ϊ0���޷첥��
    g_sdd_open_parm.loop_interval_ms = (uint8) com_get_config_default(SETTING_TIMEALARM_LOOP_PLAY);
    io.open_arg = (void*)&g_sdd_open_parm;
    io.file_type = FILE_SOURCE_SD;
    io.read = sdd_read;
    io.tell = sdd_tell;
    io.seek = sdd_seek;
    io.open = sdd_open;
    msg.type = MSG_MENGINE_SET_FIO_SYNC;
    msg.content.addr = &io;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    TOOLS Ӧ�����Ӳ��ų�ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       bool TRUE��ʾ��ʼ���ɹ���FALSE��ʾ��ʼ��ʧ��
 * \ingroup
 * \note
 *******************************************************************************/
bool alarm_play_init(alarm_ring_var_t *alarm_ring_var)
{
    alarm_alarm_t *p_alarm_alarm = &(alarm_ring_var->cur_alarm_record.alarm);
    engine_type_e engine_type;
    uint8 disk_type = DISK_SD;
    uint8 tmp_ring_type = p_alarm_alarm->ring_type;
    uint16 tmp_file_seq = (uint16) (p_alarm_alarm->sound.ring_seq);

    engine_type = get_engine_type();
    //�ر�����
    if (engine_type != ENGINE_NULL)
    {
        alarm_close_engine(engine_type);
    }

    if ((tmp_ring_type >= RING_TYPE_FOLDER1) && (tmp_ring_type <= RING_TYPE_FOLDER4))
    {
        if (g_comval.hard_support_card == 1)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1) // ���̴���
            {
                //���Կ���
                if (alarm_fsel_init(DISK_H, tmp_ring_type, tmp_file_seq) == TRUE)
                {
                    disk_type = DISK_H;
                    g_alarm_path.file_source = FSEL_TYPE_COMMONDIR;
                    goto alarm_fsel_init_ok;
                }
            }
        }

        //���̲����ڻ��������Ҳ������������������һ��
        tmp_ring_type = RING_TYPE_BUILTIN;
        tmp_file_seq = 1;
    }

    //������������
    if (tmp_ring_type == RING_TYPE_BUILTIN)
    {
        sd_handle alarm_handle;
        file_location_t *plocation;
        uint8 tmp_ring_file[12];

        com_load_share_data(COM_SHARE_DATA_RING_BUILDIN, tmp_ring_file, 12, (int) (p_alarm_alarm->sound.ring_seq - 1));

        alarm_handle = sys_sd_fopen(tmp_ring_file);
        if (alarm_handle != NULL)
        {
            g_alarm_path.file_source = FSEL_TYPE_SDFILE;
            plocation = &(g_alarm_path.file_path.dirlocation);
            plocation->disk = DISK_SD;

            libc_memcpy(plocation->file_info.file_name_info.file_name, tmp_ring_file, 12);

            sys_sd_fclose(alarm_handle);
            goto alarm_fsel_init_ok;
        }
    }

    //�ļ�ѡ��ʧ�ܣ����� FALSE
    return FALSE;

    alarm_fsel_init_ok:
    //������
    alarm_open_engine(disk_type);

    //���������ļ�·��
    alarm_set_filepath(&g_alarm_path);

    //��������
    com_set_mute(FALSE);
    com_reset_sound_volume(0);

    //����ѭ��ģʽ
    alarm_set_playmode(FSEL_MODE_LOOPONE);

    if (tmp_ring_type == RING_TYPE_BUILTIN)
    {
        set_SDD_file_IO();
    }

    //������������
    alarm_play();
    set_play_disk_type(PLAY_DISK_NULL);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    TOOLS Ӧ�����Ӳ��Źر�
 * \param[in]    none
 * \param[out]   none
 * \return       bool TRUE��ʾ�رճɹ���FALSE��ʾ�ر�ʧ��
 * \ingroup
 * \note
 *******************************************************************************/
bool alarm_play_deinit(void)
{
    engine_type_e engine_type;

    engine_type = get_engine_type();

    //if (g_alarm_mute_backup == TRUE)
    //{
    //    com_set_mute(TRUE);
    //}

    if (engine_type == ENGINE_MUSIC)
    {
        //ֹͣ����
        alarm_stop();
        //�ر�����
        alarm_close_engine(engine_type);
    }

    return TRUE;
}
