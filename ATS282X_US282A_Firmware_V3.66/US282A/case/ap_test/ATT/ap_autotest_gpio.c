/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     user1_main.c
 * \brief    picture��ģ�飬������̳�ʼ�����˳�������������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_manager_test.h"
#include "ap_autotest_gpio.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  gpio�����Ӻ���������GPIO�Ƿ���ڶ�·
 * \param[in]    reg_val   ��������ֵ
 * \param[in]    reg_base  gpio�Ĵ�������ַ����ΪGPIOA��GPIOB
 * \param[out]   the result TRUE/FALSE
 * \return       none
 * \ingroup      music_test
 * \note
 *******************************************************************************/
int32 test_gpio_short(uint32 reg_valA, uint32 reg_baseA, uint32 reg_valB, uint32 reg_baseB, uint32 reg_valSIO,
        uint32 reg_baseSIO, int *index)
{
    uint8 i;
    uint32 reg_value;
    uint32 gpio_dat;

    gpio_ret_t *gpio_regA = (gpio_ret_t *) reg_baseA;

    gpio_ret_t *gpio_regB = (gpio_ret_t *) reg_baseB;

    gpio_ret_t *gpio_regSIO = (gpio_ret_t *) reg_baseSIO;

    //ȡ������������
    gpio_regA->pu_en &= (~reg_valA);

    gpio_regA->pd_en &= (~reg_valA);

    gpio_regB->pu_en &= (~reg_valB);

    gpio_regB->pd_en &= (~reg_valB);

    gpio_regSIO->pu_en &= (~reg_valSIO);

    gpio_regSIO->pd_en &= (~reg_valSIO);

    //A0Ĭ������ߵ�ƽ��IO�ڽ���������
    act_writel(act_readl(GPIOAOUTEN) | (1 << 0), GPIOAOUTEN);
    act_writel(act_readl(GPIOADAT) | (1 << 0), GPIOADAT);

    //���������VRO(SIO_7), ��������Ϊ���ƿ�  Ĭ������ߵ�ƽ��IO�ڽ���������
    if (!(reg_valSIO & (1 << 7)))
    {
        act_writel(act_readl(SIO_OUTEN) | (1 << 7), SIO_OUTEN);
        act_writel(act_readl(SIO_DAT) | (1 << 7), SIO_DAT);
    }

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valA & reg_value) != 0)
        {
            //���Թܽ�Ϊ����״̬
            gpio_regA->out_en &= (~reg_value);

            gpio_regA->in_en |= reg_value;

            sys_mdelay(1);

            //ʹ�������ܽ�Ϊ���״̬
            gpio_regA->out_en |= (reg_valA - reg_value);

            //�����ܽ�����ߵ�ƽ
            gpio_regA->dat |= (reg_valA - reg_value);

            gpio_regB->out_en |= (reg_valB);

            gpio_regB->dat |= reg_valB;

            gpio_regSIO->out_en |= reg_valSIO;
            gpio_regSIO->dat |= reg_valSIO;

            sys_mdelay(1);

            gpio_dat = (gpio_regA->dat & reg_value);

            //�����ܽ�����͵�ƽ
            gpio_regA->dat &= (~(reg_valA - reg_value));

            gpio_regB->dat &= (~reg_valB);

            gpio_regSIO->dat &= (~reg_valSIO);

            sys_mdelay(1);

            //���Թܽ������ƽ�б仯��˵�����ڶ�·
            if ((gpio_regA->dat & reg_value) != gpio_dat)
            {
                att_write_test_info("GPIOA err short:", i, 1);
                *index = i;
                return TEST_GPIOA_SHORT;
            }
            else
            {
                //һֱ��⵽�ߵ�ƽ��˵��VCC��·
                if (gpio_dat != 0)
                {
                    att_write_test_info("GPIOA err short vcc:", i, 1);
                    *index = i;
                    return TEST_GPIOA_SHORT_VCC;
                }
            }
        }
    }

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valB & reg_value) != 0)
        {
            //���Թܽ�Ϊ����״̬
            gpio_regB->out_en &= (~reg_value);

            gpio_regB->in_en |= reg_value;

            sys_mdelay(1);

            //ʹ�������ܽ�Ϊ���״̬
            gpio_regB->out_en |= (reg_valB - reg_value);

            //�����ܽ�����ߵ�ƽ
            gpio_regB->dat |= (reg_valB - reg_value);

            gpio_regA->out_en |= (reg_valA);

            gpio_regA->dat |= reg_valA;

            gpio_regSIO->out_en |= (reg_valSIO);

            gpio_regSIO->dat |= reg_valSIO;

            sys_mdelay(1);

            gpio_dat = (gpio_regB->dat & reg_value);

            //�����ܽ�����͵�ƽ
            gpio_regB->dat &= (~(reg_valB - reg_value));

            gpio_regA->dat &= (~reg_valA);

            gpio_regSIO->dat &= (~reg_valSIO);

            sys_mdelay(1);

            //���Թܽ������ƽ�б仯��˵�����ڶ�·
            if ((gpio_regB->dat & reg_value) != gpio_dat)
            {
                att_write_test_info("GPIOB err short:", i, 1);
                *index = i;
                return TEST_GPIOB_SHORT;
            }
            else
            {
                //һֱ��⵽�ߵ�ƽ��˵��VCC��·
                if (gpio_dat != 0)
                {
                    att_write_test_info("SIO err short vcc:", i, 1);
                    *index = i;
                    return TEST_GPIOB_SHORT_VCC;
                }
            }
        }
    }

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valSIO & reg_value) != 0)
        {
            //���Թܽ�Ϊ����״̬
            gpio_regSIO->out_en &= (~reg_value);

            gpio_regSIO->in_en |= reg_value;

            sys_mdelay(1);

            //ʹ�������ܽ�Ϊ���״̬
            gpio_regSIO->out_en |= (reg_valSIO - reg_value);

            //�����ܽ�����ߵ�ƽ
            gpio_regSIO->dat |= (reg_valSIO - reg_value);

            gpio_regA->out_en |= (reg_valA);

            gpio_regA->dat |= reg_valA;

            gpio_regB->out_en |= (reg_valB);

            gpio_regB->dat |= reg_valB;

            sys_mdelay(1);

            gpio_dat = (gpio_regSIO->dat & reg_value);

            //�����ܽ�����͵�ƽ
            gpio_regSIO->dat &= (~(reg_valSIO - reg_value));

            gpio_regA->dat &= (~reg_valA);

            gpio_regB->dat &= (~reg_valB);

            sys_mdelay(1);

            //���Թܽ������ƽ�б仯��˵�����ڶ�·
            if ((gpio_regSIO->dat & reg_value) != gpio_dat)
            {
                att_write_test_info("SIO err short:", i, 1);
                *index = i;
                return TEST_GPIOSIO_SHORT;
            }
            else
            {
                //һֱ��⵽�ߵ�ƽ��˵��VCC��·
                if (gpio_dat != 0)
                {
                    att_write_test_info("SIO err short vcc:", i, 1);
                    *index = i;
                    return TEST_GPIOSIO_SHORT_VCC;
                }
            }
        }
    }

    return TEST_PASS;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  gpio�����Ӻ���������GPIO�Ƿ���ڶ�·
 * \param[in]    reg_val   ��������ֵ
 * \param[in]    reg_base  gpio�Ĵ�������ַ����ΪGPIOA��GPIOB
 * \param[out]   the result TRUE/FALSE
 * \return       none
 * \ingroup      music_test
 * \note
 *******************************************************************************/
