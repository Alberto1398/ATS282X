#include "app_mengine.h"
#include "audio_device.h"

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     3

#ifdef WAVES_ASET_TOOLS
waves_t  g_waves;
#endif
output_channel_e g_output_channel_config;

//globle variable
//�����м�����
void *g_mp_handle = NULL;

//����ļ�ϵͳid
int32 g_file_sys_id = -1;

//����ģʽ
mengine_enter_mode_e g_mengine_enter_mode;

//������Ϣ
mengine_info_t g_mengine_info[2];
mengine_info_t *g_mengine_info_p;
//�м�������ļ���Ϣ
mmm_mp_file_info_t g_eg_file_info;
//������Ϣ(��������)
mengine_config_t g_eg_config;

//��ʼ��ʱ���̷�
uint8 g_open_disk;

//�ļ���buffer
uint8 g_file_name[FILE_NAME_LEN];

//�м��״̬��ⶨʱ��
int8 g_status_timer;

//�м��״̬����־λ
uint8 g_check_status_flag;

//music engine��ʱ����
uint8 g_delay_time;

//�ؼ�����ֵ
app_result_e g_mengine_result;

//�������ñ����ṹ��ָ��
mengine_config_t *g_eg_cfg_p;

//����״̬�����ṹ��ָ��
mengine_status_t *g_eg_status_p;

//���沥����Ϣ�ṹ��ָ��
mengine_playinfo_t *g_eg_playinfo_p;

//eq data
mmm_mp_eq_info_t g_eq_info;
//private_msg_t g_cur_msg;

//for clk adjust
uint32 g_save_freq = 0;

#ifdef MENGINE_ID3_SUPPORT
//���id3��Ϣ
id3_info_t g_id3_info;

//���⻺����
uint8 g_title_buf[Id3BufSIZE];

//���ֻ�����
uint8 g_artist_buf[ArtBufSIZE];

//ר��������
uint8 g_album_buf[ArtBufSIZE];

#endif
#ifdef DIR_NAME_SUPPORT
uint8 g_dir_name_buf[32];
#endif
//�����б���
bool g_list_access_allow = FALSE;

//app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t mengine_app_timer_vector[APP_TIMER_COUNT];

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
bool g_stop_by_tts_flag;

uint8 g_shuffle_flag;
fsel_mode_e g_repeat_mode;
//�Ƿ�֧�ֿ�
uint8 g_card_support;
//�Ƿ�֧��uhost
uint8 g_uhost_support;
fsel_type_e g_fsel_type;
//MMM_MP_OPENָ���Ƿ񷢳�
bool g_mmm_opened = FALSE;

//for dae adjust freq
dae_config_t *g_dae_cfg_shm;

bool g_music_replay_by_prev_key;

//for clr vram
uint8 g_need_clr_vm = 0;

