#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "device/inc/device.h"


/*
 * �A�V���N���i�X�E�V���A���E�C���^�t�F�[�X�i UARTD�j
 */
#define UDnChannelNum			UINT_C(8)
#define UDnCH0					UINT_C(0)
#define UDnCH1					UINT_C(1)
#define UDnCH2					UINT_C(2)
#define UDnCH3					UINT_C(3)
#define UDnCH4					UINT_C(4)
#define UDnCH5					UINT_C(5)
#define UDnCH6					UINT_C(6)
#define UDnCH7					UINT_C(7)

/*
 * UARTDn���䃌�W�X�^ 0�i UDnCTL0�j
 */
#define UDnCTL0_BASE			UINT_C(0xFFFFFA00)
#define UDnCTL0(CH)				(UDnCTL0_BASE + ((CH) * 16U))

/*
 * UARTDn���䃌�W�X�^ 1�i UDnCTL1�j
 */
#define UDnCTL1_BASE			UINT_C(0xFFFFFA01)
#define UDnCTL1(CH)				(UDnCTL1_BASE + ((CH) * 16U))
/*
 * UARTDn���䃌�W�X�^ 2�i UDnCTL2�j
 */
#define UDnCTL2_BASE			UINT_C(0xFFFFFA02)
#define UDnCTL2(CH)				(UDnCTL2_BASE + ((CH) * 16U))

/*
 * UARTDn �I�v�V�������䃌�W�X�^ 0�i UDnOPT0�j
 */
#define UDnOPT0_BASE			UINT_C(0xFFFFFA03)
#define UDnOPT0(CH)				(UDnOPT0_BASE + ((CH) * 16U))

/*
 * UARTDn �I�v�V�������䃌�W�X�^ 1�i UDnOPT1�j
 */
#define UDnOPT1_BASE			UINT_C(0xFFFFFA05)
#define UDnOPT1(CH)				(UDnOPT1_BASE + ((CH) * 16U))

/*
 * UARTDn��ԃ��W�X�^�i UDnSTR�j
 */
#define UDnSTR_BASE				UINT_C(0xFFFFFA04)
#define UDnSTR(CH)				(UDnSTR_BASE + ((CH) * 16U))

/*
 * UARTDn���M�f�[�^�E���W�X�^�i UDnTX�j
 */
#define UDnTX_BASE				UINT_C(0xFFFFFA07)
#define UDnTX(CH)				(UDnTX_BASE + ((CH) * 16U))

/*
 * UARTDn��M�f�[�^�E���W�X�^�i UDnRX�j
 */
#define UDnRX_BASE				UINT_C(0xFFFFFA06)
#define UDnRX(CH)				(UDnRX_BASE + ((CH) * 16U))

typedef struct {
	bool (*recv) (uint8 ch, uint8 *data);
	bool (*send) (uint8 ch, uint8 data);
} DeviceSerialOpType;
/*
 *  UARTD0��M���������ݔԍ�
 */
#define INTNO_INTUD0R	UINT_C(36)
/*
 *  UARTD0���M���������ݔԍ�
 */
#define INTNO_INTUD0T	UINT_C(37)
extern void device_serial_register_ops(void *serial, uint8 ch, DeviceSerialOpType *ops);


#endif /* _SERIAL_H_ */
