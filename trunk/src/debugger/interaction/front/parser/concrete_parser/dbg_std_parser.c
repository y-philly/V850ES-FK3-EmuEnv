#include "dbg_std_parser.h"
#include <string.h>

#define BREAK_STR_LEN		5
#define BREAK_INFO_STR_LEN	4

static const TokenStringType break_string = {
		.len = 5,
		.str = { 'b', 'r', 'e', 'a', 'k', '\0' },
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

	if (token_strcmp(&token_container->array[0].body.str, &break_string) == TRUE) {
		if (token_container->array[1].type == TOKEN_TYPE_VALUE_HEX) {
			arg->std_id = DBG_CMD_STD_ID_BREAK;
			parsed_args->type = DBG_CMD_BBREAK_SET;
			parsed_args->break_addr = token_container->array[1].body.hex.value;
			return arg;
		}
	}
	else if (token_strcmp(&token_container->array[0].body.str, &break_info_string) == TRUE) {
		if (token_strcmp(&token_container->array[1].body.str, &break_string) == TRUE) {
			arg->std_id = DBG_CMD_STD_ID_BREAK;
			parsed_args->type = DBG_CMD_BREAK_INFO;
			return arg;
		}
	}
	return NULL;
}


