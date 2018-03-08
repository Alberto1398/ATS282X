//������Ŀ¼�б���ļ��Ĳ���
#include "eh_fsel_entry.h"

//��ȡ��ǰĿ¼����һ���ļ�
//strfile ��׺��
bool fsel_dir_prev(uint8* strfile)
{
    uint8 dir_type = DIR_PREV;
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
            fsel_init_shuffle(eh_file_total_all);
            return FALSE;
        }
        return fsel_dir_get_byno_all(strfile, temp_num);
    }

    //��ʾǰһ���Ѿ�Ŀ¼�ĵ�һ��
    if (eh_file_no == 1)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_file_no == 0) //��ʾ�ս����Ŀ¼
    {
        eh_file_no = eh_file_total;
        //��λ�����һ��Ŀ¼��
        dir_type = DIR_TAIL;
    }
    else
    {
        eh_file_no--;
    }

    //dir�ļ�
    eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
    if (eh_cur_ext_name != 0)
    {
        if ((eh_file_no_all == 0) || (eh_file_no_all == 1))
        {
            //�����δ��ȡ�����ļ���,��ɨ�����
            if (eh_file_total_all == 0)
            {
                eh_file_total_all = fsel_get_all_dirs_total_files();
            }
            eh_file_no_all = eh_file_total_all;
        }
        else
        {
            //���������ʾ
            eh_file_no_all--;
        }
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

//��λ����ǰĿ¼�����һ���ļ���
bool fsel_last_dir(void)
{
    //Ŀ¼�������㼶,����ʧ��
    if (eh_dir_layer >= (DIRLAYER_MAX - 1))
    {
        return FALSE;
    }
    // if no dir ,return error
#ifdef FILTER_RECORD_DIR
    if ((0 == vfs_dir(eh_vfs_mount, DIR_TAIL, NULL, EH_GET_EXT_DIR)) || (TRUE == filtrate_dir(DIR_PREV)))
#else
    if (0 == vfs_dir(eh_vfs_mount, DIR_TAIL, NULL, EH_GET_EXT_DIR))
#endif
    {
        return FALSE;
    }

    return TRUE;
}

//��ȡ��һ���ļ�,�����л�Ŀ¼
//strfile ��׺��
bool fsel_dir_prev_layer(uint8 *strfile)
{
    //ȡ�õ�ǰĿ¼�ļ�����
    if (eh_file_total == 0)
    {
        eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);
    }

    if (FALSE == fsel_dir_prev(strfile))
    {
        //��鸸Ŀ¼
        ParentDiragainNext:
        //����Ǹ�Ŀ¼�򷵻�
        if (eh_dir_layer == 0)
        {
            return FALSE;
        }
        //����һ��
        if (FALSE != vfs_cd(eh_vfs_mount, CD_BACK, NULL))
        {
            eh_dir_layer--;
#ifdef FILTER_RECORD_DIR
            //����Ŀ¼
            if ((0 != vfs_dir(eh_vfs_mount, DIR_PREV, NULL, EH_GET_EXT_DIR)) && (FALSE == filtrate_dir(DIR_PREV)))
#else 
            if (0 != vfs_dir(eh_vfs_mount, DIR_PREV, NULL, EH_GET_EXT_DIR))
#endif
            {
                //������Ŀ¼
                vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                //Ŀ¼�㼶
                eh_dir_layer++;
                //��λ����ǰĿ¼�����һ���ļ���
                while (FALSE != fsel_last_dir())
                {
                    //������Ŀ¼
                    vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                    eh_dir_layer++;
                    eh_file_total = 0;
                    eh_file_no = 0;
                }
                //��ȡ��ǰĿ¼�µ��ļ�����
                eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);
                if (eh_file_total != 0)
                {
                    eh_file_no = 0;
                    //ȡ�õ�ǰĿ¼�µ��ļ�
                    return fsel_dir_prev(strfile);
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }
            else
            {
                //��ǰĿ¼���������ļ��У���ʼ���ļ�
                eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);
                if (eh_file_total != 0)
                {
                    eh_file_no = 0;
                    return fsel_dir_prev(strfile);
                }
                goto ParentDiragainNext;
            }
        }
        else
        {
            //������
            fsel_init_sysdata();
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
    }
    return TRUE;
}

