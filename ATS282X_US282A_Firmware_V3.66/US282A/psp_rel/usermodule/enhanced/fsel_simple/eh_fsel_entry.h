/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������ļ�ѡ������ؽӿ�
 * ���ߣ�Gary Wang
 ********************************************************************************/

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
//
//��ϸ������
//  ��ģ��Ĺ����ǣ��ļ��Ķ�λ��ͳ�ơ�
//  �����ַ�ʽ��
//      ������Ŀ¼�б�
//      ����Ŀ¼�б����������������˵Ľ�Ŀ������
//      ���������Ž�Ŀ������
//

#ifndef _EH_FSEL_ENTRY_
#define _EH_FSEL_ENTRY_

#include "enhanced.h"
#include "eh_globle_data.h"

//�������λ���飬�������λ�����ڵĳ�Ա��������һ�����е���һ��

#define EH_GET_EXT_FILE EXT_NAME_ALL_FILE    //ǰ���ֽ�Ϊ��*f������ʾdir�����ļ�
#define EH_GET_EXT_DIR  EXT_NAME_ALL_DIR     //ǰ���ֽ�Ϊ��*d������ʾdir����Ŀ¼
#define EH_GET_EXT_ALL  EXT_NAME_ALL         //ǰ���ֽ�Ϊ��**������ʾdir����Ŀ¼���ļ�
#define EH_DIR_FLAG   ('d'*0x100+'*')
#define EH_FILE_FLAG  ('f'*0x100+'*')

typedef bool (*fsel_get_next_p)(uint8 *strfile);
typedef bool (*fsel_get_prev_p)(uint8 *strfile);
typedef bool (*fsel_get_byno_p)(uint8 *strfile, uint16 num);
typedef bool (*fsel_set_location_p)(void *location);
typedef bool (*fsel_get_location_p)(void *location);
typedef bool (*fsel_get_nextdir_p)(uint8 *strdir);
typedef bool (*fsel_get_prevdir_p)(uint8 *strdir);
typedef uint16 (*fsel_get_total_p)(void);

//
extern void fsel_init_sysdata(void);

//for shffule
extern bool fsel_init_shuffle(uint16 num_total);

extern uint16 fsel_shuffle_next_file(void);

extern uint16 fsel_shuffle_prev_file(void);
//end for shuufle

//for directory functions
//extern uint16 fsel_total_file_cur(uint8* dir_info, uint8 check_flag);

extern uint16 fsel_get_all_dirs_total_files(void);

extern bool fsel_dir_get_byno(uint8 *strfile, uint16 num);

extern bool fsel_dir_get_byno_all(uint8 *strfile, uint16 num);

extern bool fsel_dir_get_location(file_location_t *location);

extern bool fsel_dir_set_location(file_location_t *location);

extern bool fsel_dir_get_next(uint8 *strfile);

extern bool fsel_dir_next_dir(uint8 *strdir);

extern bool fsel_dir_next_layer(uint8 *strfile);

extern bool fsel_dir_get_prev(uint8 *strfile);

extern bool fsel_dir_prev_dir(uint8 *strdir);

extern bool fsel_dir_prev_layer(uint8 *strfile);

//for dirlist functions

extern bool fsel_list_get_byno_all(uint8 *strfile, uint16 num, uint16 show_num);

extern uint16 fsel_total_file_curdir(void);

extern uint16 fsel_total_file_alldir(void);

extern bool fsel_list_get_location(file_location_t *location);

extern bool fsel_list_set_location(file_location_t *location);

extern bool fsel_list_get_next(uint8 *strfile);

extern bool fsel_list_next_dir(uint8 *strdir);

extern bool fsel_list_get_prev(uint8 *strfile);

extern bool fsel_list_prev_dir(uint8 *strdir);

//for sorted dir list functions
extern uint16 fsel_check_index_num(uint16 cur_num);

extern uint16 fsel_sortlsit_vram_read(uint16 src_num);

extern bool fsel_sortlist_get_byno_all(uint8 *strfile, uint16 num);

extern bool fsel_sortlist_get_location(file_location_t *location);

extern bool fsel_sortlist_set_location(file_location_t *location);

extern bool fsel_sortlist_get_next(uint8 *strfile);

extern bool fsel_sortlist_get_prev(uint8 *strfile);

extern bool fsel_sortlist_next_dir(uint8 *strdir);

extern bool fsel_sortlist_prev_dir(uint8 *strdir);

extern uint16 fsel_total_file_sortlist(void);

bool filtrate_dir(uint8 dir_type);
bool fsel_last_list_entry(void);
bool fsel_first_list_entry(void);

#endif //_EH_FSEL_ENTRY_
