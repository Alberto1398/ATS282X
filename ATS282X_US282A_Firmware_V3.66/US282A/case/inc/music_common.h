/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-08 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __MUSIC_COMMON_H__
#define __MUSIC_COMMON_H__

#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_mp.h"

//#define AB_SUPPORT

//�ڼ����Ӷϵ㲥��֮ǰ���������ļ����ڵ�Ŀ¼�������Ƿ���ˣ�������ˣ�����ϵ���Ϣ
#define CHECK_BK_DIR

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
    /*! ֹͣ */
    StopSta = 0,
    /*! ��ͣ */
    PauseSta,
    /*! ���� */
    PlaySta,
    /*! AB���� */
    PlayAB,
    /*! ����� */
    PlayFast
} play_status_e;

/*!
 * \brief
 *  AB����״̬
 */
typedef enum
{
    /*! ��AB����״̬ */
    PlayAB_Null,
    /*! �ȴ�����A�� */
    PlayAB_A,
    /*! �ȴ�����B�� */
    PlayAB_B,
    /*! AB�������״̬ */
    PlayAB_Do
} ab_status_e;

/*!
 * \brief
 *  �����״̬
 */
typedef enum
{
    /*! �������� */
    FBPlay_Null,
    /*! ���״̬ */
    FFPlaySta,
    /*! ����״̬ */
    FBPlaySta
} fast_status_e;

//���ֲ���ģʽ
typedef enum
{
    //��������
    PLAY_NORMAL,
    //����
    PLAY_RESUME,
    //�������
    PLAY_FFON,
    //���˲���
    PLAY_FBON,
    //����Ҫ����
    PLAY_NO_PLAY
} play_mode_e;

/*!
 * \brief
 *  ������Ϣ
 */
typedef enum
{
    /*! û�д��� */
    EG_ERR_NONE = 0,
    /*�������ļ�*/
    EG_ERR_NO_FILE,
    /*���ʰγ�*/
    EG_ERR_DISK_OUT,
    /*! ���ļ�ʧ�� */
    EG_ERR_OPEN_FILE,
    /*! �ļ���ʽ��֧�� */
    EG_ERR_NOT_SUPPORT,
    /*! ������� */
    EG_ERR_DECODER_ERROR,
    /*! ��license�����Ŵ����������ǲ���ʱ���Ѿ�����  */
    EG_ERR_NO_LICENSE,
    /*! DRMʱ�Ӵ���  */
    EG_ERR_SECURE_CLOCK,
    /*! DRM��Ϣ����  */
    EG_ERR_DRM_INFO,
    /*! ������ʱ  */
    EG_ERR_RAED_CARD_TIMEOUT
} eg_err_e;

/*! �����ʼ��״̬�� */
typedef enum
{
    /*! ���ڳ�ʼ�������˶�ȡ״̬�����ܶ���������κβ��� */
    EG_CTL_INITING,
    /*! �Ѿ���ʼ����� */
    EG_CTL_NORMAL,
    EG_CTL_MAX
} eg_ctl_e;

/*!
 * \brief
 *  ���ŵ��ļ�״̬
 */
typedef enum
{
    /*! �Ӷϵ㿪ʼ */
    FIRST_PLAY_FROM_BK,
    /*!��ͷ��ʼ */
    FIRST_PLAY_FROM_START,
    /*!illegal */
    PLAY_FILE_MAX
} first_play_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */
    play_status_e play_status;
    /*! AB����״̬  */
    ab_status_e ab_status;
    /*! �����״̬  */
    fast_status_e fast_status;
    /*! ����״̬,��ȡ�����  */
    eg_err_e err_status;
    /*!��ǰ����״̬*/
    eg_ctl_e ctl_status;
    /*!��һ�β��ŵ��ļ�״̬*/
    first_play_e first_file_status;
} mengine_status_t;

/*!
 * \brief
 *  ��ǰ�����ļ���Ϣ
 */
typedef struct
{
    /*! ��ǰʱ�� */
    uint32 cur_time;
    /*! ������ */
    uint32 cur_bitrate;
    /*! ��ʴ��ڱ�־ */
    uint8 cur_lyric;
    /*! �ļ��л���־ bit0: �����Ƿ��л� bit1:audible�½��Ƿ��л�  */
    uint8 cur_file_switch;
    /*! �����ֽڣ�4�ֽڶ��� */
    uint8 reserve[2];
} mengine_playinfo_t;

/*!
 * \brief
 *  ������Ϣ�ṹ���������
 */
typedef struct
{
    /******************************************************************
     *          ����״̬��Ϣ
     *******************************************************************/
    mengine_status_t eg_status;

    /******************************************************************
     *          �����ļ���ʱ���bit����Ϣ
     *******************************************************************/
    mengine_playinfo_t eg_playinfo;
} mengine_info_t;

/*!
 * \brief
 *  music�ļ���������ż���
 */
typedef struct
{
    /*! ��ǰ�ļ���� */
    uint16 num;
    /*! ��ǰ�ļ����� */
    uint16 total;
} musfile_cnt_t;

/*!
 * \brief
 *  ����music�������ģʽ
 */
//��Ϊ����Щ����£�����DISK_H�����һ���ֽ�
//Ҫע�⣺�������һ���ֽں󣬻��ܷ�ֿ�������
//���Ա����ͱ�����ֵ�Ƕ��٣����н����ģ��������������ӡ�
typedef enum
{
    /*! ѡ��������� */
    ENTER_MUSIC_START = 0, ENTER_RECOD_PLAY,
    /*! alarm��������  */
    ENTER_ALARM,
} mengine_enter_mode_e;

//bit5 card; bit6 uhost��
typedef enum
{
    /*! ѡ��������� */
    ENTER_MUSIC_DISK_C = (0 << 5),

    ENTER_MUSIC_DISK_H = (1 << 5),

    ENTER_MUSIC_DISK_U = (1 << 6),

} mengine_enter_disk_e;

//bit7 used for if need bk
#define MUSIC_NEED_BK (1<<7)

/*!
 * \brief
 *  �������ñ����ṹ�嶨��
 */
typedef enum _shuffle_e
{
    SHUFFLE_NONE = 0,
    SHUFFLE = (1 << 4),
} shuffle_e;

#define FILE_NAME_LEN   (32)

typedef struct
{
    /*! ħ�� */
    uint16 magic;
    /*! ��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ*/
    musfile_location_u location;
    /*! �ϵ���Ϣ*/
    mmm_mp_bp_info_t bk_infor;
    //for file name
    uint8 name_buf[FILE_NAME_LEN];
#ifdef CHECK_BK_DIR
    //�ļ�����Ŀ¼������
    uint8 dir_name[32];
#endif
} mengine_config_t;

typedef struct
{
    /*! �ļ��������� */
    uint8 *fname_buffer;
    /*! �ļ�����󳤶� */
    uint8 fname_length;
} music_file_name_t;

//����ɨ��ʱ����Ѹ����������ļ���Ŀ¼����¼��vram
//��࣬��¼��������(ÿ����������Ϊ512�ֽ�)
#define DIR_INFO_SECTOR_SIZE    (512)
#define DIR_INFO_SECTOR_COUNT   (8)

#endif //__MUSIC_COMMON_H__
