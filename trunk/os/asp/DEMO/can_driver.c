#include "can_driver.h"

static CanDriverReturnType EnableBuffer(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverUint8 retry);
static void DisableBuffer(CanDriverChannelType channel, CanDriverMboxType mbox);
static void InitMboxTx(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid);
static void InitMboxRx(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverCanIdType canid);
static void InitMboxUnused(CanDriverChannelType channel, CanDriverMboxType mbox);

#define CAN_DRIVER_CHANNEL	1U
#define CAN_DRIVER_DLC		8U
#define CAN_DRIVER_ID_STD_SHIFT	2U
#define CAN_DRIVER_TX_MBOX		0U
#define CAN_DRIVER_RX_MBOX		1U
#define CAN_DRIVER_XX_MBOX		2U
#define CAN_DRIVER_DATA_IS_ARRIVAL(reg16)	((((reg16) & MPU_CAN_CnMCTRLm_DN_READ_BIT) == MPU_CAN_CnMCTRLm_DN_READ_BIT))

#define CAN_DRIVER_TX_CANID		0x100
#define CAN_DRIVER_RX_CANID		0x200
void can_driver_init(void)
{
	//TODO チャネルの初期化
	/*
	 * デモ用なので省略する．
	 */
	
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


	CanDriver_RegWrite16((CanDriverUint16 *)(MPU_CAN_ADDR_CnMCTRLm(channel, mbox)), 0x001CU);

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
