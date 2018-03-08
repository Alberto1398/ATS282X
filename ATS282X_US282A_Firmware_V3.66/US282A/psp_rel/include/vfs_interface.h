/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     vfs_interface.h
 * \brief    �����ļ�ϵͳ�ӿ�
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __VFS_INTERFACE_H__
#define __VFS_INTERFACE_H__

#include <storage_device.h>
#include <vfs.h>
#include <attr.h>
#include <typeext.h>

#define FORMAT_FAT32 0
#define FORMAT_FAT16 1
#define FORMAT_EXFAT 2

//����CD����Ĳ�������
#define  CD_UP			'\\'    //�ı䵱ǰĿ¼����һ����Ŀ¼��Ŀ¼��ָ��ָ��Ŀ¼��ʼλ�ã�
#define  CD_BACK		'|'     //�ı䵱ǰĿ¼����һ����Ŀ¼��Ŀ¼��ָ��ָ��֮ǰCD��ȥ����Ŀ¼
#define  CD_ROOT		':'     //�ı䵱ǰĿ¼����Ŀ¼     
#define  CD_SUB			'/'     //�ı䵱ǰĿ¼����ǰĿ¼���Ӧ����Ŀ¼����ʱ����c������

//����DIR����Ĳ�������
#define  DIR_NEXT			0x80    //������
#define  DIR_PREV			0x81    //��ǰ����
#define  DIR_HEAD			0x82    //��Ŀ¼��������
#define  DIR_TAIL			0x83    //��Ŀ¼β��ǰ����

//����DIR ��չ����ӳ��
#define  EXT_BITMAP_BUFF		0x80000000
#define  EXT_NAME_MP3	        (0x80000000>>1)
#define  EXT_NAME_WMA	        (0x80000000>>2)
#define  EXT_NAME_WAV	        (0x80000000>>3)
#define  EXT_NAME_AAC	        (0x80000000>>4)
#define  EXT_NAME_FLAC        	(0x80000000>>5)
#define  EXT_NAME_APE	        (0x80000000>>6)
#define  EXT_NAME_AVI	        (0x80000000>>7)
#define  EXT_NAME_TXT	        (0x80000000>>8)
#define  EXT_NAME_JPG	        (0x80000000>>9)
#define  EXT_NAME_JPEG        	(0x80000000>>10)
#define  EXT_NAME_BMP	        (0x80000000>>11)
#define  EXT_NAME_GIF	        (0x80000000>>12)
#define  EXT_NAME_LRC	        (0x80000000>>13)
#define  EXT_NAME_ASF	        (0x80000000>>14)
#define  EXT_NAME_OGG	        (0x80000000>>15)
#define  EXT_NAME_WMV	        (0x80000000>>16)
#define  EXT_NAME_AMV	        (0x80000000>>17)
#define  EXT_NAME_AAX	        (0x80000000>>18)
#define  EXT_NAME_AA 	        (0x80000000>>19)
#define  EXT_NAME_MP1	        (0x80000000>>20)
#define  EXT_NAME_MP2	        (0x80000000>>21)
#define  EXT_NAME_M4A	        (0x80000000>>22)
#define  EXT_NAME_POS	        (0x80000000>>23)
#define  EXT_NAME_ACT	        (0x80000000>>24)   /* ACT is added by fix FS_ROM rom_ext_name.  2013/3/13 14:42:24. */
#define  EXT_NAME_BMK	        (0x80000000>>25)
#define  EXT_NAME_BIN	        (0x80000000>>26)
#define  EXT_NAME_APK	        (0x80000000>>27)
#define  EXT_NAME_PCM	        (0x80000000>>28)
#define  EXT_NAME_ALL			(0x80000000>>29)
#define  EXT_NAME_ALL_FILE 		(0x80000000>>30)
#define  EXT_NAME_ALL_DIR 		(0x80000000>>31)

