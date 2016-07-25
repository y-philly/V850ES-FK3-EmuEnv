#include "dbg_can.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <memory.h>
#include <windows.h>

#include <canlib.h>

static bool dbg_kavaser_can_init(uint32 ch);
static bool dbg_kavaser_can_recv(uint32 *ch, uint32 *canid, uint32 *ex_canid, uint8 *data, uint8 *dlc);
static bool dbg_kavaser_can_send(uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc);

DeviceCanOpType dbg_can_ops = {
	dbg_kavaser_can_init,
	dbg_kavaser_can_recv,
	dbg_kavaser_can_send,
};

typedef struct {
	int channel;
	int baud;
	CanHandle handle;
	canStatus status;
} DbgKvaserType;

static DbgKvaserType dbg_kvaser = {
	0,
	BAUD_500K,
	0,
	canOK
};

void dbg_do_can_recvbuf(DbgCmdType *cmd)
{
	//nothing to do.
	return;
}

static bool dbg_kavaser_can_init(uint32 ch)
{
	CanHandle handle;

	canInitializeLibrary();

	handle = canOpenChannel(dbg_kvaser.channel, canOPEN_EXCLUSIVE);
	if (handle < 0) {
		printf("dbg_kavaser_can_init:canOpenChannel err=%d\n", handle);
		return FALSE;
	}
	printf("dbg_kavaser_can_init:canOpenChannel OK:handle=%d\n", handle);

	dbg_kvaser.handle = handle;
	dbg_kvaser.status = canSetBusParams(handle, dbg_kvaser.baud, 0, 0, 0, 0, 0);
	if (dbg_kvaser.status != canOK) {
		printf("dbg_kavaser_can_init:canSetBusParams err=%d\n", dbg_kvaser.status);
		return FALSE;
	}
	printf("dbg_kavaser_can_init:canSetBusParams OK:baud=%d\n", dbg_kvaser.baud);
	dbg_kvaser.status = canBusOn(handle);
	if (dbg_kvaser.status != canOK) {
		printf("dbg_kavaser_can_init:canBusOn err=%d\n", dbg_kvaser.status);
		return FALSE;
	}
	printf("dbg_kavaser_can_init:canBusOn OK\n");

	return TRUE;
}
static bool dbg_kavaser_can_recv(uint32 *ch, uint32 *canid, uint32 *ex_canid, uint8 *data, uint8 *dlc)
{
	unsigned int flag = 0;
	unsigned int out_dlc;
	unsigned long time;
	dbg_kvaser.status = canReadWait(dbg_kvaser.handle, (long*)canid, (void*)data, &out_dlc, (unsigned int*)&flag, &time, 0);
	if (dbg_kvaser.status != canOK) {
		//printf("dbg_kavaser_can_recv:canReadWait err=%d\n", dbg_kvaser.status);
		return FALSE;
	}
	return TRUE;
}

static bool dbg_kavaser_can_send(uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc)
{
	int flag = 0;

	dbg_kvaser.status = canWriteWait(dbg_kvaser.handle, msg_id, data, dlc, flag, -1);
    if (dbg_kvaser.status != canOK) {
		printf("dbg_kavaser_can_send:canWriteWait err=%d\n", dbg_kvaser.status);
		return FALSE;
    }
	return TRUE;
}

