#ifndef _EH_GLOBLE_DATA_H
#define _EH_GLOBLE_DATA_H

#include "enhanced.h"
#include "dir_list.h"

#define SET_GROUP_MAX   64
#define SET_MEMBER_MAX  256
#define SET_PREV_MAX    16

extern uint8 eh_fsel_type; //ģ������

extern fsel_param_t fsel_init_val; //ģ�����
extern fsel_error_e fsel_error; //��������
extern uint8 eh_dir_layer; //��ǰĿ¼���б�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
extern uint8 eh_shuffle_sw; //SHUFFLE ����

extern uint16 eh_file_no; //��ǰ�ļ���Ŀ¼���б��е����
extern uint16 eh_file_total; //��ǰĿ¼���б�����ļ�����

extern uint16 eh_file_no_all; //��ǰ�ļ��ڴ����е����
extern uint16 eh_file_total_all; //��ǰ�������ļ�����

extern uint32 eh_cur_ext_name; //��ǰѡ���������չ��
//extern uint16 eh_root_dir; //��Ŀ¼����Ŀ¼ָ�룬1,2��������

extern int32 eh_vfs_mount; //FS ʶ���

extern uint16 eh_dir_no; //dir�ĸ���

//���Ŀ¼·����Ϣ
extern pdir_layer_t pdir_layer_buffer;
//����ļ�λ����Ϣ
extern pfile_offset_t pfile_offset_buffer;

//for total all save use
//���Ŀ¼·����Ϣ
extern pdir_layer_t pdir_layer_total;
//����ļ�λ����Ϣ
extern pfile_offset_t pfile_offset_total;

//for shuffle
extern uint16 shuffle_total;//shuffle���ܸ���
extern uint8 group_map[SET_GROUP_MAX / 8];//���飬��64��(bitΪ1-δ��,0-����)
extern uint8 member_map[SET_MEMBER_MAX / 8];//���ڳ�Ա����256(bitΪ1-δ��,0-����)
extern uint8 member_avg;//��Աƽ��ֵ����ֵ
extern uint8 member_avg_group;//ƽ��ֵ��Ա�������ʼ
extern uint8 group_max;//��Ч�����
extern uint8 member_max;//��ǰ�������Ա����
extern uint8 use_member_flag;//ʹ������shuffle����
extern uint8 cur_group;//��ǰ����ţ�1��2��������
extern uint16 cur_member;//��ǰ���ڵĳ�Ա�ţ�1��2��������
extern uint16 filtrate_shuffle_no; //shuffle���˲�����ţ������ظ���ȡֵΪ0,1~n

extern uint16 play_num_list[SET_PREV_MAX];
extern uint8 cur_save_pt; //��ǰ���ָ��(0~)
extern uint8 cur_save_prev_count;
extern uint8 cur_save_next_count;
extern uint8 cur_overturn_flag; //��ת���,1-yes,0-no

//end for shuffle

//for dir list

//�б�洢λ��
extern uint8 *plist_start_addr;
//Ŀ¼�б�����Ϣ
extern list_dir_info_t list_dir_info;
//ָ��
extern list_dir_info_t* plist_info;
//Ŀ¼�б��ͷ��Ϣ
extern list_head_info_t list_head_info;
//ָ��
extern list_head_info_t*plist_head;

//end for dirlist

//record long name
extern uint8 name_buf[16];

//for sort list file num 
extern uint16 eh_disk_file_no;
#endif //#ifndef _EH_GLOBLE_DATA_H
