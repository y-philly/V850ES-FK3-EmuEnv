#ifndef _CAN_H_
#define _CAN_H_

#include "device/inc/device.h"

#define DEVICE_CAN_RCV_EX_CANID_NONE	(0xFFFFFFFFU)
typedef struct {
	bool (*recv) (uint32 *ch, uint32 *canid,uint32 *ex_canid, uint8 *data, uint8 *dlc);
	bool (*send) (uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc);
} DeviceCanOpType;

/*
 *  CANóMŠ®—¹Š„‚İ”Ô†
 */
#define INTNO_INTC1REC	69U

extern void device_can_register_ops(void *can, DeviceCanOpType *ops);


#endif /* _CAN_H_ */
