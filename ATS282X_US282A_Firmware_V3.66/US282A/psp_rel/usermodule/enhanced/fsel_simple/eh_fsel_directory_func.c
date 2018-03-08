//������Ŀ¼�б���ļ��Ĳ���
#include "eh_fsel_entry.h"
#include "dir_list.h"

//ͳ�Ƶ�ǰĿ¼���ļ�������
//dir_info���ļ�λ����Ϣ(8�ֽ�)
//check_flag: ���Ϊ1����ʾ��˳�����eh_file_no
uint16 fsel_get_cur_dir_total_files(uint8* dir_info, uint8 check_flag)
{
    uint8 temp_offset_buf[8];
    uint8 dir_type = DIR_HEAD;
    uint16 total = 0;

    //���浱ǰ·��
    vfs_file_dir_offset(eh_vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer, 0);

    do
    {
        if (0 != vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit))
        {
            total++;
            if(total > SUPPORT_MUSIC_FILE)//�����ļ�֧����
            {
                break;
            }
            if (check_flag == 1)
            {
                //�Ƚ�8�ֽ�offset
                vfs_file_dir_offset(eh_vfs_mount, NULL, &temp_offset_buf, 0);
                if (libc_memcmp(dir_info, &temp_offset_buf, 8) == 0)
                {
                    eh_file_no = total;
                    check_flag = 0;
                }
            }
        }
        else
        {
            break;
        }
        dir_type = DIR_NEXT;
    } while (1);

    //�ָ���ǰ·��
    vfs_file_dir_offset(eh_vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer, 1);

    if (check_flag == 1)
    {
        eh_file_no = 0;
    }

    return total;
}

//��ȡ��ǰĿ¼�µ��¸��ļ�
bool fsel_get_cur_dir_next_file(void)
{
    uint8 dir_type;
    if (eh_file_no == 0)
    {
        //���Ϊ0���ͷ��ʼ����
        dir_type = DIR_HEAD;
    }
    else
    {
        //��Ų�Ϊ0��dir��һ��
        dir_type = DIR_NEXT;
    }
    //dir�ļ�
    if (vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit) != 0)
    {
        //�ҵ��ļ����������
        eh_file_no++;
        return TRUE;
    }
    return FALSE;
}

//�ڴ��̷�Χ������һ���ļ�
bool fsel_get_disk_dir_next_file(void)
{
    //�ڵ�ǰĿ¼�������ļ�
    if (FALSE == fsel_get_cur_dir_next_file())
    {
        CurrentDiragain:
        if (eh_dir_layer >= (MAX_DIR_LAYER - 1)) //�����ǰ��8��Ŀ¼�Ͳ������ļ���
        {
            goto ParentDiragainNext;
        }
#ifdef FILTER_RECORD_DIR
        //Ѱ�Ҹ�Ŀ¼�ĵ�һ�����ļ���
        if ((vfs_dir(eh_vfs_mount, DIR_HEAD, NULL, EXT_NAME_ALL_DIR) != 0) && (FALSE == filtrate_dir(DIR_NEXT)))
#else
        if (vfs_dir(eh_vfs_mount, DIR_HEAD, NULL, EXT_NAME_ALL_DIR) != 0)
#endif
        {
            //�������ļ��У���ø����ļ��е��ļ�����
            if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                //Ŀ¼�㼶����
                eh_dir_layer++;
                //��ǰĿ¼�µ��ļ���Ź���
                eh_file_no = 0;
                // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
                if (FALSE != fsel_get_cur_dir_next_file())
                {
                    return TRUE;
                }
                else //�����Ŀ¼
                {
                    goto CurrentDiragain;
                }
            }
            else //������
            {
                return FALSE;
            }
        }
        else //���û�����ļ��У���������һ�㣬Ѱ���뵱ǰĿ¼ƽ������һ��Ŀ¼��ƽ��Ѱ�ң�
        {
            ParentDiragainNext:

            //�����ǰ�Ǹ�Ŀ¼�򷵻�
            if (eh_dir_layer == 0)
            {
                return FALSE;
            }
            //������Ŀ¼
            if (FALSE == vfs_cd(eh_vfs_mount, CD_BACK, NULL))
            {
                //�쳣����
                return FALSE;
            }
            //Ŀ¼�㼶��
            eh_dir_layer--;
#ifdef FILTER_RECORD_DIR
            if ((vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR) != 0) && (FALSE == filtrate_dir(DIR_NEXT)))
#else
            if (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR) != 0)
