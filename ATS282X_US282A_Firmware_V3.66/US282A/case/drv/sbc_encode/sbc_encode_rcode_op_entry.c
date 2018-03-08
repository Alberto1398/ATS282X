/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������sbc������������ӿ���ڡ�
 * ���ߣ�kehaitao
 ********************************************************************************/

#include "sbc_encode.h"

typedef void* (*sbc_op_i)(void*, void*, void*);
/*1.�����ڲ�ʵ�ֵľ���ӿں�������*/
typedef struct 
{
    sbc_op_i __sbc_init;
    sbc_op_i __sbc_finish;
    sbc_op_i __encode;
}block_sbc_operations;

/*2.����ӿڲ������ṹ�壬�����г�ʼ��*/
block_sbc_operations blk_op =
{
    (sbc_op_i)sbc_init,
    (sbc_op_i)sbc_finish,
    (sbc_op_i)sbc_encode_entry,
};


