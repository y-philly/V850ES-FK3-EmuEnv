#include "front/parser/dbg_parser.h"
#include "front/parser/lib/dbg_parser_lib.h"
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	DbgCmdExecutorType *res;
	char *str = (char*)argv[1];
	int len = strlen(str) + 1;

	res = dbg_parse((uint8*)str, (uint32)len);

	if (res != NULL) {
		res->run(res);
		free_dbg_cmd_executor(res);
	}

	return 0;
}
