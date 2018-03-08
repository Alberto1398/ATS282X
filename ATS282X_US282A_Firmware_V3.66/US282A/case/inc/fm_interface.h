#ifndef _FM_INTERFACE_H         
#define _FM_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"

typedef struct
{
    uint32 freq; //��ǰ��̨��Ƶ�ʡ���λΪ1k (���λ��Ϊ1)��ȡ���2 �ֽ�
    uint8 station; //��ǰ��̨��Ӧ�ĵ�̨����ţ�from 1 to station_count����ǰ��̨δ����ʱΪ0
    uint8 station_count; //��̨���еı����̨����
} fm_play_status_t;

/* ��̨����ģʽ��Ϊ����ģʽ�Ͳ˵�ѡ��ģʽ
 ** ����ڷ��û���̨����ģʽ�£����ŵĲ��κ�
 ** �˵���ѡ��Ĳ���һ�¡�
 ** �û���̨����ʱ������ģʽ�Ĳ���ΪBand_MAX
 ** �˵��еĲ���ά�ֲ���
 ** ��ʱ���ѡ�����Ԥ��򱣴浽Ԥ�裬���ǽ�
 ** �û���̨��Ƶ�㱣�浽��ǰ�˵�����ָ����
 ** Ԥ���б���ߴӲ˵�����ָ�����б�ѡ��Ƶ��ɾ��
 ** �����û���̨ʱ������ڲ˵��иı�˵�����
 ** ��ѡ��ֵ������в����л���������ΪԤ�貥��ģʽ*/
typedef enum
{
    Band_China_US = 0, //china/USA band   87500---108000,  step 100kHz
    Band_Japan, //Japan band   76000---90000, step 100kHz
    Band_Europe, //Europe band  87500---108000, step 50kHz
    Band_MAX //�����û���̨ʱ������ģʽ����ΪBand_MAX
} radio_band_e;

typedef enum
{
    releaseMUTE = 0, //�������
    SetMUTE, //����
} FM_MUTE_e;

typedef enum
{
    HARDSEEK = 0, //����Ӳ��seek ģʽ
    SOFTSEEK, //�������seek ģʽ
} FM_SeekMode_e;

typedef enum
{
    DIR_DOWN = 0, //������̨
    DIR_UP //������̨
} FM_SeekDir_e;

/* FM ��������ö������*/
typedef enum
{
    /*FMģ���ʼ��*/
    FM_INIT = 0,
    /*FMģ�����standby*/
    FM_STANDBY,
    /*����Ƶ�㿪ʼ����*/
    FM_SETFREQ,
    /*��ȡ��ǰ״̬��Ϣ*/
    FM_GETSTATUS,
    /*������������*/
    FM_MUTE_VOL,
    /*���������̨����*/
    FM_SEARCH,
    /*����Ӳ����̨����*/
    FM_HARDSEEK,
    /* ���õ�̨Ƶ��*/
    FM_SETBAND,
    /* ������̨����*/
    FM_SETTHROD,
    /* �ֶ��˳�Ӳ��seek ����*/
    FM_BREAKSEEK,
    /* ��ȡӲ��seek �Ƿ���ɱ�ʶ*/
    FM_GET_FLAG,
    /* ��ȡ��ǰƵ����Ϣ*/
    FM_GET_BAND,
    /* ��ȡ��ǰƵ����Ϣ*/
    FM_GET_FREQ,
    /* ��ȡ��ǰ��̨�ź�ǿ��*/
    FM_GET_INTENTY,
    /* ��ȡ����״̬*/
    FM_GET_ANTEN,
    /* ��ȡ��̨������״̬*/
    FM_GET_STEREO,
    //���Խӿ�
    FM_DEBUG,
} fm_cmd_e;

/* FM �������ϲ��ṩ��ͳһ��ڶ���*/
typedef void * (*fm_op_func)(void *, void *, void *);

/* �����ڲ�ʵ�ֵľ���ӿں�������*/
typedef struct
{
    fm_op_func Init;
    fm_op_func Standby;
    fm_op_func SetFreq;
    fm_op_func GetStatus;
    fm_op_func Mute;
    fm_op_func Search;
    fm_op_func HardSeek;
    fm_op_func SetBand;
    fm_op_func SetThrod;
    fm_op_func SeekBreak;
    fm_op_func GetSeekOver;
    fm_op_func GetBand;
    fm_op_func GetFreq;
    fm_op_func GetIntsity;
    fm_op_func GetAntenna;
    fm_op_func GetStereo;
    fm_op_func Debug;
} fm_driver_operations;

extern void * fm_op_entry(void *param1, void *param2, void *param3, fm_cmd_e cmd)__FAR__;

//��ʼ��
//uint8_band: ���Σ�radio_band_e����
//uint8_level:����
//uint32_freq:Ƶ��(��λkHz)
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_open(uint8_band, uint8_level, uint32_freq)\
    (int)fm_op_entry((void *)(uint32)(uint8_band),\
    (void *)(uint32)(uint8_level),\
    (void *)(uint32_freq), FM_INIT)

