//����Ŀ¼�б���ļ�����
//ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�(��ΪĿ¼�б��������ڴ���)
#include "eh_fsel_entry.h"
#include "dir_list.h"

//��ȡ��ǰĿ¼����һ���ļ�
//strfile ��׺��
//��������������Ŀ¼�б�Ҳ��������PLIST_INFO_ADDR
bool fsel_list_next(uint8* strfile)
{
    uint16 temp_num;

    //��ǰĿ¼�ļ�����Ϊ0
    if (eh_file_total == 0)
    {
        return FALSE;
    }

    //���shuffle��
    if (TRUE == eh_shuffle_sw)
    {
        temp_num = fsel_shuffle_next_file();
        if (temp_num == 0)
        {
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
        return fsel_list_get_byno_all(strfile, temp_num, temp_num);
    }

    //��ʾ��ǰ�������һ��������
    if (eh_file_no == eh_file_total)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_file_no == 0)
    {
        //��ͷ��ʼ
        //�����ļ�ϵͳ�ӿ����dir
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, 0, plist_info, fsel_init_val.file_type_bit);
    }
    else
    {
        //��һ��
        //�����ļ�ϵͳ�ӿ����dir
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, 1, plist_info, fsel_init_val.file_type_bit);
    }
    if (eh_cur_ext_name != 0)
    {
        //dir�ɹ�
        eh_file_no++;
        if (strfile != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strfile = eh_cur_ext_name;
        }
    }
    else
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }
    return TRUE;
}

//��ȡ��һ���ļ�,�����л�Ŀ¼
//strfile ��׺��
bool fsel_list_next_layer(uint8 *strfile)
{
    bool jump_ret = TRUE;
    list_dir_info_t* cur_entry_ptr = plist_info;
    //�ڵ�ǰĿ¼�²����ļ�
    jump_ret = fsel_list_next(strfile);
    if ((jump_ret == FALSE) && (eh_shuffle_sw == TRUE))
    {
        return jump_ret;
    }
    if (jump_ret == FALSE)
    {
        if (eh_dir_no >= plist_head->dir_total)
        {
            return FALSE;
        }
        eh_dir_no++;
        //������һ���ļ���
        plist_info++;

        //�л�����һ��Ŀ¼
        jump_ret = vfs_jump_to_direntry(eh_vfs_mount, 1, plist_info, cur_entry_ptr);
        if (jump_ret == TRUE)
        {
            //��ø����ļ��е��ļ�����
            eh_file_total = fsel_total_file_curdir();
            eh_file_no = 0;
            // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
            if ((eh_file_total > 0) && (fsel_list_next(strfile) != FALSE))
            {
                fsel_error = 0;
                return TRUE;
            }
            else
            {
                fsel_error = FSEL_ERR_EXCEPTION;
                return FALSE;
            }
        }

    }

    return jump_ret;
}

//��λ����һ��Ŀ¼
bool fsel_first_list_entry(void)
{
    list_dir_info_t* cur_locat = plist_info;

    //��λ����һ��Ŀ¼λ��
    eh_dir_no = 1;

    //ָ���һ��Ŀ¼
    plist_info = (list_dir_info_t *) (plist_start_addr + sizeof(list_head_info_t));

    //��һ��Ŀ¼���ļ�����
    eh_file_total = fsel_total_file_curdir();

    eh_file_no = 0;

    //��ת����һ��Ŀ¼
    eh_cur_ext_name = vfs_jump_to_direntry(eh_vfs_mount, PREV_FLAG, plist_info, cur_locat);
    return TRUE;
}

//�����ظ�ģʽ����ȡ��һ���ļ�
//strfile ��׺��
bool fsel_list_get_next(uint8 *strfile)
{
    bool ret_val;

    switch (fsel_init_val.fsel_mode)
    {
        //�ظ���ǰһ���ļ�
        case FSEL_MODE_LOOPONE:

        if (eh_file_no_all == 0)
        {
            //Ĭ��ȡ��һ��
            ret_val = fsel_list_get_byno_all(strfile, 1, 1);
        }
        else
        {
            //֮ǰ��ȡ,ֱ�ӷ���ext
            if (strfile != NULL)
            {
                vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
                *(uint32*) strfile = eh_cur_ext_name;
            }
            ret_val = TRUE;
        }
        break;

        //shuffle��
        case FSEL_MODE_RANOM:
        //�����ļ�˳���ȡ���ظ�
        case FSEL_MODE_LOOPALL:
        if (FALSE != fsel_list_next_layer(strfile))
        {
            ret_val = TRUE;
        }
        else
        {

            if (TRUE == eh_shuffle_sw)
            {
                fsel_init_shuffle(eh_file_total_all);
                //��֤shuffle�ķ�����ȷ
                eh_file_no_all = 0;
            }
            //��λ����һ��Ŀ¼��
            fsel_first_list_entry();
            ret_val = fsel_list_next_layer(strfile);
        }
        break;

        //��ǰĿ¼�ظ�����
        case FSEL_MODE_LOOPONEDIR:

        if (FALSE != fsel_list_next(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            eh_file_no = 0;
            //��ȡ��ǰĿ¼��һ��
            ret_val = fsel_list_next(strfile);
        }
        break;

        default:
        {
            ret_val = FALSE;
            break;
        }
    }
    //�ļ����
    eh_file_no_all = (eh_file_no + plist_info->begin_index - 1);
    return ret_val;
}

