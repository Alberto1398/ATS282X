

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
 * \par  Description: LED��ʼ��װ��
 * \param[in]   none
 * \param[out]  none
 * \return      ��ʼ���Ƿ�ɹ�
 * \retval      1: ��ʼ���ɹ�
 * \retval      0: ��ʼ���ɹ�  
 *   <author>    <time>     
 *    Wekan   2015-3-26pm  
 *******************************************************************************/

int led_init(void *null0, void *null1, void *null2)
{
    hdrv_hard_init();
    return 0;
}


/******************************************************************************
 * \par  Description:LEDж��
 * \param[in]   none
 * \param[out]  none
 * \return      ж���Ƿ�ɹ�
 * \retval      1: ��ʼ���ɹ�
 * \retval      0: ��ʼ���ɹ�
 *   <author>    <time>     
 *    Wekan   2015-3-26pm  
 *******************************************************************************/
int led_exit(void *null0, void *null1, void *null2)
{
    return 0;
}

module_init(led_init)
module_exit(led_exit)