//����OPEN�ļ��ķ�ʽ
#define  R_NORMAL_SEEK		0x00    //��ͨseekģʽ
#define  R_FAST_SEEK		0x01    //����seekģʽ(R_FAST_SEEKÿ�δ򿪺���Ҫ��seek���ļ�ĩβ)
#define  OPEN_MODIFY		0x02    //�޸�ģʽ���������Խ�ļ���Сд��
#define  LARGE_FILE_SEEK	0x03    //���ļ�seekģʽ�����ļ��򿪿��ܽ�����
#define  OPEN_RECOVER		0x04    //�ָ�ģʽ������ָ�д��û������close���ļ�,��Ҫ����¼������ָ���


//�����ļ���Ŀ¼����������
#define  FS_DIR				0x00
#define  FS_FILE			0x01


//����ʱ�����������
#define  FS_TIME_CREATE		0X00
#define  FS_TIME_MODIFY		0x01


//�����ļ�SEEK�ķ�ʽ
#define SEEK_SET    0   // ���ļ������ļ�β��λ��offsetΪ����
#define SEEK_CUR    1   //�ӵ�ǰλ�����ļ�ͷ��β��λ��������ʾ���ļ�βseek��������ʾ���ļ�ͷseek
#define SEEK_END    2   // ���ļ�β���ļ��׶�λ��offsetΪ����


//�����ȡ�����ռ������
#define  GET_TOTAL_SPACE			0x00
#define  GET_SPARE_SPACE			0x01

//�����ļ�����
#define         ATTR_NORMAL         0x00    // normal file
#define         ATTR_READONLY       0x01    // file is readonly
#define         ATTR_HIDDEN         0x02    // file is hidden
#define         ATTR_SYSTEM         0x04    // file is a system file
#define         ATTR_VOLUME         0x08    // entry is a volume label
#define         ATTR_LONG_FILENAME  0x0f    // this is a long filename entry
#define         ATTR_DIRECTORY      0x10    // entry is a directory name
#define         ATTR_ARCHIVE        0x20    // file is new or modified

//�������֧��Ŀ¼�㼶
#define  MAX_DIR_LAYER		9

#define  LONG_NAME_TAG		0xfeff

#define  UDISK_VRAM_MAGIC    (0x5aa5)

typedef struct {
	uint16 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 minute;
	uint8 second;
} file_time_t;

//����Ŀ¼�㼶�ṹ���ڴ�ռ�����
typedef struct {
	uint8 buffer[MAX_DIR_LAYER*8+1];
} pdir_layer_t;

typedef struct {
	uint8 buffer[MAX_DIR_LAYER*8+2];
} pdir_layer_ext_t;
typedef struct {
	uint8 buffer[8];
} pfile_offset_t;

typedef struct
{   
    uint16 magic;
    uint16 write_flag;
} udisk_vram_info_t;

//�����ļ�ϵͳ��������
#define  ERR_PHY_R          		0x02   //���̶�����   
#define  ERR_PHY_W          		0x03   //����д����  
#define  ERR_NO_FORMAT          	0x04   //����δ��ʽ��   
#define  ERR_WRITE_PROTECTED    	0x05   //����д����     
#define  ERR_FILE_OVER_OFFSET		0x06   //�ļ����������ļ��߽�   
#define  ERR_DIR_OVER_OFFSET    	0x07   //Ŀ¼��������Ŀ¼�߽�    
#define  ERR_FILE_NO_EXIST    		0x08   //�ļ�������   
#define  ERR_DIR_NO_EXIST    		0x09   //Ŀ¼������       
#define  ERR_FILE_OP_SPACE    		0x10   //��ʾ�ļ�����ʱû�д��̿ռ�,����д����   
#define  ERR_DIR_OP_SPACE    		0x11   //Ŀ¼����ʱû�д��̿ռ�,������չĿ¼,�½���Ŀ¼   
#define  ERR_FILE_OP_ROOT_FULL		0x12   //Ŀ¼������       
#define  ERR_DEL_DIR_NO_EMPTY		0x13   //ɾ��Ŀ¼ʱ����,��ʾɾ����Ŀ¼�ǿ�        
#define  ERR_DIRENTRY_ATTR			0x14
#define  ERR_DIR_TYPE               0x15  
#define  ERR_FHANDLE_FULL			0x16
#define  ERR_DIR_EXIST              0x17   //Ŀ¼�Ѵ���(����Ŀ¼ʱ)
#define  ERR_FILE_EXIST             0x18   //�ļ��Ѵ��ڣ������ļ�ʱ��
#define  ERR_VAILD_PARA				0x19   //�Ƿ��Ĳ���

