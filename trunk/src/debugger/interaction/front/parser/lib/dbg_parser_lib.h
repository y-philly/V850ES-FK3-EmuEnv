#ifndef _DBG_PARSER_LIB_H_
#define _DBG_PARSER_LIB_H_

#include "dbg_executor.h"

#if 0 //TODO 削除予定
/*
 * メモリ獲得失敗したらexitする．
 */
extern DbgCmdExecutorType *alloc_dbg_cmd_executor(void);
extern void free_dbg_cmd_executor(DbgCmdExecutorType *entry);
#endif

typedef enum {
	TOKEN_TYPE_VALUE_HEX,
	TOKEN_TYPE_VALUE_DEC,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_UNKNOWN,
} TokenEnumType;

typedef struct {
	uint32 value;
} TokenValueHexType;

typedef struct {
	uint32 value;
} TokenValueDecType;

#define TOKEN_STRING_MAX_SIZE	1024
typedef struct {
	uint32 len;
	uint8 str[TOKEN_STRING_MAX_SIZE];
} TokenStringType;

typedef struct {
	TokenEnumType	type;
	union {
		TokenValueHexType	hex;
		TokenValueDecType	dec;
		TokenStringType		str;
	} body;
} TokenValueType;

#define TOKEN_CONTAINER_MAX_SIZE	128
typedef struct {
	uint32	num;
	TokenValueType	array[TOKEN_CONTAINER_MAX_SIZE];
} TokenContainerType;

extern Std_ReturnType token_split(TokenContainerType *token_container, uint8 *str, uint32 len);

extern bool token_strcmp(const TokenStringType *str1, const TokenStringType *str2);

#endif /* _DBG_PARSER_LIB_H_ */