void __section__(".text.BANK2") _clear_vm_deal(uint8 clr_sel)
{
    libc_memset(&g_eg_cfg_p->location.dirlocation, 0, sizeof(musfile_location_u));
    g_eg_cfg_p->location.dirlocation.disk = g_open_disk;
    libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

    if ((clr_sel == 0) || (clr_sel == DISK_H))
    {
        sys_vm_write(g_eg_cfg_p, VM_CARD_BKINFO, sizeof(mengine_config_t));

#if (SUPPORT_MUSIC_FOLDER == 1)
        uint8 i;
        //�����������Ŀ¼VRAM����
        for(i = 0; i < MUSIC_FOLDER_MAX; i++)
        {
            sys_vm_write(g_eg_cfg_p, g_music_folder_vm_addr[i], sizeof(mengine_config_t));
        }
#endif
    }

    if ((clr_sel == 0) || (clr_sel == DISK_U))
    {
        mengine_vram_write(DISK_U);
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    uint16 vram_magic;

    g_music_replay_by_prev_key = (bool) com_get_config_default(MUSIC_REPLAY_BY_PREV_KEY);

    if (g_open_disk == DISK_H)
    {
        vram_magic = VRAM_MAGIC(VM_CARD_BKINFO);
        //if ((g_app_info_state.need_update_playlist & CARD_NEED_SCAN) == 0)
        if (g_need_clr_vm == 0)
        {
            sys_vm_read(g_eg_cfg_p, VM_CARD_BKINFO, sizeof(mengine_config_t));
        }
        else
        {
            _clear_vm_deal(g_open_disk);
        }
    }
    else
    {
        vram_magic = VRAM_MAGIC(VM_UHOST_BKINFO);
        //if ((g_app_info_state.need_update_playlist & UHOST_NEED_SCAN) == 0)
        if (g_need_clr_vm == 0)
        {
            sys_vm_read(g_eg_cfg_p, VM_UHOST_BKINFO, sizeof(mengine_config_t));
        }
        else
        {
            _clear_vm_deal(g_open_disk);
        }
    }
    g_need_clr_vm = 0;

    get_bk_info();
    g_fsel_type = FSEL_TYPE_COMMONDIR;

    if (g_open_disk != DISK_SD)
    {
        //�ָ��ϵ�
        g_eg_cfg_p->location.dirlocation.disk = g_open_disk;
    }

    g_card_support = (uint8) com_get_config_default(SETTING_APP_SUPPORT_CARDPLAY);
    g_uhost_support = (uint8) com_get_config_default(SETTING_APP_SUPPORT_UHOSTPLAY);

    if ((g_eg_cfg_p->magic != vram_magic) && (g_open_disk != DISK_SD))
    {
        g_eg_cfg_p->magic = vram_magic;
        //�ļ�ѭ��ģʽ
        g_repeat_mode = FSEL_MODE_LOOPALL;
        //shuffle flag
        g_shuffle_flag = SHUFFLE_NONE;

        //(��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ)
        libc_memset(&g_eg_cfg_p->location.dirlocation, 0, sizeof(musfile_location_u));
        //����Ĭ��Ϊudisks
        g_eg_cfg_p->location.dirlocation.disk = g_open_disk;
        //�ϵ���Ϣ
        libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));
        mengine_vram_write(g_open_disk);
    }

    if (g_mengine_enter_mode == ENTER_ALARM)
    {
        if (g_open_disk == DISK_SD)
        {
            g_fsel_type = FSEL_TYPE_SDFILE;
        }
        //(��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ)
        libc_memset(&g_eg_cfg_p->location.dirlocation, 0, sizeof(musfile_location_u));
        g_eg_cfg_p->location.dirlocation.disk = (uint8) g_open_disk;
    }
    else
    {
        //����ļ���Ч��
        //��Ҫ������ȡ�ļ���Ϣ
        g_eg_playinfo_p->cur_file_switch |= 0x01;
    }

	g_output_channel_config = (uint8)com_get_config_default(OUTPUT_CHANNEL_CONFIG);
}

//����������Ϣ
void _save_cfg(void)
{
    /*����VM����*/
    //��ǰ���ŵ���music����
    if ((g_fsel_type < FSEL_TYPE_SDFILE) && (g_mengine_enter_mode != ENTER_RECOD_PLAY) && (g_mengine_enter_mode
            != ENTER_ALARM))
    {
        //ֻ��music����ʱ�ű���vram
        mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
    }
}

#ifdef WAVES_ASET_TOOLS
void waves_init(void)
{
    if (STUB_PC_TOOL_WAVES_ASET_MODE == g_app_info_state_all.stub_pc_tools_type)
    {
        g_waves.tuning_status = TUNING;
    }
    else if (STUB_PC_TOOL_UNKOWN == g_app_info_state_all.stub_pc_tools_type)
    {
        g_waves.tuning_status = NO_TUNING;
    }
    g_waves.bin_number = g_app_info_state_all.bin_number;
    g_waves.input_para_enable = (uint8) com_get_config_default(SETTING_APP_SUPPORT_WAVES_INPUT_PARAM); 
}
#endif

//��ʼ�����桢��ȡ������Ϣ����ʼ���ļ�ѡ�������м��
int _app_init(void)
{
    int app_init_ret = 0;
    bool init_fsel_ret;
    char *mmm_name =
    { "mmm_mp.al" };

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_MENGINE, APP_TYPE_CONSOLE);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL10, FREQ_NULL);

    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ�� applib ��Ϣģ��
    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(mengine_app_timer_vector, APP_TIMER_COUNT);

    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL6, FREQ_NULL);

    //�����м������
    sys_load_mmm(mmm_name, TRUE);

    //��ʼ���м����
    app_init_ret = mmm_mp_cmd(&g_mp_handle, MMM_MP_OPEN, (unsigned int) NULL);

    if (app_init_ret == 0)
    {
        if(1 == g_app_info_state_all.fix_sample_rate_flag)
        {
            mmm_mp_cmd(&g_mp_handle, MMM_MP_FIX_SAMPLE_RATE, NULL);
        }
        g_mmm_opened = TRUE;
        com_set_sound_out(SOUND_OUT_START);

        //for front ap check if eg init ok
        g_eg_status_p->ctl_status = EG_CTL_INITING;
        //update status
        me_update_share();

        sys_os_time_dly(2); //��ǰ̨����һ���û�����

        if (g_fsel_type != FSEL_TYPE_SDFILE)
        {
            //��ʼ���ļ���ȡ�ɲ����ļ�/�ϵ�
            init_fsel_ret = mengine_file_init();
            if (init_fsel_ret == FALSE)
            {
                g_eg_playinfo_p->cur_file_switch &= (uint8) 0xfe;
            }
        }
        else
        {
            init_fsel_ret = TRUE;
        }

        g_eg_status_p->ctl_status = EG_CTL_NORMAL;
        //update status
        me_update_share();

        sys_os_time_dly(2); //��ǰ̨����һ���û�����
    }

    g_delay_time = 2;
    //��һ�ν���������ѯ״̬���Ը���mengine����
    g_check_status_flag = TRUE;

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);
    g_eg_playinfo_p->cur_time = (uint32)g_eg_cfg_p->bk_infor.bp_time_offset;

