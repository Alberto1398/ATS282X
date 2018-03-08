/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������fm���������ӿ�
 * ���ߣ�Gary Wang
 ********************************************************************************/
#ifndef _FM_DRV_H
#define _FM_DRV_H

#include "psp_includes.h"
#include "fmengine.h"
#include "TWI.h"

//ģ��TWI  ��ʱ����������Ƶ��أ��贫�ε���
#define PARAM_DELAY_TWI   20

/*��������ȫ�ֱ�������*/
extern gpio_init_cfg_t *twi_gpio;
extern uint8 delay_twi;

/*FM �����ڲ���������*/
uint8 TWI_Trans_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
uint8 TWI_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
void TWI_Start(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_Stop(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay);
uint8 TWI_ReadByte(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_Init(gpio_init_cfg_t* gpio, uint8* delay);
void TWI_Exit(gpio_init_cfg_t* gpio);
void TWI_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay);
uint8 TWI_GetAck(gpio_init_cfg_t* gpio, uint8* delay);

/*FM ��������ӿں�������*/
int sFM_Init(radio_band_e band, uint8 level, uint32 freq) __FAR__;
int sFM_Standby(void* null1, void* null2, void* null3) __FAR__;
int sFM_SetFreq(uint32 freq, void* null2, void* null3) __FAR__;
int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3) __FAR__;
int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3) __FAR__;
int sFM_Search(uint32 freq, uint8 direct, void* null3) __FAR__;
int sFM_HardSeek(uint32 freq, uint8 direct, void* null3) __FAR__;
int sFM_SetBand(radio_band_e band, void* null2, void* null3) __FAR__;
int sFM_SetThrod(uint8 level, void* null2, void* null3) __FAR__;
int sFM_BreakSeek(void* null1, void* null2, void* null3) __FAR__;
int sFM_GetHardSeekflag(void* flag, void* null2, void* null3) __FAR__;
int sFM_GetBand(void* band, void* null2, void* null3) __FAR__;
int sFM_GetFreq(void* freq, void* null2, void* null3) __FAR__;
int sFM_GetIntsity(void* intensity, void* null2, void* null3) __FAR__;
int sFM_GetAnten(void* antenna, void* null2, void* null3) __FAR__;
int sFM_GetStereo(void* stereo, void* null2, void* null3) __FAR__;
int sFM_Debug(void* null1, void* null2, void* null3) __FAR__;

#endif /*_FM_DRV_H */

