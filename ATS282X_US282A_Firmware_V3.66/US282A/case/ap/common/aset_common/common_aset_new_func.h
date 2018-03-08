#ifndef _COMMON_ASET_NEW_FUNC_H
#define _COMMON_ASET_NEW_FUNC_H

#include "case_independent.h"
#include "psp_includes.h"
#include "mmm_dae_def.h"
#include "stub_interface.h"
#include "case_mem_define.h"
#include "stub_command.h"
#include "setting_common.h"
#include "common_func.h"

#define EQMAXPOINTCOUNT          14  //ÿ�������������14��PEQ��
#define EQMAXPOINTCOUNT_ALL      20  //pc�����֧�ֵĵ���

//�Զ�Ƶ���������
#define MAX_FR_POINT 4097
#define MOD_IR_NUM 1025
#define MAX_INVERSE_IR_NUM 64
#define LPC_ORDER 127

#define ASET_FIR_INFO_LEN 128

typedef struct
{
    uint32  update_flag;            //1��ʾPC���߸�����ϵ������Ҫ���ݵ�DSP LIB
    uint32  length;                 //�˴�Ҫ���µ�ϵ����ĳ���
    uint32  memory_addr;            //��Ҫ�����ڴ�ĵ�ַ       
}aset_waves_coeff_property_t;

typedef struct
{
    uint8 state; //0: not start 1:start
    uint8 upload_data; //0: not upload 1:upload
    uint8 volume_changed; //0:not changed 1:changed
    uint8 download_data; //0:not download 1:download

    uint8 upload_case_info; //0:not upload 1:upload
    uint8 main_switch_changed; //0:not changed 1:changed
    uint8 update_audiopp; //0:not update 1:update
    uint8 reserved[9];
    //ofs = 16
    uint8 eq_data_changed; //0:not change 1:change
    uint8 vbass_data_changed;
    uint8 te_data_changed;
    uint8 limiter_data_changed;
    // osf = 20
    uint8 spk_compensation_data_changed;
    uint8 surr_data_changed;
    uint8 multiband_drc_data_changed;//������Чģ����MDRC
    uint8 Left_frcurve_data_changed;
    // ofs = 24
    uint8 see_data_changed;
    uint8 sew_data_changed;
    uint8 SignalD_data_changed;
    uint8 Right_frcurve_data_changed;
   // ofs = 28
    uint8 compressor_changed;
    uint8 standard_mode_mdrc_changed;//��׼��Чģ����MDRC
    uint8 reserved1[2];
} aset_status_t;

//**********С���˵����ݽṹ********
typedef struct
{
    int16 freq; //EQƵ���Ƶ��
    int16 gain; //EQƵ�������ֵ
    int32 Qval; //EQƵ���Qֵ----תint
    int16 type; //EQƵ�������
    int8 enable; //EQƵ���Ƿ�ʹ�ܣ�0��ʾ��ֹ��1��ʾSpeaker PEQ��2��ʾPost PEQ
    int8 reserved[1]; //����
} aset_peq_point_t;

typedef struct
{
    int16 main_gain_value; //��������
    int16 equivalent_gain_value; //��Ч����
    int8 main_gain_enable; //��������ʹ��
    int8 peq_enable; //EQģ���Ƿ�ʹ��
    int8 nPointTotal; //����
    int8 MaxPointTotal; //������ 8bytes
    aset_peq_point_t peq_array[EQMAXPOINTCOUNT]; //���б� 12*14
} aset_peq_data_t;

typedef struct
{
    int8 nBlockPointTotal;  //�ε������̶�Ϊ14��
    int8 nMaxPointTotal;    //С���ϱ���������
    int8 szRev[2];
    aset_peq_point_t peq_array[EQMAXPOINTCOUNT]; //���б� 12*14
} aset_peq_data_2_3_t;

typedef struct
{
    int16 cutoff_freq; //�𣺣����������գ�  default:90
    int16 vbass_gain;  //PC���ݹ����������������ķ�Χ��0.0dB ~ 24.0dB  ����Ϊ0.1dB ������Ҫ��ȥ12.0dB�ٴ��ݸ�DSP
    int8 vbass_enable; //VBassģ��ʹ��
    int8 reserved[3]; //����
} aset_vbass_data_t;

