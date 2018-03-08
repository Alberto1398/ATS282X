#include "app_mengine.h"

//дrtc��Ĵ�������
void flush_rtc_reg(void)
{
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//qac
    }
}

//���ESD RESTART��־�����������벥��ʱ��ǣ�����ܹ������˳����ţ������ clear_esd_restart_tag �����
void set_esd_restart_tag(void)
{
    act_writel(act_readl(RTC_BAK0) | (1 << MY_RTC_ESD_FLAG), RTC_BAK0);
    flush_rtc_reg();
}

//���ESD RESTART��־���������˳�����ʱ����
void clear_esd_restart_tag(void)
{
    act_writel(act_readl(RTC_BAK0) & (~(1 << MY_RTC_ESD_FLAG)), RTC_BAK0);
    flush_rtc_reg();
}

//�Ƿ�ESD RESTART
bool is_esd_restart(void)
{
    if ((act_readl(RTC_BAK0) & (1 << MY_RTC_ESD_FLAG)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//�ϵ�ָ�
bool get_bk_info(void)
{
    bool ret = FALSE;
    if (is_esd_restart() == TRUE)
    {
        g_eg_cfg_p->bk_infor.bp_file_offset = (int)act_readl(RTC_BAK1);
        g_eg_cfg_p->bk_infor.bp_time_offset = (int)act_readl(RTC_BAK2);
        ret = TRUE;
        libc_print("re-bk-info", g_eg_cfg_p->bk_infor.bp_file_offset, 2);
    }
    return ret;
}