#endif 
            {
                //����Ŀ¼,���Ҹ�Ŀ¼�µ��ļ�
                if (FALSE != vfs_cd(eh_vfs_mount, CD_SUB, NULL))
                {
                    //Ŀ¼�㼶����
                    eh_dir_layer++;
                    //�ļ����Ϊ0
                    eh_file_no = 0;

                    if (FALSE != fsel_get_cur_dir_next_file())
                    {
                        return TRUE;
                    }
                    else
                    {
                        // �����Ŀ¼.
                        goto CurrentDiragain;
                    }
                }
                else //������
                {
                    return FALSE;
                }
            }
            else
            {
                goto ParentDiragainNext;
            }
        }
    }
    return TRUE;
}

//������һ���ж����ļ�
uint16 fsel_get_all_dirs_total_files(void)
{
    uint8 temp_dir_layer;
    //bool save_shuffle_flag;
    uint16 total;
    //uint32 ext_name;

    uint16 save_cur_no, save_cur_total;
    uint16 save_file_no_all;

    if (eh_vfs_mount == -1)
    {
        return 0;
    }
    //���浱ǰĿ¼���ļ���ź�����
    save_cur_no = eh_file_no;
    save_cur_total = eh_file_total;
    temp_dir_layer = eh_dir_layer;

    save_file_no_all = eh_file_no_all;

    //���浱ǰ·��
    vfs_file_dir_offset(eh_vfs_mount, &pdir_layer_total, &pfile_offset_total, 0);

    fsel_init_sysdata();

    total = 0;
    do
    {
        //�ڴ��̷�Χ������һ���ļ�
        if (fsel_get_disk_dir_next_file() != FALSE)
        {
            total++;
            if (total >= SUPPORT_MUSIC_FILE)
            {
                break;
            }
        }
        else
        {
            break;
        }
    } while (1);

    //�ָ���ǰ·��
    vfs_file_dir_offset(eh_vfs_mount, &pdir_layer_total, &pfile_offset_total, 1);

    //eh_shuffle_sw = save_shuffle_flag;

    if ((TRUE == eh_shuffle_sw) && (total > 0))
    {
        fsel_init_shuffle(total);
    }

    //�ָ���ǰĿ¼����ź�����
    eh_file_no = save_cur_no;
    eh_file_total = save_cur_total;
    eh_dir_layer = temp_dir_layer;

    eh_file_no_all = save_file_no_all;

    eh_file_total_all = total;

    return total;

}

//������Ż�ȡ�ļ�(�������ֵ��)
//strfile ��׺��
//num �����е����
bool fsel_dir_get_byno_all(uint8 *strfile, uint16 num)
{
    uint8 dir_type;

    //save shuffle flag;
    uint8 save_shuffle_flag;

    bool ret_val = TRUE;

    uint16 dir_num, i;

    //�����δ��ȡ�����ļ���,��ɨ�����
    if (eh_file_total_all == 0)
    {
        eh_file_total_all = fsel_get_all_dirs_total_files();
    }

    //��Ч�����ֱ�ӷ���ʧ��
    if ((num > eh_file_total_all) || (num == 0))
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }
    save_shuffle_flag = eh_shuffle_sw;
    eh_shuffle_sw = FALSE;
    if (eh_file_no_all < num)
    {
        //���ڵ�ǰ�ļ��ţ����Dir
        dir_num = num - eh_file_no_all;
        i = 0;
        while (i < dir_num)
        {
            //���dir�ļ�
            if (fsel_dir_next_layer((uint8 *) &eh_cur_ext_name) != FALSE)
            {
                i++;
            }
            else
            {
                ret_val = FALSE;
                break;
            }
        }
    }
    else if (eh_file_no_all > num)
    {
        //С�ڵ�ǰ�ļ��ţ���ǰDir
        dir_num = eh_file_no_all - num;

        i = 0;

        while (i < dir_num)
        {
            //��ǰdir�ļ�
            if (fsel_dir_prev_layer((uint8*) &eh_cur_ext_name) != FALSE)
            {
                i++;
            }
            else
            {
                ret_val = FALSE;
                break;
            }
        }
    }
    else
    {
        //�͵�ǰ���ļ������
        if (eh_file_no_all == 1)
        {
            //��һ���ļ�
            dir_type = DIR_HEAD;
            eh_file_no = 0;

            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 == eh_cur_ext_name)
            {
                ret_val = FALSE;
            }
        }
    }
    if (ret_val == TRUE)
    {
        eh_file_no_all = num;
        if (strfile != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strfile = eh_cur_ext_name;
        }
    }
    eh_shuffle_sw = save_shuffle_flag;
    return ret_val;
}

