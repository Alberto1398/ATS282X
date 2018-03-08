/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������AVRCP���Ӧ�ò�ӿڣ��൱��BT STACK��AVRCP PROFILE������㡣
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"
#include "common_func.h"
#include "common_btmanager.h"
#include "avrcp_opid.h"

typedef struct
{
    uint8 op_type;
    uint8 op_id;
} btstack_avrcp_op_t;

static const btstack_avrcp_op_t s_btstack_avrcp_ops[] =
{
    { AVRCP_OP_REQ, AVRCP_OPID_PAUSE }, /* 0 BTPLAY_AVRCP_PAUSE */
    { AVRCP_OP_REQ, AVRCP_OPID_PLAY }, /* 1 BTPLAY_AVRCP_PLAY */
    { AVRCP_OP_REQ, AVRCP_OPID_FORWARD }, /* 2 BTPLAY_AVRCP_NEXT */
    { AVRCP_OP_REQ, AVRCP_OPID_BACKWARD }, /* 3 BTPLAY_AVRCP_PREV */
    { AVRCP_OP_PRESS, AVRCP_OPID_FAST_FORWARD }, /* 4 BTPLAY_AVRCP_FFORWARD */
    { AVRCP_OP_PRESS, AVRCP_OPID_REWIND }, /* 5 BTPLAY_AVRCP_FBACKWARD */
    { AVRCP_OP_RELEASE, AVRCP_OPID_FAST_FORWARD }, /* 6 BTPLAY_AVRCP_RELEASEFF */
    { AVRCP_OP_RELEASE, AVRCP_OPID_REWIND }, /* 7 BTPLAY_AVRCP_RELEASEFB */
};

//return 0: ok
//return -1: opid invalid
//return -2: avrcp not connect 
int com_btmanager_avrcp_op(btplay_avrcp_opid_e opid)
{
    msg_apps_t msg;

    if (opid >= BTPLAY_AVRCP_MAXID)
    {
        return -1;
    }

//    if ((g_bt_stack_cur_info.serv_conn & AVRCP_CONNECTED_MY) == 0)
//    {
//        return -2;
//    }

    msg.type = MSG_BTSTACK_AVRCP_TX_OPID_SYNC;
    msg.content.data[0] = s_btstack_avrcp_ops[opid].op_type;
    msg.content.data[1] = s_btstack_avrcp_ops[opid].op_id;

    //����ͬ����Ϣ
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].addr.bytes, &msg, NULL, 0);

    return 0;
}

#ifdef __BQB_TEST_PROFILE_
void com_btmanager_delay_report(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTSTACK_DELAY_REPORT_SYNC;

    //����ͬ����Ϣ
    send_sync_msg_btmanager(g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].addr.bytes, &msg, NULL, 0);
}
#endif

app_result_e com_btmanager_avrcp_ind(uint8 op_id, bool tts_flag)
{
    app_result_e result = RESULT_NULL;

    switch (op_id)
    {
        case AVRCP_OPID_VOLUME_UP:
        com_volume_add(tts_flag);
        break;

        case AVRCP_OPID_VOLUME_DOWN:
        com_volume_sub(tts_flag);
        break;

        default:
        break;
    }

    return result;
}

//NOTE:IPhone�ֻ��л�����ʱ���ὫIPhone 0-16 <--> 0-127 AVRCP VOLUME ��������ֵ���õ�����ˣ�
//�����п���������΢���������������
//С�����ֻ�ͬ����ʱ���Ѿ�����ʱ,����Ӧ�ֻ���С��ͬ������
app_result_e com_btmanager_avrcp_set_volume_to_soundbox(uint8 vol, bool tts_flag)
{
    app_result_e result = RESULT_NULL;
    uint16 tmp_volume, tmp_volume_remainder;

#ifndef ENABLE_TRUE_WIRELESS_STEREO
    if ((g_this_app_info->app_id == APP_ID_BTPLAY) && (g_btmanager_gl_var.avrcp_timer_id == -1))
#else    	
    if (((g_this_app_info->app_id == APP_ID_BTPLAY) || (g_this_app_info->app_id == APP_ID_LINEIN)) 
    	     && (g_btmanager_gl_var.avrcp_timer_id == -1))
#endif    	     
    {
        tmp_volume = (uint16) vol * VOLUME_VALUE_MAX;
        tmp_volume_remainder = tmp_volume % AVRCP_ABSOLUTE_VOLUME_MAX;
        tmp_volume = tmp_volume / AVRCP_ABSOLUTE_VOLUME_MAX;
        //if (tmp_volume_remainder >= (AVRCP_ABSOLUTE_VOLUME_MAX - VOLUME_VALUE_MAX))
        if (tmp_volume_remainder != 0)
        { //��ֹ������soundbox <--> phone ֮��ͬ�����Գ�
            tmp_volume++;
        }
        tmp_volume = (tmp_volume > VOLUME_VALUE_MAX) ? VOLUME_VALUE_MAX : tmp_volume;

        if (tts_flag == FALSE)
        {
          //  libc_print("tts flag F",0,0);
            com_volume_set((uint8) tmp_volume, SET_VOLUME_VIEW | SET_VOLUME_TTS);
        }
        else
        {
           //  libc_print("tts flag T",0,0);
            //TTS���ڲ��ŵĹ����У�������ϵͳ����������TTS�ڲ����Ĺ������������޸�
           // if (g_tts_play_info.status == TTS_STATUS_PLAYING)
            if (g_tts_play_info.status >= TTS_STATUS_READY)
            {
                
            }
            else
            {
                com_volume_set((uint8) tmp_volume, SET_VOLUME_TTS);
            }
            //com_volume_set((uint8) tmp_volume, SET_VOLUME_TTS);
        }
    }

    return result;
}