typedef struct
{
    int16 te_gain; //��������0-15dB default:7dB
    int16 cutoff_freq; //������ǿ��ֹƵ�� Ĭ��ֵΪ10KHz
    int8 te_enable; //TEģ��ʹ��
    int8 reserved[3]; //����
} aset_te_data_t;

typedef struct
{
    int16 head_angle; //˫����������ͷ�ĽǶ��𣺣����������գ�defualt:10
    int16 surr_gain; //��������0~1   default:0.707
    int8 surr_enable; //Surroundģ��ʹ��
    int8 reserved[3]; //����
} aset_surr_data_t;

typedef struct
{
    int16 threhold; //��ֵ��-60:0.1:0dB����ʼ������-1dB
    int16 attack_time; //����ʱ�䣬.05:0.05:10ms����ֵ���󱶣�����Χ:5:1000����ʼ��Ϊ0.1ms
    int16 release_time; //�ͷ�ʱ�䣬:1:1000ms����ʼ��Ϊ300ms
    int8 limiter_enable; //ģ��ʹ��
    int8 reserved[1]; //����
} aset_limiter_data_t;

typedef struct
{
    int16 threshold; //��ֵ��-60:0.1:0dB����ʼ������-1dB
    int16 attack_time; //����ʱ�䣬.05:0.05:10ms����ֵ���󱶣�����Χ:5:1000����ʼ��Ϊ0.1ms
    int16 release_time; //�ͷ�ʱ�䣬:1:1000ms����ʼ��Ϊ300ms
} aset_mdrc_band_t;

typedef struct
{
    aset_mdrc_band_t mdrc_band[MAX_MDRC_SEG];
    int8 mdrc_enable;
    int8  mDRC2_enable;//���DRCģ���������־��������һ��������Ҫ����
    short sIndex;     //���ģ����ţ���һ��Ϊ���Դ�����
    char  szRev[2];   //����
} aset_mdrc_data_t;





/* ���ɵĲ����˲���ϵ����Ҫ����С����
** ����fir_orderΪ�˲�������
** fir_coeff_QΪ�˲���ϵ���Ķ���λ
** fir_coeffΪ�˲���ϵ������
*/
typedef struct{
    int fir_order;
    int fir_coeff_Q;
    uint16 total_size;
    uint16 cuf_size;
    int8 fir_coeff[ASET_FIR_INFO_LEN];
}aset_FIR_info_t;


typedef struct
{
    aset_FIR_info_t stFirInfo;
    int8 next_stFirInfo_index;      //��һ���ļ�����Ϊ0ʱ��ʾû����һ��
    int8 szRev[5];
    uint8 index;                    //0 ~ 15 ����ţ�ÿ����128�ֽ�
    bool bEnFRCurve;
}aset_FRCurve_data_t;


typedef struct
{
    short signalPrecut;             //�źź�˥��΢������Χ-3db ~ 0db����λ0.1db��Ĭ��ֵΪ0db
    short fMVolume;                 //����΢����������Χ-3db  �� 3db��step Ϊ.1db��Ĭ��Ϊdb
    short   nMaxMid;                //��Ƶ��Ƶ��Ƶ�㣺Hz ��20KHz��Ĭ��ֵΪKHz
    short   nMidMin;                //��Ƶ��Ƶ��Ƶ�㣺Hz ��20KHz��Ĭ��ֵΪHz
    short   limiterThresholdDiff;   //MDRC��Limiter��ֵ�Ĳ�ֵ
    int8    szRev[6];               //����
}aset_mDRC2_data_t;



typedef struct
{
    short nHpCutoffFre;
    short nRev;
    short nStartTVal;
    short nHoldTVal;
    short nEnhanceRange;
}aset_HP_t;//��ͨ�ṹ��


