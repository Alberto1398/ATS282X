/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������EJTAG ���Կ����ã�����CASEʹ�õ�IO�ڶ�����CASEӦ�ý�EJTAG����Ϊû�б�ռ��
 *       ��EJTAG�ڣ����CASE������EJTAG�鶼ռ���ˣ��ͱ��뽫EJTAG �ڹرա�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "case_independent.h"

void welcome_ejtag(void)
{
    //EJTAG��1��: B1 ~ B5
    //EJTAG��2��: A1 ~ A4, A7
    //EJTAG��3��: A8 ~ A12
    //EJTAG��4��: B11, A16, A21 ~ A23

    //EJTAG��ѡ��MFP_CTL2 [28 29], 0��ʾ1�飬1��ʾ2�飬2��ʾ3�飬3��ʾ4��
#if (CASE_BOARD_TYPE == CASE_BOARD_EVB)
    act_writel((act_readl(MFP_CTL2) | (0x80000000)), MFP_CTL2); //����MEJTAG��1��
#else
    act_writel((act_readl(MFP_CTL2) & (~(0xc0000000))), MFP_CTL2); //�ر�EJTAG����
#endif
}
