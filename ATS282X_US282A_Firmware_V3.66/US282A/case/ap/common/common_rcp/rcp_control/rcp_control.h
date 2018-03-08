/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RCP ����ṹ�嶨�塣
 * ���ߣ�cailizhen
 ********************************************************************************/
 
#ifndef _RCP_CONTROL_H
#define _RCP_CONTROL_H

#include "psp_includes.h"

#define RCP_MAGIC_SOH            (0x01)
#define RCP_MAGIC_SOH_R          (0xfe)
#define RCP_CHAN_MASK            (0x03 << 0)
#define RCP_CHAN_SPP             (0x00 << 0)
#define RCP_CHAN_I2C_SPI_UART    (0x01 << 0)
#define RCP_CHAN_GPIO            (0x02 << 0)
#define RCP_SEGMENT_MASK         (0x03 << 2)
#define RCP_SEGMENT_NONE         (0x00 << 2)
#define RCP_SEGMENT_FIRST        (0x01 << 2)
#define RCP_SEGMENT_CONT         (0x02 << 2)
#define RCP_SEGMENT_LAST         (0x03 << 2)
#define RCP_ACKFLAG_MASK         (0x01 << 4)
#define RCP_ACKFLAG_ACK          (0x01 << 4)
#define RCP_SEQ_MASK             (0x0f << 0)

/*! RCP����ṹ�� */
typedef struct
{
    /*! 0x01, 0xfe ,ASCII���SOH start of head ���䷴�� */
    uint8 magic[2];

    /*! ��һ���ֽڵ�һЩ�����־��
     *    bit0-1 ��ʾͨ��ģʽ��00��ʾSPP��01��ʾI2C��SPI��UARTͨ����10��ʾGPIOͨ����11δ���塣
     *    bit2-3 ��ʾ���ݰ�λ�ã�00��ʾ���÷ְ���01��ʾĳ������ĵ�һ�����ݰ���10��ʾ�����������һ������
     *           11��ʾ���һ������
     *    bit4   ACK��־��1��ʾ��ҪӦ���ACK���ܼ��������������
     *    bit5-7 δ���壬Ӧ�ó�ʼ��Ϊ0��
     *  �ڶ����ֽ���ʱ���á�
     */
    uint8 flags[2];

    /*! ��������
     * ��1�� 'P'��Ӧ���л��� (0x50)
     * ��2�� 'C'��������     (0x43)
     * ��3�� 'K'������ģ���� (0x4B)
     * ��4�� 'Q'����ѯ��     (0x51)
     * ��5�� 'A'����ѯӦ���� (0x41)
     * ��6�� 'S'������������ (0x53)
     * ��7�� 'R': ACK/NACK�� (0x52)
     */
    uint8 cmd_type;

    /*! ����ID�� */
    uint8 cmd_id;
    /*! ����ȣ�����ͷ+�������ȣ�����magic��flags����������checksum */
    uint16 cmd_len;
    /*! �������1 */
    uint32 param1;
    /*! �������2 */
    uint32 param2;
} rcp_command_t;

#endif
