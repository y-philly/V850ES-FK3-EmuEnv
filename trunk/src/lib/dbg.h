#ifndef _DBG_H_
#define _DBG_H_

#define DEBUG_EMU
#ifdef DEBUG_EMU
#include <stdio.h>
#include "std_types.h"

#define DBG_BUFP_LEN		(1024U)
#define DBG_BUFP_MAX_CNT	(8192U)

typedef struct {
	int fd;
	bool is_view_mode;
	char *filepath;
	int count;
	struct {
		uint32 write_len;
		char p[DBG_BUFP_LEN];
	} buf[DBG_BUFP_MAX_CNT];
} DbgExecOpBufferType;

extern DbgExecOpBufferType DbgExecOpBuffer;
#define DBG_EXEC_OP_BUF()		((DbgExecOpBuffer.buf[DbgExecOpBuffer.count].p))
#define DBG_EXEC_OP_BUF_LEN()	(DBG_BUFP_LEN)
extern void dbg_print_init(char *filepath);
extern void dbg_print_sync(void);
static inline void dbg_print_set_view_mode(bool on)
{
	DbgExecOpBuffer.is_view_mode = on;
}

static inline bool dbg_print_is_view_mode(void)
{
	return DbgExecOpBuffer.is_view_mode;
}



#define DBG_PRINT(arg)	\
do { \
	if (dbg_print_is_view_mode() == TRUE) {	\
		DbgExecOpBuffer.buf[DbgExecOpBuffer.count].write_len = snprintf	arg;	\
		DbgExecOpBuffer.count++;	\
		dbg_print_sync();	\
	}	\
} while (0)
#else
#define DBG_PRINT(arg)
#endif


#endif /* _DBG_H_ */
