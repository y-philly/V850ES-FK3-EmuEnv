#ifndef _DBG_STD_PARSER_H_
#define _DBG_STD_PARSER_H_

#include "dbg_executor.h"
#include "front/parser/lib/dbg_parser_lib.h"

typedef enum {
	DBG_CMD_BBREAK_SET,
	DBG_CMD_BREAK_INFO
} DbgCmdBreakType;
typedef struct {
	DbgCmdBreakType	type;
	uint32 			break_addr;
} DbgCmdExecutorBreakType;
extern DbgCmdExecutorType *dbg_parse_break(DbgCmdExecutorType *arg, const TokenContainerType *token_container);

typedef enum {
	DBG_CMD_DELETE_ALL,
	DBG_CMD_DELETE_ONE
} DbgCmdDeleteType;
typedef struct {
	DbgCmdDeleteType 	type;
	uint32 				delete_break_no;
} DbgCmdExecutorDeleteType;
extern DbgCmdExecutorType *dbg_parse_delete(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
extern DbgCmdExecutorType *dbg_parse_cont(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
extern DbgCmdExecutorType *dbg_parse_elaps(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
extern DbgCmdExecutorType *dbg_parse_next(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
extern DbgCmdExecutorType *dbg_parse_return(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
extern DbgCmdExecutorType *dbg_parse_view(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
typedef struct {
	uint32 				addr;
	uint32 				size;
} DbgCmdExecutorPrintType;
extern DbgCmdExecutorType *dbg_parse_print(DbgCmdExecutorType *arg, const TokenContainerType *token_container);
extern DbgCmdExecutorType *dbg_parse_quit(DbgCmdExecutorType *arg, const TokenContainerType *token_container);


#endif /* _DBG_STD_PARSER_H_ */
