#include "music_scan_file.h"
#include "case_independent.h"
#include "common_func.h"

//���Կ��أ�scan��·��
//#define __SCAN_DEBUG__

//��ɨ�赽��Ŀ¼����vram
//ÿ��Ŀ¼��Ӧһ��list_dir_info_t
//��list_dir_info_t����ǰ���ǡ�ͷ��(list_head_info_t)

static uint8 s_buf[536]; //ǰ��12���ֽڷ�list_head_info_t������Ŀռ���������list_dir_info_t�ṹ
//Ҫ�����list_dir_info_t��ָ��(ָ��s_buf��ĳ��λ��)
static list_dir_info_t *s_plist;

//ͷ��ָ��(��¼���ж��ٸ��ļ�)(ָ��s_buf��ĳ��λ��)
static list_head_info_t *s_phead;

//��ǰʱ�䣬���ڿ���250ms�ͷ�10msʱ��Ƭ��ǰ̨Ӧ��
uint32 g_scan_cur_timer;

//�ļ�����
uint16 g_file_total = 0;

//��ǰĿ¼�µ��ļ����
uint16 g_dir_file_no = 0;

//Ŀ¼����
uint16 g_dir_total = 0;

//Ŀ¼�㼶
uint16 g_dir_layer = 0;

//����ļ�ϵͳid
int32 g_fs_mount_id = -1;

//�ļ��㼶
pdir_layer_t g_dir_layer_info;

//����ļ�λ����Ϣ
//pfile_offset_t g_file_offset;

//�Ƿ�ɨ�����
bool g_scan_end = FALSE;

//�Ƿ���� 0x80Ϊ����
uint8 g_scan_error = 0;

//�߳�ָ��
music_scan_param_t *g_pscan_handle;

//ɨ���Ŀ¼��0xff��ʾȱʡ��ʽ��0x80��ʾ¼��Ŀ¼�� 0��ʾ����Ŀ¼1�� 1��ʾ����Ŀ¼2
uint8 g_scan_dir_index;

//record cur disk flag;
uint8 g_scan_disk;

uint8 *g_scan_thread_task_addr;

static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

//¼��Ŀ¼������
static uint8 s_rec_folder_short[8 + 1] =
{ 0 };
static uint8 s_rec_folder_long[16 + 1] =
{ 0 };

static const uint32 info_vram_addr[] =
{
    VM_PLIST_INFO_START0,
    VM_PLIST_INFO_START1,
    VM_PLIST_INFO_START2,
    VM_PLIST_INFO_START3,
    VM_PLIST_INFO_START4,
    VM_PLIST_INFO_START5,
    VM_PLIST_INFO_START6,
    VM_PLIST_INFO_START7
};

//ע�⣺�������Щ���壬��Ҫ��DIR_INFO_SECTOR_COUNT���Ӧ
//���������������һ��Ԫ�أ�DIR_INFO_SECTOR_COUNTҲҪ��1
//ͬ��ҲҪ��DIR_INFO_SECTOR_SIZE���Ӧ

//��һ��512�ֽ�,��dir�ﵽ25��ʱ��ʼд
//�ڶ���512�ֽ�,��dir����51���ǿ�ʼд
//...
//��һ��Ԫ�أ�(VAM_BUF_SIZE - sizeof(list_head_info_t)) / sizeof(list_dir_info_t)
static const uint8 dir_count[ARR_COUNT(info_vram_addr)] =
{ 25, 51, 77, 102, 128, 153, 179, SUPPORT_MUSIC_DIR };
//��һ��512�ֽ�,��dir�ﵽ25��ʱ��ʼд512�ֽڡ�д����0���ֽڵ���Ϣûд��ȥ��
//�ڶ���512�ֽ�,��dir����51���ǿ�ʼд512�ֽڡ�д����8���ֽ�(��"β��")ûд��ȥ(����д����һ������)
//...
//ps.���һ��(��204��)list_dir_info_t�������Ĵ浽vram(����4���ֽ�)
static const uint8 tail[ARR_COUNT(dir_count)] =
{ 0, 8, 16, 4, 12, 0, 8, 0 };

