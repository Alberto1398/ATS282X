/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *        brief   ����������ʼap�Ͳ���
 *      <author>       <time>
 *      Cailizhen          2015-1          creat
 *       Wekan        2015-3-30       review
 *******************************************************************************/

#include  "btcall_engine.h"

/*ȫ�ֱ�������*/
//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     2

app_timer_t btcall_timer_vector[APP_TIMER_COUNT];

uint32 g_dsp_m4k_share_mem[128/4];//dsp-m4k��������

uint32 g_agc_aec_shm_ptr;

uint8 g_stub_tools_type;

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
OS_STK *ptos = (OS_STK *) AP_BACK_LOW_STK_POS;
INT8U prio = AP_BACK_LOW_PRIO;

/******************************************************************************
 * \par  Description:   ��ȡӦ��VM�е�aec�������ò�����config.txt������
 * \param[in]     null0
 * \param[out]   sv_prms ��ȡ�Ĳ�����ŵ�ָ��
 * \return           none
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

void actaec_init_sv_prms(sv_prms_t *sv_prms)
{
    int tmp_value;

    if(actaec_init_sv_prms_from_VM(sv_prms) == TRUE)
    {
        return;
    }
    sv_prms->frame_size = 16; //֡��ʱ�䣬��λms��Ŀǰ��֧�� 16ms ֡��
    sv_prms->sampling_rate = 8000; //�����ʣ���λHz��Ŀǰ��֧�� 8000Hz ������

    // aec prms
    //aecģ�鿪�أ�1���򿪣�0���ر�
    sv_prms->aec_prms.aec_enable = (uint8) com_get_config_default(BTCALL_AEC_MODULE_ENABLE);
    //����β������λ ms����֧��32ms��64ms��96ms��128ms
    sv_prms->aec_prms.tail_length = (uint8) com_get_config_default(BTCALL_AEC_TAIL_LENGTH);

    //�����Դ�����ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ 1
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_NLP_ENABLE);

    sv_prms->aec_prms.nlp_enable = tmp_value; //Ĭ��Ϊ1
    //�����Դ���̶ȣ�����0~15,Խ���������Դ���Խ����
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_NLP_AGGRESSIVENESS);
    sv_prms->aec_prms.nlp_aggressiveness = tmp_value; //Ĭ��Ϊ10
    //˫���������������̶ȣ�����0~15��Խ���ʾ˫���ǽ�����������Խ�࣬���ǿ��ܻ���������Ļ��������Ҹ�����ֵ��
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_DT_THRESHOLD);
    sv_prms->aec_prms.dt_threshold = tmp_value;//Ĭ��Ϊ5
    //��������������ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ1
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_AES_ENABLE);
    sv_prms->aec_prms.aes_enable = tmp_value;//Ĭ��Ϊ1
    //����������ģ�鵥���������Ʊȣ���λdB�� 0~40
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_ECHO_SUPPRESS);
    sv_prms->aec_prms.echo_suppress = tmp_value;//Ĭ��Ϊ40
    //����������ģ��˫���������Ʊȣ���λdB�� 0~40
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_ECHO_SUPPRESS_ACTIVE);
    sv_prms->aec_prms.echo_suppress_avtive = tmp_value;//Ĭ��Ϊ40

    //ns prms
    //���뿪�أ�1���򿪣�0���ر�
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_ANS_ENABLE);
    sv_prms->ns_prms.ans_enable = tmp_value;//Ĭ��Ϊ1
    //�������Ʊȣ���λdB�� 0~25
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_NOISE_SUPPRESS);
    sv_prms->ns_prms.noise_suppress = tmp_value;//Ĭ��Ϊ25

    //cng prms
    //�����������أ�1���򿪣�0���ر�
    sv_prms->cng_prms.cng_enable = (uint8) com_get_config_default(BTCALL_AEC_CNG_ENABLE);
    //��������ˮƽ����λ dB Fs, һ��ӦС��-45 dB��
    tmp_value = (int) (uint32) com_get_config_default(BTCALL_AEC_NOISE_FLOOR);
    sv_prms->cng_prms.noise_floor = tmp_value - 5;//ʵ��ˮƽ�ȸ�ֵ��5db���ң���������-5����ӦĬ��ֵΪ-45
    sv_prms->speaker_channel = (uint8) com_get_config_default(BTCALL_SPEAKER_CHANNEL_ENABLE);
    sv_prms->speaker_channel_weak_sel = (uint8) com_get_config_default(BTCALL_SPEAKER_CHANNEL_WEAK_SEL);
    sv_prms->speaker_channel_weak_val = (uint8) com_get_config_default(BTCALL_SPEAKER_CHANNEL_WEAK_VAL);
    sv_prms->hfp_mode = (uint8) com_get_config_default(BTCALL_AEC_HALF_FULL_MODE);//��ȡͨ����˫����
    sv_prms->pre_agc_level = (uint8) com_get_config_default(BTCALL_AGC_PRETREAT_LEVEL);
    sv_prms->pos_agc_level = (uint8) com_get_config_default(BTCALL_AGC_POSTREAT_LEVEL);
	
	if((g_p_btcall_info != NULL) && (g_p_btcall_info->status == BTCALL_SIRI))
	{
		PRINT_INFO("SIRI aec param setting");
		sv_prms->aec_prms.aec_enable = 0;
		sv_prms->aec_prms.nlp_enable = 0;
		sv_prms->aec_prms.aes_enable = 0;
	}
}


/******************************************************************************
 * \par  Description:   ��ȡӦ��VM�е�aec������ͨ��ASQTд��
 * \param[in]     null0
 * \param[out]   dsp_cfg ��ȡ�Ĳ�����ŵ�ָ��
 * \return          TRUE��ȡ�ɹ�
 * \                   FALSE û�б������
 * \note
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-7-13     creat
 *******************************************************************************/
