/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ASET���߹��ⲿ���õ�ͷ�ļ�
 * ���ߣ�cailizhen
 ********************************************************************************/
#ifndef __COMMON_ASET_INTERFACE_H
#define __COMMON_ASET_INTERFACE_H

typedef enum
{
    /*ASET������PC������*/
    ASET_TOOLS_DISCONNECT = 0,
	/*ASET����δ��������PC��С������������*/
    ASET_TOOLS_NOT_WORK, 
    /*ASET������������ģʽ*/
    ASET_TOOLS_WORK   
} aset_tools_run_state_e;

//��¼ASET���ߵ�����״̬
typedef struct
{
    uint8 err_cnt:6;  //7bit
    uint8 run_state:2;    
} aset_tools_run_state_t;

extern void aset_global_para_init(void) __FAR__;
extern bool aset_test_init(void) __FAR__;

extern void aset_test_loop_deal(void) __FAR__;

extern void aset_test_exit(void) __FAR__;

extern void aset_test_set_dae_init(void) __FAR__;


extern aset_tools_run_state_t g_aset_run_state;
extern bool g_aset_sync_volume;
extern bool g_aset_neednot_take_effect;
extern uint16 g_aset_main_gain_value;
extern uint16 g_aset_equivalent_gain;
extern bool g_aset_main_switch_flag;


#endif
