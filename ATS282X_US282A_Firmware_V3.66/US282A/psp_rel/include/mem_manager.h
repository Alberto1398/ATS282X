#ifndef __MEM_MANAGER_H
#define __MEM_MANAGER_H

//�Ƿ��ػ�����ʱ�ڴ�����ݱ�����
#define MPU_MONITOR_MEM_POOL

//�Ƿ��ػ���bank�Ƿ����ݱ�����
#define BANK_DATA_MONITOR 

//0x29000-0x34000
#define DSP_CACHE_MEM_ADDR              (0x29000)
#define DSP_CACHE_MEM_SIZE              (44 * 1024)

//0x29000-0x30000
#define DSP_CACHE_MEM_ADDR_L            (0x29000)       //usoundʹ�ã���ʹ��0x33000-0x34000�Ŀռ�
#define DSP_CACHE_MEM_SIZE_L            (28 * 1024)

//0x28000-0x34000
#define DSP_CACHE_MEM_ADDR2             (0x28000)
#define DSP_CACHE_MEM_SIZE2             (48 * 1024)

//0x34800-0x3c000
#define DSP_CACHE_MEM_ADDR_H            (0x34800)
#define DSP_CACHE_MEM_SIZE_H            (30 * 1024)

//0x34800-0x38000
#define DSP_CACHE_MEM_ADDR_H2           (0x34800)
#define DSP_CACHE_MEM_SIZE_H2           (14 * 1024)

#define URAM1_CACHE_MEM_ADDR            (0x3f800)
#define URAM1_CACHE_MEM_SIZE            (0x800)

#define PCMRAM1_CACHE_MEM_ADDR          (0x3e000)
#define PCMRAM1_CACHE_MEM_SIZE          (0x800)

#define CARD_FIFO_MEM_ADDR              (0xfc00)
#define CARD_FIFO_MEM_SIZE              (0x400)

#define CARD_FS_CACHE_MEM_ADDR          (0x16c00)
#define CARD_FS_CACHE_MEM_SIZE          (0x2980)

#define SBC_INPUT_BUFFER_ADDR           (0x16400)
#define SBC_INPUT_BUFFER_SIZE           (0x2980 + 0x800)

#define KEYTONE_CACHE_URAM0_ADDR0       (0x3f000)
#define KEYTONE_CACHE_URAM0_SIZE        (0x800)

#define KEYTONE_CACHE_PCMRAM2_ADDR0     (0x3e000)
#define KEYTONE_CACHE_PCMRAM2_SIZE      (0x1000)

#define RADIO_FS_CACHE_MEM_ADDR         (0x17400)
#define RADIO_FS_CACHE_MEM_SIZE         (0x2180)

#define WAV_DECODE_CACHE_ADDR           (0x2c000)
#define WAV_DECODE_CACHE_SIZE           (32 * 1024)

#define WAV_DECODE_CACHE_ADDR2          (0x2c000)       //usoundʹ�ã���ʹ��0x33000-0x34000�Ŀռ�
#define WAV_DECODE_CACHE_SIZE2          (16 * 1024)

#define TTS_MEM_CACHE_ADDR              (0x38000)
#define TTS_MEM_CACHE_SIZE              (14 * 1024)

#define SEND_BUFFER_ADDR                (0x30800)
#define SEND_BUFFER_SIZE                (14 * 1024)

#define USOUND_DATA_BUFFER_ADDR         (0x30000)
#define USOUND_DATA_BUFFER_SIZE         (0x4000)

#define USPEAKER_DATA_BUFFER_ADDR       (0x30000)
#define USPEAKER_DATA_BUFFER_SIZE       (0x3000)

#define USPEAKER24_DATA_BUFFER_ADDR     (0x28000)
#define USPEAKER24_DATA_BUFFER_SIZE     (0x4000)

#define SBC_INPUT_BUFFER_LOW_ADDR       (0x16400)
#define SBC_INPUT_BUFFER_LOW_SIZE       (0x800)

/* 18λ�ڴ��ַ */
#define __addr18(_a32)  \
    \
    ((uint32)(_a32) & 0x3ffff)

/* 32λת16λ�ڴ��ַ */
#define __addr16(_a32)  \
    \
    ((uint16)((uint32)(_a32) >> 2))

/* 16λת32λ�ڴ��ַ */
#define __addr32(_a16)  \
    \
    (((uint32)(_a16) << 2))

#define MEM_MANAGER_DEBUG

typedef struct 
{
	uint32 addr;
	uint32 size;
	uint8 *mem_map;
	uint8 *mem_map2;
	uint8 block_size_mask;
	uint8 data_feature;
	uint8 insert_mode;
	uint8 mem_global;
}mem_pool_param_t;