#ifdef WAVES_ASET_TOOLS
    waves_init();
#endif   

    return app_init_ret;
}

//�˳�app�Ĺ��ܺ���,����������Ϣ
bool _app_deinit(void)
{
    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL10, FREQ_NULL);

    //�ر��ļ�ѡ����
    fsel_exit();

    //�ر������豸
    //mmm_mp_cmd(g_mp_handle, MMM_MP_AOUT_CLOSE, (unsigned int) NULL);
    //�ر��м���豸
    if (g_mmm_opened == TRUE)
    {
        mmm_mp_cmd(g_mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);
        g_mmm_opened = FALSE;
    }

    com_set_sound_out(SOUND_OUT_STOP);

    //save config дvram
    _save_cfg();

    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);
    //ж�ؽ�������
    sys_free_mmm(TRUE);

    //��Ƶ
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    //ִ��applib���ע������
    applib_quit();
    return TRUE;
}

//argc��Ȼ��int�ͣ���ֻ�е�8λ����(��Ϊap_switch_info_t.param��8λ��)
//argv��û���õ�(Ҳ����Ϊap_switch_info_t��ԭ��)
/*argc(��8λ��Ч)�����ĺ���:
***********��0λ�͵�1λ��ʾ����ģʽ*****************
*00  ��ͨ�ļ�
*01  ¼���ļ�
*10  ����
*11  ��û��
************��2��3��4λΪ��չλ*********************

************��5λ�͵�6λ��ʾ�洢����****************
*00  ��û��
*01  ��
*10  U��
*11  ��û��
************��7λ��ʾ�ϵ�***************************
*0   ��ʾ����Ҫ�ϵ���Ϣ
*1   ��ʾ��Ҫ�ϵ���Ϣ
*/
int main(int argc, const char *argv[])
{
    g_mengine_enter_mode = (mengine_enter_mode_e) (argc & 0x1f);

    //����ǰ��̨���̷�һ��
    //g_open_disk = (uint8) (argc & 0x60);
    if ((argc & 0x20) == ENTER_MUSIC_DISK_H)
    {
        g_open_disk = DISK_H;
    }
    else if ((argc & 0x40) == ENTER_MUSIC_DISK_U)
    {
        g_open_disk = DISK_U;
    }
    else
    {
        g_open_disk = DISK_SD;
    }

    //���ҽ���argc�ĵ�7λΪ1��������������ģʽ��Ϊ����ʱ������ϵ���Ϣ
    if (((argc & MUSIC_NEED_BK) == 0)&&(g_mengine_enter_mode!=ENTER_ALARM))
    {
        g_need_clr_vm = 1;
    }
    else
    {
        g_need_clr_vm = 0;
    }

    //ȫ�ֱ���ָ��
    g_eg_cfg_p = &g_eg_config;

    g_mengine_info_p = &g_mengine_info[0]; //Ӧ�ü���ʱ��ʼ��Ϊ0�������ٳ�ʼ����

    //���������ѯ����ʱ���뱣֤ǰ̨����ȡ״̬ʱ�ǿ��õ�
    g_mengine_info_p = sys_share_query_creat(APP_ID_MENGINE, g_mengine_info_p, sizeof(mengine_info_t));
    if (g_mengine_info_p == NULL)
    {
        PRINT_ERR("music share query create fail!\n");
        while (1)
        {
            ; //QAC
        }
    }

    //get for dae freqency
    //����DAE���ò����������ڴ�
    g_dae_cfg_shm = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (g_dae_cfg_shm == NULL)
    {
        PRINT_ERR("dae_cfg shm not exist!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    g_eg_status_p = &(g_mengine_info_p->eg_status);
    g_eg_playinfo_p = &(g_mengine_info_p->eg_playinfo);
    //��ʼ��
    _app_init();
    set_esd_restart_tag();
    //ѭ��
    g_mengine_result = mengine_control_block(); //app���ܴ���

    //�˳�
    _app_deinit();

    //���ٹ����ѯ
    if (sys_share_query_destroy(APP_ID_MENGINE) == -1)
    {
        PRINT_ERR("music share query destroy fail!\n");
        while (1)
        {
            ; //QAC
        }
    }
    clear_esd_restart_tag();
    return g_mengine_result;
}
