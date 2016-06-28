#ifndef _DBG_H_
#define _DBG_H_

#include "std_types.h"
#include "cpu.h"
#include "device/inc/device.h"

typedef struct {
	char *name;
	uint32 addr;
	uint32 size;
} DbgSymbolType;


extern const uint32 symbol_func_size;
extern DbgSymbolType symbol_func[];

extern const uint32 symbol_gl_size;
extern DbgSymbolType symbol_gl[];

extern int symbol_get_func(char *funcname, uint32 func_len, uint32 *addrp, uint32 *size);
extern int symbol_get_gl(char *gl_name, uint32 gl_len, uint32 *addrp, uint32 *size);

extern char * symbol_pc2func(uint32 pc);
extern int symbol_pc2funcid(uint32 pc, uint32 *funcaddr);
extern char * symbol_funcid2funcname(int id);
extern uint32 symbol_funcid2funcaddr(int id);

extern void debug_cmd(void);
extern void debug_init(CpuManagerType *cpu, DeviceType *device);
extern bool debug_is_view_mode(void);
extern bool is_debug_mode(void);

typedef enum {
	DBG_CMD_ID_BREAK,
	DBG_CMD_ID_DELETE_BREAK,
	DBG_CMD_ID_INTR,
	DBG_CMD_ID_NEXT,
	DBG_CMD_ID_CONT,
	DBG_CMD_ID_VIEW,
	DBG_CMD_ID_MEMORY_REF_SIZE,
	DBG_CMD_ID_MEMORY_REF_OFF,
	DBG_CMD_ID_PMEM,
	DBG_CMD_ID_PMEMTYPE,
	DBG_CMD_ID_SERIAL_IN,
	DBG_CMD_ID_CAN_RCVIN,
	DBG_CMD_ID_ADC,
	DBG_CMD_ID_UNKNOWN
} DbgCmdIdType;

typedef struct {
	DbgCmdIdType id;
	uint32		 addr;
	uint32		 intno;
	char		 cmd_char;
	uint32		 off;
	uint32		 size;
	char		 symbol_name[1024];
	char		 serial_buffer[1024];
	uint32		 serial_buflen;
	uint8		 adc_cntl;
	int			 adc_ch; /* -1 �̏ꍇ�͑S�� */
	uint32		 adc_data;
} DbgCmdType;

extern int debugger_docmd(DbgCmdType *cmd);
extern void debugger_getcmd(DbgCmdType *cmd);
extern void debugger_callback(void);
extern int debugger_is_break(void);

extern void set_force_break(void);

#define DBG_BUFP_LEN		(1024U)
#define DBG_BUFP_MAX_CNT	(8192U)


typedef struct {
	int fd;
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
extern void debugger_exec_op_bufsync(void);
extern void debugger_exec_op_bufinit(char *filepath);
extern void debugger_save(void);

#define DEBUG_EMU
#ifdef DEBUG_EMU
#include <stdio.h>
#define DBG_PRINT(arg)	\
do { \
	if (debug_is_view_mode() == TRUE) {	\
		DbgExecOpBuffer.buf[DbgExecOpBuffer.count].write_len = snprintf	arg;	\
		DbgExecOpBuffer.count++;	\
		debugger_exec_op_bufsync();	\
	}	\
} while (0)
#else
#define DBG_PRINT(arg)
#endif


#endif /* _DBG_H_ */
