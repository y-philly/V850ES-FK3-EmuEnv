#include "front/parser/dbg_parser.h"
#include "front/parser/lib/dbg_parser_lib.h"
#include <stdio.h>
#include <string.h>

static int debugger_getline(char *line, int size)
{
	int n = 0;
	char c;
	while (TRUE) {
		c = fgetc(stdin);
		if (c < 0 || c == '\n') {
			break;
		}
		line[n] = c;
		n++;
	}
	return n;
}

int main(int argc, const char *argv[])
{
	DbgCmdExecutorType *res;
	char buffer[1024];
	int len;


	while (TRUE) {
		printf(">");
		fflush(stdout);
		len = debugger_getline(buffer, 1024);
		buffer[len] = '\0';
		res = dbg_parse((uint8*)buffer, (uint32)len);

		if (res != NULL) {
			res->run(res);
			free_dbg_cmd_executor(res);
		}
	}

	return 0;
}
