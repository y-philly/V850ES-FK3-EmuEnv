#include "front/parser/dbg_parser.h"
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	DbgCmdExecutorType *res;
	char *str = (char*)argv[1];
	int len = strlen(str) + 1;

	res = dbg_parse((uint8*)str, (uint32)len);

	printf("res=0x%x\n", res);
	if (res != NULL) {
		printf("org_str=%s\n", res->original_str);
		printf("std_id=%d\n", res->std_id);
	}

	return 0;
}
