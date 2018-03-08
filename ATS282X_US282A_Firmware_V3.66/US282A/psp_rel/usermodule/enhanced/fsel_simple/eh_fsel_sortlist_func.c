#include "eh_fsel_entry.h"
#include "dir_list.h"

//���ڵ�ǰ�ļ��ţ�������
static bool _down(uint16 num)
{
    list_dir_info_t* p_entry = plist_info;
    //��ת�ĸ���
    uint16 gap_num;

    while (1)
    {
        if (eh_dir_no == plist_head->dir_total)
        {
            //���һ��
            break;
        }
        if ((num >= plist_info->begin_index) && (num < (plist_info + 1)->begin_index))
        {
            break;
        }

        eh_dir_no++;
        plist_info++;
    }

    if (plist_info != p_entry)
    {
        //�ڱ��Ŀ¼��
        //��λ��Ŀ¼(�����)
        if(vfs_jump_to_direntry(eh_vfs_mount, 0, plist_info, p_entry) == FALSE)
        {
            return FALSE;
        }
        //��λ���ļ�
        gap_num = num - (plist_info->begin_index - 1);
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, gap_num, plist_info,
            fsel_init_val.file_type_bit);
        eh_file_no = gap_num;
        eh_file_total = fsel_total_file_curdir();
        return TRUE;
    }

    //��ͬһ��Ŀ¼��
    gap_num = num - eh_disk_file_no;
    eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount,
        gap_num, plist_info, fsel_init_val.file_type_bit);
    eh_file_no = num - (plist_info->begin_index - 1);
    return TRUE;
}

//С�ڵ�ǰ�ļ��ţ���ǰ����
static bool _up(uint16 num)
{
    list_dir_info_t* p_entry = plist_info;
    //��ת�ĸ���
    uint16 gap_num;

    //ȷ��Ŀ¼
    while (1)
    {
        if ((eh_dir_no == plist_head->dir_total) && (num >= plist_info->begin_index))
        {
            //���һ��
            break;
        }
        else if ((num >= plist_info->begin_index) && (num < (plist_info + 1)->begin_index))
        {
            break;
        }
        else if (eh_dir_no == 1)
        {
            //��һ��
            break;
        }
        else
        {
            ;//QAC
        }

        eh_dir_no--;
        plist_info--;
    }
    if (plist_info != p_entry)
    {
        //�ڱ��Ŀ¼��
        //��λ��Ŀ¼(��ǰ��)
        if(vfs_jump_to_direntry(eh_vfs_mount, PREV_FLAG, plist_info, p_entry) == FALSE)
        {
            return FALSE;
        }

        //��λ���ļ�
        gap_num = num - (plist_info->begin_index - 1);
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, gap_num, plist_info,
            fsel_init_val.file_type_bit);
        eh_file_no = gap_num;
        eh_file_total = fsel_total_file_curdir();

        return TRUE;
    }
    //��ͬһ��Ŀ¼��
    gap_num = eh_disk_file_no - num;
    eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, PREV_FLAG | gap_num, plist_info,
        fsel_init_val.file_type_bit);
    eh_file_no -= gap_num;
    return TRUE;
}

//���ݴ�����Ŷ�λ�ļ�
//strfile ��׺��
//num �������
//show_num ���ļ��Ľ�Ŀ���
bool fsel_list_get_byno_all(uint8 *strfile, uint16 num, uint16 show_num)
{
    bool ret_vals = TRUE;

    //�����δ��ȡ�����ļ���,��ɨ�����
    if (eh_file_total_all == 0)
    {
        eh_file_total_all = fsel_total_file_alldir();
    }

    //��Ч�����ֱ�ӷ���ʧ��
    if ((num > eh_file_total_all) || (num == 0))
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (eh_disk_file_no < num)
    {
        ret_vals = _down(num);
    }
    else if (eh_disk_file_no > num)
    {
        ret_vals = _up(num);
    }
    else
    {
        /*�͵�ǰ���ļ������*/
        eh_file_no = (uint16)(eh_disk_file_no - plist_info->begin_index + 1);

        if (eh_disk_file_no == 1)
        {
            /*��һ���ļ�*/
            //��λ���ļ�
            eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, 0, plist_info, fsel_init_val.file_type_bit);
            eh_file_no = 1;
        }
    }
    //��ȷ(need modify)
    if (eh_cur_ext_name == 0)
    {
        ret_vals = FALSE;
    }
    if ((ret_vals == TRUE) && (eh_cur_ext_name != 0))
    {
        eh_file_no_all = show_num;
        //update real num for next show
        eh_disk_file_no = num;

        if (strfile != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strfile = eh_cur_ext_name;
        }
    }
    return ret_vals;
}



