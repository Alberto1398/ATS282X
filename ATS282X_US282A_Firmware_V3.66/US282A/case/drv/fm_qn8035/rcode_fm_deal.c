#include "fm_drv.h"
#include "fm_qn8035.h"

//�������ݵ�ȫ�ֱ���
set_value_t g_set_value;

FM_init_parm_t fm_init_parm;

uint8 qnd_xtal_div0, qnd_xtal_div1, qnd_xtal_div2;


//QN8035ģ��bug��855���ò��ɹ�
uint8 g_freq_855;
//����Ƶ��
//freq: �����Ƶ��ֵ(��λ:kHz)
//����ֵ: 0->�ɹ�, -1->ʧ��
int sFM_SetFreq(uint32 freq, void* null2, void* null3)
{
    PRINTD_DBG_INT("SetFreq:", freq);
    QND_TuneToCH((uint16) (freq / 10));
    return 0;
}

//���ò���
int __section__(".bank") sFM_SetBand(radio_band_e band, void* null2, void* null3)
{
    //int result;
    //PRINTD_DBG_INT("FM_SetBand=", band);
    if (band == Band_Japan)
    {
        //�ձ�����
        g_set_value.band = 1;
        //����
        g_set_value.span = 100;
        g_set_value.step = QND_FSTEP_100KHZ;
        //���Ƶ��
        g_set_value.freq_low = 76000;
        //���Ƶ��
        g_set_value.freq_high = 90000;

    }
    else if (band == Band_Europe)
    {
        //ŷ��Ƶ�β���50
        g_set_value.band = 0;
        //����
        g_set_value.span = 50;
        g_set_value.step = QND_FSTEP_50KHZ;
        //���Ƶ��
        g_set_value.freq_low = 87500;
        //���Ƶ��
        g_set_value.freq_high = 108000;
    }
    else
    {
        //��ͨƵ�μ�����
        g_set_value.band = 0;
        //����
        g_set_value.span = 100;
        g_set_value.step = QND_FSTEP_100KHZ;
        //���Ƶ��
        g_set_value.freq_low = 87500;
        //���Ƶ��
        g_set_value.freq_high = 108000;
    }

    return 0;
}

//����/�������
int __section__(".bank") sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
{
    QNF_SetMute(mode);
    return 0;
}

//��������
int __section__(".bank") sFM_SetThrod(uint8 level, void* null2, void* null3)
{
    //��������
    QND_RXSetTH(level);
    return 0;
}

//�����̨���ж��Ƿ���Ч��̨
//freq: �����Ƶ��ֵ
//direct: ��̨����
//����ֵ��  -1:û�ź�
//          0: ���ź�
//          1: ������
int sFM_Search(uint32 freq, uint8 direct, void* null3)
{
#if(HARD_SEEK == 0)
    int result;

    PRINTD_DBG_INT("Ƶ��(kHz):", freq);
    result = QND_RXValidCH(freq / 10, g_set_value.step);
    if(result == 1)
    {
        return 0;
    }
#endif

    return -1;

}

//����Ӳ����̨
//freq: �����Ƶ��ֵ
int sFM_HardSeek(uint32 freq, uint8 direct, void* null3)
{
#if(HARD_SEEK != 0)
    uint16 val;
    if(direct != 0)
    {
        val = g_set_value.freq_high / 10;
    }
    else
    {
        val =g_set_value.freq_low / 10;
    }
    QND_RXSeekCH((uint16) (freq / 10), val, g_set_value.step);
#endif
    return 0;
}

//Ӳ��seek �����У��ֶ�ֹͣseek ����
int sFM_BreakSeek(void* null1, void* null2, void* null3)
{
#if(HARD_SEEK != 0)

#endif
    return 0;
}

//��ȡӲ��seek �����Ƿ������־
int sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
{
#if(HARD_SEEK != 0)
    uint8* pend = (uint8 *) flag;
    uint8 regValue = QND_ReadReg(SYSTEM1);
    //if it seeks a potential channel, the loop will be quited
    if ((regValue & CHSC) == 0)
    {
        //������
        *pend = 1;
    }
    else
    {
        *pend = 0;
    }

    if (*pend != 0)
    {
        uint8 snr = QND_ReadReg(SNR);
        uint8 rssi = QND_ReadReg(RSSISIG);
        uint8 failed = (QND_ReadReg(STATUS1) & RXCCA_FAIL);

        PRINTD_DBG_INT("locked:", QNF_GetCh() * 10);
        PRINTD_DBG_INT("", (failed << 16) | (snr << 8) | rssi);

        if((snr > 21) && (rssi > 69) && (failed == 0))
        {
            *pend |= (1 << 1);
        }
    }
#endif
    return 0;
}

//��ȡ��ǰƵ��
int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
{
    Engine_Status_t * ptr_buf = (Engine_Status_t *) pstruct_buf;

    ptr_buf->FM_CurrentFreq = QNF_GetCh() * 10;

    return 0;
}

//��ȡ��ǰ������Ϣ
int __section__(".bank") sFM_GetBand(void* band, void* null2, void* null3)
{
    radio_band_e* bandinfo;
    int result = 0;

    bandinfo = (radio_band_e*) band;

    //����ȥȡ��ֱ�Ӵ�������ȫ�ֱ�����ȡ��
    //�й�/ ������̨
    //Ƶ��
    if (g_set_value.band == 1)
    {
        *bandinfo = Band_Japan;
    }
    else if ((g_set_value.band == 0) && (g_set_value.span == 50))
    {
        *bandinfo = Band_Europe;
    }
    else
    {
        *bandinfo = Band_China_US;
    }
    return result;

}

//��ȡ��ǰƵ��
int sFM_GetFreq(void* freq, void* null2, void* null3)
{
    uint32* point_freq = (uint32*) freq;
    *point_freq = QNF_GetCh() * 10;
    if(g_freq_855)
    {
        //g_freq_855 = 0;
        *point_freq = 85500;
    }
    return 0;
}

//��ȡ��ǰ��̨�ź�ǿ��
int __section__(".bank") sFM_GetIntsity(void* intensity, void* null2, void* null3)
{
    return 0;
}

//��ȡ��ǰ��̨��������Ϣ
int __section__(".bank") sFM_GetStereo(void* stereo, void* null2, void* null3)
{
    return 0;
}

int __section__(".bank") sFM_Debug(void* null1, void* null2, void* null3)
{
    PRINTD_DBG_INT("SNR = ", (uint32)QND_ReadReg(SNR));
    PRINTD_DBG_INT("RSSISIG = ", (uint32)QND_ReadReg(RSSISIG));
    return 0;
}
