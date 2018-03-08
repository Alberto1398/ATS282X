#include "music.h"

int32 eh_vfs_mount = -1;

uint16 g_vmfile_total;
uint16 g_vmdir_total;
//vram record disk
uint16 g_vmdisk_flag;
uint16 g_plist_file_count;
//��ǰĿ¼
list_dir_info_t g_cur_dir;
//��ǰĿ¼�����(1��ʼ,0��ʾg_cur_dir�����������Ч)
uint16 g_cur_dir_no;
//��vram���������������buffer
uint8 g_sector_buf[DIR_INFO_SECTOR_SIZE];
//�����˵ڼ�������(0��ʾû�ж���),�������buffer����װ���ǵڼ�������������
uint8 g_sector_no = 0;
//�ϴζ�λ�����ĸ�Ŀ¼(�����(��1��ʼ))
uint16 g_last_dir_no = 0;
//�ϴζ�λ���ļ������(1��ʼ)
uint16 g_last_get_index = 0;
//for id3 get(��Ϊ����bss��,����Ĭ��ֵ����0)
music_id3_info_t g_cur_id3_info;

rcp_file_status_t g_rcp_file_status;

MUS_STATIC const uint8 card_drv_name[] = "card.drv";
MUS_STATIC const uint8 uhost_drv_name[] = "uhost.drv";


//���˸�Ŀ¼�µ�RECORDĿ¼
MUS_STATIC bool _is_filter(void)
{
#ifdef FILTER_RECORD_DIR
    uint8 dir_name_buffer[32];
    uint16 name_len;
    uint8 add_len, i, j;
    uint8 *buf;
    uint8 rec_dir_name[8 + 1];

    libc_memset(rec_dir_name, 0, sizeof(rec_dir_name));
    com_load_share_data(COM_SHARE_DATA_REC_FOLDER, rec_dir_name, sizeof(rec_dir_name) - 1, 0);

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

    return TRUE;

#else
    return FALSE;
#endif
}

//��ȡ�ļ�������Ŀ¼����
//�����ļ�����
uint16 music_rcp_first_get(void)
{
    //��ȡ�ļ�������Ŀ¼����
    sys_vm_read(g_sector_buf,
            VM_PLIST_INFO_START0, //����vram_addr[0]
            sizeof(g_sector_buf));
    g_sector_no = 1;
    g_vmfile_total = ((list_head_info_t *) g_sector_buf)->file_total;
    libc_print("get file num",g_vmfile_total,2);
    g_vmdir_total = ((list_head_info_t *) g_sector_buf)->dir_total;
    
    g_vmdisk_flag = ((list_head_info_t *) g_sector_buf)->cur_disk;

    //��ʵ��music_rcp_dir_first�Ѿ�������һ��Ŀ¼�ˡ�
    //������Ȼ��ֵ0���������g_cur_dir��ֵ
    g_last_dir_no = 0;
    g_last_get_index = 0;
    g_sector_no = 0;
    return g_vmfile_total;
}

