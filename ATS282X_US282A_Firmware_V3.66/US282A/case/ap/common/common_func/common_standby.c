/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ����S2����ģʽ���У�������޶Ƚ��͹��ģ�����ģʽ�£�����ѡ����ַ�ʽ
 *       ���ѣ����磺������������ң�����������������������ⲿ�豸�β壬��ʱ���ӣ�
 *       �ȵȣ����ĺ�ϵͳ����״������Ϊ���ѷ�ʽ֧�ֶ���������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"

#define KEY_VAL_DATA_MAX   0x62

static int switch_low_freq(void);
static int enter_low_power_mode(int low_freq_time);
static int switch_high_freq(void);
static int exit_low_power_mode(int low_freq_time);
static int wait_wake_event(int low_freq_time, uint32 wake_type);
static void com_standby_enter_cb(void);
static void com_standby_exit_cb(void);

/* �κ�Ӧ�ÿ���ʱ�����Խ���S2����ģʽ��ǰ̨���������������״̬��Ȼ�����͹���ģʽ��
 * ���У������м����DSP���Ѿ�ж�ص��������Ѿ�����8����û�����ݹ�����ǰ̨���ڿ���״̬
 *       ���Ѿ�����7����û�з����κ��¼�
 * ǰ���������ɵ�ع��磬�������USB�߻����ߣ���ô�Ͳ�����S2����ģʽ
 * ����ģʽ�����ֻ��ѷ�ʽ�����ϵͳ���л�����Ҫ�����ͷ�ļ�����
 *           ���ж�ʱ���ӣ�USB�߲��룬��������߲���Ĭ��֧��
 */
app_result_e com_standby(uint32 wake_type)
{
    app_result_e result = RESULT_NULL;
    int ret_val = 0;
    int low_freq_time = -1; //��Ƶ�������ڣ���λ1ms��-1��ʾһֱ��Ƶ��0��ʾһֱ��Ƶ��
    os_event_t *standby_sem;
    engine_type_e engine_type = get_engine_type();
    uint32 irq_save;

    g_low_power_var.high_freq_flag = 1;

    //��ʱ��ܽ����������ż�������bug��һֱ�ܸ�Ƶ
#if 1
    low_freq_time = 0;
#endif

    //step 1:���ݻ���ģʽȷ������ʱϵͳ���з�ʽ����Ƶ����ģʽʱЧ�ԣ�
    if ((wake_type & WAKE_TYPE_IR_KEY) != 0)
    {
        low_freq_time = 0;
    }

    if (low_freq_time == 0)
    {
        if ((wake_type & WAKE_TYPE_I2C_KEY) != 0)
        {
            g_low_power_var.i2c_key_timer = 20;
            g_low_power_var.card_detect_timer = 200;
        }
#if (SUPPORT_CARD_DETECT == DETECT_BY_CMD)
        else if ((wake_type & WAKE_TYPE_CARD_DETECT) != 0)
        {
            g_low_power_var.card_detect_timer = 200;
        }
#endif
        else
        {
            ; //nothing
        }
    }
    else
    {
        if ((wake_type & WAKE_TYPE_I2C_KEY) != 0)
        {
            low_freq_time = 100;
            g_low_power_var.i2c_key_timer = 1;
            g_low_power_var.card_detect_timer = 5;
        }
#if (SUPPORT_CARD_DETECT == DETECT_BY_CMD)
        else if ((wake_type & WAKE_TYPE_CARD_DETECT) != 0)
        {
            low_freq_time = 500;
            g_low_power_var.card_detect_timer = 1;
        }
#endif
        else
        {
            ; //nothing
        }
    }

    //step 2:����Ӧ�û��������������˯��״̬
    com_standby_enter_cb();
    if (g_standby_enter_cb != NULL)
    {
        g_standby_enter_cb();
    }

    if (engine_type != ENGINE_NULL)
    {
        msg_apps_t msg;

        libc_sem_init(&standby_sem, 0);

        msg.type = MSG_ENGINE_STANDBY_SYNC;
        msg.content.addr = standby_sem;

        send_sync_msg(APP_ID_THEENGINE, &msg, NULL, 0);
    }

    irq_save = sys_local_irq_save();

    //step 3:����͹�������ģʽ
    enter_low_power_mode(low_freq_time);

    //step 4:���Ѽ��
    ret_val = wait_wake_event(low_freq_time, wake_type);

    //step 5:�˳��͹�������ģʽ
    exit_low_power_mode(low_freq_time);

    PRINT_INFO_INT("wake ret:", ret_val);

    sys_local_irq_restore(irq_save);

    //step 6:�������˳�˯��ģʽ���ָ�Ӧ�û���
    if (engine_type != ENGINE_NULL)
    {
        libc_sem_post(standby_sem);
        libc_sem_destroy(&standby_sem);
    }

    //���Ƶ��
    g_sys_counter.ui_idle_counter = 0;
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL7, FREQ_NULL);
    g_app_info_state_all.ui_status = UI_STATUS_IDLE;

    sys_reset_timer();
    com_standby_exit_cb();
    if (g_standby_exit_cb != NULL)
    {
        g_standby_exit_cb();
    }

    g_ap_switch_var.wake_from_s2 = TRUE;
    g_ap_switch_var.wake_ret_from_s2 = (uint8)ret_val;

    return result;
}