//��������ڵ�ǰĿ¼�»�ȡ�ļ�
//strfile ��׺��
//num �ļ��ڴ����е����
//ע�⣺��ı�eh_file_no��ֵ;
//          ȡ�ļ�ʱ����ǰ���ܸı�ǰһ�׵�eh_file_no��
//          ��Ϊ�ýӿ�Ҫ����eh_file_no�߲�ͬ������
bool fsel_dir_get_byno(uint8 *strfile, uint16 num)
{
    uint8 dir_type;

    uint16 dir_num;

    if ((num > eh_file_total) || (num == 0))
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
        //ȡ��һ��
        dir_type = DIR_NEXT;
    }
    if (eh_file_no < num)
    {
        //���ڵ�ǰ�ļ��ţ����Dir
        dir_num = num - eh_file_no;

        eh_file_no = 0;
        while (eh_file_no < dir_num)
        {
            //���dir�ļ�
            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            //�ҵ��ļ�
            if (0 != eh_cur_ext_name)
            {
                eh_file_no++;
            }
            else
            {
                return FALSE;
            }
            dir_type = DIR_NEXT;
        }
    }
    else if (eh_file_no > num)
    {
        //С�ڵ�ǰ�ļ��ţ���ǰDir
        dir_type = DIR_PREV;

        dir_num = eh_file_no - num;

        eh_file_no = 0;

        while (eh_file_no < dir_num)
        {
            //��ǰdir�ļ�
            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);

            if (0 != eh_cur_ext_name)
            {
                eh_file_no++;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        //�͵�ǰ���ļ������
        if (eh_file_no == 1)
        {
            //��һ���ļ�
            dir_type = DIR_HEAD;
            eh_file_no = 0;

            eh_cur_ext_name = vfs_dir(eh_vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 == eh_cur_ext_name)
            {
                return FALSE;
            }
        }
    }

    eh_file_no = num;
    eh_file_no_all = eh_file_no;
    if (strfile != NULL)
    {
        vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
        *(uint32*) strfile = eh_cur_ext_name;
    }
    return TRUE;
}



//��λ���ļ�
//location ���·����Ϣָ��
bool fsel_dir_set_location(file_location_t *location)
{
    bool ret_val;

    //��������locationΪ��Чֵ
    if ((location == NULL) || (*(uint32*) &location->file_info.file_extend_info.file_ext == 0))
    {
        //��ǰĿ¼�µ��ļ�����
        eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);
        //�ļ����
        eh_file_no = 0;
        return FALSE;
    }
    //��λ�ļ�
    ret_val = vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, \
    &location->file_info.file_extend_info.cluster_no, 1);

    if (ret_val != FALSE)
    {
        vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
        if (*(uint32*) &location->file_info.file_extend_info.file_ext != eh_cur_ext_name)
        {
            ret_val = FALSE;
        }
        else
        {
            //eh_file_no ��total_file_cur�︳ֵ
            eh_file_total = fsel_get_cur_dir_total_files((uint8*) &location->file_info.file_extend_info.cluster_no, 1);
            eh_file_no_all = location->file_num;
            //eh_file_total_all = location->file_total;

        }
    }

    if ((ret_val == FALSE) || (eh_file_no == 0))
    {
        //Ŀ¼�ջ�Ŀ¼��Ϣ���󣬻ص���Ŀ¼
        fsel_init_sysdata();

        eh_file_total = fsel_get_cur_dir_total_files(NULL, 0);

        return FALSE;
    }

    eh_dir_layer = location->dir_layer;

    //��ֵ����eh_cur_ext_name
    libc_memcpy(&eh_cur_ext_name, location->file_info.file_extend_info.file_ext, 4);

    return TRUE;
}

