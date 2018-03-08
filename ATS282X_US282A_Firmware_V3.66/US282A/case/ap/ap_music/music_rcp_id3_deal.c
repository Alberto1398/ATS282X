#include "music.h"

uint8 g_id3_title_buf[64];
uint8 g_id3_artist_buf[32];
uint8 g_id3_album_buf[32];
uint8 g_id3_filename_buf[64];

//MUS_STATIC id3_info_t cur_id3_info _BANK_DATA_ATTR_;
//MUS_STATIC id3_info_t cur_id3_info;
pfile_offset_t g_temp_pfile_offset;
pdir_layer_t g_temp_pdir_layer;

//��һ��������ͷ12���ֽ�װ����list_head_info_t
//���ſ�ʼ��list_dir_info_t
//list_dir_info_t��20���ֽڵ�
//
//��һ��Ԫ�أ���һ��512�ֽڴ�ĵ�һ��list_dir_info_t�ṹ�����(1��ʼ)��1
//�ڶ���Ԫ�أ��ڶ���512�ֽڴ�ĵ�һ��list_dir_info_t�ṹ�����(1��ʼ)��26
//������Ԫ�أ�������512�ֽڴ�ĵ�һ��list_dir_info_t�ӿڵ����(1��ʼ)��52
//              ���������ͷ��entry_off[2](��8)�ֽ�װ����51��(0��ʼ)��һ����
//...
//���һ��Ԫ�أ��ڰ˸�512�ֽڵ����һ��list_dir_info_t�ṹ(����������)�����
MUS_STATIC const uint8 dir_bound[] =
{
    1, 26, 52, 78, 103, 129, 154, 180, 204
};

MUS_STATIC const uint8 entry_off[] =
{
    12, 0, 8, 16, 4, 12, 0, 8
};

//for vram addr
MUS_STATIC const uint32 vram_addr[] =
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

//����const data ���к�׺����
MUS_STATIC const uint8 support_ext[MUSIC_EXT_NUM][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "APE", //ape
    "FLA", //flac
    "OGG", //ogg
};

MUS_STATIC const id3_type_e support_id3_type[] =
{
    ID3_TYPE_MP3, //mp1
    ID3_TYPE_MP3, //mp2
    ID3_TYPE_MP3, //mp3
    ID3_TYPE_WMA, //wma
    ID3_TYPE_APE, //ape
    ID3_TYPE_FLAC, //fla
    ID3_TYPE_OGG, //ogg
};

 id3_type_e _check_id3_type(uint8 *extname)
{
    uint8 cnt;
    id3_type_e ret;
    //ת����ǰ��׺��(ת�ɴ�д)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < MUSIC_EXT_NUM; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    if (cnt == MUSIC_EXT_NUM)
    {
        ret = ID3_TYPE_END;
    }
    return ret;
}

 bool _rcp_get_id3_info(id3_info_t *id3_info_ptr, uint32 file_ext_name)
{
    //file id3 check
    id3_type_e cur_id3_type;
    cur_id3_type = _check_id3_type((uint8 *) &file_ext_name);
    if (cur_id3_type != ID3_TYPE_END)
    {
        //����id3_info_t
        fsel_get_id3_info(id3_info_ptr, NULL, cur_id3_type);
    }
    return TRUE;
}

 bool music_get_name_info(music_id3_info_t *rcp_id3_info)
{
    //save current path
    //vfs_file_dir_offset(eh_vfs_mount, &g_temp_pdir_layer, &g_temp_pfile_offset, 0);

    //��ȡ�ļ���
    vfs_get_name(eh_vfs_mount, rcp_id3_info->pfile_buf, rcp_id3_info->name_len / 2 - 2);

    if(rcp_id3_info->p_id3_info.tit2_buffer[0] == 0)
    {
        libc_memcpy(rcp_id3_info->p_id3_info.tit2_buffer,
                rcp_id3_info->pfile_buf,
                (uint32)MIN(rcp_id3_info->p_id3_info.tit2_length,
                rcp_id3_info->name_len));
    }
    //restore saveed path
    //vfs_file_dir_offset(eh_vfs_mount, &g_temp_pdir_layer, &g_temp_pfile_offset, 1);
    return TRUE;
}

//����Ŀ¼������ҵ�Ŀ¼��
//dir_no��1��ʼ���
//���û���ҵ�(������)������FALSE
 bool _get_dir_by_dirno(uint16 dir_no, list_dir_info_t *p_dir)
{
    uint8 sector_index;
    uint32 pos;
    uint32 offset;
    uint32 byte_count;

    if (dir_no > g_vmdir_total)
    {
        return FALSE;
    }

    //��������ĸ�����

    //��һ��������ͷsizeof(list_head_info_t)�ֽڲ��Ǵ�ŵ�list_dir_info_t
    pos = sizeof(list_head_info_t) + (dir_no - 1) * sizeof(list_dir_info_t);
    sector_index = (uint8) (pos / DIR_INFO_SECTOR_SIZE + 1); //��1��ʼ���
    if (sector_index > DIR_INFO_SECTOR_COUNT)
    {
        return FALSE;
    }

    if (sector_index != g_sector_no)
    {
        g_sector_no = sector_index;
        sys_vm_read(g_sector_buf, vram_addr[g_sector_no - 1], sizeof(g_sector_buf));
    }

    offset = pos % DIR_INFO_SECTOR_SIZE;
    if ((offset + sizeof(list_dir_info_t)) < DIR_INFO_SECTOR_SIZE)
    {
        libc_memcpy(p_dir, &g_sector_buf[offset], sizeof(*p_dir));
        return TRUE;
    }

    //����һ������g_sector_buf��
    byte_count = DIR_INFO_SECTOR_SIZE - offset;
    libc_memcpy(p_dir, &g_sector_buf[offset], byte_count);

    //�ٶ�ʣ�µĲ���
    if (g_sector_no >= (sizeof(vram_addr)/sizeof(uint32)))
    {
        g_sector_no = 0;
    }
    sys_vm_read(g_sector_buf, vram_addr[g_sector_no], sizeof(g_sector_buf));
    g_sector_no++;
    
    libc_memcpy((uint8 * )p_dir + byte_count, g_sector_buf,
            sizeof(list_dir_info_t) - byte_count);

    return TRUE;
}

