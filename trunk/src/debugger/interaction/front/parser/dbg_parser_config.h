#ifndef _DBG_PARSER_CONFIG_H_
#define _DBG_PARSER_CONFIG_H_

#include "dbg_interaction_std_ids.h"
#include "dbg_parser_config.h"
#include "dbg_executor.h"
#include "front/parser/lib/dbg_parser_lib.h"

typedef struct {
	DbgCmdExecutorType* (*parse) (DbgCmdExecutorType *arg, const TokenContainerType *token_container);
} DbgCmdParserTableType;

extern DbgCmdParserTableType dbg_cmd_parser_std_table[DBG_CMD_STD_ID_NUM];

#endif /* _DBG_PARSER_CONFIG_H_ */
