/********************************************************************************
 *                            Module: radio_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-08 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __FMENGINE_H__
#define __FMENGINE_H__

#include "psp_includes.h"
#include "fm_interface.h"
#include "mmm_pp.h"

typedef enum
{
    STATUS_PLAY = 0, // ��������״̬
    STATUS_PAUSE, //������ͣ״̬(����״̬)
    STATUS_FMENGINE_ERROR, //Ӳ������
} FM_Playstatus_e;

typedef enum
{
    FM_MONO = 0, //[1]������
    STERE, //[0]������
} FM_Audio_e;

/*Ӳ��seek ״̬ö��*/
typedef enum
{
    HARDSEEK_NOT_START = 0, //δ����Ӳ����̨���ߴ���Ӳ����̨δ�ѵ���Ч̨
    HARDSEEK_DOING, //Ӳ����̨������
    HARDSEEK_COMPLETE, //����Ӳ����̨���̽������ѵ���Ч̨
    HARDSEEK_COMPLETE_NOT_VALID, //����Ӳ����̨���̽�����û���ѵ���Ч̨
    HARDSEEK_FALSE = 4, //������Ӳ����̨��for the module have no hard seek��
} hardseek_status_e;

typedef struct
{
    UINT32 FM_CurrentFreq; //��ǰƵ��
    UINT16 FM_SignalStg; //��ǰƵ���ź�ǿ��
    FM_Audio_e FM_Stereo_Status; //0������������0��������
    radio_band_e FM_CurBand; //��ǰ����
    FM_Playstatus_e FM_VolStatus; //��ǰ����״̬
    hardseek_status_e STC_flag; //Ӳ��seek �Ƿ���ɱ�־����Ҫ����Ӳ��seek ��Ҫ
} Engine_Status_t;

typedef enum
{
    PA_MODE = 0, //��ֱ��
    PA_DDV_MODE, //ֱ��
} PA_DDV_Mode_e;

/* FM ����������Ϣ�ṹ��*/
typedef struct
{
    uint16 magic;
    uint8 fm_threshold; //ģ��Ĭ����̨����ֵ
    /*! wav/mp3/wma��Ч���� */
    mmm_pp_eq_info_t eq_info;
} FMEngine_cfg_t;

/* FM ģ��״̬ö��*/
typedef enum
{
    MODULE_NO_INIT = 0, //ģ�鴦��δ��ʼ��״̬
    MODULE_WORK, //ģ���ʼ����ɣ�������������״̬
    MODULE_STANDBY, //ģ�鴦��Standby ״̬
} Module_Status_e;

//��̨����
typedef struct
{
    uint32 freq;
    uint8 dir;
} seek_param_t;

//�������ò���
typedef struct
{
    uint32 freq;
    uint8 band;
    uint8 threshold;
} reinit_param_t;
#endif //__FMENGINE_H__
