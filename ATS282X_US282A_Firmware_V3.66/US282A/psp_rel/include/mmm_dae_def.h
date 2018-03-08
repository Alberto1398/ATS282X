/*! \cond MMM_MP_API*/
/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao    2010-08-27 15:00     1.0             build this file
 ********************************************************************************/

/*! \endcond*/
#ifndef __MMM_DAE_DEF__
#define __MMM_DAE_DEF__

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

/*! \cond MMM_MP_API */

#define MAX_PEQ_SEG    EQ_NUM_BANDS //PEQ��Чband��
#define MAX_GEQ_START  7            //GEQ��ʼ��
#define MAX_GEQ_SEG    7            //GEQ��Чband��
#define MAX_DEW_SEG    6  //6����Ч��ǿ�����
#define DEW_BAND_HIGH_PASS  0 //��ͨ�˲�
#define DEW_BAND_LOW_FREQ   1 //����Ч��
#define DEW_BAND_HIGH_FREQ  2 //����Ч��

//for default value
#define DEAULT_FADEIN_TIME  100

#define DEAULT_FADEOUT_TIME 50

#define MAX_MDRC_SEG   MDRC_NUM_BANDS  //3��DRC

#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
#define MFBE_BANDS     10
#define MAX_FREQ_POINT 10
#endif

typedef enum
{
   OUTPUT_CHANNEL_NORMAL = 0, 					//�������м�����ʼ��Ϊ����
   OUTPUT_CHANNEL_L_R_SWITCH = 1, 				//������������ L = R0, R = L0
   OUTPUT_CHANNEL_L_R_MERGE = 2,				//���������ϲ���������������������� L = R = (L0 + R0) / 2
   OUTPUT_CHANNEL_L_R_ALL_L = 3,				//������������������� L = R = L0
   OUTPUT_CHANNEL_L_R_ALL_R = 4, 				//������������������� L = R = R0
}output_channel_e;

typedef struct
{
    /*����Ƶ�㣨���߸�ͨ�˲����Ľ�ֹƵ�ʣ�����λHz*/
    uint16 cutoff;
    /*Qֵ����λ0.1���Ƽ���Χ��3��30*/
    uint16 q;
    /*���棬��λ0.1db���Ƽ���Χ��-240��+120*/
    int16 gain;
    /*�˲������ͣ�0����Ч��1�ǳ���EQ�˲�����2�Ǹ�ͨ�˲���������ֵΪreserve*/
    uint16 type;
} peq_band_t;

typedef struct
{
    short threshold;
    short attack_time;
    short release_time;
    short reserve;//ȷ��4�ֽڶ���
} mdrc_band_t;

typedef struct
{
    /*! Ƶ�ʣ����ڸ�ͨ�˲�����ʾƫ��ֵ�����ڵ�Ƶ�͸�Ƶ����ʾ��ֹƵ�ʣ����bit��ʾ��������͸�����ǿ������PEQ�㣬��ʾ����Ƶ�� */
    uint16 cutoff;
    /*! 0��ʾ��Ч */
    int16 gain;
} dae_enh_weaken_band_t;

#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
//�ýṹ��DSP���м����case���ã�caseά��һ���������ã��м��ά��һ���û����ø�DSP��DSP�ڻ�ȡDAE����ʱ����һ��
typedef struct
{
    uint8 num_band;                         //Ƶ����,�㷨�ڲ����֧��10��
    uint8 num_freq_point;                   //Ƶ����,�㷨�ڲ����֧��12��
    short duration_ms;                      //ͳ��ʱ������λms
    short f_c[MFBE_BANDS];					//��ͨ����Ƶ��
    short freq_point[MAX_FREQ_POINT];       //��Ƶ������ 
} dae_attributes_t;
#endif


/*********Ϊ���ݱ�׼��Чģ�����ӵ�DAE�ṹ��********/
#define MAX_MDRC_SEG_STANDARD   3

typedef struct
{
    short threshold1;      //��ֵ1 
    short ratio1;          //ѹ����1 
	short threshold2;      //��ֵ2
    short ratio2;          //ѹ����2
	short tav;             //RMS���ƽ��ʱ��(ms)
	short attack_time;     //����ʱ�� 
	short release_time;    //�ͷ�ʱ��	
}dae_compressor_standard_mode_t;

typedef struct
{
    short threshold1;
    short compress_ratio1;
	short threshold2;
    short compress_ratio2;
	short tav_time;
    short attack_time;
    short release_time;
    short reserve;//ȷ��4�ֽڶ���
}dae_mdrc_band_standard_mode_t;

typedef struct
{
	int16 signal_adjust;
	short mdrc_crossover_freq0; //MDRC���з�Ƶ��
	short mdrc_crossover_freq1; //MDRC�и߷�Ƶ��
	int16 makeup_gain; 
}mdrc_extend_para_standard_mode_t;


