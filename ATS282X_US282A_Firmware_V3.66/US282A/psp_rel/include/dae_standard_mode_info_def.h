/********************************************************************************
 *                              GL5116
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 *      History:
 *      <author>            <time>           <version >             <des>
 *     zhuangyongkang    2016-05-26 10:00       1.0             Standard Sound Models
 ********************************************************************************/

#ifndef _DAE_PARA_STANDARD_MODE_INFO_DEF_H
#define _DAE_PARA_STANDARD_MODE_INFO_DEF_H

#include <typeext.h>
#include <attr.h>
#include <dae_info_def.h>

//NOTE:��׼��Чģ��
#define EQ_NUM_BANDS_STANDARD_MODE             EQ_NUM_BANDS
#define MDRC_NUM_BANDS_STANDARD_MODE            3

struct eq_band_setting_standard_mode    
{
    short cutoff;                       //����Ƶ�ʣ�20:1:24000Hz
    short q;                            //0.3:0.1:3����ֵ����10��������Χ-3:1:30
    short gain;                         //-20:0.1:12dB����ֵ����10��������Χ-200:1:120
    short type;                         ///*�˲������ͣ�0����Ч��1�ǳ���EQ�˲�����2�Ǹ�ͨ�˲���������ֵΪreserve*/
};

struct mdrc_band_setting_standard_mode
{
    short threshold1;
    short compress_ratio1;
	short threshold2;
    short compress_ratio2;
	short tav_time;
    short attack_time;
    short release_time;
	short reserve;//ȷ��4�ֽڶ���
};

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
    short  output_channel_config;//1:������������ 2: ���˶���Ӧ��(L=R=ԭʼ��(L/2 + R/2)), 0 or ����ֵ, ��ԭʼ����������һ�£��ù�������Ч�޹أ����� BYPASS Ӱ��
    short  bypass;               //bypass,1��ʾֱͨ��0��ʾDAE����
	
    short precut;                 //Ԥ˥������ʼ��Ϊ0��precut ���� BYPASS Ӱ��
	/***********************************/

//ȷ��4�ֽڶ���
    short Vbass_enable;         //Vbass����
    short Surround_enable;      //Surround����
    short TrebleEnhance_enable; //Treble����
    short Peq_enable;           //Peq����
    short SpeakerCompensation_enable;//SpeakerCompensation����
	short Compressor_enable;
    short MultibandDRC_enable;  //MDRC����
	short DownwardExpander_enable; //downward expander(noise gate)����
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
	short MultiFreqBandEnergy_enable;
	short FreqSpetrumDisplay_enable;
#endif
	
	/*******for noise reduction*********/
	short DownExp_attack_time;				//default: 10ms,step 0.01ms, set 10*100 = 20000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;				//default: 5, step 1, set 5;��ʾ����ֵ�ڸ�ֵ����ֱ����0
	short DownExp_threshold;					//default: -75dB(min), step 0.1dB, set -750
	short DownExp_ratio;						//default: 5(greater than 1)
	/***********************************/

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
    int crossover_freqency[MDRC_NUM_BANDS_STANDARD_MODE-1];
    short MDRC_compensation_peak_detect_attack_time;	//default: 0.5ms, step: 0.01ms, setting: 5*10
	short MDRC_compensation_peak_detect_release_time;	//default: 500ms, step: 1ms, setting: 500
	short MDRC_compensation_threshold;				//default: -100,step 0.1dB,setting: -10*10 = -100
	short MDRC_compensation_filter_Q;					//default: 1,step 0.1, setting:0.1*10 = 1, 0.1~0.9
    /***********************************/
    int Peq_change_flag;        //peq�����Ĳ����ı��ʶλ��peq�����ı�ʱ��1��ͬʱDAE_change_flag��1
    struct eq_band_setting_standard_mode band_settings[EQ_NUM_BANDS_STANDARD_MODE]; //peqƵ���������band_settings���ĸ��������иı�ʱPeq_change_flag��DAE_change_flagͬʱ��1
    short band_enable_type[EQ_NUM_BANDS_STANDARD_MODE]; //peq���ͺ�enableѡ�1��ʾSpeaker PEQ��2��ʾPost PEQ��0��ʾ��ִ��
    /***********************************/
    short SpeakerCompensationChangeFlag;
    short FilterOrder;
    short FilterQvalLeft;
    short FilterQvalRight;
    /***********************************/
    int makeup_gain;            //makeup gain �������µ����ã��������������ȣ�С������Ϊ0  
    
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
	/**************Multi-freq band energy*********************/
	short duration_ms;						//ͳ��ʱ������λms
	short num_band;							//Ƶ����,�㷨�ڲ����֧��10��
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
	short Compressor_threshold1;
	short Compressor_ratio1;
	short Compressor_threshold2;
	short Compressor_ratio2;
	short Compressor_tav;
	short Compressor_attack_time;
	short Compressor_release_time;
	short power_regulation;                      //���ʵ���
	struct mdrc_band_setting_standard_mode mdrc_band_settings[MDRC_NUM_BANDS_STANDARD_MODE];
	/***********************************/
	
  
}DAE_para_info_standard_mode_t;


#endif //_DAE_PARA_H
