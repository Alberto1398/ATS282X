#include "app_mengine.h"
#include <analog.h>

void reset_by_PW_off(void);


//������Ϣ����,����������յ���˽����Ϣ��ϵͳ��Ϣ
app_result_e mengine_message_done_bank(private_msg_t* cur_msg)
{
    g_mengine_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        case MSG_MENGINE_GET_PLAYMODE_SYNC: //��ȡѭ��ģʽ+shuffle
        mengine_get_playmode(cur_msg);
        break;

        case MSG_MENGINE_GET_FILEPATH_SYNC: //��ȡ�ļ�location��Ϣ
        mengine_get_filepath(cur_msg);
        break;

        /*���ò������*/
        case MSG_SET_DAE_CFG_SYNC: //����eq +eq curve data
        mengine_set_eq(cur_msg);
        break;

        case MSG_MENGINE_SET_PLAYMODE_SYNC: //����ѭ��ģʽ+shuffle
        mengine_set_playmode(cur_msg);
        break;
#ifdef AB_SUPPORT
        /*AB�������*/
        case MSG_MENGINE_SET_AB_COUNT_SYNC: //����AB��������
        mengine_set_ab_count(cur_msg);
        break;

        case MSG_MENGINE_SET_AB_GAP_SYNC: //����AB�������
        mengine_set_ab_gap(cur_msg);
        break;

        case MSG_MENGINE_START_AB_SYNC: //����ab�������ȴ�����a��
        mengine_start_ab(cur_msg);
        break;

        case MSG_MENGINE_SET_APOINT_SYNC: //����a��
        mengine_set_apoint(cur_msg);
        break;

        case MSG_MENGINE_SET_BPOINT_SYNC: //����b�㣬����ʼ����
        mengine_set_bpoint(cur_msg);
        break;

        case MSG_MENGINE_CLEAR_AB_SYNC: //���ab����
        mengine_clear_ab(cur_msg);
        break;
#endif //#ifdef AB_SUPPORT
        /*�����������*/
        case MSG_MENGINE_PLAY_SYNC: //��ʼ��������
        mengine_play(cur_msg);
        break;

        case MSG_MENGINE_STOP_SYNC: //ֹͣ��������
        mengine_stop(cur_msg);
        break;

        case MSG_MENGINE_PAUSE_SYNC: //��ͣ��������
        mengine_pause(cur_msg);
        break;

        case MSG_MENGINE_RESUME_SYNC: //�ָ���������(����ͣ������)
        mengine_resume(cur_msg);
        break;

        /*��������*/
        case MSG_MENGINE_FFWD_SYNC:
        mengine_fast_forward(cur_msg); //���
        break;

        case MSG_MENGINE_FBWD_SYNC:
        mengine_fast_backward(cur_msg); //����
        break;

        case MSG_MENGINE_CANCEL_FFB_SYNC:
        mengine_cancel_ffb(cur_msg); //ȡ�������
        break;

        case MSG_MENGINE_SET_FFBSTEP_SYNC:
        mengine_fastplay_step(cur_msg); //���ÿ���˲���
        break;

        //���ò����ļ���·��location
        case MSG_MENGINE_SET_FILEPATH_SYNC:
        mengine_set_filepath(cur_msg);
        break;

        case MSG_MENGINE_PLAY_NEXT_SYNC: //������һ��
        mengine_play_next(cur_msg);
        break;

        case MSG_MENGINE_PLAY_PREV_SYNC: //������һ��
        mengine_play_prev(cur_msg);
        break;

        case MSG_MENGINE_SWITCH_DISK_SYNC: //�л�����
        mengine_switch_disk(cur_msg);
        break;

        case MSG_MENGINE_LIST_ACCESS_ALLOW_SYNC: //�ɷ����б�
        mengine_list_access_allow(cur_msg);
        break;

        case MSG_MENGINE_DIGITAL_SONG_SYNC: //���ֵ��
        mengine_play_digital_song(cur_msg);
        break;

        case MSG_MENGINE_FOLFER_NEXT_SYNC: //�л���һ��Ŀ¼
        mengine_play_folder_next(cur_msg);
        break;

        case MSG_MENGINE_FOLFER_PREV_SYNC: //�л���һ��Ŀ¼
        mengine_play_folder_prev(cur_msg);
        break;
        case MSG_MENGINE_GET_FILEINFO_SYNC: //��ȡ�ļ���ʽ��b/s����ʱ�����Ϣ
        mengine_get_fileinfo(cur_msg);
        break;

        case MSG_MENGINE_CLEAR_FILESWITCH_SYNC:
        mengine_clear_fileswitch(cur_msg);
        break;

        case MSG_MENGINE_GET_PLAYTYPE_SYNC:
        mengine_get_playtype(cur_msg);
        break;

        case MSG_MENGINE_SET_SORTNUMBER_SYNC: //�����ļ����
        mengine_set_sortnumber(cur_msg);
        break;

        case MSG_MENGINE_GET_DISKNUMBER_SYNC: //��ȡ��ǰ�ļ����
        mengine_get_disknumber(cur_msg);
        break;

        /*��ȡid3��Ϣ*/
        case MSG_MENGINE_GET_ID3INFO_SYNC:
        #ifdef MENGINE_ID3_SUPPORT
        mengine_getid3_info(cur_msg);
#endif
        break;

        case MSG_MENGINE_GET_DIRNAME_SYNC:
        #ifdef DIR_NAME_SUPPORT
        mengine_get_dirname(cur_msg);
#endif
        break;

        case MSG_ENGINE_STANDBY_SYNC:
        g_mengine_result = mengine_standby(cur_msg);
        break;

        case MSG_APP_QUIT: //�˳�app
        if ((cur_msg->msg.content.data[0] & 0x01) != 0)
        {
            PRINT_INFO("eg enter s3bt");
        }
        g_mengine_result = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //׼������TTS����
        case MSG_SWITCH_AOUT_START_SYNC: //׼���л���Ƶ���
        g_mengine_result = mengine_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS�������
        case MSG_SWITCH_AOUT_STOP_SYNC: //����л���Ƶ���
        g_mengine_result = mengine_tts_stop(cur_msg);
        break;

        case MSG_MENGINE_SAVE_BKINFO_SYNC:
        mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
        mengine_reply_msg(cur_msg, TRUE);
        g_mengine_result = RESULT_IGNORE;
        break;

        case MSG_MENGINE_SD_PW_RESET_SYNC://SD���ϵ����ϵ�
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
        reset_by_PW_off();
        mengine_reply_msg(cur_msg, TRUE);
#else
        mengine_reply_msg(cur_msg, FALSE);
#endif
        break;

        case MSG_MENGINE_SET_FIO_SYNC: //����SDD��չ���ļ������ӿ�
        {
            bool msg_ret = FALSE;
            void *io = (void*)cur_msg->msg.content.addr;
            if(g_mp_handle != NULL)
            {
                mmm_mp_cmd(g_mp_handle, MMM_MP_SET_SDD_FILE_IO, io);
                msg_ret = TRUE;
            }
            mengine_reply_msg(cur_msg, msg_ret);
        }
        break;
        
        //����ϵ�����־
        case MSG_MENGINE_CLR_BKERR_SYNC:
        g_eg_status_p->first_file_status = FIRST_PLAY_FROM_BK;
        mengine_reply_msg(cur_msg, TRUE); 
        break;

        default:
        break;
    }

    return g_mengine_result;
}