typedef enum {
    VFS_CD = 0,
    VFS_DIR,
    VFS_GET_NAME,
    VFS_FILE_DIR_OFFSET,
    VFS_MAKE_DIR,
    VFS_FILE_OPEN,
    VFS_FILE_CREATE,
    VFS_FILE_CLOSE,
    VFS_FILE_DIR_REMOVE,
    VFS_FILE_GET_SIZE,
    VFS_GET_TIME,
    VFS_FILE_SEEK,
    VFS_FILE_TELL,
    VFS_FILE_READ,
    VFS_FILE_WRITE,
    VFS_GET_SPACE,
    VFS_GET_ERR_INFO,
    VFS_FILE_DIR_EXIST,
    VFS_FILE_CUT,
    VFS_FILE_ATTRALTER,
    VFS_FILE_RENAME,
    VFS_CREATEVOLUME,
    VFS_SET_TIME,
    VFS_SET_PRIVATE_DATA,
    VFS_CLEAR_PRIVATE_DATA,
    VFS_FILE_MOVE,
    VFS_DIR_FILE,
    VFS_JUMP_DIR,
    VFS_CUT_FILE_TAIL,
    VFS_FILE_DIVIDED,
    VFS_FILE_INSERT   
}vfs_cmd_e;

extern void * vfs_op_entry(void *vfs_mount_index,void *param1, void *param2, void *param3,  vfs_cmd_e cmd) __FAR__;

/*
bool vfs_cd(vfs_mount_t *vfs_mount, uint8 mode, uint8* input_name)
���������������û����뽫��ǰĿ¼ָ��ǰĿ¼����Ŀ¼����Ŀ¼��ֱ�ӷ��ظ�Ŀ¼
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
mode��cd�Ĳ����������ͣ��������������vfs_interface.h ��31���µĶ���
ptr_input_name����Ŀ¼����bufferָ�룻ΪNULL��ʱ��Ĭ��Ϊ���뵱ǰĿ¼��ǰ�����ַ�Ϊ"0xfffe"���ʾͨ�������л�Ŀ¼
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_cd(vfs_mount,mode,input_name)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(uint32)(mode), (void*)(input_name), (void*)(0), VFS_CD)

/*
uint32 vfs_dir(vfs_mount_t *vfs_mount, uint8 mode, uint8* input_name,uint32 ext_name_bitmap)
�����������ڵ�ǰĿ¼�����ַ�ʽ����Ŀ¼���ļ�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
mode: ���������ʽ���������������vfs_interface.h ��37���µĶ���
ptr_input_name��������������ļ�����bufferָ�롣��ָ��ΪNULL����ֱ�Ӵӵ�ǰĿ¼�ʼdir��������
ext_name_bitmap��������Ч�ڴ��ַ�����ʾ������չ���ַ����ĵ�ַ���粻�����ʾҪdir��bitmap���˲��������λΪ1��ʾ������ڴ��ַ����������bitmapֵ
		 �����bitmap��Ӧ�����������vfs_interface.h ��43���µĶ���
����ֵ��0-ʧ�ܣ���0-�ɹ�
*/
#define vfs_dir(vfs_mount,mode,input_name,ext_name_bitmap)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(uint32)(mode), (void*)(input_name), (void*)(ext_name_bitmap), VFS_DIR)

