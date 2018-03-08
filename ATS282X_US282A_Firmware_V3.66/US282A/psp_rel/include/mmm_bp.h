/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona    2015-02-28 15:00     1.0             build this file
 ********************************************************************************/

/*! \endcond*/
#ifndef __MMM_BP_H__
#define __MMM_BP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ����ϵͳͷ�ļ�
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#endif

#include "mmm_dae_def.h"

//for bt data get pip
/*!
 *  \brief
 *  //�������ݹܵ�
 */
typedef struct
{
    uint16 read_ptr;
    uint16 write_ptr;
    uint16 capacity;
    uint16 length;
    uint16 sbc_input_length;
    uint8 *raw_data;
    uint8 *ext_info;
} bt_stack_pipe_t;

/*!
 *  \brief
 *  //TWS play status 
 
 */
typedef enum
{
    TWS_PLAY_IDLE = 0,
    TWS_PLAY_START,
    TWS_PLAY_SYNC,
    TWS_PLAY_MAX
}bt_tws_play_flag_e;


/*!
 *  \brief
 *  //TWS ��Ϣ����
 */
typedef struct
{
    //uint8  start_flag;
    bt_tws_play_flag_e start_flag;
     uint8  tts_start_flag;
     uint8  tts_stop_flag;
     uint8 mmm_init_flag;
	 
    uint8 pri_pause_flag;
     uint8  pri_exit_mmm;
     uint8 sub_exit_mmm;
     uint8  sub_full_exit_flag;
     uint8  sub_back_bl_flag;
     uint8 sub_quit_bl_flag;
     uint8 pri_full_exit_flag;
     uint8 pri_empty_exit_mmm;
     uint8 tws_sync_cmd;
     uint8 asrc_change_flag;  //src �������ڱ�־
     uint8 tws_play_sync_flag; //tws ���䲥��ͬ��
     //for linein start time use 
     uint8 tws_start_cnt;//linein start cnt 
     uint16 tws_sub_pipe_length;
     uint16 dsp_dec_err_count;
     uint16 dsp_dec_err_count1;
    uint32 buffer_time_length;
    uint32 sample_count; //���䲥�ŵ�sample count
    uint32 sample_count1; //���䲥�ŵ�sample count
    uint32 src_value;    //����src���ں��ֵ
     uint16 tws_send_buf_len;
    uint32 tws_send_buf_base;
} bt_stack_twsinfo_t;

/*!
 *  \brief
 *  ���ֲ��Ŵ���ţ��ڵ��ýӿڷ��ش�������м�������棩״̬Ϊ����ʱ��Ч
 */
typedef enum
{
    /*! ������� */
    MMM_BP_ERR_DECODER_ERROR = 0x10001,
    /*! ��Ч���� */
    MMM_BP_ERR_CMD_ILLEGAL = 0x10002,
    /*! �����ֲ����� */
    MMM_BP_ERR_CMD_NOTEXIST = 0x10003,
    /*! decoder���ش��� */
    MMM_BP_ERR_DECODER_LOAD = 0x10004,
    /*! δ֪���� */
    MMM_BP_ERR_UNKNOWN = 0x100FF
} mmm_bp_err_e;

/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{

    /*! �������м�� */
    MMM_BP_OPEN = 0,
    /*! �ر������м�� */
    MMM_BP_CLOSE,
    /*! ��������� */
    MMM_BP_AOUT_OPEN,
    /*! �ر�������� */
    MMM_BP_AOUT_CLOSE,

    /*! ���õ���ʱ�� */
    MMM_BP_FADE_IN_TIME,
    /*! ���õ���ʱ�� */
    MMM_BP_FADE_OUT_TIME,    
    /*! ��������˥������ */    
    MMM_BP_SET_NOISE_REDUCTION_PARA,
    /*! ���� */
    MMM_BP_PLAY,
    /*! ֹͣ */
    MMM_BP_STOP,

    /*! ��ȡ����״̬ */
    MMM_BP_GET_STATUS,
    /*! ��ȡ������Ϣ�� */
    MMM_BP_GET_ERRORNO,

    /*! ������Ч */
    MMM_BP_SET_EQ,

    /*! ���ò����ļ���ʽ */
    MMM_BP_SET_MUSIC_TYPE,

    /*! ���ûص����� */
    MMM_BP_SET_CALLBACK,

    /*! ��ȡ������������Ϣ��ͨ����ȡ��ַ��Ȼ��Ϳ���ֱ�ӿ��ٷ��� */
    MMM_BP_GET_ENERGY_INFO,

    /*! ���������������������������������������ϵ� */
    MMM_BP_SET_OUTPUT_CHANNEL,
    /*! TWS SYNC START */
    MMM_BP_TWS_SYNC_START,
    /*! tws sample count*/
    MMM_BP_TWS_GET_SAMPLE_COUNT,
    /*! fix sample rate*/
    MMM_BP_FIX_SAMPLE_RATE,
    /*! ��������:ֻ�Ƕ�SBC������źŽ��д���*/
    MMM_BP_RESTRAIN_NOISE, 
} mmm_bp_cmd_e;

