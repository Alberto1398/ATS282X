


/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*        brief   ����������ʼap�Ͳ���
*      <author>       <time>  
*      Cailizhen          2015-1          creat
*       Wekan        2015-3-30       review
*******************************************************************************/

#include "btcall_eg_stub_asqt.h"



sv_asqt_ram_t *g_pAsqtRam;

dsp_m4k_share_t *g_p_share_mem;

sv_prms_t g_dsp_init_parm;//���dsp��ʼ������

uint8 g_real_call_mode = 0;

int8 g_stub_task_timer_id = -1;//��ʱ��id

asqt_parm_t g_asqt_parm;

asqt_peq_addr_t asqt_peq_addr;

PC_curStatus_e stub_asqt_tools_status(void);


/******************************************************************************/
/*!
 * \par  Description: дfifo���������̻��ȫ���жϣ�ʱ����us��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
uint32 asqt_write_fifo(Fifo_ctl_t *fifo, uint8 *buf, uint32 len)
{
    uint32 irq_save; 
    uint32 ret;
    irq_save = sys_local_irq_save();
    for(ret = 0; ret < len; ret++)
    {
        if(fifo->data_size == fifo->total_size)//д��
        {
            sys_irq_print("asqt-full.&",(uint32)fifo,2);
            break;
        }
        ((uint8*)fifo->data)[fifo->in_index] = buf[ret];
        fifo->data_size++;
        fifo->in_index++;
        if(fifo->in_index >= fifo->total_size)
        {
            fifo->in_index  = 0;
        }
    }
    sys_local_irq_restore(irq_save);
    return ret;     
}


/******************************************************************************/
/*!
 * \par  Description: ��fifo���������̻��ȫ���жϣ�ʱ����us��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
uint32 asqt_read_fifo(Fifo_ctl_t *fifo, uint8 *buf, uint32 len)
{
    uint32 irq_save;
    uint32 ret;
    irq_save = sys_local_irq_save();
    for(ret = 0; ret < len; ret++)
    {
        if(fifo->data_size == 0)//û����
        {
            break;
        }
        buf[ret] = ((uint8*)fifo->data)[fifo->out_index];
        fifo->data_size--;
        fifo->out_index++;
        if(fifo->out_index >= fifo->total_size)
        {
            fifo->out_index  = 0;
        }
    }
    sys_local_irq_restore(irq_save);
    return ret;
}


/******************************************************************************/
/*!
 * \par  Description:��ȡASQT֡�ĵ�ַ
 * \param[in]    index ʱ����
 * \param[out]   null
 * \return       ֡���ݵ�ַ
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-9-9      creat
 *******************************************************************************/
uint32 get_asqt_frame_addr(uint8 index)
{
    uint32 addr;
    addr = 256L * (g_asqt_parm.out_frame_cnt[index] % ASQT_PERIOD_BLOCK);
    addr = addr + g_asqt_parm.m4k_addr[index];
    g_asqt_parm.out_frame_cnt[index]++;
    return addr;
}

/******************************************************************************/
/*!
 * \par  Description:д���ݽ�fifo
 * \param[in]   fifo 
 * \param[index]    ʱ����
 * \param[out]     null
 * \return       TRUE д��ɹ�
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-9-9      creat
 *******************************************************************************/

bool push_to_fifo(Fifo_ctl_t *fifo ,uint8 index)
{
    
    if(fifo->total_size >= (fifo->data_size + 256))
    {
        asqt_write_fifo(fifo, (uint8*)get_asqt_frame_addr(index), 256);
        return TRUE;
    }
    return FALSE;
}



/******************************************************************************/
/*!
 * \par  Description: д���ݵ�asrc��һ�����д120��(240B)��10�������һ��
 * \param[in]    none
 * \param[out]   none
 * \return       ����д��asrc�ĵ���
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
uint32 send_data_to_asrc(Fifo_ctl_t *p_fifo)
{   
    static bool asrc_dat_new = FALSE;
    static uint32 asrc_dat = 0;
    uint32 ret = 0;
    uint32 timeout = 0;
    while(ret < 160)//10m = 80,1point = 2bytes
    {
        timeout++;
        
        if(asrc_dat_new == FALSE)
        {
            if(asqt_read_fifo(p_fifo, (uint8*)&asrc_dat, 2) == 0)
            {
                break;
            }
            asrc_dat_new = TRUE;
        }
        
        if ((act_readl(ASRC_IN_IP) & (1 << ASRC_IN_IP_WFIFOEmpty)) != 0)
        {
            asrc_dat <<= 16;
            act_writel(asrc_dat, ASRC_IN_WFIFO);
            act_writel(asrc_dat, ASRC_IN_WFIFO);
            asrc_dat_new = FALSE;
            timeout = 0;
            ret++; 
        }
        else
        {
            if(timeout > 1000)
            {
                break;
            }
        }
    }
    return ret;
    
}

/******************************************************************************/
/*!
 * \par  Description: С����PC��������
 * \param[in]    
 * \param[out] 
 * \return       
 * \ 
 * \ 
 *******************************************************************************/
