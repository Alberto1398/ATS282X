/********************************************************************************
 *                              GL5116
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona    2015-01-27 15:00     1.0             build this file
 ********************************************************************************/

#ifndef _DAE_PARA_H
#define _DAE_PARA_H

#include <typeext.h>
#include <attr.h>

//֧�ֶ�Ƶ���Ƶ�ο���
#define SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING   1
#define EQ_NUM_BANDS 20   //NOTE!! ��ͬ���� mmm_dae_def.h �궨�� MAX_PEQ_SEG

struct eq_band_setting    //NOTE!! ��ͬ���� mmm_dae_def.h �ṹ�嶨�� peq_band_t
{
    short cutoff;   //����Ƶ�ʣ�20:1:24000Hz
    short q;        //0.3:0.1:3����ֵ����10��������Χ-3:1:30
    short gain;     //-20:0.1:12dB����ֵ����10��������Χ-200:1:120
    short type;     ///*�˲������ͣ�0����Ч��1�ǳ���EQ�˲�����2�Ǹ�ͨ�˲���������ֵΪreserve*/
};

#define MDRC_NUM_BANDS 3  //NOTE!! ��ͬ���� mmm_dae_def.h �궨�� MAX_MDRC_SEG

struct mdrc_band_setting  //NOTE!! ��ͬ���� mmm_dae_def.h �ṹ�嶨�� mdrc_band_t
{
    short threshold;
    short attack_time;
    short release_time;
    short reserve;//ȷ��4�ֽڶ���
};

typedef struct
{
    short enable;				            //default: 200ms,step 0.01ms, set 200*100 = 20000    
    short DownExp_attack_time;				//default: 200ms,step 0.01ms, set 200*100 = 20000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;		    //default: 1ms, step 0.01ms, set 100;
	short DownExp_threshold;			    //default: -80dB(min), step 0.1dB, set -800
	short DownExp_ratio;				    //default: 3(greater than 1)  
}dae_noise_reduction_t;

#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
#define MFBE_BANDS     10
#define MAX_FREQ_POINT 10
#endif

typedef struct
{
    int sample_rate;            //�����ʣ�֧��44100,48000Hz����
    short channels;               //Ŀǰ���ǰ�˫ͨ������
    short block_size;             //֡����ĿǰΪ128�㳤��
	
    int   fade_handle;            //���뵭����������������ص���Ч�ⲻ֧�ֵ��뵭������ֵΪ���뵭�������ĵ�ַ�����֧����ΪNULL
    short fade_in_time_ms;        //���볤�ȣ�[50 100 200 300 400 500]ms
    short fade_out_time_ms;       //�������ȣ�[50 100 200 300 400 500]ms
    short mute_time_ms;           //�������ȣ�����û�����ƣ���λms
    short fade_in_flag;           //�����־λ����1��ʾ��һ֡��ʼ����
    short fade_out_flag;          //������־λ����1��ʾ��һ֡��ʼ����
    short mute_flag;              //������־λ����1��ʾ��һ֡��ʼ����
	
    short DAE_init_flag;          //��ʼ����ʶλ
    short DAE_change_flag;        //DAEȫ�������ı��ʶλ
    short output_channel_config;//1:������������ 2: ���˶���Ӧ��(L=R=ԭʼ��(L/2 + R/2)), 0 or ����ֵ, ��ԭʼ����������һ�£��ù�������Ч�޹أ����� BYPASS Ӱ��
    short bypass;               //bypass,1��ʾֱͨ��0��ʾDAE����
    short precut;                 //Ԥ˥������ʼ��Ϊ0��precut ���� BYPASS Ӱ��
	/***********************************/

//ȷ��4�ֽڶ���
    short Vbass_enable;         //Vbass����
    short Surround_enable;      //Surround����
    short TrebleEnhance_enable; //Treble����
    short Peq_enable;           //Peq����
    short SpeakerCompensation_enable;//SpeakerCompensation����
    short Limiter_enable;       //Limiter����
    short MultibandDRC_enable;  //MDRC����
    short DownwardExpander_enable; //downward expander(noise gate)����	//ȷ��4�ֽڶ���
    /***********************************/
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)    
    short MultiFreqBandEnergy_enable;//��Ƶ���������㿪��
    short FreqSpetrumDisplay_enable;//��Ƶ���������㿪��
