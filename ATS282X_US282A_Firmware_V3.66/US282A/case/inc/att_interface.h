#ifndef __ATT_INTERFACE_H
#define __ATT_INTERFACE_H

typedef struct
{
    uint8 filename[11];
    uint8 reserved1[5];
    uint32 offset;
    uint32 length;
    uint8 reserved2[8];
} atf_dir_t;

typedef struct
{
    uint8 magic[8];
    uint8 sdk_version[4];
    uint8 reserved[20];
    atf_dir_t atf_dir[15];
} atf_head_t;

typedef struct
{
    /* ��Ϣ�ṹ��С:
     * ���Գ���ɼ��ô�С��ȷ����ǰ�̼��Ͳ��Գ����Ƿ����,
     * �������������Ҫ�����ӿ��������չ̼�;
     */
    uint8  info_size;
    
    /* ���Խ׶�:
     * 0: config Ӧ�ù���ȴ�ֱ�����Խ׶� 1;
     * 1: config Ӧ�ü������в�������������;
     */
    uint8  test_stage;
    
    uint8  base_type;
    uint8  stub_phy_type;
    uint32 ap_file_offset;
    uint32 cfg_file_offset;
    uint32 cfg_file_len;
}test_ap_info_t;

typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint16 offsetl;
    uint16 offseth;
    uint16 lengthl;
    uint16 lengthh;
}att_fread_arg_t;

#endif
