/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*     brief    led��ʾ����AP����
*      <author>       <time>           
*       Wekan       2015-3-26pm            
*******************************************************************************/


#include "led_inner.h"
uint32 bak_seg_sreen_data0;
uint32 bak_seg_sreen_data1;

/******************************************************************************
* \par  Description: ��˸��Ļ
* \param[in]   onoff --1��ʾ��ʾԭ���ݣ�0��ʾ���ԭ����
* \param[out]  none
* \return      none
* \note:��˸�������ȵ��ùرգ�Ȼ�������ʾԭ����
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

void sdrv_flash_screen(uint8 onoff, void *null2, void *null3)
{
    if (0 == onoff)
    {
        bak_seg_sreen_data0 = act_readl(SEG_SREEN_DATA0);
        bak_seg_sreen_data1 = (act_readl(SEG_SREEN_DATA1));
        reg_writel(0, SEG_SREEN_DATA0);
        reg_writel(0, SEG_SREEN_DATA1);
    }
    else
    {
        reg_writel((act_readl(SEG_SREEN_DATA0) |(uint32)bak_seg_sreen_data0), SEG_SREEN_DATA0);
        reg_writel((act_readl(SEG_SREEN_DATA1) |(uint32)bak_seg_sreen_data1), SEG_SREEN_DATA1);
    }
    //wait for data trans end
    wait_for_trans();
}

