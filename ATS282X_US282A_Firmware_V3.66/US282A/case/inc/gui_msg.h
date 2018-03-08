/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������߼� GUI������������ң�أ�TK����Ϣ��صĺ궨�壬ö�����ͣ��ṹ�嶨��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _gui_msg_H_
#define _gui_msg_H_

#include "psp_includes.h"
#include "key_phy.h"

/*!
 * \brief
 *  input_msg_type_e ��������Ϣ���ͣ�Ҳ�� gui ��Ϣ����
 */
typedef enum
{
    /*! ��ͨ���� */
    INPUT_MSG_KEY = 0x00,
} input_msg_type_e;

/*!
 * \brief
 *  key_value_e �߼�����ö������
 */
typedef enum
{
    /*! �հ��� */
    KEY_NULL = 0x00,
    /*! ����/��ͣ���� */
    KEY_PLAY = 0x01,
    /*! ��һ��󣩰��� */
    KEY_PREV = 0x02,
    /*! ��һ��ң����� */
    KEY_NEXT = 0x03,
    /*! �������ڿ�ݰ��� */
    KEY_VOL = 0x04,
    /*! �����ӿ�ݰ��� */
    KEY_VADD = 0x05,
    /*! ��������ݰ��� */
    KEY_VSUB = 0x06,
    /*! �˵�/ѡ��� */
    KEY_MODE = 0x07,
    /*! eq���� */
    KEY_EQ = 0x08,
    /*! ѭ��ģʽ���� */
    KEY_REPEAT = 0x09,
    /*! mute���� */
    KEY_MUTE = 0x0a,
    /*! clock���� */
    KEY_CLK = 0x0b,
    /*! folder+���� */
    KEY_FOLDER_ADD = 0x0c,
    /*! folder-���� */
    KEY_FOLDER_SUB = 0x0d,
    /*!next_vol_add���ð���*/
    KEY_NEXT_VOLADD = 0x0e,
    /*!prev_vol_sub���ð���*/
    KEY_PREV_VOLSUB = 0x0f,
    /*! ����0���� */
    KEY_NUM0 = 0x10,
    /*! ����1���� */
    KEY_NUM1 = 0x11,
    /*! ����2���� */
    KEY_NUM2 = 0x12,
    /*! ����3���� */
    KEY_NUM3 = 0x13,
    /*! ����4���� */
    KEY_NUM4 = 0x14,
    /*! ����5���� */
    KEY_NUM5 = 0x15,
    /*! ����6���� */
    KEY_NUM6 = 0x16,
    /*! ����7���� */
    KEY_NUM7 = 0x17,
    /*! ����8���� */
    KEY_NUM8 = 0x18,
    /*! ����9���� */
    KEY_NUM9 = 0x19,
    /*! ��ָ���ְ��� */
    KEY_NUMBER = 0x1a,
    /*! FM�������� */
    MSG_SCAN = 0x1b,
    /*!���ػ�����*/
    KEY_POWER = 0x1c,
    /*!for ab/record key*/
    KEY_ABREC = 0x1d,
    /*!Phone ��*/
    KEY_PHONE = 0x1e,
    /*!Phone/Play ��*/
    KEY_PHONEPLAY = 0x1f,
} key_value_e;

/*!
 * \brief
 *  key_type_e ��������
 */
typedef enum
{
    /*! û�а���*/
    KEY_TYPE_NULL = 0x00,
    /*! ˫������*/
    KEY_TYPE_DBL_CLICK = 0x20,
    /*! ��������*/
    KEY_TYPE_DOWN = 0x10,
    /*! ��������*/
    KEY_TYPE_LONG = 0x08,
    /*! ��������*/
    KEY_TYPE_HOLD = 0x04,
    /*! �����̰�����*/
    KEY_TYPE_SHORT_UP = 0x02,
    /*! ������������*/
    KEY_TYPE_LONG_UP = 0x01,
    /*! ���ⰴ������ */
    KEY_TYPE_ALL = 0x3f,
    /*!������*/    
    KEY_TYPE_LONG_10S = 0x40,
} key_type_e;

typedef enum
{
    KEY_DEAL_NULL, //�������⴦��
    KEY_DEAL_FILTER, //���˺�������
    KEY_DEAL_FILTER_UP, //������̧�𰴼�
    KEY_DEAL_ITSELF,//��������������ֹ����������ظ���Ӧ
} key_deal_e;

/*!
 * \brief  
 *  key_event_t �����¼��ṹ��
 */
typedef struct
{
    /*! �������߼���ֵ*/
    key_value_e val;
    /*! ����ѡ�bit0 ��ʾ�Ƿ���Ҫ��������������ʱ���ΰ����� */
    uint8 option;
    /*! ��������*/
    key_type_e type;
    /*! ������������ѡ��*/
    key_deal_e deal;
} key_event_t;

/*!
 * \brief  
 *  input_gui_msg_t ��������Ϣ���ֳ� gui ��Ϣ�ṹ��
 */
typedef struct
{
    /*! ��Ϣ����*/
    input_msg_type_e type;
    /*! ��Ϣ����*/
    union
    {
        /*! �����¼�*/
        key_event_t kmsg;
    } data;
} input_gui_msg_t;

/*!
 * \brief  
 *  app_msg_hook ������Ϣ���⴦������
 */
typedef bool (*gui_msg_hook)(input_gui_msg_t *input_msg);

/*! 
 *  \brief
 *  g_this_gui_msg_hook Ӧ��˽����Ϣ������
 */
extern gui_msg_hook g_this_gui_msg_hook;

/*!
 *  \brief
 *  g_buffer_gui_msg ����gui��Ϣ
 */
extern input_gui_msg_t g_buffer_gui_msg;

//�����߼�����ģ��
typedef enum
{
    KEY_STATUS_NULL,
    KEY_STATUS_DOWN,
    KEY_STATUS_HOLD,
    KEY_STATUS_DOWN_1ST,
    KEY_STATUS_UP_1ST,
    KEY_STATUS_DOWN_2ND,
    KEY_STATUS_DCK_MATCH,
} key_status_e;

#define KEY_MAX_COUNT     8
#define TK_KEY_MAX_COUNT  8

typedef struct
{
    uint32 dck_map; //�߼������Ƿ���˫�������¼���ÿ���߼�������Ӧ1��bit�������߼�����ID����ֵҪ����get_gui_msgǰ����
    uint16 dck_timestamp;
    uint16 last_detect_timestamp;
    uint8 dck_val;
    bool dck_fail;
    bool dck_flag;
    uint8 count_fail;
    key_status_e status;
    uint8 count;
    /*! ���˵��������������������� KEY_TYPE_HOLD & KEY_TYPE_SHORT_UP */
    uint8 filter_key;
    /*! ���˵��������������������� KEY_TYPE_SHORT_UP */
    uint8 filter_key_up;
    /*! ���˳���������������������*/
    uint8 filter_key_itself;
    //�߼�����ӳ���
    uint8 key_val_map[KEY_MAX_COUNT];
    //��������ӳ���
    uint8 tk_key_val_map[TK_KEY_MAX_COUNT];
} key_infor_t;

extern key_infor_t g_key_infor;

//������Ϣ�ӿ�
extern bool get_gui_msg(input_gui_msg_t *input_msg) __FAR__;
extern bool com_phy_key_fsm(input_gui_msg_t *input_msg) __FAR__;
extern bool com_need_cont_dck_deal(uint8 key_count) __FAR__;
extern void com_filter_keymsg_in_queue(key_value_e k_val, key_type_e k_type) __FAR__;

#endif
