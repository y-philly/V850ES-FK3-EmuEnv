/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
 * 
 *  Copyright (C) 2010 by Meika Sugimoto
 * 
 *  �嵭����Ԥϡ��ʲ��� (1)��(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 */

#ifndef TOPPERS_V850JG2_H
#define TOPPERS_V850JG2_H

#define DEFAULT_STK_TOP		((STK_T *const)0xFFFF0000)
#define STACKTOP			0x03FFEFFF

/* I/O���ɥ쥹(ɬ�פʤ�ΤΤ�) */

/* �ݡ��� */
#define PCT			(0xFFFFF00A)
#define PCMT		(0xFFFFF02A)
#define PMC3L		(0xFFFFF446)
#define PFC3		(0xFFFFF466)

/* UART */
#define UA0CTL0		(0xFFFFFA00)
#define UA0CTL1		(0xFFFFFA01)
#define UA0CTL2		(0xFFFFFA02)
#define UA0STR		(0xFFFFFA04)

#define UA0TX		(0xFFFFFA07)
#define UA1TX		(0xFFFFFA17)
#define UA2TX		(0xFFFFFA27)

#define UA0RX		(0xFFFFFA06)
#define UA1RX		(0xFFFFFA16)
#define UA2RX		(0xFFFFFA26)

#define UA0STR		(0xFFFFFA04)
#define UA1STR		(0xFFFFFA14)
#define UA2STR		(0xFFFFFA24)

/* ����å� */
#define PRCMD		(0xFFFFF1FC)
#define CKC			(0xFFFFF822)
#define LOCKR 		(0xFFFFF824)
#define VSWC		(0xFFFFF06E)
#define PLLCTL		(0xFFFFF82C)

/* �����å��ɥå� */
#define WDTM2		(0xFFFFF6D0)
#define RCM			(0xFFFFF80C)

/* ������ */
#define TM0CMP0		(0xFFFFF694)
#define TM0CTL0		(0xFFFFF690)

/* ����� */
#define INTF0		(0xFFFFFC00)
#define INTR0		(0xFFFFFC20)
#define INTF3		(0xFFFFFC00)
#define INTR3		(0xFFFFFC20)
#define INTF9H		(0xFFFFFC13)
#define INTR9H		(0xFFFFFC33)

#define IMR_SIZE	(4)
#define IMR0		(0xFFFFF100)
#define IMR1		(0xFFFFF102)
#define IMR2		(0xFFFFF104)
#define IMR3		(0xFFFFF106)


/*
 *	���������쥸���������Ϥ򻻽Ф��뤿��Υޥ���
 *
 *	���������쥸�����ϳ�����ֹ����¤�Ǥ��뤿�ᡤ
 *	�١������ɥ쥹����Υ��ե��åȤǥ��ɥ쥹����뤳�Ȥ��Ǥ��롥
 */

#define INTREG_BASE				(0xFFFFF110)
#define INTREG_ADDRESS(intno)	(INTREG_BASE + (((intno) - 8) * 2))

/*
 *	��������߶�������쥸�����ơ��֥�
 */

#define INT_POLREG_TABLE											\
	{ 0 , 0 , 0 } , 			/* �ꥻ�å����ϤʤΤ�̵�� */		\
	{ INTF0 , INTR0 , 2 } ,		/* NMI */							\
	{ 0, 0 , 0 } , 				/* ��������ߤǽФʤ��Τ�̵�� */	\
	{ 0 , 0 , 0 } , 			/* ��������ߤǽФʤ��Τ�̵�� */	\
	{ 0 , 0 , 0 } , 			/* ��������ߤǽФʤ��Τ�̵�� */	\
	{ 0 , 0 , 0 } , 			/* ��������ߤǽФʤ��Τ�̵�� */	\
	{ 0 , 0 , 0 } , 			/* ��������ߤǽФʤ��Τ�̵�� */	\
	{ 0 , 0 , 0 } , 			/* ��������ߤǽФʤ��Τ�̵�� */	\
	{ INTF0 , INTR0 , 3 } , 	/* INT0 */							\
	{ INTF0 , INTR0 , 4 } , 	/* INT1 */							\
	{ INTF0 , INTR0 , 5 } , 	/* INT2 */							\
	{ INTF0 , INTR0 , 6 } ,		/* INT3 */							\
	{ INTF9H , INTR9H , 5 } , 	/* INT4 */							\
	{ INTF9H , INTR9H , 6 } , 	/* INT5 */							\
	{ INTF9H , INTR9H , 7 } , 	/* INT6 */							\
	{ INTF3 , INTR3 , 1 } 		/* INT7 */

#endif	/* TOPPERS_V850JG2_H */
