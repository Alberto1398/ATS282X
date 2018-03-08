//����Ŀ¼�б���ļ�����
//ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�(��ΪĿ¼�б��������ڴ���)
#include "eh_fsel_entry.h"
#include "dir_list.h"

//��ȡ��ǰĿ¼����һ���ļ�
//strfile ��׺��
bool fsel_list_prev(uint8* strfile)
{
    uint16 temp_num;

    //�ļ�����Ϊ0
    if (eh_file_total == 0)
    {
        return FALSE;
    }
    //�����shuffle
    if (TRUE == eh_shuffle_sw)
    {
        //shuffle
        temp_num = fsel_shuffle_prev_file();
        if (temp_num == 0)
        {
            return FALSE;
        }
        return fsel_list_get_byno_all(strfile, temp_num, temp_num);
    }

    if (eh_file_no == 1)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_file_no == 0)
    {
        //��ʾ�ս����Ŀ¼
        eh_file_no = eh_file_total;

        //�����Ŀ¼,���Ҹ�Ŀ¼���һ���ļ�
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, PREV_FLAG, plist_info, fsel_init_val.file_type_bit);

    }
    else
    {
        eh_file_no--;
        //�ӵ�ǰλ�ò���ǰһ���ļ�
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, PREV_FLAG | 0x01, plist_info,
                fsel_init_val.file_type_bit);
    }
    if (eh_cur_ext_name != 0)
    {
        if (strfile != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strfile = eh_cur_ext_name;
        }
        return TRUE;
    }

    fsel_error = FSEL_ERR_FAIL;
    return FALSE;
}

//��ȡ��һ���ļ�,�����л�Ŀ¼
//strfile ��׺��
bool fsel_list_prev_layer(uint8 *strfile)
{
    bool jump_dir_ret = TRUE;

    list_dir_info_t* cur_dir_ptr = plist_info;

    //ȡ�õ�ǰĿ¼�ļ�����
    if (eh_file_total == 0)
    {
        eh_file_total = fsel_total_file_curdir();
    }

    if (FALSE == fsel_list_prev(strfile))
    {
        if (eh_dir_no == 1)
        {
            return FALSE;
        }

        eh_dir_no--;

        //������һ���ļ���
        plist_info--;

        jump_dir_ret = vfs_jump_to_direntry(eh_vfs_mount, PREV_FLAG | 0x01, plist_info, cur_dir_ptr);

        if (jump_dir_ret == TRUE)
        {
            //��ø����ļ��е��ļ�����
            eh_file_total = fsel_total_file_curdir();
            eh_file_no = 0;
            //��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
            if ((eh_file_total > 0) && (fsel_list_prev(strfile) != FALSE))
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
    return jump_dir_ret;
}

//�����ظ�ģʽ����ȡ��һ���ļ�
//strfile ��׺��
bool fsel_list_get_prev(uint8 *strfile)
{
    bool ret_val;

    switch (fsel_init_val.fsel_mode)
    {
        case FSEL_MODE_LOOPONE:
        if (eh_file_total == 0)
        {
            ret_val = FALSE;
        }
        else if (eh_file_no == 0)
        {
            //ȡ��һ���ļ�
            ret_val = fsel_list_get_byno_all(strfile, 1, 1);
        }
        else
        {
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
        //ȫ���ظ�
        case FSEL_MODE_LOOPALL:
        if ((eh_file_no == 0) && (eh_dir_no == 0))
        {
            goto get_init_prev;
        }

        if (FALSE != fsel_list_prev_layer(strfile))
        {
            ret_val = TRUE;
        }
        else //������ǰ��һ�����л�������Ŀ¼���һ���ļ�
        {
            get_init_prev:

            if (TRUE == eh_shuffle_sw)
            {
                fsel_init_shuffle(eh_file_total_all);
                //��֤shuffle�ķ�����ȷ
                eh_file_no_all = eh_file_total_all;
            }

            //��λ�����һ���ļ���Ŀ¼��λ��
            fsel_last_list_entry();

            ret_val = fsel_list_prev_layer(strfile);
        }
        break;

        case FSEL_MODE_LOOPONEDIR:

        //��ǰĿ¼�µ�ǰһ���ļ�
        if (FALSE != fsel_list_prev(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            eh_file_no = 0;

            ret_val = fsel_list_prev(strfile);
        }

        default:

        ret_val = FALSE;
        break;

    }
    //�ļ����
    eh_file_no_all = (eh_file_no + plist_info->begin_index - 1);
    return ret_val;
}