int32 test_gpio_short_gnd(uint32 reg_valA, uint32 reg_baseA, uint32 reg_valB, uint32 reg_baseB, uint32 reg_valSIO,
        uint32 reg_baseSIO, int *index)
{
    uint8 i;
    uint32 reg_value;
    uint32 gpio_dat;
    gpio_ret_t *gpio_regA = (gpio_ret_t *) reg_baseA;
    gpio_ret_t *gpio_regB = (gpio_ret_t *) reg_baseB;
    gpio_ret_t *gpio_regSIO = (gpio_ret_t *) reg_baseSIO;

    //A0Ĭ������͵�ƽ��IO�ڽ���������
    act_writel(act_readl(GPIOAOUTEN) | (1 << 0), GPIOAOUTEN);
    act_writel(act_readl(GPIOADAT) & (~(1 << 0)), GPIOADAT);

    //���������VRO(SIO_7), ��������Ϊ���ƿ�  Ĭ������͵�ƽ��IO�ڽ���������
    if (!(reg_valSIO & (1 << 7)))
    {
        act_writel(act_readl(SIO_OUTEN) | (1 << 7), SIO_OUTEN);
        act_writel(act_readl(SIO_DAT) & (~(1 << 7)), SIO_DAT);
    }

    //ȡ������������
    gpio_regA->pu_en &= (~reg_valA);

    gpio_regA->pd_en &= (~reg_valA);

    gpio_regB->pu_en &= (~reg_valB);

    gpio_regB->pd_en &= (~reg_valB);

    gpio_regSIO->pu_en &= (~reg_valSIO);

    gpio_regSIO->pd_en &= (~reg_valSIO);

    //���Թܽ�Ϊ����״̬
    gpio_regA->out_en &= (~reg_valA);

    gpio_regA->in_en |= reg_valA;

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valA & reg_value) != 0)
        {
            sys_mdelay(2);

            //���Թܽ������ƽΪ0��˵���̽ӵ���
            if ((gpio_regA->dat & reg_value) == 0)
            {
                att_write_test_info("GPIOA err short gnd:", i, 1);

                *index = i;

                return TEST_GPIOA_SHORT_GND;
            }
        }
    }

    //���Թܽ�Ϊ����״̬
    gpio_regB->out_en &= (~reg_valB);

    gpio_regB->in_en |= reg_valB;

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valB & reg_value) != 0)
        {
            sys_mdelay(2);

            //���Թܽ������ƽΪ0��˵���̽ӵ���
            if ((gpio_regB->dat & reg_value) == 0)
            {
                att_write_test_info("GPIOB err short gnd:", i, 1);

                *index = i;

                return TEST_GPIOB_SHORT_GND;
            }
        }
    }

    DEBUG_ATT_PRINT("SIO REG:", reg_valSIO, 2);

    //���Թܽ�Ϊ����״̬
    gpio_regSIO->out_en &= (~reg_valSIO);

    gpio_regSIO->in_en |= reg_valSIO;

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valSIO & reg_value) != 0)
        {
            sys_mdelay(2);

            //���Թܽ������ƽΪ0��˵���̽ӵ���
            if ((gpio_regSIO->dat & reg_value) == 0)
            {
                att_write_test_info("SIO err short gnd:", i, 1);

                *index = i;

                return TEST_GPIOSIO_SHORT_GND;
            }
        }
    }

    return TEST_PASS;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  gpio�����Ӻ���������GPIO�Ƿ���ڶ�·
 * \param[in]    reg_val   ��������ֵ
 * \param[in]    reg_base  gpio�Ĵ�������ַ����ΪGPIOA��GPIOB
 * \param[out]   the result TRUE/FALSE
 * \return       none
 * \ingroup      music_test
 * \note
 *******************************************************************************/