uint8 com_btmanager_avrcp_volume2avrcp_volume(uint8 vol)
{
    uint16 tmp_volume;

    tmp_volume = (uint16) vol;
    tmp_volume = (uint16) ((tmp_volume * AVRCP_ABSOLUTE_VOLUME_MAX) / VOLUME_VALUE_MAX);

    return (uint8) tmp_volume;
}

static int __avrcp_update_volume_to_phone(uint8 vol, uint8 *bd)
{
    msg_apps_t msg;

    msg.type = MSG_BTSTACK_AVRCP_VOLUME_SYNC;
    msg.content.data[0] = com_btmanager_avrcp_volume2avrcp_volume(vol);

    send_sync_msg_btmanager(bd, &msg, NULL, 0);

    return 0;
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO

//master to slave interface
#if 0
int com_btmanager_tws_send(uint8 data, uint8 *bd,uint32 type)
{
	msg_apps_t  tws_msg;
	
	  tws_msg.type = type;
	  data=com_btmanager_avrcp_volume2avrcp_volume(data);
	  tws_msg.content.data[0] = data;
	  libc_print("data:",data,2);
	  send_sync_msg_btmanager(bd, &tws_msg, NULL, 0);
	
	  return 0;

}

#endif

/*
    parameters:
    data:cmd flag
    data1~data3:cmd value
*/
int com_btmanager_tws_send(uint8 data,uint8 data1, uint8 data2,uint8 data3,uint32 type)
{
    
      msg_apps_t  tws_msg;
      tws_msg.type = type;
      //tws_msg.type = MSG_BTSTACK_TWS_SEND_MSG_
      if(data==VOLUME_VALUE_FLAG)
      {
        data1=com_btmanager_avrcp_volume2avrcp_volume(data1);       
      }
      tws_msg.content.data[0] = data;
      tws_msg.content.data[1] = data1;
      tws_msg.content.data[2] = data2;
      tws_msg.content.data[3] = data3;
      libc_print("data1:",data1,2);
      send_sync_msg_btmanager(NULL, &tws_msg, NULL, 0);

      return 0;

}

#endif
int com_btmanager_avrcp_update_volume_to_phone(uint8 vol)
{
    if (/*((g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].serv_conn & AVRCP_CONNECTED_MY) == 0)
            ||*/ (g_btmanager_gl_var.enable_avrcp_volume_sync == 0))
    {

//֧��������ͬ������������ͬ���ֻ�������
#ifdef ENABLE_TRUE_WIRELESS_STEREO
#if 0
            if(g_bt_stack_cur_info.dev_role!=0)
            {
                com_btmanager_tws_send(vol, NULL);  
            }
#else
        if(g_bt_stack_cur_info.dev_role!=0)
        {
            com_btmanager_tws_send(VOLUME_VALUE_FLAG, vol,NULL,NULL,MSG_BTSTACK_TWS_SEND_MSG_SYNC);  
        }
#endif


#endif
        return 0;
    }

    //�ȵ�Сһ�������ios�����������ͬ��������
    if (vol == VOLUME_VALUE_MAX)
    {
        __avrcp_update_volume_to_phone(vol - 1,
                g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].addr.bytes);
    }

    __avrcp_update_volume_to_phone(vol,
            g_bt_stack_cur_info.rmt_dev[g_bt_stack_cur_info.a2dp_active_id].addr.bytes);

    return 0;
}

