/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������� GUI������������ң�أ�TK����Ϣ��صĺ궨�壬ö�����ͣ��ṹ�嶨��ȡ�
 *       ���� GUI ��Ϣ�Ӱ���������������COMMON����з���ת��Ϊ�߼� GUI ��Ϣ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _KEY_PHY_H_
#define _KEY_PHY_H_

#include "psp_includes.h"

/*!
 * \brief
 *  key_type_e ����������
 */
typedef enum
{
    /*! ��ͨ���� */
    KEY_TYPE_KEY = 0x00,
    /*! IR����ң�� */
    KEY_TYPE_IR = 0x01,
    /*! �������� */
    KEY_TYPE_TK = 0x02,
} key_phy_type_e;

typedef enum
{
    /*! ��ʼ���� */
    KEY_PHY_DOWN = 0x00,
    /*! �������� */
    KEY_PHY_CONT = 0x01,
    /*! ̧�𰴼� */
    KEY_PHY_UP = 0x02,
} key_phy_status_e;

//����KEY ID
#define NO_KEY          0xff
#define KEY_Hold        0x30
#define KEY_UnHold      0x31
#define KEY_PHY_PLAY    0x32
#define KEY_UP          0x80

//IRң������������
#define IR_CODE_FF00    0
#define IR_CODE_BF00    1
#define IR_CODE_7F80    2
#define IR_CODE_C33C    3
#define IR_CODE_MAX     4

#define IR_KEY_MAX      21

/*!
 * \brief  
 *  key_phy_msg_t ��������Ϣ
 */
typedef struct
{
    uint8 key_type :3;
    uint8 key_status :2;
    uint8 key_ir_code :2;
    uint8 key_reserve :1;
    uint8 key_id;/*0��ʼ��0x80��ʾ̧��0xff��ʾ�ް���*/
    uint16 key_timestamp;
} key_phy_msg_t;

#endif
