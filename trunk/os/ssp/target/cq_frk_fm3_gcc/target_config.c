/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012 Meika Sugimoto
 *  Copyright (C) 2015 by Naoki Saito
 *              Nagoya Municipal Industrial Research Institute, JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
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
 * �������åȰ�¸�⥸�塼��(CQ-FRM-FM3��)
 */
#include "kernel_impl.h"
#include <sil.h>
#include "target_serial.h"
#include "target_syssvc.h"

/*
 * �������åȥ����ƥ�Υϡ��ɥ������񸻤����
 */
#include "cq_frm_fm3.h"


/*
 * �������åȰ�¸�������������
 */
void target_initialize(void)
{
	/*
	 *  �ץ��å���¸���ν����
	 */
	prc_initialize();

	/*
	 *  �ڥ�ե���������
	 */
	
	/*
	 *  �ϡ��ɥ�����WDT̵����
	 */
	sil_wrw_mem((void *)WDG_LCK , WDG_UNLOCK_VAL);
	sil_wrw_mem((void *)WDG_LCK , ~WDG_UNLOCK_VAL);
	sil_wrw_mem((void *)WDG_CTL , (WDG_RESDIS | WDG_RESINTDIS));
	
	/*
	 *  I/O�ݡ��Ȥν����
	 */
#if (SIO_PORTID == 1)
	/* PFR������ */
	
	/* MFS(RX,TX) CH0 */
	sil_andw((void *)EPFR7 , ~(0x03 << 2));	/* SIN0_0 , SOT0_0����� */
	sil_orw((void *)EPFR7 , (0x01 << 6));	/* SIN0_0 , SOT0_0����� */
	
	/* P21(SIN),P22(SOT) */
	sil_orw((void *)PFR2 , (1 << 1) | (1 << 2));
	sil_andw((void *)ADE , ~(1 << 31));

#elif (SIO_PORTID == 2)
	/* USART2(RX,TX) CH2 */
	sil_andw((void *)EPFR8 , ~(0x03 << 6));	/* SIN4_2����� */
	sil_orw((void *)EPFR8 , (0x03 << 6));	/* SIN4_2 , SOT4_2����� */
	
	/* PD2(SIN),PD3(SOT) */
	sil_orw((void *)PFR0 , (1 << 5) | (1 << 6));
	
#endif

	/*
	 *  �Хʡ������ѤΥ��ꥢ������
	 */
	
	target_low_output_init(SIO_PORTID);
	sil_dly_nse(10 * 1000 * 1000);
	
	/* LED�ݡ��� */
	sil_andw((void *)PFRF , ~BITMAP_3);
	sil_orw((void *)PDDRF , BITMAP_3);
	sil_andw((void *)PDORF , ~BITMAP_3);
	
}

/*
 * �������åȰ�¸�� ��λ����
 */
void target_exit(void)
{
	/* �ץ��å���¸���ν�λ���� */
	prc_terminate();
}

/*
 * �����ƥ�������٥���ϤΤ����ʸ������
 */
void target_fput_log(char c)
{
	if (c == '\n') {
		sio_pol_snd_chr('\r', SIO_PORTID);
	}
	sio_pol_snd_chr(c, SIO_PORTID);
}


