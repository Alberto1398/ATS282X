//����Ŀ¼�б���Ŀ�����ļ�����
//ִ��ʱ��ռ��PLIST_INFO_ADDR��ʼ��0x1000�ռ�(��ΪĿ¼�б��������ڴ���)
//ִ��ʱ��ռ��PLIST_INDEX_ADDR��ʼ��0x1000�ռ�(��Ϊ��Ŀ����������ڴ���)
#include "eh_fsel_entry.h"

static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

//��ʼ����������
void fsel_init_sysdata(void)
{
    //��ǰĿ¼�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
    eh_dir_layer = 0;
    //��Ŀ¼����Ŀ¼ָ�룬1,2��������
    //eh_root_dir = 0;
    //��ǰ�ļ���Ŀ¼���б��е����
    eh_file_no = 0;
    //��ǰĿ¼���б�����ļ�����
    eh_file_total = 0;

    //��ǰ�����ļ����
    eh_file_no_all = 0;
    //��ǰ�����ļ�����
    eh_file_total_all = 0;

    //��ͷָ��
    plist_head = (list_head_info_t *) plist_start_addr;
    //�б���ָ��
    plist_info = (list_dir_info_t *) (plist_start_addr + sizeof(list_head_info_t));

    eh_dir_no = 1;

    //Ĭ�Ͻ����Ŀ¼
    vfs_cd(eh_vfs_mount, CD_ROOT, 0);
}

//��ȡ������Ϣ
fsel_error_e fsel_get_error(void)
{
    return vfs_get_err_info(eh_vfs_mount);
}



void fsel_set_fileno(uint16 set_num)
{
    eh_file_no_all = set_num;
}

//��ʼ��
int32 fsel_init(fsel_param_t *init_param, uint8 drv_mode)
{
    //������������
    uint8 *drv_name;
    //��������
    uint8 drv_type;
    //������Ϣ
    fsel_error = FSEL_NO_ERR;
    //����Ϊ����ֱ�ӷ���
    if (init_param == NULL)
    {
        return -1;
    }
    //�����ʼ������,�ļ�ѡ�����˳�ʱʹ��
    libc_memcpy(&fsel_init_val, init_param, sizeof(fsel_param_t));

    //ѭ��ģʽ
    fsel_init_val.fsel_mode &= 0x0f;
    //shuffle�Ƿ��
    if ((init_param->fsel_mode & 0xf0) != 0)
    {
        eh_shuffle_sw = TRUE;
    }
    else
    {
        eh_shuffle_sw = FALSE;
    }

    if (init_param->disk == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8 *) &card_drv_name;
    }
    else if (init_param->disk == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return -1;
    }

    //��װ�洢����
    if (sys_drv_install(drv_type, (uint32)drv_mode, drv_name) != 0)
    {
        fsel_error = FSEL_ERR_STG;
        return -1;
    }

    //����FS
    eh_vfs_mount = sys_mount_fs(drv_type, fsel_init_val.disk, 0);

    if (eh_vfs_mount == -1)
    {
        sys_drv_uninstall(drv_type);
        fsel_error = FSEL_ERR_FS;
        return -1;
    }

    /*if (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL) == 0)
    {
        eh_vfs_mount = -1;
    }
    else*/
    //{
    //��ʼ������
    fsel_init_sysdata();
    //}

    //�ļ�ѡ��ģʽΪĿ¼ģʽ
    eh_fsel_type = 0;

    return eh_vfs_mount;
}

//�˳�
bool fsel_exit(void)
{
    int32 ret_val;
    //��������
    uint8 drv_type;

    fsel_error = FSEL_NO_ERR;
    //�ļ�ϵͳ���سɹ�
    if (eh_vfs_mount != -1)
    {
        ret_val = sys_unmount_fs(eh_vfs_mount);
        eh_vfs_mount = -1;
    }
    else
    {
        return TRUE;
    }
    //�ļ�ϵͳж�سɹ�
    if (ret_val < 0)
    {
        fsel_error = FSEL_ERR_FS;
        return FALSE;
    }

    if (fsel_init_val.disk == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
    }
    else if (fsel_init_val.disk == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return -1;
    }
    //ж�ؽ�������
    ret_val = sys_drv_uninstall(drv_type);

    if (ret_val < 0)
    {
        fsel_error = FSEL_ERR_STG;
        return FALSE;
    }

    return TRUE;
}

