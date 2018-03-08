#ifndef _MMM_AGC_AEC_DEF_H_
#define _MMM_AGC_AEC_DEF_H_


//AEC���ܲ����ṹ�����£�
typedef struct
{
	uint32 nms_in;
	uint32 nms_mic;
	uint32 nms_echo;
	uint32 nms_aec;
	uint32 nms_ns;
	uint32 nms_out;
	uint32 maxrefenv;
	uint32 refisaudible;
	uint32 leak;
	uint32 coeff;
	uint32 dt;
}sv_aec_info_t;


typedef struct
{
    uint32 mic_op_gain;      //MICģ�����棺0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db��Ĭ��Ϊ0
    uint32 mic_adc_gain;     //MIC�������棺0:-70.5db, 47:0db, ÿ������1.5db��Ĭ��Ϊ47
    uint32 speaker_channel;  //����ѡ��1��ʾ��������2��ʾ��������3��ʾ˫������Ĭ��Ϊ1
    uint32 dac_weak;         //����˥����0~18db��1-18��˥��1db-18db����ʾ�����ȣ�33-50��˥��1db-18db����ʾ�����ȣ�Ĭ��Ϊ0
    uint32 pa_volume;        //IC�ڲ�pa volume�����40����С0��Ĭ��Ϊ28
    uint32 pre_agc_enable;   //ǰ��AGCʹ�����Ĭ��Ϊ1
    uint32 pre_agc_level;    //ǰ��AGC level��12000 - 32000��Ĭ��20000
    uint32 pos_agc_enable;   //���AGCʹ�����Ĭ��Ϊ1
    uint32 pos_agc_level;    //���AGC level��12000 - 32000��Ĭ��20000
    
    uint32 frame_size;       //֡��ʱ�䣬��λms��Ŀǰ��֧�� 16ms ֡��
    uint32 sampling_rate;    //�����ʣ���λHz��Ŀǰ��֧�� 8000Hz ������
    uint32 aec_enable;       //aecģ�鿪�أ����򿪣����ر�
    uint32 tail_length;      //����β������λ ms������Ϊ16�ı���,Ŀǰ��֧�� 32 ms
    uint32 nlp_enable;       //�����Դ�����ģ�鿪�أ����򿪣����رգ�Ϊ1 ʱ���뱣֤aec_enable Ϊ1
    uint32 nlp_aggressiveness; //�����Դ���̶ȣ�����~15,Խ���������Դ���Խ����
    uint32 dt_threshold;     //˫���������������̶ȣ�����~15��Խ���ʾ˫���ǽ�����������Խ�࣬���ǿ��ܻ���������Ļ��������Ҹ�����ֵ��
    uint32 aes_enable;       //��������������ģ�鿪�أ����򿪣����رգ�Ϊ1 ʱ���뱣֤aec_enable Ϊ
    uint32 echo_suppress;    //����������ģ�鵥���������Ʊȣ���λdB��0~40
    uint32 echo_suppress_avtive; //����������ģ��˫���������Ʊȣ���λdB��0~40
    
    uint32 ans_enable;       //���뿪�أ����򿪣����ر�
    uint32 noise_suppress;   //�������Ʊȣ���λdB��0~25
    
    uint32 cng_enable;       //�����������أ����򿪣����ر�
    uint32 noise_floor;      //��������ˮƽ����λdB Fs, һ��ӦС��-45 dB
    
    uint32 analysis_mode;    //����ģʽ�����ڷ���ʹ��
    uint32 hfp_mode;//ͨ��//0��˫��1�ǵ���
}sv_configs_t;



