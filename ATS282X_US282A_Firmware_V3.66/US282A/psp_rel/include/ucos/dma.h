/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������dma����ͷ�ļ�
 * ���ߣ�liminxian
 ********************************************************************************/

#ifndef __SYS_DMA_H__
#define __SYS_DMA_H__

#include <ucos/irq.h>
#include <asm-mips/exception.h>

#ifndef _ASSEMBLER_
/*!
 *  DMA���� \n
 *  \li DMA_CHAN_TYPE_SPECIAL : רҵͨ��DMA
 *  \li DMA_CHAN_TYPE_BUS: ����ͨ��DMA
 *
 */
enum {
	DMA_CHAN_TYPE_SPECIAL, DMA_CHAN_TYPE_BUS
};

struct dma_chan {
	unsigned int io;
	int irq;
};

#include <asm-mips/mach-gl5116/dma.h>

#endif /*_ASSEMBLER_*/
#endif /* __DMA_H__ */
