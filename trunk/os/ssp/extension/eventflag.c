/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011 by Meika Sugimoto
 *  Copyright (C) 2015 by Naoki Saito
 *             Nagoya Municipal Industrial Research Institute, JAPAN
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
 *  イベントフラグ機能
 */


#include "kernel_impl.h"
#include "check.h"
#include "eventflag.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_SET_FLG_ENTER
#define LOG_SET_FLG_ENTER(flgid, setptn)
#endif /* LOG_SET_FLG_ENTER */

#ifndef LOG_SET_FLG_LEAVE
#define LOG_SET_FLG_LEAVE(ercd)
#endif /* LOG_SET_FLG_LEAVE */

#ifndef LOG_ISET_FLG_ENTER
#define LOG_ISET_FLG_ENTER(flgid, setptn)
#endif /* LOG_ISET_FLG_ENTER */

#ifndef LOG_ISET_FLG_LEAVE
#define LOG_ISET_FLG_LEAVE(ercd)
#endif /* LOG_ISET_FLG_LEAVE */

#ifndef LOG_CLR_FLG_ENTER
#define LOG_CLR_FLG_ENTER(flgid, clrptn)
#endif /* LOG_CLR_FLG_ENTER */

#ifndef LOG_CLR_FLG_LEAVE
#define LOG_CLR_FLG_LEAVE(ercd)
#endif /* LOG_CLR_FLG_LEAVE */

#ifndef LOG_POL_FLG_ENTER
#define LOG_POL_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn)
#endif /* LOG_POL_FLG_ENTER */

#ifndef LOG_POL_FLG_LEAVE
#define LOG_POL_FLG_LEAVE(ercd, flgptn)
#endif /* LOG_POL_FLG_LEAVE */



/* イベントフラグIDIDからの配列オフセットへの変換マクロ */
#define INDEX_FLG(flgid)	((uint_t)((flgid) - TMIN_FLGID))

#ifdef TOPPERS_iniflg

void initialize_eventflag(void)
{
	int_t i;
	
	for(i = 0 ; i < tmax_flgid ; i++)
	{
		/* ビットパターンの初期化 */
		flgcb_flgptn[i] = flginib_iflgptn[i];
	}
}

#endif /* TOPPERS_iniflg */

/*
 *  イベントフラグ条件が成立しているかのチェック
 */
Inline bool_t
check_flg_cond(FLGPTN curptn , FLGPTN waiptn, MODE wfmode)
{
	bool_t flgset = false;
	
	if ((wfmode & TWF_ORW) != 0U)
	{
		if((curptn & waiptn) != 0U)
		{
			flgset = true;
		}
	}
	else	/* (wfmode & TWF_ANDW) != 0U  */
	{
		if((curptn & waiptn) == waiptn)
		{
			flgset = true;
		}
	}
	
	return (flgset);
}

/*
 *  イベントフラグのセット
 */
#ifdef TOPPERS_set_flg

ER
set_flg(ID flgid, FLGPTN setptn)
{
	ER		ercd;
	
	LOG_SET_FLG_ENTER(flgid, setptn);
	CHECK_TSKCTX_UNL();
	CHECK_FLGID(flgid);
	
	t_lock_cpu();
	flgcb_flgptn[INDEX_FLG(flgid)] |= setptn;
	ercd = E_OK;
	t_unlock_cpu();
	
  error_exit:
	LOG_SET_FLG_LEAVE(ercd);
	return(ercd);
}


#endif /* TOPPERS_set_flg */

/*
 *  イベントフラグのセット（非タスクコンテキスト用）
 */
#ifdef TOPPERS_iset_flg

ER
iset_flg(ID flgid, FLGPTN setptn)
{
	ER		ercd;
	
	LOG_ISET_FLG_ENTER(flgid, setptn);
	CHECK_INTCTX_UNL();
	CHECK_FLGID(flgid);
	
	i_lock_cpu();
	flgcb_flgptn[INDEX_FLG(flgid)] |= setptn;
	ercd = E_OK;
	i_unlock_cpu();
	
  error_exit:
	LOG_ISET_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_iset_flg */

/*
 *  イベントフラグのクリア
 */
#ifdef TOPPERS_clr_flg

ER
clr_flg(ID flgid, FLGPTN clrptn)
{
	ER		ercd;
	
	LOG_CLR_FLG_ENTER(flgid, clrptn);
	CHECK_TSKCTX_UNL();
	CHECK_FLGID(flgid);
	
	t_lock_cpu();
	flgcb_flgptn[INDEX_FLG(flgid)] &= clrptn; 
	ercd = E_OK;
	t_unlock_cpu();
	
  error_exit:
	LOG_CLR_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_clr_flg */


/*
 *  イベントフラグ待ち（ポーリング）
 */
#ifdef TOPPERS_pol_flg

ER
pol_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	ER		ercd;
	int_t	index;
	
	LOG_POL_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn);
	CHECK_TSKCTX_UNL();
	CHECK_FLGID(flgid);
	CHECK_PAR(waiptn != 0U);
	CHECK_PAR(wfmode == TWF_ORW || wfmode == TWF_ANDW);
	
	t_lock_cpu();
	
	index = INDEX_FLG(flgid);
	
	if (check_flg_cond(flgcb_flgptn[index], waiptn, wfmode)) {
		ercd = E_OK;
		*p_flgptn = flgcb_flgptn[index];

		if((flginib_atr[index] & TA_CLR) != 0U)
		{
			flgcb_flgptn[index] = 0U;
		}
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_POL_FLG_LEAVE(ercd, *p_flgptn);
	return(ercd);
}

#endif /* TOPPERS_pol_flg */


