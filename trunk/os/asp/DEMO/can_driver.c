#include "can_driver.h"
#include "can_driver_cfg.h"
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"

static CanDriverReturnType EnableBuffer(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverUint8 retry);
static void DisableBuffer(CanDriverChannelType channel, CanDriverMboxType mbox);
static void InitMboxTx(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid);
static void InitMboxRx(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid);
static void InitMboxUnused(CanDriverChannelType channel, CanDriverMboxType mbox);

static void CanDriver_RegWrite8(CanDriverUint8 *addr, CanDriverUint8 data)
{
	(*(volatile CanDriverUint8*)addr) = data;
	return;
}

static CanDriverUint8 CanDriver_RegRead8(CanDriverUint8 *addr)
{
	return (*(volatile CanDriverUint8*)addr);
}


static void CanDriver_RegWrite16(CanDriverUint16 *addr, CanDriverUint16 data)
{
	(*(volatile CanDriverUint16*)addr) = data;
	return;
}

static CanDriverUint16 CanDriver_RegRead16(CanDriverUint16 *addr)
{
	return (*(volatile CanDriverUint16*)addr);
}

void can_driver_init(void)
{
	//TODO チャネルの初期化
	/*
	 * デモ用なので省略する．
	 */
	/*
	 * CANモジュールの受信割り込みの許可を設定する．
	 *
	 * 設定対象レジスタ：
	 * CANモジュール割込み許可レジスタ　CIE1ビットの設定
	 * bit: SetCIE1	  : 9
	 * 		ClearCIE1 : 0
	 * SetCIE1ビット： 1　ClearCIE1ビット： 0　：　受信割り込みを許可ビットをセットする．
	 */
	CanDriver_RegWrite16((CanDriverUint16 *)MPU_CAN_ADDR_CnIE(CAN_DRIVER_CHANNEL), MPU_CAN_CnIE_CIE1_WRITE_SET_BIT);
	
	
	
	//MBOXの初期化
	/*
	 * 0: TX, 0x100
	 */
	(void)EnableBuffer(CAN_DRIVER_CHANNEL, CAN_DRIVER_TX_MBOX, 1U);
	InitMboxTx(CAN_DRIVER_CHANNEL, CAN_DRIVER_TX_MBOX, CAN_DRIVER_TX_CANID);
	DisableBuffer(CAN_DRIVER_CHANNEL, CAN_DRIVER_TX_MBOX);
	/*
	 * 1: RX, 0x200
	 */
	(void)EnableBuffer(CAN_DRIVER_CHANNEL, CAN_DRIVER_RX_MBOX, 1U);
	InitMboxRx(CAN_DRIVER_CHANNEL, CAN_DRIVER_RX_MBOX, CAN_DRIVER_RX_CANID);
	DisableBuffer(CAN_DRIVER_CHANNEL, CAN_DRIVER_RX_MBOX);
	
	InitMboxUnused(CAN_DRIVER_CHANNEL, CAN_DRIVER_XX_MBOX);
	return;
}

CanDriverReturnType can_driver_write(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverMessageBufferType buffer)
{
	CanDriverUint16 reg16;
	CanDriverUint8 i;
	CanDriverReturnType error;
	
	if (channel != CAN_DRIVER_CHANNEL) {
		return CAN_DRIVER_E_INVALID;
	}
	else if (mbox != CAN_DRIVER_TX_MBOX) {
		return CAN_DRIVER_E_INVALID;
	}

#if 1
	reg16 = CanDriver_RegRead16((CanDriverUint16*)MPU_CAN_ADDR_CnMCTRLm(channel, mbox));
	if ((reg16 & MPU_CAN_CnMCTRLm_TRQ_READ_BIT) == MPU_CAN_CnMCTRLm_TRQ_READ_BIT) {
		return CAN_DRIVER_E_BUSY;
	}
	error = EnableBuffer(channel, mbox, 1U);
	if (error != CAN_DRIVER_E_OK) {
		return error;
	}

	/*
	 * 送信データをレジスタに設定
	 */
	for (i = 0; i < CAN_DRIVER_DLC; i++) {
		CanDriver_RegWrite8((CanDriverUint8 *)MPU_CAN_ADDR_CnMDATAxm(channel, mbox, i), buffer[i]);
	}

	
	DisableBuffer(channel, mbox);

	reg16 = MPU_CAN_CnMCTRLm_TRQ_WRITE_SET_BIT;
	CanDriver_RegWrite16((CanDriverUint16*)MPU_CAN_ADDR_CnMCTRLm(channel, mbox), reg16);
#else
	for (i = 0; i < CAN_DRIVER_DLC; i++) {
		CanDriver_RegWrite8((CanDriverUint8 *)MPU_CAN_ADDR_CnMDATAxm(channel, mbox, i), buffer[i]);
	}
	reg16 = MPU_CAN_CnMCTRLm_TRQ_WRITE_SET_BIT;
	CanDriver_RegWrite16((CanDriverUint16*)MPU_CAN_ADDR_CnMCTRLm(channel, mbox), reg16);
#endif

	return CAN_DRIVER_E_OK;
}

