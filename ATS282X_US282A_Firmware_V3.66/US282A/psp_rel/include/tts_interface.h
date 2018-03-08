#ifndef _TTS_INTERFACE_H
#define _TTS_INTERFACE_H

#include <typeext.h>
#include <api.h>

//tts����Ƭ����Ϣ
typedef struct
{
    uint32 offset; //����Ƭ����ʼ��ַ
    uint32 length; //����Ƭ�����ݳ���
} tts_section_t;

//TTS������ͷ���ݽṹ,ռ��32byte,reserved 24byte
typedef struct
{
    uint16 wordcnt; //TTS�ֿ⺺�ֵĸ���                 //������6768
    uint16 bytes1; //ÿ��������Ϣ��ռ�ֽ���            // 8
    uint16 multi_cnt; //������������������                //��ǰ15��
    uint16 bytes2; //ռ���ֽ���                        //8
    uint16 citiaocnt; //��չ�����ĸ���                    //31
    uint16 bytes3; //ÿ����չ������Ϣ��ռ�ֽ���        // 6
} tts_header_t;

typedef struct
{
    uint32 start_offset; //
    uint16 str_length; //
    uint16 multi_offset; //      ����������ƫ�ƣ�0XFFFF����û�ж�����
} tts_pos_t;

typedef struct
{
    uint8* str_buf; //�洢�ַ���Ϣ��ַ���ַ�������ķ�ʽ�洢
    uint8 reserve[2]; //����
} tts_info_t;

typedef struct
{
    char multitone_tag; //
    char qu; //
    char wei; //      ����������ƫ�ƣ�0XFFFF����û�ж�����
} tts_multitone_t;

/* tts ��������ö������*/
typedef enum
{
    /*tts��ʼ��*/
    TTS_PLAY_INIT = 0,
    /*tts�˳�*/
    TTS_PLAY_EXIT,
    /*TTS֡����*/
    TTS_PLAY_FRAME
} tts_cmd_e;

//�������
extern void* tts_op_entry(void *param1, void *param2, void *param3, tts_cmd_e cmd)__FAR__;

/*int32 tts_play_init(uint8 *section_ids, uint8 section_cnt, tts_info_t* pb_info)*/
/*����tts���Ų���*/
/*section_ids:���ŵĴ�����id���;section_cnt:��������;pb_info:�绰�������ַ���(���ΪNULL��Ϊ��������)*/
#define tts_status_play_init(a,b,c)    (int)tts_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), TTS_PLAY_INIT)

/*int32 tts_play_exit(void* null0, void* null1, void* null2)*/
/*�ر�tts��Դ�ļ�,ж��dsp����ͷ�tts������Դ�Ȳ���*/
#define tts_status_play_exit()         (int)tts_op_entry((void*)(0), (void*)(0), (void*)(0), TTS_PLAY_EXIT)

/*int32 tts_play_frame(void* null0, void* null1, void* null2)*/
/*����ÿһ֡����,�Լ�������������dsp�Ľ�������*/
#define tts_status_play_frame()        (int)tts_op_entry((void*)(0), (void*)(0), (void*)(0), TTS_PLAY_FRAME)

#endif //_TTS_INTERFACE_H
