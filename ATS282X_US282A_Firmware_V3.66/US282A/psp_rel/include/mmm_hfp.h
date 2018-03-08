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
/*!
 * \file     mmm_mp.h
 * \brief    �������ֲ����м��ṩ�Ľӿڼ�����
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_HFP_H__
#define __MMM_HFP_H__

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

/*! �����Ч�������� */
#define MAX_EQ_SEG                  12

typedef enum
{
    /*! ���ļ�ʧ�� */
    MMM_HFP_ERR_OPEN_FILE = 0x10001,
    MMM_HFP_ERR_DECODER_ERROR,
    MMM_HFP_ERR_DECODER_LOAD,
    MMM_HFP_ERR_DECODER,
} mmm_hfp_err_t;
/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �ֿ鴦�������Ч�� */

    /*! �������м�� */
    MMM_HFP_OPEN = 0,
     /*! ��������� */
    MMM_HFP_AOUT_OPEN,
     /*! ��mic���� */
    MMM_HFP_AIN_OPEN,

    /*��ʼ����*/
    MMM_HFP_PLAY,
     /*��ȡ״̬*/
    MMM_HFP_GET_STATUS,
    /*ֹͣ����*/
    MMM_HFP_STOP_PLAY,
     /*! �ر�mic���� */
    MMM_HFP_AIN_CLOSE,
    /*! �ر�������� */
    MMM_HFP_AOUT_CLOSE,
    /*! �ر������м�� */
    MMM_HFP_CLOSE,
    /*! �̶������� */
    MMM_HFP_FIX_SAMPLE_RATE,
   
    
    /*���̲߳���*/
    MMM_HFP_SINGLE_THREAD,
} mmm_hfp_cmd_t;

typedef struct
{
    /*! eq�����ͣ����mmm_mp_eqmode_t*/
    int eq_type;
    /*! eq�ľ������ */
    char eq_para[MAX_EQ_SEG];
} mmm_hfp_eq_info_t;

typedef struct
{
    /*! ��������ģʽ��0: ��ֱ����1:ֱ����Ĭ��ֵ0*/
    int drive_mode;
    /*! Ĭ�ϲ����ʣ���λ��KHz��Ĭ��ֵ44 */
    int default_samplerate;
    /*! Ĭ��ͨ������Ĭ��ֵ2 */
    int default_channels;
    /*! Ĭ������ ��λ������Ĭ��ֵ24 */
    int default_volume;
} mmm_hfp_aout_setting_t;

/*!
 *  \brief
 *     ���������������Ϣ
 */
typedef struct
{
    /*! ������Դ ���μ� mmm_ai_type_t*/
    int input_source;
    /*! �Ƿ������0: ������, 1:���� */
    int listening_flag;
    /*! �����ʽ�� 0:��ֱ����1:ֱ�� */
    int drive_mode;
    /*! ������Դ���漶��0-7,�Ƽ�ֵ��FM/LINE IN 2, MIC IN 7; step 1.5db*/
    int input_gain;
    /*! ADC���漶��0-0x3f,�Ƽ�ֵ��FM/LINE IN 0x2f, MIC IN 0x33; step 1.5db*/
    int adc_gain;
    /*! �������漶��0,1,2, �㣺disable������: enable��step 6db */
    int digital_gain;
} mmm_mr_ain_setting_t;

typedef struct
{
    /*! ����ֵ����������̶����������ṩ���õĽӿ�*/

    /*! ϵͳĬ�ϲ����������и������ã������ýӿڣ�ԭ����ֻ����һ��*/
    mmm_hfp_aout_setting_t aout_setting;
    
    /*!������������*/
    mmm_mr_ain_setting_t ain_setting;
    
    /*! ����ͨ��ѡ�� */
    /*! ����ͨ��ģʽ��0��������1��ֻ�����������2��ֻ�����������3�������������� */
    int chanel_select_mode;
    /*! ���������ʼ��ַ */
    /*! ������񣬰���Ӳ���������������� */
    void *vloume_tbl_addr;

    /*! ���뵭�� */
    /*! ����ʱ�䣬��λ���� */
    int fade_in_time;
    /*! ����ʱ�䣬��λ���� */
    int fade_out_time;

  
    /*! �û��ɵ��ڲ�������ǰ�趨��������һ�׸裬�����ýӿڣ��������ö��*/
    /*! �������� */
    int volume_level;
    /*! ��Ч���ã��μ�mmm_mp_eq_info_t*/
    mmm_hfp_eq_info_t eq_setting;

          
} hfp_music_setting_t;

typedef struct
{
    /*! 0: vfs, 1: sd file, 2: file handle*/
    int fs_type;
    /*! need mount id when fs_type == 0*/
    void *file_mount_id;
    /*! need file handle when fs_type == 2, need file name when fs_type == 1*/
    void *file_handle;
} mmm_mfp_fs_para_t;

extern int mmm_hfp_cmd(void *, mmm_hfp_cmd_t, unsigned int) __FAR__;

/*! \endcond */
#ifdef __cplusplus
}
#endif
#endif 
