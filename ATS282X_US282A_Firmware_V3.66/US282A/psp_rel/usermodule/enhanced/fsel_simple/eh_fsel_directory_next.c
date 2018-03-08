//������Ŀ¼�б���ļ��Ĳ���
#include "eh_fsel_entry.h"

const uint8 root_str[] =
{ 'R', 'O', 'O', 'T', 0 };

#ifdef FILTER_RECORD_DIR
const uint8 rec_dir_name[] = "RECORD  ";

//���˸�Ŀ¼�µ�RECORDĿ¼
//dir_type: ����DIR_NEXT, DIR_HEAD
bool filtrate_dir(uint8 dir_type)
{
    uint8 dir_name_buffer[32];
    uint16 name_len;
    uint8 add_len, i, j;
    uint8 *buf;

    if (eh_dir_layer != 0)
    {
        return FALSE;
    }

    name_len = vfs_get_name(eh_vfs_mount, dir_name_buffer, sizeof(dir_name_buffer) / 2 - 2);

    if ((name_len == 11) && (dir_name_buffer[0] != 0xff))
    {
        //8+3
        buf = dir_name_buffer;
        name_len = 8;
        add_len = 1;
    }
    else
    {
        if ((name_len == 8) && (dir_name_buffer[0] == 0xff))
        {
            //unciode
            buf = dir_name_buffer + 2;
            name_len = 6;
            add_len = 2;
        }
        else
        {
            return FALSE;
        }
    }

    i = 0;
    for (j = 0; j < name_len; j++)
    {
        if (rec_dir_name[j] != buf[i])
        {
            return FALSE;
        }
        i = i + add_len;
    }

    //���˳ɹ�������/��һ���ļ��з���
    if (0 != vfs_dir(eh_vfs_mount, dir_type, NULL, EH_GET_EXT_DIR))
    {
        //�ҵ������ҵ��ļ�������
        return FALSE;
    }
    else
    {
        //δ�ҵ����ܵݹ�����
        return TRUE;
    }

}
#endif

//��ȡ��ǰĿ¼����һ���ļ�
//strfile ��׺��
bool fsel_dir_next(uint8* strfile)
{
    uint8 dir_type;
    uint16 temp_num;

    if (eh_file_total == 0)
    {
        return FALSE;
    }
    //���shuffle����
    if (TRUE == eh_shuffle_sw)
    {
        temp_num = fsel_shuffle_next_file();
        if (temp_num == 0)
        {
            fsel_init_shuffle(eh_file_total_all);
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
        return fsel_dir_get_byno_all(strfile, temp_num);
    }

    //��ʾ��ǰ�������һ��������
    if (eh_file_no == eh_file_total)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_file_no == 0)
    {
        //��ͷ��ʼdir
        dir_type = DIR_HEAD;
    }
    else
    {
        //dir��һ��
        dir_type = DIR_NEXT;
    }
    //�����ļ�ϵͳ�ӿ�
    eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
    if (0 != eh_cur_ext_name)
    {
        eh_file_no++;
        //ֻ��һ��Ŀ¼,���¼���ļ����ŵ�����
        if (eh_file_total_all == eh_file_total)
        {
            eh_file_no_all = eh_file_no;
        }
        else
        {
            //�����ļ����
            eh_file_no_all++;
        }

        if (strfile != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strfile = eh_cur_ext_name;
        }
        return TRUE;
    }
    else
    {
        fsel_error = FSEL_ERR_FAIL;
    }
    return FALSE;
}

//��ȡ��һ���ļ�,�����л�Ŀ¼
//strfile ��׺��
bool fsel_dir_next_layer(uint8 *strfile)
{
    //�ڵ�ǰĿ¼�������ļ�
    if (eh_file_total == 0)
    {
        eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);
        eh_file_no = 0;
    }

    if (fsel_dir_next(strfile) == FALSE)
    {
        CurrentDiragain:

        if (eh_dir_layer >= (DIRLAYER_MAX - 1))
        {
            goto ParentDiragainNext;
        }
#ifdef FILTER_RECORD_DIR
        //Ѱ�Ҹ�Ŀ¼�ĵ�һ�����ļ���
        if ((0 != vfs_dir(eh_vfs_mount, DIR_HEAD, NULL, EH_GET_EXT_DIR)) && (FALSE == filtrate_dir(DIR_NEXT)))
#else
        if (0 != vfs_dir(eh_vfs_mount, DIR_HEAD, NULL, EH_GET_EXT_DIR))
#endif
        {
            if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                //�������ļ��У���ø����ļ��е��ļ�����
                eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);
                eh_dir_layer++;

                eh_file_no = 0;
                // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
                if ((eh_file_total > 0) && (fsel_dir_next(strfile) != FALSE))
                {
                    fsel_error = 0;
                    return TRUE;
                }
                else
                {
                    //�����Ŀ¼
                    goto CurrentDiragain;
                }
            }
            else //������
            {
                fsel_init_sysdata();
                fsel_error = FSEL_ERR_OUTOF;
                return FALSE;
            }
        }
        else //���û�����ļ��У���������һ�㣬Ѱ�ҵ�ǰĿ¼����һ��Ŀ¼��ƽ��Ѱ�ң�
        {
            ParentDiragainNext:

            //��ǰ�Ǹ�Ŀ¼
            if (0 == eh_dir_layer)
            {
                fsel_init_sysdata();
                fsel_error = FSEL_ERR_OUTOF;
                return FALSE;
            }

            if (vfs_cd(eh_vfs_mount, CD_BACK, NULL) != FALSE)
            {
                eh_dir_layer--;
#ifdef FILTER_RECORD_DIR
                if ((vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EH_GET_EXT_DIR) != 0) && (FALSE == filtrate_dir(DIR_NEXT)))
#else
                if (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EH_GET_EXT_DIR) != 0)
