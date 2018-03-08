//����Ŀ¼�б���ļ�����
//ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�(��ΪĿ¼�б��������ڴ���)
#include "eh_fsel_entry.h"
#include "dir_list.h"

//ͳ�Ƶ�ǰĿ¼���ļ�������
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
uint16 fsel_total_file_curdir(void)
{
    uint16 total = 0;

    list_dir_info_t* next_entry_ptr;

    next_entry_ptr = (plist_info + 1);

    if (eh_dir_no == plist_head->dir_total)
    {
        total = plist_head->file_total - (plist_info->begin_index - 1);
    }
    else
    {
        //��ȡ�б����ļ�����
        total = (next_entry_ptr->begin_index - plist_info->begin_index);
    }

    return total;
}

//ͳ�ƴ����ļ���������˳�����shuffle����
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
uint16 fsel_total_file_alldir(void)
{
    uint16 total = 0;
    //�����ļ�����
    total = plist_head->file_total;
    //shuffleģʽ��Ҫ��ʼ��
    if ((TRUE == eh_shuffle_sw) && (total > 0))
    {
        fsel_init_shuffle(total);
    }
    return total;
}

//��λ���һ��Ŀ¼
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
bool fsel_last_list_entry(void)
{
    uint16 i = 0;

    list_dir_info_t* cur_entry_info = plist_info;

    //��λ�����һ��Ŀ¼λ��
    eh_dir_no = plist_head->dir_total;

    //ָ�����һ���б���
    plist_info = (list_dir_info_t*) (plist_start_addr + sizeof(list_head_info_t));

    while (i < (eh_dir_no - 1))
    {
        plist_info++;
        i++;
    }

    //��ǰĿ¼�ļ�����
    eh_file_total = fsel_total_file_curdir();

    eh_file_no = 0;

    vfs_jump_to_direntry(eh_vfs_mount, 0, plist_info, cur_entry_info);

    return TRUE;
}



//���������cur_num���ļ����ڽ�Ŀ�����ǵڼ���
//
//ע�⣺��Ϊ�����õ���0x2e000��ʼ��0x2000�ռ�(�˿ռ䱻����������)��
//���ԣ�ֻ���ڽ�������������ʱ�򣬲��ܵ��á�
//(�����и��ʱ����Ϊ��STOP�ˣ����ԣ��Ϳ��Ե���)
//
uint16 fsel_check_index_num(uint16 cur_num)
{
    uint16 list_file_total, i;
    list_head_info_t* head_ptr = (list_head_info_t*) PLIST_INFO_ADDR;
    uint16* index_ptr = (uint16*) PLIST_INDEX_ADDR;

    fsel_sortlsit_vram_read(0);
    list_file_total = head_ptr->file_total;

    for (i = 1; i <= list_file_total; i++)
    {
        if (i > SUPPORT_MUSIC_FILE)
        {
            return cur_num;
        }

        if (((*index_ptr) == cur_num))
        {
            break;
        }
        index_ptr++;
    }
    return cur_num;
}

//�����ļ����ŵ�˳��
//type: ����˳��(����˳��, ��Ŀ��)
//cur_num: �������
//ע�⣺
//  ��Ϊ������fsel_check_index_num��ԭ��
//  ���ԣ�������ִ��ʱ��ռ��0x2e000��ʼ��0x2000�ռ�
bool fsel_set_seltype(fsel_type_e type, uint16 cur_num)
{
    fsel_error = FSEL_NO_ERR;

    //��������������,��Ĭ�Ͻ�����ͨĿ¼ģʽ
    if (type >= FSEL_TYPE_MAX)
    {
        type = FSEL_TYPE_COMMONDIR;
    }
    if (type != eh_fsel_type)
    {
        eh_fsel_type = type;
    }

    //����Ϊ��Ŀ��ģʽ
    if (eh_fsel_type == FSEL_TYPE_LISTDIR)
    {
        eh_disk_file_no = cur_num;

        eh_file_no_all = fsel_check_index_num(cur_num);

        //if it has alredy update to file list
        /*if(cur_num!= eh_file_no_all)
        {
            //have already set
            //no need to check again
            eh_check_list_update = TRUE;
        }*/
        //��ͷָ��
        plist_head = (list_head_info_t *) plist_start_addr;
        //�б���ָ��
        plist_info = (list_dir_info_t *) (plist_start_addr + sizeof(list_head_info_t));

        eh_dir_no = 1;
        //no scan correctly
        if (plist_info->begin_index == 0)
        {
            //����û���ļ�(��û���ļ�Ҳ���ᵽ������)
            //������
            eh_fsel_type = FSEL_TYPE_COMMONDIR;
            eh_file_no_all = cur_num;

            return FALSE;
        }

        //����totalֵ(֮ǰ�Ŀ���Ϊ��һ��Ŀ¼���ļ�����)
        eh_file_total_all = plist_head->file_total;
        //check file index valid
        if (eh_file_no_all > eh_file_total_all)
        {
            if (plist_head->dir_total == 1)
            {
                eh_file_no_all = eh_file_total_all;
            }
            else
            {
                eh_file_no_all = 0;
                //Ĭ�Ͻ����һ��Ŀ¼��
                vfs_jump_to_direntry(eh_vfs_mount, 1, plist_info, plist_info);
            }
            eh_disk_file_no = eh_file_no_all;

            return TRUE;
        }

        if (cur_num != plist_info->begin_index)
        {
            //ƥ�䵱ǰ���ĸ��ļ���
            while (1)
            {
                if ((cur_num >= plist_info->begin_index) && (cur_num < (plist_info + 1)->begin_index))
                {
                    break;
                }
                else if (eh_dir_no == plist_head->dir_total)
                {
                    //���һ��
                    break;
                }
                else
                {
                    eh_dir_no++;
                    plist_info++;
                }
            }
        }
    }

    return TRUE;
}


