/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2010 by Meika Sugimoto
 *
 *  Copyright (C) 2016 by Eiwa System Management, Inc.
 *
 *  上記著作権者は，以下の(1)~(4)の条件を満たす場合に限り，本ソフトウェ
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
 *	ターゲットシステム依存モジュール（V850エミュレータ用）
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H
/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "v850es_fk3_emu_env.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  ターゲット依存の文字出力に使用するポートとボーレートの定義
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
 *  ASPカーネル動作時のメモリマップと関連する定義
 */

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#define DEFAULT_ISTKSZ    0x2000U

#define DEFAULT_ISTK      (void *)(DEFAULT_STK_TOP - DEFAULT_ISTKSZ)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void target_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void target_exit(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  プロセッサ依存モジュール
 */
#include "v850_gcc/prc_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */
