/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������������ģ�飬֧��FIFO��ʽ�ӳٲ��ţ�ͨ������һ�������İ������������߳�
 *       ���в��ţ���ģ�黹�������ð�����ASRCͨ��������DAC�����ʣ��Լ����������ء�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_keytone.h"

typedef struct
{
    sd_handle kt_handle;
    uint16 kt_offset;
    uint16 kt_length;
    uint16 kt_frame_size;
    uint8  kt_fill_flag;
    uint32 kt_compact;
} kt_play_info_t;

#define KEYTONE_FRAME_SIZE  512

uint8 g_bitstream_buffer[KEYTONE_FRAME_SIZE] _BANK_DATA_ATTR_;

keytone_infor_t __section__(".gl_kt_data") g_keytone_infor;

uint8 keytone_fifo_out(void);
bool keytone_fifo_in(uint8 kt_id);

/******************************************************************************/
/*!
 * \par  Description:
 * \void keytone_init_set(void)
 * \��������ʼ������
 * \param[in]    none
 * \param[out]   none
 * \return       int the result
 * \retval           0 sucess
 * \retval           -1 failed
 * \ingroup
 * \note
 */
/*******************************************************************************/
void keytone_init_set(void)
{
    //CHAN 0 -- URAM0 = 2KB
    //CHAN 1 -- PCMRAM2 = 4KB
    enable_dac(DAFIS_ASRC, g_keytone_infor.dac_chan);
    config_asrc((g_keytone_infor.dac_chan == DAF0_EN)? K_OUT0_U0 : K_OUT1_P2, g_keytone_infor.dac_chan);
    //set_dac_rate(g_keytone_infor.dac_sample, g_keytone_infor.dac_chan);
    set_asrc_rate(g_keytone_infor.dac_sample, g_keytone_infor.dac_chan, 5000);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void keytone_send_to_asrc(uint32 data_count)
 * \read decoder frame data
 * \param[in]    data_count send data number
 * \param[in]
 * \param[out]   none
 * \return
 * \ingroup
 * \note
 */
/*******************************************************************************/
void keytone_asrc_fill(int32 fill_data)
{
    fill_data = fill_data / 100 * (int32) sys_comval->mdrc_percent;
    fill_data = fill_data / 100 * (int32) sys_comval->kt_tts_percent;

    if (g_keytone_infor.dac_chan == DAF0_EN)
    {
        (*(volatile int32 *) ASRC_OUT0_WFIFO) = fill_data;
        (*(volatile int32 *) ASRC_OUT0_WFIFO) = fill_data;
    }
    else
    {
        (*(volatile int32 *) ASRC_OUT1_WFIFO) = fill_data;
        (*(volatile int32 *) ASRC_OUT1_WFIFO) = fill_data;
    }
}
void keytone_send_to_asrc(kt_play_info_t *p_kt_play_info)
{
    uint32 data_count = p_kt_play_info->kt_frame_size;
    //temp use
    long long shift_use;
    //position to buffer
    uint32 buf_ptr = 0;
    int16 chanel_data;
    uint32 check_ret;

    while (data_count > 0)
    {
        if (g_keytone_infor.dac_chan == DAF0_EN)
        {
            check_ret = act_readl(ASRC_OUT0_NUM);
        }
        else
        {
            check_ret = act_readl(ASRC_OUT1_NUM);
        }

        if (check_ret != 0)
        {
            chanel_data = *(int16 *) &g_bitstream_buffer[buf_ptr];
            shift_use = (long long) chanel_data * p_kt_play_info->kt_compact;
#pragma __PRQA_IGNORE_START__
            shift_use >>= 10;
#pragma __PRQA_IGNORE_END__
            shift_use <<= 16;
            if (shift_use > 0x7fffffff)
            {
                shift_use = 0x7fffffff;
            }
            if (shift_use < -0x7fffffff)
            {
                shift_use = -0x7fffffff;
            }

            //out first sample data
            keytone_asrc_fill((int32) shift_use);

            //position move
            buf_ptr += 2;
            //data count sub
            data_count -= 2;
        }
        else
        {
            p_kt_play_info->kt_fill_flag = TRUE;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int32 keytone_play_frame(void* null0, void* null1, void* null2)
 * \����������
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup
 * \note
 */
/*******************************************************************************/
int32 keytone_play_frame(kt_play_info_t *p_kt_play_info)
{
    //������Ƶ֡
    int32 play_ret;

    if (p_kt_play_info->kt_offset >= p_kt_play_info->kt_length)
    {
        return -1;
    }

    sys_os_sched_lock();

    //������Ƶ֡
    if (sys_sd_fseek(p_kt_play_info->kt_handle, 0, p_kt_play_info->kt_offset) == -1)
    {
        play_ret = -1;
        goto file_seek_fail;
    }
    //read frame data
    play_ret = sys_sd_fread(p_kt_play_info->kt_handle, g_bitstream_buffer, p_kt_play_info->kt_frame_size);

    if (play_ret != -1)
    {
        if (p_kt_play_info->kt_offset == 0)
        {//ǰ4�ֽ�Ϊ�ļ����ȣ���Ϊ0
            g_bitstream_buffer[0] = 0;
            g_bitstream_buffer[1] = 0;
            g_bitstream_buffer[2] = 0;
            g_bitstream_buffer[3] = 0;
        }

        //change data offset
        p_kt_play_info->kt_offset += (uint16) play_ret;

        //file size
        if ((uint16) play_ret < p_kt_play_info->kt_frame_size)
        {
            p_kt_play_info->kt_frame_size = (uint16) play_ret;
        }

        //send data to asrc
        keytone_send_to_asrc(p_kt_play_info);
    }

    file_seek_fail:
    sys_os_sched_unlock();

    return play_ret;
}

void keytone_play_loop(void *param)
{
    kt_play_info_t kt_play_info;
    int32 play_ret;
    uint8 kt_id = (uint8) (uint32) (param);

    //PRINT_DBG_INT("keytone enter:", kt_id);

    g_keytone_infor.playing = TRUE;

    kt_play_info.kt_compact = 0x0fec; //�̶�ϵ��
    kt_play_info.kt_fill_flag = FALSE;
    sys_os_sched_lock();
    kt_play_info.kt_handle = sys_sd_fopen(g_keytone_filelist[kt_id]);
    sys_os_sched_unlock();
    if (kt_play_info.kt_handle == NULL)
    {
        //open fail
        goto fail_open;
    }

    //set hardware
    keytone_init_set();

    kt_play_info.kt_frame_size = KEYTONE_FRAME_SIZE;
    sys_sd_fseek(kt_play_info.kt_handle, 0, 0);
    sys_sd_fread(kt_play_info.kt_handle, &(kt_play_info.kt_length), 2);
    kt_play_info.kt_offset = 0;

    while (1)
    {
        play_ret = keytone_play_frame(&kt_play_info);
        if (play_ret == -1)
        {
            break;
        }

        //ASRC OUT���������Ҫ��ʱ
        if (kt_play_info.kt_fill_flag == TRUE)
        {
            kt_play_info.kt_fill_flag = FALSE;
            sys_os_time_dly(2);
        }
    }

    //�ȴ����������Ž���
    while (1)
    {
        if (g_keytone_infor.dac_chan == DAF0_EN)
        {
            if ((act_readl(ASRC_OUT0_IP) & (1 << ASRC_OUT0_IP_PCMFIFOEmpty)) != 0)
            {
                break;
            }
        }
        else
        {
            if ((act_readl(ASRC_OUT1_IP) & (1 << ASRC_OUT1_IP_PCMFIFOEmpty)) != 0)
            {
                break;
            }
        }

        sys_os_time_dly(1);
    }

    if (g_keytone_infor.dac_chan == DAF0_EN)
    {
        close_asrc(K_OUT0_U0);
    }
    else
    {
        close_asrc(K_OUT1_P2);
    }

    //disable dac channel zero
    disable_dac(g_keytone_infor.dac_chan);
    //close source file
    sys_sd_fclose(kt_play_info.kt_handle);

    fail_open:

    g_keytone_infor.playing = FALSE;

    libc_pthread_exit();
}

void keytone_play_thread(uint8 kt_id)
{
    //�򿪰������߳�
    pthread_ext_param_t param;

    //��ʼ�������̵߳Ĳ���
    param.pthread_param.start_rtn = (void *) keytone_play_loop;
    param.pthread_param.arg = (void *) (uint32) kt_id;
    //param.pthread_param.ptos = (void *) KEYTONE_PLAY_STK_POS;/*ʵ��ʹ��0x190*/
    g_keytone_infor.keytone_thread_addr = sys_malloc_large_data(KEYTONE_PLAY_STK_SIZE);
    param.pthread_param.ptos = (void *)(g_keytone_infor.keytone_thread_addr + KEYTONE_PLAY_STK_SIZE);

    //libc_print("keytone addr", g_keytone_infor.keytone_thread_addr, 2);
    
    param.stk_size = KEYTONE_PLAY_STK_SIZE;

    g_keytone_infor.thread_eixt = TRUE;

    if (libc_pthread_create(&param, KEYTONE_PLAY_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
    }

    return;
}

void keytone_thread_task_stack_free(void)
{
    if(g_keytone_infor.keytone_thread_addr != 0)
    {
        //libc_print("free keytone", g_keytone_infor.keytone_thread_addr, 2);
        sys_free_large_data(g_keytone_infor.keytone_thread_addr);
        g_keytone_infor.keytone_thread_addr = 0;
        g_keytone_infor.thread_eixt = FALSE;
    }
}

void keytone_play_deal_wait(void)
{
    while (g_keytone_infor.playing == TRUE)
    {
        sys_os_time_dly(1);
    }

    keytone_thread_task_stack_free();
}


void keytone_set_dac_chan(dac_chenel_e dac_chan)
{
    if (dac_chan == g_keytone_infor.dac_chan)
    {
        return;
    }

    //�ȴ�����������������л�dacͨ��
    keytone_play_deal_wait();

    g_keytone_infor.dac_chan = dac_chan;
}

//ֻ�����ڳ�ʼ��ʱ����
void keytone_set_dac_sample(uint8 dac_sample)
{
    g_keytone_infor.dac_sample = dac_sample;
}

//������������2����ȫ�ֿ��غͳ������أ�������ǳ�������
void keytone_set_on_off(bool on_off)
{
    if (on_off == g_keytone_infor.enable)
    {
        return;
    }

    //�ȴ�����������������л�dacͨ��
    keytone_play_deal_wait();

    g_keytone_infor.enable = on_off;
}

void keytone_play_deal(void)
{
    uint8 kt_id;

    if (g_keytone_infor.playing == TRUE)
    {
        return;
    }

    keytone_thread_task_stack_free();

    kt_id = keytone_fifo_out();
    if (kt_id == 0xff)
    {
        //PRINT_WARNING("kt fifo out err!!");
        return;
    }

    if (g_keytone_infor.enable == TRUE)
    {
        keytone_play_thread(kt_id);
    }
}



bool keytone_play(uint8 kt_id, uint8 mode)
{
    soundout_state_e state = get_soundout_state();

    if (g_app_info_state.inner_pa_inited == FALSE)
    {
        return FALSE;
    }

    if (g_keytone_infor.enable == FALSE)
    { //�ѽ��ð�����
        return FALSE;
    }

    if (g_app_info_state.stub_tools_type != 0)
    {
        return FALSE;
    }

    if (g_app_info_state.autotest_mode != 0)
    {
        return FALSE;
    }

    if(g_keytone_infor.kt_mute == 1)
    {
        return FALSE;
    }

#if ((CASE_BOARD_TYPE == CASE_BOARD_ATS2823) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823))
    if (get_card_state() != CARD_STATE_CARD_NULL)
    {
        return FALSE;
    }
#endif

    if ((sys_comval->key_tone_overlay == FALSE) && ((state != SOUND_OUT_STOP) && (state != SOUND_OUT_PAUSE)))
    { //�������������ӵ�������
        return FALSE;
    }

    if (get_mute_enable() == TRUE)
    { //����״̬�£�ֱ�ӷ���
        return FALSE;
    }

    if ((sys_comval->volume_current == 0) && (sys_comval->kt_tts_min_volume == 0))
    { //��������Ϊ0ʱû�а�������TTS
        return FALSE;
    }

    if (mode == KEYTONE_NOBLOCK)
    {
        keytone_fifo_in(kt_id);

        keytone_play_deal();
    }
    else
    {
        keytone_play_deal_wait();

        keytone_fifo_in(kt_id);

        while (1)
        {
            kt_id = keytone_fifo_out();
            if (kt_id == 0xff)
            {
                break;
            }

            if (g_keytone_infor.enable == TRUE)
            {
                keytone_play_thread(kt_id);
                keytone_play_deal_wait();
            }
        }
    }

    return TRUE;
}

void com_start_key_tone(uint8 mode)
{
    //������ʹ�ܣ��ŷ���������
    if (sys_comval->key_tone == TRUE)
    {
        keytone_play(KEYTONE_KEYTONE, mode);
    }
}

uint8 keytone_fifo_out(void)
{
    uint8 kt_id, i;
    
    if ((g_keytone_infor.count == 0) || (g_keytone_infor.count > KEYTONE_FIFO_DEPTH))
    {
        return -1;
    }

    kt_id = g_keytone_infor.kt_fifo[0];
    g_keytone_infor.count--;
    
    for (i = 0; i < g_keytone_infor.count; i++)
    {
        g_keytone_infor.kt_fifo[i] = g_keytone_infor.kt_fifo[i + 1];
    }

    return kt_id;
}

bool keytone_fifo_in(uint8 kt_id)
{
    if (g_keytone_infor.count >= KEYTONE_FIFO_DEPTH)
    {
        PRINT_WARNING("kt full!");
        return FALSE;
    }

    g_keytone_infor.kt_fifo[g_keytone_infor.count] = kt_id;
    g_keytone_infor.count++;

    return TRUE;
}

/*! \endcond */