CanDriverReturnType can_driver_read(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverMessageBufferType buffer)
{
	CanDriverUint16 reg16;
	CanDriverUint8 i;
	
	if (channel != CAN_DRIVER_CHANNEL) {
		return CAN_DRIVER_E_INVALID;
	}
	else if (mbox != CAN_DRIVER_RX_MBOX) {
		return CAN_DRIVER_E_INVALID;
	}
	
	reg16 = CanDriver_RegRead16((CanDriverUint16*)MPU_CAN_ADDR_CnMCTRLm(channel, mbox));
	if (!CAN_DRIVER_DATA_IS_ARRIVAL(reg16)) {
		return CAN_DRIVER_E_NOENT;
	}

	if ((reg16 & MPU_CAN_CnMCTRLm_MUC_READ_BIT) == MPU_CAN_CnMCTRLm_MUC_READ_BIT) {
		return CAN_DRIVER_E_BUSY;
	}

	CanDriver_RegWrite16((CanDriverUint16*)MPU_CAN_ADDR_CnMCTRLm(channel, mbox), MPU_CAN_CnMCTRLm_DN_WRITE_CLEAR_BIT);


	for (i = 0; i < CAN_DRIVER_DLC; i++) {
		buffer[i] = CanDriver_RegRead8((CanDriverUint8*)MPU_CAN_ADDR_CnMDATAxm(channel, mbox, i));
	}

	return CAN_DRIVER_E_OK;
}

void target_can_handler(void)
{
	int err;
	syslog(LOG_NOTICE, "target_can_handler:enter");

	err = iact_tsk(CAN_RCV_TASK);
	
	syslog(LOG_NOTICE, "target_can_handler:exit:err=%d", err);
	return;
}
void can_rcv_task(intptr_t exinf)
{
	syslog(LOG_NOTICE, "can_rcv_task:enter");
	
	syslog(LOG_NOTICE, "can_rcv_task:exit");

	ext_tsk();
	return;
}
/* -------------- static --------------- */
static void InitMboxCanId(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid);

static CanDriverReturnType EnableBuffer(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverUint8 retry)
{
	int i;
	CanDriverUint16 reg16;

	i = 0;
	while (i != retry) {
		reg16 = MPU_CAN_CnMCTRLm_RDY_WRITE_CLEAR_BIT;
		CanDriver_RegWrite16((CanDriverUint16 *)MPU_CAN_ADDR_CnMCTRLm(channel, mbox), reg16);

		reg16 = CanDriver_RegRead16((CanDriverUint16 *)MPU_CAN_ADDR_CnMCTRLm(channel, mbox));
		if (MPU_CAN_CnMCTRLm_RDY_READ_BIT != (reg16 & MPU_CAN_CnMCTRLm_RDY_READ_BIT)) {
			break;
		}
		else {
			i++;
		}
	}

	if (i == retry) {
		return CAN_DRIVER_E_BUSY;
	}

	return CAN_DRIVER_E_OK;
}

static void DisableBuffer(CanDriverChannelType channel, CanDriverMboxType mbox)
{
	CanDriverUint16 reg16;

	reg16 = MPU_CAN_CnMCTRLm_RDY_WRITE_SET_BIT;
	CanDriver_RegWrite16((CanDriverUint16 *)MPU_CAN_ADDR_CnMCTRLm(channel, mbox), reg16);
	return;
}


static void InitMboxTx(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid)
{
	CanDriverUint8 i;
	CanDriver_RegWrite8((CanDriverUint8 *)(MPU_CAN_ADDR_CnMCONFm(channel, mbox)), 0x81U);

	/* ID設定 */
	InitMboxCanId(channel, mbox, canid);


	CanDriver_RegWrite8((CanDriverUint8 *)(MPU_CAN_ADDR_CnMDLCm(channel, mbox)),0x08);

	/* DATA */
	for (i = 0; i < CAN_DRIVER_DLC; i++) {
		CanDriver_RegWrite8((CanDriverUint8 *)(MPU_CAN_ADDR_CnMDATAxm(channel, mbox, i)), 0x00U);
	}


	CanDriver_RegWrite16((CanDriverUint16 *)(MPU_CAN_ADDR_CnMCTRLm(channel, mbox)), 0x001CU);
	return;
}

static void InitMboxRx(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid)
{

	CanDriver_RegWrite8((CanDriverUint8 *)(MPU_CAN_ADDR_CnMCONFm(channel, mbox)), 0x89U);

	/*ID*/
	InitMboxCanId(channel, mbox, canid);


	CanDriver_RegWrite16((CanDriverUint16 *)(MPU_CAN_ADDR_CnMCTRLm(channel, mbox)), 0x0816U);

	return;
}

static void InitMboxUnused(CanDriverChannelType channel, CanDriverMboxType mbox)
{

	CanDriver_RegWrite8((CanDriverUint8 *)(MPU_CAN_ADDR_CnMCONFm(channel, mbox)), 0x80U);

	CanDriver_RegWrite16((CanDriverUint16 *)(MPU_CAN_ADDR_CnMCTRLm(channel, mbox)), 0x001CU);

	return;
}

static void InitMboxCanId(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid)
{
	CanDriverUint16 reg16;

	CanDriver_RegWrite16((CanDriverUint16 *)MPU_CAN_ADDR_CnMIDLm(channel, mbox), 0x00);

	reg16 = (canid << CAN_DRIVER_ID_STD_SHIFT);
	CanDriver_RegWrite16((CanDriverUint16 *)MPU_CAN_ADDR_CnMIDHm(channel, mbox), reg16);

	return;
}
