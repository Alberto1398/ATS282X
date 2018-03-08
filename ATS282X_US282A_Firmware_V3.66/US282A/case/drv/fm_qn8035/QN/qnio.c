#include "fm_drv.h"
#include "fm_qn8035.h"

void TWI_SDA_Output_Dis(gpio_init_cfg_t* gpio);
void TWI_SDA_Input_Dis(gpio_init_cfg_t* gpio);
void TWI_SCL_Output_Dis(gpio_init_cfg_t* gpio);
void TWI_SCL_Input_Dis(gpio_init_cfg_t* gpio);

void SwitchGPIO(void)
{
    TWI_SDA_Output_Dis(twi_gpio);
    TWI_SDA_Input_Dis(twi_gpio);
    TWI_SCL_Output_Dis(twi_gpio);
    TWI_SCL_Input_Dis(twi_gpio);
}

//����ֵ��ʧ��->-1; �ɹ�->0
UINT8 QND_WriteReg(UINT8 Regis_Addr, UINT8 Data)
{
    uint8 i;
    int result = 0;
    uint8 writebuffer[2];

    uint32 flags;
    //��ΪA31������I2C_SCL, ��ͬʱ�п��Ա�����SPI_MO/SPI_MI
    //���I2C��ʼ������ˣ�����ϣ�ȥ��дSPI, ��������

    //���ж�
    flags = sys_local_irq_save();

    writebuffer[0] = Regis_Addr;
    //register data
    writebuffer[1] = Data;

    for (i = 0; i < 10; i++)
    {
        //send 3 byte to fm module
        if (TWI_Trans_Bytes(writebuffer, TWI_WRITE_ADDR,
                sizeof(writebuffer), twi_gpio, &delay_twi) != 0)
        {
            break;
        }
    }

    SwitchGPIO();
    //���ж�
    sys_local_irq_restore(flags);
    if (i == 10)
    {
        result = -1;
    }
    return result;
}

UINT8 QND_ReadReg(UINT8 Regis_Addr)
{
    uint8 Data = 0;
    uint32 flags;
    uint8 init_cnt = 0;
    uint8 ret = 0;

    //��ΪA31������I2C_SCL, ��ͬʱ�п��Ա�����SPI_MO/SPI_MI
    //���I2C��ʼ������ˣ�����ϣ�ȥ��дSPI, ��������

    //���ж�
    flags = sys_local_irq_save();
    //TWI init
    for (init_cnt = 0;init_cnt<10; init_cnt++)
    {
        TWI_Init(twi_gpio, &delay_twi);

        TWI_Start(twi_gpio, &delay_twi);

        //send TWI write addr
        TWI_WriteByte(TWI_WRITE_ADDR, twi_gpio, &delay_twi);

        ret = TWI_GetAck(twi_gpio, &delay_twi);
        if (ret != 0)
        {
            //���Ĵ�����ַ,��ַ�ڸ�λ��������1λ����λΪ1��ʾ������0x0a�Ĵ���������
            TWI_WriteByte(Regis_Addr, twi_gpio, &delay_twi);

            ret = TWI_GetAck(twi_gpio, &delay_twi);
            if (ret != 0)
            {
                TWI_Start(twi_gpio, &delay_twi);
                //send TWI read addr
                TWI_WriteByte(TWI_WRITE_ADDR | 1, twi_gpio, &delay_twi);

                ret = TWI_GetAck(twi_gpio, &delay_twi);
                if (ret != 0)
                {
                    //receive data
                    Data = TWI_ReadByte(twi_gpio, &delay_twi);
                    //���һ�ֽڷ���NACK
                    TWI_SendAck(1, twi_gpio, &delay_twi);
                }
            }
            TWI_Stop(twi_gpio, &delay_twi);
        }

        if (ret != 0)
        {
            break;
        }
    }
    SwitchGPIO();
    //���ж�
    sys_local_irq_restore(flags);
    return Data;
}

/*************************************************************************************************************
 end file
 ************************************************************************************************************/
