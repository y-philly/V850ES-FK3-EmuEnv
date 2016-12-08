#include "std_errno.h"
#include "front/parser/dbg_parser.h"
#include "front/parser/dbg_parser_config.h"
#include "concrete_executor/dbg_std_executor.h"
#include <string.h>

DbgCmdExecutorType *dbg_parse(uint8 *str, uint32 len)
{
	Std_ReturnType err;
	uint32 i;
	DbgCmdExecutorType *res;
	DbgCmdExecutorType *arg;
	TokenContainerType token_container;

	/*
	 * デフォルトエラーを作成する．
	 */
	arg = alloc_dbg_cmd_executor();
	memcpy(arg->original_str, str, len);
	arg->original_str[len] = '\0';
	arg->std_id = DBG_CMD_STD_ID_PARSE_ERROR;
	//TODO arg->run = dbg_std_executor_parse_error;

	/*
	 * 文字列をトークン分割する
	 */
	err = token_split(&token_container, str, len);
	if (err != STD_E_OK) {
		goto errdone;
	}


	/*
	 * パースする．
	 */
	for (i = 0; i < DBG_CMD_STD_ID_NUM; i++) {
		res = dbg_cmd_parser_std_table[i].parse(arg, &token_container);
		if (res != NULL) {
			return res;
		}
	}

errdone:
	/*
	 * ヒットしない場合は，デフォルトエラーを返す．
	 */
	return arg;
}
