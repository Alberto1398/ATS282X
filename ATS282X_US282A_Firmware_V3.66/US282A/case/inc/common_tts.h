/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TTS ������صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _common_tts_H_
#define _common_tts_H_

#include "psp_includes.h"
#include "case_independent.h"
#include "applib.h"
#include "setting_common.h"
#include "tts_interface.h"
#include "common_rcp.h"
#include "config_id.h"
#include "common_btmanager.h"
#include "app_view.h"
#include "common_tts_id.h"

#define TTS_LANGUAGE_TOTAL     2   //֧��TTS������������

#define TTS_MODE_TYPE_MASK          (3 << 0)  //TTS����ѡ��
#define TTS_MODE_ONLYONE            (0 << 0)  //��1������
#define TTS_MODE_SECLIST            (1 << 0)  //�����У���0xffΪ������־
#define TTS_MODE_STRING             (2 << 0)  //�ַ���

#define TTS_MODE_MIN_VOLUME_MASK    (1 << 5)  //�Ƿ����������������������ĳ������������TTS
#define TTS_MODE_MIN_VOLUME_YES     (0 << 5)  //�����������
#define TTS_MODE_MIN_VOLUME_NO      (1 << 5)  //��������������������ǰ����Ϊ0����ô�����Ქ��TTS

#define TTS_MODE_NORECV_ALL_EV_MASK (1 << 6)  //�����պʹ��������¼�ѡ�����ػ�ʱTTS
#define TTS_MODE_NORECV_ALL_EV_NO   (0 << 6)  //���պʹ��������¼�
#define TTS_MODE_NORECV_ALL_EV_YES  (1 << 6)  //�����պʹ��������¼�������TTS

#define TTS_MODE_PB_OPTION_MASK     (1 << 7)  //�绰������ѡ��
#define TTS_MODE_PB_NOPLAY          (0 << 7)  //�ǵ绰������
#define TTS_MODE_PB_PLAY            (1 << 7)  //�绰������

#define TTS_MODE_RCP_OPTION_MASK    (1 << 8)  //TTS��RCPѡ��
#define TTS_MODE_RCP_BREAK          (0 << 8)  //��ֹTTS����Ӧ����Щ�������TTS�в�����Ӧ
#define TTS_MODE_RCP_NORECV         (1 << 8)  //����ȡRCP����

#define TTS_MODE_SYS_OPTION_MASK    (1 << 9)  //TTS��ϵͳ��Ϣѡ��
#define TTS_MODE_SYS_BREAK          (0 << 9)  //��ֹTTS����Ӧ
#define TTS_MODE_SYS_NORECV         (1 << 9)  //����ȡϵͳ��Ϣ

#define TTS_MODE_KEY_OPTION_MASK    (3 << 10) //TTS�а���ѡ��
#define TTS_MODE_KEY_BREAK_REDEAL   (0 << 10) //��ֹTTS����Ӧ
#define TTS_MODE_KEY_BREAK_FILTER   (1 << 10) //��ֹTTS�ҹ���
#define TTS_MODE_KEY_FILTER         (2 << 10) //ֱ�ӹ���
#define TTS_MODE_KEY_NORECV         (3 << 10) //����ȡ������Ϣ

#define TTS_MODE_BLOCK_OPTION_MASK  (1 << 12) //����ѡ��
#define TTS_MODE_BLOCK              (0 << 12) //����ģʽ    ����ѡ����Ч
#define TTS_MODE_NOBLOCK            (1 << 12) //������ģʽ  ����ѡ����Ч�����û��Լ���֤�����н����ͻ���⣻
//������ģʽ��������ʹ�ã�������ڿ���ʱʹ�ã��ÿ���TTS�뿪�����ò�����

#define TTS_MODE_FIFO_OPTION_MASK   (1 << 15) //�ȷŵ�FIFO���ٲ��ţ��򻯴����߼�������֧�ֶ��TTS����
#define TTS_MODE_NOFIFO             (0 << 15) //��ʹ��FIFO����������
#define TTS_MODE_USEFIFO            (1 << 15) //ʹ��FIFO�ӻ�����

//״̬��������
#define TTS_FIFO_DEPTH              (2)   //һЩʱ���޹ص���ʾTTS�������
#define TTS_SECTIONS_MAX            (16)  //һ�������������Ƭ���������1��Ϊ0xff��ʾ������
                                          //��������1234 ��һǧ������ʮ�ġ�����7��

/*! TTS��غ궨�� */
#define IGNORE_PLAY_TTS             (1 << 7)
#define FORCE_STOP_TTS              (1 << 6)

/*! TTS��غ궨�� */
#define TTS_MIN_VOL                  8     //TTS����ʱ����С����

typedef enum
{
    TTS_CTRL_IGNORE = 0,
    TTS_CTRL_CANCEL_IGNORE = 1,
    TTS_CTRL_FORCE_STOP = 2,
    TTS_CTRL_CANCEL_FORCE_STOP = 3,
} tts_control_opr_e;

typedef enum
{
    TTS_STATUS_IDLE = 0, //��ʾ����״̬
    TTS_STATUS_READY = 1, //��ʾ��ҪTTS����
    TTS_STATUS_PLAYING = 2, //��ʾ�ѳ�ʼ���ã����ڲ���
    TTS_STATUS_PLAYOVER = 3, //��ʾ�Ѳ������
} tts_status_e;

typedef struct
{
    uint16 cur_mode;
    uint8 count;
    /*! ����TTS���ƣ�[bit7] ���Բ���TTS��[bit6] ǿ����ֹ����TTS */
    uint8 option;
    /*!TTS����״̬��*/
    tts_status_e status;
    uint8 section_ids[TTS_SECTIONS_MAX];
    uint8 tts_fifo[TTS_FIFO_DEPTH][TTS_SECTIONS_MAX];
    uint16 tts_mode[TTS_FIFO_DEPTH];
    void* tts_str;
} tts_play_info_t;

extern tts_play_info_t g_tts_play_info;

typedef enum
{
    TTS_PLAY_RET_NORMAL,
    TTS_PLAY_RET_BY_KEY,
    TTS_PLAY_RET_BY_SYS,
    TTS_PLAY_RET_BY_BTEV,
} tts_play_ret_e;

extern tts_play_ret_e com_tts_state_play(uint16 tts_mode, void *tts_info) __FAR__;
extern tts_play_ret_e com_tts_state_play_wait(void) __FAR__;
extern tts_play_ret_e com_tts_state_play_deal(void) __FAR__;
extern void com_tts_state_play_control(tts_control_opr_e opr) __FAR__;

#endif
