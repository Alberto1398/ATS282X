/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_manager_test.h"
#include "ap_autotest_channel_test.h"
#include "fm_interface.h"
#include "ap_autotest_channel_test_SNR.h"


/******************************************************************************/
/*!
 * \par  Description:
 *   ��ָ�����ȵĶ����������Ӧ��λ����ֵ
 * \param[in]  N: Ҫ�����ֵ
 * \param[in]  L��bit��
 * \param[out] none  
 * \return     N��λ����ֵ
 * \par
 * \note
 * \li   ���������ȫ�ֱ���BitRev������
 *******************************************************************************/
LEN_TYPE DoBitRev(LEN_TYPE N, LEN_TYPE L)
{
    LEN_TYPE Temp1, Temp2;
    LEN_TYPE i;
    for (i = 0; i < (L / 2); i++)
    {
        Temp1 = 0;
        Temp2 = 0;
        if ((N & (1 << i)) != 0)
        {
            Temp1 = 1;
        }
        
        if ((N & (1 << (L - 1 - i))) != 0)
        {
            Temp2 = 1;
        }
        
        N &= ~(1 << i);
        N &= ~(1 << (L - 1 - i));
        N |= (Temp2 << i);
        N |= (Temp1 << (L - 1 - i));
    }
    return N;
}

/******************************************************************************/
/*!
 * \par  Description:
 *   ��ʼ��λ����ֵ���飬��FFT����ʱ�����
 * \param[in]  none
 * \param[out] none  
 * \return     none
 * \par
 * \note
 * \li   ���������ȫ�ֱ���BitRev������
 *******************************************************************************/
