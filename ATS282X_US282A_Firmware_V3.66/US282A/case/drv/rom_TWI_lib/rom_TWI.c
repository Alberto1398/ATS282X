//�ף���ע�⣺
//�ⲿ�ִ����Ѿ���¼������оƬ��(����spiflash��)���ǲ��ܸĶ���!!!
//�Ѵ��뷢�������ֻ��Ϊ�˷����������롣
#include "TWI.h"
#include "TWI_inner.h"

void TWI_Init(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_Exit(gpio_init_cfg_t* gpio);
void TWI_Delay(uint8* delay);
void TWI_SDA_high_hold(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_SDA_low_hold(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_SCL_high_hold(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_SCL_low_hold(gpio_init_cfg_t* gpio, uint8* delay);

void TWI_SDA_Output_En(gpio_init_cfg_t* gpio);
void TWI_SDA_Output_Dis(gpio_init_cfg_t* gpio);
void TWI_SDA_Input_En(gpio_init_cfg_t* gpio);
void TWI_SDA_Input_Dis(gpio_init_cfg_t* gpio);
void TWI_SCL_Output_En(gpio_init_cfg_t* gpio);
void TWI_SCL_Output_Dis(gpio_init_cfg_t* gpio);
void TWI_SCL_Input_En(gpio_init_cfg_t* gpio);
void TWI_SCL_Input_Dis(gpio_init_cfg_t* gpio);
uint8 TWI_Data_Input(gpio_init_cfg_t* gpio);

void TWI_Start(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_Stop(gpio_init_cfg_t* gpio, uint8* delay);

uint8 TWI_Trans_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
uint8 TWI_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
void TWI_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay);
uint8 TWI_ReadByte(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay);
uint8 TWI_GetAck(gpio_init_cfg_t* gpio, uint8* delay);

//TWI��ʼ��
//gpio: ģ��TWI�������õ�IO
//delay: ������ʱ
void TWI_Init(gpio_init_cfg_t* gpio, uint8* delay)
{
    TWI_SDA_high_hold(gpio, delay);
    TWI_SCL_high_hold(gpio, delay);
    TWI_SDA_Output_En(gpio);
    TWI_SCL_Output_En(gpio);
    TWI_SCL_Input_En(gpio);
    TWI_SDA_Input_En(gpio);
}

//TWI�˳�
void TWI_Exit(gpio_init_cfg_t* gpio)
{
    TWI_SDA_Output_Dis(gpio);
    TWI_SDA_Input_Dis(gpio);
    TWI_SCL_Output_Dis(gpio);
    TWI_SCL_Input_Dis(gpio);
}

//��ʱ����
void TWI_Delay(uint8* delay)
{
    uint8 j, value;

    value = *delay;
    for (j = 0; j < value; j++)
    {
        //��ʱ����
        //;
    }
}

//SDA�����
void TWI_SDA_high_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl((gpio + 1)->reg_data) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_data);
    TWI_Delay(delay);
}

//SDA�����
void TWI_SDA_low_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl((gpio + 1)->reg_data) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_data);
    TWI_Delay(delay);
}

//SCL�����
void TWI_SCL_high_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl(gpio->reg_data) | (gpio->reg_bit)), gpio->reg_data);
    TWI_Delay(delay);
}

//SCL�����
void TWI_SCL_low_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl(gpio->reg_data) & (~(gpio->reg_bit))), gpio->reg_data);
    TWI_Delay(delay);
}

//SDA���ʹ��
void TWI_SDA_Output_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_out) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_out);
}

//SDA�����ʹ��
void TWI_SDA_Output_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_out) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_out);
}

//SDA����ʹ��
void TWI_SDA_Input_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_in) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_in);
}

//SDA���벻ʹ��
void TWI_SDA_Input_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_in) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_in);
}

//SDA����ʹ��
void TWI_SCL_Output_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_out) | (gpio->reg_bit)), gpio->reg_out);
}