uint32 stub_asqt_write_data(uint16 cmd, void* buf, uint16 len)
{
	uint8* buf_temp = NULL;
	uint8  ret_val = 0;
	uint8  err_cnt = 0;
	
	buf_temp = (uint8*)buf;
	
retry:	

	ret_val = stub_set_data(cmd, buf_temp, len-STUB_COMMAND_HEAD_SIZE);
	if (0 != ret_val)
	{
		err_cnt++;
		PRINT_ERR("---stub asqt write data error---");
		if (err_cnt < 10)
		{
			goto retry;
		}	
	}
	
    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description: С����PC��������
 * \param[in]    
 * \param[out] 
 * \return       
 * \ 
 * \ 
 *******************************************************************************/
uint32 stub_asqt_read_data(uint16 cmd, void* buf, uint16 len)
{
	uint8* buf_temp = NULL;
	
	buf_temp = buf;
	
	stub_get_data(cmd, buf_temp, len);
	
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description: С����ȡPC�������õ�����
 * \param[in]    stub_buf����ռ�--����8
 * \param[out]   none
 * \return       ״̬���--ʹ�ý����Ҫ&0xFF
 * \  <author>       <time>        <opt>
 * \   none       2016-6-1      none
 *******************************************************************************/
uint32 stub_asqt_get_volums(uint32 *stub_buf)//0x0d
{
    stub_get_data(0x0d00, stub_buf, 4);
    return stub_buf[0];
}

/******************************************************************************/
/*!
 * \par  Description: С����ȡPC����״̬
 * \param[in]    stub_buf����ռ�--����8
 * \param[out]   none
 * \return       ״̬���--ʹ�ý����Ҫ&0xFF
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/

uint32 stub_asqt_get_status(uint32 *stub_buf)//0x01
{
    stub_get_data(0x0100, stub_buf, 4);
    return stub_buf[0];
}


/******************************************************************************/
/*!
 * \par  Description: С����ȡPC�еĲ���
 * \param[in]    stub_buf����ռ�--����512
 * \param[out]   none
 * \return       ״̬���--ʹ�ý����Ҫ&0xFF
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
int stub_asqt_get_parameter(uint32 *stub_buf, sv_configs_t *parm)//0x02
{
    int ret = -2;
    try_again:
    ret = stub_get_data(0x0200, stub_buf, sizeof(sv_configs_t));
    if(ret != -1)
    {
        if(parm != NULL)
        {
            libc_memcpy((uint8*)parm, (uint8*)stub_buf, sizeof(sv_configs_t));
        }
        g_real_call_mode = ((sv_configs_t*)stub_buf)->analysis_mode;
        if(ret != sizeof(sv_configs_t))//������ǰ�ɵ�pc-tool
        {
            ((sv_configs_t*)stub_buf)->hfp_mode = 1;//Ĭ����˫��
        }
    }
    else
    {
        sys_os_time_dly(50);
        libc_print("asqt-get-parm-err", 0, 0);
        goto try_again;//������һ�㲻������������һֱ��ѭ
    }
    return ret;
}


/******************************************************************************/
/*!
 * \par  Description:PC��ȡС����Զ���������ݣ����ݸ�ʽΪ��8KHz�����ʣ�16bit�ֱ���
 * \param[in]   stub_buf ��Ҫ�ϴ������ݵ�ָ��
 * \param[out]   none
 * \return       non
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
bool stub_asqt_get_remote_voice(uint32 *stub_buf)//0x03
{
    bool ret = FALSE; 
    //160�ǹ̶����ȣ�10ms�Ĳ�������
    if((g_asqt_parm.ppv_fifo.data_size + 160) <= g_asqt_parm.ppv_fifo.total_size)
    {
        if(stub_get_data(0x0300, stub_buf, 160) > -1)
        {
            asqt_write_fifo(&g_asqt_parm.ppv_fifo, (uint8*)stub_buf, 160);
            ret = TRUE;   
        }
    }
    return ret;
}

/******************************************************************************/
/*!
*   �ϴ�ԭʼ�绰����
* \param[in]   stub_buf ��Ҫ�ϴ������ݵ�ָ��
* \param[out]   none
* \return       non
* \  <author>       <time>        <opt>
* \   Wekan       2015-4-1      creat
*******************************************************************************/
#if CASE_TYPE_2825//���䷽��

void stub_asqt_send_remote_phone_volce(uint32 *stub_buf)//0x06
{
    stub_buf[0] = 0;
    stub_buf[1] = 0;
    stub_set_data(0x0600, stub_buf, 160);
}

#endif
/******************************************************************************/
/*!
*   �ϴ�ʱ��PLC��AEC������
* \param[in]   stub_buf ��Ҫ�ϴ������ݵ�ָ��
* \param[out]   none
* \return       non
* \  <author>       <time>        <opt>
* \   Wekan       2015-9-1      creat
*******************************************************************************/

#if CASE_TYPE_2825 //���䷽��


void stub_asqt_send_result_ext(uint32 *stub_buf ,uint32 order ,uint32 frama_cnt)//0x0A & 0B 
{
    uint8 cur_index;
    uint8 *pbuf = (uint8*)&stub_buf[2];
    if(order == 0x0A00)
    {
        cur_index = 1;
    }
    else//0x0B00
    {
        cur_index = 4;
    }
    while(g_asqt_parm.out_frame_cnt[cur_index] < frama_cnt)
    {
        stub_buf[0] = 0;
        stub_buf[1] = 0;
        libc_memcpy(pbuf, (uint8*)get_asqt_frame_addr(cur_index), 256);
        stub_set_data(order, stub_buf, 256);
    }
}

#endif




#if CASE_TYPE_2823 //���������ϲ�tel-plc-aec����
void stub_asqt_send_result_merger(uint32 *stub_buf,uint32 dsp_cnt)//�ϲ�06 0A 0B����
{
    uint8 *pbuf;
    while(g_asqt_parm.out_frame_cnt[4] < dsp_cnt)
    {
        stub_buf[0] = 0;
        stub_buf[1] = 0;
        pbuf = (uint8*)&stub_buf[2];
        if(g_real_call_mode == ASQT_CALL_MODE)//��绰��ԭ�绰����
        {
            asqt_read_fifo(&g_asqt_parm.ppv_fifo, pbuf, 256);//tel=0x06 
        }
        else
        {
            libc_memset(pbuf, 0x00, 256);
        }
        pbuf += 256;
        
        asqt_read_fifo(&g_asqt_parm.plc_fifo, pbuf, 256);;//plc=0x0A
        pbuf += 256;
       
        libc_memcpy(pbuf, (uint8*)get_asqt_frame_addr(4), 256);//AEC

        stub_set_data(0x0C00, stub_buf, 256 * 3);
    }
}

#endif

/******************************************************************************/
/*!
 * \par  Description:  �ϴ�������ݣ���˷�¼����Ԥ�����绰����
 *    AEC���������AEC���ܲ���
 * \param[in]   stub_buf ��Ҫ�ϴ������ݵ�ָ��
 * \param[in]   pResult �������
 * \param[out]   none
 * \return       non
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/

void stub_asqt_send_result(uint32 *stub_buf)//0x04
{
    uint8 *pbuf = (uint8*)&stub_buf[2];
    uint8 tm_buf[12];
    stub_buf[0] = 0;
    stub_buf[1] = 0;
    //mic¼������
    asqt_read_fifo(&g_asqt_parm.mic_fifo, pbuf, 256);
    pbuf += 256;
    //Ԥ����绰����
    asqt_read_fifo(&g_asqt_parm.agc_fifo, pbuf, 256);
    pbuf += 256 - 8;
    libc_memcpy(tm_buf, pbuf - 4, 12);
    libc_memcpy(pbuf, tm_buf, 12);//δ֪bug��512֮���ظ�4�ֽ�
    pbuf += 12;
    
    libc_memcpy(pbuf, (uint8*)get_asqt_frame_addr(5), 256);//pOut_bt
    pbuf += 256;
    //����
    libc_memcpy(pbuf, (uint8*)&g_asqt_parm.AEC_info, sizeof(sv_aec_info_t));
    stub_set_data(0x0400, stub_buf, 256*3u + sizeof(sv_aec_info_t) + 4);
}


/******************************************************************************/
/*!
 * \par  Description:  �ϴ�������ݣ���˷�¼����Ԥ�����绰����
 *    AEC���������AEC���ܲ��� 0x0e �����0x04������һ�������ǲ���Ҫ����4���ֽ�����
 * \param[in]   stub_buf ��Ҫ�ϴ������ݵ�ָ��
 * \param[in]   pResult �������
 * \param[out]   none
 * \return       non
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
//void stub_asqt_send_result_0x0e(uint32 *stub_buf)//0x0e
//{
//    uint8 *pbuf = (uint8*)&stub_buf[2];
//    uint8 tm_buf[12];
//    stub_buf[0] = 0;
//    stub_buf[1] = 0;
//    //mic¼������
//    asqt_read_fifo(&g_asqt_parm.mic_fifo, pbuf, 256);
//    pbuf += 256;
//    //Ԥ����绰����
//    asqt_read_fifo(&g_asqt_parm.agc_fifo, pbuf, 256);
//    pbuf += 256 - 8;
//    libc_memcpy(tm_buf, pbuf - 4, 12);
//    libc_memcpy(pbuf, tm_buf, 12);//δ֪bug��512֮���ظ�4�ֽ�
//    pbuf += 12;
//    
//    libc_memcpy(pbuf, (uint8*)get_asqt_frame_addr(5), 256);//pOut_bt
//    pbuf += 256;
//    //����
//    libc_memcpy(pbuf, (uint8*)&g_asqt_parm.AEC_info, sizeof(sv_aec_info_t));
//    stub_set_data(0x0400, stub_buf, 256*3u + sizeof(sv_aec_info_t) + 4);
//}


/******************************************************************************/
/*!
 * \par  Description:
 *   ��ȡdsp������̵����ݣ�ͨ�������ڴ�
 * \param[in]    pResult ��Ż�ȡ�����ݵ�ָ���ַ
 * \param[out]   none
 * \return       true�����ݸ��£�falseû��������
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void asqt_get_result_and_send(uint32 *stub_buf)
{
    uint32 frame_tmp[6];
    uint8 i;
    //��֤֡��ͬ��
    for(i = 0; i < MAX_AEC_PERIOD;i++)
    {
        frame_tmp[i] = (vint32)g_p_share_mem->Asqt_ctl[i].frame_cnt;
    }

    //ǰ���ĸ�ʱ�ڵ����ݲ���Ҫ�Ⱥ�����ʱ��Լ��40ms
    //����Ҫ��push_to_fifo�Ȼ���

    if(g_real_call_mode == ASQT_CALL_MODE)//��绰��ԭ�绰����
    {
        while(g_asqt_parm.out_frame_cnt[0] < frame_tmp[0])
        {
            if(g_app_info_state_all.stub_phy_type == STUB_PHY_USB)
            {
                push_to_fifo(&g_asqt_parm.ppv_fifo, 0);
                while(g_asqt_parm.ppv_fifo.data_size >= 160)//��绰���ŵ绰������
                {
                    asqt_read_fifo(&g_asqt_parm.ppv_fifo, (uint8*)&stub_buf[2], 160);
                    stub_asqt_send_remote_phone_volce((uint32*)stub_buf);
                }
            }
            else
            {
                if(push_to_fifo(&g_asqt_parm.ppv_fifo, 0) == FALSE)//д��
                {
                    break;
                }
            }
        }
    }

    if(g_app_info_state_all.stub_phy_type == STUB_PHY_USB)
    {
        stub_asqt_send_result_ext(stub_buf ,0x0A00, frame_tmp[1]);//plc
    }
    else
    {
        while(g_asqt_parm.out_frame_cnt[1] < frame_tmp[1])//plc
        {
            if(push_to_fifo(&g_asqt_parm.plc_fifo, 1) == FALSE)//д��
            {
                break;
            }
        }
    }

    while(g_asqt_parm.out_frame_cnt[2] < frame_tmp[2])//agc(speaker)
    {
        if(push_to_fifo(&g_asqt_parm.agc_fifo, 2) == FALSE)//д��
        {
            break;
        }
    }
    while(g_asqt_parm.out_frame_cnt[3] < frame_tmp[3])//mic
    {
        if(push_to_fifo(&g_asqt_parm.mic_fifo, 3) == FALSE)//д��
        {
            break;
        }
    }

    
    if(g_app_info_state_all.stub_phy_type == STUB_PHY_USB)
    {    
        stub_asqt_send_result_ext(stub_buf ,0x0B00, frame_tmp[4]);//aec
    }
    else
    {
        stub_asqt_send_result_merger(stub_buf,frame_tmp[4]);//�ϲ�tel plc aec
    }
    
    while(g_asqt_parm.out_frame_cnt[5] < frame_tmp[5])
    {
        //libc_memcpy(&g_asqt_parm.AEC_info,frame_tmp,6*4);//����
        //libc_memcpy(&g_asqt_parm.AEC_info.maxrefenv,g_asqt_parm.out_frame_cnt,5*4);//����
        //ʱ��6�У���Ϊ3\4\6������
        // 2 ��3��������ʱ���pOutҪ��Լ40ms
        stub_asqt_send_result(stub_buf);
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 *   ��ʵ��绰���ԣ����й���
 * \param[in]    none
 * \param[out]   none
 * \return       non
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void real_call_mode_deal(PC_curStatus_e pc_tool_status, bool reset_dsp_falg)
{
    static btcall_status_e last_call_status = BTCALL_IDLE;
    static uint8 tool_step = 0;
    btcall_status_e call_sta = g_p_btcall_info->status;
    uint8 i;
    if(reset_dsp_falg == TRUE)
    {
        tool_step = 0;
    }
    if(tool_step == 0x05)
    {
       stub_asqt_send_end(0x07);//���ݾɰ汾
       tool_step = 0;
    }
    //�����������ݿռ��ͻ��������hfp���ܵ�����
    if(last_call_status != call_sta)
    {
        if((call_sta != BTCALL_HFP) && (last_call_status == BTCALL_HFP))
        {
            stub_asqt_send_end(0x05);
            tool_step = 0x05;
        }
    }
    last_call_status = call_sta;//��һ��״̬
    
    if((pc_tool_status == sUserStart) && (call_sta == BTCALL_HFP))
    {
        if(tool_step == 0)
        {
            for(i = 0; i < MAX_AEC_PERIOD; i++)
            {
                g_asqt_parm.out_frame_cnt[i] = (vint32)g_p_share_mem->Asqt_ctl[0].frame_cnt;
            }
            tool_step = 1;
            for(i = 0; i < 6; i++)
            {
                PRINT_INFO_INT("dsp-print:", g_p_share_mem->debug[i]);
            }
        }
        asqt_get_result_and_send((uint32*)g_pAsqtRam->stub_buf);
    }
   
}



/******************************************************************************/
/*!
 * \par  Description:
 *   ģ���绰���ԣ����й���
 * \param[in]    none
 * \param[out]   none
 * \return       non
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void simu_call_mode_deal(PC_curStatus_e pc_tool_status)
{
    static bool g_mmm_need_load = TRUE;
    if((pc_tool_status == sUserStop) && (g_mmm_need_load == FALSE))
    {
        stub_asqt_send_end(0x05);
        btcalleg_free_hfp_mmm();
        g_mmm_need_load = TRUE;
    }
    
    if(pc_tool_status != sUserStart)
    {
        return;
    }
    
    if(g_mmm_need_load == TRUE)
    {
        deal_load_simu_mmm();
        g_mmm_need_load = FALSE;
    }
    
    while(stub_asqt_get_remote_voice((uint32*)g_pAsqtRam->stub_buf) == TRUE)
    {
        ;//getԶ������
    }
    
    if(g_asqt_parm.ppv_fifo.data_size > 1)
    {
        send_data_to_asrc(&g_asqt_parm.ppv_fifo);
    }
    else
    {
        stub_asqt_send_end(0x05);
    }
    asqt_get_result_and_send((uint32*)g_pAsqtRam->stub_buf);

}

void __section__(".rcode")get_asqt_peq_para_from_pc(asqt_peq_status_t* p_peq_status_t)
{
    stub_asqt_read_data(STUB_CMD_ASQT_READ_EQ1_DATAS, asqt_peq_addr.band_peq1_addr, p_peq_status_t->package1_len);

	if (STUB_CMD_ASQT_MAQ_EQ_NUMS > STUB_CMD_ASQT_EQ_NUMS_EVERY_PACKAGE)
	{
		stub_asqt_read_data(STUB_CMD_ASQT_READ_EQ2_DATAS, asqt_peq_addr.band_peq2_addr, p_peq_status_t->package2_len);
	}

	g_p_share_mem->peq_info.band_peq1   = ADDR_M4K2DSP((uint32)asqt_peq_addr.band_peq1_addr);
	g_p_share_mem->peq_info.band_peq2   = ADDR_M4K2DSP((uint32)asqt_peq_addr.band_peq2_addr);
	g_p_share_mem->peq_info.change_flag = 1;
	
#if 0
    PRINT_INFO_INT("change_flag", g_p_share_mem->peq_info.change_flag); 	
	PRINT_DATA(asqt_peq_addr.band_peq1_addr,STUB_CMD_ASQT_MAX_BYTES_EVERY_PACKAGE);
	PRINT_DATA(asqt_peq_addr.band_peq2_addr,STUB_CMD_ASQT_MAX_BYTES_EVERY_PACKAGE);	
#endif    
}

void __section__(".rcode")update_asqt_para(void)
{
    asqt_peq_status_t	peq_status; 
    stub_asqt_read_data(STUB_CMD_ASQT_READ_PEQ_STATUS, 	&peq_status, sizeof(asqt_peq_status_t));

    if (TRUE == peq_status.update_flag)
    {
        get_asqt_peq_para_from_pc(&peq_status);
    }
}

/******************************************************************************/
/*!
 * \par  Description:10m��ʱ��
 * \param[in]    none
 * \param[out]   none
 * \return       non
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
void __section__(".rcode") task_timer10ms(void)
{
    static  uint16 task_timer_dly = 0;
    static PC_curStatus_e last_pc_tool_status = sNotReady;
    PC_curStatus_e pc_tool_status = stub_asqt_tools_status();
    bool reset_dsp_falg = FALSE;
   
    if(task_timer_dly  < 500)//�����tts��ͻ
    {
        task_timer_dly++;
        if(task_timer_dly == 500)
        {
            reset_aec_asqt();
        }
        return;
    }
    if((pc_tool_status == sUserUpdate) && (last_pc_tool_status == sUserStart))
    {
        reset_dsp(g_real_call_mode);
        PRINT_INFO("reset dsp");
        reset_dsp_falg = TRUE;
    }
    last_pc_tool_status = pc_tool_status;
    if(g_real_call_mode == ASQT_CALL_MODE)
    {
        real_call_mode_deal(pc_tool_status, reset_dsp_falg);
    }
    else
    {
        simu_call_mode_deal(pc_tool_status);
    }

	if (1 == btcall_gl_var.dsp_load_flag)
	{
        update_asqt_para();
	}		
}



/******************************************************************************/
/*!
 * \par  Description:
 *   ��ʱ��ȡpc-tool��״̬
 * \param[in]    none
 * \param[out]   none
 * \return       ��������ȡ��pc-tool��״̬
 * \  <author>       <time>        <opt>
 * \   Wekan       2015-4-1      creat
 *******************************************************************************/
PC_curStatus_e __section__(".rcode") stub_asqt_tools_status(void)
{

    static uint32 get_sta_timer_ab = 0;//��ȡ��ʱ��㣬��pc-tool״̬
    static PC_curStatus_e pc_tool_status = sNotReady;//pc-tool״̬
    uint32 sys_time;
    PC_curStatus_e pc_status_new;
    uint32 pbuf[4];
    sys_time = sys_get_ab_timer();
    if(sys_time > (get_sta_timer_ab + 500))//500ms��һ��
    {
        pc_status_new = (PC_curStatus_e)stub_asqt_get_status(pbuf);
        if( (pc_status_new == sUserUpdate)||
            ((pc_status_new == sUserStart) && (pc_tool_status != pc_status_new))
           )
        {
            stub_asqt_get_parameter(g_pAsqtRam->rev_buf, &g_asqt_parm.AsqtCfg);
            g_asqt_parm.asqt_volume = stub_asqt_get_volums(pbuf);
        }
        
        if(pc_status_new == sUserSetParam)
        {
            stub_asqt_down_param(g_pAsqtRam->rev_buf);
        }
        pc_tool_status = pc_status_new;
        get_sta_timer_ab = sys_time;
    }
    return pc_tool_status;
}




