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

	buffer[0] = 0;
	buffer[1] = 1;
	buffer[2] = 2;
	buffer[3] = 3;
	buffer[4] = 4;
	buffer[5] = 5;
	buffer[6] = 6;
	buffer[7] = 7;

	can_driver_init();

	syslog_1(LOG_NOTICE, "main_task is started. %d", 1);
	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	can_driver_write(CAN_DRIVER_CHANNEL, CAN_DRIVER_TX_MBOX, buffer);
	while (1) {
		;
	}

	SVC_PERROR(ext_ker());
	assert(0);
}
