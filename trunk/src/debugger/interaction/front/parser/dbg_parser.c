#include "std_errno.h"
#include "front/parser/dbg_parser.h"
#include "front/parser/dbg_parser_config.h"
#include "concrete_executor/dbg_std_executor.h"
#include <string.h>

static DbgCmdExecutorType *last_command = NULL;
static DbgCmdExecutorType parse_command;

DbgCmdExecutorType *dbg_parse(uint8 *str, uint32 len)
{
	Std_ReturnType err;
	uint32 i;
	DbgCmdExecutorType *res = NULL;
	DbgCmdExecutorType *arg = NULL;
	TokenContainerType token_container;

	if ((len == 0) && (last_command != NULL)) {
		/*
		 * 改行の場合は，最後のコマンドを実行する．
		 */
		arg = last_command;
		return arg;
	}
	else {
		/*
		 * デフォルトエラーを作成する．
		 */
		arg = &parse_command;
		memcpy(arg->original_str, str, len);
		arg->original_str[len] = '\0';
		arg->std_id = DBG_CMD_STD_ID_PARSE_ERROR;
		arg->run = dbg_std_executor_parse_error;
	}

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
			break;
		}
	}
	if (res == NULL) {
		goto errdone;
	}
	/*
	 * 成功の場合は，最後のコマンドを記録する．
	 */
	last_command = res;

	return res;
errdone:
	last_command = NULL;
	/*
	 * ヒットしない場合は，デフォルトエラーを返す．
	 */
	return &parse_command;
}