#endif
                {
                    vfs_cd(eh_vfs_mount, CD_SUB, NULL);
                    eh_dir_layer++;
                    //�л�Ŀ¼
                    eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);

                    eh_file_no = 0;

                    if ((eh_file_total > 0) && (FALSE != fsel_dir_next(strfile)))
                    {
                        fsel_error = 0;
                        return TRUE;
                    }
                    else
                    {
                        // �����Ŀ¼.
                        goto CurrentDiragain;
                    }
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }
            else
            { //�쳣����
                fsel_init_sysdata();
                fsel_error = FSEL_ERR_EXCEPTION;
                return FALSE;
            }
        }
    }

    return TRUE;
}

//�����ظ�ģʽ��ȡ��һ���ļ�,���ܻ��л�Ŀ¼
//strfile ��׺��
bool fsel_dir_get_next(uint8 *strfile)
{
    bool ret_val;

    switch (fsel_init_val.fsel_mode)
    {
        //�ظ���ǰһ���ļ�
        case FSEL_MODE_LOOPONE:

        if (eh_file_no == 0)
        {
            //Ĭ��ȡ��һ��
            ret_val = fsel_dir_get_byno(strfile, 1);
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
        //�����δ��ȡ�����ļ���
        //�����ļ�˳���ȡ���ظ�
        case FSEL_MODE_LOOPALL:
        if (FALSE != fsel_dir_next_layer(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            fsel_init_sysdata();
            ret_val = fsel_dir_next_layer(strfile);
        }
        break;

        //��ǰĿ¼�ظ�����
        case FSEL_MODE_LOOPONEDIR:

        if (FALSE != fsel_dir_next(strfile))
        {
            ret_val = TRUE;
        }
        else
        {
            eh_file_no = 0;
            //��ȡ��ǰĿ¼��һ��
            ret_val = fsel_dir_next(strfile);
        }
        break;

        default:
        {
            ret_val = FALSE;
            break;
        }
    }
    return ret_val;
}

//��λ����һ��Ŀ¼�ĵ�һ���ļ�
//strdir �ļ���
bool fsel_dir_next_dir(uint8 *strdir)
{
    //uint8 dir_type = DIR_NEXT;

    //file noΪ��Ŀ¼���һ�׸�
    //��һ��Ŀ¼��һ���ļ�
    eh_file_no_all -= eh_file_no;
    eh_file_no_all += eh_file_total;

    //��λ����ǰĿ¼���һ��,�л�����
    eh_file_no = eh_file_total;

    if (fsel_dir_get_next((uint8*) &eh_cur_ext_name) == TRUE)
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

//��ȡ��ǰĿ¼��
//strdir: ���Ŀ¼���Ƶ�ָ��
//name_len: Ҫ��ȡ�ĳ����ַ���������������Ǻͽ�������
//location: ��ǰ·��
bool fsel_get_dir_name(uint8 *strdir, uint8 name_len, file_location_t *location)
{
    bool ret_len = 0;

    if (strdir == NULL)
    {
        return FALSE;
    }
    //��ȡ��ǰ�ļ���·����Ϣ
    if (location->dir_layer == 0)
    {
        libc_memcpy(strdir, root_str, sizeof(root_str));
        return TRUE;
    }
    //save current path
    vfs_file_dir_offset(eh_vfs_mount, &pdir_layer_total, &pfile_offset_total, 0);

    //set to cur dir entry
    if (vfs_cd(eh_vfs_mount, CD_BACK, NULL) == TRUE)
    {
        ret_len = (bool)vfs_get_name(eh_vfs_mount, strdir, name_len);
    }

    //back up path
    vfs_file_dir_offset(eh_vfs_mount, &pdir_layer_total, &pfile_offset_total, 1);

    return ret_len;

}
