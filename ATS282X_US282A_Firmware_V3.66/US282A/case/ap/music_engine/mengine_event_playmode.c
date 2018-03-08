#include "app_mengine.h"

//for id3 use
#ifdef MENGINE_ID3_SUPPORT

#define MusicExtNum 7

//�������к�׺��
const uint8 support_ext[MusicExtNum][4] =
{
    "MP1", //mp3
    "MP2",//mp3
    "MP3",//mp3
    "WMA",//wma
    "APE",//ape
    "FLA",//flac
    "OGG",//ogg
};

const id3_type_e support_id3_type[] =
{
    ID3_TYPE_MP3, //mp1
    ID3_TYPE_MP3,//mp2
    ID3_TYPE_MP3,//mp3
    ID3_TYPE_WMA,//wma
    ID3_TYPE_APE,//ape
    ID3_TYPE_FLAC,//fla
    ID3_TYPE_OGG,//ogg
};

//�õ�ID3������
id3_type_e mengine_check_music_type(uint8 * extname)
{
    uint8 cnt;
    id3_type_e ret = ID3_TYPE_END;
    //ת����ǰ��׺��(ת�ɴ�д)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < MusicExtNum; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    return ret;
}

//��ʼ��ID3�����buffer, ������Щbuffer��ָ��浽g_id3_info����
void mengine_init_id3buf(void)
{
    //����buffer
    libc_memset(g_title_buf, 0, sizeof(g_title_buf));

    g_id3_info.tit2_buffer = g_title_buf;
    //����buffer�ͳ���
    libc_memset(g_artist_buf, 0, sizeof(g_artist_buf));

    g_id3_info.tpe1_buffer = g_artist_buf;
    //ר��buffer
    libc_memset(g_album_buf, 0, sizeof(g_album_buf));

    g_id3_info.talb_buffer = g_album_buf;
    //����buffer
    g_id3_info.genre_buffer = NULL;
    //drm��buffer
    g_id3_info.drm_buffer = NULL;
    //����buffer
    g_id3_info.track_buffer = NULL;

    //�ļ����ⳤ��
    g_id3_info.tit2_length = Id3BufSIZE;
    //���߳���
    g_id3_info.tpe1_length = ArtBufSIZE;
    //ר������
    g_id3_info.talb_length = ArtBufSIZE;
    //���ɳ���
    g_id3_info.genre_length = 0;
    //drm����
    g_id3_info.drm_length = 0;
    //���쳤��
    g_id3_info.track_length = 0;
}

//��ȡid3��Ϣ(����浽g_id3_info)
//path_info: �ļ���·����Ϣ
void mengine_getid3_done(musfile_location_u* path_info)
{
    id3_type_e music_type;
    //bool have_no_id3 = FALSE;
    uint8* nameptr = path_info->dirlocation.file_info.file_extend_info.file_ext;

    //��ʼ������
    mengine_init_id3buf();

    //��ȡ��ǰ�ļ�����
    music_type = mengine_check_music_type(nameptr);

    //��ȡid3��Ϣ
    fsel_get_id3_info(&g_id3_info, NULL, music_type);

    //����id3������
    //play_scene_deal_id3();
}

//�ظ��ͻ��ˣ�id3��Ϣ
app_result_e mengine_getid3_info(void* msg_ptr)
{
    //���ò������͵�ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    id3_info_t* dest_ptr = (id3_info_t*)data_ptr->reply->content;

    //copy //���� info
    libc_memcpy(dest_ptr->tit2_buffer, g_title_buf,
            (unsigned int)dest_ptr->tit2_length);
    //copy //artist info
    libc_memcpy((uint8*)(dest_ptr->tpe1_buffer), g_artist_buf,
            (unsigned int)dest_ptr->tpe1_length);
    //copy //album info
    libc_memcpy((uint8*)(dest_ptr->talb_buffer), g_album_buf,
            (unsigned int)dest_ptr->talb_length);
    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

#endif //#ifdef MENGINE_ID3_SUPPORT

#ifdef DIR_NAME_SUPPORT

//��ȡĿ¼��
app_result_e mengine_get_dirname(void* msg_ptr)
{
    //���ò������͵�ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    uint8* dest_ptr = (uint8*)data_ptr->msg.content.addr;
    //dir name info
    libc_memcpy(dest_ptr,g_dir_name_buf,sizeof(g_dir_name_buf));

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

#endif

//���ÿ���˵Ĳ���
app_result_e mengine_fastplay_step(void *msg_ptr)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //�ٶȲ��� ����Ϊ��λ
    uint32 *speed_ptr = (uint32 *) (data_ptr->msg.content.addr);

    //���������� or ���
    if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_FFB_STEP, (unsigned int) (*speed_ptr)); //���
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//���
app_result_e mengine_fast_forward(void* msg_ptr)
{
    //���������� or ���
    if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) 4); //���
        g_eg_status_p->play_status = PlayFast; //�޸�״̬
        g_eg_status_p->fast_status = FFPlaySta;
    }
    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//����
