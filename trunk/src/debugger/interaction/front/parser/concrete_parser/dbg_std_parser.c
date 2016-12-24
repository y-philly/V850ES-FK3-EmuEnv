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
		else if (token_container->array[1].type == TOKEN_TYPE_STRING) {
			arg->std_id = DBG_CMD_STD_ID_BREAK;
			parsed_args->type = DBG_CMD_BBREAK_SET_SYMBOL;
			parsed_args->symbol = token_container->array[1].body.str;
			arg->run = dbg_std_executor_break;
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
			return arg;
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
	DbgCmdExecutorContType *parsed_args = (DbgCmdExecutorContType *)arg->parsed_args;

	if (token_container->num > 2) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}
	if ((token_container->num == 2) &&
			(token_container->array[1].type != TOKEN_TYPE_VALUE_DEC)) {
		return NULL;
	}


	if ((token_strcmp(&token_container->array[0].body.str, &cont_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &cont_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_CONT;
		if (token_container->num == 1) {
			parsed_args->type = DBG_CMD_CONT_ALL;
		}
		else {
			parsed_args->type = DBG_CMD_CONT_CLOCKS;
			parsed_args->cont_clocks = token_container->array[1].body.dec.value;
		}
		arg->run = dbg_std_executor_cont;
		return arg;
	}
	return NULL;
}




/************************************************************************************
 * elaps コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType elaps_string = {
		.len = 5,
		.str = { 'e', 'l', 'a', 'p', 's', '\0' },
};
static const TokenStringType elaps_string_short = {
		.len = 1,
		.str = { 'e', '\0' },
};
DbgCmdExecutorType *dbg_parse_elaps(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &elaps_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &elaps_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_ELAPS;
		arg->run = dbg_std_executor_elaps;
		return arg;
	}
	return NULL;
}


/************************************************************************************
 * next コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType next_string = {
		.len = 4,
		.str = { 'n', 'e', 'x', 't', '\0' },
};
static const TokenStringType next_string_short = {
		.len = 1,
		.str = { 'n', '\0' },
};
DbgCmdExecutorType *dbg_parse_next(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &next_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &next_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_NEXT;
		arg->run = dbg_std_executor_next;
		return arg;
	}
	return NULL;
}


/************************************************************************************
 * return コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType return_string = {
		.len = 6,
		.str = { 'r', 'e', 't', 'u', 'r', 'n', '\0' },
};
static const TokenStringType return_string_short = {
		.len = 1,
		.str = { 'r', '\0' },
};
DbgCmdExecutorType *dbg_parse_return(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &return_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &return_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_RETURN;
		arg->run = dbg_std_executor_return;
		return arg;
	}
	return NULL;
}


/************************************************************************************
 * view コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType view_string = {
		.len = 4,
		.str = { 'v', 'i', 'e', 'w', '\0' },
};
static const TokenStringType view_string_short = {
		.len = 1,
		.str = { 'v', '\0' },
};
DbgCmdExecutorType *dbg_parse_view(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &view_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &view_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_VIEW;
		arg->run = dbg_std_executor_view;
		return arg;
	}
	return NULL;
}


/************************************************************************************
 * print コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType print_string = {
		.len = 5,
		.str = { 'p', 'r', 'i', 'n', 't', '\0' },
};
static const TokenStringType print_string_short = {
		.len = 1,
		.str = { 'p', '\0' },
};
DbgCmdExecutorType *dbg_parse_print(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	DbgCmdExecutorPrintType *parsed_args = (DbgCmdExecutorPrintType *)arg->parsed_args;

	if ((token_container->num != 2) && (token_container->num != 3)) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &print_string) == FALSE) &&
			(token_strcmp(&token_container->array[0].body.str, &print_string_short) == FALSE)) {
		return NULL;
	}

	if (token_container->num == 2) {
		if (token_container->array[1].type == TOKEN_TYPE_STRING) {
			arg->std_id = DBG_CMD_STD_ID_PRINT;
			parsed_args->type = DBG_CMD_PRINT_SYMBOL;
			parsed_args->symbol = token_container->array[1].body.str;
			arg->run = dbg_std_executor_print;
			return arg;
		}
		else if (token_container->array[1].type == TOKEN_TYPE_VALUE_HEX) {
			arg->std_id = DBG_CMD_STD_ID_PRINT;
			parsed_args->type = DBG_CMD_PRINT_ADDR;
			parsed_args->addr = token_container->array[1].body.dec.value;
			arg->run = dbg_std_executor_print;
			return arg;
		}
	}
	else {
		if ((token_container->array[1].type == TOKEN_TYPE_VALUE_HEX) &&
				(token_container->array[2].type == TOKEN_TYPE_VALUE_DEC)) {
			arg->std_id = DBG_CMD_STD_ID_PRINT;
			parsed_args->type = DBG_CMD_PRINT_ADDR_SIZE;
			parsed_args->addr = token_container->array[1].body.dec.value;
			parsed_args->size = token_container->array[2].body.dec.value;
			arg->run = dbg_std_executor_print;
			return arg;
		}
	}

	return NULL;
}


/************************************************************************************
 * quit コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType quit_string = {
		.len = 4,
		.str = { 'q', 'u', 'i', 't', '\0' },
};
static const TokenStringType quit_string_short = {
		.len = 1,
		.str = { 'q', '\0' },
};
DbgCmdExecutorType *dbg_parse_quit(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &quit_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &quit_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_QUIT;
		arg->run = dbg_std_executor_quit;
		return arg;
	}
	return NULL;
}

/************************************************************************************
 * exit コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType exit_string = {
		.len = 4,
		.str = { 'e', 'x', 'i', 't', '\0' },
};
DbgCmdExecutorType *dbg_parse_exit(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &exit_string) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_EXIT;
		arg->run = dbg_std_executor_exit;
		return arg;
	}
	return NULL;
}


/************************************************************************************
 * serial コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType serial_string = {
		.len = 6,
		.str = { 's', 'e', 'r', 'i', 'a', 'l', '\0' },
};
static const TokenStringType serial_string_short = {
		.len = 1,
		.str = { 'S', '\0' },
};
DbgCmdExecutorType *dbg_parse_serialin(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	DbgCmdExecutorSerialInType *parsed_args = (DbgCmdExecutorSerialInType *)arg->parsed_args;

	if (token_container->num != 3) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}
	if (token_container->array[1].type != TOKEN_TYPE_VALUE_DEC) {
		return NULL;
	}
	if (token_container->array[2].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &serial_string) == TRUE) ||
			(token_strcmp(&token_container->array[0].body.str, &serial_string_short) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_SERIALIN;
		arg->run = dbg_std_executor_serialin;
		parsed_args->channel = token_container->array[1].body.dec.value;
		parsed_args->input = token_container->array[2].body.str;
		return arg;
	}
	return NULL;
}



/************************************************************************************
 * info cpu コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType info_cpu_string = {
		.len = 3,
		.str = { 'c', 'p', 'u', '\0' },
};

DbgCmdExecutorType *dbg_parse_info_cpu(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &info_cpu_string) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_INFO_CPU;
		arg->run = dbg_std_executor_info_cpu;
		return arg;
	}
	return NULL;
}

/************************************************************************************
 * func trace コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType func_trace_string = {
		.len = 2,
		.str = { 'f', 't', '\0' },
};
extern DbgCmdExecutorType *dbg_parse_func_trace(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	DbgCmdExecutorFuncTraceType *parsed_args = (DbgCmdExecutorFuncTraceType *)arg->parsed_args;

	if (token_container->num != 2) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}
	if (token_container->array[1].type != TOKEN_TYPE_VALUE_DEC) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &func_trace_string) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_FUNC_TRACE;
		parsed_args->bt_number = token_container->array[1].body.dec.value;
		arg->run = dbg_std_executor_func_trace;
		return arg;
	}
	return NULL;
}


/************************************************************************************
 * back trace コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType back_trace_string = {
		.len = 2,
		.str = { 'b', 't', '\0' },
};
extern DbgCmdExecutorType *dbg_parse_back_trace(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &back_trace_string) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_BACK_TRACE;
		arg->run = dbg_std_executor_back_trace;
		return arg;
	}
	return NULL;
}

/************************************************************************************
 * profile コマンド
 *
 *
 ***********************************************************************************/
static const TokenStringType prof_string = {
		.len = 4,
		.str = { 'p', 'r', 'o', 'f', '\0' },
};
DbgCmdExecutorType *dbg_parse_profile(DbgCmdExecutorType *arg, const TokenContainerType *token_container)
{
	if (token_container->num != 1) {
		return NULL;
	}

	if (token_container->array[0].type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	if ((token_strcmp(&token_container->array[0].body.str, &prof_string) == TRUE)) {
		arg->std_id = DBG_CMD_STD_ID_PROFILE;
		arg->run = dbg_std_executor_profile;
		return arg;
	}
	return NULL;
}

