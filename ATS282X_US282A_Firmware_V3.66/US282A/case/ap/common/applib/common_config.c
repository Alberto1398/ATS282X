/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������������ģ�飬���������ýű���SD���ļ�������ȡ������ȹ��ܽӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ӧ�����ýű��ļ�������ȡ���������ļ�ͷ��
 * \param[in]    config_file Ӧ�����ýű��ļ�����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��Ӧ�����ýű��ɹ���
 * \retval           FALSE ��Ӧ�����ýű�ʧ��
 * \ingroup      common_func
 * \note
 * \li  �ýӿ�ֻ�� manager ap�� globe_data_init �е���һ�Σ�Ҳ����˵��ϵͳ����ʱ
 *      ����һ�δ򿪳�ʼ����֮���κεط���ȡӦ��������ʱ������򿪡�
 *******************************************************************************/
bool com_open_config_file(const char* config_file)
{
    uint8 head[16];

    config_fp = sys_sd_fopen(config_file);
    if (config_fp == NULL)
    {
        return FALSE;
    }

    if (sys_sd_fread(config_fp, head, 16) == -1)
    {
        return FALSE;
    }

    if ((head[0] != 'c') || (head[1] != 'f') || (head[2] != 'g'))
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ر�Ӧ�����ýű��ļ���
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �ر�Ӧ�����ýű��ɹ���
 * \retval           FALSE �ر�Ӧ�����ýű�ʧ�ܡ�
 * \ingroup      common_func
 * \note
 *******************************************************************************/
#if 0
bool com_close_config_file(void)
{
    if (sys_sd_fclose(config_fp) == -1)
    {
        return FALSE;
    }

    return TRUE;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡӦ�������
 * \param[in]    config_id ���ò���ID
 * \param[in]    buf_len ���������������С
 * \param[out]   buf �������������
 * \return       bool
 * \retval           TRUE ��ȡ������ɹ���
 * \retval           FALSE ��ȡ������ʧ�ܡ�
 * \ingroup      common_func
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1����ȡ��������ʱ������ṹ��
 if(com_get_config_struct(CONFIG_ID_LIGHT_TIME, config_buf, CONFIG_BUF_LEN) == TRUE)
 {
 config_nonlinear_t *config_param;

 config_param = (config_nonlinear_t *)config_buf;
 Ȼ��Ϳ��Զ� config_param �� config_nonlinear_t �ṹ���з���
 }
 else
 {
 û���ҵ���Ӧ��������
 }
 * \endcode
 * \note
 * \li  ȡ�ɱ䳤�� config_string_t �� config_nonlinear_t�������ȸ���ʹ�ó����������
 *      ��config������ uint8 ����������ȡ���ݣ���ȡ�ɹ���Ѹû�����ǿ��ת��Ϊ
 *      config_string_t �� config_nonlinear_t ����ָ�룬Ȼ��Ϳ��԰��� config_string_t
 *      �� config_nonlinear_t���ͽ��з��ʡ�
 *******************************************************************************/
bool com_get_config_struct(uint16 config_id, uint8 *buf, uint16 buf_len)
{
    config_index_t temp_index;

    //��ȡ�����������ṹ��
    if (sys_sd_fseek(config_fp, SEEK_SET, 16 + config_id * sizeof(config_index_t)) == -1)
    {
        return FALSE;
    }
    if (sys_sd_fread(config_fp, &temp_index, sizeof(config_index_t)) == -1)
    {
        return FALSE;
    }

    //��ȡ����������
    if (sys_sd_fseek(config_fp, SEEK_SET, temp_index.offset) == -1)
    {
        return FALSE;
    }
    if (temp_index.length > buf_len)
    {
        if (sys_sd_fread(config_fp, buf, buf_len) == -1)
        {
            return FALSE;
        }
    }
    else
    {
        if (sys_sd_fread(config_fp, buf, temp_index.length) == -1)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡӦ��������Ĭ��ֵ��
 * \param[in]    config_id ���ò���ID
 * \param[out]   none
 * \return       uint16
 * \retval           ���ض�ȡ����Ӧ��������Ĭ��ֵ��
 * \ingroup      common_func
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1����ȡ��������ʱ��Ĭ��ֵ
 light_time = com_get_config_default(CONFIG_ID_LIGHT_TIME);
 * \endcode
 * \note
 * \li  ��������ֵ�Ͳ����������û���֤ config_id ��ȷ�����Ҳ����������򷵻�0��
 *******************************************************************************/
uint16 com_get_config_default(uint16 config_id)
{
    config_index_t temp_index;
    uint16 default_value;

    //��ȡ�����������ṹ��
    if (sys_sd_fseek(config_fp, SEEK_SET, 16 + config_id * sizeof(config_index_t)) == -1)
    {
        return 0;
    }
    if (sys_sd_fread(config_fp, &temp_index, sizeof(config_index_t)) == -1)
    {
        return 0;
    }

    //��ȡ����������
    if (sys_sd_fseek(config_fp, SEEK_SET, temp_index.offset) == -1)
    {
        return 0;
    }
    if (sys_sd_fread(config_fp, &default_value, sizeof(uint16)) == -1)
    {
        return 0;
    }

    return default_value;
}

/*! \endcond */
