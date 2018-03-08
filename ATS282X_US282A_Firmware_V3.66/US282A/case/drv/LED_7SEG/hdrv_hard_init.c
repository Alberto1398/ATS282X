
/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*
*      <author>       <time>           
*       Wekan       2015-3-26pm            
*******************************************************************************/


#include "led_inner.h"


bool Led_sleep_falg = TRUE;


/******************************************************************************
* \par  Description: ������ʾ��ؼĴ���
* \param[in]   none
* \param[out]  none
* \return      none
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/
void hdrv_set_mfp_to_emif(void)
{
    //set sgement
    reg_writel((act_readl(MFP_CTL0) & (uint32)0xfffff800), MFP_CTL0); //A15/A14/A13/A12/A11
    reg_writel((act_readl(MFP_CTL1) & (uint32)0x007fffff), MFP_CTL1); //A10/A9/A8

    //driver cabability
    reg_writel(((act_readl(PADDRV0) &(uint32)0x0000ffff)), PADDRV0); //A15/A14/A12/A11/A10/A9/A8 drv

    //selcet high osc   
    reg_writel((act_readl(CMU_SEGLCDCLK)|(uint32)0x04), CMU_SEGLCDCLK); //LCD clock div 
    
    //set current
    reg_writel((act_readl(LED_SEG_BIAS_EN)|(uint32)0x03), LED_SEG_BIAS_EN);  //4mA
    
    //selcet hosc
    reg_writel((act_readl(CMU_SEGLCDCLK)&(uint32)0xfffffff0), CMU_SEGLCDCLK); //LCD clock div
    //4��Ƶ--ע����ʾ1��Ƶ
    // reg_writel((act_readl(CMU_SEGLCDCLK)|(uint32)0x03), CMU_SEGLCDCLK); //LCD clock div

}

/******************************************************************************
* \par  Description: ����led��ʾ����
* \param[in]   none
* \param[out]  none
* \return      none
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/
void hdrv_set_led(void)
{
    //����������ʱ�� bit18
    reg_writel((act_readl(CMU_DEVCLKEN) | (uint32)0x040000), CMU_DEVCLKEN);
    //���ÿ�����Ϊ��������ģʽ bit 14
    reg_writel((act_readl(MRCR) | (uint32)0x4000), MRCR);

    //����Ϊ7matrixģʽ��������ʾ����
    reg_writel((act_readl(SEG_SREEN_CTL) | (uint32)0xac), SEG_SREEN_CTL);
    Led_sleep_falg = FALSE;//תΪ������״̬
}


/******************************************************************************
* \par  Description: ��ʼ��LED��Ӳ���豸
* \param[in]   none
* \param[out]  none
* \return      none
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

void hdrv_hard_init(void)
{
    //led controller init
    hdrv_set_led();
    //select led segment and com
    hdrv_set_mfp_to_emif();
}


/******************************************************************************
* \par  Description: ���ػ���ǰ��״̬
* \param[in]   ����û������
* \param[out]  none
* \return      FALSE����(������)��TRUE��ʾ(���軽��)
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

uint8 sdrv_wake_up_LED_display(uint8 a,uint8 b,uint8 c)
{
    if(Led_sleep_falg != 0)
    {
        hdrv_set_led();
        return 0;
    }
    return 1;
}

/******************************************************************************
* \par  Description: ʹled��ʾ��������״̬
* \param[in]   ����û������
* \param[out]  none
* \return    
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

void sdrv_sleep_LED_display(uint8 a,uint8 b,uint8 c)
{
    
    Led_sleep_falg = TRUE;
    //����Ϊ��ģʽ
    reg_writel((act_readl(SEG_SREEN_CTL) & (uint32)(~0xA0)), SEG_SREEN_CTL);
    //�ؿ�����ʱ�� bit18
    reg_writel((act_readl(CMU_DEVCLKEN) & (uint32)(~0x040000)), CMU_DEVCLKEN);
}


