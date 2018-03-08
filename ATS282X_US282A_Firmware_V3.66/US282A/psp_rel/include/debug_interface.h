/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������ӡ����
 * ���ߣ�liminxian
 ********************************************************************************/


#ifndef __DEBUG_INTERFACE_H__
#define __DEBUG_INTERFACE_H__

#define PRINT_STRING_ONLY       0
#define PRINT_DATA_ONLY         1
#define PRINT_STRING_AND_DATA   2
#define PRINT_HEX_BYTES         3

#define PRINT_ALL_FUNC  libc_print

#define PRINT_IRQ_FUNC  sys_irq_print

#define APP_ERR_PRE   "<E> "
#define APP_WARN_PRE  "<W> "
#define APP_INFO_PRE  "<I> "
#define APP_DBG_PRE   "<D> "
#define APP_DATA_PRE  "<data> "

#define BT_ERR_PRE    "<BT E> "
#define BT_WARN_PRE   "<BT W> "
#define BT_INFO_PRE   "<BT I> "
#define BT_DBG_PRE    "<BT D> "
#define BT_DATA_PRE   "<BT data> "

#define DRV_ERR_PRE   "<drv E> "
#define DRV_WARN_PRE  "<drv W> "
#define DRV_INFO_PRE  "<drv I> "
#define DRV_DBG_PRE   "<drv D> "
#define DRV_DATA_PRE  "<drv data> "


#define BTDRV_INFO_PRE   "<BT drv> "

/******************************************
APPӦ�ô�ӡ
*******************************************/
//err��Ϣ����������Զ�򿪣����ǿռ䲻����
#define PRINT_ERR(str)                PRINT_ALL_FUNC(APP_ERR_PRE str,0,PRINT_STRING_ONLY)
#define PRINT_ERR_STR(str)            PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINT_ERR_INT(str,data)       PRINT_ALL_FUNC(APP_ERR_PRE str,data,PRINT_STRING_AND_DATA)

//������Ϣ���û�����AP�õ��Ľ��٣���Ҫ��ϵͳʹ�ã��������
#define PRINT_WARNING(str)            PRINT_ALL_FUNC(APP_WARN_PRE str,0,PRINT_STRING_ONLY)
#define PRINT_WARNING_STR(str)        PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINT_WARNING_INT(str,data)   PRINT_ALL_FUNC(APP_WARN_PRE str,data,PRINT_STRING_AND_DATA)

//�ؼ�·�����㷨����Ҫ��Ϣ
#define PRINT_INFO(str)               PRINT_ALL_FUNC(APP_INFO_PRE str,0,PRINT_STRING_ONLY)
#define PRINT_INFO_STR(str)           PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINT_INFO_INT(str,data)      PRINT_ALL_FUNC(APP_INFO_PRE str,data,PRINT_STRING_AND_DATA)

//��ͨ�����Ľ��롢�˳��������ȵ�����Ϣ������bugʱ��
#define PRINT_DBG(str)                //PRINT_ALL_FUNC(APP_DBG_PRE str,0,PRINT_STRING_ONLY)
#define PRINT_DBG_STR(str)            //PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINT_DBG_INT(str,data)       //PRINT_ALL_FUNC(APP_DBG_PRE str,data,PRINT_STRING_AND_DATA)

//��ӡ ������
#define PRINT_DATA(buf,len)           do{ \
    PRINT_ALL_FUNC(APP_DATA_PRE,0,PRINT_STRING_ONLY); \
    PRINT_ALL_FUNC(buf,len,PRINT_HEX_BYTES); \
}while(0)


/******************************************
BT STACK��ӡ
*******************************************/
//err��Ϣ����������Զ�򿪣����ǿռ䲻����
#define BT_PRINT_ERR(str)                PRINT_ALL_FUNC(BT_ERR_PRE str,0,PRINT_STRING_ONLY)
#define BT_PRINT_ERR_STR(str)            PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define BT_PRINT_ERR_INT(str,data)       PRINT_ALL_FUNC(BT_ERR_PRE str,data,PRINT_STRING_AND_DATA)

