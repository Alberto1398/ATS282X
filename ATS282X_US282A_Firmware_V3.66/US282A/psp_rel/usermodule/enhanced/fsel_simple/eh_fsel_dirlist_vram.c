//����Ŀ¼�б���Ŀ�����ļ�����
//ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�(��ΪĿ¼�б��������ڴ���)
//ִ��ʱ��ռ��PLIST_INDEX_ADDR��ʼ��0x1000�ռ�(��Ϊ��Ŀ����������ڴ���)
#include "eh_fsel_entry.h"
#include "dir_list.h"
#include "vm_fwsp_def.h"

//���ݽ�Ŀ���(�ڼ�������)�õ�������š�
//��src_num�����ŵ��ļ��Ĵ����е����(����FAT�����������)
//˳�㣺��Ŀ¼����ϢҲ�������ڴ�.
//ע�⣺��Ϊʹ�õ���0x2e000��ʼ��8k�ڴ棬
//  ���ԣ�������ִ��ʱ������ڴ�һ�����ܱ���������ʹ��!!!!
uint16 fsel_sortlsit_vram_read(uint16 src_num)
{
    uint8* read_ptr = (uint8 *) PLIST_INFO_ADDR;
    uint16* index_ptr = (uint16 *) PLIST_INDEX_ADDR;
    uint16 dst_num;
    //�ļ�ƫ��

    //����Ŀ¼�б�
    sys_vm_read(read_ptr, VM_PLIST_INFO_START0, 0x1000);
    //����Ŀ��
//    sys_vm_read(index_ptr, VM_PLIST_INDEX_START0, 0x1000);

//    if (src_num > 0)
//    {
//        src_num--;
//    }
//    if (src_num < SUPPORT_MUSIC_FILE)
//    {
//        //�ļ����
//        dst_num = index_ptr[src_num];
//    }
//    else
//    {
//        src_num++;
//        dst_num = src_num;
//    }  
    libc_print("######num change",0,0);

    return src_num;
}

//���ݽ�Ŀ��Ŷ�λ���ļ�
bool fsel_sortlist_get_byno_all(uint8 *strfile, uint16 num)
{
    //�����е�λ��
    uint16 real_num;
    if (num > eh_file_total_all)
    {
        return FALSE;
    }

    //�õ��������
    real_num = fsel_sortlsit_vram_read(num);

    return fsel_list_get_byno_all(strfile, real_num, num);
}

//��ȡ��ǰ�ļ�·����Ϣ
bool fsel_sortlist_get_location(file_location_t *location)
{
    //��vram
    fsel_sortlsit_vram_read(location->file_num);

    return fsel_list_get_location(location);
}

//���õ�ǰ·����Ϣ
bool fsel_sortlist_set_location(file_location_t *location)
{
    //��vram
    fsel_sortlsit_vram_read(location->file_num);

    return fsel_list_set_location(location);
}

//��ȡ��ǰ�ļ�·����Ϣ
//location: ���Ŀ¼·����Ϣָ��
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
bool fsel_list_get_location(file_location_t *location)
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
    //Ŀ¼�㼶
    location->dir_layer = plist_info->cur_layer;
    //�������ļ�����
    if (eh_file_total_all == 0)
    {
        eh_file_total_all = fsel_total_file_alldir();
    }
    location->file_total = eh_file_total_all;
    //��ǰ�ļ����
    location->file_num = eh_file_no_all;

    return TRUE;
}

//���õ�ǰ·����Ϣ
//location ���·����Ϣָ��
//ǰ�᣺Ŀ¼�б��Ѿ�����
//������ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�
bool fsel_list_set_location(file_location_t *location)
{
    bool ret_val;

    //��������locationΪ��Чֵ
    if ((location == NULL) || (*(uint32*) &location->file_info.file_extend_info.file_ext == 0))
    {
        //��ǰĿ¼�µ��ļ�����
        eh_file_total = fsel_total_file_curdir();
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
            eh_file_total = fsel_total_file_curdir();
            //eh_file_no ��ֵ
            eh_file_no = 0;
        }
    }

    if ((ret_val == FALSE) || (eh_file_no == 0))
    {
        //Ŀ¼�ջ�Ŀ¼��Ϣ���󣬻ص���Ŀ¼
        fsel_init_sysdata();

        eh_file_total = fsel_total_file_curdir();

        return FALSE;
    }

    //��ֵ����eh_cur_ext_name
    libc_memcpy(&eh_cur_ext_name, location->file_info.file_extend_info.file_ext, 4);

    return TRUE;
}


