#ifndef _CAN_DRIVER_H_
#define _CAN_DRIVER_H_

#define UINT_C(arg)	arg##U

/*
 * CANレジスタ定義
 */
#define MPU_CAN_ADDR_CAN_PBA			0x03FEC000
#define MPU_CAN_ADDR_BASE_ADDR(pchannel)	(MPU_CAN_ADDR_CAN_PBA + (0x600 * (pchannel)))

#define MPU_CAN_ADDR_CnGMCTRL(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x000)
#define MPU_CAN_ADDR_CnGMCS(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x002)
#define MPU_CAN_ADDR_CnGMABT(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x006)
#define MPU_CAN_ADDR_CnGMABTD(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x008)
#define MPU_CAN_ADDR_CnMASK1L(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x040)
#define MPU_CAN_ADDR_CnMASK1H(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x042)
#define MPU_CAN_ADDR_CnMASK2L(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x044)
#define MPU_CAN_ADDR_CnMASK2H(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x046)
#define MPU_CAN_ADDR_CnMASK3L(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x048)
#define MPU_CAN_ADDR_CnMASK3H(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x04A)
#define MPU_CAN_ADDR_CnMASK4L(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x04C)
#define MPU_CAN_ADDR_CnMASK4H(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x04E)
#define MPU_CAN_ADDR_CnCTRL(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x050)
#define MPU_CAN_ADDR_CnLEC(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x052)
#define MPU_CAN_ADDR_CnINFO(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x053)
#define MPU_CAN_ADDR_CnERC(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x054)
#define MPU_CAN_ADDR_CnIE(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x056)
#define MPU_CAN_ADDR_CnINTS(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x058)
#define MPU_CAN_ADDR_CnBRP(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x05A)
#define MPU_CAN_ADDR_CnBTR(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x05C)
#define MPU_CAN_ADDR_CnLIPT(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x05E)
#define MPU_CAN_ADDR_CnRGPT(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x060)
#define MPU_CAN_ADDR_CnLOPT(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x062)
#define MPU_CAN_ADDR_CnTGPT(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x064)
#define MPU_CAN_ADDR_CnTS(pchannel)		(MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x066)

#define MPU_CAN_ADDR_CnMDATAxm(pchannel, mboxno, byte)			(((mboxno) * 0x20) + (byte) + MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x100)
#define MPU_CAN_ADDR_CnMDLCm(pchannel, mboxno)				(((mboxno) * 0x20) + 0x08 + MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x100)
#define MPU_CAN_ADDR_CnMCONFm(pchannel, mboxno)				(((mboxno) * 0x20) + 0x09 + MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x100)
#define MPU_CAN_ADDR_CnMIDLm(pchannel, mboxno)				(((mboxno) * 0x20) + 0x0A + MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x100)
#define MPU_CAN_ADDR_CnMIDHm(pchannel, mboxno)				(((mboxno) * 0x20) + 0x0C + MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x100)
#define MPU_CAN_ADDR_CnMCTRLm(pchannel, mboxno)				(((mboxno) * 0x20) + 0x0E + MPU_CAN_ADDR_BASE_ADDR((pchannel)) + 0x100)

/* Can register bit */
#define MPU_CAN_CnMCTRLm_TRQ_READ_BIT				UINT_C(0x0002)
#define MPU_CAN_CnMCTRLm_TRQ_WRITE_SET_BIT			UINT_C(0x0200)
#define MPU_CAN_CnMCTRLm_RDY_READ_BIT				UINT_C(0x0001)
#define MPU_CAN_CnMCTRLm_RDY_WRITE_CLEAR_BIT			UINT_C(0x0001)
#define MPU_CAN_CnMCTRLm_RDY_WRITE_SET_BIT			UINT_C(0x0100)
#define MPU_CAN_CnMCTRLm_MUC_READ_BIT				UINT_C(0x2000)
#define MPU_CAN_CnMCTRLm_DN_READ_BIT				UINT_C(0x0004)
#define MPU_CAN_CnMCTRLm_DN_WRITE_CLEAR_BIT			UINT_C(0x0004)
#define MPU_CAN_CnIE_CIE1_WRITE_SET_BIT				UINT_C(0x0200)
#define MPU_CAN_CnINTS_CINTS1_WRITE_CLEAR_BIT			UINT_C(0x0002)

#define MPU_CAN_CnCTRL_OPMODE_READ_BIT				UINT_C(0x0007)
#define MPU_CAN_CnCTRL_OPMODE_READ_NORMAL_OP_BIT		UINT_C(0x0001)
#define MPU_CAN_CnCTRL_OPMODE_READ_INIT_OP_BIT			UINT_C(0x0000)
#define MPU_CAN_CnCTRL_OPMODE_WRITE_NORMAL_OP_BIT		UINT_C(0x0106)
#define MPU_CAN_CnCTRL_OPMODE_WRITE_INIT_OP_BIT			UINT_C(0x0007)
#define MPU_CAN_CnMCONFm_RTR_BIT				UINT_C(0x40)
#define MPU_CAN_CnMIDHm_IDE_BIT					UINT_C(0x80)
#define MPU_CAN_CnINFO_BOFF_BIT					UINT_C(0x10)


/*
 * API定義
 */
typedef unsigned int 	CanDriverMboxType;
typedef unsigned int 	CanDriverChannelType;
typedef unsigned char 	CanDriverMessageBufferType[8U];
typedef unsigned char	CanDriverUint8;
typedef unsigned short 	CanDriverUint16;
typedef unsigned int	CanDriverUint32;
typedef unsigned int	CanDriverReturnType;
typedef unsigned short 	CanDriverCanIdType;

/*
 * エラー番号定義
 */
#define CAN_DRIVER_E_OK		0x00
#define CAN_DRIVER_E_BUSY	0x01
#define CAN_DRIVER_E_INVALID	0x02
#define CAN_DRIVER_E_NOENT	0x03

extern void can_driver_init(void);
extern CanDriverReturnType can_driver_write(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverMessageBufferType buffer);
extern CanDriverReturnType can_driver_read(CanDriverChannelType channel, CanDriverMboxType mbox, CanDriverMessageBufferType buffer);

static inline void CanDriver_RegWrite8(CanDriverUint8 *addr, CanDriverUint8 data)
{
	(*addr) = data;
	return;
}

static inline CanDriverUint8 CanDriver_RegRead8(CanDriverUint8 *addr)
{
	return (*addr);
}


static inline void CanDriver_RegWrite16(CanDriverUint16 *addr, CanDriverUint16 data)
{
	(*addr) = data;
	return;
}

static inline CanDriverUint16 CanDriver_RegRead16(CanDriverUint16 *addr)
{
	return (*addr);
}


#endif /* _CAN_DRIVER_H_ */