/*
uint16 vfs_get_name(vfs_mount_t *vfs_mount, uint8* output_name, uint16 longname_length)
����������ȡ��ǰ���ļ��������ȷ��س�����û�г����򷵻ض��������׺��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
output_name�������������ļ�����bufferָ��
longname_length������ΪҪ��ȡ�ĳ����ַ���������������Ǻͽ�����������Ϊ0��ʾ��ȡ��׺��
����ֵ��0-ʧ�ܣ���0-ʵ�ʷ��صĻ�ȡ�����ļ����ַ�����Ϊȡ����ʱ����ʵ�ʺ�������ĳ����ַ��ĸ���������������Ǻͽ�����0x0000����Ϊ����ʱ����11����ȡ��׺��ʱ����3
*/
#define vfs_get_name(vfs_mount,output_name,longname_length)  (uint16)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(output_name), (void*)(uint32)(longname_length), (void*)(0), VFS_GET_NAME)

/*
bool vfs_file_dir_offset(vfs_mount_t *vfs_mount, pdir_layer_t* ptr_layer, pfile_offset_t* ptr_file, uint8 mode)
������������ȡ�����õ�ǰĿ¼���λ����Ϣ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_layer: �洢Ҫ��ȡ�����õ�Ŀ¼�㼶��Ϣbufferָ�룻ΪNULL��ʾ��ǰĿ¼�����Ϣ 
ptr_file ���洢Ҫ��ȡ�����õ��ļ�λ����Ϣbufferָ��
mode     ��������ʽ��0-��ʾ��ȡ��1-��ʾ����
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_dir_offset(vfs_mount,ptr_layer,ptr_file,mode)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_layer), (void*)(ptr_file), (void*)(uint32)(mode), VFS_FILE_DIR_OFFSET)

/*
bool vfs_make_dir(vfs_mount_t *vfs_mount, uint8* input_name)
�����������ڵ�ǰĿ¼������һ���û�����ָ��Ŀ¼������Ŀ¼��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
input_name: Ҫ������Ŀ¼��ָ�� 
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_make_dir(vfs_mount,input_name)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(input_name), (void*)(0), (void*)(0), VFS_MAKE_DIR)

/*
uint32 vfs_file_open(vfs_mount_t *vfs_mount, uint8* input_name,uint8 mode)
���������������û�������ļ����ڵ�ǰĿ¼�д�һ���Ѵ��ڵ��ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
input_name: �����ļ����ļ���������ָ�룻Ϊnull��ʾֱ�Ӵ򿪵�ǰĿ¼��ָ����ļ�
mode ��    �򿪷�ʽ���������vfs_interface.h ��77���µĶ��� 
����ֵ��0-ʧ�ܣ���0-�򿪵��ļ����
*/
#define vfs_file_open(vfs_mount,input_name,mode)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(input_name), (void*)(uint32)(mode), (void*)(0), VFS_FILE_OPEN)

/*
uint32 vfs_file_create(vfs_mount_t *vfs_mount, uint8* input_name,uint32 lentgh)
���������������û�������ļ�������һ���ļ�,�û��ɻ�õ�ǰ�����ļ��ľ����
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
input_name: Ϊ��Ҫ�������ļ����ļ����ַ���ָ��
lentgh ��   �������ļ��ĳ��ȣ���λΪbyte,����512byte��512���㣩��Ϊ0��ʾ�������ļ� 
����ֵ��0-ʧ�ܣ���0-�������ļ����
*/
#define vfs_file_create(vfs_mount,input_name,lentgh)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(input_name),(void*)(lentgh), (void*)(0), VFS_FILE_CREATE)

/*
bool vfs_file_close(vfs_mount_t *vfs_mount, uint32 fhandle)
�����������ر��ļ�,�û�������Ҫ�رյ��ļ������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
fhandle: ��Ҫ�رյ��ļ���� 
����ֵ��0-ʧ�ܣ�1-�ɹ�
˵����fhandle��vfs_file_open��vfs_file_create���صľ��
*/
#define vfs_file_close(vfs_mount,fhandle)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(fhandle), (void*)(0), (void*)(0), VFS_FILE_CLOSE)