//��λ�ļ����ڵ�Ŀ¼��
//file_no: �ļ����(��1��ʼ���)
//�ҵ��ˣ����װ��g_cur_dir������TRUE
//���û�ҵ�������FALSE
bool _get_dir_by_fileno(uint16 file_no)
{
    list_dir_info_t next_dir;

    if (file_no < g_cur_dir.begin_index)
    {
        //�ӵ�һ��DIR��ʼ��
        _get_dir_by_dirno(1, &g_cur_dir);
        g_cur_dir_no = 1;
    }

    for (;;)
    {
        if (!_get_dir_by_dirno(g_cur_dir_no + 1, &next_dir))
        {
            //�Ѿ�������
            if (file_no >= g_cur_dir.begin_index)
            {
                //�ҵ���
                return TRUE;
            }
            //�����ܽ�������
            return FALSE;
        }

        if (file_no < next_dir.begin_index)
        {
            //�ҵ���
            return TRUE;
        }

        libc_memcpy(&g_cur_dir, &next_dir, sizeof(g_cur_dir));
        g_cur_dir_no++;
    }
}

//file_index: �ļ����(��1��ʼ���)
bool music_rcp_get_list_id3(music_id3_info_t *rcp_id3_info, uint16 file_index)
{
    uint32 cur_ext_name = 0;
    uint16 gap_num = 0;
    list_dir_info_t entry;
    bool prev_flag = FALSE;

    if(g_last_dir_no == 0)
    {
        //music scan�ոս���
        g_cur_dir_no = 1;
        _get_dir_by_dirno(g_cur_dir_no, &g_cur_dir);
        g_last_dir_no = g_cur_dir_no;
        vfs_get_name(eh_vfs_mount, &cur_ext_name, 0);
    }
    else
    {
        libc_memcpy(&entry, &g_cur_dir, sizeof(entry));

        if (file_index < g_cur_dir.begin_index)
        {
            prev_flag = TRUE;
        }
        if (!_get_dir_by_fileno(file_index))
        {
            //����ļ������˼�¼(vram����û�м�¼��)
            return FALSE;
        }

        if (g_cur_dir_no != g_last_dir_no)
        {
            //����Ŀ¼
            if(!vfs_jump_to_direntry(eh_vfs_mount,
                ((prev_flag == TRUE) ? PREV_FLAG : 0), &g_cur_dir, &entry))
            {
                PRINT_ERR_STR("E1");
                return FALSE;
            }
            g_last_dir_no = g_cur_dir_no;
            gap_num = (file_index - (g_cur_dir.begin_index - 1));
            prev_flag = FALSE;
        }
        else if(file_index > g_last_get_index)
        {
            prev_flag = FALSE;
            gap_num = file_index - g_last_get_index;
        }
        else
        {
            prev_flag = TRUE;
            gap_num = g_last_get_index - file_index;
        }
        cur_ext_name = vfs_dir_current_entry_file(eh_vfs_mount,
            (prev_flag ? PREV_FLAG : 0) | gap_num,
            &g_cur_dir, MUSIC_BITMAP);
    }
    if (cur_ext_name == 0)
    {
        PRINT_ERR_INT("E2:", gap_num);
        return FALSE;
    }

#if ((SUPPORT_PLIST_TITLE == 1) || (SUPPORT_PLIST_ARTIST == 1) || (SUPPORT_PLIST_ALBUM == 1))
    //��ȡID3
    _rcp_get_id3_info(&rcp_id3_info->p_id3_info, cur_ext_name);
#endif
    //��ȡ�ļ���
    music_get_name_info(rcp_id3_info);
    g_last_get_index = file_index;

    return TRUE;
}

bool __section__(".text.bank2") music_rcp_get_cur_info(music_id3_info_t *rcp_id3_info)
{
    bool get_ret;
    uint32 now_ext_name;

    //save current path
    vfs_file_dir_offset(eh_vfs_mount, &g_temp_pdir_layer, &g_temp_pfile_offset, 0);
    //set to cur file path
    get_ret = vfs_file_dir_offset(eh_vfs_mount, &g_file_path_info.file_path.dirlocation.dir_layer_info,
            &g_file_path_info.file_path.dirlocation.file_info.file_extend_info.cluster_no, 1);
    if (get_ret == TRUE)
    {
        vfs_get_name(eh_vfs_mount, &now_ext_name, 0);
        _rcp_get_id3_info(&rcp_id3_info->p_id3_info, now_ext_name);
        music_get_name_info(rcp_id3_info);
    }

    //restore saveed path
    vfs_file_dir_offset(eh_vfs_mount, &g_temp_pdir_layer, &g_temp_pfile_offset, 1);
    return get_ret;
}


bool get_dir_by_dirno(uint16 dir_no, list_dir_info_t *p_dir)
{
    return _get_dir_by_dirno(dir_no,p_dir);
}
