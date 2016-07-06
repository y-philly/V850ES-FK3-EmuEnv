/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011 by Meika Sugimoto
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 * 
 */
 
/*
 *		データキュー機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "dataqueue.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_PSND_DTQ_ENTER
#define LOG_PSND_DTQ_ENTER(dtqid, data)
#endif /* LOG_PSND_DTQ_ENTER */

#ifndef LOG_PSND_DTQ_LEAVE
#define LOG_PSND_DTQ_LEAVE(ercd)
#endif /* LOG_PSND_DTQ_LEAVE */

#ifndef LOG_IPSND_DTQ_ENTER
#define LOG_IPSND_DTQ_ENTER(dtqid, data)
#endif /* LOG_IPSND_DTQ_ENTER */

#ifndef LOG_IPSND_DTQ_LEAVE
#define LOG_IPSND_DTQ_LEAVE(ercd)
#endif /* LOG_IPSND_DTQ_LEAVE */

#ifndef LOG_PRCV_DTQ_ENTER
#define LOG_PRCV_DTQ_ENTER(dtqid, p_data)
#endif /* LOG_PRCV_DTQ_ENTER */

#ifndef LOG_PRCV_DTQ_LEAVE
#define LOG_PRCV_DTQ_LEAVE(ercd, data)
#endif /* LOG_PRCV_DTQ_LEAVE */

/*
 *  データキューへのデータ送信
 */
void
enqueue_data(intptr_t* const data , uint8_t* const tail , const uint8_t size , const intptr_t sdata);

/*
 *  データキューからのデータ受信
 */
void
dequeue_data(intptr_t* const data , uint8_t* const head , const uint8_t size , intptr_t* rdata);


/*
 *  データキューの数
 */
#define tnum_dtq	((uint_t)(tmax_dtqid - TMIN_DTQID + 1))

/*
 *  データキューIDからデータキュー管理ブロックを取り出すためのマクロ
 */
#define INDEX_DTQ(dtqid)	((uint_t)((dtqid) - TMIN_DTQID))

/*
 *  データキュー機能の初期化
 */
#ifdef TOPPERS_dtqini

void
initialize_dataqueue(void)
{
	uint_t	i;

	for (i = 0U ; i < tnum_dtq ; i++) {
		dtqcb_count[i] = 0U;
		dtqcb_head[i] = 0U;
		dtqcb_tail[i] = 0U;
	}
}

#endif /* TOPPERS_dtqini */


/*
 *  データキューが満杯かのチェック
 */

Inline bool_t
data_full(uint8_t count , uint8_t size)
{
	return (count >= size)? true : false;
}

/*
 *  データキューが空かのチェック
 */

Inline bool_t
data_empty(uint8_t count)
{
	return (count == 0)? true : false;
}


/*
 *  データキューへのデータ送信
 */


#ifdef TOPPERS_dtqenq

void
enqueue_data(intptr_t* const data , uint8_t* const tail , const uint8_t size , const intptr_t sdata)
{
	data[*tail] = sdata;
	(*tail)++;
	if (*tail >= size) {
		*tail = 0U;
	}
}

#endif /* TOPPERS_dtqenq */

/*
 *  送信待ちキューの先頭タスクからのデータ受信
 */
#ifdef TOPPERS_dtqdeq

void
dequeue_data(intptr_t* const data , uint8_t* const head , const uint8_t size , intptr_t* rdata)
{
	*rdata = data[*head];
	(*head)++;
	if (*head >= size) {
		*head = 0U;
	}
	
}

#endif /* TOPPERS_dtqdeq */

/*
 *  データキューへの送信（ポーリング）
 */
#ifdef TOPPERS_psnd_dtq

ER
psnd_dtq(ID dtqid, intptr_t data)
{
	ER		ercd;
	int_t	index;
	
	LOG_PSND_DTQ_ENTER(dtqid, data);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	
	t_lock_cpu();
	index = INDEX_DTQ(dtqid);
	
	if (!data_full(dtqcb_count[index] , dtqinib_size[index]))
	{
		enqueue_data(dtqinib_data[index] , &(dtqcb_tail[index]) , 
					dtqinib_size[index] , data);
		dtqcb_count[index]++;
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();
	
  error_exit:
	LOG_PSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_psnd_dtq */

/*
 *  データキューへの送信（ポーリング，非タスクコンテキスト用）
 */
#ifdef TOPPERS_ipsnd_dtq

ER
ipsnd_dtq(ID dtqid, intptr_t data)
{
	ER		ercd;
	int_t	index;

	LOG_IPSND_DTQ_ENTER(dtqid, data);
	CHECK_INTCTX_UNL();
	CHECK_DTQID(dtqid);

	i_lock_cpu();
	index = INDEX_DTQ(dtqid);
	
	if (!data_full(dtqcb_count[index] , dtqinib_size[index]))
	{
		enqueue_data(dtqinib_data[index] , &(dtqcb_tail[index]) , 
					dtqinib_size[index] , data);
		dtqcb_count[index]++;
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	i_unlock_cpu();

  error_exit:
	LOG_IPSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ipsnd_dtq */

/*
 *  データキューからの受信（ポーリング）
 */
#ifdef TOPPERS_prcv_dtq

ER
prcv_dtq(ID dtqid, intptr_t *p_data)
{
	ER		ercd;
	int_t	index;

	LOG_PRCV_DTQ_ENTER(dtqid, p_data);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);

	t_lock_cpu();
	index = INDEX_DTQ(dtqid);
	
	if (!data_empty(dtqcb_count[index]))
	{
		dequeue_data(dtqinib_data[index] , &(dtqcb_head[index]) , 
					dtqinib_size[index] , p_data);
		dtqcb_count[index]--;
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_PRCV_DTQ_LEAVE(ercd, *p_data);
	return(ercd);
}

#endif /* TOPPERS_prcv_dtq */