app_result_e mengine_fast_backward(void* msg_ptr)
{
    //��ǰ���������� or ����
    if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) -4); //����
        g_eg_status_p->play_status = PlayFast; //�޸�״̬
        g_eg_status_p->fast_status = FBPlaySta;
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//ȡ���������
app_result_e mengine_cancel_ffb(void* msg_ptr)
{
    //��ǰ�ǿ����״̬
    if ((g_eg_status_p->play_status == PlayFast) && (g_eg_status_p->fast_status != FBPlay_Null))
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_SET_FFB, (unsigned int) 0); //ȡ�������
        g_eg_status_p->play_status = PlaySta; //�޸�״̬
        g_eg_status_p->fast_status = FBPlay_Null;
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

#ifdef AB_SUPPORT

//����ab����,�ȴ�����a��
app_result_e mengine_start_ab(void* msg_ptr)
{
    //mengine_status_t *eg_status = &mengine_info.eg_status;
    if (g_eg_status_p->play_status == PlaySta)
    {
        g_eg_status_p->ab_status = PlayAB_A; //�ȴ�����A��
        g_eg_status_p->play_status = PlayAB;

        //���ø���������������
        //mmm_mp_cmd(g_mp_handle, MMM_MP_AB_COUNT, (unsigned int)(g_eg_cfg_p->replaytimes));

        //���ø��������������  10msΪ��λ
        //mmm_mp_cmd(g_mp_handle, MMM_MP_AB_WAIT_TIME, (unsigned int) (g_eg_cfg_p->replaygap * 100));
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//����a��
app_result_e mengine_set_apoint(void* msg_ptr)
{
    //mengine_status_t *eg_status = &mengine_info.eg_status;
    g_eg_status_p->ab_status = PlayAB_B; //�ȴ�����B��
    g_eg_status_p->play_status = PlayAB;
    mmm_mp_cmd(g_mp_handle, MMM_MP_SET_A, (unsigned int) NULL); //��������A������

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//����b��,��ʼ����
app_result_e mengine_set_bpoint(void* msg_ptr)
{
    //mengine_status_t *eg_status = &mengine_info.eg_status;
    g_eg_status_p->ab_status = PlayAB_Do; //��ʼab����
    g_eg_status_p->play_status = PlayAB; //���ò���״̬ΪAB����
    mmm_mp_cmd(g_mp_handle, MMM_MP_SET_B, (unsigned int) NULL); //��������B������

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//���ab����
app_result_e mengine_clear_ab(void* msg_ptr)
{
    //mengine_status_t *eg_status = &mengine_info.eg_status;
    g_eg_status_p->ab_status = PlayAB_Null; //���AB����״̬
    g_eg_status_p->play_status = PlaySta; //�ָ���������״̬
    mmm_mp_cmd(g_mp_handle, MMM_MP_CLEAR_AB, (unsigned int) NULL); //�������AB��������

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//����ab��������
app_result_e mengine_set_ab_count(void* msg_ptr)
{
    //���ø���������ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint16 rep_count;
    //��ȡ��ǰ��������
    rep_count = *(uint16*) (data_ptr->msg.content.addr);
    //mengine_info.eg_config.replaytimes = rep_count;//���渴������
    //���ø���������������
    mmm_mp_cmd(g_mp_handle, MMM_MP_AB_COUNT, (unsigned int) rep_count);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

//����ab�������ʱ��
app_result_e mengine_set_ab_gap(void* msg_ptr)
{
    //���ø��������ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint8 ab_gap;
    ab_gap = *(uint8*) (data_ptr->msg.content.addr); //��ȡ��ǰ�������
    //mengine_info.eg_config.replaygap = ab_gap;///���渴��������������
    //���ø��������������  10msΪ��λ
    mmm_mp_cmd(g_mp_handle, MMM_MP_AB_WAIT_TIME, (unsigned int) ab_gap * 100);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
#endif //#ifdef AB_SUPPORT
