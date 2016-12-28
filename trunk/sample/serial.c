#include "types.h"
#include "prc_sil.h"
#include "v850esfk3.h"

void serial_init(void)
{

	/*
	 * 通信設定
	 *
	 *　UARTD0制御レジスタ0（UD0CTL0）
	 *　UARTD0動作許可，送受信禁止，転送方向：LSB，パリティ：なし，データ：8ビット，ストップビット：1ビット
	 *
	 */
	sil_wrb_mem((void*)UDnCTL0(UDnCH0),0x92);

	/*
	 * ボーレート設定
	 *
	 * UARTD0制御レジスタ1(UD0CTL1)
	 *  UARTD0クロック：fxx/2 (PRSI =0)
	 *
	 * UARTD0制御レジスタ2(UD0CTL2)
	 *  規定値：130(0x82) 、シリアルクロック：fuclk/130
	 */
	sil_wrb_mem((void *) UDnCTL1(UDnCH0), 0x01);
	sil_wrb_mem((void *) UDnCTL2(UDnCH0), 0x82);

	/*
	 * UARTD0オプション制御レジスタ1(UD0OPT1)
	 * 	送信データ通常出力，受信データ通常入力
	 *  データ一貫性チェックなし
	 */
	sil_wrb_mem((void *) UDnOPT0(UDnCH0), 0x14);
	sil_wrb_mem((void *) UDnOPT1(UDnCH0), 0x00);

	/*
	 * 割込み許可設定
	 * 割込み制御レジスタを設定．
	 * 割込みマスク・フラグ（6bit）を許可（0）し，優先順位（2-0bit）をレベル7（最低位）に設定する．
	 * 割込みマスク・レジスタを設定．
	 * マスカブル割込みの割込みマスク状態の設定．UD0RMK（4bit）を0（許可）にする．
	 */
	sil_wrb_mem((void *) 0xFFFFF158, 0x07);
	sil_wrb_mem((void *) 0xFFFFF104, 0xEF);

	/*
	 * 送受信の許可
	 * UARTD0動作許可、送受信許可、転送方向：LSB、パリティ：なし、データ：８ビット、ストップビット：１ビット
	 */
	sil_wrb_mem((void *) UDnCTL0(UDnCH0), 0xF2);
	return;
}

void serial_write(uint8 data)
{
	/*
	 * 送信データを設定する．
	 * UARTDn送信データ・レジスタへの書き込みが送信トリガとなる．
	 */
	sil_wrb_mem((void *) UDnTX(UDnCH0), data);

	return;
}

void serial_read(uint8* data)
{
	/*
	 * 受信データを取得する．
	 */
	*data = sil_reb_mem((void *) UDnRX(UDnCH0));

	return;
}
