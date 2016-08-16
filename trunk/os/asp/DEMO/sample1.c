#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"
#include "can_driver.h"

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))
void main_task(intptr_t exinf)
{
	CanDriverMessageBufferType buffer;

	buffer[0] = 9;
	buffer[1] = 9;
	buffer[2] = 9;
	buffer[3] = 9;
	buffer[4] = 9;
	buffer[5] = 9;
	buffer[6] = 9;
	buffer[7] = 9;

	can_driver_init();

	syslog_1(LOG_NOTICE, "main_task is started. %d", 1);
	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	can_driver_write(CAN_DRIVER_CHANNEL, CAN_DRIVER_TX1_MBOX, buffer);
	while (1) {
		;
	}

	SVC_PERROR(ext_ker());
	assert(0);
}

void target_can_handler(void)
{
	int err;
	//syslog(LOG_NOTICE, "target_can_handler:enter");

	err = iact_tsk(CAN_RCV_TASK);
	
	syslog(LOG_NOTICE, "target_can_handler:exit:err=%d", err);
	return;
}
void can_rcv_task(intptr_t exinf)
{
	int i;
	CanDriverReturnType err;
	CanDriverMessageBufferType buffer;
	
	syslog(LOG_NOTICE, "can_rcv_task:enter");
	
	err = can_driver_read(CAN_DRIVER_CHANNEL, CAN_DRIVER_RX_MBOX, buffer);
	syslog(LOG_NOTICE, "can_driver_read:%d", err);
	if (err == CAN_DRIVER_E_OK) {
#if 1
		for (i = 0; i < CAN_DRIVER_DLC; i++) {
			syslog(LOG_NOTICE, "msg[%d]=%d", i, buffer[i]);
		}
#endif
		err = can_driver_write(CAN_DRIVER_CHANNEL, CAN_DRIVER_TX2_MBOX, buffer);
		syslog(LOG_NOTICE, "can_driver_write:result=%d", err);
	}
	else {
		syslog(LOG_NOTICE, "can not read:%d", err);
	}
	
	syslog(LOG_NOTICE, "can_rcv_task:exit");

	ext_tsk();
	return;
}