//ע�������˳�STANDBY��Ӧ�ûص�����
void com_standby_register(standby_enter_hook enter_cb, standby_exit_hook exit_cb)
{
    g_standby_enter_cb = enter_cb;
    g_standby_exit_cb = exit_cb;
}

static void delay_ms_32k(int ms)
{
    int count = ms * 5;

    while (count != 0)
    {
        count--;
    }
}

static void delay_ms_13m(int ms)
{
    int count = ms * 13000 * 12;

    while (count != 0)
    {
        count--;
    }
}

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
static void flush_rtc_reg(void)
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

static int switch_low_freq(void)
{
    if (g_low_power_var.high_freq_flag == 0)
    {
        return 0;
    }

    g_low_power_var.bak_24MPLL_CTL = act_readl(_24MPLL_CTL);
    g_low_power_var.bak_CORE_PLL_CTL = act_readl(CORE_PLL_CTL);
    g_low_power_var.bak_CMU_SYSCLK = act_readl(CMU_SYSCLK);

    act_writel(0, CMU_SYSCLK); //ѡ���Ƶ
    act_writel(0, _24MPLL_CTL); //�ص���Ƶ
    act_writel(0, CORE_PLL_CTL); //��COREPLL

    g_low_power_var.high_freq_flag = 0;

    return 0;
}

static int enter_low_power_mode(int low_freq_time)
{
    PRINT_INFO("enter_s2");

    g_low_power_var.bak_DAC_ANACTL = act_readl(DAC_ANACTL);
    g_low_power_var.bak_AUDIO_PLL_CTL = act_readl(AUDIO_PLL_CTL);

    act_writel(0, DAC_ANACTL);
    act_writel(0, AUDIO_PLL_CTL);

    if (low_freq_time != 0)
    {
        g_low_power_var.high_freq_flag = 1;
        switch_low_freq();
    }

    return 0;
}

static int switch_high_freq(void)
{
    if (g_low_power_var.high_freq_flag == 1)
    {
        return 0;
    }

    act_writel(0x8, _24MPLL_CTL);
    delay_ms_32k(3);//��Ƶ�ȶ���ʱ
    act_writel(g_low_power_var.bak_24MPLL_CTL, _24MPLL_CTL);
    act_writel(g_low_power_var.bak_CORE_PLL_CTL, CORE_PLL_CTL);
    act_writel(g_low_power_var.bak_CMU_SYSCLK, CMU_SYSCLK);

    g_low_power_var.high_freq_flag = 1;

    return 0;
}

static int exit_low_power_mode(int low_freq_time)
{
    if (low_freq_time != 0)
    {
        switch_high_freq();
    }

    act_writel(g_low_power_var.bak_AUDIO_PLL_CTL, AUDIO_PLL_CTL);
    act_writel(g_low_power_var.bak_DAC_ANACTL, DAC_ANACTL);

    PRINT_INFO("exit_s2");

    return 0;
}