/*
bool vfs_file_dir_remove(vfs_mount_t *vfs_mount, uint8* input_name, uint8 type)
�����������ڵ�ǰĿ¼��ɾ��һ���û�����ָ��Ŀ¼���ļ���ɾ��Ŀ¼ʱҪ��Ŀ¼Ϊ�ա�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
input_name: Ҫɾ����Ŀ¼���ļ�������Ϊnull��ֱ��ɾ����ǰĿ¼��ָ���Ŀ¼���ļ���
type  ��Ҫ���������ͣ�0-ɾ��Ŀ¼��1-ɾ���ļ�
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_dir_remove(vfs_mount,input_name,type)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(input_name), (void*)(uint32)(type), (void*)(0), VFS_FILE_DIR_REMOVE)


/*
bool vfs_file_get_size(vfs_mount_t *vfs_mount, uint32* output_length, uint8* ptr_file, uint8 mode)
����������ȡ�ļ��ĳ��ȣ��ֽ�Ϊ��λ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
output_length: ���ص��ļ����ȣ����ֽ�Ϊ��λ��
ptr_file�� ����ȡ���ȵ��ļ�������ļ�������ָ��
mode  ����ʾ����ptr_file�����塣0-��ʾptr_fileΪ�ļ������1-��ʾptr_fileΪ�ļ���ָ��;ptr_fileΪnull��ʾ��ǰĿ¼��ָ����ļ�

����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_get_size(vfs_mount,output_length,ptr_file,mode) (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(output_length), (void*)(ptr_file), (void*)(uint32)(mode), VFS_FILE_GET_SIZE)

/*
bool vfs_get_time(vfs_mount_t *vfs_mount, file_time_t* output_time, uint8* input_name, uint8 type)
������������ȡ�ļ��Ĵ���ʱ����޸�ʱ��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
output_time:  ʱ�����bufָ��
ptr_input_name�� ����ȡʱ����ļ����ļ�������ָ�룻Ϊnull��ʾ��ǰĿ¼��ָ����ļ�
type  ��FS_TIME_CREATE-��ȡ����ʱ�䣻FS_TIME_MODIFY-��ȡ�޸�ʱ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_get_time(vfs_mount,output_time,input_name,type)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(output_time), (void*)(input_name), (void*)(uint32)(type), VFS_GET_TIME)

/*
bool vfs_file_seek(vfs_mount_t *vfs_mount, int32 offset, uint8 type, uint32 fhandle)
������������λ�ļ����ֽ�ƫ�ƣ�ʵ�֣�
	a.��������ļ��׵�ƫ������ʵ���û������˳������������
	b.ʵ�ִӵ�ǰλ�����ļ��׻��ļ�βƫ�ƣ�ʵ���û������˳������������
	c.��������ļ��׵�ƫ������ʵ���û�����Ķ�ָ��λ�����ݵ��޸ġ�
	d.���read֧���û�����˳������ݣ���������Լ������λ�����ݣ�������Է���ʵ�ֻ�д�޸��Ѿ����ɵ��ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
offset:  ��ӦSEEKƫ��������Χ��2GB
type�� ��ӦSEEK���ͣ��������vfs_interface.h ��95���µĶ���
fhandle  ���Ѵ򿪵��ļ���� 
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_seek(vfs_mount,offset,type,fhandle)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(offset), (void*)(uint32)(type), (void*)(fhandle), VFS_FILE_SEEK)

/*
bool vfs_file_tell(vfs_mount_t *vfs_mount, uint32* ptr_offset ,uint32 fhandle)
����������ȡ��ǰ��д������ָ�룬ָ����ָ����ļ�ͷ���ֽ�ƫ������������ʱ�û����øú�����¼AB�㣬���seek��readʵ�����ݵ�AB��ȡ��
	  д����ʱ��֧���û������޸������ɵ��ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_offset:  �ļ���ǰ��дλ������ļ�ͷ��ƫ����
fhandle  ���Ѵ򿪵��ļ���� 
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_tell(vfs_mount,ptr_offset,fhandle)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_offset), (void*)(fhandle), (void*)(0), VFS_FILE_TELL)

/*
uint32 vfs_file_read(vfs_mount_t *vfs_mount, uint8* ptr_data_buffer, uint32 byte_count, uint32 fhandle)
�������������ļ��ĵ�ǰλ�ö�ȡһ����С������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_data_buffer:  �������������buffer��ָ�룬buffer�Ĵ�Сһ������С�ں���ָ����byte_count
byte_count�������ֽ���
fhandle  ���Ѵ򿪵��ļ���� 
����ֵ��0-ʧ�ܣ�����0-ʵ�ʶ������ֽ���
*/
#define vfs_file_read(vfs_mount,ptr_data_buffer,byte_count,fhandle)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_data_buffer), (void*)(byte_count), (void*)(fhandle), VFS_FILE_READ)

