#ifndef __AP_AUTOTEST_CHANNEL_TEST_THD_H
#define __AP_AUTOTEST_CHANNEL_TEST_THD_H

//����ú꽫ʹ�ò������������
#define USE_TABLE

//������FFT�ĵ���
#define LENGTH  128

//������ô����FFT��Ҫ���ٸ�������λ
//bL=log2(LENGTH)
#define bL 7

#define IN_TYPE  short int
#define OUT_TYPE long int
#define LEN_TYPE long int


typedef long long __int64 ;

#define PI 3.1415926535897932384626433832795

typedef struct
{   
    //����õ����������ֵ
    int32 SNR;    
    //��ֵ����
    int32 MaX_Sig_Point;
    //ʱ�����ֵ
    int32 AmpMax;
    //ʱ����Сֵ
    int32 AmpMin;
    //Ƶ���źŷ��ȸ�32bit
    int32 SigHigh;    
    //Ƶ���źŷ��ȵ�32bit
    int32 SigLow;
    //Ƶ���������ȸ�32bit
    int32 NoiseHigh;  
    //Ƶ���������ȵ�32bit
    int32 NoiseLow;  
}snr_result_t;

extern LEN_TYPE BitRev[LENGTH]; 

//���ʵ������
extern OUT_TYPE Re[LENGTH];    

//����鲿����
extern OUT_TYPE Im[LENGTH]; 

extern const short int COS_TABLE[];
extern const short int SIN_TABLE[];

extern void InitBitRev(void);
extern void FftInput(IN_TYPE *pIn);
extern void FftExe(IN_TYPE *pIn, OUT_TYPE *pRe, OUT_TYPE *pIm);

extern uint32 thd_test(void *buffer_addr, channel_test_arg_t *channel_test_arg) __FAR__;

#endif