//�ϵ�SD����λ
void reset_by_PW_off(void)
{
    #if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    uint32 bk_init,bk_set;
    bk_init = Get_SD_PW_PIN_INOUT();
    bk_set = Get_SD_PW_PIN_ONOFF();
    PRINT_ERR("SD-PW-RESET...");
    SET_SD_PW_PIN_ON();
    SD_PW_PIN_SET_OUTPUT();
    sys_os_time_dly(1);
    SET_SD_PW_PIN_RESET();
    sys_os_time_dly(8);
    SET_SD_PW_PIN_ON();
    sys_os_time_dly(2);
    if(bk_init == 0)//�Ƿ�Ҫ�Ļ�����״̬
    {
        SD_PW_PIN_SET_INPUT();
    }
    
    if(bk_set != 0)//�Ƿ�Ҫ�Ļ�Ϊ��λ״̬
    {
        SET_SD_PW_PIN_RESET();
    }
    #endif
    
}

//��ʼ����
app_result_e mengine_play(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = play(PLAY_NORMAL); //����
    g_eg_playinfo_p->cur_file_switch |= 0x01;

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//ֹͣ����
app_result_e mengine_stop(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = stop(STOP_NORMAL); //ֹͣ����

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//��ͣ����
app_result_e mengine_pause(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = stop(STOP_PAUSE); //��ͣ����

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//�ָ�����(����ͣ�ָ�)
app_result_e mengine_resume(void* msg_ptr)
{
    bool ret_vals;
    private_msg_t *p = (private_msg_t *)msg_ptr;

    //�����ļ�����
    ret_vals = play(*((play_mode_e *)p->msg.content.addr)); //����

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

//������������
void mengine_save_errno_no(void)
{
    eg_err_e err_status = EG_ERR_NONE;

    mmm_mp_status_t mmm_status;

    //��ȡ�����
    mmm_mp_cmd(g_mp_handle, MMM_MP_GET_ERRORNO, (unsigned int) &mmm_status.err_no);

    //ת�������
    switch (mmm_status.err_no)
    {
        case MMM_MP_ERR_OPEN_FILE:
        err_status = EG_ERR_OPEN_FILE; //���ļ�ʧ��
        break;

        case MMM_MP_ERR_FILE_NOT_SUPPORT: //�ļ���ʽ��֧��
        err_status = EG_ERR_NOT_SUPPORT;
        PRINT_ERR("FILE NOT SUPPORT");
        break;

        case MMM_MP_ERR_FILE_DAMAGED: //�ļ����
        err_status = EG_ERR_NOT_SUPPORT;
        PRINT_ERR("FILE DESTORY");
        break;

        case MMM_MP_ERR_DECODER_ERROR: //�������
        err_status = EG_ERR_DECODER_ERROR;
        PRINT_ERR("DECODER ERROR");
        break;

        case MMM_MP_ERR_NO_LICENSE: //��license
        err_status = EG_ERR_NO_LICENSE;
        break;

        case MMM_MP_ERR_SECURE_CLOCK: //DRMʱ�Ӵ���
        err_status = EG_ERR_SECURE_CLOCK;
        break;

        case MMM_MP_ERR_LICENSE_INFO: //���Ŵ����������ǲ���ʱ���Ѿ�����
        err_status = EG_ERR_DRM_INFO;
        break;
        
        case MMM_MP_SAMPLE_RATE_NOT_SUPPORT: //�����ʲ�֧��
        err_status = EG_ERR_NOT_SUPPORT;
        PRINT_ERR("SAMPLE RATE NOT SUPPORT");
        break;   
        
        default:
        err_status = EG_ERR_DECODER_ERROR; //�������
        break;
    }
    //�������״̬
    g_eg_status_p->err_status = err_status;

}

void error_handle2(void)
{
    //�رս����м��
    stop(STOP_NORMAL);

    if (g_file_sys_id != -1)
    {
        //��ȡ�ļ�����(��֤δɨ�����ʱ��ȡ����ȷ���ļ�����)
        g_eg_cfg_p->location.dirlocation.file_total = fsel_get_total();
    }
}

//������
void error_handle(void)
{
    //�������״̬
    mengine_save_errno_no();
    error_handle2();
}

