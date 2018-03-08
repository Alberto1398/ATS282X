#include "eh_fsel_entry.h"
#include "dir_list.h"

static const fsel_get_next_p fsel_get_next_array[2] =
{
    fsel_dir_get_next, //������˳��ģʽ��ѡ����һ���ļ�
    fsel_sortlist_get_next //����Ŀ����ģʽ��ѡ����һ���ļ�
};

static const fsel_get_prev_p fsel_get_prev_array[2] =
{
    fsel_dir_get_prev, //������˳��ģʽ��ѡ����һ���ļ�
    fsel_sortlist_get_prev //����Ŀ����ģʽ��ѡ����һ���ļ�
};

static const fsel_get_byno_p fsel_get_byno_array[2] =
{
    fsel_dir_get_byno_all, //������˳��ģʽ�¸������ѡ���ļ�
    fsel_sortlist_get_byno_all //����Ŀ����ģʽ�¸������ѡ���ļ�
};

static const fsel_set_location_p fsel_set_location_array[2] =
{
    (void *) fsel_dir_set_location, //������˳��ģʽ������·����Ϣ
    (void *) fsel_sortlist_set_location //����Ŀ����ģʽ������·����Ϣ
};

static const fsel_get_location_p fsel_get_location_array[2] =
{
    (void *) fsel_dir_get_location, //������˳��ģʽ�»�ȡ·����Ϣ
    (void *) fsel_sortlist_get_location //����Ŀ����ģʽ�»�ȡ·����Ϣ
};

static const fsel_get_nextdir_p fsel_get_nextdir_array[2] =
{
    fsel_dir_next_dir, //������˳��ģʽ���л���һ��Ŀ¼
    fsel_sortlist_next_dir //����Ŀ����ģʽ���л���һ��Ŀ¼
};
static const fsel_get_prevdir_p fsel_get_prevdir_array[2] =
{
    fsel_dir_prev_dir, //������˳��ģʽ���л���һ��Ŀ¼
    fsel_sortlist_prev_dir //����Ŀ����ģʽ���л���һ��Ŀ¼
};

static const fsel_get_total_p fsel_get_total_array[2] =
{
    fsel_get_all_dirs_total_files, //������˳��ģʽ�»�ȡ�ļ�����
    fsel_total_file_sortlist //����Ŀ����ģʽ�»�ȡ�ļ�����
};

//��ȡ��һ���ļ�
bool fsel_get_nextfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (eh_vfs_mount == -1)
    {
        return 0;
    }
    return fsel_get_next_array[eh_fsel_type](strfile);
}

//��ȡ��һ���ļ�
bool fsel_get_prevfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (eh_vfs_mount == -1)
    {
        return 0;
    }
    return fsel_get_prev_array[eh_fsel_type](strfile);
}

//��ָ����Ż�ȡ�ļ�
//num �ڴ���˳��ģʽ�£�Ϊ������ţ��ڽ�Ŀ��˳����Ϊ��Ŀ���
bool fsel_get_byno(char *strfile, uint16 num)
{
    fsel_error = FSEL_NO_ERR;

    if (num == 0)
    {
        return FALSE;
    }
    return fsel_get_byno_array[eh_fsel_type](strfile, num);
}

//�ļ�����
uint16 fsel_get_total(void)
{
    fsel_error = FSEL_NO_ERR;

    if (eh_file_total_all != 0)
    {
        return eh_file_total_all;
    }
    else
    {
        return fsel_get_total_array[eh_fsel_type]();
    }
}

//��ǰ�ļ��Ĵ������
uint16 fsel_get_fileno(void)
{
    uint16 file_no = eh_file_no_all;
    if (eh_fsel_type == FSEL_TYPE_LISTDIR)
    {
        file_no = eh_disk_file_no;
    }
    return file_no;
}

//���貥��ģʽ
bool fsel_set_mode(uint8 mode)
{
    fsel_error = FSEL_NO_ERR;
    //check shuffle
    if (((uint8) mode & 0xf0) > 0)
    {
        if (eh_shuffle_sw == FALSE)
        {
            //֮ǰ����shuffle,Ҫ��ʼ��
            eh_shuffle_sw = TRUE;
            //if(eh_file_total_all == 0)
            //{
            eh_file_total_all = fsel_get_total();
            //}
            fsel_init_shuffle(eh_file_total_all);
            filtrate_shuffle_no = eh_file_no_all;
        }
    }
    else
    {
        eh_shuffle_sw = FALSE;
    }

    fsel_init_val.fsel_mode = (uint8) (mode & 0x0f);

    return TRUE;
}

//��λ�ļ�
bool fsel_set_location(void *location)
{
    fsel_error = FSEL_NO_ERR;

    if (location == NULL)
    {
        return FALSE;
    }

    return fsel_set_location_array[eh_fsel_type](location);
}

//��ȡ��ǰѡ���ļ���location
bool fsel_get_location(void *location)
{
    fsel_error = FSEL_NO_ERR;

    if ((eh_file_no == 0) || (location == NULL))
    {
        return FALSE;
    }

    return fsel_get_location_array[eh_fsel_type](location);
}

//��ȡ��һ��Ŀ¼
bool fsel_get_nextdir(uint8 *strdir)
{
    fsel_error = FSEL_NO_ERR;

    return fsel_get_nextdir_array[eh_fsel_type](strdir);
}

//��ȡ��һ��Ŀ¼
bool fsel_get_prevdir(uint8 *strdir)
{
    fsel_error = FSEL_NO_ERR;

    return fsel_get_prevdir_array[eh_fsel_type](strdir);
}

//��ȡ��ǰ�ļ�·����Ϣ
//location ���·����Ϣָ��
bool fsel_dir_get_location(file_location_t *location)
{
    vfs_get_name(eh_vfs_mount, &eh_cur_ext_name, 0);

    *(uint32*) &location->file_info.file_extend_info.file_ext = eh_cur_ext_name;
    //��ȡ��ǰ�ļ���·����Ϣ
    if (FALSE == vfs_file_dir_offset(eh_vfs_mount, &location->dir_layer_info, \
        &location->file_info.file_extend_info.cluster_no, 0))
    {
        return FALSE;
    }

    location->disk = fsel_init_val.disk;
    location->dir_layer = eh_dir_layer;

    if (eh_file_total_all == 0)
    {
        //��ǰĿ¼�ļ�����
        location->file_total = eh_file_total;
    }
    else
    {
        location->file_total = eh_file_total_all;
    }
    //�ļ����
    location->file_num = eh_file_no_all;

    return TRUE;
}

