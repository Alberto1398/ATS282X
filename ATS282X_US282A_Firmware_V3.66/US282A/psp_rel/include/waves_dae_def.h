#ifndef __WAVES_DAE_DEF__
#define __WAVES_DAE_DEF__
#include <psp_includes.h>

//0x9fc3afe0-9fc3afff(32�ֽ�)��ַ���PC���߷�����ϵ���� Infobuffer
#define WAVES_ASET_COEFF_ADDR                     (0x9fc3afe0)//(dsp��ַ:0x2d7f0)
//M4K��DSPЭ�齻���ṹ��
typedef struct
{
    int DAE_change_flag;            //1��ʾ��������Чϵ��;!!!ע��M4K��λ��DSPȡ�߲�����λ
    int parameter_address;          //������Чϵ��������ڴ��е���ʼ��ַ
    int parameter_length;           //���µ���Чϵ������
    int reserve[5];                 //��չ��
}coeff_t;


/*****��0x9fc37000��ʼ��ַ���1616�ֽڵ����ݴ�ŵ���WAVES��Ч����*********************///DAEbuffer
#define WAVES_DAE_PARA_START_ADDR                 (0x9fc37000)//(dsp��ַ:0x2b800)
#define WAVES_DAE_PARA_END_ADDR                   (0x9fc37650)//(dsp��ַ:0x2bb28)
#define WAVES_DAE_PARA_LENGTH                     (WAVES_DAE_PARA_END_ADDR - WAVES_DAE_PARA_START_ADDR)//(1616�ֽ�) 

//waves��Чinput������ʼ��ַ
#define WAVES_INPUT_PARA_START_ADDR               (0x9fc37008)//(mips��ַ:0x9fc37008) 

#define VM_DATA_LENGTH                            (512) 
#define VM_WAVES_AL_DATA1                         (0x00310000)
#define VM_WAVES_AL_DATA2                         (0x00320000) 
#define VM_WAVES_AL_DATA3                         (0x00330000) 
#define VM_WAVES_AL_DATA4                         (0x00340000) 
#define VM_WAVES_AL_DATA5                         (0x00350000) 
#define VM_WAVES_AL_DATA6                         (0x00360000) 
#define VM_WAVES_AL_DATA7                         (0x00370000) 
#define VM_WAVES_AL_DATA8                         (0x00380000) 
#define VM_WAVES_AL_DATA9                         (0x00390000) 
#define VM_WAVES_AL_DATA10                        (0x003a0000) 
#define VM_WAVES_AL_DATA11                        (0x003b0000) 
#define VM_WAVES_AL_DATA12                        (0x003c0000) 
#define VM_WAVES_AL_DATA13                        (0x003d0000) 
#define VM_WAVES_AL_DATA14                        (0x003e0000) 

typedef struct
{
    uint8  data[VM_DATA_LENGTH];
}waves_dae_para_t;


//PC���ߴ��ݵ���Чϵ����ṹ��
typedef struct
{
    uint32  update_flag;            //1��ʾPC���߸�������Чϵ��; 0��ʾû�и���
    uint32  length;                 //���µ���Чϵ������
    uint32  memory_addr;            //������Чϵ��������ڴ��е���ʼ��ַ      
}coeff_property_t;

typedef enum
{
    /*!����ϵ����*/
    SET_WAVES_COEFF_PROPERTY,
    /*!������Ч���ò���*/
    SET_WAVES_EFFECT_PARAM,
    /*!��Ч�������ڴ�д��VRAM*/
    WAVES_DAE_WRITE_VRAM,
    /*!��Ч�������ڴ�д��VRAM*/
    WAVES_DAE_READ_VRAM,
    /*!��bin�ļ��м�����Ч����*/
    SET_WAVES_PARAM_FROM_SDFS,
    /*!����input��Ч����*/
    SET_WAVES_INPUT_PARAM 
} set_waves_effect_type_e;

//0x9fc3b000-0x9fc3b800(2k�ռ�������Ϊ���ߺ�С�����ݽ����Ļ���buffer������ֻ����256���ֽڣ�ʣ��Ŀռ������������;)Tmpbuffer
//#define WAVES_ASET_SHARE_MEM_START_ADDR           (0x9fc3b000)//(dsp��ַ:0x2d800)
//#define WAVES_ASET_SHARE_MEM_END_ADDR             (0x9fc3b800)//(dsp��ַ:0x2dc00)
/******************************************************************/

//waves��Ч���֧�ּ���bin�ļ����ж�̬�л�
#define WAVES_DAE_PARA_BIN_MAX_NUM                  3
    
#endif