//������Ϣ���û�����AP�õ��Ľ��٣���Ҫ��ϵͳʹ�ã��������
#define BT_PRINT_WARNING(str)            PRINT_ALL_FUNC(BT_WARN_PRE str,0,PRINT_STRING_ONLY)
#define BT_PRINT_WARNING_STR(str)        PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define BT_PRINT_WARNING_INT(str,data)   PRINT_ALL_FUNC(BT_WARN_PRE str,data,PRINT_STRING_AND_DATA)

//�ؼ�·�����㷨����Ҫ��Ϣ
#define BT_PRINT_INFO(str)               PRINT_ALL_FUNC(BT_INFO_PRE str,0,PRINT_STRING_ONLY)
#define BT_PRINT_INFO_STR(str)           PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define BT_PRINT_INFO_INT(str,data)      PRINT_ALL_FUNC(BT_INFO_PRE str,data,PRINT_STRING_AND_DATA)

//��ͨ�����Ľ��롢�˳��������ȵ�����Ϣ������bugʱ��
#define BT_PRINT_DBG(str)                //PRINT_ALL_FUNC(BT_DBG_PRE str,0,PRINT_STRING_ONLY)
#define BT_PRINT_DBG_STR(str)            //PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define BT_PRINT_DBG_INT(str,data)       //PRINT_ALL_FUNC(BT_DBG_PRE str,data,PRINT_STRING_AND_DATA)

//��ӡ ������
#define BT_PRINT_DATA(buf,len)           do{ \
    PRINT_ALL_FUNC(BT_DATA_PRE,0,PRINT_STRING_ONLY); \
    PRINT_ALL_FUNC(buf,len,PRINT_HEX_BYTES); \
}while(0)

/******************************************
BT CONTROLLER DRV��ӡ
*******************************************/
#define BT_DRV_PRINT_INFO(str)               PRINT_ALL_FUNC(BTDRV_INFO_PRE str,0,PRINT_STRING_ONLY)


/******************************************
DRV������ӡ
*******************************************/
//err��Ϣ����������Զ�򿪣����ǿռ䲻����
#define PRINTD_ERR(str)               PRINT_ALL_FUNC(DRV_ERR_PRE str,0,PRINT_STRING_ONLY)
#define PRINTD_ERR_STR(str)           PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINTD_ERR_INT(str,data)      PRINT_ALL_FUNC(DRV_ERR_PRE str,data,PRINT_STRING_AND_DATA)

//������Ϣ���û�����AP�õ��Ľ��٣���Ҫ��ϵͳʹ�ã��������
#define PRINTD_WARNING(str)           PRINT_ALL_FUNC(DRV_WARN_PRE str,0,PRINT_STRING_ONLY)
#define PRINTD_WARNING_STR(str)       PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINTD_WARNING_INT(str,data)  PRINT_ALL_FUNC(DRV_WARN_PRE str,data,PRINT_STRING_AND_DATA)

//�ؼ�·�����㷨����Ҫ��Ϣ
#define PRINTD_INFO(str)              PRINT_ALL_FUNC(DRV_INFO_PRE str,0,PRINT_STRING_ONLY)
#define PRINTD_INFO_STR(str)          PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINTD_INFO_INT(str,data)     PRINT_ALL_FUNC(DRV_INFO_PRE str,data,PRINT_STRING_AND_DATA)

//��ͨ�����Ľ��롢�˳��������ȵ�����Ϣ������bugʱ��
#define PRINTD_DBG(str)               //PRINT_ALL_FUNC(DRV_DBG_PRE str,0,PRINT_STRING_ONLY)
#define PRINTD_DBG_STR(str)           //PRINT_ALL_FUNC(str,0,PRINT_STRING_ONLY)
#define PRINTD_DBG_INT(str,data)      //PRINT_ALL_FUNC(DRV_DBG_PRE str,data,PRINT_STRING_AND_DATA)

//��ӡ ������
#define PRINTD_DATA(buf,len)          do{ \
    PRINT_ALL_FUNC(DRV_DATA_PRE,0,PRINT_STRING_ONLY); \
    PRINT_ALL_FUNC(buf,len,PRINT_HEX_BYTES); \
}while(0)


/******************************************
�жϴ�ӡ
*******************************************/
#define DEBUG_PRINT_IRQ(str,data,mode)    PRINT_IRQ_FUNC(str,data,mode)

#endif //#ifndef __DEBUG_INTERFACE_H__

