/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*        brief  btcallͷ�ļ�
*      Cailizhen          2015-1          creat
*       Wekan        2015-3-30       review
*******************************************************************************/


#include "btcall_eg_stub_asqt.h"


//dspĬ�ϲ���
const int dsp_init_set[]={16,8000,  1,64,  1,6,15,  1,40,40,  1,15,  0,-60,  0,256,128,3,10,4,7, 0, 0, 3}; 

void show_dat(char*s,uint32 *pd, uint32 len)
{
    uint32 i;
    PRINT_INFO_INT("addr=", (uint32)pd);
    for(i=0;i<len;i++)
    {    
        libc_print(s, pd[i], 2);
        if((i & 0x07) == 0x07)
        {   
            sys_os_time_dly(1);
        }
    }
}

/******************************************************************************
 * \  par  Description:   ��ʼ��fifo������
 * \  param[in]     null0
 * \  param[out]   null
 * \  return           null
 * \  note
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void init_asqt_fifo(Fifo_ctl_t *p_fifo, uint32 size, uint8*pdata)
{
    p_fifo->in_index = 0;
    p_fifo->out_index = 0;
    p_fifo->data_size = 0;
    p_fifo->total_size = size;
    p_fifo->data = pdata;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ��ʼ��m4k��dsp��������ݿ�
 * \param[in]   en_ASQT_period ��Ҫ�ϴ���ʱ�ڵ�����bit0��ʾʱ��1������
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void init_m4k_dsp_ctl(uint8 en_ASQT_period)
{
    uint8 i;
    uint32 M4k_addr = 0;
    if(en_ASQT_period > 0)
    {
        M4k_addr = (uint32)g_pAsqtRam->share_buf;
    }
    for(i = 0; i < MAX_AEC_PERIOD; i++)
    {
        if((en_ASQT_period & (SET_ENABLE << i)) != 0)
        {
             g_asqt_parm.out_frame_cnt[i] = 0;
             g_p_share_mem->Asqt_ctl[i].frame_cnt = 0;
             g_asqt_parm.m4k_addr[i] = M4k_addr;
             g_p_share_mem->Asqt_ctl[i].block_num = ASQT_PERIOD_BLOCK;
             g_p_share_mem->Asqt_ctl[i].frame_size = 128; //uint16
             g_p_share_mem->Asqt_ctl[i].dsp_addr = (uint32*)ADDR_M4K2DSP(M4k_addr);//M4k_addr���ܴ���0x9fc40000;
             g_p_share_mem->Asqt_ctl[i].period_flag = i + 1;//��Ҫ��ʱ�ڵ�ASQT����
             M4k_addr += 256 * ASQT_PERIOD_BLOCK;
        }
        else
        {
            g_p_share_mem->Asqt_ctl[i].block_num = 0;
            g_p_share_mem->Asqt_ctl[i].frame_size = 0;
            g_p_share_mem->Asqt_ctl[i].period_flag = 0;
            g_p_share_mem->Asqt_ctl[i].dsp_addr = NULL;
        }
    } 
}



uint16 get_bt_air_len(void)
{
    return g_p_bt_stack_cur_info->rmt_dev[g_p_bt_stack_cur_info->hfp_active_id].hfp_attr.air_pkt_len;
}


/******************************************************************************/
/*!
 * \par  Description:����ΪdspĬ�ϵĽ������
 * \param[in]    
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 * \   ��ʵ��绰������mmmʱ����
 *******************************************************************************/
