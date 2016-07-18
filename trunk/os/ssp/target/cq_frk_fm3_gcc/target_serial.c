/*
 *  TOPPERS/SSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012 by Meika Sugimoto
 *  Copyright (C) 2015 by Naoki Saito
 *              Nagoya Municipal Industrial Research Institute, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 */

/*
 * シリアルドライバ（CQ-FRM-FM3用）
 */

#include <sil.h>
#include "kernel_int.h"
#include "target_serial.h"
#include "target_syssvc.h"

/*
 * レジスタ設定値
 */
#define PORT2SIOPID(x)	((x) + 1)
#define INDEX_PORT(x)	((x) - 1)
#define GET_SIOPCB(x)	(&siopcb_table[INDEX_PORT(x)])

/*
 * USARTレジスタ定義
 *  #define MFS_SMR(ch)				(MFS_BASEADDR((ch)) + 0x00)
 *  #define MFS_SCR(ch)				(MFS_BASEADDR((ch)) + 0x01)
 *  #define MFS_ESCR(ch)			(MFS_BASEADDR((ch)) + 0x04)
 *  #define MFS_SSR(ch)				(MFS_BASEADDR((ch)) + 0x05)
 *  #define MFS_RDRTDR(ch)			(MFS_BASEADDR((ch)) + 0x08)
 *  #define MFS_BGR0(ch)			(MFS_BASEADDR((ch)) + 0x0B)
 *  #define MFS_BGR1(ch)			(MFS_BASEADDR((ch)) + 0x0C)
 *
 */

/*
 *  ビット割り当てはバイトアクセス用に記述している
 */

/* SCR */
#define SCR_UPCL					(1U << 7)
#define SCR_RIE						(1U << 4)
#define SCR_TIE						(1U << 3)
#define SCR_TEIE					(1U << 2)
#define SCR_RXE						(1U << 1)
#define SCR_TXE						(1U << 0)

/* SMR */
#define SMR_MODE_ASYNC_NORMAL		(0U << 5)
#define SMR_MODE_ASYNC_MULTI		(1U << 5)
#define SMR_MODE_CLKSYNC			(2U << 5)
#define SMR_MODE_LIN				(3U << 5)
#define SMR_MODE_I2C				(4U << 5)

#define SMR_WAKEUP					(1U << 4)

#define SMR_SBL_1OR3				(0U << 3)	/* ESCR.ESBL = 0で1 */
#define SMR_SBL_2OR4				(1U << 3)	/* ESCR.ESBL = 0で2 */

#define SMR_BDS_LSBFIRST			(0U << 2)
#define SMR_BDS_MSBFIRST			(1U << 2)

#define SMR_SOE_DISABLE				(0U << 0)
#define SMR_SOE_ENABLE				(1U << 0)

/* SSR */
#define SSR_REC						(1U << 7)
#define SSR_PE						(1U << 5)
#define SSR_FRE						(1U << 4)
#define SSR_ORE						(1U << 3)
#define SSR_RDRF					(1U << 2)
#define SSR_TDRE					(1U << 1)
#define SSR_TBI						(1U << 0)

/* ESCR */
#define ESCR_FLWEN_ENABLE			(1U << 7)
#define ESCR_FLWEN_DISABLE			(0U << 7)
#define ESCR_ESBL					(1U << 6)
#define ESCR_INV_NRZ				(0U << 5)
#define ESCR_INV_INVNRZ				(1U << 5)
#define ESCR_PEN					(1U << 4)
#define ESCR_PEN_DISABLE			(0U << 4)
#define ESCR_PEN_ENABLE				(1U << 4)
#define ESCR_P_EVEN					(0U << 3)
#define ESCR_P_ODD					(1U << 3)
#define ESCR_DATALEN_8				(0U << 0)
#define ESCR_DATALEN_5				(1U << 0)
#define ESCR_DATALEN_6				(2U << 0)
#define ESCR_DATALEN_7				(3U << 0)
#define ESCR_DATALEN_9				(4U << 0)


/*
 * シリアルI/Oポート管理ブロックエリア
 */
SIOPCB siopcb_table[TNUM_PORT];

static const uint32_t sioreg_table[TNUM_PORT] = {
	MFS_BASEADDR(0),
#if (TNUM_PORT >= 2)
	MFS_BASEADDR(4)
#endif
};

Inline bool_t  sio_putready(SIOPCB* siopcb)
{
	return ((sil_reb_mem((void*)MFS_SSR(siopcb->port)) & SSR_TDRE) != 0);
}

Inline bool_t sio_getready(SIOPCB* siopcb)
{
	return ((sil_reb_mem((void*)MFS_SSR(siopcb->port)) & SSR_RDRF) != 0);
}

/*
 *  低レベル出力の初期化
 */

