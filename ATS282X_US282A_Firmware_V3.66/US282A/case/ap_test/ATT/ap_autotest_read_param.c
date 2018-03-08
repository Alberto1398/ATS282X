/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     user1_main.c
 * \brief    picture��ģ�飬������̳�ʼ�����˳�������������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_manager_test.h"

static uint32 _check_bt_addr_valid(uint8 *bt_addr)
{
    uint32 i;

    for (i = 0; i < 6; i++)
    {
        if ((bt_addr[i] != 0) && (bt_addr[i] != 0xff))
        {
            break;
        }
    }

    if (i == 6)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

//�ò��������ڶ�ȡ������ַ���ṩ��PC���߽���У�飬ȷ�ϵ�ַ�Ƿ���ȷ
int act_test_read_bt_addr(void *arg_buffer)
{
    uint32 i;
    uint8 cmd_data[16];
    int ret_val;
    return_result_t *return_data;
    int trans_bytes;
    nvram_param_rw_t param_rw;
    uint32 bt_addr_valid;
   
    bt_addr_vram_t bt_addr_vram;

    bt_addr_valid = TRUE;
    
    if(g_support_norflash_wp == TRUE)
    {
        sys_vm_read(&bt_addr_vram, VM_BTSTACK, sizeof(bt_addr_vram));
    
        if (bt_addr_vram.magic != VRAM_MAGIC(VM_BTSTACK))
        {
            param_rw.logical_index = PARAM_BT_ADDR;
            param_rw.rw_len = 6;
            param_rw.rw_buffer = bt_addr_vram.bt_addr;
    
            ret_val = base_param_read(&param_rw);
    
            if (ret_val != 0)
            {
                bt_addr_valid = FALSE;
            }
        }
   }
   else
   {
        param_rw.logical_index = PARAM_BT_ADDR;
        param_rw.rw_len = 6;
        param_rw.rw_buffer = bt_addr_vram.bt_addr;

        ret_val = base_param_read(&param_rw);

        if (ret_val != 0)
        {
            sys_vm_read(&bt_addr_vram, VM_BTSTACK, sizeof(bt_addr_vram));
        
            if (bt_addr_vram.magic != VRAM_MAGIC(VM_BTSTACK))
            {
                bt_addr_valid = FALSE;    
            }            
        }
         
   }
    if(bt_addr_valid = TRUE)
    {
        if (_check_bt_addr_valid(bt_addr_vram.bt_addr) == TRUE)
        {
            ret_val = 1;
        }
        else
        {
            ret_val = 0;
        }
    }
    else
    {
        ret_val = 0;    
    }

    if (ret_val == 1)
    {
        for (i = 0; i < 6; i++)
        {
            cmd_data[i] = bt_addr_vram.bt_addr[i];
        }
        
        if(g_test_mode != TEST_MODE_CARD)
        {    
            print_log("bt addr: %x:%x:%x:%x:%x:%x\n", cmd_data[5], cmd_data[4], cmd_data[3], cmd_data[2], cmd_data[1],
                cmd_data[0]);
        }
        else
        {
            for (i = 0; i < 6; i++)
            {
                att_write_test_info("addr: ", cmd_data[5 - i], 1);
            }                
        }
    }
    else
    {
        libc_memset(cmd_data, 0, sizeof(cmd_data));
    }

    if (g_test_mode != TEST_MODE_CARD)
    {
        return_data = (return_result_t *) (STUB_ATT_RETURN_DATA_BUFFER);

        return_data->test_id = TESTID_READ_BTADDR;

        return_data->test_result = ret_val;

        trans_bytes = 0;

        //������ַתunicode��ʽ
        bytes_to_unicode(cmd_data, 5, 6, return_data->return_arg, &trans_bytes);

        //��ӽ�����
        return_data->return_arg[trans_bytes++] = 0x0000;

        //�������δ���ֽڶ��룬Ҫ���ֽڶ��봦��
        if ((trans_bytes % 2) != 0)
        {
            return_data->return_arg[trans_bytes++] = 0x0000;
        }

        act_test_report_result(return_data, trans_bytes * 2 + 4);
    }
    else
    {
        act_test_report_test_log(ret_val, TESTID_READ_BTADDR);
    }
    
    return ret_val;
}

static int cmp_bt_name(uint8 *bt_name0, uint8 *bt_name1, uint32 cmp_len)
{
    uint32 i;

    for (i = 0; i < cmp_len; i++)
    {
        if (bt_name0[i] != bt_name1[i])
        {
            break;
        }

        if (bt_name0[i] == '\0')
        {
            return TRUE;
        }
    }

    if (i < cmp_len)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static int32 utf8str_to_unicode(uint8 *utf8, uint32 utf8Len, uint16 *unicode, uint32 *unicode_len)
{
    int32 count = 0;
    uint8 c0, c1;
    uint32 scalar;

    while (--utf8Len >= 0)
    {
        c0 = *utf8;
        utf8++;

        if (c0 < 0x80)
        {
            *unicode = c0;

            if (*unicode == 0x00)
            {
                //count += 2;
                break;
            }
            unicode++;
            count += 2;
            continue;
        }

        /*��ascii�ַ�������ĵ�һ���ֽڲ�Ӧ��Ϊ10xxxxxx*/
        if ((c0 & 0xc0) == 0x80)
        {
            return -1;
        }

        scalar = c0;
        if (--utf8Len < 0)
        {
            return -1;
        }

        c1 = *utf8;
        utf8++;
        /*��2���ֽڵı����ʽӦ��Ϊ10xxxxxx*/
        if ((c1 & 0xc0) != 0x80)
        {
            return -1;
        }
        scalar <<= 6;
        scalar |= (c1 & 0x3f);

        /*���r0 & 0x20����0��˵����utf���볤��Ϊ2����������볤�ȴ���2*/
        if (!(c0 & 0x20))
        {
            if ((scalar != 0) && (scalar < 0x80))
            {
                /*unicode����С��0x80��utf8���볤��ȴΪ2��over encoding*/
                return -1;
            }
            *unicode = (unsigned short) scalar & 0x7ff;
            if (*unicode == 0x00)
            {
                count += 2;
                break;
            }
            unicode++;
            count += 2;
            continue;
        }

        if (--utf8Len < 0)
        {
            return -1;
        }

        c1 = *utf8;
        utf8++;
        /*��3���ֽڵı����ʽӦ��Ϊ10xxxxxx*/
        if ((c1 & 0xc0) != 0x80)
        {
            return -1;
        }
        scalar <<= 6;
        scalar |= (c1 & 0x3f);

        /*���r0 & 0x10����0��˵����utf���볤��Ϊ3����������볤�ȴ���3*/
        if (!(c0 & 0x10))
        {
            if (scalar < 0x800)
            {
                return -1;
            }
            if ((scalar >= 0xd800) && (scalar < 0xe000))
            {
                return -1;
            }
            *unicode = (unsigned short) scalar & 0xffff;
            if (*unicode == 0x00)
            {
                count += 2;
                break;
            }
            unicode++;
            count += 2;
            continue;
        }

        return -1;
    }

    //��֤�ַ������ֽڶ���
    if ((count % 4) != 0)
    {
        unicode++;
        count += 2;
        *unicode = 0x00;
    }

    *unicode_len = count;
    return 0;
}

int act_test_read_bt_name(void *arg_buffer)
{
    uint32 i;

    uint32 ret_val;

    read_btname_test_arg_t *read_btname_arg;

    nvram_param_rw_t param_rw;

    bt_addr_vram_t bt_addr_vram;

    return_result_t *return_data;

    uint32 trans_bytes;

    uint32 count;
    
    uint32 bt_name_valid = TRUE;
    
    uint32 ble_name_valid = TRUE;

    read_btname_arg = (read_btname_test_arg_t *) arg_buffer;

    libc_memset(&bt_addr_vram, 0, sizeof(bt_addr_vram));
    
    if(g_support_norflash_wp == TRUE)
    {
        sys_vm_read(&bt_addr_vram, VM_BTSTACK, sizeof(bt_addr_vram));

        if (bt_addr_vram.magic != VRAM_MAGIC(VM_BTSTACK))
        {
            param_rw.logical_index = PARAM_BT_NAME;
            param_rw.rw_len = sizeof(bt_addr_vram.device_name);
            param_rw.rw_buffer = &(bt_addr_vram.device_name[0]);
    
            //��ȡ��������
            ret_val = base_param_read(&param_rw);
    
            if (ret_val != 0)
            { 
                bt_name_valid = FALSE;         
            }  
            param_rw.logical_index = PARAM_BT_BLENAME;
            param_rw.rw_len = sizeof(bt_addr_vram.ble_device_name);
            param_rw.rw_buffer = &(bt_addr_vram.ble_device_name[0]);

            //��ȡ����BLE����
            ret_val = base_param_read(&param_rw);

            if (ret_val != 0)
            {
                ble_name_valid = FALSE;
            }         
        }
    }
    else
    {
        param_rw.logical_index = PARAM_BT_NAME;
        param_rw.rw_len = sizeof(bt_addr_vram.device_name);
        param_rw.rw_buffer = &(bt_addr_vram.device_name[0]);

        //��ȡ��������
        ret_val = base_param_read(&param_rw);

        param_rw.logical_index = PARAM_BT_BLENAME;
        
        param_rw.rw_len = sizeof(bt_addr_vram.ble_device_name);
        param_rw.rw_buffer = &(bt_addr_vram.ble_device_name[0]);

        //��ȡ����BLE����
        ret_val += base_param_read(&param_rw);       

        if (ret_val != 0)
        {  
            sys_vm_read(&bt_addr_vram, VM_BTSTACK, sizeof(bt_addr_vram));
    
            if (bt_addr_vram.magic != VRAM_MAGIC(VM_BTSTACK))
            {
                bt_name_valid = FALSE; 
                ble_name_valid = FALSE;   
            }
        }          
    }
 
    ret_val = FALSE;
    
    if(bt_name_valid == TRUE)
    {
        if (bt_addr_vram.device_name[0] != 0)
        {
            if(ble_name_valid == TRUE)
            {    
                if (bt_addr_vram.ble_device_name[0] != 0)
                {
                    ret_val = TRUE;
                }
            }    
        }
    }

    if (ret_val == TRUE)
    {
        if(g_test_mode != TEST_MODE_CARD)
        {
            print_log("read bt name: %s\n", bt_addr_vram.device_name);
    
            print_log("read ble name: %s\n", bt_addr_vram.ble_device_name);
        }
        else
        {
            att_write_test_info("read bt name: ", bt_addr_vram.device_name, 1);
            
            att_write_test_info("read ble name: ", bt_addr_vram.ble_device_name, 1);        
        }
    }

    if (read_btname_arg->cmp_btname_flag == TRUE)
    {
        ret_val = cmp_bt_name(bt_addr_vram.device_name, read_btname_arg->cmp_btname, sizeof(bt_addr_vram.device_name));
    }

    if (ret_val == TRUE)
    {
        if (read_btname_arg->cmp_blename_flag == TRUE)
        {
            ret_val = cmp_bt_name(bt_addr_vram.ble_device_name, read_btname_arg->cmp_blename,
                    sizeof(bt_addr_vram.ble_device_name));
        }

    }

    if (g_test_mode != TEST_MODE_CARD)
    {
        trans_bytes = 0;
        
        return_data = (return_result_t *) (STUB_ATT_RETURN_DATA_BUFFER);

        return_data->test_id = TESTID_READ_BTNAME;

        return_data->test_result = ret_val;

        //utf8תunicode���������buffer�϶࣬�����������֮ǰ�ַ���buffer��2��
        //�����Ҫһ����һ���buffer�����ַ���
        utf8str_to_unicode(bt_addr_vram.device_name, sizeof(bt_addr_vram.device_name), return_data->return_arg, &count); 

        trans_bytes += (count >> 1);

        //��Ӳ����ָ���','
        return_data->return_arg[trans_bytes++] = 0x002c;   

        //utf8תunicode���������buffer�϶࣬�����������֮ǰ�ַ���buffer��2��
        //�����Ҫһ����һ���buffer�����ַ���
        utf8str_to_unicode(bt_addr_vram.ble_device_name, sizeof(bt_addr_vram.ble_device_name), &(return_data->return_arg[trans_bytes]), &count);        

        trans_bytes += (count >> 1);
                
        //�������δ���ֽڶ��룬Ҫ���ֽڶ��봦��
        if ((trans_bytes % 2) != 0)
        {
            return_data->return_arg[trans_bytes++] = 0x0000;
        }        
        act_test_report_result(return_data, trans_bytes * 2 + 4);
    }
    else
    {
        act_test_report_test_log(ret_val, TESTID_READ_BTNAME);
    }
    
    return ret_val;
}
