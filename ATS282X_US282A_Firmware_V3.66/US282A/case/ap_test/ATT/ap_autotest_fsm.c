/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     user1_main.c
 * \brief    picture��ģ�飬������̳�ʼ�����˳�������������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_manager_test.h"

#if 0
test_result_e act_test_fmplay_test(void *arg_buffer)
{
    //fmplay_test_arg_t *fmplay_test_arg = (fmplay_test_arg_t *)arg_buffer;  

    test_result_e result = TEST_PASS;

    return result;     
}
#endif


static void sys_reboot(void)
{       
    DEBUG_ATT_PRINT("reboot...", 0, 0);
    
    sys_local_irq_save();
    
    ENABLE_WATCH_DOG(1);
    
    while(1);
}
    


void act_test_change_test_timeout(uint16 timeout)
{
    uint32 ret_val;

    uint8 *cmd_data;

    cmd_data = (uint8*)STUB_ATT_RW_TEMP_BUFFER;

    //�ó�ʱʱ������Ϊ��λ��ȡ�������������ʹ�õ�ʱ��
    cmd_data[6] = (timeout & 0xff);  // ����������������ʱ
    cmd_data[7] = ((timeout >> 8) & 0xff);
    cmd_data[8] = 0;
    cmd_data[9] = 0;  

    ret_val = att_write_data(STUB_CMD_ATT_GET_TEST_ID, 4, STUB_ATT_RW_TEMP_BUFFER);

    if(ret_val == 0)
    {    
        ret_val = att_read_data(STUB_CMD_ATT_GET_TEST_ID, 4, STUB_ATT_RW_TEMP_BUFFER);

        if(ret_val == 0)
        {   
            //�ظ�ACK
            ret_val = att_write_data(STUB_CMD_ATT_ACK, 0, STUB_ATT_RW_TEMP_BUFFER);  
        }
    } 

    return;
}

//���յ��������в���IDΪ0xffff��С���˲�Ҫ�ٷ��κ����ݸ�ATT���ߣ���������ATT���߲���Ӧ�����
int32 act_test_read_testid(uint8 *arg_buffer, uint32 arg_len)
{
    int ret_val;
    uint8 *cmd_data;
    uint8 temp_data[8];

    if(g_test_mode != TEST_MODE_CARD)
    {  
        cmd_data = (uint8*)STUB_ATT_RW_TEMP_BUFFER;

        //�ó�ʱʱ������Ϊ��λ��ȡ�������������ʹ�õ�ʱ��
        cmd_data[6] = 20;  // ����������������ʱ
        cmd_data[7] = 0;
        cmd_data[8] = 0;
        cmd_data[9] = 0;
        
        ret_val = att_write_data(STUB_CMD_ATT_GET_TEST_ID, 4, STUB_ATT_RW_TEMP_BUFFER);

        if(ret_val == 0)
        {    
            ret_val = att_read_data(STUB_CMD_ATT_GET_TEST_ID, 4, STUB_ATT_RW_TEMP_BUFFER);

            if(ret_val == 0)
            {   
                cmd_data = (uint8 *)STUB_ATT_RW_TEMP_BUFFER;
                
                g_test_info.test_id = (cmd_data[6] | (cmd_data[7] << 8));

                g_test_info.arg_len = (cmd_data[8] | (cmd_data[9] << 8));

                //TESTIDΪTESTID_ID_QUIT���ظ�ACK
                if(g_test_info.test_id != TESTID_ID_QUIT)
                {
                    //�ظ�ACK
                    ret_val = att_write_data(STUB_CMD_ATT_ACK, 0, STUB_ATT_RW_TEMP_BUFFER);  
                }
                
                if((g_test_info.test_id == TESTID_PRODUCT_TEST)
                    || (g_test_info.test_id == TESTID_FLASHTEST)
                    || (g_test_info.test_id == TESTID_BER_TEST)
                    || (g_test_info.test_id == TESTID_RSSI_TEST))
                {
                    act_test_change_test_timeout(60);                    
                }              
            }
        }

        libc_print("arg len", g_test_info.arg_len, 2);

        if(g_test_info.arg_len != 0)
        {
            ret_val = att_write_data(STUB_CMD_ATT_GET_TEST_ARG, 0, STUB_ATT_RW_TEMP_BUFFER);

            if(ret_val == 0)
            {    
                ret_val = att_read_data(STUB_CMD_ATT_GET_TEST_ARG, g_test_info.arg_len, STUB_ATT_RW_TEMP_BUFFER);

                if(ret_val == 0)
                {
                    //����ʹ��STUB_ATT_RW_TEMP_BUFFER����Ϊ��������Ҫʹ�ø�buffer��������
                    ret_val = att_write_data(STUB_CMD_ATT_ACK, 0, temp_data);                   
                }
            }

            act_test_read_test_arg(g_test_info.test_id, (uint8 *)(STUB_ATT_RW_TEMP_BUFFER + sizeof(stub_ext_cmd_t)), arg_buffer, arg_len);
        }
    }
    else
    {
        ret_val = 0;

        act_test_read_test_info(g_cur_line_num, &g_test_info.test_id, arg_buffer, arg_len);

        if(g_test_info.test_id != 0xff)
        {
            g_cur_line_num++;
        }
    }
    return ret_val;    
}

