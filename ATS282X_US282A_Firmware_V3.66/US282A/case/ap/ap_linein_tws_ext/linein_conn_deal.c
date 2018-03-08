/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������line in tws�Ƹ�ǰ̨Ӧ���û�����ʵ�֣����������¼���ϵͳ�¼�����
 * ���ߣ�
 ********************************************************************************/

#include  "ap_linein.h"

/******************************************************************************/
/*!
 * \par  Description:
 *      �Ƿ��Ѿ����ӣ�������Ͽ�
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void linein_check_disconn_deal(void)
{
	uint8 id;
	
	//disconnect 
	for(id=0;id<RMT_DEV_NUM;id++)
	{
		//connct to phone
		if(g_bt_stack_cur_info.rmt_dev[id].dev_type == NO_TWS_DEV)
		{
			//disconnect
			msg_apps_t msg;
            msg.content.data[0] = id;
            //��Ϣ����(����Ϣ����)
            msg.type = MSG_BTSTACK_FORCE_DISCONN; 
            //����ͬ����Ϣ
            send_sync_msg_btmanager(NULL, &msg, NULL, 0); 
            g_enter_disc_flag = 1;
			break;
		}
	}
	
	//paired undiscover
	for(id=0;id<RMT_DEV_NUM;id++)
	{		
		//if two box paired
		//if((g_bt_stack_cur_info.rmt_dev[id].dev_type == TWS_DEV)&&(g_set_undicover_flag == 0))
		if((g_bt_stack_cur_info.rmt_dev[id].a2dp_status != A2DP_STATUS_NONE)
			&&(g_bt_stack_cur_info.rmt_dev[id].dev_type == TWS_DEV)
			&&(g_set_undicover_flag == 0))
		{			           
            com_set_discoverable(2);            
            
			g_set_undicover_flag = 1;
						
			//g_set_dicover_flag = 0;
			
			break;
		}
	}
	
	//if has been conn 
	for(id=0;id<RMT_DEV_NUM;id++)
	{		
		//if two box paired
		if(g_bt_stack_cur_info.rmt_dev[id].dev_type != DEV_TYPE_NONE)
		{						
			break;
		}
	}

	//didn't conn any
	if((id == RMT_DEV_NUM)&&(g_set_undicover_flag == 1))	
	{
		//com_set_discoverable(1);
            
		//g_set_dicover_flag = 1;	
		
		g_set_undicover_flag = 0;	
	}	
	
}


/******************************************************************************/
/*!
 * \par  Description:
 *      ����Ӧ��ʱ����Ͽ�������Ҫ����������򿪿ɼ��ԺͿ�������
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void linein_recover_conn_deal(void)
{
    if((g_set_undicover_flag == 1)&&(g_bt_stack_cur_info.dev_role == TWS_MASTER))
    {
        //discoverable restore
	    com_set_discoverable(1);
    }
		
	if(g_enter_disc_flag == 1)
	{
		//need auto connect
		com_restart_auto_connect_set();
	}
	
}


/******************************************************************************/
/*!
 * \par  Description:
 *      ����Ӧ��ʱ����Ͽ�������Ҫ����������򿪿ɼ��ԺͿ�������
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void linein_set_discover_ctl(uint8 ctl_mode)
{
    com_set_discover_ctl(ctl_mode);
}