//��֤�����ݽṹ��16�ֽ�
typedef struct
{
	uint16 addr; 				//��ʼ��ַ
	uint16 next;	            //ָ����һ���ڴ��������
	uint8 *mem_map;
	uint8 *mem_map2;	
	uint32 total_index:7; 		//�����index����,��256Ϊһ���飬���ӳ��256k����16Ϊһ���飬���ӳ��16k
	uint32 blk_size:4;			//������С
	uint32 data_feature:6; 		//�Ƿ�ɷ�������, ���ݶΣ��������ݶ�,bank���ݶ�, ap���ݶε�
	uint32 mem_last:11;			//����index��Ŀ,2^7*8=1024 
	uint32 pool_lock : 1;       //�ڴ���Ƿ�����������ʹ��
	uint32 mem_global : 1;      //�ڴ���Ƿ���ȫ���ڴ��
	uint32 mpu_enable : 1;
	uint32 reserved:1;
}mem_pool_info_t;

//��֤�ýṹ��16�ֽڶ���
typedef struct bank_info_stu
{
    uint16 vaddr;           //bank�����ַ
    uint16 size;            //bank��С
    uint16 pmaddr;          //bank�����ַ
    uint16 run_addr;        //bankʵ�����е�ַ
    uint16 switch_count;    //�л�����ͳ��
    uint16 pool_addr;       //ָ��mem_pool_info_t�ṹ��
    uint16 next;            //��һ��bank_info    
    uint16 data_tag;        //bank���ݱ�ǣ�ȷ��bank�����Ƿ��ƻ�
}bank_info_t;


typedef struct
{
    uint16 use_bt_data;        //bt_dataʹ����
    uint16 free_bt_data;       //bt���ݿ�����
    uint16 free_bt_data_peak;  //bt����ʣ���ֵ
    uint16 bt_data_total;        
    uint16 use_global_data;    //С����ʹ���� MEM_DATA, MEM_BANK_INFO
    uint16 global_data_total;  //С����������������BT DATA
#ifdef MEM_MANAGER_DEBUG    
    mem_pool_info_t **mem_pool;
    bank_info_t **bank_info;
#endif    
}mem_use_info_t;

typedef enum
{
    //ϵͳʹ�ã����ڻ������ 
    MEM_TEXT = (1 << 0), 
    //BANK DATA,��ʱʹ��,������һ������ͻ��ͷ�
    MEM_BANK_DATA = (1 << 1),
    //��ͨҪʹ�õ�ȫ������
    MEM_DATA = (1 << 2),  
    //�������������
    MEM_BT_DATA = (1 << 3), 
    //ϵͳʹ�ã������ݿռ䲻��ʱ�����ͷ��ⲿ�ֿռ�
    MEM_BANK_INFO = (1 << 4), 
    //��ͨҪʹ�õĴ����ݣ�����512�ֽڣ���಻����ȫ��cache��С
    MEM_LDATA = (1 << 5),     
}mem_type_e;

typedef struct
{
    //�Ƿ�ʹ�þֲ��ڴ��
    uint8 use_temp_pool:1;
    //�Ƿ�ʹ��send buffer
    uint8 use_send_buffer:1;
    //�Ƿ���tws linein����
    uint8 mem_scene:6;
}mem_use_scene_info_t;

typedef enum
{
    MEM_USE_NORMAL = 0,
    MEM_USE_TWS_LINEIN,
    MEM_USE_USOUND,
    MEM_USE_MUSICPLAY,
}mem_use_info_e;


#define MEM_IN_BANK 0

#define INSERT_MEM_POOL_TAIL  0

#define INSERT_MEM_POOL_HEAD  1

#define CLR_MEM_LOCK          0

#define SET_MEM_LOCK          1

#define MAX_MEM_LOCK_ITEM     1


typedef struct
{
    uint16 mem_addr;
    uint16 mem_size;   
}mem_lock_info_t;

typedef enum
{
    //���mem pool��������
    MEM_POOL_OP_CLEAR = 0,
    //����mem pool
    MEM_POOL_OP_DESTROY = 1,
    //ǿ������mem pool,��ʹ������Ȼ������
    MEM_POOL_OP_FORCE_DESTROY = 2,
    //ֻ���ͷ��ڴ�أ����ڲ������Ժ��ͷ�
    MEM_POOL_OP_ARRANGE
}mem_pool_ops_e;


extern mem_pool_info_t *g_p_mem_pool_info;

extern uint32 mem_pool_create(mem_pool_param_t *mem_pool_param) __FAR__;

extern uint32 mem_pool_destroy(uint32 addr, uint32 size, uint32 mode);
#endif
