#include "app_radio.h"

//���ų���ѭ������
app_result_e playing_loop_deal(void)
{
    app_result_e ret = RESULT_NULL;

    //radio ���ų���ѭ��
    while (1)
    {
        ret = com_view_loop();
        //iOS APPֻҪ����˵��ͻᷢһ��ֹͣ��̨����������������ǲ���������̨
        if ((ret > RESULT_COMMON_RESERVE) && (ret != RESULT_STOP_SEARCH))
        {
            break;
        }

#ifdef SUPPORT_ASET_TEST
        if ((g_app_info_state.stub_tools_type == STUB_PC_TOOL_WAVES_ASET_MODE) || (g_app_info_state.stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE))
        {
            aset_test_loop_deal();
        }
#endif 
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    return ret;
}