bool test_gpio_open(uint32 reg_valA, uint32 reg_baseA, uint32 reg_valB, uint32 reg_baseB, uint32 reg_valSIO,
        uint32 reg_baseSIO, int32 *index)
{
    uint8 i;
    uint32 reg_value;
    gpio_ret_t *gpio_regA = (gpio_ret_t *) reg_baseA;
    gpio_ret_t *gpio_regB = (gpio_ret_t *) reg_baseB;
    gpio_ret_t *gpio_regSIO = (gpio_ret_t *) reg_baseSIO;

    //A0Ĭ������ߵ�ƽ��IO�ڽ���������
    act_writel(act_readl(GPIOAOUTEN) | (1 << 0), GPIOAOUTEN);
    act_writel(act_readl(GPIOADAT) | (1 << 0), GPIOADAT);

    //���������VRO(SIO_7), ��������Ϊ���ƿ�  Ĭ������ߵ�ƽ��IO�ڽ���������
    if (!(reg_valSIO & (1 << 7)))
    {
        act_writel(act_readl(SIO_OUTEN) | (1 << 7), SIO_OUTEN);
        act_writel(act_readl(SIO_DAT) | (1 << 7), SIO_DAT);
    }

    //ȡ������������
    gpio_regA->pu_en &= (~reg_valA);

    gpio_regA->pd_en &= (~reg_valA);

    gpio_regB->pu_en &= (~reg_valB);

    gpio_regB->pd_en &= (~reg_valB);

    gpio_regSIO->pu_en &= (~reg_valSIO);

    gpio_regSIO->pd_en &= (~reg_valSIO);

    gpio_regA->out_en &= (~reg_valA);

    gpio_regB->out_en &= (~reg_valB);

    gpio_regSIO->out_en &= (~reg_valSIO);

    //ʹ�ܹܽ�����
    gpio_regA->in_en |= reg_valA;

    gpio_regB->in_en |= reg_valB;

    gpio_regSIO->in_en |= reg_valSIO;

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valA & reg_value) != 0)
        {
            //ʹ���ڲ�����
            gpio_regA->pu_en |= reg_value;

            //�ȴ������ȶ�
            sys_mdelay(2);

            //��ȡgpio��ֵ
            if ((gpio_regA->dat & reg_value) != 0)
            {
                att_write_test_info("GPIOA err open:", i, 1);
                *index = i;
                return TEST_GPIOA_OPEN;
            }
        }
    }

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valB & reg_value) != 0)
        {
            //ʹ���ڲ�����
            gpio_regB->pu_en |= reg_value;

            //�ȴ������ȶ�
            sys_mdelay(2);

            //��ȡgpio��ֵ
            if ((gpio_regB->dat & reg_value) != 0)
            {
                att_write_test_info("GPIOB err open:", i, 1);
                *index = i;
                return TEST_GPIOB_OPEN;
            }
        }
    }

    for (i = 0; i < 31; i++)
    {
        reg_value = (1 << i);

        if ((reg_valSIO & reg_value) != 0)
        {
            //ʹ���ڲ�����
            gpio_regSIO->pu_en |= reg_value;

            //�ȴ������ȶ�
            sys_mdelay(2);

            //��ȡgpio��ֵ
            if ((gpio_regSIO->dat & reg_value) != 0)
            {
                att_write_test_info("SIO err open:", i, 1);
                *index = i;
                return TEST_GPIOSIO_OPEN;
            }
        }
    }

    return TEST_PASS;
}