//��λ����һ��Ŀ¼�ĵ�һ���ļ�
//strdir������ļ�����ָ��
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
bool fsel_list_next_dir(uint8 *strdir)
{
    bool jump_val;

    list_dir_info_t* cur_dirinfo = plist_info;

    if (plist_head->dir_total == 1)
    {
        //gap_set = (eh_disk_file_no - plist_info->begin_index);
        jump_val = TRUE; //do nothing
    }
    //�������һ��Ŀ¼
    else if (eh_dir_no == plist_head->dir_total)
    {
        //�л��ص���һ��Ŀ¼
        eh_dir_no = 1;
        //ָ��ָ���һ���б���
        plist_info = (list_dir_info_t*) (plist_start_addr + sizeof(list_head_info_t));
        jump_val = vfs_jump_to_direntry(eh_vfs_mount, PREV_FLAG, plist_info, cur_dirinfo);
    }
    else
    {
        plist_info++;

        eh_dir_no++;
        //�л�����һ��Ŀ¼
        jump_val = vfs_jump_to_direntry(eh_vfs_mount, 1, plist_info, cur_dirinfo);
    }
    //��ǰĿ¼�ĵ�һ���ļ�
    if (jump_val == TRUE)
    {
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, 0, plist_info, fsel_init_val.file_type_bit);
    }
    if (eh_cur_ext_name != 0)
    {
        if (strdir != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strdir = eh_cur_ext_name;
        }
        eh_file_no = 1;
        eh_file_total = fsel_total_file_curdir();
        eh_disk_file_no = (eh_file_no + plist_info->begin_index - 1);
        eh_file_no_all = fsel_check_index_num(eh_disk_file_no);
    }
    else
    {
        jump_val = FALSE;
    }

    return jump_val;
}

//��λ����һ��Ŀ¼����һ���ļ�
//strdir������ļ�����ָ��
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
bool fsel_list_prev_dir(uint8 *strdir)
{
    bool jump_retval;

    list_dir_info_t* cur_dir_info = plist_info;

    //�����һ��Ŀ¼
    if (eh_dir_no == 1)
    {
        jump_retval = fsel_last_list_entry();
    }
    else
    {
        //�л�����һ��Ŀ¼
        eh_dir_no--;
        plist_info--;
        jump_retval = vfs_jump_to_direntry(eh_vfs_mount, PREV_FLAG | 0x01, plist_info, cur_dir_info);
    }
    //��ǰĿ¼�ĵ�һ���ļ���׺��
    if (jump_retval == TRUE)
    {
        //�����Ŀ¼,���Ҹ�Ŀ¼���һ���ļ�
        eh_cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount, PREV_FLAG, plist_info, fsel_init_val.file_type_bit);
    }
    if (eh_cur_ext_name != 0)
    {
        if (strdir != NULL)
        {
            vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);
            *(uint32*) strdir = eh_cur_ext_name;
        }
        eh_file_total = fsel_total_file_curdir();
        eh_file_no = eh_file_total;

        eh_disk_file_no = (eh_file_no + plist_info->begin_index - 1);

        eh_file_no_all = fsel_check_index_num(eh_disk_file_no);
    }
    else
    {
        jump_retval = FALSE;
    }
    return jump_retval;
}

