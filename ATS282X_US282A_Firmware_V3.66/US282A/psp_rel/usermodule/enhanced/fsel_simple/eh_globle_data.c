#include "enhanced.h"
#include "eh_globle_data.h"
#include "dir_list.h"

//�ļ�����ʲô˳�򲥷�(����ԭʼ�Ĵ���˳�򲥷ţ�
//  ���ǰ���Ŀ��(����vram����)��˳�򲥷�)
uint8 eh_fsel_type = FSEL_TYPE_COMMONDIR;

fsel_param_t fsel_init_val; //ģ�����

fsel_error_e fsel_error; //��������

uint8 eh_dir_layer; //��ǰĿ¼���б�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
uint8 eh_shuffle_sw; //SHUFFLE ����

uint16 eh_file_no; //��ǰ�ļ���Ŀ¼���б��е����
uint16 eh_file_total; //��ǰĿ¼�����ļ�����

uint16 eh_file_no_all; //����˳��ģʽ�£���ʾ������ţ��ڽ�Ŀ��ģʽ�£���ʾ�ڼ�����Ŀ
uint16 eh_file_total_all; //��ǰ�������ļ�����

uint32 eh_cur_ext_name; //��ǰѡ���������չ��
//uint16 eh_root_dir; //��Ŀ¼����Ŀ¼ָ�룬1,2��������

int32 eh_vfs_mount = -1; //vFS ʶ���

uint16 eh_dir_no = 1; //Ŀ¼���(Ŀ¼�б��еĵڼ���)

//���Ŀ¼·����Ϣ
pdir_layer_t pdir_layer_buffer;
//����ļ�λ����Ϣ
pfile_offset_t pfile_offset_buffer;

//for total all save use
//���Ŀ¼·����Ϣ
pdir_layer_t pdir_layer_total;
//����ļ�λ����Ϣ
pfile_offset_t pfile_offset_total;

//for shuffle
uint16 shuffle_total;//shuffle���ܸ���
uint8 group_map[SET_GROUP_MAX / 8];//���飬��64��(bitΪ1-δ��,0-����)
uint8 member_map[SET_MEMBER_MAX / 8];//���ڳ�Ա����256(bitΪ1-δ��,0-����)
uint8 member_avg;//��Աƽ��ֵ����ֵ
uint8 member_avg_group;//ƽ��ֵ��Ա�������ʼ
uint8 group_max;//��Ч�����
uint8 member_max;//��ǰ�������Ա����
uint8 use_member_flag;//ʹ������shuffle����
uint8 cur_group;//��ǰ����ţ�1��2��������
uint16 cur_member;//��ǰ���ڵĳ�Ա�ţ�1��2��������
uint16 filtrate_shuffle_no; //shuffle���˲�����ţ������ظ���ȡֵΪ0,1~n

uint16 play_num_list[SET_PREV_MAX];
uint8 cur_save_pt; //��ǰ���ָ��(0~)
uint8 cur_save_prev_count;
uint8 cur_save_next_count;
uint8 cur_overturn_flag; //��ת���,1-yes,0-no
//end for shuffle

//for dir list

//�б�洢λ��
uint8 *plist_start_addr = (uint8*) PLIST_INFO_ADDR;

//Ŀ¼�б�����Ϣ
list_dir_info_t list_dir_info;
//ָ��
list_dir_info_t* plist_info;
//Ŀ¼�б��ͷ��Ϣ
list_head_info_t list_head_info;
//ָ��
list_head_info_t* plist_head;
//end for list

//record long name
uint8 name_buf[16];

//���ļ��Ĵ����е����(������FAT,���ǵڼ������ŵ��ļ�)
uint16 eh_disk_file_no = 0;