typedef struct
{
    short nLfCutoffFre;
    short nEnhanceVBass;
    short nStartTVal;
    short nHoldTVal;
    short nEnhanceRange;
}aset_LF_t;//��Ƶ�ṹ��


typedef struct
{
    short nHfCutoffFre;
    short nEnhanceTE;
    short nStartTVal;
    short nHoldTVal;
    short nEnhanceRange;
}aset_HF_t;//��Ƶ�ṹ��


typedef struct
{
    short nFrePoint;
    short nRev;
    short nStartTVal;
    short nHoldTVal;
    short nEnhanceRange;
}aset_FrePoint_t;//����Ƶ��


typedef struct
{
    aset_HP_t stHpWeakenInfo;     //��ͨ����
    aset_LF_t stLfEnhanceInfo;    //��Ƶ��ǿ
    aset_HF_t stHfEnhanceInfo;    //��Ƶ��ǿ
    aset_FrePoint_t arFrePoints[3];  //��������Ƶ��
    bool bEnableSEE;
    uint8 peak_ratio; //��Ч��ǿ�����ʣ���0.1��Ϊ��λ
    char szRev[2];
}aset_SEEnhance_data_t;




typedef struct
{
    aset_HP_t stHpEnhanceInfo;     //��ͨ��ǿ
    aset_LF_t stLfWeakenInfo;      //��Ƶ����
    aset_HF_t stHfWeakenInfo;      //��Ƶ����
    aset_FrePoint_t arFrePoints[3];   //��������Ƶ��
    bool bEnableSEW;
    char szRev[3];
}aset_SEWeaken_data_t;


typedef struct
{
    short fSignalDetePer;           //�źż�����ڣ�s ��2s��step Ϊms��Ĭ��ֵΪ.5s��
    short nSignalDetePerNum;        //�źż������������20��stepΪ��Ĭ��Ϊ��
    short fSignalDesTVal;           //С�ź�Ԥ����ֵ��-1db ��-6db��stepΪ0.1db��Ĭ��Ϊ-20��
    short nSignalDesPerNum;         //С�ź�Ԥ������������20��step Ϊ��Ĭ��ֵΪ��
    short nSignalMinVal;            //�źŴ�С��Ч��Сֵ( -60   �� 0��step Ϊ0.1db��Ĭ��ֵΪ-21db)
    bool  bEnableSD;
    char szRev[5];
}aset_SignalDete_data_t;


typedef struct
{
    int8 bEQ_v_1_0;
    int8 bVBass_v_1_0;
    int8 bTE_v_1_0;
    int8 bSurround_v_1_0;

    int8 bLimiter_v_1_0;
    int8 bMDRC_v_1_0;
    int8 bSRC_v_1_0;
    int8 bSEE_v_1_0;

    int8 bSEW_v_1_0;
    int8 bSD_v_1_0;
    int8 bEQ_v_1_1;
    int8 bMS_v_1_0;
    
    int8 bVBass_v_1_1;
    int8 bTE_v_1_1;

    int8 bEQ_v_1_2;
    int8 bMDRC_v_1_1;
    int8 bComPressor_v_1_0;   
    int8 szRev[119];
    int8 szVerInfo[8];//�����ϴ��ķ������Ͱ汾�ţ�ͳһ��д   
}aset_interface_config_t;



typedef struct
{
    int8 peq_point_num;        //֧�ֵ�PEQ����
    int8 download_data_over;   //�Ƿ��Ѷ�������
    int8 reserved[30];         //�����ֽ�
    aset_interface_config_t stInterface;
} aset_case_info_t;

//���ݱ�׼��Чģ�����ӵĽṹ��
typedef struct
{
    short nThreshold_01;           //��ֵ��-60::0dB����ʼ��-15dB
    short   nCpRatio_01;           //ѹ���ȣ�1~1000  ��ʼ��ֵΪ
    short nThreshold_02;           //��ֵ��-60::0dB����ʼ��-15dB
    short   nCpRatio_02;           //ѹ���ȣ�1~1000  ��ʼ��ֵΪ
    short nRmsAvgTime;             //RMS���ƽ��ʱ��(ms):   0.02~100.00   ��ʼ��ֵΪ.00
    short fAttack_time;            //����ʱ�䣬.02~100ms����ֵ���󱶣���ʼ��Ϊ.00
    short nRelease_time;           //�ͷ�ʱ�䣬~5000ms����ʼ��Ϊ

    bool  bLmEnable;               //ģ��ʹ��
    char  nIndex;                  //��־��Ϊaux��Ϊ��aux
    char  szRev[16];               //����
}Compressor_TP_Info_M;

