/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ�������ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "music_common.h"
#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ������ر���Ƶ�����
 * \param[in]    state �������״̬���� soundout_state_e ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ������ر���Ƶ����ɹ���
 * \retval           FALSE ������ر���Ƶ���ʧ�ܡ�
 * \ingroup      common_func
 * \note
 * \li  �ر���Ƶ���ʱ�����������ʹ�ܣ�����а������������á�
 *******************************************************************************/
bool com_set_sound_out(soundout_state_e state)
{
    //������Ƶ���״̬
    change_soundout_state(state);

    return TRUE;
}

/*! \endcond */