//����ѭ��ģʽ����Ŀ����ȡ��һ���ļ�
//strfile ��׺��
bool fsel_sortlist_get_next(uint8 *strfile)
{
    //�����е�λ��
    uint16 real_num, temp_num, dir_num;
    uint16 loopi = 0;
    bool ret_val = FALSE;

    switch (fsel_init_val.fsel_mode)
    {
        //�ظ���ǰһ���ļ�
        case FSEL_MODE_LOOPONE:
        if (eh_file_no_all == 0)
        {
            //Ĭ��ȡ��һ��
            //eh_file_no_all = 1;
            real_num = fsel_sortlsit_vram_read(eh_file_no_all);
            ret_val = fsel_list_get_byno_all(strfile, real_num, 1);
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
        //��ǰĿ¼�ظ�����(sort û��Ŀ¼����,��ѭ����ʽ��Ч)
        case FSEL_MODE_LOOPONEDIR:
        //Ŀ¼���
        dir_num = eh_file_no;
        //�����л�
        dir_num++;
        //check
        temp_num = eh_file_no_all;

        temp_num++;

        if (dir_num <= eh_file_total)
        {
            real_num = fsel_sortlsit_vram_read(temp_num);
        }
        else
        {
            //��Ŀ¼��һ��
            temp_num -= eh_file_total;
            //eh_file_no = 1;
            //��ȡ�ļ�
            real_num = fsel_sortlsit_vram_read(temp_num);
        }

        ret_val = fsel_list_get_byno_all(strfile, real_num, temp_num);
        break;

        //shuffle��
        case FSEL_MODE_RANOM:
        //�����ļ�˳���ȡ���ظ�
        case FSEL_MODE_LOOPALL:
        temp_num = eh_file_no_all;
        while ((ret_val == FALSE) && (loopi < eh_file_total_all))
        {
            if (TRUE == eh_shuffle_sw)
            {
                temp_num = fsel_shuffle_next_file();
            }
            else
            {
                temp_num++;
            }

            if ((temp_num > eh_file_total_all) || (temp_num == 0))
            {
                temp_num = 1;
                //real_num = fsel_sortlsit_vram_read(temp_num);
                if (TRUE == eh_shuffle_sw)
                {
                    //��֤shuffle�ķ�����ȷ
                    fsel_init_shuffle(eh_file_total_all);
                    //shuffle first one
                    temp_num = fsel_shuffle_next_file();
                }
            }

            real_num = fsel_sortlsit_vram_read(temp_num);

            ret_val = fsel_list_get_byno_all(strfile, real_num, temp_num);

            loopi++;
            if (ret_val == FALSE)
            {
                //set to first entry
                fsel_first_list_entry();

                eh_file_no_all = 0;

                eh_disk_file_no = eh_file_no_all;

                temp_num = eh_file_no_all;
            }
        }

        break;

        default:
        ret_val = FALSE;
        break;
    }

    return ret_val;
}

//��ȡ��һ���ļ�
//strfile ��׺��
bool fsel_sortlist_get_prev(uint8 *strfile)
{
    bool ret_val = FALSE;

    uint16 loopi = 0;

    //�����е�λ��
    uint16 real_num, temp_num, dir_num;

    switch (fsel_init_val.fsel_mode)
    {
        case FSEL_MODE_LOOPONE:
        if (eh_file_no_all == 0)
        {
            //ȡ��һ���ļ�
            //eh_file_no_all = 1;
            real_num = fsel_sortlsit_vram_read(0);
            ret_val = fsel_list_get_byno_all(strfile, real_num, 1);
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
        //��ǰĿ¼�ظ�����(sort û��Ŀ¼����,��ѭ����ʽ��Ч)
        case FSEL_MODE_LOOPONEDIR:
        //read vram
        dir_num = eh_file_no;

        temp_num = eh_file_no_all;

        if (dir_num > 0)
        {
            dir_num--;
            //�����л�
            temp_num--;
        }

        if (dir_num > 0)
        {
            real_num = fsel_sortlsit_vram_read(temp_num);
        }
        else
        {
            //eh_file_no = eh_file_total;

            temp_num += eh_file_total;

            real_num = fsel_sortlsit_vram_read(temp_num);
        }
        ret_val = fsel_list_get_byno_all(strfile, real_num, temp_num);
        break;

        //shuffle��
        case FSEL_MODE_RANOM:
        //ȫ���ظ�
        case FSEL_MODE_LOOPALL:
        temp_num = eh_file_no_all;

        while ((ret_val == FALSE) && (loopi < eh_file_total_all))
        {
            if (TRUE == eh_shuffle_sw)
            {
                temp_num = fsel_shuffle_next_file();
            }
            else
            {
                //���ǵ�һ�׸�,������ǰ��
                if (temp_num > 0)
                {
                    temp_num--;
                }
            }

            if ((temp_num > eh_file_total_all) || (temp_num == 0))
            {
                //��֤shuffle�ķ�����ȷ
                temp_num = eh_file_total_all;

                if (TRUE == eh_shuffle_sw)
                {
                    fsel_init_shuffle(eh_file_total_all);
                    //shuffle first one
                    temp_num = fsel_shuffle_prev_file();
                }
            }

            real_num = fsel_sortlsit_vram_read(temp_num);

            ret_val = fsel_list_get_byno_all(strfile, real_num, temp_num);

            loopi++;

            if (ret_val == FALSE)
            {
                //��λ�����һ���ļ���Ŀ¼��λ��
                fsel_last_list_entry();

                eh_file_no_all = 1;

                eh_disk_file_no = 1;

                temp_num = eh_file_no_all;
            }
        }
        break;

        default:

        ret_val = FALSE;
        break;

    }
    //�ļ����
    return ret_val;
}

//������һ��Ŀ¼
bool __section__(".text.bank2") fsel_sortlist_next_dir(uint8 *strdir)
{
    //list have no dir concept so we switch to next file
    fsel_sortlsit_vram_read(eh_file_no_all);
    return fsel_list_next_dir(strdir);
}

//������һ��Ŀ¼
//strfile ��׺��
bool __section__(".text.bank2") fsel_sortlist_prev_dir(uint8 *strdir)
{
    fsel_sortlsit_vram_read(eh_file_no_all);
    return fsel_list_prev_dir(strdir);
}

//ͳ�ƴ����ļ�������
uint16 __section__(".text.bank2") fsel_total_file_sortlist(void)
{
    fsel_sortlsit_vram_read(0);
    return fsel_total_file_alldir();
}

