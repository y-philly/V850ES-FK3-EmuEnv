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
 * ターゲット依存モジュール(CQ-FRM-FM3用)
 */
#include "kernel_impl.h"
#include <sil.h>
#include "target_serial.h"
#include "target_syssvc.h"

/*
 * ターゲットシステムのハードウェア資源の定義
 */
#include "cq_frm_fm3.h"


/*
 * ターゲット依存部　初期化処理
 */
void target_initialize(void)
{
	/*
	 *  プロセッサ依存部の初期化
	 */
	prc_initialize();

	/*
	 *  ペリフェラルの設定
	 */
	
	/*
	 *  ハードウェアWDT無効化
	 */
	sil_wrw_mem((void *)WDG_LCK , WDG_UNLOCK_VAL);
	sil_wrw_mem((void *)WDG_LCK , ~WDG_UNLOCK_VAL);
	sil_wrw_mem((void *)WDG_CTL , (WDG_RESDIS | WDG_RESINTDIS));
	
	/*
	 *  I/Oポートの初期化
	 */
#if (SIO_PORTID == 1)
	/* PFRの設定 */
	
	/* MFS(RX,TX) CH0 */
	sil_andw((void *)EPFR7 , ~(0x03 << 2));	/* SIN0_0 , SOT0_0を使用 */
	sil_orw((void *)EPFR7 , (0x01 << 6));	/* SIN0_0 , SOT0_0を使用 */
	
	/* P21(SIN),P22(SOT) */
	sil_orw((void *)PFR2 , (1 << 1) | (1 << 2));
	sil_andw((void *)ADE , ~(1 << 31));

#elif (SIO_PORTID == 2)
	/* USART2(RX,TX) CH2 */
	sil_andw((void *)EPFR8 , ~(0x03 << 6));	/* SIN4_2を使用 */
	sil_orw((void *)EPFR8 , (0x03 << 6));	/* SIN4_2 , SOT4_2を使用 */
	
	/* PD2(SIN),PD3(SOT) */
	sil_orw((void *)PFR0 , (1 << 5) | (1 << 6));
	
#endif

	/*
	 *  バナー出力用のシリアル初期化
	 */
	
	target_low_output_init(SIO_PORTID);
	sil_dly_nse(10 * 1000 * 1000);
	
	/* LEDポート */
	sil_andw((void *)PFRF , ~BITMAP_3);
	sil_orw((void *)PDDRF , BITMAP_3);
	sil_andw((void *)PDORF , ~BITMAP_3);
	
}

/*
 * ターゲット依存部 終了処理
 */
void target_exit(void)
{
	/* プロセッサ依存部の終了処理 */
	prc_terminate();
}

/*
 * システムログの低レベル出力のための文字出力
 */
void target_fput_log(char c)
{
	if (c == '\n') {
		sio_pol_snd_chr('\r', SIO_PORTID);
	}
	sio_pol_snd_chr(c, SIO_PORTID);
}


