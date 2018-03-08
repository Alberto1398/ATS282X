/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2014-11-05        1.0              create this file
 *******************************************************************************/

#include "app_uengine.h"

#include "audio_device.h"

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT        3

#ifdef WAVES_ASET_TOOLS
waves_t g_waves;
#endif

output_channel_e g_output_channel_config;

//for share status inquiry
uengine_status_t g_cur_play_status[2];

uengine_status_t* g_cur_play_status_p;

//for stop usb
uint32 g_stop_count;

//globle variable
//�����м�����
void *g_mmm_pp_handle = NULL;

//�м��״̬��ⶨʱ��
int32 g_status_timer;

//hid������ʱ��
int32 g_hid_timer;

usound_status_t g_status_data;

mmm_pp_ainout_setting_t g_ain_out_setting;

uint32 g_volume_syn_cfg;

//�м��״̬����־λ
uint8 g_check_status_flag;

//usound engine��ʱ����
uint8 g_delay_time;

uint8 g_auto_play_flag;

uint8 g_switch_song_flag;

uint8 g_pa_type;

uint32 g_current_vol = 0xffffffff;

uint32 g_vol_sync_timer;

uint32 g_auto_play_count;

//�����������������Ϊ��ʱ��1
uint8 g_data_flag;

//�ؼ�����ֵ
app_result_e g_uengine_result;

//vram�ָ�����
//mengine_config_t g_bkinfo_bak;

//eq data
//mmm_mp_eq_info_t g_eq_info;

//app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t g_uengine_timer_vector[APP_TIMER_COUNT];

OS_STK *ptos = (OS_STK *) AP_UENGINE_STK_POS;

INT8U prio = AP_UENGINE_PRIO;

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
//bool stop_by_tts_flag;
//driver select  USOUND_TYPE
uint32 g_usound_cfg;

//for tts flags;
uint32 g_stop_by_tts;

uint32 energy_count;

uint8 g_mute_flag = 1;

uint32 mute_cnt;

uint8 g_mute_cnt_flag = 0;

uint8 g_force_equ;

uint8 g_vol_chg_cnt = 0;

uint8 g_vol_no_chg;

int8 g_pre_dif;

uengine_sync_volume_t g_sync_vol_share;

uengine_sync_volume_t* g_share_ptr;

//for dae adjust freq
dae_config_t *g_dae_cfg_shm;

uengine_config_t g_usoud_config_val;

bool g_uspeaker_24bit;
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
 * \ingroup      uengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    g_usound_cfg = com_get_config_default(USOUND_TYPE);
    
    g_pa_type = (uint8)com_get_config_default(SETTING_AUDIO_OUTPUT_CHANNAL);

    //sys_vm_read(&g_usoud_config_val, VM_AP_USOUND, sizeof(uengine_config_t));

    //if (g_usoud_config_val.magic != VRAM_MAGIC(VM_AP_USOUND))
    {
        //g_usoud_config_val.magic = VRAM_MAGIC(VM_AP_USOUND);

        if(g_usound_cfg == 2)
        {
            g_usoud_config_val.dac_sample = 96;

            g_usoud_config_val.adc_sample = 96;
        }
        else if(1 == g_app_info_state_all.fix_sample_rate_flag)
        {
            g_usoud_config_val.dac_sample = 48;

            g_usoud_config_val.adc_sample = 48;
        }
        else
        {
            g_usoud_config_val.dac_sample = 44;

            g_usoud_config_val.adc_sample = 44;
        }
    }
	
	g_output_channel_config = (uint8)com_get_config_default(OUTPUT_CHANNEL_CONFIG);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      uengine_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    g_usoud_config_val.dac_sample = (uint32)g_ain_out_setting.dac_sample;

    g_usoud_config_val.adc_sample = (uint32)g_ain_out_setting.adc_sample;

    //sys_vm_write(&g_usoud_config_val, VM_AP_USOUND, sizeof(uengine_config_t));
}

/******************************************************************************/
/*!
 * \par  Description:
 * ������Ƶ�������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      uengine_main.c
 * \note
 *******************************************************************************/
