#ifndef __AP_AUTOTEST_BERTEST_H
#define __AP_AUTOTEST_BERTEST_H

#define BER_HCI_CMD_BIN   "berdata.bin"
#define RX_DEBUG_BYTES   10

//��������ֵ����С����һ��ber������ʱ��
//�����RECV_PKT_REPORT_INTERVALȡ������PKT_INFO_COUNT��ȡ��������
//Ȼ����ݲ����õ������ݾ���ber�Ƿ���ȷ

//С����ȡrx report���ʱ��
#define RECV_PKT_REPORT_INTERVAL    50
//С��ÿ�λ�ȡ���ٸ�report��һ��ber����
#define PKT_INFO_COUNT              10


#define INVALID_BER_VAL             0

//����berƫ�����ֵ��������ֵ�����߱���
#define MAX_BER_DIFF_VAL            (5)

//һ�ν������ٽ�����ȷ��BER��ֵ����
#define MIN_BER_RESULT_NUM          (8)

#define MAX_BER_RETRY_CNT           (1)

//����ber����ʱ������յ���С����
#define MIN_RECV_PKT_NUM            (0x40)

/** BEGIN����ʱ�ϴ���PC���ߵĲ���
*/
typedef struct
{
    uint8 ic_type;
    uint8 channel;    
    uint8 tx_gain_idx;
    uint8 tx_gain_val;        
    uint8 payload;
    uint8 pkt_type;
    uint8 tx_dac;
    uint8 whitening_cv; 
    uint16 pkt_header;
    uint16 tx_pkt_cnt;
    uint32 hit_target_l;
    uint32 hit_target_h;
    uint8 sut_state;            //0:Ĭ��״̬ begin 1:continue 2:stop
    uint8 report_interval;      //��msΪ��λ
    uint8 skip_report_count;    //���Ե�ǰn��report���
    uint8 once_report_pkts;     //����report cfo��Ҫ�İ���
    uint8 report_timeout;       //����rx report�ĳ�ʱʱ�䣬��Ϊ��λ
    uint8 recv_cfo_count;       //���շ��ص�cfo,pwr������stop����
    uint8 reserved[6];
}ber_param_t;



/** PC����ͨ��STOP����صĲ���
*/
typedef struct
{
    uint32 ber;
    uint8 reserve[28];
}ber_retrun_arg_t;


/** DUT��STOP��ʱ�򴫸�PC���߷�����Ľ��
    ����: pkg number,error bit
*/
typedef struct
{
	uint32	 pkts;     //DUTʵ�ʽ��յİ���
	uint32	 errpkts;  //DUTʵ�ʽ��յĴ������������PER
	uint32   bits;	   //DUTʵ�ʽ��յ�bits ��
	uint32	 errbits;  //DUTʵ�ʽ��յĴ���bits ��������BER
	uint32   ber_thr; //BER���ޣ���λ�����֮һ
	uint8    reserve[12];
}ber_update_arg_t;


/** �����ȼ�����յİ���Ϣ
*/
typedef struct
{
    uint32 time;
    uint16 result;
}ber_pktinfo_t;

/** ��������״̬
*/
typedef enum
{
    BER_STATE_STOP = 0,
    BER_STATE_START,
    BER_STATE_FINISH
}ber_test_state_e;

typedef enum
{
    BER_CMD_STATE_WRITE_PACKET_NUM = 0,
    BER_CMD_STATE_READ_PACKET_NUM = 1,
    BER_CMD_STATE_WRITE_ERROR_BIT = 2,
    BER_CMD_STATE_READ_ERROR_BIT = 3,
    BER_CMD_STATE_WRITE_RSSI = 4,
    BER_CMD_STATE_READ_RSSI = 5,
    BER_CMD_STATE_WRITE_CLEAR_REPORT = 6,
    BER_CMD_STATE_DELAY,
}ber_cmd_state_e;


/** ȫ�ֿ��ƽṹ
*/
typedef struct
{
    btt_hci_deal_t g_hci_deal;
    ber_update_arg_t dut2pc_data;  //stopʱ�ϴ�PC����
    ber_retrun_arg_t pc2dut_data;  //stopʱ��PC��ȡ�Ĳ���
    int32 get_pktinfo_timer; //RX ��ض�ʱ��
    uint16 berdata_len; //berdata.bin�����ݳ���
    uint8 rev_pkt_flag; //���յ����ݰ��ı�־λ
    uint8 rev_valid_pkt_flag; //���յ���Ч���ݰ���־λ
    uint8 rx_debug_buf[RX_DEBUG_BYTES]; 
    ber_pktinfo_t pkt_info;//����������Ϣ
    uint8 pkt_info_count;
    uint8 ber_value[PKT_INFO_COUNT];
    uint8 rssi_value[PKT_INFO_COUNT];
    ber_test_state_e ber_test_state;//�����Ȳ���״̬ 1��ʾstop, 2��ʾstart, 3��ʾfinish
    uint8 test_cnt;
    uint8 cmd_state; // 0 ȡpacket num 1 ȡ error bit 2 ȡrssi
    uint16  rssi;
    uint8 tick_cnt;
    uint8 rssi_test; //�Ƿ�ֻ����RSSI����
}ber_control_t;

extern ber_control_t *g_ber_control;

extern void _ber_test_read_hci_data(uint16 read_num, uint8 hci_overflow) __FAR__;
#endif

