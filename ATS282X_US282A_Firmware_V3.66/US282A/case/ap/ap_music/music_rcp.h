/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������Ŵ���
 * ���ߣ�Gary Wang
 ********************************************************************************/
#ifndef __APP_MUSIC_RCP_H__
#define __APP_MUSIC_RCP_H__

#include "psp_includes.h"
#include "case_include.h"
#include "music_common.h"
#include "enhanced.h"
#include "dir_list.h"

//for rcp get info
typedef struct
{
    //�����л����ĸ���
    uint16 file_seq[2];
    //����ɨ���߳�״̬(1��ʾɨ�����)
    uint8 scan_status;
    //�����Ƿ��Ѿ��������л�������RCP��״̬������0
    uint8 file_switch;
    //���浱ǰ�����Ƿ��и��
    uint8 lyric_exist;

} rcp_file_status_t;

//����׺������
#define MUSIC_EXT_NUM 7

//#define LIST_BUF_SIZE 1024

typedef struct
{
    id3_info_t p_id3_info;
    /*! �ļ���buffer */
    uint8 *pfile_buf;
    /*! �ļ���buffer���� */
    uint8 name_len;

} music_id3_info_t;

/*typedef struct
 {
 uint16 dest_num;//��ȡid3���ļ�����ţ�δ����
 music_id3_info_t p_id3_info;
 }mrcp_id3_infor_t;*/
//RCP �ӿڣ��ص�������
typedef struct
{
    /*! ѭ��ģʽ����μ� fsel_mode_e ���� */
    uint8 repeat_mode;
    /*! ����״̬����μ� play_status_e ���� */
    uint8 play_status;
    /*! AB����״̬����μ� ab_status_e ����  */
    uint8 ab_status;
    /*! �����״̬����μ� fast_status_e ����  */
    uint8 fast_status;
    /*! ����״̬����μ� eg_err_e ����  */
    uint8 err_status;
    /*! ��ʴ��ڱ�־ */
    uint8 lyric;
    /*! �ļ��л���־  */
    uint8 file_switch;
    uint8 reserve;
    /*! ��ǰ����ʱ�䣬��msΪ��λ */
    uint32 cur_time;
    /*! ��ǰ������ʱ�䣬��msΪ��λ */
    uint32 total_time;
    /*! ��ǰ������� */
    uint32 file_seq;
    /*! �������� */
    uint32 file_total;
} music_rcp_info_t;

typedef struct
{
    com_rcp_info_t com_info;
    music_rcp_info_t app_info;
} music_global_rcp_info_t;

typedef enum
{
    MUSIC_TYPE_MP3, //mp3
    MUSIC_TYPE_WMA, //wma
    MUSIC_TYPE_WAV, //wav
    MUSIC_TYPE_FLA, //flac
    MUSIC_TYPE_APE, //ape
    MUSIC_TYPE_MP1, //mp3
    MUSIC_TYPE_MP2,
} music_type_e;

typedef struct
{
    /*! ��ǰ������� */
    uint32 file_seq;
    /*! �������ͣ���μ� music_type_e ���� */
    uint8 music_type;
#if (SUPPORT_PLIST_FILE == 1)
    /*! �ļ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 file_type;
    /*! �ļ���buffer */
    uint8 file_buffer[64];
#endif
#if (SUPPORT_PLIST_TITLE == 1)
    /*! ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 tit2_type;
    /*! ����buffer */
    uint8 tit2_buffer[Id3BufSIZE];
#endif
#if (SUPPORT_PLIST_ARTIST == 1)
    /*! �ݳ��߱������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 tpe1_type;
    /*! �ݳ���buffer */
    uint8 tpe1_buffer[ArtBufSIZE];
#endif
#if (SUPPORT_PLIST_ALBUM == 1)
    /*! �ļ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 talb_type;
    /*! ר��buffer */
    uint8 talb_buffer[ArtBufSIZE];
#endif
#if (SUPPORT_PLIST_GENRE == 1)
    /*! �ļ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 genre_type;
    /*! ����buffer */
    uint8 genre_buffer[ArtBufSIZE];
#endif
} music_rcp_plist_info_t;

