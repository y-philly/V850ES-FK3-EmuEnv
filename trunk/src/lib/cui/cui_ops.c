#include "cui_ops.h"
#include "std_types.h"
#include <unistd.h>
#include <stdio.h>

static FileOpType *cui_fileop = NULL;

int	cui_fileop_register(FileOpType *fileop)
{
	cui_fileop = fileop;
	return 0;
}


int  cui_getline(char *line, int size)
{
	int n = 0;
	char c;
	int rc;

	while (TRUE) {
		if (n >= size) {
			printf("ERROR:input is too long\n");
			return -1;
		}
		rc = read(cui_fileop->read_fd, &c, 1);
		if (rc <= 0) {
			return -1;
		}
		if (c < 0 || c == '\n') {
			break;
		}
		line[n] = c;
		n++;
	}
	return n;
}

void cui_write(char *line, int size)
{
	//TODO
	return;
}
