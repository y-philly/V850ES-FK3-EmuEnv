#include "front/parser/dbg_parser.h"
#include "front/parser/lib/dbg_parser_lib.h"
#include "loader/loader.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define DBG_LOADER_TEST

#ifdef DBG_LOADER_TEST
#define BUFSIZE (1024*1024)
static uint8 buf[BUFSIZE];
#endif

#ifdef DBG_PARSER_TEST
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

static void dbg_parser_test(void)
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
		}
	}
}
#endif

#ifdef DBG_LOADER_TEST
static int dbg_loader_test(int argc, const char *argv[])
{
    int len = 0;
    int ret = 0;
    int fd;
    FILE *fp;
    struct stat st;
	Std_ReturnType err;

    if (argc != 2) {
		fprintf(stderr, "argc=%d\n", argc);
		fprintf(stderr, "Usage: elf\n");
		return 1;
    }
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
		fprintf(stderr, "ERROR %s\n", argv[1]);
		fprintf(stderr, "Usage: elf\n");
		return 1;
    }
    fd = fileno(fp);

    fstat(fd, &st);
    printf("st.st_size=%d\n", st.st_size);
    len = fread(buf, st.st_size, 1, fp);
    printf("len=%d\n", len);

	err = elf_load(buf);

    fclose(fp);
}
#endif

int main(int argc, const char *argv[])
{
#ifdef DBG_PARSER_TEST
	dbg_parser_test();
	return 0;
#endif
#ifdef DBG_LOADER_TEST
	return dbg_loader_test(argc, argv);
#endif
}
