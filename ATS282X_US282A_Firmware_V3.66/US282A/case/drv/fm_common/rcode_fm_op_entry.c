#include "fm_interface.h"
#include "fm_drv.h"

gpio_init_cfg_t gpio_twi[];

/* TWI ����ָ��*/
gpio_init_cfg_t *twi_gpio = (gpio_init_cfg_t *) gpio_twi;

/* TWI ����ģ��GPIO ��Ϣ�ṹ*/
gpio_init_cfg_t gpio_twi[2] =
{
    { GPIOAINEN, GPIOAOUTEN, GPIOADAT, GPIO_SCL_BIT },
    { GPIOAINEN, GPIOAOUTEN, GPIOADAT, GPIO_SDA_BIT }
};

/* TWI ������ʱ*/
uint8 delay_twi = PARAM_DELAY_TWI;

/*FM ��������ӿں���*/
fm_driver_operations fm_drv_op =
{
    (fm_op_func) sFM_Init, //��ʼ��
    (fm_op_func) sFM_Standby, //standby
    (fm_op_func) sFM_SetFreq, //����Ƶ��
    (fm_op_func) sFM_GetStatus, //��ȡ״̬
    (fm_op_func) sFM_Mute, //����
    (fm_op_func) sFM_Search, //�����̨
    (fm_op_func) sFM_HardSeek, //Ӳ����̨
    (fm_op_func) sFM_SetBand, //���ò���
    (fm_op_func) sFM_SetThrod, //������̨����
    (fm_op_func) sFM_BreakSeek, //�ж�����
    (fm_op_func) sFM_GetHardSeekflag, //��ȡӲ����̨���Ƿ����
    (fm_op_func) sFM_GetBand, //��ȡ��ǰ����
    (fm_op_func) sFM_GetFreq, //��ȡ��ǰ����Ƶ��
    (fm_op_func) sFM_GetIntsity, //��ȡ��ǰ�ź�ǿ��
    (fm_op_func) sFM_GetAnten, //�Ƿ�������
    (fm_op_func) sFM_GetStereo, //�Ƿ�������
    (fm_op_func) sFM_Debug, //���Խӿڣ� ���ڷ�����˾������
};