void target_low_output_init(ID siopid)
{
	uint32_t ch = INDEX_PORT(siopid);
	
	/* 初期化済みの場合はすぐにリターン */
	if((sil_reb_mem((void*)MFS_SCR(ch)) & (SCR_TXE | SCR_RXE)) != 0)
	{
		return ;
	}
	
	/* USARTの無効化，リセット */
	sil_wrb_mem((void *)MFS_SCR(ch) , SCR_UPCL);
	sil_wrb_mem((void *)MFS_SCR(ch) , 0x0);
	
	/* 通信モード設定 */
	sil_wrb_mem((void *)MFS_SMR(ch) , 
			(SMR_MODE_ASYNC_NORMAL | SMR_SBL_1OR3 | SMR_BDS_LSBFIRST | SMR_SOE_DISABLE));
	sil_wrb_mem((void *)MFS_ESCR(ch) ,
			(ESCR_FLWEN_DISABLE | ESCR_INV_NRZ | ESCR_PEN_DISABLE | ESCR_DATALEN_8));
	/* 通信速度設定 */
	sil_wrh_mem((void *)MFS_BGR0(ch) , BPS_SETTING);	/* ハーフワードアクセスする */
	/* FIFO無効化 */
	sil_wrb_mem((void *)MFS_FCR0(ch) , 0x00);
	sil_wrb_mem((void *)MFS_FCR1(ch) , 0x00);
	
	/* USARTの有効化 */
	sil_orb((void *)MFS_SMR(ch) , SMR_SOE_ENABLE);
	sil_orb((void*)MFS_SCR(ch) , (SCR_TXE | SCR_RXE));
}

/*
 *  ターゲットのシリアル初期化
 */
void target_usart_init(ID siopid)
{
	target_low_output_init(siopid);
	
	/* 割込み許可 */
	x_clear_int(INTNO_SIO_TX);
	x_clear_int(INTNO_SIO_RX);
	/* 割込み禁止 */
	(void)ena_int(INTNO_SIO_TX);
	(void)ena_int(INTNO_SIO_RX);
}

/*
 *  ターゲットのシリアル終了
 */
void target_usart_term(ID siopid)
{
	/* 割込み禁止 */
	(void)dis_int(INTNO_SIO_TX);
	(void)dis_int(INTNO_SIO_RX);
	
	/* USARTの無効化 */
	sil_andb((void*)MFS_SCR(INDEX_PORT(siopid)) , (SCR_TXE | SCR_RIE));
}

/*
 *  SIO初期化
 */
void sio_initialize(intptr_t exinf)
{
	int i;

	for (i = 0; i < TNUM_PORT; i++) {
		siopcb_table[i].port = i;
		siopcb_table[i].reg = sioreg_table[i];
		siopcb_table[i].exinf = 0;
	}
}

/*
 *  シリアルオープン
 */
SIOPCB *sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB* siopcb;

	if (siopid > TNUM_PORT) {
		return NULL;
	}
	
	siopcb = GET_SIOPCB(siopid);
	siopcb->exinf = exinf;

	target_usart_init(siopid);

	return siopcb;
}

/*
 *  シリアルクローズ
 */
void sio_cls_por(SIOPCB *p_siopcb)
{
	target_usart_term(p_siopcb->port);
}

/*
 *  割込みハンドラ
 */
void sio_isr_tx(intptr_t exinf)
{
	SIOPCB* siopcb = GET_SIOPCB(exinf);

	if (sio_putready(siopcb)) {
		sio_irdy_snd(siopcb->exinf);
	}
}

void sio_isr_rx(intptr_t exinf)
{
	SIOPCB* siopcb = GET_SIOPCB(exinf);

	if (sio_getready(siopcb)) {
		sio_irdy_rcv(siopcb->exinf);
	}
}

/*
 *  1文字送信
 */
bool_t sio_snd_chr(SIOPCB *siopcb, char c)
{
	bool_t ret = false;
	
	if (sio_putready(siopcb)) {
		sil_wrb_mem((void*)MFS_RDRTDR(siopcb->port), c);
		
		ret = true;
	}

	return ret;
}

/*
 *  1文字受信
 */
int_t sio_rcv_chr(SIOPCB *siopcb)
{
	int_t c = -1;

	if (sio_getready(siopcb)) {
		c = (int)sil_reb_mem((void*)MFS_RDRTDR(siopcb->port));
	}

	return c;
}

/*
 *  コールバックの許可
 */
void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	case SIO_RDY_SND:
		sil_orb((void*)MFS_SCR(siopcb->port), SCR_TIE);
		break;
	case SIO_RDY_RCV:
		sil_orb((void*)MFS_SCR(siopcb->port), SCR_RIE);
		break;
	default:
		break;
	}
}

/* 
 *  コールバックの禁止
 */
void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	case SIO_RDY_SND:
		sil_andb((void*)MFS_SCR(siopcb->port), ~SCR_TIE);
		break;
	case SIO_RDY_RCV:
		sil_andb((void*)MFS_SCR(siopcb->port), ~SCR_RIE);
		break;
	default:
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void sio_pol_snd_chr(char c, ID siopid)
{
	sil_wrb_mem((void*)MFS_RDRTDR(INDEX_PORT(siopid)), (int)c);
	
	while ((sil_reb_mem((void*)MFS_SSR(INDEX_PORT(siopid))) & SSR_TDRE) == 0U)
		;
}