//������
//  ��һ����̨���ݵ���FAT����ĵڼ����ļ���
//  �ڶ�����̨���ݵ���FAT����ĵڼ����ļ���
//  ��������̨���ݵ���FAT����ĵڼ����ļ���
//  ...
static const uint32 index_vram_addr[] =
{
    VM_PLIST_INDEX_START0,
    VM_PLIST_INDEX_START1,
    VM_PLIST_INDEX_START2,
    VM_PLIST_INDEX_START3,
    VM_PLIST_INDEX_START4,
    VM_PLIST_INDEX_START5,
    VM_PLIST_INDEX_START6,
    VM_PLIST_INDEX_START7
};

void music_scan_time_dly(uint32 dly)
{
    uint32 tmp_cur_timer = sys_get_ab_timer();

    if ((tmp_cur_timer - g_scan_cur_timer) >= 250)
    {
        g_scan_cur_timer = tmp_cur_timer;

        sys_os_time_dly(dly);
    }
    //sys_os_time_dly(dly);
}

#ifdef FILTER_RECORD_DIR

//���˸�Ŀ¼�µ�RECORDĿ¼
//dir_type: DIR_NEXT��
//����ֵ�������ǰĿ¼RECORD, ���л�����һ��Ŀ¼(����dir_type�л�)���ɹ���
//  �ŷ���TRUE,
//  ��������FALSE.
//  Ҳ����˵���ɹ�������RECORDĿ¼���ͷ���FALSE��û���������ͷ���TRUE
bool music_filtrate_dir(uint8 dir_type)
{
    uint8 dir_name_buffer[32];
    uint16 name_len;
    uint8 add_len, i, j;
    uint8 *buf;

    if (g_dir_layer != 0)
    {
        return FALSE;
    }

    name_len = vfs_get_name(g_fs_mount_id, dir_name_buffer, 16);

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
        if (s_rec_folder_short[j] != buf[i])
        {
            return FALSE;
        }
        i = i + add_len;
    }

    //���˳ɹ�������/��һ���ļ��з���
    if (0 != vfs_dir(g_fs_mount_id, dir_type, NULL, EXT_NAME_ALL_DIR))
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

#endif //#ifdef FILTER_RECORD_DIR

//װ��������ʺ��ļ�ϵͳ
int32 __section__(".init") music_scan_fs_init(uint8 disk_type, uint8 drv_mode)
{
    uint8 drv_type;
    uint8 *drv_name;
    int32 vfs_mount;

    g_scan_disk = disk_type;

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
        return -1;
    }

    //��װ�洢����
    if (sys_drv_install(drv_type, (uint32)drv_mode, drv_name) != 0)
    {
        return -1;
    }

    //����FS
    vfs_mount = sys_mount_fs(drv_type, disk_type, 0);

    if (vfs_mount == -1)
    {
        sys_drv_uninstall(drv_type);
    }

    return vfs_mount;
}

