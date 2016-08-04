/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2010 by Meika Sugimoto
 *
 *  �嵭����Ԥϡ��ʲ���(1)���(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
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
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 */

/*
 *	�������åȥ����ƥ��¸�⥸�塼���V850���ߥ�졼���ѡ�
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H
/*
 *  �������åȥ����ƥ�Υϡ��ɥ������񸻤����
 */
#include "v850es_fk3_emu_env.h"

/*
 *  �ȥ졼�����˴ؤ�������
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  �������åȰ�¸��ʸ�����Ϥ˻��Ѥ���ݡ��Ȥȥܡ��졼�Ȥ����
 */
#define TARGET_PUTC_PORTID	(1)
#define TARGET_PUTC_BAUD	(38400)

#if TARGET_PUTC_PORTID == 1
	#define TARGET_FPUTC_UAnSTR	UA0STR
	#define TARGET_FPUTC_UAnTX	UA0TX
#elif TARGET_PUTC_PORTID == 2
	#define TARGET_FPUTC_UAnSTR	UA1STR
	#define TARGET_FPUTC_UAnTX	UA1TX
#elif TARGET_PUTC_PORTID == 3
	#define TARGET_FPUTC_UAnSTR	UA2STR
	#define TARGET_FPUTC_UAnTX	UA2TX
#else
	#error "TARGET_PUTC_BAUD must set [1 - 3]."
#endif


#if TARGET_PUTC_BAUD == 38400
	#define TARGET_FPUTC_UAnCTL1_SETTING		BAUD_38400_UA0CTL1
	#define TARGET_FPUTC_UAnCTL2_SETTING		BAUD_38400_UA0CTL2
#elif TARGET_TARGET_FPUTC_PUTC_BAUD == 19200
	#define TARGET_FPUTC_UAnCTL1_SETTING		BAUD_19200_UA0CTL1
	#define TARGET_FPUTC_UAnCTL2_SETTING		BAUD_19200_UA0CTL2
#elif TARGET_PUTC_BAUD == 9600
	#define TARGET_FPUTC_UAnCTL1_SETTING		BAUD_9600_UA0CTL1
	#define TARGET_FPUTC_UAnCTL2_SETTING		BAUD_9600_UA0CTL2
#else
	#error "TARGET_PUTC_BAUD must set 38400 , 19200 , or 9600."
#endif


/*
 *  ASP�����ͥ�ư����Υ���ޥåפȴ�Ϣ�������
 */

/*
 *  �ǥե���Ȥ��󥿥�������ƥ������ѤΥ����å��ΰ�����
 */
#define DEFAULT_ISTKSZ    0x2000U

#define DEFAULT_ISTK      (void *)(DEFAULT_STK_TOP - DEFAULT_ISTKSZ)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  �������åȥ����ƥ��¸�ν����
 */
extern void target_initialize(void);

/*
 *  �������åȥ����ƥ�ν�λ
 *
 *  �����ƥ��λ������˻Ȥ���
 */
extern void target_exit(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  �ץ��å���¸�⥸�塼��
 */
#include "v850_gcc/prc_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */
