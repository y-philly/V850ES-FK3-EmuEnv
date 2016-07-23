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



void dbg_do_can_recvbuf(DbgCmdType *cmd)
{
	//nothing to do.
	return;
}

static bool dbg_kavaser_can_init(uint32 ch)
{
	canInitializeLibrary();

	return TRUE;
}
static bool dbg_kavaser_can_recv(uint32 *ch, uint32 *canid, uint32 *ex_canid, uint8 *data, uint8 *dlc)
{
	return FALSE;
}

static bool dbg_kavaser_can_send(uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc)
{
	return FALSE;
}

