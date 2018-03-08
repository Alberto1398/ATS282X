/*! \cond MUSIC_SCAN_API*/
/********************************************************************************
 *                              5110
 *                            Module: image_decode
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>       <time>           <version >             <desc>
 *      cailizhen    2011-08-27           1.0             build this file
 ********************************************************************************/
/*!
 * \file     music_scan_cmd.h
 * \brief    ����MUSICĿ¼ɨ������ID
 * \author   cailizhen
 * \version  1.0
 * \date  2011/09/19
 *******************************************************************************/
/*! \endcond*/
#ifndef __MUSIC_SCAN_CMD_H__
#define __MUSIC_SCAN_CMD_H__

/* MIPS */
#include "psp_includes.h"

/*! \cond MMM_ID_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�ò��ͼƬ��������
 */
typedef enum {
	/*! ��MUSIC SCAN */
	MUSIC_SCAN_OPEN = 0,
	/*! �ر�MUSIC SCAN */
	MUSIC_SCAN_CLOSE,
	/*! ��ȡ״̬ */
	MUSIC_SCAN_GET_STATUS,
} music_scan_cmd_t;


/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
extern int music_scan_cmd(void *ms_handle, music_scan_cmd_t cmd, unsigned int param)__FAR__;

#endif // __MUSIC_SCAN_CMD_H__