//�����ظ�ģʽ��ȡ��һ���ļ�,���ܻ��л�Ŀ¼
//strfile ��׺��
bool fsel_dir_get_prev(uint8 *strfile)
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
            ret_val = fsel_dir_get_byno(strfile, 1);
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
        if ((eh_file_no == 0) && (eh_dir_layer == 0))
        {
            goto prev_init_get;
        }

        if (FALSE != fsel_dir_prev_layer(strfile))
        {
            ret_val = TRUE;
        }
        else //������ǰ��һ�����л�������Ŀ¼���һ���ļ�
        {
            prev_init_get: fsel_init_sysdata();
            //��λ����ǰĿ¼�����һ���ļ���Ŀ¼��λ��
            while (FALSE != fsel_last_dir())
            {
                vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                eh_dir_layer++;
                eh_file_total = 0;
                eh_file_no = 0;
            }
            ret_val = fsel_dir_prev_layer(strfile);
        }
        break;

        case FSEL_MODE_LOOPONEDIR:
        //��ǰĿ¼�µ�ǰһ���ļ�
        if (FALSE != fsel_dir_prev(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            eh_file_no = 0;

            ret_val = fsel_dir_prev(strfile);
        }
        break;

        default:
        ret_val = FALSE;
        break;
    }

    return ret_val;
}

//��λ����һ��Ŀ¼�����һ��
//strdir �ļ���׺��
bool fsel_dir_prev_dir(uint8 *strdir)
{
    //file noΪ��Ŀ¼���һ�׸�
    //ǰһ��Ŀ¼���һ���ļ�
    eh_file_no_all -= (eh_file_no - 1);

    //��λ����ǰĿ¼��һ��,�л�����
    eh_file_no = 1;

    if (fsel_dir_get_prev((uint8*) &eh_cur_ext_name) == TRUE)
    {
        if (eh_cur_ext_name != 0)
        {
            if (strdir != NULL)
            {
                vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
                *(uint32*) strdir = eh_cur_ext_name;
            }
            return TRUE;
        }
    }
    //���û���ҵ��򷵻�ʧ��
    return FALSE;
}

//����Ŀ¼����λ��ĳ���ļ�
//name_short Ŀ¼����
//name_long Ŀ¼����
//dir_direct ����(��һ�����������һ��)
bool fsel_enter_dir(uint8 *name_short, uint8* name_long, uint8 dir_direct)
{
    bool ret_dir = TRUE;

    //����Ŀ¼
    fsel_init_sysdata();

    //check long name dir
    //ret_dir = vfs_dir(eh_vfs_mount, DIR_HEAD, name_long, EXT_NAME_ALL_DIR);

    //check short name dir
    if (vfs_dir(eh_vfs_mount, DIR_HEAD, name_long, EXT_NAME_ALL_DIR) == FALSE)
    {
        //ret_dir = vfs_dir(eh_vfs_mount, DIR_HEAD, name_short, EXT_NAME_ALL_DIR);
        if (vfs_dir(eh_vfs_mount, DIR_HEAD, name_short, EXT_NAME_ALL_DIR) == FALSE)
        {
            ret_dir = FALSE;
        }
    }
    //enter directory
    if (ret_dir == TRUE)
    {
        //����Ŀ¼
        if (vfs_cd(eh_vfs_mount, CD_SUB, NULL) == FALSE)
        {
            ret_dir = FALSE;
        }
    }

    if (ret_dir == TRUE)
    {
        eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);

        eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_direct, NULL, fsel_init_val.file_type_bit);
        if (eh_cur_ext_name != 0)
        {
            if (dir_direct == DIR_TAIL)
            {
                //��λ�����һ���ļ�
                eh_file_no = eh_file_total;
                eh_file_no_all = eh_file_total;
            }
            else
            {
                //��λ����һ���ļ�
                eh_file_no = 1;
                eh_file_no_all = 1;
            }
            eh_file_total_all = eh_file_total;
        }
        else
        {
            ret_dir = FALSE;
        }
    }
    if (ret_dir == FALSE)
    {
        //�ص���Ŀ¼
        fsel_init_sysdata();
    }
    return ret_dir;
}

