/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2010,2013 by Meika Sugimoto
 *
 *  �嵭����Ԥϡ��ʲ��� (1)���(4) �ξ�狼��Free Software Foundation
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

/*
 *  �������åȰ�¸�⥸�塼���V850ES���ߥ�졼���ѡ�
 */

#include "kernel_impl.h"
#include <sil.h>
#include "v850es_fk3_emu_env.h"

/* �Хʡ������ѤΥ��ꥢ��ݡ��Ƚ���� */
static void target_fput_initialize(void);

/*
 *  �������åȰ�¸�ν����
 */
void
target_initialize(void)
{
	uint16_t wr_mem_h;
	uint32_t wr_mem_w;

	/*
	 *	�ץ��å���¸�ν����
	 */
	prc_initialize();

	/*
	 * �Хʡ������ѤΥ��ꥢ�륳��ȥ���ν����
	 */
	target_fput_initialize();

	/*
	 *	LED��³�ݡ��Ȥ�����
	 */
	clr_bit(LED1_BITPOS , LED1_ADDRESS);
	clr_bit(LED1_BITPOS , PCMT);

	/*
	 * UART0��I/O�ݡ��Ȥ�����
	 *
	 * ������P30��������P31����ǽ���꤬ɬ��(PMC3 , PFC3)
	 */
	wr_mem_h = (sil_reh_mem((void *)PMC3L) & 0xfffc) | 0x003;
	sil_wrh_mem((void *)PMC3L, wr_mem_h);
	wr_mem_w = (sil_reh_mem((void *)PFC3) & 0xfffc);
	sil_wrh_mem((void *)PFC3, wr_mem_w);
}

/*
 *  �������åȰ�¸�ν�λ����
 */
void
target_exit(void)
{
	volatile uint_t i = 1;

	/*
	 *	�ץ��å���¸�ν�λ����
	 */
	prc_terminate();

	/*
	 *	�����ˤ���ʤ�
	 *
	 *	�ѿ�i��volatile��������Τϡ���Ŭ���ˤ��
	 *	�롼�פ���������Τ��ɤ����ᡣ
	 */
	while(i)
		;
}

static void target_fput_initialize(void)
{
	/* ư��ػ� */
	clr_bit(7 , UA0CTL0);

	/* �⡼�ɡ��ܡ��졼������ */
	sil_wrb_mem((void *)UA0CTL1 , TARGET_FPUTC_UAnCTL1_SETTING);
	sil_wrb_mem((void *)UA0CTL2 , TARGET_FPUTC_UAnCTL2_SETTING);

	/* �����Τߵ��ġ�LSB�ե������ȡ����ȥåץӥå�1bit��8bitĹ���ѥ�ƥ��ʤ� */
	sil_wrb_mem((void *)UA0CTL0 , 0xD2);
}

/*
 *  �����ƥ�������٥���ϤΤ����ʸ������
 */
void
target_fput_log(char c)
{
	if (c == '\n') {
		while((sil_reb_mem((void *)TARGET_FPUTC_UAnSTR) & 0x80) != 0x00)
			;
		sil_wrb_mem((void *)TARGET_FPUTC_UAnTX , '\r');
	}
	while((sil_reb_mem((void *)TARGET_FPUTC_UAnSTR) & 0x80) != 0x00)
		;
	sil_wrb_mem((void *)TARGET_FPUTC_UAnTX , c);
}
