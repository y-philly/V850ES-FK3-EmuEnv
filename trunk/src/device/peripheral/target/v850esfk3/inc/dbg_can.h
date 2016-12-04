#ifndef _DBG_CAN_H_
#define _DBG_CAN_H_

#include "inc/device.h"
#include "dbg.h"

typedef struct {
	bool (*init) (uint32 ch);
	bool (*recv) (uint32 *ch, uint32 *canid,uint32 *ex_canid, uint8 *data, uint8 *dlc);
	bool (*send) (uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc);
} DeviceCanOpType;

extern DeviceCanOpType dbg_can_ops;

#endif /* _DBG_CAN_H_ */
