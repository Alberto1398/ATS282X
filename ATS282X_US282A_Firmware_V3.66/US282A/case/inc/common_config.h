/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������������ģ����غ궨�壬�ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _common_config_H_
#define _common_config_H_

#include "psp_includes.h"
#include "case_independent.h"
#include "case_type.h"

/*!
 * \brief
 *  config_index_t �������������ݽṹ
 */
typedef struct
{
    /*! ��������������ļ�ͷ��ƫ�� */
    uint32 offset;
    /*! ���������ݳ��� */
    uint32 length;
} config_index_t;

/*!
 * \brief
 *  config_string_t �ַ������������ݽṹ
 */
typedef struct
{
    /*! �ַ������ݣ��ɱ䳤���飬������txt����һ�£���'\0'���� */
    uint8 value[1];
} config_string_t;

/*!
 * \brief
 *  config_linear_t ������ֵ���������ݽṹ
 */
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ȡֵ�������Сֵ */
    uint16 min;
    /*! ȡֵ��������ֵ */
    uint16 max;
    /*! ���� */
    uint16 step;
} config_linear_t;

/*!
 * \brief
 *  config_nonlinear_t ��������ֵ���������ݽṹ
 */
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��Чֵ���飬�ɱ䳤���� */
    uint16 value[1];
} config_nonlinear_t;

//������ӿ�����
extern bool com_open_config_file(const char* config_file) __FAR__;
extern bool com_close_config_file(void) __FAR__;
extern bool com_get_config_struct(uint16 config_id, uint8 *buf, uint16 buf_len) __FAR__;
extern uint16 com_get_config_default(uint16 config_id) __FAR__; //��������ֵ�Ͳ���

#endif
