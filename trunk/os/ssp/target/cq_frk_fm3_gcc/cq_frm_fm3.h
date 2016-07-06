/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2012 Meika Sugimoto
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 */

/*
 *  ターゲットボードに関する定義(CQ-FRM-FM3用)
 */

#ifndef TOPPERS_CQ_FRM_FM3_H
#define TOPPERS_CQ_FRM_FM3_H

#include <sil.h>

#include "MB9BF618T.h"

/*
 *  外部オシレータクロックの定義
 */

#define OSC_CLOCK		(4 * 1000 * 1000)	/* 4MHz */

/*
 *  外部オシレータを元に生成するシステムクロック関連の定義
 */

/* PLLクロック */
#define PLLK_VALUE	(1)
#define PLLN_VALUE	(36)
#define PLLM_VALUE	(2)

/* (((OSC_CLOCK / PLLK) * PLLN) * PLLM) / PLLM = OSC_CLOCK / PLLK * PLLN */
#define PLL_CLOCK		(144 * 1000 * 1000)

#define BASE_CLOCK		(PLL_CLOCK)
#define PCLK0_CLOCK		(PLL_CLOCK / 8)	/* 18MHz */
#define PCLK1_CLOCK		(PLL_CLOCK / 8)	/* 18MHz */
#define PCLK2_CLOCK		(PLL_CLOCK / 8)	/* 18MHz */

#define SYSTIC_CLOCK	(PLL_CLOCK)	/* 18MHz */

/* シリアル通信ボーレート設定(バスクロック18MHzの場合の設定) */
#define BGR_VALUE_115200BPS		(155)
#define BGR_VALUE_57600BPS		(311)
#define BGR_VALUE_38400BPS		(467)
#define BGR_VALUE_9600BPS		(1874)


#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CQ_FRM_FM3_H */