//Ϊ�˵�Ƶ�¼�ʱƫ��Сһ�㣬��Ƶģʽ�����еĴ��뾡�����⺯������
//�������һֱ�����ڸ�Ƶ����ôͬ����Ҫ��ʱ����ΪĳЩ�ӿڲ���ִ�е�ԽƵ��Խ�ã�����TK���ӿ�
static int wait_wake_event(int32 low_freq_time, uint32 wake_type)
{
    int wake_ret = 0;
    int32 low_freq_cycle; //��Ƶ+��Ƶ�л��µ�Ƶ����ʱ��
    int32 low_freq_cycle_per_while; //whileѭ��û���κ��¼�����������
    uint8 i2c_key_counter = 0;
    uint8 card_detect_counter = 0;

    if (low_freq_time > 0)
    {
        low_freq_cycle = low_freq_time*32;
    }

    low_freq_cycle_per_while = 75; //ͨ��LST��������

    while ((act_readl(WAKE_PD) & (1 << WAKE_PD_BT_PD)) != 0)
    {
        act_writel(act_readl(WAKE_PD), WAKE_PD); //��pending
    }

    while (1)
    {
        //alarm����
        if ((act_readl(RTC_CTL) & 0x01) != 0)
        {
            //��pending
            wake_ret = WAKE_BY_ALARM;
            break;
        }

#if (SUPPORT_OUTER_CHARGE == 1)
        if (sys_comval->bat_charge_mode == BAT_CHARGE_MODE_OUTER)
        {
            //����9V����߻���
            if (IS_EXTERN_CAHREGE_IN() == 1)
            {
                wake_ret = WAKE_BY_USB;
                break;
            }
        }
        else
#endif
        {
            //usb&adapter����
            if ((act_readl(CHG_DET) & (1 << CHG_DET_UVLO)) != 0)
            {
                wake_ret = WAKE_BY_USB;
                break;
            }
        }

#if (SUPPORT_OUTER_CHARGE == 1)
        if (sys_comval->bat_charge_mode == BAT_CHARGE_MODE_OUTER)
        {
            //�͵��˳��͹���ģʽ
            if (((act_readb(TEMPADC_DATA)) & 0x7F) < TEMP_ADC_1_62_V)
            {
                wake_ret = WAKE_BY_LOWPOWER;
                break;
            }
        }
        else
#endif
        {
            //�͵��˳��͹���ģʽ
            if (((act_readb(BATADC_DATA)) & 0x7f) < BAT_ADC_3_40_V)
            {
                wake_ret = WAKE_BY_LOWPOWER;
                break;
            }
        }

        if ((wake_type & WAKE_TYPE_BT_RX) != 0)
        {
            if ((act_readl(WAKE_PD) & (1 << WAKE_PD_BT_PD)) != 0)
            {
                wake_ret = WAKE_BY_BT;
                break;
            }
        }

        //onoff key����
        if ((wake_type & WAKE_TYPE_ONOFF_KEY) != 0)
        {
            if ((act_readl(ONOFF_KEY) & (1<<ONOFF_KEY_ONOFF_PRESS_0)) != 0)
            {
                wake_ret = WAKE_BY_KEY;
                break;
            }
        }

        //����������
        if ((wake_type & WAKE_TYPE_PHY_KEY) != 0)
        {
            if ((act_readb(LRADC1_DATA) & 0x7F) < KEY_VAL_DATA_MAX)
            {
                wake_ret = WAKE_BY_KEY;
                break;
            }
        }

        //����ң��������
        if ((wake_type & WAKE_TYPE_IR_KEY) != 0)
        {
            if (((act_readl(IRC_STA) & (1 << IRC_STA_UCMP)) != 0)
                    || ((act_readl(IRC_STA) & (1 << IRC_STA_IIP)) != 0))
            {
                wake_ret = WAKE_BY_KEY;
                break;
            }
        }

        //AUX��⻽��
        if ((wake_type & WAKE_TYPE_AUX_DETECT) != 0)
        {
#if (SUPPORT_AUX_DETECT == DETECT_BY_GPIO)
            bool aux_in;

#if (AUX_GPIO_EN_LEVEL == ENABLE_LEVEL_LOW)
            aux_in = (bool)((act_readl(AUX_GPIO_DATA) & AUX_GPIO_PIN) == 0);
#else
            aux_in = ((act_readl(AUX_GPIO_DATA) & AUX_GPIO_PIN) != 0);
#endif
            if (((get_linein_state() == LINEIN_STATE_LINEIN_NULL) && (aux_in == TRUE))
                    || ((get_linein_state() == LINEIN_STATE_LINEIN_IN) && (aux_in == FALSE)))
            {
                wake_ret = WAKE_BY_AUX_DET;
                break;
            }
#endif
        }

        //����⻽��
#if (SUPPORT_CARD_DETECT != DETECT_BY_CMD)
        if ((wake_type & WAKE_TYPE_CARD_DETECT) != 0)
        {
#if (SUPPORT_CARD_DETECT == DETECT_BY_GPIO)
            bool card_in;

#if (CARD_GPIO_EN_LEVEL == ENABLE_LEVEL_LOW)
            card_in = ((act_readl(CARD_GPIO_DATA) & CARD_GPIO_PIN) == 0);
#else
            card_in = ((act_readl(CARD_GPIO_DATA) & CARD_GPIO_PIN) != 0);
#endif
            if (((get_card_state() == CARD_STATE_CARD_NULL) && (card_in == TRUE))
                    || ((get_card_state() != CARD_STATE_CARD_NULL) && (card_in == FALSE)))
            {
                wake_ret = WAKE_BY_CARD_DET;
                break;
            }
#endif
        }
#endif

        //���¼�ⷽʽ���ڸ�Ƶ�½���
        if (g_low_power_var.high_freq_flag == 1)
        {
            if (low_freq_time == 0)
            {
                //delay 1ms
                delay_ms_13m(1);
            }

            //I2C ��������Ұ���ģ�飬���紥��������������
            if ((wake_type & WAKE_TYPE_I2C_KEY) != 0)
            {
                i2c_key_counter++;
                if (i2c_key_counter >= g_low_power_var.i2c_key_timer)
                {
                    i2c_key_counter = 0;
#if (SUPPORT_TK_DRIVER == TK_DRIVER_OUTER)
                    if (key_extern_tk_op(TK_OP_CHECK) != 0)
                    {
                        wake_ret = WAKE_BY_KEY;
                        break;
                    }
#endif
                }
            }

            //CMD ����⻽��
#if (SUPPORT_CARD_DETECT == DETECT_BY_CMD)
            if ((wake_type & WAKE_TYPE_CARD_DETECT) != 0)
            {
                card_detect_counter++;
                if (card_detect_counter >= g_low_power_var.card_detect_timer)
                {
                    int8 card_ret;
                    card_detect_counter = 0;

                    card_ret = (int8) sys_detect_disk(DRV_GROUP_STG_CARD);
                    if (((get_card_state() == CARD_STATE_CARD_NULL) && (card_ret == 0))
                            || ((get_card_state() != CARD_STATE_CARD_NULL) && (card_ret == -1)))
                    {
                        wake_ret = WAKE_BY_CARD_DET;
                        break;
                    }
                }
            }
#endif

            //UHOST��⻽��
            if ((wake_type & WAKE_TYPE_UHOST_DETECT) != 0)
            {

            }
        }

        if (low_freq_time > 0)
        {
            if (low_freq_cycle > low_freq_cycle_per_while)
            {
                low_freq_cycle -= low_freq_cycle_per_while;

                if (g_low_power_var.high_freq_flag == 1)
                {
                    //�л�����Ƶ
                    switch_low_freq();
                }
            }
            else
            {
                low_freq_cycle = low_freq_time*32;

                if (g_low_power_var.high_freq_flag == 0)
                {
                    //�л�����Ƶ
                    switch_high_freq();
                }
            }
        }

        //��ʱ����жϻ������ȱ��������幷
        CLEAR_WATCH_DOG;
    }

    return wake_ret;
}

static void com_standby_enter_cb(void)
{
    //�ص�LED����ܣ�LED��
#if (SUPPORT_LED_DRIVER == 1)
    if(sys_comval->support_led_display == 1)
    {
        led_sleep_display();
    }
#endif

    discrete_led_low_power();

    SPEAKER_OFF();
    SPEAKER_CONTROL_DISABLE();
}

static void com_standby_exit_cb(void)
{
    standby_restart_all_app_timer(g_app_timer_vector, g_app_timer_count);
    standby_restart_all_app_timer(g_com_app_timer_vct, COM_APP_TIMER_MAX);

    libc_memset(&g_sys_counter, 0x00, sizeof(sys_counter_t));

    //����LED�����
#if (SUPPORT_LED_DRIVER == 1)
    if(sys_comval->support_led_display == 1)
    {
        led_wake_up_display();
    }
#endif

    discrete_led_restore();

    SPEAKER_CONTROL_ENABLE();
    SPEAKER_ON();
    //I2S ENABLE�����������������
    if (get_mute_enable() == FALSE)
    {
        com_reset_sound_volume(0);
    }
}
