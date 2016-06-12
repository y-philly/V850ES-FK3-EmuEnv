#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef TRUE
#define TRUE  1U
#endif

#ifndef FALSE
#define FALSE 0U
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef int	bool;

typedef struct {
	bool is_rcv;
	uint32 ch;
	uint32 canid;
	uint32 ex_canid;
	uint8 data[8U];
	uint8 dlc;
} DbgCmdCanRcvDataType;

DbgCmdCanRcvDataType test_rcv_data;

int main(int argc, const char *argv[])
{
	int fd;
	int err;

	test_rcv_data.is_rcv = TRUE;
	test_rcv_data.ch = 1U;

	test_rcv_data.canid = 0x200;
	test_rcv_data.ex_canid = 0xFFFFFFFF;

	test_rcv_data.data[0] = 0;
	test_rcv_data.data[1] = 1;
	test_rcv_data.data[2] = 2;
	test_rcv_data.data[3] = 3;
	test_rcv_data.data[4] = 4;
	test_rcv_data.data[5] = 5;
	test_rcv_data.data[6] = 6;
	test_rcv_data.data[7] = 7;

	test_rcv_data.dlc = 8U;

	fd = open(".\\can.txt", O_CREAT |O_WRONLY|O_BINARY, 00777);
	if (fd < 0) {
		printf("open err=%d\n", errno);
		fflush(stdout);
		return FALSE;
	}
	err = write(fd, (char*)&test_rcv_data, sizeof(DbgCmdCanRcvDataType));
	if (err <= 0) {
		printf("write err=%d\n", errno);
		fflush(stdout);
		return FALSE;
	}
	close(fd);
	return 0;
}
