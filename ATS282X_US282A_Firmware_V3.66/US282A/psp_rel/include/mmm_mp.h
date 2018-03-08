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
#ifndef __MMM_MP_H__
#define __MMM_MP_H__

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

/*! \cond MMM_MP_API */

/*! �����Ч�������� */
#define MAX_EQ_SEG                  12
/*! vps�������÷�ΧΪ[-vps_max_level��vps_max_level]������Ϊ���ţ�����Ϊ��ţ�0��ʾvps�ر�*/
#define VPS_MAX_LEVEL               8

/*!
 *  \brief
 *      ��������
 */
typedef enum
{
   UNSUPPORT_TYPE = 0,
   MP3_TYPE,
   WMA_TYPE,
   WAV_TYPE,
   FLAC_TYPE,
   APE_TYPE,
   SBC_TYPE,
   OGG_TYPE,
   AAX_TYPE,
   AUD_TYPE,   
   AAC_TYPE,         
   AIN_TYPE,
   ACT_TYPE,

}music_type_t;

/*!
 *  \brief
 *  ���ֲ��Ŵ���ţ��ڵ��ýӿڷ��ش�������м�������棩״̬Ϊ����ʱ��Ч
 */
typedef enum
{
    /*! ���ļ�ʧ�� */
    MMM_MP_ERR_OPEN_FILE = 0x10001,
    /*! �ļ���ʽ��֧�� */
    MMM_MP_ERR_FILE_NOT_SUPPORT = 0x10002,
    /*! �ļ�����*/
    MMM_MP_ERR_FILE_DAMAGED = 0x10003,
    /*! ������� */
    MMM_MP_ERR_DECODER_ERROR = 0x10004,
    /*! DRM��license */
    MMM_MP_ERR_NO_LICENSE = 0x10005,
    /*! DRMʱ�Ӵ��� */
    MMM_MP_ERR_SECURE_CLOCK = 0x10006,
    /*! DRMʱ�Ӵ��� */
    MMM_MP_ERR_LICENSE_INFO = 0x10007,
    /*! ��Ч���� */
    MMM_MP_ERR_CMD_ILLEGAL = 0x10008,
    /*! �����ֲ����� */
    MMM_MP_ERR_CMD_NOTEXIST = 0x10009,
    /*! seek����ʱ��������ʱ��֮�� */
    MMM_MP_ERR_SEEK_BEHIND = 0x1000A,
    /*! parser���ش��� */
    MMM_MP_ERR_PARSER_LOAD = 0x1000B,
    /*! decoder���ش��� */
    MMM_MP_ERR_DECODER_LOAD = 0x1000C,
    /*! ����fifo���� */
    MMM_MP_ERR_FIFO_CREATE = 0x1000D,
    /*! ����ʱ�䳬����ʱ�� */
    MMM_MP_ERR_TIME_OVERFLOW = 0x1000E,
    /*! ����Դ������豸��ƥ�� */
    MMM_MP_ERR_IO_MISMATCH = 0x1000F,
    /*! ������ļ�β��ֹͣ */
    MMM_MP_ERR_FF_END = 0x10010,
    /*! ������ļ�ͷ��ֹͣ */
    MMM_MP_ERR_FB_START = 0x10011,
    /*! parser���� */
    MMM_MP_ERR_PARSER_ERROR = 0x10012,
    /*! ����豸֧�ֳ��� */
    MMM_MP_ERR_DEVICE_ERROR = 0x10013,
    /*! �����ʲ�֧�� */
    MMM_MP_SAMPLE_RATE_NOT_SUPPORT = 0x10014,
    /*! δ֪���� */
    MMM_MP_ERR_UNKNOWN = 0x100FF
} mmm_mp_err_t;
/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
    typedef enum
    {
        /*! �ֿ鴦�������Ч�� */
    
        /*! �������м�� */
        MMM_MP_OPEN = 0,
        /*! �ر������м�� */
        MMM_MP_CLOSE,
        /*! ��������� */
        MMM_MP_AOUT_OPEN,
        /*! �ر�������� */
        MMM_MP_AOUT_CLOSE,
    
        /*! �������ֲ���������Ϣ */
        MMM_MP_SET_MUSIC_SETTING,
        /*! ��ȡ���ֲ���������Ϣ */
        MMM_MP_GET_MUSIC_SETTING,
        /*! ������������׵�ַ */
        MMM_MP_VOLUME_TBL_ADDR,
        /*! ���õ���ʱ�� */
        MMM_MP_FADE_IN_TIME,
        /*! ���õ���ʱ�� */
        MMM_MP_FADE_OUT_TIME,
        /*! ���ÿ���˲��� */
        MMM_MP_FFB_STEP,
        /*! ���ÿ���˲���ʱ�� */
        MMM_MP_FFB_PLAY_TIME,
        /*! ���ÿ���˼�� */
        MMM_MP_FFB_WAIT_TIME,
        /*! ����AB���Ŵ��� */
        MMM_MP_AB_COUNT,
        /*! ����B��ȴ�ʱ�� */
        MMM_MP_AB_WAIT_TIME,
        /*! ���öϾ���������ż� */
        MMM_MP_AB_ENERGY_LIMIT,
        /*! ���öϾ����ʱ���ż� */
        MMM_MP_AB_TIME_LIMIT,
    
        /*! ���ò����ļ� */
        MMM_MP_SET_FILE,
        /*! ��������ļ� */
        MMM_MP_CLEAR_FILE,
        /*! ��ȡý����Ϣ */
        MMM_MP_MEDIA_INFO,
        /*! ��ȡDRM��Ϣ */
        MMM_MP_GET_DRM_INFO,
        /*! ���öϵ㲥����Ϣ */
        MMM_MP_SET_BREAK_POINT,
        /*! ��ȡ�ϵ㲥����Ϣ */
        MMM_MP_GET_BREAK_POINT,
        /*! ����AB������Ϣ */
        MMM_MP_SET_AB_INFO,
        /*! ��ȡAB������Ϣ */
        MMM_MP_GET_AB_INFO,
        /*! ���� */
        MMM_MP_PLAY,
        /*! ֹͣ */
        MMM_MP_STOP,
        /*! ��ȡ�������� */
        MMM_MP_GET_PERFORMANCE,
    
        /*! ��ȡ����״̬ */
        MMM_MP_GET_STATUS,
        /*! ��ȡ������Ϣ�� */
        MMM_MP_GET_ERRORNO,
    
        /*! �������� */
        MMM_MP_SET_VOLUME,
        /*! ������Ч */
        MMM_MP_SET_EQ,
        /*! ���ÿ���� */
        MMM_MP_SET_FFB,
        /*! ����A�� */
        MMM_MP_SET_A,
        /*! ����B�� */
        MMM_MP_SET_B,
        /*! ȡ��AB���� */
        MMM_MP_CLEAR_AB,
        /*! �����½ڲ��� */
        MMM_MP_SET_CHAPTER,
    
        /* Ԥ����δ��ʵ�� */
        /*! ��λ */
        MMM_MP_SEEK,
        /*! ����AOUT SETTING */
        MMM_MP_SET_AOUT,
        /*! ���̲߳��� */
        MMM_MP_SINGLE_THREAD,
        
        /*! ���ûص����� */
        MMM_MP_SET_CALLBACK,    
    
        /*! ��ȡ������������Ϣ��ͨ����ȡ��ַ��Ȼ��Ϳ���ֱ�ӿ��ٷ��� */
        MMM_MP_GET_ENERGY_INFO,
    
        /*! ���������������������������������������ϵ� */
        MMM_MP_SET_OUTPUT_CHANNEL,
        /*����SD�����ݲ����ӿ�*/
        MMM_MP_SET_SDD_FILE_IO,
        /*���ò����ʹ̶�*/
        MMM_MP_FIX_SAMPLE_RATE,
        
        /*�Ƿ�֧�������ļ�����*/	
        MMM_MP_DECRYPT_STATUS = 0x90,
        /* ���ܺ���ָ�� */	
        MMM_MP_SET_DECRYPT_FUNC,
    } mmm_mp_cmd_t;