/*
uint32 vfs_file_write(vfs_mount_t *vfs_mount, uint8* ptr_data_buffer, uint32 byte_count, uint32 fhandle)
�������������ļ��ĵ�ǰλ�ö�ȡһ����С������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_data_buffer:  д����buffer��ָ��
byte_count��д���ֽ���
fhandle  ���Ѵ򿪵��ļ���� 
����ֵ��0-ʧ�ܣ�����0-ʵ��д����ֽ���
*/
#define vfs_file_write(vfs_mount,ptr_data_buffer,byte_count,fhandle)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_data_buffer), (void*)(byte_count), (void*)(fhandle), VFS_FILE_WRITE)

/*
bool vfs_get_space(vfs_mount_t *vfs_mount, uint32* ptr_sector_count, uint8 type)
������������ȡ���̷����ռ䣬�������������ͬѡ��Ҫ�󷵻ش��̷����ܵ�����������ʣ��������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_sector_count:  ���ط�����������
type��0-��ʾ���ý����ر�ʾ��ǰ���̷����ܿռ����������1-��ʾ���ص�ǰ���̷���ʣ��ռ��������
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_get_space(vfs_mount,ptr_sector_count,type)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_sector_count), (void*)(uint32)(type), (void*)(0), VFS_GET_SPACE)

/*
uint8 vfs_get_err_info(vfs_mount_t *vfs_mount)
������������ȡ�ļ�ϵͳ������Ϣ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
����ֵ��0-û�д��󣻷�0-������Ϣ���������vfs_interface.h ��143���µĶ���
*/
#define vfs_get_err_info(vfs_mount)  (uint8)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(0), (void*)(0), (void*)(0), VFS_GET_ERR_INFO)

/*
uint32 vfs_file_dir_exist(vfs_mount_t *vfs_mount, uint8* input_name, uint8 type)
�����������жϵ�ǰĿ¼�Ƿ���ָ������Ŀ¼���ļ�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
input_name:Ҫ�жϵ��ļ���Ŀ¼��ָ��
type    :Ҫ�жϵ����ͣ�0-Ŀ¼��1-�ļ�
����ֵ��0-�����ڣ���0-���ڣ������ظ��ļ���Ŀ¼���ڵ�Ŀ¼���״غ�
*/
#define vfs_file_dir_exist(vfs_mount,input_name,type)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(input_name), (void*)(uint32)(type), (void*)(0), VFS_FILE_DIR_EXIST)

/*
uint32 fat_file_cut(vfs_mount_t *vfs_mount, uint32 reserve1, uint32 reserve2, uint32 reserve3)
�����������ú�����ʱ��������ʵ���κι���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
reserve1:����
reserve2:����
reserve3:����
����ֵ��0-ʧ�ܣ���0-�ɹ�
*/
#define vfs_file_cut(vfs_mount,reserve1,reserve2,reserve3)  (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(reserve1), (void*)(reserve3), (void*)(reserve3), (void*)(0), VFS_FILE_CUT)

