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
 * \file     config_main.c
 * \brief    ���ػ��Ĵ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "manager.h"
#include "stub_interface.h"
#include "stub_command.h"
#include "applib.h"

static const char stub_name[] = { "stub.al" };


uint8 pc_connect_probe(void)
{
    uint32 pc_type = 0;

    int8 ret;

    while (1)
    {
        ret = (int8) stub_get_data(STUB_CMD_OPEN, &pc_type, 4);

        if (ret == 0)
        {
            if(pc_type != 0)
            {
                break;
            }
        }

        sys_os_time_dly(5);
    }

    pc_type = (uint8) pc_type;

    return pc_type;
}

void clear_esd_flag(void)
{
    uint8 temp_data[4];
    uint32 i;

    for(i = 0; i < 4; i++)
    {
        temp_data[i] = 0;
    }

    sys_vm_write(temp_data, VM_AP_CONFIG, sizeof(temp_data));   
}

uint8 manager_open_stub(uint8 need_uninstall_led)
{
    uint8 stub_type; 

    //����ϵͳƵ��
    sys_adjust_clk(85 | (180 << 8), 0);
    
    //����Ƶ��Ϊ��MIPS -> 85M, DSP -> 180
    sys_lock_adjust_freq(85 | (180 << 8)); 

    /* ���� stub ������� manager ����ʱ�Ѽ��� */
    /* ���� USB stub ����� */
    if(g_app_info_state.stub_phy_type == STUB_PHY_USB)
    {
        sys_load_mmm(stub_name, FALSE);
    }
    
    sys_os_sched_lock();
    
    //���Կ�ʼ֮ǰ�ؿ��Ź�����ֹ��������
    DISABLE_WATCH_DOG;  
#if 0
    //�������ģʽ������ESD���ܣ����֮ǰ�����AP����
    act_writel(act_readl(RTC_BAK0) & (~(0xff << MY_RTC_FUNC_INDEX)), RTC_BAK0);

    //rtc register
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//wait for register update
    }     

    act_writel(act_readl(RTC_BAK0) | (APP_FUNC_INVALID << MY_RTC_FUNC_INDEX), RTC_BAK0);

    //rtc register
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//wait for register update
    }     

    clear_esd_flag();
#endif
    stub_open(need_uninstall_led);

    PRINT_INFO("stub init");

    sys_os_sched_unlock();

    //���STUBͨ�ţ�ͨ��OK��Ž������
    stub_type = pc_connect_probe();

    g_app_info_state.stub_tools_type = stub_type;
    g_app_info_state_all.stub_pc_tools_type = stub_type;
    g_app_info_state_all.stub_phy_type = g_app_info_state.stub_phy_type;

    return stub_type;
}