typedef struct
{
	int aec_enable;			//aecģ�鿪�أ�1���򿪣�0���ر�
	int tail_length;		//����β������λ ms������Ϊ16�ı���,Ŀǰ��֧�� 32 ms
	//int bulk_delay;			//bulk delay
	int nlp_enable;			//�����Դ�����ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ 1
	int nlp_aggressiveness; //�����Դ���̶ȣ�����0~15,Խ���������Դ���Խ����
	int dt_threshold;	    //˫���������������̶ȣ�����0~15��Խ���ʾ˫���ǽ�����������Խ�࣬���ǿ��ܻ���������Ļ��������Ҹ�����ֵ��
	int aes_enable;			//��������������ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ1
	int echo_suppress;		//����������ģ�鵥���������Ʊȣ���λdB�� 0~40
	int echo_suppress_avtive; //����������ģ��˫���������Ʊȣ���λdB�� 0~40
}aec_prms_t;



typedef struct
{
    int ans_enable;         //���뿪�أ�1���򿪣�0���ر�
    int noise_suppress;     //�������Ʊȣ���λdB�� 0~25
}ns_prms_t;

typedef struct
{
    int cng_enable;     //�����������أ�1���򿪣�0���ر�
    int noise_floor;    //��������ˮƽ����λ dB Fs, һ��ӦС��-45 dB
}cng_prms_t;


#if 1
typedef struct
{
	int pf_enable;		//ʹ�ܿ��أ�1���򿪣� 0���ر�
	int blocksize;		//�����ĳ��ȣ��̶�256�����Բ��ÿ���
	int overlapsize;	//����齻���ӳ��ȣ��̶�128�����Բ��ÿ���
	int order;			//�˲�������;ȡֵ��Χ[2~8]��Ĭ��3
	int lamda;			//�˲���ƽ��ϵ����Խ���ʾԽƽ�����������ٶ�Խ��;ȡֵ��Χ[0,31]��Ĭ��12
	int belta;			//�˲������������Բ�����Խ��ȥ����Խ����������Ӱ��˫������;ȡֵ��Χ[0~15]��Ĭ��10
	int stepsize;		//�˲���ϵ�����²���,Խ�����Խ�죬��ʧ������Խ��ȡֵ��Χ[0~15]��Ĭ��7

}pflt_prms_t;
#endif


typedef struct
{
    uint32 frame_size;   //֡��ʱ�䣬��λms��Ŀǰ��֧�� 16ms ֡��
    uint32 sampling_rate; //�����ʣ���λHz��Ŀǰ��֧�� 8000Hz ������
    // aec prms
    aec_prms_t aec_prms;    //ofs = 4*2B
    //ns prms
    ns_prms_t ns_prms;    // ofs = 4*10B
    //cng prms
    cng_prms_t cng_prms; //ofs = 4*12B
    //powerfilter prms
#if 1
    pflt_prms_t pflt_prms;//ofs = 4*14B
#endif
    uint32  pcm_mode;   //0:�ϴ�pcm, 1:���ϴ���2�ϴ�ȫ0����(mute),ofs = 4*21B
    uint32  air_pkt_len;     //�����������ݰ���ofs = 4*22B
    uint32 speaker_channel; //��绰ʱ��������ʹ�ܣ�1����������2����������3��˫������
    uint32 speaker_channel_weak_sel;//0��ʾ��˥����1��ʾ��������2��ʾ������
    uint32 speaker_channel_weak_val;//˥�����ȣ�0-18db
    uint32 hfp_mode;//ͨ��0��˫��1�ǵ���
    uint32 pre_agc_level;//ǰ��AGC level��[0-15]��
    uint32 pos_agc_level;//���AGC level��[0-15]��
    uint32 agc_pre_en;//ǰ��agcʹ�ܣ�0�رգ�1��
    uint32 agc_back_en;//���agcʹ�ܣ�0�رգ�1��
    uint32 hfp_connect_mod;// /0 û��ͨ   1 ��ͨ
}sv_prms_t;//dsp�ڲ��ṹ

typedef struct
{
    int nms_in;
    int nms_mic;
    int nms_echo;
    int nms_aec;
    int nms_ns;
    int nms_out;
    int maxrefenv;
    int refisaudible;
    int leak;
    int coeff;
    int dt;
}sv_info_t;

#endif //_MMM_AGC_AEC_DEF_H_