/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_MP_ENGINE_STOPPED = 0,
    /*! ����״̬ */
    MMM_MP_ENGINE_PLAYING,
    /*! ��ͣ״̬ */
    MMM_MP_ENGINE_PAUSED,
    /*! ����״̬ */
    MMM_MP_ENGINE_ERROR
} mmm_mp_engine_t;
/*!
 *  \brief
 *      DRM����
 */
typedef enum
{
    /*! ����DRM�ļ� */
    MMM_NO_DRM = 0,
    /*! ACTIONS��DRM��Ƶ�ļ� */
    MMM_ACT_DRM,
    /*! ΢���DRM��Ƶ�ļ� */
    MMM_MS_DRM,
    /*! �������͵�DRM�ļ� */
    MMM_DRM_OTHER,
    /*! DRM�ļ���ȱLicense */
    MMM_DRM_NO_LICENCE
} mmm_drm_type_t;
/*!
 * \brief
 *      ����EQƵ����
 */
enum
{
    /*! 5��Ƶ�� */
    EQ_BAND5 = 5,
    /*! 7��Ƶ�� */
    EQ_BAND7 = 7,
};
/*!
 *  \brief
 *     EQģʽ
 */
typedef enum
{
    /*! ��ͨģʽ*/
    EQ_NORMAL = 0,
    /*! EQ�û�ģʽ*/
    EQ_EQ_USR_MODE = 7,
    /*! ��Ч�Զ���ģʽ������*/
    EQ_TYPE_USR_MODE = 0x4f,
    /*! �����û�ģʽ*/
    EQ_VPS_USR_MODE = 0x77,
    /*! ����O K �û�ģʽ*/
    EQ_KARAOKE_USR_MODE = 0x88,
    /*! FULLSOUNDģʽ*/
    EQ_FULLSOUND_MODE = 0x99,    
} mmm_mp_eqmode_t;
/*!
 *  \brief
 *      AB����״̬
 */
