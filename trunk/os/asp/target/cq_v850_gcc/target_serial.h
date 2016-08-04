/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  Copyright (C) 2005 by Freelines CO.,Ltd
 *
 *  Copyright (C) 2010,2013 by Meika Sugimoto
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
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
 * ターゲット依存シリアルI/Oモジュール（V850ES/SG2 内蔵非同期シリアルch0/ch1/ch2用）
 *   ボーレート設定はSERIAL_COMPAREVALUEを変更する。
 *   データフォーマットは8bit data, non-parity, 1stop-bit固定
 */

#ifndef TOPPERS_TARGET_SERIAL_H
#define TOPPERS_TARGET_SERIAL_H

#include "cq_v850.h"

#define SIO_RDY_SND		(1u)
#define SIO_RDY_RCV		(2u)


/*
 *  SIOの割込みハンドラのベクタ番号
 */
#define INHNO_SIO_TX	 50			/* 割込みハンドラ番号 */
#define INTNO_SIO_TX	 50			/* 割込み番号 */
#define INHNO_SIO_RX	 49			/* 割込みハンドラ番号 */
#define INTNO_SIO_RX	 49			/* 割込み番号 */
#define INTPRI_SIO		 (-4)		/* 割込み優先度 */
#define INTATR_SIO		 TA_NULL	/* 割込み属性 */


#ifndef TOPPERS_MACRO_ONLY

/* シリアル管理構造体の前方宣言 */
typedef struct sio_port_control_block SIOPCB;


/*
 *  SIOドライバの初期化
 */
extern void sio_initialize(intptr_t exinf);

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB *sio_opn_por(ID siopid, intptr_t exinf);

/*
 *  シリアルI/Oポートのクローズ
 */
extern void sio_cls_por(SIOPCB *p_siopcb);

/*
 *  SIOの割込みサービスルーチン
 */
extern void sio_tx_isr(intptr_t exinf);
extern void sio_rx_isr(intptr_t exinf);

/*
 *  シリアルI/Oポートへの文字送信
 */
extern bool_t sio_snd_chr(SIOPCB *siopcb, char c);

/*
 *  シリアルI/Oポートからの文字受信
 */
extern int_t sio_rcv_chr(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void sio_irdy_snd(intptr_t exinf);

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void sio_irdy_rcv(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */


#endif /* TOPPERS_TARGET_SERIAL_H */