#endif  

    /*******for noise reduction*********/
	short DownExp_attack_time;				//default: 100ms,step 0.01ms, set 10*100 = 1000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;				//default: 5, step 1, set 5;��ʾ����ֵ�ڸ�ֵ����ֱ����0
	short DownExp_threshold;					//default: -75dB(min), step 0.1dB, set -750
	short DownExp_ratio;						//default: 5(greater than 1)

//ȷ��4�ֽڶ���
    /***********************************/
	short Vbass_low_cut_off_frequency;
	short Vbass_gain;
    int Vbass_type;             //�������������
    /***********************************/
	short Surround_angle;
	short Surround_gain;
    /***********************************/
	short Treble_frequency;
	short Treble_gain;
    /***********************************/
    //int MDRC_change_flag;     //MDRC�����Ĳ����ı��ʶλ��MDRC�����ı�ʱ��1��ͬʱDAE_change_flag��1
    int crossover_freqency[MDRC_NUM_BANDS-1];
    short MDRC_compensation_peak_detect_attack_time;	//default: 0.5ms, step: 0.01ms, setting: 5*10
	short MDRC_compensation_peak_detect_release_time;	//default: 500ms, step: 1ms, setting: 500
	short MDRC_compensation_threshold;					//default: -100,step 0.1dB,setting: -10*10 = -100
	short MDRC_compensation_filter_Q;					//default: 1,step 0.1, setting:0.1*10 = 1, 0.1~0.9
    /***********************************/
    int Peq_change_flag;        //peq�����Ĳ����ı��ʶλ��peq�����ı�ʱ��1��ͬʱDAE_change_flag��1
    struct eq_band_setting band_settings[EQ_NUM_BANDS]; //peqƵ���������band_settings���ĸ��������иı�ʱPeq_change_flag��DAE_change_flagͬʱ��1
    short band_enable_type[EQ_NUM_BANDS]; //peq���ͺ�enableѡ�1��ʾSpeaker PEQ��2��ʾPost PEQ��0��ʾ��ִ��
    /***********************************/
    short SpeakerCompensationChangeFlag;
    short FilterOrder;
    short FilterQvalLeft;
    short FilterQvalRight;
    /***********************************/
    int makeup_gain;            //makeup gain�����ź�������0DB���Խ������������������
    
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
    /**************Multi-freq band energy*********************/
    short duration_ms;                      //ͳ��ʱ������λms
    short num_band;                         //Ƶ����,�㷨�ڲ����֧��10��
	short f_c[MFBE_BANDS];					//��ͨ����Ƶ��
	short energys[MFBE_BANDS];				//���������Ƶ���������������˶�!!!
	/**************freq spetrum display*********************/
	int num_freq_point;						//Ƶ����,�㷨�ڲ����֧��12��
	short freq_point[MAX_FREQ_POINT];			//��Ƶ������
	short freq_point_mag[MAX_FREQ_POINT];		//���������Ƶ���������������˶�!!!
#endif
 
    /***********************************/
    short energy_detect_enable; //�ź��������ʹ��
    short period;               //ͳ������ʱ������1msΪ��λ
    short period_count;         //��Ч������
    short preadjust_db;         //�źŴ�С���ͽ϶������Ԥ�����ƣ���0.1dbΪ��λ
    short preadjust_count;      //��ʼԤ�������źŴ�С����������Ȼ��ÿ�����ڽ�����Ӧ����
//signal_energy_inner�������energyǰһ��λ��
    short signal_energy_inner;  //����������ȥ����ֵ���� �л��źż��ʱ��ų�ʼֵ
    /***********************************/
    short  energy;     //����֡pcmƽ��ֵ
    short  energy_max; //����֡pcm����ֵ���ֵ
//����3����Ա���������
	
	/***********************************/
	short Limiter_threshold;
	short Limiter_attack_time;
	short Limiter_release_time;
	short reserve;
	
	struct mdrc_band_setting mdrc_band_settings[MDRC_NUM_BANDS];
	
}DAE_para_info_t;

extern DAE_para_info_t g_dae_param_info;

#endif //_DAE_PARA_H