typedef enum
{
    /*! AB���ű���� */
    AB_PLAY_CLEAR = 0,
    /*! ��ǰ����B�㣬�����ȴ���״̬ */
    AB_PLAY_B,
    /*! AB����״̬�� */
    AB_PLAY_ING
} mmm_mp_ab_t;

typedef enum _FILE_SOURCE_TYPE
{
    FILE_SOURCE_NORMAL = 0,//��ͨ�ļ�
    FILE_SOURCE_SD,//SD���ļ�
    FILE_SOURCE_LINEIN_AA,//ռλ��
    FILE_SOURCE_LINEIN_ADDA,//LINEIN AD-DAͨ��
    FILE_SOURCE_BT_SBC,//����sbc����
} FILE_SOURCE_TYPE;

/*!
 *  \brief
 *     ���ļ�����Ĳ�����Ϣ
 */
typedef struct
{
    /*! 0: vfs, 1: sd file system 2: line in aa mode 3: line in ad da mode 4: sbc input mode */
    int fs_type;
    /*! vfs mount id or sbc read function handle */
    void *file_mount_id;
    /*! file name */
    char *file_name;
} mmm_mp_fs_para_t;
/*!
 *  \brief
 *     �����ļ���Ϣ
 */
typedef struct
{
    /*! ������ʱ��,�Ժ���Ϊ��λ*/
    int total_time;
    /*! ���ֱ����ʣ���KbpsΪ��λ*/
    int bitrate;
    /*! ���ֲ����ʣ���KHzΪ��λ*/
    int sample_rate;
//    /*! ����������*/
//    int channels;
    /*! ���ָ�ʽ����*/
    int music_type;    
    /*! drm���ͣ����mmm_drm_type_t */
    int drm_type;
} mmm_mp_file_info_t;
/*!
 *  \brief
 *     DRM������Ϣ
 */
typedef struct
{
    short year;
    char month;
    char day;
} drm_date_t;
/*!
 *  \brief
 *     DRMʱ����Ϣ
 */
typedef struct
{
    short hour;
    char minute;
    char second;
} drm_time_t;
/*!
 *  \brief
 *     DRM��Ϣ
 */
typedef struct
{
    /*! ��ʼ�������� */
    drm_date_t BeginDate;
    /*! ��ʼ����ʱ�� */
    drm_time_t BeginTime;

    /*! ��ֹ�������� */
    drm_date_t ExpDate;
    /*! ��ֹ�������� */
    drm_time_t ExpTime;

    /*! ��һ���ù�֮�����õ�Сʱ������ͬһ�׸��ǹ̶�ֵ */
    int RemainTime;
    /*! ���¿ɴ洢��ʱ��(����), ���ڱ����˽ӿڣ�����ʱ���� */
    int RemainStoreTime;
    /*! 0: ʱ�ӻص�ʱɾ�� 1��ʱ�ӻص�ʧЧ */
    int DealOnClkBck;

    /*! �������ϸ���(Ϊ1��λ��ʾ��Ӧ��򿪣�Ϊ0ʱ��ʾ�ر�) */
    int switchflag;
} mmm_mp_drm_info_t;
/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_mp_engine_t */
    int status;
    /*! ����Ĵ���ţ����mmm_mp_err_t */
    int err_no;
    /*! �Զ�AB����ʱ��״̬�� ���mmm_mp_ab_t*/
    int ab_play_flag;
    /*! ��ǰ�Ĳ���ʱ�䣬��λ���� */
    int cur_time;
    /*! ��ǰ�����ʣ���λkbps */
    int cur_bitrate;
    /*! ��ǰ�������� */
    int cur_energy;
    /*! Todo spectrum*/
} mmm_mp_status_t;
/*!
 *  \brief
 *     ���ֲ�������
 */