/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬��DSPû�м��� */
    MMM_BP_ENGINE_STOPPED = 0,
    /*! ����״̬��DSP�Ѽ��أ������ʺ�Ƶ������ */
    MMM_BP_ENGINE_PLAYING,
    /*! ��ͣ״̬��DSP�Ѽ��أ������Ժܵ�Ƶ������ */
    MMM_BP_ENGINE_PAUSED,
    /*! ����״̬ */
    MMM_BP_ENGINE_ERROR
} mmm_bp_engine_e;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_bp_engine_e */
    int status;
    /*! ����Ĵ���ţ����mmm_bp_err_e */
    int err_no;
    /*! ��ǰ�������� */
    int cur_energy;
    /*! Todo spectrum*/
} mmm_bp_status_t;
/*!
 *  \brief
 *     ���ֲ�������
 */
typedef struct
{
    /*! parser�ĵ�ʱ�� */
    int parser_elapsed;
    /*! decoder�ĵ�ʱ�� */
    int dec_elapsed;
    /*! ��ȡ�ļ��ĵ�ʱ�� */
    int io_elapsed;
    /*! ������ģ���format_check */
    int other_elapsed;
} mmm_bp_performance_t;

/*!
 *  \brief
 *     ���ݹ���
 */
typedef struct
{
    /*! ����buffer*/
    uint8* data_buf;
    /*! ���ݴ�ȡָ��λ��*/
    uint32 ptr_pos;
    /*! �����ֽ��� */
    uint32 data_count;
} mmm_bp_data_info_t;

/*!
 *  \brief
 *     �������Ͳ���
 */

typedef enum
{
    /*! sbc  */
    TYPE_SBC = 0,
    /*! aac */
    TYPE_AAC,
    /*! mp3 */
    TYPE_MP3,
    /*! aac */
    TYPE_MAX

} mmm_bp_music_type_e;

/*!���������Ϣ*/
typedef struct
{
    /*!���ŵ��ļ����� �� mmm_bp_music_type_e*/
    mmm_bp_music_type_e music_type;
    /*!������ʼ��ʱ*/
    uint8 play_delay_start;
    /*!���������ʱ*/
    uint8 play_delay_lowest;
    /*!��ͨģʽ����twsģʽ*/
    uint8 work_mode;
} mmm_bp_set_type_param_t;

/*!
 *  \brief
 *     ���������Ϣ
 */
typedef struct
{
    /*! sample rate */
    uint32 dac_sample;
    /*! dac param dac chanel */
    uint32 dac_chanel;
    /*! ÿ֡�������������Բ����ʼ�����֡ʱ�� */
    uint32 sample_per_frame;
    /*!���������Ϣ*/
    mmm_bp_set_type_param_t type_param;
} mmm_bp_music_setting_t;

/*!
 *  \brief
 *     ���ֲ�����Ϣ
 */
typedef struct
{

    /*! ����������Ϣ*/
    mmm_bp_music_setting_t music_setting;

    /*! ����������Ϣ*/
    mmm_bp_data_info_t raw_data;

    /*! ����״̬*/
    mmm_bp_status_t music_status;

    /*! ���뵭�� */
    /*! ����ʱ�䣬��λ���� */
    int fade_in_time;
    /*! ����ʱ�䣬��λ���� */
    int fade_out_time;

    /*!���ֲ�������*/
    mmm_bp_performance_t cur_perform;

} music_bt_player_info_t;

typedef struct
{
    /*!asrc index*/
    int asrc_index;
    /*!dac param dac chanel*/
    int dac_chanel;
    /*!asrc mode select */
    int asrc_mode_sel;

} mmm_bp_aout_setting_t;


/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h
 */
extern int mmm_bp_cmd(void *, mmm_bp_cmd_e, unsigned int) __FAR__;

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_BP_H__
