

/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*
*      <author>       <time>           
*       Wekan       2015-3-26pm            
*******************************************************************************/


#include "led_inner.h"

/******************************************************************************
* \par  Description: �ȴ�led�������
* \param[in]   none
* \param[out]  none
* \return      none
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

void wait_for_trans(void)
{
    reg_writel((act_readl(SEG_SREEN_CTL) | (uint32)0x10), SEG_SREEN_CTL);
    while(1)
    {
        if ((act_readl(SEG_SREEN_CTL) & (uint32)0x10) == 0)
        {
            break;
        }
        if(Led_sleep_falg != 0)//��������
        {   
            break;
        }
    } 
}

/******************************************************************************
* \par  Description: �������led������
* \param[in]   dis_val_ptr ��ʾ����ָ��
* \param[out]  none
* \return      none
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

void hdrv_data_send(uint32* dis_val_ptr)
{
    uint32 mask_val = 0;
    uint32 dis_val;
    //send data to led data register
    dis_val = *dis_val_ptr;
    //first 4 byte
    reg_writel(((act_readl(SEG_SREEN_DATA0) & mask_val) | (uint32) dis_val), SEG_SREEN_DATA0);

    dis_val_ptr++;
    //left fouth byte
    dis_val = *dis_val_ptr;

    reg_writel(((act_readl(SEG_SREEN_DATA1) & mask_val )| (uint32)dis_val), SEG_SREEN_DATA1);

    //wait for data trans end
    wait_for_trans();
}