//SCL�����ʹ��
void TWI_SCL_Output_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_out) & (~(gpio->reg_bit))), gpio->reg_out);
}

//SCL����ʹ��
void TWI_SCL_Input_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_in) | (gpio->reg_bit)), gpio->reg_in);
}

//SCL���벻ʹ��
void TWI_SCL_Input_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_in) & (~(gpio->reg_bit))), gpio->reg_in);
}

//�����32 bit ������һ��bit Ϊ1
//����ֵ�����ҵ���Ϊ1��bitλ(0~31)(�ӵ�Ϊ����λ����)
uint8 TWI_Data_Bit(uint32 value)
{
    uint8 loop;
    uint32 temp;
    for (loop = 0; loop < 32; loop++)
    {
        temp = (value >> loop) & 0x00000001;
        if (temp != 0)
        {
            break;
        }
    }
    return loop;
}

//���أ�SDA�ϵĵ�ƽ
uint8 TWI_Data_Input(gpio_init_cfg_t* gpio)
{
    uint32 value;
    uint8 ret;
    value = act_readl((gpio + 1)->reg_data) & ((gpio + 1)->reg_bit);
    ret = TWI_Data_Bit((gpio + 1)->reg_bit);
    value = (value >> ret) & 0x00000001;
    ret = (uint8) value;
    return ret;
}

//ģ��TWI   Start  ����
/*
 ____
 SDA:|_______

 ______
 SCL:  |_____
 */
void TWI_Start(gpio_init_cfg_t* gpio, uint8* delay)
{
    TWI_SDA_high_hold(gpio, delay);
    TWI_SCL_high_hold(gpio, delay);
    TWI_SDA_low_hold(gpio, delay);
    TWI_SCL_low_hold(gpio, delay); /*ǯסTWI���ߣ�SCLΪ�͵�ƽ��׼�����ͻ�������� */
}

//ģ��TWI  Stop  ����
/*
 SDA      ________
 ________|
 ___________
 SCL:       |______
 */
void TWI_Stop(gpio_init_cfg_t* gpio, uint8* delay)
{
    TWI_SDA_low_hold(gpio, delay);
    TWI_SCL_high_hold(gpio, delay);
    TWI_SDA_high_hold(gpio, delay);
}

//����ŵ�*buf��ַ�ﳤ��Ϊlength�����ݷ��ͳ�ȥ
//buf: ���ݴ�ŵ�ַ
//address: �豸��ַ
//length: ���ݳ���, ��byteΪ��λ
//����ֵ:   FALSE: �������ݽ��յ�nack
//          TRUE: �������ݽ��յ�ack
uint8 TWI_Trans_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ret = 0;

    TWI_Init(gpio, delay);

    TWI_Start(gpio, delay);

    /*����slave ��ַ*/
    TWI_WriteByte(address, gpio, delay);
    if (TWI_GetAck(gpio, delay) != 0)
    {
        //��������
        do
        {
            TWI_WriteByte(*buf, gpio, delay);
            buf++;
            if (!TWI_GetAck(gpio, delay))
            {
                break;
            }
            --length;
        }
        while (length != 0);
        if (length == 0)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }

    TWI_Stop(gpio, delay);
    return ret;
}

//��
//buf: ���������ݷŵĵط�(���յ����һ���ֽں����ػᷢ��NACK)
//address: �豸��ַ(һ�����������BIT0Ϊ1)
//length: �����ٸ��ֽ�
//����ֵ: FALSE: slave����Ӧ�� TRUE: �յ�����
uint8 TWI_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ret = 0;
    uint8 *p;

    p = buf;

    TWI_Init(gpio, delay);

    TWI_Start(gpio, delay);

    /*����slave ��ַ*/
    TWI_WriteByte(address, gpio, delay);
    if (!TWI_GetAck(gpio, delay))
    {
        //ret = 0;
        goto recev_ret;
    }

    while (length != 0)
    {
        if (length == 1) //the last byte, sent nack
        {
            *p = TWI_ReadByte(gpio, delay);
            TWI_SendAck(1, gpio, delay);
        }
        else
        {
            *p = TWI_ReadByte(gpio, delay);
            TWI_SendAck(0, gpio, delay);
        }
        p++;
        length--;
    }
    ret = 1;
    recev_ret:
    TWI_Stop(gpio, delay);
    return ret;
}