bool actaec_init_sv_prms_from_VM(sv_prms_t *dsp_aec)
{
    uint8 pbuf[16];
    uint32 i,sum;
    uint8 *p8;
    VM_SAVE_t *parm_head = (VM_SAVE_t *)pbuf;
    sys_vm_read(parm_head, VM_ASQT_PARM_BASE, 8);
    if(parm_head->magic == 0xAA55)
    {
        sys_vm_read(dsp_aec, VM_ASQT_PARM_BASE + 8, sizeof(sv_prms_t));
        p8 = (uint8*)dsp_aec;
        for(i = 0, sum = 0; i < sizeof(sv_prms_t); i++)
        {
            sum += p8[i];
        }
        if(sum == parm_head->cs)
        {
            PRINT_INFO_INT("get AEC VM parms.", parm_head->times);
            return TRUE;
        }
        PRINT_INFO_INT("AEC VM params cs err.", parm_head->cs);
        PRINT_INFO_INT("AEC VM params cs=", sum);
    }
    return FALSE;
}



/******************************************************************************
 * \par  Description:   ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]     null0
 * \param[out]   sv_prms ��ȡ�Ĳ�����ŵ�ָ��
 * \return           none
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

static void _read_var(void)
{
    btcall_gl_var.ignore_uncall_sco = (uint8) com_get_config_default(BTCALL_IGNORE_UNCALLSCO);

    btcall_gl_var.callin_ring_mode = (uint8) com_get_config_default(BTCALL_CALLIN_RING_MODE);
    btcall_gl_var.callin_ring_space_time = (uint8) com_get_config_default(BTCALL_CALLIN_RING_BUILDIN_SPACE_TIME);
    if (btcall_gl_var.callin_ring_space_time == 0)
    {
        btcall_gl_var.callin_ring_space_time = 1; //����100ms
    }

    //ȫ�ֱ�����0��Чֵ��ʼ��
    g_callring_data.callring_timer_id = -1;

    g_p_bt_stack_cur_info = (bt_stack_info_t *) sys_shm_mount(SHARE_MEM_ID_BTSTACK_STATUS);
    if (g_p_bt_stack_cur_info == NULL)
    {
        PRINT_ERR("btstack status shm not exist!\n");
        while (1)
        {
            ;//nothing
        }
    }

    g_p_btcall_info = &g_btcall_info[0];

    switch (g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->hfp_active_id].hfp_status)
    {
        case HFP_STATUS_LINKED:
        g_p_btcall_info->status = BTCALL_STOP;
        break;

        case HFP_STATUS_INCOMING:
        g_p_btcall_info->status = BTCALL_CALLIN;
        break;

        case HFP_STATUS_OUTGOING:
        g_p_btcall_info->status = BTCALL_CALLOUT;
        break;

        case HFP_STATUS_CALLING:
        g_p_btcall_info->status = BTCALL_HFP;
        break;

        case HFP_STATUS_PHONE:
        g_p_btcall_info->status = BTCALL_PHONE;
        break;

		case HFP_STATUS_SIRI:
        g_p_btcall_info->status = BTCALL_SIRI;
        break;
		
        case HFP_STATUS_NONE:
        default:
        g_p_btcall_info->status = BTCALL_IDLE;
        break;
    }

    //���������ѯ����ʱ���뱣֤ǰ̨����ȡ״̬ʱ�ǿ��õ�
    g_p_btcall_info = sys_share_query_creat(APP_ID_BTCALLEG, g_p_btcall_info, sizeof(btcall_info_t));
    if (g_p_btcall_info == NULL)
    {
        PRINT_ERR("btcall share query create fail!\n");
        while (1)
        {
            ;//nothing
        }
    }

    //���������ڴ�
    g_agc_aec_shm_ptr = (uint32)&g_dsp_m4k_share_mem;

    if (sys_shm_creat(SHARE_MEM_ID_AGC_AEC, g_agc_aec_shm_ptr, sizeof(sv_prms_t)) == -1)
    {
        PRINT_ERR("agc aec shm create fail!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }
}

/******************************************************************************
 * \par  Description:   ����VRAM������Ŀǰֻ�ǽӿڣ�δ�й���
 * \param[in]      null
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

static void _save_var(void)
{
    //;//nothing
    //���ٹ����ڴ�
    //g_agc_aec_shm_ptr = (uint32)&g_dsp_m4k_share_mem;

    if (sys_shm_destroy(SHARE_MEM_ID_AGC_AEC) == -1)
    {
        PRINT_ERR("agc aec shm destroy fail!!");
    }
}

/******************************************************************************
 * \par  Description:   appӦ�ó�ʼ���� ������ȡ��ʼ��������
 *\                             app�⣬��ʱ������Ϣ
 * \param[in]      null
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/
static void _app_init(void)
{
    /*����VM����*/
    _read_var();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_BTCALLEG, APP_TYPE_CONSOLE);

    /*��ʼ����ʱ��*/
    init_app_timers(btcall_timer_vector, APP_TIMER_COUNT);

    /*��ʼ�� applib ��Ϣģ��*/
    applib_message_init();

    change_engine_state(ENGINE_STATE_PAUSE);
}

/******************************************************************************
 * \par  Description:   appӦ���˳�,
 * \param[in]      null
 * \param[out]   null
 * \return           null
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

static void _app_exit(void)
{
	sys_free_large_data(asqt_peq_addr.band_peq1_addr);
	sys_free_large_data(asqt_peq_addr.band_peq2_addr);
	
	/*ִ��applib���ע������*/
    applib_quit();

    change_engine_state(ENGINE_STATE_PAUSE);
    _save_var();
}

/******************************************************************************
 * \par  Description:   Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]      ����ֻΪ�ӿ�ͳһ��û��������
 * \param[out]   none
 * \return       ��app-manage��Ӧ���¼�
 * \note
 * \  <author>       <time>        <opt>
 * \  Cailizhen          2015-1          creat
 * \   Wekan       2015-3-30     reviwe
 *******************************************************************************/

int main(int argc, const char *argv[])
{
    app_result_e result = RESULT_NULL;
    /*ap��ʼ��*/
    _app_init();
    com_set_sound_out(SOUND_OUT_START);
    btcall_eg_asqt_deal((uint8) argc);
    result = get_message_loop();
    com_set_sound_out(SOUND_OUT_STOP);

    _app_exit();
    return result;
}