void InitBitRev(void)
{
    LEN_TYPE i;
    for (i = 0; i < LENGTH; i++) //��λ��ת
    {
        BitRev[i] = DoBitRev(i, bL);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *   ���������ݽ��е�����
 * \param[in]  pIn �������ݵĻ�����
 * \param[out] pIn �������ݵĻ�����  
 * \return     none
 * \par
 * \note
 * \li   �ڵ���FftExe֮ǰӦ�õ��øú��������ݽ��е�����
 *******************************************************************************/
void FftInput(IN_TYPE *pIn)
{
    LEN_TYPE i;
    IN_TYPE Temp;
    for (i = 0; i < LENGTH; i++)
    {
        if (BitRev[i] > i) //�����λ���ԭ����򽻻�
        {
            Temp = pIn[i];
            pIn[i] = pIn[BitRev[i]];
            pIn[BitRev[i]] = Temp;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *   ����FFT����
 * \param[in]  pIn �������ݵĻ�����
 * \param[out] pRe ����ʵ���Ļ����� 
 * \param[out] pIm �����鲿�Ļ�����  
 * \return     none
 * \par
 * \note
 * \li   �ڵ��øú���֮ǰӦ�õ���FftInput���������ݽ��е�����
 *******************************************************************************/
void FftExe(IN_TYPE *pIn, OUT_TYPE *pRe, OUT_TYPE *pIm)
{
    LEN_TYPE i, j;
    LEN_TYPE BlockSize;
    OUT_TYPE tr, ti;
    LEN_TYPE OffSet1, OffSet2;

#ifdef USE_TABLE
    LEN_TYPE OffSet0;
#endif

    long c, s;

    //�ȼ���2���
    for (j = 0; j < LENGTH; j += 2)
    {
        tr = pIn[j + 1];
        pRe[j + 1] = (pIn[j] - tr);
        pIm[j + 1] = 0;
        pRe[j] = (pIn[j] + tr);
        pIm[j] = 0;
    }

    for (BlockSize = 4; BlockSize <= LENGTH; BlockSize <<= 1) //��һ������
    {
        for (j = 0; j < LENGTH; j += BlockSize)
        {
            for (i = 0; i < (BlockSize / 2); i++)
            {
#ifndef USE_TABLE
                c=(long)(1024*cos(2*PI*i/BlockSize));
                s=(long)(1024*sin(2*PI*i/BlockSize));
#else
                OffSet0 = LENGTH / BlockSize * i;
                c = COS_TABLE[OffSet0];
                s = SIN_TABLE[OffSet0];
#endif

                OffSet1 = i + j;
                OffSet2 = OffSet1 + BlockSize / 2;
                tr = (OUT_TYPE)((c * pRe[OffSet2] + s * pIm[OffSet2]) / 1024);
                ti = (OUT_TYPE)((c * pIm[OffSet2] - s * pRe[OffSet2]) / 1024);
#ifdef UNITARY  //���Ҫ�Խ����һ��������ÿ������Ҫ����2
                pRe[OffSet2]=(pRe[OffSet1]-tr)/2;
                pIm[OffSet2]=(pIm[OffSet1]-ti)/2;
                pRe[OffSet1]=(pRe[OffSet1]+tr)/2;
                pIm[OffSet1]=(pIm[OffSet1]+ti)/2;
#else
                pRe[OffSet2] = (pRe[OffSet1] - tr);
                pIm[OffSet2] = (pIm[OffSet1] - ti);
                pRe[OffSet1] = (pRe[OffSet1] + tr);
                pIm[OffSet1] = (pIm[OffSet1] + ti);
#endif
            }
        }
    }
#ifdef UNITARY
    pRe[0]/=2;
    pIm[0]/=2;
#endif
}

/*****************************************/    
/*Function: �����Ŵ���                                */    
/*��ڲ���������������32λ����           */    
/*���ڲ��������������16λ����           */    
/****************************************/    
unsigned int sqrt_fixed(unsigned long a)   
{   
    unsigned long i,c;   
    unsigned long b = 0;   
    for(i = 0x40000000; i != 0; i >>= 2)   
    {   
        c = i + b;   
        b >>= 1;   
        if(c <= a)   
        {   
            a -= c;   
            b += i;   
        }   
    }   
    return (unsigned int)b;   
}

static uint32 libc_abs(int32 value)
{
    if (value > 0)
    {
        return value;
    }
    else
    {
        return (0 - value);
    }
}

__int64 mul64(__int64 val0, __int64 val1)
{
    __int64 temp_val = val0 * val1;

    return temp_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 *   �Բɼ�����Ƶ���ݽ��м򵥵�Ƶ�׷���
 * \param[in] pdata ��Ƶ���ݻ�����
 * \param[in] sample_rate ����Ƶ�� KHZ 
 * \return    ���������Ƿ�����Ƶ������
 * \par
 * \note
 * \li   Ҫ������ԴΪ�̶���1KHZ���Ҳ��������޿������㣬���ܽ��з�ֵ������������
         ʵ�����ݵ����������ǿ��Դ��·�����Ӧ����������
 *******************************************************************************/
uint8 analyse_sound_data(IN_TYPE *pdata, uint16 sample_rate, uint32 *p_snr, uint32 *max_point)
{
    uint8 i;
    int16 max_value = 0;
    //uint32 *pRe = &Re;   

    uint32 total_value = 0;
    uint32 snr;
    
    uint8 Sig_Max_Point;
    
    __int64 tmp_sum,  tmp1, tmp2,Sig_Max, Noise, Sig;

    int AmpMax; //�źŷ��������ֵ
    int AmpMin; //�źŷ��ȸ����ֵ    
    
    //��������Ƶ��λ��
    uint8 mean_index = (uint8)(LENGTH / sample_rate);

    AmpMax = pdata[0];
    AmpMin = pdata[0];

    //�ҵ�ʱ������ֵ����Сֵ
    for (i = 0; i < LENGTH; i++)
    {
        if (AmpMax < pdata[i])
            AmpMax = pdata[i];
        if (AmpMin > pdata[i])
            AmpMin = pdata[i];
    }
    
    load_win_data();
          
    //��ʼ����������
    InitBitRev();
    
    //��������
    FftInput(pdata); 
    
    //�����н���fft�任
    FftExe(pdata, Re, Im);   

    Sig_Max = 0;
    tmp_sum = 0;

    //������һ��ֱ����������FFT���жԳ����ԣ�����һ������
    for (i = 1; i < (LENGTH / 2); i++)
    {        
        Re[i] = libc_abs(Re[i]);

        Im[i] = libc_abs(Im[i]);
        
        //libc_print("Re0", Re[i], 2);
        
        //libc_print("Im0", Im[i], 2);

        //ע����������64λ�ĳ˷�������ֱ����32λ�˷����������ݻ����
        tmp1 = mul64(Re[i], Re[i]);
		
		tmp2 = mul64(Im[i], Im[i]);

		tmp1 += tmp2;
        
        if (Sig_Max < tmp1)
        {
            Sig_Max = tmp1;
            Sig_Max_Point = i;
        }   
        
        tmp_sum += tmp1;     

        //�������ݴ�ӡ������������
        //libc_print("Re1", tmp1, 2);

        //libc_print("\n", 0, 0);
    }
    
    libc_print("Sig max point", Sig_Max_Point, 2);
    
   //�ҵ��ź��������㼰����,ֻ��һ��

    Sig = Sig_Max; //����ź����߸�N_sig-1��

    for (i = 1; i < 5; i++)
    {
        tmp1 = mul64(Re[Sig_Max_Point - i], Re[Sig_Max_Point - i]) + mul64(Im[Sig_Max_Point - i], Im[Sig_Max_Point - i]);
        
        tmp2 = mul64(Re[Sig_Max_Point + i], Re[Sig_Max_Point + i])+ mul64(Im[Sig_Max_Point + i], Im[Sig_Max_Point + i]);

        Sig = Sig + tmp1 + tmp2;
    }

    Noise = tmp_sum - Sig;

    //libc_print("Sig", Sig, 2);

    //libc_print("Total", tmp_sum, 2);

    //libc_print("Noise", Noise, 2);
    
    snr = (int) ((((int) (Sig >> 15)) / ((int) (Noise >> 10))) << 5);//δȡlog

    libc_print("SNR", snr, 2);

    *p_snr = snr;

    *max_point = Sig_Max_Point;
    
    return 0;
}

uint32 cal_threadshold(uint32 threadshold_db)
{
    uint32 i;
    uint32 tmp_val;
    uint32 shift_cnt = threadshold_db / 10;

    tmp_val = 1;
    for(i = 0; i < shift_cnt; i++)
    {
        tmp_val *= 10;
    }

    return tmp_val;
}


uint32 thd_test(void *buffer_addr, channel_test_arg_t *channel_test_arg)
{
    int32 i;

    uint32 snr;
    uint32 max_point;

    uint32 ret_val = TRUE;

    IN_TYPE *p_data_buffer = (IN_TYPE *)buffer_addr;

    //read_temp_file(1, LINEIN_SOUND_DATA_ADDR, SOUND_DATA_LEN);

    //������������
    for(i = 0; i < (LENGTH * 2); i++)
    {
        p_data_buffer[i + (LENGTH * 2)] = p_data_buffer[i * 2 + 1];
        p_data_buffer[i] = p_data_buffer[i * 2];
    }   

    if(channel_test_arg->test_left_ch == TRUE)
    {
        if(channel_test_arg->test_left_ch_SNR == TRUE)
        {
            analyse_sound_data(p_data_buffer, ADC_SAMPLE_RATE, &snr, &max_point);

            if(snr >= cal_threadshold(channel_test_arg->left_ch_SNR_threadshold) 
                && channel_test_arg->left_ch_max_sig_point == max_point)
            {
                ret_val = TRUE;
            }
            else
            {
                ret_val = FALSE;
            }

            channel_test_arg->left_ch_SNR_threadshold = snr;
            channel_test_arg->left_ch_max_sig_point = max_point;

            if(g_test_mode == TEST_MODE_CARD)
            {
                DEBUG_ATT_PRINT("left SNR:", snr, 2);
                DEBUG_ATT_PRINT("left Max Sig Point:", max_point, 2);
            }
        }
        else
        {
            channel_test_arg->left_ch_SNR_threadshold = 0;
            channel_test_arg->left_ch_max_sig_point = 0;
        }
    }


    if(channel_test_arg->test_right_ch == TRUE)
    {
        if(channel_test_arg->test_right_ch_SNR == TRUE)
        {
            analyse_sound_data(&p_data_buffer[(LENGTH * 2)], ADC_SAMPLE_RATE, &snr, &max_point);

            if(snr >= cal_threadshold(channel_test_arg->right_ch_SNR_threadshold) 
                && channel_test_arg->right_ch_max_sig_point == max_point)
            {
                if(ret_val == TRUE)
                {
                    ret_val = TRUE;
                }
            }
            else
            {
                ret_val = FALSE;
            }

            channel_test_arg->right_ch_SNR_threadshold = snr;
            channel_test_arg->right_ch_max_sig_point = max_point;

            if(g_test_mode == TEST_MODE_CARD)
            {
                DEBUG_ATT_PRINT("right SNR:", snr, 2);
                DEBUG_ATT_PRINT("right Max Sig Point:", max_point, 2);
            }            
        }
        else
        {
            channel_test_arg->right_ch_SNR_threadshold = 0;
            channel_test_arg->right_ch_max_sig_point = 0;
        }   
    }
    
    return ret_val;
}
