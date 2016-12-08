#include "front/parser/concrete_parser/dbg_std_parser.h"
#include "concrete_executor/dbg_std_executor.h"
#include <string.h>

/************************************************************************************
 * break コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType break_string = {
		.len = 5,
		.str = { 'b', 'r', 'e', 'a', 'k', '\0' },
};
static const TokenStringType break_string_short = {
		.len = 1,
		.str = { 'b', '\0' },
};


static const TokenStringType break_info_string = {
		.len = 4,
		.str = { 'i', 'n', 'f', 'o', '\0' },
};

DbgCmdExecutorType *dbg_parse_break(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	DbgCmdExecutorBreakType *parsed_args = (DbgCmdExecutorBreakType *)arg->parsed_args;

	if (token_container->num != 2) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &break_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &break_string_short) == TRUE)) {
		if (token_container->array[1].type == TOKEN_TYPE_VALUE_HEX) {
			arg->std_id = DBG_CMD_STD_ID_BREAK;
			arg->run = dbg_std_executor_break;
			parsed_args->type = DBG_CMD_BBREAK_SET;
			parsed_args->break_addr = token_container->array[1].body.hex.value;
			return arg;
		}
	}
	else if (token_strcmp(&token_container->array[0].body.str, &break_info_string) == TRUE) {
		if (token_strcmp(&token_container->array[1].body.str, &break_string) == TRUE) {
			arg->std_id = DBG_CMD_STD_ID_BREAK;
			arg->run = dbg_std_executor_break;
			parsed_args->type = DBG_CMD_BREAK_INFO;
			return arg;
		}
	}
	return NULL;
}

/************************************************************************************
 * delete コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType delete_string = {
		.len = 6,
		.str = { 'd', 'e', 'l', 'e', 't', 'e', '\0' },
};
static const TokenStringType delete_string_short = {
		.len = 1,
		.str = { 'd', '\0' },
};

DbgCmdExecutorType *dbg_parse_delete(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	DbgCmdExecutorDeleteType *parsed_args = (DbgCmdExecutorDeleteType *)arg->parsed_args;

	if (token_container->num > 2) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &delete_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &delete_string_short) == TRUE)) {
		if (token_container->num == 1) {
			arg->std_id = DBG_CMD_STD_ID_DELETE;
			parsed_args->type = DBG_CMD_DELETE_ALL;
			arg->run = dbg_std_executor_delete;
			return arg;
		}
		else if ((token_container->num == 2) &&  (token_container->array[1].type == TOKEN_TYPE_VALUE_DEC)) {
			arg->std_id = DBG_CMD_STD_ID_DELETE;
			parsed_args->type = DBG_CMD_DELETE_ONE;
			parsed_args->delete_break_no = token_container->array[1].body.dec.value;
			arg->run = dbg_std_executor_delete;
		}
	}
	return NULL;
}


/************************************************************************************
 * cont コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType cont_string = {
		.len = 4,
		.str = { 'c', 'o', 'n', 't', '\0' },
};
static const TokenStringType cont_string_short = {
		.len = 1,
		.str = { 'c', '\0' },
};

DbgCmdExecutorType *dbg_parse_cont(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &cont_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &cont_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_CONT;
		arg->run = dbg_std_executor_cont;
		return arg;
	}
	return NULL;
}
