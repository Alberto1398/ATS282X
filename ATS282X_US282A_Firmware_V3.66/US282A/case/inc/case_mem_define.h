/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE ʹ�õ����ݿռ䶨��
 * ���ߣ�cailizhen
 ********************************************************************************/
#ifndef __CASE_MEM_DEFINE_H
#define __CASE_MEM_DEFINE_H

#define ASET_READ_DATA_BUF         		(g_aset_rw_buffer)

#define ASET_WRITE_DATA_BUF_WITH_HEAD 	(g_aset_rw_buffer)
#define ASET_WRITE_DATA_BUF        		(ASET_WRITE_DATA_BUF_WITH_HEAD + 8)

#define ASET_RW_DATA_BUF_WITH_HEAD      (g_aset_rw_buffer)
#define ASET_RW_DATA_BUF                (ASET_RW_DATA_BUF_WITH_HEAD + 8)


#endif
