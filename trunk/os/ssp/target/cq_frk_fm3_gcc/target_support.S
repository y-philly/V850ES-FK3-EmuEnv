/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012 by Meika Sugimoto
 *  Copyright (C) 2015 by Naoki Saito
 *             Nagoya Municipal Industrial Research Institute, JAPAN
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
 * ターゲット依存部　アセンブリ言語（CQ-FRM-FM3用）
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val)			(val)
#define ULONG_C(val)		(val)
#include "kernel_impl.h"
#include "cq_frm_fm3.h"

/*
 * 低レベルのターゲット依存の初期化
 *
 * メモリ初期化の前に呼び出される
 */

	.globl	hardware_init_hook

	.text
	.align 2
	.syntax unified
	.code 16
	.thumb
	.type	hardware_init_hook, function
hardware_init_hook:
	/* 各バスクロックのプリスケーラ設定 */
	ldr	r0 , =#BSC_PSR					/* ベースクロック(1分周) */
	mov	r1 , #BSC_CLK_DIV_1
	str	r1 , [r0]
	
	ldr	r0 , =#APBC0_PSR				/* APB0(8分周) */
	mov	r1 , #APB_CLK_DIV_8
	str	r1 , [r0]
	
	ldr	r0 , =#APBC1_PSR				/* APB1(8分周) */
	mov	r1 , #(APB_CLK_DIV_8 | APBC_EN | APBC_RESET)
	str	r1 , [r0]
	mov	r1 , #(APB_CLK_DIV_8 | APBC_EN)
	str	r1 , [r0]
	
	ldr	r0 , =#APBC2_PSR				/* APB2(8分周) */
	mov	r1 , #(APB_CLK_DIV_8 | APBC_EN | APBC_RESET)
	str	r1 , [r0]
	mov	r1 , #(APB_CLK_DIV_8 | APBC_EN)
	str	r1 , [r0]
	
	ldr	r0 , =#INT_CLR		/* INT_CLRのクリア */
	mov	r1 , #(INT_CLL_MCS | INT_CLL_SCS | INT_CLL_PCS | INT_CLL_FCS)
	str	r1 , [r0]
	
	/* メインクロック発信安定待ち割込みの設定 */
	ldr	r0 , =#CSW_PSR
	mov	r1 , #PSW_TMR_MOWT_CONF12
	str	r1 , [r0]
	
	/* メインクロック入力安定待ち割込み有効化 */
	ldr	r0 , =#INT_ENR
	mov	r1 , #INT_CLL_MCS
	str	r1 , [r0]
	
	/* メインクロック起動 */
	ldr	r0 , =#SCM_CTL
	ldr	r1 , [r0]
	and r1 , #SCM_CTL_MOSCE
	str	r1 , [r0]
	
	/* メインクロック安定待ち */
	ldr	r0 , =#SCM_STR
  wait_main_clock_stable:
	ldr	r1 , [r0]
	orr	r1 , #SCM_STR_MORDY
	beq wait_main_clock_stable
	
	/* PLL入力クロックと発信安定待ち割込みの設定 */
	ldr	r0 , =#PSW_TMR
	mov	r1 , #(PSW_TMR_PINC_MO | PSW_TMR_POWT_CONF7)	/* 待ち時間は最大値取る */
	str	r1 , [r0]
	
	/* PLL入力安定待ち割込み有効化 */
	ldr	r0 , =#INT_ENR
	mov	r1 , #INT_CLL_PCS
	str	r1 , [r0]
	
	/* PLLの分周率設定 */
	ldr	r0 , =#PLL_CTL1		/* PLLK , PLLM */
	mov	r1 , #(PLL_CTL1_PLLK(PLLK_VALUE) | PLL_CTL1_PLLM(PLLM_VALUE))
	str	r1 , [r0]
	ldr	r0 , =#PLL_CTL2		/* PLLN */
	mov	r1 , #PLL_CTL2_PLLN(PLLN_VALUE)
	str	r1 , [r0]
	
	/* PLL起動 */
	ldr	r0 , =#SCM_CTL
	ldr	r1 , [r0]
	orr r1 , #SCM_CTL_PLLE
	str	r1 , [r0]
	
	/* PLL安定待ち */
	ldr	r0 , =#SCM_STR		/* APB2(8分周) */
  wait_pll_stable:
	ldr	r1 , [r0]
	and	r1 , #SCM_STR_PLRDY
	beq wait_pll_stable

	/* マスタクロック切り替え */
	ldr	r0 , =#SCM_CTL
	ldr	r1 , [r0]
	and r1 , #~SCM_CTL_RCS_MASK
	orr r1 , #SCM_CTL_RCS_PLLCLK
	str	r1 , [r0]
	
	isb
	
	/* トレースクロックの設定 */
	ldr	r0 , =#TTC_PSR				/* TTC(8分周) */
	mov	r1 , #(APB_CLK_DIV_8)
	str	r1 , [r0]
	
	/* トレースポートの設定 */
	ldr	r0 , =#PFR0
	ldr	r1 , [r0]
	orr r1 , #(0x01F << 5)
	str	r1 , [r0]
	
	ldr	r0 , =#EPFR0
	ldr	r1 , [r0]
	orr r1 , #(0x03 << 24)
	str	r1 , [r0]
	
	bx	lr

