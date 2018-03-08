/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_power_phy.c
 * \brief    ���ػ���ԴӲ���ӿ�
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

/************************************** ********************/
/*!
 * \par  Description:
 *  config�ȴ�RTCVCC��ļĴ���д��
 * \param[in]    none
 * \param[out]   none
 * \return       non
 * \ingroup      config
 * \note
 ******************************************************/
void config_flush_rtc_reg(void)
{
    uint32 i;

    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//nothing for QAC
    }

    for (i = 0; i < 256 * 4; i++)
    {
        act_writel((act_readl(WD_CTL) | 0x01), WD_CTL);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *  config����Դ�Ƿ����
 * \param[in]    none
 * \param[out]   none
 * \return       TRUE��Դ����FALSEû�е�Դ
 * \ingroup      config
 * \note
 *******************************************************************************/
bool __section__(".rcode") detect_power_on(void)
{
    if((act_readl(CHG_DET) & (1 << CHG_DET_UVLO)) != 0)//��Դ����
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/************************************** ********************/
/*!
 * \par  Description:
 *  configʹ��play���Ļ��ѹ���
 Ĭ���ǵ�Դ����ͳ�������
 * \param[in]    none
 * \param[out]   none
 * \return       non
 * \ingroup      config
 * \note
 ******************************************************/
void en_play_wake_up(bool standby_enter)
{
    uint32 tmp;
    tmp = act_readl(WKEN_CTL);

    tmp |= (1 << WKEN_CTL_SHORT_WKEN) //play�̰�����
    | (1 << WKEN_CTL_LONG_WKEN); //play��������

    if (standby_enter == TRUE)
    {
        tmp |= (1 << WKEN_CTL_BT_WK_EN); //BT����
    }
    else
    {
        tmp &= ~(1 << WKEN_CTL_BT_WK_EN); //��ֹBT����
    }

    act_writel(tmp, WKEN_CTL); //���û��ѷ�ʽ
    config_flush_rtc_reg();


    /** ��pending
    */
    if(((act_readl(INTC_PD) & 0x01) != 0)&&(standby_enter == TRUE))
    {
    }
    else
    {
        act_writel(act_readl(WAKE_PD), WAKE_PD);
    }
}

//����standby S3BT/S4ģʽ��DV5V��alarm���ⲿ�жϣ�ADC_KEY���ѣ�ON_OFF�����ѣ�BT����
//���ٷ��أ�ͨ�����ֻ����ֶν�ϵͳreset
void enter_standby_S3S4(bool standby_enter)
{
    UINT32 i;
    msg_apps_t msg;
    en_play_wake_up(standby_enter);
	
    /** ��֤anti-pop֮ǰIC PA ��ʼ�����
    */
    while (g_app_info_state.inner_pa_inited == FALSE)
    { //�ȴ� IC PA ��ʼ�����
        sys_os_time_dly(1);
    }
    
#if ((CASE_BOARD_TYPE == CASE_BOARD_ATS2823) || (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823))
//ATS2823 �϶�����֧����Ļ��������ʱ�ӵ����󣬽�ʱ�Ӹ�У�͹رգ����͵���
    act_writel(act_readl(RTC_CTL) | (1 << 2), RTC_CTL);
    act_writel(act_readl(HCL_CTL) & (~(1 << 0)), HCL_CTL);
    act_writel(0xA596, RTC_REGUPDATA);
    while (0x5A69 != act_readl(RTC_REGUPDATA))
    {
        ;//delay for RTC update complete
    }
#endif    

    //antipop��ȥ���ػ�����
    disable_pa(g_app_info_state.aout_mode);

    if (standby_enter == TRUE)
    {
        PRINT_INFO("ENTER S3BT");

        //���ý���S3BT��־
        act_writel(act_readl(RTC_BAK0) | (1 << MY_RTC_S3BT), RTC_BAK0);
        config_flush_rtc_reg();
        
        //���ý���S3BT��־
        if((act_readl(INTC_PD) & 0x01) != 0)
        {
            PRINT_INFO("reboot exit S3BT");

            //act_writel(0x1e, WD_CTL); //ʹ��S3BT
            //while(1);
           
             //��PA��PA�����󳣿�
            if (g_app_info_state.aout_mode == AO_SOURCE_DAC)
            {
                uint8 pa_mode = (uint8) com_get_config_default(SETTING_AUDIO_INNER_PA_SET_MODE);
                enable_pa(pa_mode, 0, AO_SOURCE_DAC);
            }
            else if (g_app_info_state.aout_mode == AO_SOURCE_I2S)
            {
#if (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
                //DEMO IC:A0,A1,A2,VRO_S
                enable_pa(3, 0, AO_SOURCE_I2S);
#elif (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2825)
                //DVB:B7,B8,B9,A6
                enable_pa(4, 0, AO_SOURCE_I2S);
#elif (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823)
                //DVB:AOUTL,AOUTR,VRO,VROS
                enable_pa(0, 0, AO_SOURCE_I2S);
#else
                //DEMO IC:A0,A1,A2,A3
                enable_pa(1, 0, AO_SOURCE_I2S);
#endif
            } 
            else
            {
                ;//nothing
            }

            //config ap����ִ��һ�ο�������
            msg.content.data[0] = APP_ID_CONFIG;
            msg.content.data[1] = SWITCH_ON;
            msg.type = MSG_CREAT_APP;
            
            send_async_msg(APP_ID_MANAGER, &msg);
            return;
        }
        else
        {
            //SPEC�� ��Ҫ��ʱ����disable S1
            act_writel(0x03, POWER_CTL); //ʹ��S3BT
            for(i = 0;i < 100;i++)
            {
                ;//delay
            }
            act_writel(0x02, POWER_CTL); //disable S1
        }
    }
    else
    {
        PRINT_INFO("poweroff");
        /** �ػ�֮ǰ��Ҫ�ȴ�RTC�Ĵ���ˢ��.
        */
        while (0x5A69 != act_readl(RTC_REGUPDATA))
        {
            ;//delay for RTC update complete
        }
        act_writel(0x00, POWER_CTL); //disable All�����߽�S4
    }

    //�����ʱ��Ŀ��Ź�����ʱ�䣬������ʱ���ڻ�δ����S3BT��S4,��ϵͳֱ������
    ENABLE_WATCH_DOG(0x07);
    while (1)
    {
        ;//nothing
    }
}

//�Ӹýӿڷ���ʱ�����뱣֤ϵͳ������ģʽ���У���ģ�鶼���������õ�
app_result_e sys_power_off(bool standby_enter)
{
    app_result_e result = RESULT_NULL;

    //�ص�LED����ܣ�LED��
#if (SUPPORT_LED_DRIVER == 1)
    if(g_comval.support_led_display == 1)
    {
        led_clear_screen();
        led_sleep_display();
    }
#endif

#if (SUPPORT_OUTER_CHARGE == 1)
    if ((sys_comval->bat_charge_mode == BAT_CHARGE_MODE_OUTER) && (standby_enter == FALSE))
    {
        if (g_app_info_state.extern_bat_in == 1)
        {
#if (SUPPORT_TK_DRIVER == TK_DRIVER_OUTER)
            key_extern_tk_op(TK_OP_OFF);
#endif
            g_app_info_state.extern_bat_in = 0;
        }
    }
#endif

    while ((act_readl(ONOFF_KEY) & (1 << ONOFF_KEY_ONOFF_PRESS_0)) != 0)
    {
        ;//�ȴ��ſ�
    }

    SPEAKER_OFF();
    SPEAKER_CONTROL_DISABLE();

#if (SUPPORT_OUTER_CHARGE == 1)
    if (sys_comval->bat_charge_mode == BAT_CHARGE_MODE_OUTER)
    {
        enter_standby_S3S4(standby_enter);
    }
    else
#endif
    {
        if (detect_power_on() == TRUE) //��Դ���룬����ػ����
        {
            result = config_charging(SWITCH_OFF);
            if (result == RESULT_POWER_OFF)
            {
                //���S3BT���룬��ô��ʹ�����⵽�ػ�����ֵ����Ȼ����S3BT������
                //����ִ�У���ֹ�������ػ���������ִ�У�����controller���ӶϿ���
                //���
                //standby_enter = FALSE;
                goto enter_power_off;
            }
        }
        else//3�֡��ػ���ģʽѡ��һ��
        {
            enter_power_off: enter_standby_S3S4(standby_enter);
        }
    }

    SPEAKER_CONTROL_ENABLE();
    SPEAKER_ON();

    //����LED�����
#if (SUPPORT_LED_DRIVER == 1)
    if(g_comval.support_led_display == 1)
    {
        led_wake_up_display();
        led_clear_screen();
        led_display(0, NUM_ERROR, 1);
        led_display(1, NUM_ERROR, 1);
        led_display(2, NUM_ERROR, 1);
        led_display(3, NUM_ERROR, 1);
    }
#endif

    return result;
}

