#include "front/parser/lib/dbg_parser_lib.h"
#include "std_errno.h"
#include "std_types.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

DbgCmdExecutorType *alloc_dbg_cmd_executor(void)
{
	DbgCmdExecutorType *entry;
	entry = malloc(sizeof(DbgCmdExecutorType));
	ASSERT(entry != NULL);
	return entry;
}

void free_dbg_cmd_executor(DbgCmdExecutorType *entry)
{
	if (entry != NULL) {
		free(entry);
	}
	return;
}

bool token_strcmp(const TokenStringType *str1, const TokenStringType *str2)
{
	if (str1->len != str2->len) {
		return FALSE;
	}
	if (strncmp((const char*)str1->str, (const char*)str2->str, str1->len) != 0) {
		return FALSE;
	}
	return TRUE;
}

/*
 * デミリタ=空白,",",":"
 */
static bool is_delimiter(char c)
{
	if (isspace(c)) {
		return TRUE;
	}
	else if (c == ',') {
		return TRUE;
	}
	else if (c == ':') {
		return TRUE;
	}
	else if (c == '\0') {
		return TRUE;
	}
	return FALSE;
}

static void set_token(TokenContainerType *token_container, TokenStringType *buffer)
{
	uint32 inx = token_container->num - 1;
	buffer->str[buffer->len] = '\0';
	char   *endptr;

	long long ret64;
	long ret32;

	/*
	 * 10進数チェック
	 */
	errno = 0;
	ret32 = strtol((const char*)buffer->str, &endptr, 10);
	if ((errno == 0) && (*endptr == '\0')) {
		token_container->array[inx].type = TOKEN_TYPE_VALUE_DEC;
		token_container->array[inx].body.dec.value = (uint32)ret32;
		return;
	}
	/*
	 * 16進数チェック
	 */
	if ((buffer->len > 2) && buffer->str[0] == '0' && buffer->str[1] == 'x') {
		errno = 0;
		ret64 = strtoull((const char*)buffer->str, &endptr, 16);
		if ((errno == 0) && (*endptr == '\0')) {
			token_container->array[inx].type = TOKEN_TYPE_VALUE_HEX;
			token_container->array[inx].body.hex.value = (uint32)ret64;
			return;
		}
	}
	token_container->array[inx].type = TOKEN_TYPE_STRING;
	token_container->array[inx].body.str = *buffer;
	return;
}

typedef enum {
	TOKEN_CHECK_STATE_DEMILITER = 0,
	TOKEN_CHECK_STATE_CODE,
} TokenCheckStateType;
Std_ReturnType token_split(TokenContainerType *token_container, uint8 *str, uint32 len)
{
	uint32 i;
	volatile TokenCheckStateType state;
	TokenStringType buffer;

	state = TOKEN_CHECK_STATE_DEMILITER;
	buffer.len = 0;

	token_container->num = 0;

	for (i = 0; i < len; i++) {
		switch (state) {
		case TOKEN_CHECK_STATE_DEMILITER:
			if (is_delimiter(str[i]) == TRUE) {
				break;
			}
			if (token_container->num > TOKEN_CONTAINER_MAX_SIZE) {
				return STD_E_INVALID;
			}
			token_container->num++;
			buffer.str[buffer.len] = str[i];
			buffer.len++;
			state = TOKEN_CHECK_STATE_CODE;
			break;
		case TOKEN_CHECK_STATE_CODE:
			if (is_delimiter(str[i]) == TRUE) {
				set_token(token_container, &buffer);
				buffer.len = 0;
				state = TOKEN_CHECK_STATE_DEMILITER;
				break;
			}
			if (buffer.len >= (TOKEN_STRING_MAX_SIZE - 1)) {
				return STD_E_INVALID;
			}
			buffer.str[buffer.len] = str[i];
			buffer.len++;
			if (i == (len - 1)) { //終端
				set_token(token_container, &buffer);
				buffer.len = 0;
				state = TOKEN_CHECK_STATE_DEMILITER;
				break;
			}
			break;
		default:
			return STD_E_INVALID;
		}
	}

	return STD_E_OK;
}
