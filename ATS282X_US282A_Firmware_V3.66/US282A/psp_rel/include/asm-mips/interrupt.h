/********************************************************************************
 *            Copyright(c) 2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������ж���������ʵ��
 * ���ߣ�liminxian
 ********************************************************************************/
 
#ifndef __ASM_INTERRUPT_H__
#define __ASM_INTERRUPT_H__

#include <asm-mips/config.h>
#include <asm-mips/hazards.h>
#ifndef PC
__asm__ (
	"	.macro	local_irq_enable				\n"
	"	.set	push						\n"
	"	.set	reorder						\n"
	"	.set	noat						\n"

	"	ei							\n"

	"	irq_enable_hazard					\n"
	"	.set	pop						\n"
	"	.endm");

static inline void local_irq_enable(void)
{
	__asm__ __volatile__(
		"local_irq_enable"
		: /* no outputs */
		: /* no inputs */
		: "memory");
}

/*
 * For cli() we have to insert nops to make sure that the new value
 * has actually arrived in the status register before the end of this
 * macro.
 * R4000/R4400 need three nops, the R4600 two nops and the R10000 needs
 * no nops at all.
 */
__asm__ (
	"	.macro	local_irq_disable\n"
	"	.set	push						\n"
	"	.set	noat						\n"
	"	di							\n"

	"	irq_disable_hazard					\n"
	"	.set	pop						\n"
	"	.endm							\n");

static inline void local_irq_disable(void)
{
	__asm__ __volatile__(
		"local_irq_disable"
		: /* no outputs */
		: /* no inputs */
		: "memory");
}

__asm__ (
	"	.macro	local_save_flags flags				\n"
	"	.set	push						\n"
	"	.set	reorder						\n"
	"	mfc0	\\flags, $12					\n"
	"	.set	pop						\n"
	"	.endm							\n");

#define local_save_flags(x)						\
__asm__ __volatile__(							\
	"local_save_flags %0"						\
	: "=r" (x))

__asm__ (
	"	.macro	local_irq_save result				\n"
	"	.set	push						\n"
	"	.set	reorder						\n"
	"	.set	noat						\n"
	"	di	\\result					\n"

	"	irq_disable_hazard					\n"
	"	.set	pop						\n"
	"	.endm							\n");

/*!
 * \brief  
 *      �������жϣ������浱ǰ�жϵ�״̬
 * \li  x: �洢��ǰ�ж�״̬�ı��� 
 * \par          exmaple code
 * \code 
 *unsigned int flags;
 *local_irq_save(flags);
 *...
 *local_irq_restore(flags);
 * \endcode	
 */
//#define local_irq_save(x)						
//__asm__ __volatile__(							
//	"local_irq_save\t%0"						
//	: "=r" (x)							
//	: /* no inputs */						
//	: "memory")

/*��Ƕ�����ΪAPI����ʵ��*/


__asm__ (
	"	.macro	local_irq_restore flags				\n"
	"	.set	noreorder					\n"
	"	.set	noat						\n"

	"	mfc0	$1, $12						\n"
	"	ins	$1, \\flags, 0, 1				\n"
	"	mtc0	$1, $12						\n"

	"	irq_disable_hazard					\n"
	"	.set	at						\n"
	"	.set	reorder						\n"
	"	.endm							\n");

/*!
 * \brief  
 *      �ָ�local_irq_save()�����״̬
 * \li  flags: local_irq_save()�����״ֵ̬
 * \par          exmaple code
 * \code 
 *unsigned int flags;
 *local_irq_save(flags);
 *...
 *local_irq_restore(flags);
 * \endcode
 */
//#define local_irq_restore(flags)					
//do {									
//	unsigned long __tmp1;						
//									
//	__asm__ __volatile__(						
//		"local_irq_restore\t%0"					
//		: "=r" (__tmp1)						
//		: "0" (flags)						
//		: "memory");						
//} while(0)
#endif /*PC*/
extern unsigned int local_irq_save(void *null0, void* null1, void* null2);
extern unsigned int local_irq_save_fix(void *null0, void* null1, void* null2);
extern void local_irq_restore(unsigned int flags, void* null1, void* null2);

#define irqs_disabled()							\
({									\
	unsigned long flags;						\
	local_save_flags(flags);					\
	!(flags & 1);							\
})

#endif/*__ASM_INTERRUPT_H__*/
