/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������music��ؽӿ�
 * ���ߣ�Gary Wang
 ********************************************************************************/
#include "music.h"
#include <uhost.h>

//music_maic.c��music.h, music_common*.c��Ӧ�ü���
//music_play*.*�ǲ�����ͼ��������ͼ
//music_ab*.*�Ǵ���AB��ͼ��
//music_digit_song*.*������ѡ��
//music_hint*.*����ʾ��Ϣ
//music_loop_mode*.*��ѭ��ģʽ
//music_song_number*.*����Ŀ��
//musc_rcp*.*�Ǵ���rcp��ص�

//����music�ķ�ʽ
app_param_e g_enter_mode;

//��ʼ���׶Σ����Թ��˵�һЩ�¼�����
bool g_musicplay_init_flag;

//�����и赽�ڼ���
uint16 g_manul_switch_song_seq;

//ϵͳȫ�ֱ���
comval_t g_comval;

//����ɨ���߳̾��
void *g_scan_handle = NULL;

//ɨ����
uint8 g_scan_flag = 0;

#ifdef PRINT_BANK_INFO
//��ӡ��־λ
int print_label;
#endif

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     (3)
//Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t music_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

bool g_music_restore_from_s3bt;

MUS_STATIC void _load_cfg(void)
{
    //ϵͳ������ȡ
    com_setting_comval_init(&g_comval);

    //���ûָ�AB����
    //�ظ�ģʽ���������Ҫ�ָ�

    sys_vm_read(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t));
    if (g_music_config.magic != VRAM_MAGIC(VM_AP_MUSIC))
    {
        g_music_config.magic = VRAM_MAGIC(VM_AP_MUSIC);
        //ѭ��ģʽ
        if ((g_enter_mode == PARAM_RECORD_UPLAY) || (g_enter_mode == PARAM_RECORD_CPLAY))
        {
            g_music_config.repeat_mode = FSEL_MODE_LOOPONEDIR;
        }
        else
        {
            g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        }

        sys_vm_write(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t));
    }
}

//����������Ϣ
void _save_cfg(void)
{
    sys_vm_write(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t));
    sys_vm_write(&g_comval, VM_AP_SETTING, sizeof(comval_t));
}

bool _app_init(void)
{
    //��ȡ������Ϣ
    _load_cfg();
    //ѭ��ģʽ
    if (g_music_config.repeat_mode < MIN_LOOP_MODE)
    {
        g_music_config.repeat_mode = DEF_LOOP_MODE;
    }
    libc_memset(&g_rcp_file_status, 0, sizeof(g_rcp_file_status));

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_MUSIC, APP_TYPE_GUI);

    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(music_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //������init_app_timers �������
    sys_timer_init();

    com_rcp_init();//������xxx_rcp_var_init֮ǰ
    //��ʼ��RCP�ص�����
    music_rcp_var_init();

    com_view_manager_init();

#ifdef SUPPORT_ASET_TEST
    if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) 
        || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        aset_test_init();
    }
#endif
    com_set_dae_chan(FALSE, FALSE);
    return TRUE;
}

//����������Ϣ
bool _app_deinit(void)
{
#ifdef SUPPORT_ASET_TEST
    if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) 
        || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        aset_test_exit();
    }
#endif

    //ע��RCP�ص�����
    music_rcp_var_exit();

    com_view_manager_exit();

    //����ϵͳ��ʱ��
    sys_timer_exit();

    //ִ��applib���ע������
    applib_quit();

    //дvram
    _save_cfg();

    return TRUE;
}

//��ڲ���˵����
//argc:
//  PARAM_RECORD_CPLAY: ����¼������
//  PARAM_RECORD_UPLAY: U��¼������
//  PARAM_MUSIC_SDCARD: ������
//  PARAM_MUSIC_UHOST: U����
//  ע�����Ӳ��Ų���������
int main(int argc, const char *argv[])
{
    app_result_e retval = RESULT_NEXT_FUNCTION;

    g_standby_exit_flag = (bool) ((argc & PARAM_S3_EXIT) != 0);
    g_music_restore_from_s3bt = g_standby_exit_flag;
    argc = argc & (~PARAM_S3_EXIT);

    g_esd_restart_flag = (bool) ((act_readl(RTC_BAK0) & (1 << MY_RTC_ESD_FLAG)) != 0);

    g_enter_mode = argc;

    //Ҫ����U�̻��ǿ�
    if ((argc == PARAM_MUSIC_UHOST) || (argc == PARAM_RECORD_UPLAY))
    {
        g_file_path_info.file_path.dirlocation.disk = DISK_U;
    }
    else
    {
        g_file_path_info.file_path.dirlocation.disk = DISK_H;
    }

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    discrete_led_set(LED_ID_MUSIC_PLAY, LED_ALWAYS_ON, NULL);
#endif

    //��ʼ��
    _app_init();
    if ((com_get_config_default(SETTING_APP_SUPPORT_CARDPLAY)
            | com_get_config_default(SETTING_APP_SUPPORT_UHOSTPLAY)) != 0)
    {
        retval = music_scene_play();
    }

    //�����¼������,����ڽ�����һ���Ĵ����ɻص���һ�β���
    if (retval == RESULT_NEXT_FUNCTION)
    {
        if ((g_enter_mode == PARAM_RECORD_CPLAY) || (g_enter_mode == PARAM_RECORD_UPLAY))
        {
            retval = RESULT_LASTPLAY;
        }
    }

    if (retval == RESULT_SYSTEM_ENTER_S3)
    {
        PRINT_INFO("ap enter s3bt");
    }

    com_ap_switch_deal(retval);

    //�˳�
    _app_deinit();

#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
    discrete_led_set(LED_ID_MUSIC_PLAY, LED_ALWAYS_OFF, NULL);
#endif

    PRINT_DBG_INT("music exit:", retval);
    return retval;
}