typedef struct
{
    short nThreshold_01;           //��ֵ��-60::0dB����ʼ��-15dB
    short   nCpRatio_01;           //ѹ���ȣ�1~1000  ��ʼ��ֵΪ
    short nThreshold_02;           //��ֵ��-60::0dB����ʼ��-15dB
    short   nCpRatio_02;           //ѹ���ȣ�1~1000  ��ʼ��ֵΪ
    short nRmsAvgTime;             //RMS���ƽ��ʱ��(ms):   0.02~100.00   ��ʼ��ֵΪ.00
    short fAttack_time;            //����ʱ�䣬.02~100ms����ֵ���󱶣���ʼ��Ϊ.00
    short nRelease_time;           //�ͷ�ʱ�䣬~5000ms����ʼ��Ϊ
}DRC_CpVal_Info_M;

typedef struct
{
    DRC_CpVal_Info_M stDrcVal_M[3];//����drc
    bool  bDrcEnable;              //DRCģ���Ƿ�ʹ��
    char cDataFlag;                //���DRCģ���������־��x5A������һ��������Ҫ���͸�С����������û��
    short sIndex;                  //���ģ����ţ���һ��Ϊ���Դ�����
    char  szRev[14];               //���� 
} DRC_CP_Info_M;

//���drc�������ṹ��
typedef struct
{
    short fSignalPrecut;            //�źź�˥��΢������Χ-3db ~ 0db����λ.1db��Ĭ��ֵΪdb
    short fMVolume;                 //����΢����������Χ-3db  �� 3db��step Ϊ.1db��Ĭ��Ϊdb
    short   nMaxMid;                //��Ƶ��Ƶ��Ƶ�㣺Hz ��20KHz��Ĭ��ֵΪKHz
    short   nMidMin;                //��Ƶ��Ƶ��Ƶ�㣺Hz ��20KHz��Ĭ��ֵΪHz
    short  nLimiterThresholdDiff;   //MDRC��Limiter��ֵ�Ĳ�ֵ

    short fTsVal;                   //�����˲�����ֵ��-60  ��0.0��Ĭ��ֵΪ
    short fQVal;                    //�����˲���Qֵ��.1  ��0.9��Ĭ��ֵΪ.1
    short fStTime;                  //�����˲�������ʱ�䣺.02  ��100ms��Ĭ��ֵΪ.5
    short nReleaseTime;             //�����˲����ͷ�ʱ�䣺ms   ��5000ms��Ĭ��ֵΪ
    short fGainCom;                 //���油����        0.0   ��20.0db��Ĭ��ֵΪdb
    char    szRev[12];              //����
}DRC_TP_Info_M3;

typedef enum
{
   UNAUX_MODE = 0,
   AUX_MODE   = 1,
}aux_mode_e;

typedef struct
{
   uint8 aux_mode;      //aux_mode_e
   uint8 reserved[7];
}application_properties_t;

extern int32 aset_read_data(uint16 cmd, void *data_buffer, uint32 data_len) __FAR__;
extern int32 aset_cmd_deal(aset_status_t *aset_status) __FAR__;
extern int32 aset_write_case_info(void) __FAR__;
extern int32 aset_upload_sound_param(void) __FAR__;
extern void  aset_set_dae_init(void) __FAR__;
extern int32  __section__(".bank_2")aset_update_audiopp(void);


extern uint8 *g_aset_rw_buffer;


#ifdef SUPPORT_ASET_TEST
extern void  aset_reconnect_deal(void) __FAR__;
extern uint8 get_audiopp_type(void)__FAR__;

#endif

#endif