const att_task_stru_t autotest_ops[] =
{
    {TESTID_MODIFY_BTNAME,              1, 1, 0, 0, act_test_modify_bt_name},

    {TESTID_MODIFY_BLENAME,             1, 1, 0, 0, act_test_modify_bt_ble_name},

    {TESTID_MODIFY_BTADDR,              1, 1, 0, 0, act_test_modify_bt_addr},

    {TESTID_BT_TEST,                    1, 1, 0, 0, act_test_bt_test},

    {TESTID_GPIO_TEST,                  1, 1, 0, 0, act_test_gpio_test},

    {TESTID_GPIO_TEST_ATS2823,          1, 1, 0, 0, act_test_gpio_test_ATS2823},

    {TESTID_LINEIN_CH_TEST_ATS2825,     1, 1, 0, 0, act_test_linein_channel_test_ATS2825},

    {TESTID_LINEIN_CH_TEST_ATS2823,     1, 1, 0, 0, act_test_linein_channel_test_ATS2823},

    {TESTID_MIC_CH_TEST,                1, 1, 0, 0, act_test_mic_channel_test}, 

//    {TESTID_FM_CH_TEST,               1, 1, 0, 0, act_test_fm_channel_test},

    {TESTID_SDCARD_TEST,                1, 1, 0, 0, act_test_sdcard_play_test},

    {TESTID_PRODUCT_TEST,               0, 1, 1, 0, act_test_product_test},

    {TESTID_PREPARE_PRODUCT_TEST,       0, 1, 1, 0, act_test_prepare_product},
	
    {TESTID_UHOST_TEST,                 1, 1, 0, 0, act_test_uhost_play_test},

//    {TESTID_LINEIN_TEST,                1, 1, 0, 0, act_test_linein_play_test},

    {TESTID_MP_TEST,                    0, 1, 0, 0, att_mp_test},

    {TESTID_MP_READ_TEST,               0, 1, 0, 0, att_mp_read_test},

    {TESTID_READ_BTADDR,                1, 1, 0, 0, act_test_read_bt_addr},

    {TESTID_READ_BTNAME,                1, 1, 0, 0, act_test_read_bt_name},

    {TESTID_FTMODE,                     1, 1, 0, 0, act_test_enter_ft_mode},

    {TESTID_BQBMODE,                    1, 1, 0, 0, act_test_enter_BQB_mode},

    {TESTID_FLASHTEST,                  1, 1, 1, 0, act_test_flashtest},

    {TESTID_BER_TEST,                   0, 1, 0, 0, act_test_ber_test},

    {TESTID_RSSI_TEST,                  0, 1, 0, 0, act_test_rssi_test},    
    
    {TESTID_LRADC_TEST,                 1, 1, 0, 0, act_test_lradc_test},

    {TESTID_MODIFY_BTADDR2,             1, 1, 0, 0, act_test_modify_bt_addr},
//    {TESTID_MONITOR,                    act_test_monitor}
};

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
void att_flush_rtc_reg(void)
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
void en_play_wake_up(void)
{
    uint32 tmp;
    tmp = act_readl(WKEN_CTL);

    //tmp |= (1 << WKEN_CTL_SHORT_WKEN) //play�̰�����
    //| (1 << WKEN_CTL_LONG_WKEN); //play��������
    
    tmp |= (1 << WKEN_CTL_LONG_WKEN);

    tmp &= ~(1 << WKEN_CTL_BT_WK_EN); //��ֹBT����

    act_writel(tmp, WKEN_CTL); //���û��ѷ�ʽ
    att_flush_rtc_reg();
    act_writel(act_readl(WAKE_PD), WAKE_PD); //��pending
}

