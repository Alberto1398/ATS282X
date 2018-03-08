/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-14          1.0              create this file
 *******************************************************************************/

#include "ap_udisk.h"

#if (SUPPORT_LED_DRIVER == 1)

/******************************************************************************/
/*!
 * \par  Description:
 * \void ud_paint_deal(void)
 * \param[in]    ��ʾ����
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      usound_paint_deal.c
 * \note
 *******************************************************************************/
void ud_paint_deal(void)
{
    led_clear_screen();

    led_display(NUMBER1, NUM_U, 1);
    led_display(NUMBER2, NUM_S, 1);
    led_display(NUMBER3, NUM_B, 1);
    led_display(NUMBER4, NUM_H, 1);
}

#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void udisk_main_view(view_update_e mode)
 * \param[in]  ˢ������ͼ
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      udisk_paint_deal.c
 * \note
 *******************************************************************************/
void udisk_main_view(view_update_e mode)
{
    switch (mode)
    {

        case VIEW_UPDATE_CREATE:

        case VIEW_UPDATE_REDRAW:

        case VIEW_UPDATE_STATUS:
#if (SUPPORT_LED_DRIVER == 1)
        if (g_comval.support_led_display == 1)        	
        {
            ud_paint_deal();
        }
#endif

        break;

        case VIEW_UPDATE_DESTROY:
        {
        }
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void usound_create_main_view(void)
 * \��������ͼ
 * \param[in]
 * \param[in]
 * \param[out]   none
 * \return       void
 * \retval       none
 * \retval       none
 * \ingroup      udisk_paint_deal.c
 * \note
 */
/********************************************************************************/
void udisk_create_main_view(void)
{

    create_view_param_t create_view_param;

    create_view_param.type = VIEW_TYPE_MAIN;

    create_view_param.unknown_key_deal = UNKNOWN_KEY_IGNORE;

    create_view_param.overtime = 0;

    create_view_param.ke_maplist = udisk_ke_maplist;
    create_view_param.se_maplist = udisk_se_maplist;

    create_view_param.view_cb = udisk_main_view;

    com_view_add(APP_VIEW_ID_MAIN, &create_view_param);

}