typedef struct
{
    short MDRC_compensation_threshold;                  //default: -100,step 0.1dB,setting: -10*10 = -100
    short MDRC_compensation_peak_detect_attack_time;    //default: 0.5ms, step: 0.01ms, setting: 5*10
    short MDRC_compensation_peak_detect_release_time;   //default: 500ms, step: 1ms, setting: 500
    short MDRC_compensation_filter_Q;                   //default: 1,step 0.1, setting:0.1*10 = 1, 0.1~0.9
}mdrc_peak_detect_standard_mode_t;

typedef struct
{
  short threshold1;
  short threshold2;
}mdrc_threshold_base_max_t;

typedef struct
{
  short threshold1;
  short threshold2;
}compressor_threshold_base_t;

typedef enum
{
    //����ģʽ    
    SMART_MODE = 0,
    //��׼ģʽ
    STANDARD_MODE = 1,
} audiopp_type_e;

typedef struct
{
    int16 mdrc_threshold_base_max_trans_standard_mode;
    mdrc_threshold_base_max_t mdrc_threshold_base_trans_standard_mode[MDRC_NUM_BANDS_STANDARD_MODE];
    short MDRC_compensation_threshold_base_trans_standard_mode;

    compressor_threshold_base_t compressor_threshold_base;
    compressor_threshold_base_t compressor_threshold_base_trans;
}dae_mdrc_para_trans_sm_t;

typedef struct
{
    short precut_standard_mode;
    short compressor_th1;
    short compressor_th2;
    
    short signal_fine_tune;                 //�ź�΢��0.1db
    short vol_fine_tune;                    //����΢��0.1db    
    short gain_compensation;                //���油��0.1db
    short compensation_filter_threshold;    //�����˲�����ֵ0.1db

    short mdrc_low_freq_th1;                
    short mdrc_low_freq_th2;
    short mdrc_middle_freq_th1;
    short mdrc_middle_freq_th2;
    short mdrc_high_freq_th1;
    short mdrc_high_freq_th2;   
}vol_dae_para_sm_t;

/**************************************************/

typedef struct
{
    bool enable; //ʹ��DAE���ܣ������ֹ�ͼ��ؿյ�DAE�⣬���ͷ�DAE��ռ�õĿռ�
    bool bypass; //bypass�򿪱�ʾ�����κκ����������ú��Ե���һ��ֻ���ڵ���
    uint16 precut_ratio; //Ԥ˥�����ȣ�0.1dbΪ��λ��ȡֵ��Χ 0 ~ 120��ͨ��������������
    uint16 equivalent_gain; //DAE��Ч���棬0.1dbΪ��λ��ȡֵ��Χ 0 ~ 120
    uint8 mdrc_precut_ratio; //MDRC�����ֵ��Ӧ��precut��0.1dbΪ��λ��ȡֵ��Χ 0 ~ 120��ͨ��������΢�������ⲿ�������油��
    uint16 makeup_gain; //makeup gain�����ź�������0DB���Խ�����������������ȣ�0.1dbΪ��λ��ȡֵ��Χ 0 ~ 120
    int8 post_precut_ratio; //���ź�˥�����������ź�΢����һ��Ϊ��ֵ��0.1dbΪ��λ
    bool peq_enable; //PEQ
    bool spk_compensation_enable; //���Ȳ���
    bool vbass_enable; //�������
    bool vsurround_enable; //������Ч
    bool treble_enhance_enable; //������ǿ
    bool mdrc_enable; //MDRC�㷨
    bool limiter_enable; //�޷��㷨
    bool energy_detect_enable; //�ź��������
    bool noise_reduction_enable;
    bool enable_2823T;
    bool dae_print_enable;
    
    uint8 run_freq; //����Ƶ��

    peq_band_t peq_bands[MAX_PEQ_SEG]; //PEQ �˲�������
    uint8 peq_band_enable_type[MAX_PEQ_SEG]; //peq���ͺ�enableѡ�1��ʾSpeaker PEQ��2��ʾPost PEQ��0��ʾ��ִ��

    uint16 treble_enhance_cut_freq;//��Ƶ��ֹƵ��
    int16 treble_enhance_ratio;    //���� ��λΪ0.1dB

    uint8 spk_comp_source; //�˲���������Դ��0��ʾSD���ļ���1��ʾVRAM
    short spk_comp_filter_order; //�˲�������
    uint8 spk_comp_filter_QvalLeft; //�������˲���Qֵ
    uint8 spk_comp_filter_QvalRight; //�������˲���Qֵ

    uint16 vbass_cut_freq; //��Ƶ��ֹƵ��
    int16 vbass_ratio; //������������� ��λΪ0.1dB 0.0dB -24dB ������Ҫ��ȥ12.0dB�ٴ��ݸ�DSP
    int8 original_bass_ratio; //ԭʼ����������
    
    uint8 vsurround_angle; //���ƽǶ�
    int8 vsurround_ratio; //����

    uint16 mdrc_crossover_freq[MAX_MDRC_SEG-1]; //mdrc��Ƶ��
    mdrc_band_t mdrc_bands[MAX_MDRC_SEG]; //MDRC����

    int16 limiter_threshold; //�޷���ֵ
    uint16 limiter_attack_time; //����ʱ��
    uint16 limiter_release_time; //�ͷ�ʱ��

    dae_enh_weaken_band_t dew_bands[MAX_DEW_SEG]; //��Ч��ǿ�����

    uint8 period_count; //��Ч������
    uint8 preadjust_count; //��ʼԤ�������źŴ�С����������Ȼ��ÿ�����ڽ�����Ӧ����
    short period; //ͳ������ʱ������1msΪ��λ
    short preadjust_db; //�źŴ�С���ͽ϶������Ԥ�����ƣ���0.1dbΪ��λ
    short signal_energy_init; //��ʼ�ź�����ֵ

    int16 mdrc_threshold_base[MAX_MDRC_SEG]; //mdrc ��̬��ֵ��ֵ
    int16 mdrc_threshold_base_max; //mdrc ��ֵ��ֵ���ֵ
    int16 limiter_threshold_diff; //mdrc ģʽ��Limiter��ֵ��mdrc�����ֵ�Ĳ�ֵ

    uint8 MultiFreqBandEnergy_enable;       //��Ƶ���������㿪��
    uint8 FreqSpetrumDisplay_enable;        //��Ƶ���������㿪��
    
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
    dae_attributes_t* dae_attributes;
#endif

   /*********Ϊ���ݱ�׼��Чģ�����ӵ�DAE����**********/
    bool compressor_enable_standard_mode;        //ѹ�����㷨
    bool mdrc_enable_standard_mode;              //mdrc�㷨
    bool audiopp_type_update_flag;               //��Чģ�͸��±�־ 
    uint8 audiopp_type;                          //audiopp_type_e
    uint16  precut_standard_mode;                  //��׼ģʽ��precutֵ
    int8  dynamic_precut_standard_mode;          //С�����¶�̬precut˥��   
    
    int16 compressor_threshold_diff; //mdrc ģʽ��compressor��ֵ��mdrc�����ֵ�Ĳ�ֵ
    uint16 makeup_gain_standard_mode;      
    dae_compressor_standard_mode_t   compressor_standard_mode;
    mdrc_extend_para_standard_mode_t mdrc_extend_para_standard_mode;
    mdrc_peak_detect_standard_mode_t        mdrc_peak_standard_mode;
    dae_mdrc_band_standard_mode_t*   p_mdrc_band_standard_mode;
   /**************************************************/
    uint8 volume_current;
} dae_config_t;

