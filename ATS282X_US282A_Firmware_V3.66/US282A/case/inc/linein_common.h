/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-08 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __LINEIN_COMMON_H__
#define __LINEIN_COMMON_H__

#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_pp.h"

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
    /*! ֹͣ */
    linein_play_status_stop = 0,
    /*! ���� */
    linein_play_status_play = 2,
} linein_play_status_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */
    linein_play_status_e play_status;
    /*! �����ֽڣ�4�ֽڶ���Ҫ�� */
    uint8 reserve[3];
} linein_eg_status_t;

#endif //__LINEIN_COMMON_H__
