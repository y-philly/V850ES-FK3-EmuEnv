#include "types.h"
#include "prc_sil.h"
#include "v850esfk3.h"

/* タイマー初期化 */
void timer_init(void)
{
	/* クロック設定 */
	sil_wrb_mem((void*)TAAnCTL0(2),0x00);

	/*
	 * クロック選択
	 * タイマ・モードの選択
	 */
	sil_wrb_mem((void*)TAAnCTL1(2),0x00);

	/*
	 * コンペア値の設定
	 * 1msec : 20000clock 0x4E20
	 */
	sil_wrh_mem((void*)TAAnCCR0(2), 0x4E20);

	/* 割り込みのマスク設定
	 * 割込みを許可する
	 * 6bit:   0：許可
	 * 0-2bit: 7:最低優先度
	 */
	sil_wrb_mem((void*)0xFFFFF13C,0x07);


	return;
}

/* タイマースタート */
void timer_start(void)
{	
	/* 動作許可
	 * (※クロック設定(下位3ビット)も上書きされるので同じ値になるよう注意) 
	 */
	sil_wrb_mem((void*)TAAnCTL0(2),0x80);
	return;
}

/* タイマーストップ */
void tiemr_stop(void)
{
	/* 動作禁止
	 * (※クロック設定(下位3ビット)も上書きされるので同じ値になるよう注意) 
	 */
	sil_wrb_mem((void*)TAAnCTL0(2),0x00);
	return;
}

/* コンペア値の設定 */
void timer_setcmp(uint16 cmp)
{
	/*
	 * 引数(clock数)でコンペア値の設定
	 */
	sil_wrh_mem((void*)TAAnCCR0(2), cmp);
	
	return;
}

/* カウント取得 */
void timer_getcount(uint16 *count)
{
	*count = sil_reh_mem((void*)TAAnCNT(2));
	return;	
}

