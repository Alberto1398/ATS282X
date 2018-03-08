

/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*        brief  btcallͷ�ļ�
*      Cailizhen          2015-1          creat
*       Wekan        2015-3-30       review
*******************************************************************************/


#ifndef _BLUETOOTH_ENGINE_H
#define _BLUETOOTH_ENGINE_H

#include "psp_includes.h"
#include "case_include.h"
#include "mmm_hfp.h"
#include "btcall_common.h"
#include "mmm_agc_aec_def.h"
#include "btcall_eg_stub_asqt.h"



#define CALLRING_DAC_BUFFER_LENGTH           (0x400*2)
extern uint8 g_btcall_callring_dac_buf[CALLRING_DAC_BUFFER_LENGTH];

#define CALLRING_READ_BUFFER_LENGTH          (0x200*2)
extern uint8 g_btcall_callring_read_buf[CALLRING_READ_BUFFER_LENGTH];

#define  ASQT_TEST_BUFFER_LENGTH (1024*10-4)
extern uint8    g_btcall_asqt_test_buf[ASQT_TEST_BUFFER_LENGTH];

#define DMA_NOT_TRANS 0
#define DMA_START_TRANS 1
#define DMA_HALF_TRANSED 2
#define DMA_TRANS_COMPLECT 3

typedef struct
{
    bool callring_thread_exist;
    sd_handle callring_handle;
    uint16 callring_file_length;   //�ļ�����
    uint16 callring_read_ptr;
    uint16 callring_silent_time;   //������������ʱ�䣬100msΪ��λ
    bool callring_silent_flag;
    int8 callring_timer_id;
} callring_data_struct;

extern callring_data_struct g_callring_data;

typedef struct
{
    uint8 g_sco_deal_flag;
    uint8 dac_clock_status;
    bool need_exit_thread;
    uint8 last_dma0_state;
    uint8 tts_play_flag;
    uint8 tts_pb_play_flag;//��־������ϢTTS����״̬��ע��:�����绰��ʱtts_play_flagҲ����1
    uint8 callin_ring_mode;//��������ģʽ��0��ʾ��������1��ʾ���绰��������2��ʾ����+�绰������
    uint8 callin_ring_space_time;//�����������ʱ������1SΪ��λ
    bool callin_ring_first;
    bool callin_pb_back;

    //��Ҫ����AEC����
    bool g_actaec_reset_flag;
    //���ε�MIC����
    bool g_mic_mute_flag;
    uint8 stub_asqt_en;//�Ƿ��asqt �Ҵ�stub

    uint8 ignore_uncall_sco; //�Ƿ���Ҫ���˷�ͨ��״̬�µ�sco���ݣ����˴����ʼǱ�������ʱ��SCO���������
                             //ע��:������˿϶��ᵼ��һЩ����
    uint8 dsp_load_flag;  	 //dsp �Ƿ��Ѿ����� 1:��ʾ�Ѿ�����; 0��ʾû�м���                       
} btcall_global_variable_struct;

typedef struct{//���2k
    uint16 magic;//0xAA55ʱ��Ч
    uint16 times;//���´���
    uint32 cs;//У����
    sv_prms_t dsp_cfg;
}VM_SAVE_t;

extern int ring_dma_isr(void);

extern app_result_e btcall_deal_app_msg(void);
extern app_result_e get_message_loop(void);

extern void dma_dac_start_transfer(void);
extern void callring_dac_dma_config(void);

extern void bt_engine_hfp_sco_connect_ind_handle(void);
extern void bt_engine_hfp_sco_disconnect_ind_handle(void);

extern void callring_start(void);
extern void *callring_thread_handle(void *param);
extern void btcalleg_free_hfp_mmm(void);
extern app_result_e app_message_deal(private_msg_t *pri_msg);


extern btcall_global_variable_struct btcall_gl_var;

extern void *mp_handle;

extern bt_stack_info_t *g_p_bt_stack_cur_info;

extern btcall_info_t g_btcall_info[2];
extern btcall_info_t *g_p_btcall_info;
extern uint8 g_stub_tools_type;

#endif/* _BLUETOOTH_ENGINE_H */

