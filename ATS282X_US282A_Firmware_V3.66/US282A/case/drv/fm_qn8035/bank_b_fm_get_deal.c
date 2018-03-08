#include "fm_drv.h"
#include "fm_qn8035.h"

//��ȡ��ǰ����״̬
int sFM_GetAnten(void* antenna, void* null2, void* null3)
{
    uint8* p_antenna;
    unsigned char adcdat;

    p_antenna = (uint8*) antenna;

    //Demo ��LRADC1 ���ж������
    adcdat = act_readb(LRADC1_DATA) & 0x3f;
    //����������
    if ((adcdat >= 0x2B) && (adcdat < 0x34))
    {
        *p_antenna = 1;
    }
    else
    {
        *p_antenna = 0;
    }
    //��ȡ״̬�ɹ�
    return 0;
}