//�ر�
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_close()\
    (int)fm_op_entry((void *)(0), (void *)(0), (void *)(0), FM_STANDBY)

//����Ƶ��
//uint32_freq:Ҫ���õ�Ƶ��(��λ:kHz)
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_set_freq(uint32_freq)\
    (int)fm_op_entry((void *)(uint32_freq), (void *)(0), (void *)(0), FM_SETFREQ)

//�õ�fm��״̬
//p_status: Engine_Status_t��ָ��
//uint8_mode: 1��ʾ��SEEK�Ĺ�����ȡ��Ϣ, 0��ʾ�������ŵĹ�����ȡ��Ϣ
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_status(p_status, uint8_mode)\
    (int)fm_op_entry((void *)(p_status),\
    (void *)(uint32)(uint8_mode),\
    (void *)(0), FM_GETSTATUS)

//���þ�������
//uint8_mode:FM_MUTE_e��
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_mute(uint8_mode)\
    (int)fm_op_entry((void *)(uint32)(uint8_mode),\
    (void *)(0), (void *)(0), FM_MUTE_VOL)

//�����̨
//uint32_freq: Ƶ��(��λ:kHz)
//uint8_direct: FM_SeekDir_e��
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ��� 1��ʾ�ɹ���������������
#define fm_soft_seek(uint32_freq, uint8_direct)\
    (int)fm_op_entry((void *)(uint32_freq),\
    (void *)(uint32)(uint8_direct),\
    (void *)(0), FM_SEARCH)

//Ӳ����̨
//uint32_freq: Ƶ��(��λ:kHz)
//uint8_direct: FM_SeekDir_e��
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_hard_seek(uint32_freq, uint8_direct)\
    (int)fm_op_entry((void *)(uint32_freq),\
    (void *)(uint32)(uint8_direct),\
    (void *)(0), FM_HARDSEEK)

//���ò���
//uint8_band: radio_band_e��
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_set_band(uint8_band)\
    (int)fm_op_entry((void *)(uint32)(uint8_band),\
    (void *)(0), (void *)(0), FM_SETBAND)

//������̨����
//uint8_level: ���޵ļ��𣬸�FM�����йء�
//  ���FM��������10�������ﴫ��ȥ�Ĳ����Ϳ�����0~9.
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_set_throd(uint8_level)\
    (int)fm_op_entry((void *)(uint32)(uint8_level),\
    (void *)(0), (void *)(0), FM_SETTHROD)

//�ж���̨����
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_set_breakseek()\
    (int)fm_op_entry((void *)(0), (void *)(0), (void *)(0), FM_BREAKSEEK)

//��ѯ�Ƿ�����̨
//p_flag:ָ��uint8�͵�ָ��
//  *p_flag��
//  bit0��ʾ��̨�Ƿ����    1:�ѽ���    0:δ����
//  bit1�ҵ����Ƿ���Ч̨    1:��Ч̨
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_seekflag(p_flag)\
    (int)fm_op_entry((void *)(p_flag),\
    (void *)(0), (void *)(0), FM_GET_FLAG)

//��ѯ����
//p_band: ָ��radio_band_e�͵�ָ��, *p_band���ǲ���
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_band(p_band)\
    (int)fm_op_entry((void *)(p_band),\
    (void *)(0), (void *)(0), FM_GET_BAND)

//��ѯƵ��
//p_freq: ָ��uint32�͵�ָ��, *p_freq����Ƶ��(��λkHz)
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_freq(p_freq)\
    (int)fm_op_entry((void *)(p_freq),\
    (void *)(0), (void *)(0), FM_GET_FREQ)

//��ѯ�ź�ǿ��
//p_intensity: ָ��uint8�͵�ָ��, *p_intensity�����ź�ǿ��
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_intensity(p_intensity)\
    (int)fm_op_entry((void *)(p_intensity),\
    (void *)(0), (void *)(0), FM_GET_INTENTY)

//��ѯ�Ƿ�������
//p_antena:ָ��uint8�͵�ָ�� *p_antenaΪ1ʱ��ʾ�����ߣ�Ϊ0ʱ��ʾû��
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_antena(p_antena)\
    (int)fm_op_entry((void *)(p_antena),\
    (void *)(0), (void *)(0), FM_GET_ANTEN)

//��ѯ�Ƿ�������
//p_stereo:ָ��uint8�͵�ָ�� *p_stereoΪ1ʱ��ʾ��������Ϊ0ʱ��ʾ����
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_get_stereo(p_stereo)\
    (int)fm_op_entry((void *)(p_stereo),\
    (void *)(0), (void *)(0), FM_GET_STEREO)

//���������Ľӿ�
//����ֵ:-1��ʾʧ�ܣ�0��ʾ�ɹ�
#define fm_debug()\
    (int)fm_op_entry((void *)(0), (void *)(0), (void *)(0), FM_DEBUG)

#endif

