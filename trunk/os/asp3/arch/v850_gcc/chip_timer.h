/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  $Id:  $
 */

/*
 *		タイマドライバ（V850ESFK3 OSタイマ用）
 *
 */

#ifndef TOPPERS_CHIP_TIMER_H
#define TOPPERS_CHIP_TIMER_H

#include <sil.h>

/*
 * (5-3-1) TCYC_HRTCNT		高分解能タイマのタイマ周期
 *
 * 高分解能タイマのタイマ周期を，このマクロに定義する．タイマ周期が2^32の
 * 場合には，このマクロを定義しない．
 */
#define TCYC_HRTCNT		65535U

/*
 * (5-3-2) TSTEP_HRTCNT	高分解能タイマのカウント値の進み幅
 *
 * 高分解能タイマのカウント値の進み幅を，このマクロに定義する．例えば，高
 * 分解能タイマが，2マイクロ秒毎に2ずつカウントアップする場合には，
 * TSTEP_HRTCNTを2に定義する．
 */
#define TSTEP_HRTCNT			1U		/* 1MHz */

/*
 * タイマ割込みの遅延時間よりも長い値
 *
 * CPU周波数を50MHzとする
 *
 * 以下，athrill での実測結果
 *
 * clock = cpu 2872400 intc 440
 * clock = cpu 2872450 intc 490
 *
 * 50clock
 *
 * 10clockはバッファ時間とする
 *
 */
#define TCYC_INT_DELAY_CNT	(50 + 10)U

/*
 * (6-13-2-6) HRTCNT_BOUND
 *
 * 高分解能タイマに設定するカウントアップ値の上限値（割込みタイミングに指
 * 定する最大値）を，このマクロに定義する．HRTCNT_BOUNDは，高分解能タイマ
 * のタイマ周期（TCYC_HRTCNT）からタイマ割込みの遅延時間よりも長い値を減
 * じた値に定義する．カーネルの実装上の都合により，HRTCNT_BOUNDは，
 * 4,294,000,000未満の値に設定しなければならない．
 *
 * HRTCNT_BOUNDを設けている理由は，高分解能タイマをタイマ周期のぎりぎりで
 * 割込みがかかるように設定した場合，割込みハンドラ内で高分解能タイマを読
 * んだ時点でタイマ周期を越えており，高分解能タイマが周回したことを見落と
 * す可能性があるためである．
 */
#define HRTCNT_BOUND	(TCYC_HRTCNT - TCYC_INT_DELAY_CNT)

/*
 *  高分解能タイマの起動処理
 */
extern void	target_hrt_initialize(intptr_t exinf);

/*
 *  高分解能タイマの停止処理
 */
extern void	target_hrt_terminate(intptr_t exinf);

/*
 *  高分解能タイマの現在のカウント値の読出し
 */
Inline HRTCNT
target_hrt_get_current(void)
{
	//TODO
	return 0;
}

/*
 *  高分解能タイマへの割込みタイミングの設定
 *
 *  高分解能タイマを，hrtcntで指定した値カウントアップしたら割込みを発
 *  生させるように設定する．
 */
Inline void
target_hrt_set_event(HRTCNT hrtcnt)
{
	//TODO
}

/*
 *  高分解能タイマ割込みの要求
 *
 *  暫定的に，1μ秒後に高分解能タイマ割込みがかかるように設定しているが，
 *  即座に割込みを要求する方法がわかれば（Set-pending Registerが使える
 *  と思われる），それに差し換える．
 */
Inline void target_hrt_raise_event(void)
{
	//TODO
}

#endif /* TOPPERS_CHIP_TIMER_H */