sv_configs_t *asqt_set_dsp_init_parm(void)
{
    static bool read_sv_prms_flag = FALSE;
    sv_configs_t *asqt_parm = NULL;
    PRINT_INFO_INT("set dsp parm:", 0);
    if(btcall_gl_var.stub_asqt_en != 0)//ASQT����ģʽ
    {
        reset_aec_asqt();
        if(g_asqt_parm.AsqtCfg.analysis_mode == ASQT_CALL_MODE)
        {
            g_dsp_init_parm.air_pkt_len = get_bt_air_len();  //�������б��İ�����
        }
        else
        {
            g_dsp_init_parm.air_pkt_len = 0x3C;//����ģʽ�µ�ģ��ͨ��ģʽĬ��0x3C=60 
        }
        show_dat("Dsp:", (uint32*)&g_dsp_init_parm, sizeof(sv_prms_t)/4);
        asqt_parm = &g_asqt_parm.AsqtCfg;
    }
    else 
    {
        if(read_sv_prms_flag != TRUE)
        {
            read_sv_prms_flag = TRUE;
            //�����Ĭ�ϵĲ������ٶ�config.txt������
            libc_memcpy((uint8*)&g_dsp_init_parm, (uint8*)dsp_init_set, sizeof(dsp_init_set));
            actaec_init_sv_prms(&g_dsp_init_parm);
            g_dsp_init_parm.pcm_mode = 0;//��������״̬
            g_dsp_init_parm.air_pkt_len = get_bt_air_len();  //�������б��İ�����
            g_dsp_init_parm.hfp_connect_mod = 0;
            show_dat("dsp:", (uint32*)&g_dsp_init_parm, sizeof(sv_prms_t)/4);
        }
        
    }
    g_p_share_mem->pAEC_parm = (sv_prms_t*)ADDR_M4K2DSP(&g_dsp_init_parm);//ת��dspֱ�ӷ���
    //����2015-5-25֮ǰ�Ŀ�
    //Ҫ��init_m4k_dsp_ctl()������
    if(g_p_share_mem->Asqt_ctl[5].dsp_addr == NULL)//��Ϊ����ƫ����96
    {
        g_p_share_mem->Asqt_ctl[5].dsp_addr = (uint32*)ADDR_M4K2DSP(&g_dsp_init_parm);//ת��dspֱ�ӷ���
    }
    else
    {
        libc_memcpy((uint8*)ADDR_DSP2M4K(g_p_share_mem->Asqt_ctl[5].dsp_addr), (uint8*)&g_dsp_init_parm, sizeof(sv_prms_t));
    }
    return asqt_parm;//return null��ʾ��  config.txt�Ĳ�����������pc-tool�Ĳ���
}


/******************************************************************************/
/*!
 * \par  Description:����aec��������Ҫ�Ǹ��ƴ�pc���õ��Ĳ���
 * \param[in]      in_parmԴ����
 * \param[out]   dsp_parm   dsp����ָ��
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1     creat
 *******************************************************************************/
void copy_dsp_aec_parm(sv_prms_t *dsp_parm, sv_configs_t *in_parm)
{
    uint32 tmp_dac_weak;
    libc_memcpy((uint8*)dsp_parm, (uint8*)&in_parm->frame_size, (2 + 8 + 2 + 2)*4);
    
    dsp_parm->aec_prms.tail_length = (dsp_parm->aec_prms.tail_length + 8) & (~0x0F);//������16��
    if(dsp_parm->aec_prms.tail_length == 0)
    {
        if(g_app_info_state_all.stub_phy_type == STUB_PHY_USB)
        {
            dsp_parm->aec_prms.tail_length = 128;
        }
        else
        {
            //��������
            dsp_parm->aec_prms.tail_length = 64;
        }
    }
    if(in_parm->analysis_mode == 0)
    {
        dsp_parm->pcm_mode = 1;
    }
    else
    { 
        dsp_parm->pcm_mode = 0;   
    }
    dsp_parm->air_pkt_len = get_bt_air_len();  //�������б��İ�����

    dsp_parm->speaker_channel = in_parm->speaker_channel;
    if(in_parm->speaker_channel != 3)//˫������˥��
    {
        tmp_dac_weak = 0;
    }
    else
    {
        tmp_dac_weak = in_parm->dac_weak;
    }
    if((tmp_dac_weak > 0) && (tmp_dac_weak < 19))
    {    
        dsp_parm->speaker_channel_weak_sel = 2;//��
        dsp_parm->speaker_channel_weak_val = in_parm->dac_weak;
    }
    else if((tmp_dac_weak > 32) && (tmp_dac_weak < 51))
    {
        dsp_parm->speaker_channel_weak_sel = 1;
        dsp_parm->speaker_channel_weak_val = in_parm->dac_weak - 33;
    }
    else
    {
        dsp_parm->speaker_channel_weak_sel = 0;
        dsp_parm->speaker_channel_weak_val = 0;
    }
    
    dsp_parm->hfp_mode = in_parm->hfp_mode;//0��˫��1�ǵ���
    if(in_parm->pre_agc_level < 0xFF)
    {
        dsp_parm->pre_agc_level = in_parm->pre_agc_level;
        dsp_parm->pos_agc_level = in_parm->pos_agc_level;
    }
    else
    {
        dsp_parm->pre_agc_level = in_parm->pre_agc_level + 30;
        dsp_parm->pos_agc_level = in_parm->pos_agc_level + 30; 
    }
    dsp_parm->agc_pre_en = in_parm->pre_agc_enable;
    dsp_parm->agc_back_en = in_parm->pos_agc_enable;
    
    dsp_parm->frame_size = 16;//�̶�
    dsp_parm->sampling_rate = 8000; //�̶�
}