typedef struct
{
    /*! parser�ĵ�ʱ�� */
    int parser_elapsed;
    /*! decoder�ĵ�ʱ�� */
    int dec_elapsed;
    /*! ��ȡ�ļ��ĵ�ʱ�� */
    int io_elapsed;
    /*! ������ģ���format_check */
    int other_elapsed;
} mmm_mp_performance_t;
/*!
 *  \brief
 *     ��Ч������Ϣ
 */
typedef struct
{
    /*! eq�����ͣ����mmm_mp_eqmode_t*/
    int eq_type;
    /*! eq�ľ������ */
    char eq_para[MAX_EQ_SEG];
} mmm_mp_eq_info_t;
/*!
 *  \brief
 *     ���ֲ��Ŷϵ���Ϣ
 *     ��music_bp_info_tһ��
 */
typedef struct
{
    /*! �ϵ�λ�õ�ʱ�䣬��λms */
    int bp_time_offset;
    /*! �ϵ�λ�����ļ��е�λ�ã���λ�ֽ� */
    int bp_file_offset;
    /*! �ϵ㲥������Ҫ�ĸ�����Ϣ*/
    /*! �����½ں���������Audible  */
    int bp_info_ext;
} mmm_mp_bp_info_t;
/*!
 *  \brief
 *     ���ֲ���AB����Ϣ
 */
typedef struct
{
    /*! ��ǰab���Ż�ʣ��Ĵ��� */
    int ab_play_count_left;
    /*! ��ǰ����B����Ƿ���Ҫ��ʱ */
    int ab_play_delay_toggle;
    /*! ��ǰab����������״̬�����mmm_mp_ab_t*/
    int ab_play_flag;
    /*! a�㸽����ʱ�������λ���� */
    int a_point_time_offset;
    /*! a�㸽�����ļ�λ�ã���λ�ֽ� */
    int a_point_file_offset;
    /*! a�㸽��������Ϣ */
    int a_point_info_ext;
    /*! b�㸽����ʱ�������λ���� */
    int b_point_time_offset;
} mmm_mp_ab_info_t;
/*!
 *  \brief
 *     ���������������Ϣ
 */
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
} mmm_mp_aout_setting_t;
/*!
 *  \brief
 *     ���ֲ��ŵ�������Ϣ
 */
typedef struct
{
    /*! ����ֵ����������̶����������ṩ���õĽӿ�*/

    /*! ϵͳĬ�ϲ����������и������ã������ýӿڣ�ԭ����ֻ����һ��*/
    mmm_mp_aout_setting_t aout_setting;
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

    /*! ����� */
    /*! ����˲�������λ���� */
    int ffb_step;
    /*! �������ʱ����λ���� */
    int ffb_delay;
    /*! ����˲���ʱ�䣬��λ���� */
    int ffb_play_time;

    /*! AB���� */
    /*! AB ���Ŵ��� */
    int ab_play_count;
    /*! ����B������ʱʱ�䣬��λ10ms */
    int ab_play_delay;

    /*! �Զ����� */
    /*! �Ͼ���ʱ���ż�����λ���� */
    int sent_dec_energy_limit;
    /*! �Ͼ��������ż�����λ���� */
    int sent_dec_time_limit;

    /*! �û��ɵ��ڲ�������ǰ�趨��������һ�׸裬�����ýӿڣ��������ö��*/
    /*! �������� */
    int volume_level;
    /*! ��Ч���ã��μ�mmm_mp_eq_info_t*/
    mmm_mp_eq_info_t eq_setting;

    /*! Ԥ����δ��ʵ��  */
    /*! ��ʼ����λ�ã���λ�ֽ�*/
    int start_pos;
    /*! ��������λ�ã���λ�ֽ�*/
    int end_pos;
} music_setting_t;

/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h
 */
extern int mmm_mp_cmd(void *, mmm_mp_cmd_t, unsigned int) __FAR__;

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
