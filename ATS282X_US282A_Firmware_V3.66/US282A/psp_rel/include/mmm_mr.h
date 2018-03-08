/*! \cond MMM_MR_API */
/*******************************************************************************
 *                              5110
 *                            Module: music
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao     2011-02-01 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mmm_mr.h
 * \brief    ��������¼���м���ṩ�Ľӿڼ�����
 * \author   jpliao
 * \version 1.0
 * \date  2011/02/01
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_MR_H__
#define __MMM_MR_H__

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

/*! \cond MMM_MR_API */
/*!
 * \brief
 *      �����ʽ
 */
typedef enum
{
    /*! PCM */
    PCM,
    /*! ADPCM */
    ADPCM,
    /*! MP3 */
    MP3,
    /*! ACT */
	ACT,
} mmm_mr_encode_mode_t;

/*!
 * \brief
 *      ��ǰcodec��״̬,��mmm_mr_status_t��statusȡֵ
 */
typedef enum
{
    /*! ������ */
    MMM_MR_ENGINE_ENCODING = 0,
    /*! ��ͣ  */
    MMM_MR_ENGINE_PAUSED,
    /*! ������ֹͣ */
    MMM_MR_ENGINE_STOPPED,
    /*! ������� */
    MMM_MR_ENGINE_ERROR,
} mmm_mr_engine_t;
/*!
 * \brief
 *      ����Ŷ���,��mmm_mr_status_t��err_noȡֵ
 */
typedef enum
{
    /*! �ļ��������� */
    MMM_MR_ERR_FILE_OP = -9,
    /*! �����ڵ����� */
    MMM_MR_ERR_CMD_NOTEXIST,
    /*! һ��δ֪���� */
    MMM_MR_ERR_UNEXPECTED,
    /*! �������س��� */
    MMM_MR_ERR_LIBLOAD_ERROR,
    /*! �����˷Ƿ��ı���� */
    MMM_MR_ERR_LIB_ERROR,
    /*! ������� */
    MMM_MR_ERR_ENCODER_ERROR,
    /*! �����ʴ��� */
    MMM_MR_ERR_FS_ERROR,
    /*! �ڴ�ռ䲻�� */
    MMM_MR_ERR_OUTOFMEMORY,
    /*! ��ʽ�������֧�֣����ܱ��� */
    MMM_MR_ERR_UNSUPPORTED,
    /*! ���� */
    MMM_MR_ERR_OK,
    /*! ������ݳ�����������С */
    MMM_MR_ERR_DATAOVERFLOW,

} mmm_mr_err_t;

/*!
 * \brief
 *      ¼���м���ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �� */
    MMM_MR_OPEN = 0,
    /*! �ر� */
    MMM_MR_CLOSE,
    /*! ���������� */
    MMM_MR_AIN_OPEN,
    /*! �ر��������� */
    MMM_MR_AIN_CLOSE,

    /*! ��ʼ¼�� */
    MMM_MR_START,
    /*! ֹͣ¼�� */
    MMM_MR_STOP,
    /*! ��ͣ¼�� */
    MMM_MR_PAUSE,
    /*! ����¼�� */
    MMM_MR_RESUME,
    /*! ����¼���ļ� */
    MMM_MR_SET_FILE,
    /*! �ر�¼���ļ� */
    MMM_MR_CLEAR_FILE,
    /*! ����¼������ */
    MMM_MR_AUDIO_PARAM,
    /*! ��ȡ����״̬ */
    MMM_MR_GET_STATUS,
    /*! ��ȡ����� */
    MMM_MR_GET_ERRORNO,
    /*! ���̲߳��Խӿ� */
    MMM_MR_SINGLE_THREAD,

	/*! ���ý������: 0�������룬1-10Ϊ���뼶��Խ����Խ����*/
	MMM_MR_SET_DENOISE,
	/*! �����Զ�������Ʋ�����0�������Զ�������ƣ�1: ������-6b����*/
	MMM_MR_SET_AGC,

} mmm_mr_cmd_t;


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

/*!
 *  \brief
 *     ���ļ�����Ĳ�����Ϣ
 */
typedef struct
{
    /*! 0: vfs, 1: sd file, 2: file handle*/
    int fs_type;
    /*! need mount id when fs_type == 0*/
    void *file_mount_id;
    /*! need file handle when fs_type == 2, need file name when fs_type == 1*/
    void *file_handle;
} mmm_mr_fs_para_t;

/*!
 *  \brief
 *     ��Ƶ����Ĳ�������
 *
 * \li     wavֻ��������ʡ������ʼ��㹫ʽΪ��ADPCM: ������*4*2��PCM��������*16*2
 * \li     mp3¼����������һ����
 * \par
 <div class="generic_text_mapping_table">
 * <table>
 * <tr>
 * <th>����</th>
 * <th>��ѡͨ����</th>
 * <th>��ѡ������</th>
 * <th>��ѡ������</th>
 * </tr>
 * <tr>
 * <td>��</td>
 * <td>1</td>
 * <td>8k,11025,12k,16k</td>
 * <td>[8,64]</td>
 * </tr>
 * <tr>
 * <td>��</td>
 * <td>1,2</td>
 * <td>22050,24k</td>
 * <td>[64,128]</td>
 * </tr>
 * <tr>
 * <td>��</td>
 * <td>1,2</td>
 * <td>32k,44100,48k</td>
 * <td>[64,192]</td>
 * </tr>
 * </table>
 * </div>
 */
typedef struct
{
    /*! ��Ƶ�����ģʽ, 0: pcm-wav, 1: adpcm-wav, 2: mp3*/
    int encode_mode;
    /*! ����ı����ʣ���64000,128000��*/
    int bitrate;
    /*! ����Ƶ����8000��16000��44100��*/
    int sample_rate;
    /*! ��Ƶ������ͨ������:1 ��������2 ˫����*/
    int channel;
    /*! �������ȣ�Ĭ��16����δʹ�� */
    int bps;
    /*! ����ģʽ��0:����¼��, 1:����ģʽ, 2:�Զ����� */
    int vad_mode;
    /*! �����������ֵ[0,128],����ȷ������ֵ��Ϊ���ʾ���ó�ʼ¼��������ƽ��ֵ��������-0.5dBһ��, Ĭ��ֵ 64 */
    int vad_threshold;
    /*! ��������ӳ�ʱ��,��λms, Ĭ��ֵ 3000 */
    int vad_delay;
    /*! ���뼶�����: 0�������룬1-10Ϊ���뼶��Խ����Խ���� */
    int denoise_level;
} mmm_mr_audio_param_t;
/*!
 * \brief
 *      ¼���м�����ظ�Ӧ�ò��״̬��Ϣ
 */
typedef struct
{
    /*! ��ǰ��¼��ʱ��*/
    int time;
    /*! ��ǰcodec��״̬*/
    int status;
    /*! �����ǰʵʱ��������,��ʱ��֧��*/
    int energy_level;
    /*! ��ǰ�Ƿ��Ǿ������ھ�����⹦�ܿ���ʱ��Ч, �㣺���������㣺����*/
    int silence_flag;
    /*! �ڳ���״̬ʱ���صĴ����*/
    int err_no;
    /*! ��ǰ¼��д���ļ��е�����������λ��512�ֽڼ�1��page*/
    int write_pages;
} mmm_mr_status_t;
/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h
 */
extern int mmm_mr_cmd(void *, mmm_mr_cmd_t, unsigned int) __FAR__;

/*! \endcond */
#ifdef __cplusplus
}
#endif
#endif // __MMM_MR_H__
