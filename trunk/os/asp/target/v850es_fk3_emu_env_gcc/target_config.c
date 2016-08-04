/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2010,2013 by Meika Sugimoto
 *
 *  上記著作権者は，以下の (1)~(4) の条件か，Free Software Foundation
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
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
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 *
 */

/*
 *  ターゲット依存モジュール（V850ESエミュレータ用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "v850es_fk3_emu_env.h"

/* バナー出力用のシリアルポート初期化 */
static void target_fput_initialize(void);

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	uint16_t wr_mem_h;
	uint32_t wr_mem_w;

	/*
	 *	プロセッサ依存の初期化
	 */
	prc_initialize();

	/*
	 * バナー出力用のシリアルコントローラの初期化
	 */
	target_fput_initialize();

	/*
	 *	LED接続ポートを点灯
	 */
	clr_bit(LED1_BITPOS , LED1_ADDRESS);
	clr_bit(LED1_BITPOS , PCMT);

	/*
	 * UART0のI/Oポートの設定
	 *
	 * 送信はP30，受信はP31，機能設定が必要(PMC3 , PFC3)
	 */
	wr_mem_h = (sil_reh_mem((void *)PMC3L) & 0xfffc) | 0x003;
	sil_wrh_mem((void *)PMC3L, wr_mem_h);
	wr_mem_w = (sil_reh_mem((void *)PFC3) & 0xfffc);
	sil_wrh_mem((void *)PFC3, wr_mem_w);
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	volatile uint_t i = 1;

	/*
	 *	プロセッサ依存の終了処理
	 */
	prc_terminate();

	/*
	 *	ここには来ない
	 *
	 *	変数iにvolatile修飾するのは、最適化により
	 *	ループが削除されるのを防ぐため。
	 */
	while(i)
		;
}

static void target_fput_initialize(void)
{
	/* 動作禁止 */
	clr_bit(7 , UA0CTL0);

	/* モード，ボーレート設定 */
	sil_wrb_mem((void *)UA0CTL1 , TARGET_FPUTC_UAnCTL1_SETTING);
	sil_wrb_mem((void *)UA0CTL2 , TARGET_FPUTC_UAnCTL2_SETTING);

	/* 送信のみ許可，LSBファースト，ストップビット1bit，8bit長，パリティなし */
	sil_wrb_mem((void *)UA0CTL0 , 0xD2);
}

/*
 *  システムログの低レベル出力のための文字出力
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
