/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 *
 * 
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2010-2012 by Meika Sugimoto
 *  Copyright (C) 2014,2015 by Naoki Saito
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
 *  TOPPERS/SSPのサンプルプログラム
 *
 *  SSPカーネルの基本的な動作を確認するためのサンプルプログラム．
 *
 *  プログラムの概要:
 *
 *  ユーザインタフェースを受け持つメインタスクと，3つの並行実行される
 *  タスクとで構成される．タスクIDおよび優先度設定は以下の通り．
 *     (タスクID)   (起動時優先度)    (実行時優先度)
 *      MAIN_TASK    MAIN_PRIORITY     (左に同じ)
 *      TASK1        TASK1_PRIORITY    (左に同じ)
 *      TASK2        TASK2_PRIORITY    (左に同じ)
 *      TASK3        TASK3_PRIORITY    TASK3_EXEPRIORITY
 *
 *  また，起動周期が2秒の周期ハンドラ（周期ハンドラID: CYC1）および
 *  メインタスク起動用の周期ハンドラ(周期ハンドラID: MAIN_CYC)を用いる．
 *
 *  並行実行されるタスクは，task_loop回空ループを実行する度に，タスクが
 *  実行中であることをあらわすメッセージを表示する．空ループを実行する
 *  のは，空ループなしでメッセージを出力すると，多量のメッセージが出力
 *  され，プログラムの動作が確認しずらくなるためである．また，低速なシ
 *  リアルポートを用いてメッセージを出力する場合に，すべてのメッセージ
 *  が出力できるように，メッセージの量を制限するという理由もある．
 *
 *  周期ハンドラ(CYC1)は，起動したことを示すメッセージを表示する．
 *  プログラムの起動直後は，周期ハンドラ(CYC1)は停止状態になっている．
 *
 *  メインタスクは，周期ハンドラ(MAIN_CYC)から100ミリ秒ごとに起動され，
 *  シリアルI/Oポートからの文字入力を行い，入力された
 *  文字に対応した処理を実行する．入力された文字と処理の関係は次の通り．
 *  'Q'が入力されると，プログラムを終了する．
 *
 *  '1' : 対象タスクをTASK1に切り換える（初期設定）．
 *  '2' : 対象タスクをTASK2に切り換える．
 *  '3' : 対象タスクをTASK3に切り換える．
 *  'a' : 対象タスクをact_tskにより起動する．
 *  'e' : 対象タスクを関数のリターンにより終了させる．
 *  'c' : 周期ハンドラを動作開始させる．
 *  'C' : 周期ハンドラを動作停止させる．
 *  'b' : アラームハンドラを5秒後に起動するよう動作開始させる．
 *  'B' : アラームハンドラを動作停止させる．
 *  'z' : 対象タスクにCPU例外を発生させる（タスクを終了させる）．
 *  's' : イベントフラグ(ID:FLG1)に当該タスクに関連するビットをセットする．
 *  'l' : すべてのタスク関連するイベントフラグ(ID:FLG1)のビットをクリアする．
 *  'p' : ポーリングでイベントフラグ(FLG1)を待つ．
 *  'd' : ポーリングでデータキュー(ID:DTQ1)へデータを送信する．
 *  'r' : ポーリングでデータキュー(ID:DTQ1)からデータを受信する．
 */

#include <kernel.h>
#include <sil.h>
#include <t_syslog.h>
#include "kernel_cfg.h"
#include "syssvc/serial.h"
#include "syssvc/logtask.h"

#include "sample1.h"


/*
 *  システムサービスのエラーハンドリング
 */