/******************************************************************************/
/*!
 * \par  Description:
 *  ����dsp��ʵ��pc����ʵʱ��Ч
 * \param[in]    call_mode ģ�����ʵ��绰
 * \param[out]   none
 * \return          none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-8-11      creat
 *******************************************************************************/

void reset_dsp(uint8 call_mode)
{
    if(call_mode == ASQT_CALL_MODE)
    {
        if(g_p_btcall_info->status == BTCALL_HFP)//����ͨ������Ҫ����dsp
        {
            btcalleg_free_hfp_mmm();
            reset_aec_asqt();
            btcalleg_load_hfp_mmm();
        }
    }
    else
    {
        btcalleg_free_hfp_mmm();
        reset_aec_asqt();
        deal_load_simu_mmm();
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 *  ֪ͨpc-tool���ϴ����ݽ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void stub_asqt_send_end(uint8 cmd)
{ 
    stub_set_data(cmd << 8, g_pAsqtRam->stub_buf, 0);
}




/******************************************************************************/
/*!
 * \par  Description: С����ȡ���������浽Vram
 * \param[in]    stub_buf����ռ�--����512
 * \param[out]   none
 * \return       -1ʧ��,0�ɹ�
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-7-13      creat
 *******************************************************************************/
int stub_asqt_down_param(uint32 *stub_buf)
{ 
    int ret;
    uint32 i, sum;
    uint8*p8;
    VM_SAVE_t *pParm = (VM_SAVE_t *)stub_buf;
    sv_configs_t *pc_parms = (sv_configs_t *)(stub_buf + (sizeof(VM_SAVE_t)/4 + 1));
    ret = stub_get_data(0x0900, (uint32*)pc_parms, sizeof(sv_configs_t));
    if(ret > -1)
    {
        copy_dsp_aec_parm(&pParm->dsp_cfg, (sv_configs_t*)stub_buf);
        sys_vm_read(pParm, VM_ASQT_PARM_BASE, 8);
        if(pParm->magic != 0xAA55)
        {
            pParm->times = 1;
        }
        else
        {
            pParm->times++;
        }
        pParm->magic = 0xAA55;
        p8 = (uint8*)&pParm->dsp_cfg;
        for(i = 0, sum = 0; i < sizeof(sv_prms_t); i++)
        {
            sum += p8[i];
        }
        pParm->cs = sum;
        sys_vm_write(pParm, VM_ASQT_PARM_BASE, sizeof(VM_SAVE_t));
        PRINT_INFO_INT("btcall-write-vm-times:", pParm->times);
    }
    return ret;
}



/******************************************************************************/
/*!
 * \par  Description:��λaec�����ڴ�ֵ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
int reset_aec_asqt(void)
{
    init_asqt_fifo(&g_asqt_parm.ppv_fifo, sizeof(g_pAsqtRam->ppv_data), (uint8*)g_pAsqtRam->ppv_data);
    init_asqt_fifo(&g_asqt_parm.mic_fifo, sizeof(g_pAsqtRam->mic_data), (uint8*)g_pAsqtRam->mic_data);
    init_asqt_fifo(&g_asqt_parm.agc_fifo, sizeof(g_pAsqtRam->agc_data), (uint8*)g_pAsqtRam->agc_data);
    init_asqt_fifo(&g_asqt_parm.plc_fifo, sizeof(g_pAsqtRam->plc_data), (uint8*)g_pAsqtRam->plc_data);
    if(stub_asqt_get_parameter(g_pAsqtRam->rev_buf, &g_asqt_parm.AsqtCfg) > -1)//����rev_buf
    {
        libc_memcpy((uint8*)&g_dsp_init_parm, (uint8*)dsp_init_set, sizeof(dsp_init_set));
        copy_dsp_aec_parm(&g_dsp_init_parm, &g_asqt_parm.AsqtCfg);
    }
    init_m4k_dsp_ctl(0x3F);// 1111111     
    return 0;
}


/******************************************************************************/
/*!
 * \par  Description: ����asrc-inΪcpu��д
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void set_asrc_cpu_clk(void)
{
    act_writel(act_readl(PCM_CTL) & (~(3u << 4)),PCM_CTL);//��pcm
    act_writel(act_readl(ASRC_IN_CTL) & (~((1u << 1)|(3u<<11))), ASRC_IN_CTL);
    act_writel(act_readl(ASRC_OUT0_CTL) & (~((1u << 1)|(3u<<11))), ASRC_OUT0_CTL);
    set_asrc_in_wclk_cpu();
    set_asrc_out0_rclk_cpu();
    act_writel(act_readl(ASRC_IN_CTL) | (1u << 1)|(3u<<11), ASRC_IN_CTL);
    act_writel(act_readl(ASRC_OUT0_CTL) | (1u << 1)|(3u<<11), ASRC_OUT0_CTL);
}



/******************************************************************************/
/*!
 * \par  Description:����ģ���绰���м��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void deal_load_simu_mmm(void)
{
    asqt_set_dsp_init_parm();
    btcalleg_load_hfp_mmm();
    btcall_gl_var.g_sco_deal_flag = 0;//�������߳��˳�mmm
    set_asrc_cpu_clk();
}

int8 asqt_init(void)
{
	asqt_interface_config_t config_info; 
	uint8 asqt_version[10];
	asqt_version[0] = 'U';
	asqt_version[1] = 'S';
	asqt_version[2] = '2';
	asqt_version[3] = '8';
	asqt_version[4] = '2';
	asqt_version[5] = 'A';
	asqt_version[6] = '1';
	asqt_version[7] = '2';
	asqt_version[8] = '5';
	asqt_version[9] = '\0';
	
	libc_memset((uint8*)&config_info, 0x00, sizeof(asqt_interface_config_t));
	config_info.eq_point_nums = STUB_CMD_ASQT_MAQ_EQ_NUMS;
	config_info.eq_version = 0;
	libc_memcpy(config_info.fw_version, asqt_version, sizeof(asqt_version));
	stub_asqt_write_data(STUB_CMD_ASQT_WRITE_CONFIG_INFO,&config_info,sizeof(asqt_interface_config_t));
	
	return 0;
}

/******************************************************************************
 * \par  Description:   ����asqt���ܣ����config.txt����ʹ�ܣ�
                                 ���첽��Ϣ�����棬�����ʼ��ΪĬ�ϲ���
 * \param[in]     null0
 * \param[out]   none
 * \return           none
 g_p_share_mem->Asqt_ctl,  g_p_share_mem->pAEC_parm  ,g_dsp_parm  ,g_pAsqtRam�����ڿ�ʼǰ��ʼ���õ�ַ
 *   <author>    <time> 
 *    Wekan   2015-4-01  ����ͨ�������õ�0x16c00�Ŀռ�
 *******************************************************************************/

void btcall_eg_asqt_deal(uint8 stub_tool_type)
{
    //g_agc_aec_shm_ptrָ���ڽ���apʱ�Ѿ����ݸ�dsp
    g_p_share_mem = (dsp_m4k_share_t *)g_agc_aec_shm_ptr;
    g_stub_tools_type = stub_tool_type;
    libc_memset((uint8*)&g_asqt_parm, 0x00, sizeof(g_asqt_parm));
    libc_memset((uint8*)g_p_share_mem, 0x00, sizeof(dsp_m4k_share_t));

	asqt_peq_addr.band_peq1_addr = sys_malloc_large_data(STUB_CMD_ASQT_MAX_BYTES_EVERY_PACKAGE);
	if (NULL == asqt_peq_addr.band_peq1_addr)
	{
		PRINT_INFO("asqt peq1 malloc fail");
	}
	
	asqt_peq_addr.band_peq2_addr = sys_malloc_large_data(STUB_CMD_ASQT_MAX_BYTES_EVERY_PACKAGE);	
	if (NULL == asqt_peq_addr.band_peq2_addr)
	{
		PRINT_INFO("asqt peq2 malloc fail");
	}
		
    init_m4k_dsp_ctl(0);

    if(stub_tool_type == STUB_PC_TOOL_ASQT_MODE)
    {
		PRINT_INFO("---start stub asqt---");
		asqt_init();
		
		btcall_gl_var.stub_asqt_en = 1;
        g_pAsqtRam = (sv_asqt_ram_t *)&g_btcall_asqt_test_buf;
        if(g_stub_task_timer_id == -1)//��ʱ��ֻ����һ��
        {
            g_stub_task_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL, 10, task_timer10ms);
        }
        PRINT_INFO_INT("asqt start.(0x2A00>)=",sizeof(sv_asqt_ram_t));
    }
}   	

/******************************************************************************/
/*!
 * \par  Description: ��ȡstub���Ե����ͣ�asqtΪ01
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
uint32 get_stub_type(void)
{
    uint32 type;
    stub_get_data(0xFF00, &type, 4);
    return type;
}