typedef struct
{
    /*! �������ͣ���μ� music_type_e ���� */
    uint8 music_type;
    /*! �ļ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 file_type;
    /*! �ļ���buffer */
    uint8 file_buffer[64];
    /*! ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 tit2_type;
    /*! ����buffer */
    uint8 tit2_buffer[Id3BufSIZE];
    /*! �ݳ��߱������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 tpe1_type;
    /*! �ݳ���buffer */
    uint8 tpe1_buffer[ArtBufSIZE];
    /*! �ļ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 talb_type;
    /*! ר��buffer */
    uint8 talb_buffer[ArtBufSIZE];
    /*! �ļ����������ͣ�0��ʾANSI��1��ʾUNICODE little endian��2��ʾUNICODE bit endian��3��ʾUTF-8 */
    uint8 genre_type;
    /*! ����buffer */
    uint8 genre_buffer[ArtBufSIZE];
} music_rcp_id3_info_t;

//ÿ��vram������¼��Ŀ¼�����Ϣ
typedef struct _sector_info_t
{
    //��һ��������list_dir_info_t�Ǽ�¼��g_sector_buf��ʲôλ��
    uint8 offset;
    //��һ��������list_dir_info_t�ı��
    uint8 begin; //1��ʼ(���Ϊ0����ʾ��Ч)
    //���һ��������list_dir_info_t�ı��
    uint8 end;
    //����滹��������ֽ�
    uint8 tail;
}
sector_info_t;
extern sector_info_t g_sector_info[DIR_INFO_SECTOR_COUNT];

#define SETTING_ID_MS_LOOP  0x00  //����ѭ��ģʽ

void init_id3_info(void) __FAR__;
void music_rcp_var_init(void) __FAR__;
void music_rcp_var_exit(void) __FAR__;
bool music_rcp_fs_init(uint8 disk_type, uint8 drv_mode) __FAR__;
bool music_rcp_fs_exit(uint8 disk_type, int32 file_sys_id) __FAR__;
app_result_e music_get_global_rcp_info(uint32 data1, uint32 data2) __FAR__;
app_result_e music_get_music_info(uint32 data1, uint32 data2) __FAR__;
app_result_e music_get_music_lyric(uint32 data1, uint32 data2) __FAR__;
app_result_e music_get_music_plist(uint32 data1, uint32 data2) __FAR__;
app_result_e music_set_music_setting(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
app_result_e music_set_song_seq(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
app_result_e music_set_music_plist(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;
app_result_e music_set_music_disk(uint32 data1, uint32 data2, void *data, uint16 data_len) __FAR__;

void music_upload_music_plist(void) __FAR__;


//APK/APP�ϵİ����Ĵ���
app_result_e music_apk_key_play(uint32 data1, uint32 data2) __FAR__;
app_result_e music_apk_key_pause(uint32 data1, uint32 data2) __FAR__;
app_result_e music_apk_key_prev(uint32 data1, uint32 data2) __FAR__;
app_result_e music_apk_key_next(uint32 data1, uint32 data2) __FAR__;

extern uint8 g_id3_title_buf[64];
extern uint8 g_id3_artist_buf[32];
extern uint8 g_id3_album_buf[32];
extern uint8 g_id3_filename_buf[64];
extern pfile_offset_t g_temp_pfile_offset;
extern pdir_layer_t g_temp_pdir_layer;

//�ļ�ϵͳ���
extern int32 eh_vfs_mount;
//�ļ�����
extern uint16 g_vmfile_total;
//Ŀ¼����
extern uint16 g_vmdir_total;

//vram record disk
extern uint16 g_vmdisk_flag;

//�Ѿ����ز����б��ļ���
extern uint16 g_plist_file_count;

//��ǰĿ¼
extern list_dir_info_t g_cur_dir;
extern uint16 g_cur_dir_no;
extern uint8 g_sector_buf[DIR_INFO_SECTOR_SIZE];

//�����˵ڼ�������(0��ʾû�ж���)
extern uint8 g_sector_no;
//�ϴζ�λ����Ŀ¼
extern uint16 g_last_dir_no;
//�ϴζ�λ���ļ������(1��ʼ)
extern uint16 g_last_get_index;

//for id3 get
extern music_id3_info_t g_cur_id3_info;

uint16 music_rcp_first_get(void);
bool music_rcp_fs_exit(uint8 disk_type, int32 file_sys_id);
bool music_rcp_get_id3(music_id3_info_t *rcp_id3_info, uint16 file_index);
bool music_rcp_get_cur_info(music_id3_info_t *rcp_id3_info);
bool music_get_name_info(music_id3_info_t *rcp_id3_info);
bool music_rcp_get_list_id3(music_id3_info_t *rcp_id3_info, uint16 file_index);
HANDLE music_lyric_check(uint32* len_val);
bool music_is_has_lyric(void);

#endif //__APP_MUSIC_RCP_H__
