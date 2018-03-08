/*! \cond MMM_PP_API*/
/********************************************************************************
 *                              GL5116
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona    2014-12-07 15:00     1.0             build this file
 ********************************************************************************/

/*! \endcond*/
#ifndef __MMM_PP_H__
#define __MMM_PP_H__

/*!
 * \brief
 *      ����ϵͳͷ�ļ�
 */

#include <psp_includes.h>

/*! \cond MMM_PP_API */

/*! �����Ч�������� */
#define MAX_EQ_SEG                  12

/*!
 *  \brief
 *  ���ֲ��Ŵ���ţ��ڵ��ýӿڷ��ش�������м�������棩״̬Ϊ����ʱ��Ч
 */
typedef enum
{
    /*! ������� */
    MMM_PP_ERR_DECODER_ERROR = 0x10001,
    /*! ��Ч���� */
    MMM_PP_ERR_CMD_ILLEGAL = 0x10002,
    /*! �����ֲ����� */
    MMM_PP_ERR_CMD_NOTEXIST = 0x10003,
    /*! decoder���ش��� */
    MMM_PP_ERR_DECODER_LOAD = 0x10004,
    /*! δ֪���� */
    MMM_PP_ERR_UNKNOWN = 0x100FF
} mmm_pp_err_e;

/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{

    /*! �������м�� */
    MMM_PP_OPEN = 0,
    /*! �ر������м�� */
    MMM_PP_CLOSE,
    /*! ��������� */
    MMM_PP_AINOUT_OPEN,
    /*! �ر�������� */
    MMM_PP_AINOUT_CLOSE,

    /*! ���õ���ʱ�� */
    MMM_PP_FADE_IN_TIME,
    /*! ���õ���ʱ�� */
    MMM_PP_FADE_OUT_TIME,

    /*! ���ò��Ų���(sample rate?) */
    MMM_PP_SET_PARAM,
    /*! ����dac sample rate */
    MMM_PP_SET_DAC_RATE,
    /*! ����adc sample rate */
    MMM_PP_SET_ADC_RATE,
    
    /*! ��ȡý����Ϣ */
    MMM_PP_MEDIA_INFO,
    /*! ��������˥������ */    
    MMM_PP_SET_NOISE_REDUCTION_PARA,
    /*! ���� */
    MMM_PP_PLAY,
    /*! ֹͣ */
    MMM_PP_STOP,
    /*! ��ʼ¼�� */
    MMM_PP_RECORD_START,
    /*! ��ʼ¼�� */
    MMM_PP_RECORD_STOP,
    /*! ��ȡ����״̬ */
    MMM_PP_GET_STATUS,
    /*! ��ȡ������Ϣ�� */
    MMM_PP_GET_ERRORNO,

    /*! ��ʼ������ */
    MMM_PP_START_SEND,
    /*! ֹͣ������ */
    MMM_PP_STOP_SEND,

    /*! �������� */
    MMM_PP_SET_VOLUME,
    /*! ������Ч */
    MMM_PP_SET_EQ,

    /*! ���ûص����� */
    MMM_PP_SET_CALLBACK,

    /*! ��ȡ������������Ϣ��ͨ����ȡ��ַ��Ȼ��Ϳ���ֱ�ӿ��ٷ��� */
    MMM_PP_GET_ENERGY_INFO,

    /*! ���������������������������������������ϵ� */
    MMM_PP_SET_OUTPUT_CHANNEL,

    /*! �����Ƿ��������ʹ̶� */
    MMM_PP_FIX_SAMPLE_RATE,
} mmm_pp_cmd_e;

/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_PP_ENGINE_STOPPED = 0,
    /*! ����״̬ */
    MMM_PP_ENGINE_PLAYING,
    /*! ��ͣ״̬ */
    MMM_PP_ENGINE_PAUSED,
    /*! ����״̬ */
    MMM_PP_ENGINE_ERROR
} mmm_pp_engine_e;

/*!
 *  \brief
 *     ���������������
 */
typedef enum
{
    /*! fm analog */
    MMM_PP_FM_IN = 0,
    /*! lienin analog */
    MMM_PP_LINEIN_IN,
    /*! usb composite */
    MMM_PP_USOUND_IN,
    /*! usb speaker */
    MMM_PP_USPEAKER_IN,
    /*max*/
    MMM_PP_MAX_IN
} mmm_pp_input_e;

/*!
 *  \brief
 *     �������������Ϣ
 */
typedef struct
{
    /*! ��mmm_pp_input_e*/
    int input_type;
    /*!asrc index*/
    int asrc_index;
    /*! sample rate */
    int dac_sample;
    /*!dac param dac chanel*/
    int dac_chanel;
    /*!asrc mode select */
    int asrc_mode_sel;
    /*!adc param adc source select */
    int adc_sample;
    /*!adc source select */
    int adc_insel;//mic or fm or linein    
    /*!adc param adc source select */
    int adc_gain;
    /*!adc source select */
    int ain_gain;//mic or fm or linein

} mmm_pp_ainout_setting_t;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_pp_engine_e */
    int status;
    /*! �Ƿ�ʼ¼�� */
    int record_status;
    /*! ����Ĵ���ţ����mmm_pp_err_e */
    int err_no;
    /*! ��ǰ�������� */
    int cur_energy;
    /*! Todo spectrum*/
} mmm_pp_status_t;
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
} mmm_pp_performance_t;
/*!
 *  \brief
 *     ��Ч������Ϣ
 */
typedef struct
{
    /*! eq�����ͣ����mmm_mp_eqmode_t*/
    int eq_type;
    /*! eq�ľ������ */
    char eq_para[MAX_EQ_SEG];
} mmm_pp_eq_info_t;

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
} mmm_pp_data_info_t;

/*!
 *  \brief
 *     ���ֲ�����Ϣ
 */
typedef struct
{

    /*! ����ֵ����������̶����������ṩ���õĽӿ�*/
    /*! ϵͳĬ�ϲ����������и������ã������ýӿڣ�ԭ����ֻ����һ��*/
    mmm_pp_ainout_setting_t ainout_setting;

    /*! ����״̬*/
    mmm_pp_status_t music_status;

    /*! ���� ����*/
    mmm_pp_data_info_t send_out_data;

    /*! �ɼ�����*/
    mmm_pp_data_info_t get_in_data;

    /*! ���뵭�� */
    /*! ����ʱ�䣬��λ���� */
    int fade_in_time;
    /*! ����ʱ�䣬��λ���� */
    int fade_out_time;

    /*! �û��ɵ��ڲ�������ǰ�趨��������һ�׸裬�����ýӿڣ��������ö��*/
    /*! �������� */
    int volume_level;

    /*!���ֲ�������*/
    mmm_pp_performance_t cur_perform;

    /*! ��Ч���ã��μ�mmm_pp_eq_info_t*/
    mmm_pp_eq_info_t eq_setting;

} music_pcm_player_info_t;

/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h
 */
extern int mmm_pp_cmd(void *, mmm_pp_cmd_e, unsigned int) __FAR__;

#endif // __MMM_PP_H__