bool music_rcp_dir_next(void)
{
    //uint8 dir_type = DIR_HEAD;
    //uint16 temp_num;

    //�����ļ�ϵͳ�ӿ�
    if (0 != vfs_dir(eh_vfs_mount, DIR_HEAD, NULL, MUSIC_BITMAP))
    {
        //eh_file_no++;
        //ֻ��һ��Ŀ¼,���¼���ļ����ŵ�����
        return TRUE;
    }
    else
    {
        //fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }
    //return FALSE;
}

bool music_rcp_dir_first(void)
{
    uint8 cur_dir_layer;
    //��Ҫ���븸Ŀ¼
    bool back;

    //ǰ��Ĵ����Ѿ�ȷ������ǰ�ڸ�Ŀ¼�£����ԣ����︳ֵΪ0
    cur_dir_layer = 0;
    for (;;)
    {
        if (music_rcp_dir_next() == TRUE)
        {
            //���ļ��ɹ���
            return TRUE;
        }

        //���ļ����ɹ�
        //����ɵĻ��Ϊ�˶�λ��һ��Ŀ¼��

        back = TRUE;
        for (;;)
        {
            if (cur_dir_layer >= (DIRLAYER_MAX - 1))
            {
                //��Ŀ¼�Ѿ������ˣ����������ң��˻ص���Ŀ¼ȥ��
                break;
            }

            if (0 == vfs_dir(eh_vfs_mount, DIR_HEAD, NULL, EXT_NAME_ALL_DIR))
            {
                //��Ŀ¼û����Ŀ¼

                //�˻ص���Ŀ¼
                break;
            }

            //��Ŀ¼����Ŀ¼

            if((cur_dir_layer == 0) && (_is_filter() == TRUE)
                && (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL,
                EXT_NAME_ALL_DIR) == FALSE))
            {
                //�˻ص���Ŀ¼
                break;
            }

            //������Ŀ¼

            if (FALSE == vfs_cd(eh_vfs_mount, CD_SUB, NULL))
            {
                //������Ŀ¼���ɹ����϶��ǳ����ˣ�
                return FALSE;
            }
            //������Ŀ¼
            cur_dir_layer++;

            back = FALSE;
            break;
        }

        if (back == FALSE)
        {
            continue;
        }

        //׼�����븸Ŀ¼

        for (;;)
        {
            //��ǰ�Ǹ�Ŀ¼
            if (0 == cur_dir_layer)
            {
                return FALSE;
            }

            if (vfs_cd(eh_vfs_mount, CD_BACK, NULL) == FALSE)
            {
                //�ز�����Ŀ¼���϶��ǳ����ˣ�
                return FALSE;
            }
            //�ص���Ŀ¼��
            cur_dir_layer--;

            //����һ��Ŀ¼
            if (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR) == 0)
            {
                //û����һ��Ŀ¼��
                continue;
            }

            if((cur_dir_layer == 0) && (_is_filter() == TRUE)
                && (vfs_dir(eh_vfs_mount, DIR_NEXT, NULL,
                EXT_NAME_ALL_DIR) == FALSE))
            {
                //û����һ��Ŀ¼��
                continue;
            }

            //������һ��Ŀ¼
            vfs_cd(eh_vfs_mount, CD_SUB, NULL);
            cur_dir_layer++;
            break;
        }
    }

    //��Ӧ��ִ�е�����
}

bool music_rcp_fs_init(uint8 disk_type, uint8 drv_mode)
{
    uint8 drv_type;
    uint8 *drv_name;
    //int32 vfs_mount;
    
    if (eh_vfs_mount != -1)
    {
        return TRUE;
    }
    if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8*) card_drv_name;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8*) uhost_drv_name;
    }
    else
    {
        return FALSE;
    }

    //��װ�洢����
    if (sys_drv_install(drv_type, (uint32)drv_mode, drv_name) != 0)
    {
        return FALSE;
    }

    //����FS
    eh_vfs_mount = sys_mount_fs(drv_type, disk_type, 0);

    if (eh_vfs_mount == -1)
    {
        sys_drv_uninstall(drv_type);
        return FALSE;
    }
    //Ĭ�Ͻ����Ŀ¼
    vfs_cd(eh_vfs_mount, CD_ROOT, 0);

    music_rcp_dir_first();
    music_rcp_first_get();

    return TRUE;
}

bool music_rcp_fs_exit(uint8 disk_type, int32 file_sys_id)
{
    int32 ret_val = 0;

    uint8 drv_type;

    if (file_sys_id != -1)
    {
        ret_val = sys_unmount_fs(file_sys_id);
        file_sys_id = -1;
        if (ret_val == 0)
        {
            if (disk_type == DISK_H)
            {
                drv_type = DRV_GROUP_STG_CARD;
            }
            else if (disk_type == DISK_U)
            {
                drv_type = DRV_GROUP_STG_UHOST;
            }
            else
            {
                return FALSE;
            }

            ret_val = sys_drv_uninstall(drv_type);
        }
    }
    if (0 > ret_val)
    {
        return FALSE;
    }

    return TRUE;
}