void _set_ainout_param(mmm_pp_ainout_setting_t* param_ptr)
{

    if (g_usound_cfg == 0)
    {
        param_ptr->input_type = MMM_PP_USOUND_IN;//usb up and down
    }
    else
    {
        param_ptr->input_type = MMM_PP_USPEAKER_IN;//usb down only
    }

    param_ptr->asrc_index = K_OUT1_P2_IN_U1;//asrc index

    param_ptr->dac_sample = (int)g_usoud_config_val.dac_sample;//48;//48k

    param_ptr->dac_chanel = 1;//dac 1

    param_ptr->asrc_mode_sel = 2;//param table index

    param_ptr->adc_sample = (int)g_usoud_config_val.adc_sample;//48;//adc sample rate

    param_ptr->adc_insel = AI_SOURCE_ADFMIC;

    param_ptr->ain_gain = 7;

    param_ptr->adc_gain = 0;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \0x00-0x0fתASCII��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      uengine_main.c
 * \note
 *******************************************************************************/
void rd_itoa(uint8 data, uint8 pos, uint8 *str)
{
    //uint8 data_low,data_high;
    uint8 data_cur = 0;
    
    if(pos != 0)
    {
        data_cur = (uint8)((data & 0xf0) >> 4);
    }
    else
    {
        data_cur = (data & 0x0f);
    }
    
    if(data_cur > 9)
    {
        data_cur += 55;
    }
    else
    {
        data_cur += 48;
    }
    *str = data_cur;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \���������SN��дVM�����VM������SN����ֱ��ʹ��VM�м�¼��SN
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      uengine_main.c
 * \note
 *******************************************************************************/

void sn_random(uint8 *buf)
{
    uint8 i = 0;
    
    uint32 num1,num2 = 0;
    
    testval_t usb_sn = {0};
    
    sys_vm_read(&usb_sn,VM_API_FLAG,sizeof(testval_t)); 
    
    if(usb_sn.usb_sound_magic == 0x5A5A)
    {
        num1 = usb_sn.usb_sound_low;
        
        num2 = usb_sn.usb_sound_high;
    }
    else
    {
        num1 = sys_random();

        num2 = sys_random();

        usb_sn.usb_sound_low = num1;
        
        usb_sn.usb_sound_high = num2;
        
        usb_sn.usb_sound_magic = 0x5A5A;
        
        sys_vm_write(&usb_sn,VM_API_FLAG,sizeof(testval_t));
    }

    for(i=0;i<8;i++)
    {
        rd_itoa((uint8)(num1>>i),0,&buf[i]);
    }

    for(i=0;i<8;i++)
    {
        rd_itoa((uint8)(num2>>i),0,&buf[i+8]);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * ��ȡUSB��ص�������Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      uengine_main.c
 * \note
 *******************************************************************************/
void _set_usb_info(void)
{
    uint8 info_str[16];
    
    bool sn_default = 0;

    //vid
    com_get_config_struct(USB_VID, info_str, 6);

    ud_set_config(0, info_str, 6);

    //pid
    com_get_config_struct(USB_SOUND_PID, info_str, 6);

    ud_set_config(1, info_str, 6);

    //vendor
    com_get_config_struct(INF_USB_VENDOR, info_str, 8);

    ud_set_config(2, info_str, 8);

    //product name
    com_get_config_struct(INF_SOUND_PRODUCT_NAME, info_str, 16);

    ud_set_config(3, info_str, 16);

    //set volume
    com_get_config_struct(INF_VOLUME_FLAG, info_str, 1);

    g_volume_syn_cfg = info_str[0];

    ud_set_config(4, info_str, 1);

    com_get_config_struct(USB_SOUND_BCD_DEVICE, info_str, 6);
    
    ud_set_config(5, info_str, 6);
    
    sn_default = (bool)com_get_config_default(USB_SN_TYPE);
    if(sn_default == 1)
    {
        com_get_config_struct(USB_SOUND_SN, info_str, 16);
    }
    else
    {
        sn_random(&info_str);
    }
    
    ud_set_config(6, info_str, 16);

    if(1 == g_app_info_state_all.fix_sample_rate_flag)
    {
        ud_set_cmd(FIX_SAMPLE_RATE,NULL);
    }
    ud_set_cmd(SET_VOLUME,(g_current_vol * 226) + 0xe3a0);
    
    //set sample type
    com_get_config_struct(USOUND_SAMPLE_TYPE, info_str, 1);
   
    ud_set_config(7, info_str, 1);
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
    else
    {
        ;//nothing
    }
    g_waves.bin_number = g_app_info_state_all.bin_number;
    g_waves.input_para_enable = (uint8) com_get_config_default(SETTING_APP_SUPPORT_WAVES_INPUT_PARAM); 
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \��ʼ�����桢��ȡ������Ϣ����ʼ���ļ�ѡ�������м��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval          -1 failed
 * \ingroup      uengine_main.c
 * \note
 */
/*******************************************************************************/
int _app_init(void)
{
    int app_init_ret = 0;
    //bool init_fsel_ret;
    char *mmm_name =
    { "mmm_pp.al" };
    scan_isr_cbk tmp_scan_isr_cbk;
    
    private_msg_t cur_msg_val;
    
    testval_t us_state = {0};
    
    sys_vm_read(&us_state,VM_API_FLAG,sizeof(testval_t));
    
#ifdef WAVES_ASET_TOOLS
    waves_init();
#endif 
  
    
    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_UENGINE, APP_TYPE_CONSOLE);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init();

    //��ʼ����ʱ��
    init_app_timers(g_uengine_timer_vector, APP_TIMER_COUNT);
    
    if (g_usound_cfg == 0)
    {
        //usound drv
        sys_drv_install(DRV_GROUP_UD, 0, "usound.drv");

    }
    else if (g_usound_cfg == 1)
    {
        //speaker drv
        sys_drv_install(DRV_GROUP_UD, 0, "uspeaker.drv");
    }
    else if (g_usound_cfg == 2)
    {
        g_uspeaker_24bit = 1;
        sys_drv_install(DRV_GROUP_UD, 0, "usound24.drv");
    }
    else
    {
        ;//for QAC
    }
    if(g_uspeaker_24bit == 1)
    {
        //����ϵͳƵ��
        sys_adjust_clk(104 | (104 << 8), 0);
        
        //����Ƶ��Ϊ��MIPS -> 85M, DSP -> 180
        sys_lock_adjust_freq(104 | (104 << 8)); 
    }
    
    ud_set_cmd(SET_MUTE_FLAG,1);

    if(0 == g_uspeaker_24bit)
    {
        //set param init
        _set_ainout_param(&g_ain_out_setting);

        //�����м������
        sys_load_mmm(mmm_name, TRUE);//������
            
        //must after usb driver install
        app_init_ret = uegine_player_open();
    }
    else if(g_pa_type == 1)
    {
        ccd_i2s_pa_set_clock(g_usoud_config_val.dac_sample);
    }
    else
    {
        ;//for qac        
    }

    _set_usb_info();

    //disable detect when init
    DISABLE_USB_DETECT();

    ud_module_start(&tmp_scan_isr_cbk);
    key_register_scan_isr_cbk(tmp_scan_isr_cbk);

    //enable detect after init
    ENABLE_USB_DETECT();
    
    if((us_state.api_flag & 0x08) != 0)
    {
        g_auto_play_flag = 1;
        us_state.api_flag &= ~0x08;
        sys_vm_write(&us_state,VM_API_FLAG,sizeof(testval_t));
    }
    
    //��һ�ν���������ѯ״̬���Ը���mengine����
    g_check_status_flag = TRUE;

    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ���,����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      uengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    testval_t us_state = {0};
    
    sys_vm_read(&us_state,VM_API_FLAG,sizeof(testval_t));
    
    if(0 == g_uspeaker_24bit)
    {
        //must before usb driver unstall
        uegine_player_close();
    }
    else
    {
        ud_set_cmd(SET_USPEAKER_24BIT,USPEAKER24_PAUSE);
    }

    //if is play then stop
    if (g_status_data.start_play == 1)
    {
        uint8 i = 0;
        ud_set_cmd(SET_HID_OPERS, 0x08);
        while (i < 5)
        {
            ud_hid_deal();
            i++;
            sys_os_time_dly(1);
        }
        us_state.api_flag |= 0x08;
        sys_vm_write(&us_state,VM_API_FLAG,sizeof(testval_t));
    }
    else if(g_auto_play_flag == 1)
    {
        us_state.api_flag |= 0x08;
        sys_vm_write(&us_state,VM_API_FLAG,sizeof(testval_t));
    }
    else
    {
        ;//for qac
    }
    
    if(g_uspeaker_24bit == 1)
    {
        sys_unlock_adjust_freq(); 
    }

    //disable detect when init
    DISABLE_USB_DETECT();
    //������ֹͣusb ��ֹͣdsp
    key_register_scan_isr_cbk(NULL);
    ud_module_stop();

    //enable detect after init
    ENABLE_USB_DETECT();

    if (0 == g_uspeaker_24bit)
    {
    //ж�ؽ�������
        sys_free_mmm(TRUE);
    }
    else if(g_pa_type == 1)
    {
        ccd_i2s_pa_set_clock(48);
        //sys_unlock_adjust_freq();
    }
    else
    {
        ;//for QAC
    }
    
    sys_drv_uninstall(DRV_GROUP_UD);

    //ִ��applib���ע������
    applib_quit();

    //save config дvram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{

    app_result_e result;
    g_current_vol = (uint8) argc;

    g_cur_play_status_p = &g_cur_play_status[0];//Ӧ�ü���ʱ��ʼ��Ϊ0�������ٳ�ʼ����

    //���������ѯ����ʱ���뱣֤ǰ̨����ȡ״̬ʱ�ǿ��õ�
    g_cur_play_status_p = sys_share_query_creat(APP_ID_UENGINE, g_cur_play_status_p, sizeof(uengine_status_t));

    if (g_cur_play_status_p == NULL)
    {
        PRINT_ERR("usound share query create fail!\n");
        while (1)
        {
            ;//do noting
        }

    }

    //���������ڴ�
    //libc_memcpy(g_share_ptr, &(setting_comval->dae_cfg), sizeof(dae_config_t));
    g_share_ptr = &g_sync_vol_share;

    if (sys_shm_creat(SHARE_MEM_ID_USOUND, g_share_ptr, sizeof(uengine_sync_volume_t)) == -1)
    {
        PRINT_ERR("usound shm create fail!!");
        while (1)
        {
            ; //nothing for QAC
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

    //��ʼ��
    _app_init();

    //ѭ��
    result = uengine_control_block();//app���ܴ���

    //�˳�
    _app_deinit();

    //���ٹ����ڴ�
    sys_shm_destroy(SHARE_MEM_ID_USOUND);

    //���ٹ����ѯ
    if (sys_share_query_destroy(APP_ID_UENGINE) == -1)
    {
        PRINT_ERR("usound share query destroy fail!\n");
        while (1)
        {
            ;//do noting
        }
    }

    return result;
}
