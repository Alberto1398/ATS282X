#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#ifndef PC
#define	GET_REAL_ADDR

#define RegisterKeyCallback
#define UnregisterKeyCallback

#else

//#include "os_cpu.h"
/*
 // �Ѿ��Ե�ַת��Ϊ�������ĵ�ַ
 // ����:    addr		С���˾��Ե�ַ
 // ����:	���ػ�������ַ
 */
extern unsigned long GetRealAddrPC(unsigned long addr);

extern int SetOtherKeyCallback(char KeyChar, int(* KeyCallback)(int KeyValue));
extern int ClearOtherKeyCallback(char KeyChar);

#define	GET_REAL_ADDR			  GetRealAddrPC

#define SDKFLAG				2	// ��Ҫ�õ� SDKFLAG ��ģ���������ļ�
// �޸� .\case\inlcude\caseconfig �е� IMAGEDIRNAME ʱ��ͬʱ�޸����� IMAGEDIRNAME �Ķ���
#define CPU400240				1		// 400x240
#define RGB320240				2		// 320x240
#define RGB480272				3		// 480x272
#define CASEFLAG				RGB320240

#define NANDFLASH				1
#define SPINOR					2
#define DISKTYPE				NANDFLASH

#define __FUNCTION__			__FILE__

#define RegisterKeyCallback			SetOtherKeyCallback
#define UnregisterKeyCallback		ClearOtherKeyCallback

#define DLL_IMP __declspec(dllimport)

#endif

#endif