#define SVC(expression)											\
	if((expression) < 0)										\
	{															\
		syslog(LOG_NOTICE , "Error at %s : %d caused by %s." ,	\
			__FILE__ , __LINE__ , #expression);					\
	}

/*
 *  並列実行されるタスクへのメッセージ領域
 */
char message[3];

/*
 *  ループ回数
 */
ulong_t	task_loop;		/* タスク内でのループ回数 */

void init_task(intptr_t exinf)
{
#ifndef TASK_LOOP
	volatile ulong_t	i;
	SYSTIM	stime1, stime2;
#endif /* TASK_LOOP */
	
	/* シリアルポートのオープン */
	SVC(serial_opn_por(TASK_PORTID));
	SVC(serial_ctl_por(TASK_PORTID , IOCTL_CRLF));
	
	/* ログタスクの初期化 */
	logtask_initialize(LOGTASK_PORTID);
	
	/* 起動メッセージの出力 */
	syslog(LOG_INFO , "Sample program starts.");
	
	/* 周期ハンドラの起動 */
	SVC(sta_cyc(MAIN_CYC));
	
	/*
 	 *  ループ回数の設定
	 *
	 *  TASK_LOOPがマクロ定義されている場合，測定せずに，TASK_LOOPに定
	 *  義された値を，タスク内でのループ回数とする．
	 *
	 *  MEASURE_TWICEがマクロ定義されている場合，1回目の測定結果を捨て
	 *  て，2回目の測定結果を使う．1回目の測定は長めの時間が出るため．
	 */
#ifdef TASK_LOOP
	task_loop = TASK_LOOP;
#else /* TASK_LOOP */

	task_loop = LOOP_REF;
	SVC(get_tim(&stime1));
	for (i = 0; i < task_loop; i++) {
		/* 時間かせぎのためのループであり，ここでは何もしない */
	}
	SVC(get_tim(&stime2));
	task_loop = LOOP_REF * 400UL / (stime2 - stime1);

#endif /* TASK_LOOP */

}


void main_task(intptr_t exinf)
{
	static ID tskid = TASK1;
	static uint_t tskno = 1;
	char c;
	

	/* シリアルポートからの文字受信 */
	if(serial_rea_dat(TASK_PORTID , &c , 1) > 0)
	{
		switch(c)
		{
		case 'e':
		case 's':
		case 'l':
		case 'p':
		case 'd':
		case 'r':
		case 'z':
			message[tskno-1] = c;
		    break;
		case '1':
			tskid = TASK1;
			tskno = 1;
			break;
		case '2':
			tskid = TASK2;
			tskno = 2;
			break;
		case '3':
			tskid = TASK3;
			tskno = 3;
			break;
		case 'a':
			syslog(LOG_INFO, "#act_tsk(%d)", tskno);
			SVC(act_tsk(tskid));
			break;
		case 'b':
			syslog(LOG_INFO, "#sta_alm(1, 5000)");
			SVC(sta_alm(ALM1 , 5000));
			break;
		case 'B':
			syslog(LOG_INFO, "#stp_alm(1)");
			SVC(stp_alm(ALM1));
			break;
		case 'c':
			syslog(LOG_INFO, "sta_cyc(1)");
			SVC(sta_cyc(CYC1));
			break;
		case 'C':
			syslog(LOG_INFO, "stp_cyc(1)");
			SVC(stp_cyc(CYC1));
			break;
		case 'Q':
			syslog(LOG_NOTICE, "Sample program ends.");
			SVC(ext_ker());
			break;
		default:
			/* エラー表示 */
			syslog(LOG_INFO , "Unknown command.");
			break;
		}
	}
}


void task(intptr_t exinf)
{
	/* exinfはタスク番号  */
	uint_t tskno = (uint_t)exinf;
	int_t		n = 0;
	char command;
	volatile ulong_t i;
	const char	*graph[] = { "|", "  +", "    *" };
	bool_t cont = true;
	FLGPTN pattern;
	const FLGPTN flgptn[] = { 0x00000001U, 0x00000002U, 0x00000004U };
	const FLGPTN allptn = 0x00000007U;
	intptr_t dtqdata;
	
	do
	{
		for (i = 0; i < task_loop; i++) {
			/* 時間かせぎのためのループであり，ここでは何もしない */
		}
		
		/* タスク番号の表示 */
		syslog(LOG_NOTICE, "task%d is running (%03d).   %s",
										tskno, ++n, graph[tskno-1]);
		
		/* コマンド取得，メッセージ領域をクリア */
		command = message[tskno - 1];
		message[tskno - 1] = 0;
		
		switch(command)
		{
		case 'e':
			cont = false;
			syslog(LOG_INFO, "#%d#terminate task", tskno);
			break;
		case 's':
			SVC(set_flg(FLG1 , flgptn[tskno - 1]));
			syslog(LOG_INFO, "#%d#set_flg(flgid=%d , mask=%d)",
									tskno , FLG1 , flgptn[tskno - 1]);
			break;
		case 'l':
			SVC(clr_flg(FLG1 , ~allptn));
			syslog(LOG_INFO, "#%d#clr_flg(flgid=%d)", tskno , FLG1);
			break;
		case 'p':
			SVC(pol_flg(FLG1 , allptn , TWF_ORW , &pattern));
			syslog(LOG_INFO, "#%d#pol_flg(flgid=%d , value=%u)", tskno , FLG1 , pattern);
			break;
		case 'd':
			SVC(psnd_dtq(DTQ1 , (intptr_t)tskno));
			syslog(LOG_INFO, "#%d#snd_dtq(dtqid=%d , value=%u)", tskno , DTQ1 , tskno);
			break;
		case 'r':
			SVC(prcv_dtq(DTQ1 , (intptr_t *)(&dtqdata)));
			syslog(LOG_INFO, "#%d#rcv_dtq(dtqid=%d , value=%u)", tskno , DTQ1 , dtqdata);
			break;
#ifdef CPUEXC1
		case 'z':
			syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
			RAISE_CPU_EXCEPTION;
			break;
#endif /* CPUEXC1 */
		default:
			break;
		}
	}while(cont == true);
}


void alarm_handler(intptr_t exinf)
{
	ID tskid = (ID)exinf;
	
	syslog(LOG_INFO , "Alarm handler is raised.");
	SVC(iact_tsk(tskid));
}

void main_task_cychdr(intptr_t exinf)
{
	ID tskid = (ID)exinf;
	
	(void)iact_tsk(tskid);
}

void cyclic_handler(intptr_t exinf)
{
	syslog(LOG_INFO , "Cyclic handler is raised.");
}

#ifdef CPUEXC1
void exc_handler(void *p_excinf)
{
	syslog(LOG_INFO , "CPU exception handler.");
	syslog(LOG_INFO , "Kernel exit.");
	
	(void)ext_ker();
}
#endif /* CPUEXC1 */