//д1���ֽڵ����ݵ�slave
//������ִ��ǰSCL����Ϊ��
//����������ʱ��SCLΪ��
void TWI_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 i;
    for (i = 0; i < 8; i++)
    {
        if (((dat << i) & 0x80) != 0) //����һ���ֽڣ��Ӹ�bit ����bit ���η���
        {
            TWI_SDA_high_hold(gpio, delay);
        }
        else
        {
            TWI_SDA_low_hold(gpio, delay);
        }
        TWI_SCL_high_hold(gpio, delay);
        TWI_SCL_low_hold(gpio, delay);
    }
}

//��ȡ1���ֽڵ�����,����ack����nack��slave
//������ִ��ǰSCL����Ϊ��
//����������ʱ��SCLΪ��
uint8 TWI_ReadByte(gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 t = 8, dat = 0;
    uint8 TWI_DATA;

    //TWI_SDA_high_hold(gpio, delay);

    //TWI_Delay(delay);
    TWI_SDA_Output_Dis(gpio);
    //TWI_SDA_Input_En(gpio);
    TWI_Delay(delay);

    do
    {
        TWI_SCL_high_hold(gpio, delay);

        dat <<= 1;
        TWI_DATA = TWI_Data_Input(gpio);
        if (TWI_DATA != 0)
        {
            dat |= 0x01;
        } //��TWI �������ϲ���һ���ֽڵ����ݣ��ǴӸ�bit ����bit ����

        TWI_SCL_low_hold(gpio, delay);
        --t;
    } while (t != 0);

    //TWI_Delay(delay);
    //TWI_SDA_Input_Dis(gpio);
    TWI_SDA_Output_En(gpio);
    TWI_Delay(delay);
    return dat;
}

//����ack/nack
//ack: 1 noack; 0 ack
//������ִ��ǰSCL����Ϊ��
//����������ʱ��SCLΪ��
void TWI_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay)
{
    if (ack != 0)
    {
        TWI_SDA_high_hold(gpio, delay); //nack
    }
    else
    {
        TWI_SDA_low_hold(gpio, delay); //ack
    }
    TWI_SCL_high_hold(gpio, delay);
    TWI_SCL_low_hold(gpio, delay);
}

//��ȡ���豸�˷��͹�������Ӧ�ź�Ack  or  NAck
//����ֵ��TRUE->�õ���ack; FALSE->�õ�����nack
//������ִ��ǰSCL����Ϊ��
//����������ʱ��SCLΪ��
uint8 TWI_GetAck(gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ack;
    uint8 TWI_DATA;

    // TWI_SDA_high_hold(gpio, delay);

    //TWI_Delay(delay);
    TWI_SDA_Output_Dis(gpio);
    //TWI_SDA_Input_En(gpio);
    TWI_Delay(delay);
    TWI_SCL_high_hold(gpio, delay);
    TWI_DATA = TWI_Data_Input(gpio); //ֻ����һ��bit
    //TWI_SDA_Input_Dis(gpio);
    if (TWI_DATA != 0)
    {
        ack = 0; //nack
        //TWI_SDA_high_hold(gpio, delay);
    }
    else
    {
        ack = 1; //ack
        //TWI_SDA_low_hold(gpio, delay);
    }
    TWI_SDA_Output_En(gpio);
    TWI_Delay(delay);
    TWI_SCL_low_hold(gpio, delay);
    TWI_SDA_low_hold(gpio, delay);
    //TWI_SDA_Input_Dis(gpio);

    return ack;
}