//����standby S3BT/S4ģʽ��DV5V��alarm���ⲿ�жϣ�ADC_KEY���ѣ�ON_OFF�����ѣ�BT����
//���ٷ��أ�ͨ�����ֻ����ֶν�ϵͳreset
void att_power_off(void)
{
    DISABLE_WATCH_DOG;
      
    en_play_wake_up();  
    
    while (1)
    {    
        libc_print("dc 5v on", 0, 0);
        
        //�ȴ�DC5V��
        if ((act_readl(CHG_DET) & (1 << CHG_DET_UVLO)) == 0)
        {
            break;
        }

        sys_mdelay(500);
    }
        
retry: 
    libc_print("poweroff", 0, 0);   
    act_writel(0x00, POWER_CTL); //disable All�����߽�S4
    
    sys_mdelay(3000);
    
    while (1)
    {        
        //�ȴ�DC5V��
        if ((act_readl(CHG_DET) & (1 << CHG_DET_UVLO)) == 0)
        {
            goto retry;
        }
    }    
}

void test_dispatch(void)
{
    int ret_val;
    int i;
    uint8 att_cmd_temp_buffer[80];

    act_test_start_deal();
    
    while(1)
    {
        ret_val = act_test_read_testid(att_cmd_temp_buffer, 80);

        if(ret_val != 0)
        {
            sys_mdelay(100);
            continue;
        }

        if(g_test_info.test_id == TESTID_ID_WAIT)
        {
            sys_mdelay(500);
            continue;
        }        

        if(g_test_info.test_id == TESTID_ID_QUIT)
        {
            break;
        }
        
		DEBUG_ATT_PRINT("test_dispatch:", g_test_info.test_id, 2);
        for(i = 0; i < sizeof(autotest_ops)/sizeof(att_task_stru_t); i++)
        {
            if(autotest_ops[i].test_id == g_test_info.test_id)    
            {
                if(g_test_mode == TEST_MODE_CARD)
                {
                    if(autotest_ops[i].support_card_test == 0)
                    {
                        att_write_test_info("Unsupport card test: ", g_test_info.test_id, 1);
                        act_test_report_test_log(FALSE, g_test_info.test_id);
                        break;
                    }
                }
                autotest_ops[i].test_func(att_cmd_temp_buffer); 
                break;                  
            }
        } 

        if(i == sizeof(autotest_ops)/sizeof(att_task_stru_t))
        {
            //DEBUG_ATT_PRINT("unknown id", 0, 0);

            break;
        }
    }

    if(g_test_mode != TEST_MODE_USB)
    {
        write_log_file(TRUE);
        //sys_mdelay(5000);
        //sys_reboot();         
        led_flash_ok();         
    } 
    else
    {          
        att_write_test_info("Test time: ", sys_get_ab_timer() - g_test_base_time, 1);
        stub_close();
        //sys_mdelay(5000);
        //sys_reboot();
    }
}


