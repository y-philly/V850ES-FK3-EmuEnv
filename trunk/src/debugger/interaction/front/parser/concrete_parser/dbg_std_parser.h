#ifndef _DBG_STD_PARSER_H_
#define _DBG_STD_PARSER_H_

#include "dbg_executor.h"
#include "front/parser/lib/dbg_parser_lib.h"

/*
 * ブレーク設定
 *
 * break <address:16進数表記>
 *  ブレークポイントを設定する
 * info break
 * 	ブレークポイントを表示
 */
typedef enum {
	DBG_CMD_BBREAK_SET,
	DBG_CMD_BREAK_INFO
} DbgCmdBreakType;
typedef struct {
	DbgCmdBreakType	type;
	uint32 			break_addr;
} DbgCmdExecutorBreakType;
extern DbgCmdExecutorType *dbg_parse_break(DbgCmdExecutorType *arg, const TokenContainerType *token_container);


/*
 * ブレーク解除
 *
 * delete
 *	全てのブレークポイントを削除する
 * delete <ブレークポイント番号：10進数表記>
 * 	ブレークポイントを削除する
 *
 */
typedef enum {
	DBG_CMD_DELETE_ALL,
	DBG_CMD_DELETE_ONE
} DbgCmdBreakType;
typedef struct {
	uint32 delete_break_no;
} DbgCmdExecutorDeleteType;
extern DbgCmdExecutorType *dbg_parse_delete(DbgCmdExecutorType *arg, const TokenContainerType *token_container);


#endif /* _DBG_STD_PARSER_H_ */