/*
uint8 vfs_file_attralter(vfs_mount_t *vfs_mount, uint8 attr, uint8* ptr_file, uint8 mode)
������������ȡ���޸��ļ�������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
attr:0-��ȡ�ļ������ԣ���0-���óɵ��ļ����ԣ��������vfs_interface.h ��105���µĶ���
ptr_file    :����ȡ���޸����Ե��ļ�������ļ�������ָ�룻
mode����ʾ����ptr_file�����塣0-��ʾptr_fileΪ�ļ������1-��ʾptr_fileΪ�ļ���ָ��;ptr_fileΪnull��ʾ��ǰĿ¼��ָ����ļ���Ŀ¼��
����ֵ��0-ʧ�ܣ���0-����Ҫ��ȡ�����õ�����ֵ
*/
#define vfs_file_attralter(vfs_mount,attr,ptr_file,mode)  (uint8)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(uint32)(attr), (void*)(ptr_file), (void*)(uint32)(mode), VFS_FILE_ATTRALTER)

/*
bool vfs_file_rename(vfs_mount_t *vfs_mount, uint8* ptr_new_name, uint8* ptr_file, uint8 mode)
������������ȡ���޸��ļ�������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_new_name:���ļ����ַ���ָ��
ptr_file    :�����������ļ�������ļ���ָ�룻
mode����ʾ����ptr_file�����塣0-��ʾptr_fileΪ�ļ������1-��ʾptr_fileΪ�ļ���ָ��;ptr_fileΪnull��ʾ��ǰĿ¼��ָ����ļ���Ŀ¼
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_rename(vfs_mount,ptr_new_name,ptr_file,mode)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_new_name), (void*)(ptr_file), (void*)(uint32)(mode), VFS_FILE_RENAME)

/*
bool vfs_create_volume(vfs_mount_t *vfs_mount, uint8* input_name)
�����������������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
input_name: ������ַ���ָ��;input_name=NULL��ʾ��ѯ�Ƿ���ھ�꣬ptr_input_name=��NULLֱ���޸ľ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_create_volume(vfs_mount,input_name)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(input_name), (void*)(0), (void*)(0), VFS_CREATEVOLUME)

/*
bool vfs_set_time(vfs_mount_t *vfs_mount, file_time_t* ptr_input_time, uint8* input_name, uint8 type)
�����������������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
ptr_input_time����Ҫ���õ�ʱ������bufָ��
input_name: ������ʱ����ļ����ļ�������ָ�룻Ϊnull��ʾǰĿ¼��ָ����ļ�
type��FS_TIME_CREATE-���ô���ʱ��;FS_TIME_MODIFY-�����޸�ʱ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_set_time(vfs_mount,ptr_input_time,input_name,type)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(ptr_input_time), (void*)(input_name), (void*)(uint32)(type), VFS_SET_TIME)

/*
bool vfs_file_move(vfs_mount_t *vfs_mount, void *fat_direntry, uint8 *ptr_file_name, uint8 mode)
����������ɾ��������Ŀ¼��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
fat_direntry��Ŀ¼����Ϣָ��
ptr_file_name: �����ӻ�ɾ����Ŀ¼���ַ���ָ�룻��mode==0ʱ�����Ϊnull��ʾǰĿ¼��ָ����ļ�;���������Ч
mode��0-ɾ��Ŀ¼�1-����Ŀ¼��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_move(vfs_mount,fat_direntry,ptr_file_name,mode)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(fat_direntry), (void*)(ptr_file_name), (void*)(uint32)(mode), VFS_FILE_MOVE)

/*
uint32 fat_dir_current_entry_file(vfs_mount_t *vfs_mount, uint32 mode, uint8* input_name, uint32 ext_name_bitmap)
����������ͨ����׺������λ���ļ�λ��
����˵����
vfs_mount��
mode��0x80000000-��β��ǰ����������ֵ����ͷ������������Ϊ0��0x80000000ʱ����ʾ������λ����һ�����ú�׺���ļ���λ�á�
      ����ֵ:mode=n���ʾ������λ����N�����ú�׺���ļ���λ�á�
ptr_input_name���洢������λ�����ļ�λ����Ϣ��
ext_name_bitmap: ������Ч�ڴ��ַ�����ʾ������չ���ַ����ĵ�ַ���粻�����ʾҪdir��bitmap���˲��������λΪ1��ʾ������ڴ��ַ����������bitmapֵ
		 �����bitmap��Ӧ�����������vfs_interface.h ��43���µĶ���
����ֵ��0-ʧ�ܣ���0-�ɹ�
*/
#define vfs_dir_current_entry_file(vfs_mount,mode,input_name,ext_name_bitmap) (uint32)vfs_op_entry((void*)(vfs_mount), (void*)(uint32)(mode), (void*)(input_name), (void*)(uint32)(ext_name_bitmap), VFS_DIR_FILE)