typedef struct
{
    short *p_fade_in_time_ms;
    short *p_fade_out_time_ms;
    short *p_fade_in_flag;
    short *p_fade_out_flag;
    short *p_DAE_change_flag;
} dae_fade_param_addr_t;

typedef enum
{
    /*!��ȡ����״̬*/
    GET_FADE_OUT_STATUS,
    /*!��ȡ���뵭��������ַ���Ա����ж���ֱ���޸�*/
    GET_FADE_PARAM_ADDR,
    /*!��ȡ����ֵ��ַ���Ա����ж���ֱ�Ӷ�ȡ*/
    GET_ENERGY_VALUE_ADDR,
} get_feature_type_e;

typedef enum
{
    /*!������ʼ��*/
    SET_PARAM_INIT = NULL,
    /*!������Ч���ò���*/
    SET_EFFECT_PARAM,
    /*!���ò�����*/
    SET_SAMPLE_INFO,
    /*!���õ���*/
    SET_FADE_OUT,
    /*!���õ���*/
    SET_FADE_IN,
    /*!�������������㷨����*/   
    SET_NOISE_REDUCTION_PARA,
    /*!max*/
    MAX_SET_TYPE
} set_effect_type_e;

#define OUTPUT_CHANNEL_NORMAL      0 //������Ĭ�ϳ�ʼ��Ϊ����
#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0

typedef struct
{
    /*!ͨ����*/
    uint32 chanel_num;
    /*!������*/
    uint32 sample_rate;
    /*!block size*/
    uint32 block_size;
    /*!���ͨ������ 1:������������ 2: ���˶���Ӧ��(L=R=ԭʼ��(L/2 + R/2)), 0 or ����ֵ, ��ԭʼ����������һ��*/
    int16 output_channel;
     /*!���뵭��������*/
    int32 fade_handle;
} init_play_param_t;

typedef struct
{
    short enable;				            //default: 200ms,step 0.01ms, set 200*100 = 20000    
    short DownExp_attack_time;				//default: 200ms,step 0.01ms, set 200*100 = 20000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;		    //default: 1ms, step 0.01ms, set 100;
	short DownExp_threshold;			    //default: -80dB(min), step 0.1dB, set -800
	short DownExp_ratio;				    //default: 3(greater than 1)  
}init_noise_reduction_t;


/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_DAE_DEF__