//ж��������ʺ��ļ�ϵͳ
bool __section__(".exit") music_scan_exit(uint8 disk_type, int32 file_sys_id)
{
    int32 ret_val = 0;

    uint8 drv_type;

    //��Ƶ
    adjust_freq_set_level(AP_FRONT_HIGH_PRIO, FREQ_NULL, FREQ_NULL);

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

//��ʼ���ļ�ѡ����
//disk_flag: �����̷�
bool __section__(".init") music_file_init(uint8 disk_flag)
{
    bool init_ret = FALSE;

    //�ļ�ѡ������ʼ��
    g_fs_mount_id = music_scan_fs_init(disk_flag, 0);

    if (g_fs_mount_id != -1)
    {
        init_ret = TRUE;
        //Ĭ�Ͻ����Ŀ¼
        vfs_cd(g_fs_mount_id, CD_ROOT, 0);

        if (g_scan_dir_index != 0xff)
        {
            //����Ŀ¼ record
            if (g_scan_dir_index == 0x80)
            {
                //init_ret = TRUE;

                //record dir
                if (vfs_dir(g_fs_mount_id, DIR_HEAD, s_rec_folder_long, EXT_NAME_ALL_DIR) == FALSE)
                {
                    if (vfs_dir(g_fs_mount_id, DIR_HEAD, s_rec_folder_short, EXT_NAME_ALL_DIR) == FALSE)
                    {
                        init_ret = FALSE;
                    }
                }

            }
#if (SUPPORT_MUSIC_FOLDER == 1)
            else
            {
                //special folder
                //init_ret = vfs_dir(g_fs_mount_id, DIR_HEAD, g_music_folder_name_long[g_scan_dir_index], EXT_NAME_ALL_DIR);
                if(vfs_dir(g_fs_mount_id, DIR_HEAD, g_music_folder_name_long[g_scan_dir_index], EXT_NAME_ALL_DIR)
                        == FALSE)
                {
                    //init_ret = TRUE;
                    if(vfs_dir(g_fs_mount_id, DIR_HEAD, g_music_folder_name_short[g_scan_dir_index], EXT_NAME_ALL_DIR)
                            == FALSE)
                    {
                        init_ret = FALSE;
                    }
                }
            }
#endif
            if (init_ret == TRUE)
            {
                //�������ļ��У���ø����ļ��е��ļ�
                vfs_cd(g_fs_mount_id, CD_SUB, NULL);
                //g_dir_layer++;
            }
        }
    }
    return init_ret;
}

//��ɨ�赽��Ŀ¼д��vram
void __section__(".save") music_dirinfo_tovm(uint32 vm_offset, uint8 head_offset)
{
    sys_vm_write(s_buf, vm_offset, VAM_BUF_SIZE);
    //move to buffer header
    if (head_offset != 0)
    {
        libc_memcpy(s_buf, &s_buf[VAM_BUF_SIZE], (uint32 )head_offset);
    }
    s_plist = (list_dir_info_t*) (s_buf + head_offset);
}

//���ɨ�赽��Ŀ¼
//���ʱ�����ˣ��ʹ棬����TRUE
//���ʱ����û���������ܴ棬Ҳ����TRUE
//����Ѿ��治���ˣ��ͷ���FALSE
bool __section__(".save") music_store_dirinfo(void)
{
    uint8 cur_layer, i;
    pfile_offset_t g_file_offset;
    
    if (g_dir_total >= dir_count[ARR_COUNT(dir_count) - 1])
    {
        return FALSE;
    }

    //Ŀ¼��������
    g_dir_total++;
    //�ڴ˴���ȡ��ǰ�ļ���·���㼶��Ϣ�洢
    s_plist->begin_index = (g_file_total + 1);
    vfs_file_dir_offset(g_fs_mount_id, &g_dir_layer_info, &g_file_offset, 0);
    //�㼶��(buffer�����һ���ֽڣ�����ǡ���ǰ�㼶��)
    cur_layer = g_dir_layer_info.buffer[MAX_DIR_LAYER * 8];
    s_plist->cur_layer = cur_layer;
    if (cur_layer > 0)
    {
        cur_layer--;
    }
    //��Ŀ¼λ����Ϣ
    libc_memcpy(&s_plist->far_cluster, &g_dir_layer_info.buffer[cur_layer * 8], 8);
    //�ļ���Ϣ
    libc_memcpy(&s_plist->cur_cluster, &g_file_offset, sizeof(pfile_offset_t));

    //check if need store vram
    for (i = 0; i < ARR_COUNT(dir_count); i++)
    {
        if (g_dir_total == dir_count[i])
        {
            break;
        }
    }
    if (i == ARR_COUNT(dir_count))
    {
        s_plist++;
    }
    else
    {
        music_dirinfo_tovm((uint32) info_vram_addr[i], tail[i]);
    }
    return TRUE;
}


//���ʽ��ͣ�
//  �����е���ţ�
//      ����FAT��ó�������š�
//      ���裺FAT��ĵ�һ����һ��TXT�ļ����ڶ�����һ��mp3�ļ���
//      ��������һ��TXT����������һ��mp3�ļ���
//      ��ô��һ��mp3�ļ��Ĵ����е���ž���1���ڶ���mp3�ļ��Ĵ����е���ž���2��
//
//  ��Ŀ����
//      ��¼��vram�����һ�ű�
//      �������ű�������ǣ�5, 3, 6...
//      ��ʾ��һ������̨���ݡ����Ǵ����е����Ϊ5���ļ���
//      �ڶ�������̨���ݡ����Ǵ����е����Ϊ3���ļ���
//      ����������̨���ݡ����Ǵ����е����Ϊ6���ļ���
//      ...
//
//  Ŀ¼�б�
//      ��¼��vram�����һ�ű�
//      ���б�ͷ�����
//      ÿ�������Ӧһ��Ŀ¼��
//      ��������Ƶ�ļ���Ŀ¼�Ż�����ű�(��������ָ���ǡ�ֱ�Ӱ�������
//          ������Ŀ¼��û����Ƶ�ļ�����������Ŀ¼���У�
//          ��ô�����Ŀ¼���ܽ���)

//��ʼ����Ŀ��
void __section__(".save") music_save_init_list(void)
{
    uint16 i, j;
    //�����е����
    uint16 n = 1;
    uint16 *p_index_list; //�����е���Ŷ���2���ֽ�
    //Ҫ��ŵ���������
    uint16 wr_cnt;
    //ÿ��������ŵĸ�������
    uint16 cap_bum;

    //ÿ��������ŵĸ�������
    cap_bum = (VAM_BUF_SIZE / sizeof(uint16));
    //���������Ҫ��ŵ���������
    wr_cnt = g_file_total / cap_bum;
    //����++
    if ((g_file_total % cap_bum) != 0)
    {
        wr_cnt++;
    }
    if (wr_cnt > ARR_COUNT(index_vram_addr))
    {
        //ֻ��8������
        wr_cnt = ARR_COUNT(index_vram_addr);
    }

    //ÿ��д1 sector
    for (i = 0; i < wr_cnt; i++)
    {
        libc_memset(s_buf, 0, VAM_BUF_SIZE);
        p_index_list = (uint16 *) s_buf;
        //���дΪn
        for (j = 0; j < cap_bum; j++)
        {
            //just write 1~g_file_total
            if (n <= g_file_total)
            {
                *p_index_list = n;
                n++;
            }
            else
            {
                break;
            }
            p_index_list++;
        }
        sys_vm_write(s_buf, index_vram_addr[i], j * sizeof(uint16));
    }
}

//���б�ͷд��vram�� ͬʱ��ʼ������Ŀ����
void __section__(".save") music_save_header(void)
{
    uint8 i = 0;
    //���һ��дlist_dir_info_t��ʱ��д��дlist_head_info_t
    //  �����д�����������Ϊ0
    uint8 head_size = 0;
    //Ҫд�뼸���ֽ�
    uint32 wr_bytes;
    //д���vram�ĵ�ַ
    uint32 wr_addr;
    //Ҫд����list_dir_info_t
    uint16 wr_dir;

    for (i = 0; i < ARR_COUNT(dir_count); i++)
    {
        if (g_dir_total <= dir_count[i])
        {
            break;
        }
    }
    if (i == ARR_COUNT(dir_count))
    {
        //������������Ϊǰ��Ĵ���(music_get_nextfile)�Ѿ���֤��
        i = ARR_COUNT(dir_count) - 1;
    }

    //���һ����Ҫд��
    if (g_dir_total == dir_count[i])
    {
        //��������£��Ѿ���music_get_nextfile->music_store_dirinfo->
        //  music_dirinfo_tovm��д��vram��
        //  ֻ�ǻ���һ��β��û��д
        wr_bytes = tail[i];
        if(i < (ARR_COUNT(dir_count) - 1))
        {
            wr_addr = info_vram_addr[i + 1];
        }
        else
        {
            //��ʱwr_bytesΪtail[���һ��]��Ϊ0
            wr_addr = 0;//Ϊ���⾯��
        }
    }
    else
    {
        wr_addr = info_vram_addr[i];

        if (i > 0)
        {
            i--;
            wr_dir = g_dir_total - dir_count[i];
        }
        else
        {
            head_size = sizeof(list_head_info_t);
            wr_dir = g_dir_total;
        }
        //Ҫд����ٸ��ֽ�
        wr_bytes = head_size + tail[i] + wr_dir * sizeof(list_dir_info_t);
    }

    if (wr_bytes != 0)
    {
        sys_vm_write(s_buf, wr_addr, wr_bytes);
    }

    //д��ͷ��
    s_phead = (list_head_info_t *) s_buf;
    //dir num bigger than bufer save
    if (g_dir_total >= dir_count[0])
    {
        //�ѵ�һ������������
        wr_bytes = VAM_BUF_SIZE;
        sys_vm_read(s_buf, info_vram_addr[0], VAM_BUF_SIZE);
    }
    else
    {
        //���ö��ˣ���Ϊs_buf����Ķ������ǵ�һ��������
        wr_bytes = g_dir_total * sizeof(list_dir_info_t) + sizeof(list_head_info_t);
    }
    s_phead->file_total = g_file_total;
    s_phead->dir_total = g_dir_total;
    s_phead->cur_disk = g_scan_disk;

    if(g_vm_file_newest_time < g_file_newest_time)
    {
        g_vfs_changed = TRUE;
        PRINT_INFO("music files changed.");
    }
    
    s_phead->file_newest_time = g_file_newest_time;
    
    sys_vm_write(s_buf, info_vram_addr[0], wr_bytes);
    //��ʼ����Ŀ��
    music_save_init_list();
}

#ifdef __SCAN_DEBUG__
static void _print_name(void)
{
    uint8 buf[16];

    vfs_get_name(g_fs_mount_id, buf, sizeof(buf) / 2 - 2);
    PRINT_DATA(buf, sizeof(buf));
}
#endif

//��ȡ��ǰĿ¼�µ��¸��ļ�
//����ɹ�������TRUE;���򷵻�FALSE
static bool _dir_next(void)
{
    uint8 dir_type;
    if (g_dir_file_no == 0)
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
    if (vfs_dir(g_fs_mount_id, dir_type, NULL, MUSIC_BITMAP) != 0)
    {
        //�ҵ��ļ����������
        //sys_os_time_dly(1);
        music_scan_time_dly(1);
        g_dir_file_no++;

#ifdef __SCAN_DEBUG__
        PRINT_INFO("*");
        //_print_name();
#endif
        get_newest_file_time(g_fs_mount_id);

        return TRUE;
    }

    return FALSE;
}

//����ҵ��ˣ�����1
//����ļ�ϵͳ�����ˣ�����-1
//���vram�����ˣ�����-2
//���û���ҵ�������0
static int8 _sub(void)
{
    //�������ļ��У���ø����ļ��е��ļ�����
    if (FALSE == vfs_cd(g_fs_mount_id, CD_SUB, NULL))
    {
        //��DIR_HEAD��ȴ����CD_SUB
        g_scan_error = 0x80;
        return -1;
    }

#ifdef __SCAN_DEBUG__
    PRINT_INFO(">");
    _print_name();
#endif

    //Ŀ¼�㼶����
    g_dir_layer++;
    //��ǰĿ¼�µ��ļ���Ź���
    g_dir_file_no = 0;
    // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
    if (_dir_next() == FALSE)
    {
        return 0;
    }

    if(music_store_dirinfo() == TRUE)
    {
        return 1;
    }

    return -2;
}

//����Ŀ¼��
//1.���Ŀ¼���������ļ����ͼ���vram
//2.���û�У����Ͳ����¼(��ʹ��������Ŀ¼��)
//
//Ŀ¼����MAX_DIR_LAYER
//����ܼ�¼dir_count[ARR_COUNT(dir_count) - 1]��Ŀ¼
//����ֵ���޷�����ʱ���ͷ���FALSE
bool music_get_nextfile(void)
{
#ifdef FILTER_RECORD_DIR
    bool dir_ret;
#endif
    bool dir_val;
    int8 n;
    //�ڵ�ǰĿ¼����
    bool cur_dir_flag;

    //�ڵ�ǰĿ¼�������ļ�
    dir_val = _dir_next();
    if(dir_val == TRUE)
    {
        return TRUE;
    }

    if(g_scan_dir_index != 0xff)
    {
        //����Ŀ¼
        //��ɨ����Ŀ¼
        return FALSE;
    }

    cur_dir_flag = TRUE;

    for(;;)
    {
        if (g_pscan_handle->thread_end_flag == TRUE)
        {
            return FALSE;
        }

        //sys_os_time_dly(1);
        music_scan_time_dly(1);

        if(cur_dir_flag == TRUE)
        {
            if (g_dir_layer >= (MAX_DIR_LAYER - 1)) //�����ǰ��8��Ŀ¼�Ͳ������ļ���
            {
                cur_dir_flag = FALSE;
                continue;
            }

#ifdef FILTER_RECORD_DIR
            //Ѱ�Ҹ�Ŀ¼�ĵ�һ�����ļ���
            dir_ret = (bool)vfs_dir(g_fs_mount_id, DIR_HEAD, NULL, EXT_NAME_ALL_DIR);
            if ((dir_ret != 0) && (music_filtrate_dir(DIR_NEXT) == FALSE))
#else
            if (vfs_dir(g_fs_mount_id, DIR_HEAD, NULL, EXT_NAME_ALL_DIR) != 0)
#endif
            {

                n = _sub();
                if(n == 0)
                {
                    //û�ҵ�
                    //�����Ŀ¼
                    continue;
                }

                if(n < 0)
                {
                    return FALSE;
                }

                return TRUE;
            }
            cur_dir_flag = FALSE;
            continue;
        }

        //���û�����ļ��У���������һ�㣬Ѱ���뵱ǰĿ¼ƽ������һ��Ŀ¼��ƽ��Ѱ�ң�
        //�����ǰ�Ǹ�Ŀ¼�򷵻�
        if (g_dir_layer == 0)
        {
            return FALSE;
        }
        //������Ŀ¼
        if (FALSE == vfs_cd(g_fs_mount_id, CD_BACK, NULL))
        {
            //�쳣����
            g_scan_error = 0x80;
            return FALSE;
        }

#ifdef __SCAN_DEBUG__
        PRINT_INFO("<");
#endif

        //Ŀ¼�㼶��
        g_dir_layer--;
#ifdef FILTER_RECORD_DIR
        dir_ret = (bool)vfs_dir(g_fs_mount_id, DIR_NEXT, NULL, EXT_NAME_ALL_DIR);
        if ((dir_ret == 0) || (TRUE == music_filtrate_dir(DIR_NEXT)))
#else
        if (vfs_dir(g_fs_mount_id, DIR_NEXT, NULL, EXT_NAME_ALL_DIR) == 0)
#endif
        {
            continue;
        }

#ifdef __SCAN_DEBUG__
        PRINT_INFO("-");
        //_print_name();
#endif

        n = _sub();
        if(n == 0)
        {
            //û�ҵ�
            //�����Ŀ¼
            cur_dir_flag = TRUE;
            continue;
        }

        if(n < 0)
        {
            return FALSE;
        }

        return TRUE;
    }
}

bool __section__(".init") music_scan_init(void *param)
{
    //��ʼ�����
    bool init_result;

    app_param_e scan_mode;

    uint8 parm_val;

    //��Ƶ
    adjust_freq_set_level(AP_FRONT_HIGH_PRIO, FREQ_LEVEL11, FREQ_NULL);

    //disk flag
    parm_val = *(uint8 *) param;
    //scan recope judge
    scan_mode = *((uint8 *) param + 1);
    //scan mode selcet

    switch (scan_mode)
    {
#if (SUPPORT_MUSIC_FOLDER == 1)
        //sepcial folder1
        case PARAM_MUSIC_FOLDER1:
        g_scan_dir_index = 0;
        break;
        //sepcial folder2
        case PARAM_MUSIC_FOLDER2:
        g_scan_dir_index = 1;
        break;
#endif
        //record folder
        case PARAM_RECORD_CPLAY:
        case PARAM_RECORD_UPLAY:
        g_scan_dir_index = 0x80;
        break;
        //scan all disk
        default:
        g_scan_dir_index = 0xff;
        break;
    }

    s_plist = (list_dir_info_t *) (s_buf + sizeof(list_head_info_t));

    sys_vm_read(&g_vm_file_newest_time, info_vram_addr[0]+(((uint32)&s_phead->file_newest_time) - (uint32)s_phead), 4);
    //���buffer
    libc_memset(s_buf, 0, sizeof(s_buf));

    com_load_share_data(COM_SHARE_DATA_REC_FOLDER, s_rec_folder_short,
            sizeof(s_rec_folder_short) - 1, 0);
    com_load_share_data(COM_SHARE_DATA_REC_FOLDER, s_rec_folder_long,
            sizeof(s_rec_folder_long) - 1, 1);

    //��ʼ���ļ�ѡ����
    init_result = music_file_init(parm_val);

    //��ճ����־
    g_scan_error = 0;
    g_scan_end = FALSE;
    //g_pscan_handle->thread_end_flag = FALSE;

    return init_result;
}

//�ļ�ɨ����̺߳���
void* music_scan_file(void* param)
{
    uint8 parm_val;
    uint8 scan_mode;
    bool init_result;

    //disk flag
    parm_val = *(uint8 *) param;
    scan_mode = *((uint8 *) param + 1);

    init_result = music_scan_init(param);
    while (1)
    {
        /* �ж�������Ҫ����߳����������Ĺ��ܱ�־λ */
        libc_pthread_mutex_lock(g_pscan_handle->mutex);
        if (g_pscan_handle->thread_end_flag == TRUE)
        {
            goto loop_exit;
        }

        //��ʼ��ʧ������ɨ��
        if (init_result == FALSE)
        {
            g_scan_error = 0x80;
            g_scan_end = TRUE;
        }

        if (g_scan_end == TRUE)
        {
            goto loop_continue;
        }

        //���涼��g_scan_end == FALSE�����

        //ɨ�����
        if (music_get_nextfile() == FALSE)
        {
            //ɨ����ɱ�������
            g_scan_end = TRUE;
            music_save_header();
//#ifdef __SCAN_DEBUG__
//          PRINT_INFO_INT("scan over:", g_file_total);
//#endif
            goto loop_continue;
        }

        if ((g_file_total == 0) && (g_dir_layer == 0))
        {
            //��ǰ�Ǹ�Ŀ¼���ǵ�һ���ļ��򱣴�
            music_store_dirinfo();
        }
        g_file_total++;

        if ((g_dir_total > dir_count[ARR_COUNT(dir_count) - 1])
                //����������������ǰ��Ĵ���(music_get_nextfile)�Ѿ���֤��
                || (g_file_total >= SUPPORT_MUSIC_FILE))
        {
            g_scan_end = TRUE;
            //ɨ����ɱ�������
            music_save_header();
        }

        loop_continue:
        libc_pthread_mutex_unlock(g_pscan_handle->mutex);
        //����10ms �����񽻻�
        //sys_os_time_dly(2);
        music_scan_time_dly(2);
    }
    loop_exit:
    //��ӡɨ�赽�ĸ�������
    PRINT_INFO_INT("scan over:", g_file_total);
    //�ļ�ѡ�����˳�
    music_scan_exit(parm_val, g_fs_mount_id);
    g_pscan_handle->thread_end_flag = FALSE;

    libc_pthread_mutex_unlock(g_pscan_handle->mutex);

    /*ע�⣺libc_pthread_exit�޷�����*/
    libc_pthread_exit();
    return 0;
}

//���ͻ��˵Ľӿں���
int __section__(".cmd") music_scan_cmd(void* ms_handle, music_scan_cmd_t cmd, unsigned int param)
{
    int32 ret = 0;

    g_pscan_handle = &g_scan_param;

    //ȷ��mutex�Ѿ���ʼ��
    if (cmd == MUSIC_SCAN_OPEN)
    {
        if (music_scan_open((void*) param) == NULL)
        {
            *((music_scan_param_t **) ms_handle) = NULL;
            ret = -1;
        }
        else
        {
            *((music_scan_param_t **) ms_handle) = g_pscan_handle;
        }
        goto music_scan_cmd_exit;
    }
    /* ���ӻ������Ա�֤�õ�����Ϣͬ����scan�߳� */
    libc_pthread_mutex_lock(g_pscan_handle->mutex);
    switch (cmd)
    {
        case MUSIC_SCAN_GET_STATUS:
        //�鿴�Ƿ�ɨ�����
        *((uint8*) param) = (g_scan_end | g_scan_error | (g_vfs_changed << 1));
        if (g_scan_error == 0x80)
        {
            //���buffer
            libc_memset(s_buf, 0, sizeof(s_buf));
        }
        break;

        case MUSIC_SCAN_CLOSE:
        ret = music_scan_close(ms_handle);
        /*mutex destroy, can not unlock*/
        goto music_scan_cmd_exit;
        default:
        ret = -1;
        break;
    }

    libc_pthread_mutex_unlock(g_pscan_handle->mutex);

    music_scan_cmd_exit:
    return ret;
}