/*
bool fat_jump_to_direntry(vfs_mount_t *vfs_mount, uint32 mode, uint8* direntry_info,uint8* reserve)
���������������ṩ��Ŀ¼��Ϣ��ֱ����ת����Ŀ¼����ʼλ��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
mode��0x80000000-��β��ǰ����������ֵ����ͷ���������
direntry_info����Ҫ��ת����Ŀ¼��Ϣ
reserve: ��������ʱ����
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_jump_to_direntry(vfs_mount,mode,direntry_info,reserve) (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(uint32)(mode), (void*)(direntry_info), (void*)(reserve), VFS_JUMP_DIR)


/*
bool vfs_cut_file_tail(vfs_mount_t *vfs_mount, uint32 cut_length, fhandle_t *fhandle)
�����������ر��ļ�֮ǰȥ���ļ���β���Ĳ�������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
cut_length��Ҫ�����ĳ��ȣ����ֽ�Ϊ��λ����ֵ���Ӧ����512��������
fhandle�������ļ��ľ����
����ֵ��0-ʧ�ܣ���0-�ɹ�
˵�����ýӿڱ������ļ��ر�֮ǰ����
*/
#define vfs_cut_file_tail(vfs_mount,cut_length,fhandle)  (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(fhandle), (void*)(fhandle), (void*)(0), VFS_CUT_FILE_TAIL)

/*
bool vfs_file_divided(vfs_mount_t *vfs_mount, uint8* source_file, uint8* new_file, uint32 div_length)
�������������ļ��ָ������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
source_file��Դ�ļ���Ҳ���Ǳ��ָ��ļ����ļ���
new_file���ָ���������ļ����ļ���
div_length����ԭ�ļ��ָ�ĳ��ȣ��磬һ���ļ��ĳ���Ϊ4096 byte��divided_lengthΪ1024����ôԴ�ļ��ᱻ�ָ�Ϊ����Ϊ1024���ļ������µ��ļ��ĳ��Ⱦ���3072byte
����ֵ��0-ʧ�ܣ���0-�ɹ�
*/
#define vfs_file_divided(vfs_mount,source_file,new_file,div_length) (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(source_file), (void*)(new_file), (void*)(uint32)(div_length), VFS_FILE_DIVIDED)

/*
bool vfs_ file_insert(vfs_mount_t *vfs_mount, fhandle_t *fhandle, uint8* insert_file_name)
������������һ���ļ������ݲ��뵽����һ���ļ���Ҳ�����ļ��ϲ�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��
fhandle���Ѵ򿪵�Դ�ļ����
insert_file_name����Ҫ���ϲ����ļ������ϲ�����ļ���Ŀ¼������ݽ���ɾ��
����ֵ��0-ʧ�ܣ���0-�ɹ�
*/
#define vfs_file_insert(vfs_mount,fhandle,insert_file_name) (bool)(uint32)vfs_op_entry((void*)(vfs_mount), (void*)(uint32)(fhandle), (void*)(insert_file_name), (void*)(0), VFS_FILE_INSERT)

#endif



