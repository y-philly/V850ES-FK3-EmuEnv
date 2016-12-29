#include "device/inc/comm.h"
#include "cpu.h"
#include "reg.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE	4096
#define SYNC_COUNT		10000	//500 usec
typedef struct {
	char *path;
	uint32 fileoff;
	uint32 off;
	uint32 size;
	uint32 max;
	char buffer[MAX_BUFFER_SIZE];
} CpuEmuCommFileType;

typedef struct {
	CpuEmuCommFileType rx_fifo;
	CpuEmuCommFileType tx_fifo;
	uint32 count;
	uint32 sync_count;
	DeviceType *device;
} CpuEmuCommDevType;

static CpuEmuCommDevType CpuEmuCommDev;
static void tx_fifo_init(void);
static void tx_fifo_sync(void);
static void tx_fifo_write(uint32 data);
static void rx_fifo_sync(void);
static void rx_fifo_read(void);
static void rx_fifo_read_status(void);

int CpuEmuCommDevisProc1;

void device_init_comm(DeviceType *device)
{
	if (CpuEmuCommDevisProc1 == 1) {
		CpuEmuCommDev.rx_fifo.path = CPU_EMU_COMM_FIFO1_PATH;
		CpuEmuCommDev.tx_fifo.path = CPU_EMU_COMM_FIFO2_PATH;
	}
	else {
		CpuEmuCommDev.rx_fifo.path = CPU_EMU_COMM_FIFO2_PATH;
		CpuEmuCommDev.tx_fifo.path = CPU_EMU_COMM_FIFO1_PATH;
	}

	CpuEmuCommDev.rx_fifo.max = MAX_BUFFER_SIZE;
	CpuEmuCommDev.tx_fifo.max = MAX_BUFFER_SIZE;

	CpuEmuCommDev.sync_count = SYNC_COUNT;

	CpuEmuCommDev.device = device;

	tx_fifo_init();
	return;
}

#if 1
void device_supply_clock_comm(DeviceType *device)
{
	CpuEmuCommDev.count++;
	if (CpuEmuCommDev.count >= CpuEmuCommDev.sync_count) {
		//polling
		tx_fifo_sync();
		//rx_fifo_sync();
		CpuEmuCommDev.count = 0;
	}
	return;
}
#endif
void comm_hook_update_reg32(CpuManagerType *cpu, uint32 regaddr, uint32 data)
{
	if (regaddr == CPU_EMU_COMM_FIFO_TX_DATA_ADDR) {
		tx_fifo_write(data);
	}
	return;
}
void comm_hook_load_reg32(CpuManagerType *cpu, uint32 regaddr)
{
	if (regaddr == CPU_EMU_COMM_FIFO_RX_DATA_ADDR) {
		rx_fifo_read();
	}
	else if (regaddr == CPU_EMU_COMM_FIFO_RX_STAT_ADDR) {
		rx_fifo_read_status();
	}
	return;
}


/*--------------------- static ---------------- */

static void tx_fifo_init(void)
{
#if 0
	int fd;
	fd = open(CpuEmuCommDev.tx_fifo.path, O_CREAT | O_TRUNC |O_WRONLY|O_BINARY, 00777);
	if (fd < 0) {
		printf("file open error:%s\n", CpuEmuCommDev.tx_fifo.path);
		exit(1);
	}

	close(fd);
#endif
	return;
}

static void tx_fifo_sync(void)
{
#if 0
	int fd;
	int err;

	fd = open(CpuEmuCommDev.tx_fifo.path, O_WRONLY|O_BINARY);
	if (fd < 0) {
		printf("file open error:%s\n", CpuEmuCommDev.tx_fifo.path);
		exit(1);
	}
	err = lseek(fd, CpuEmuCommDev.tx_fifo.fileoff, SEEK_SET);
	if (err < 0) {
		printf("lseek error:%s\n", CpuEmuCommDev.tx_fifo.path);
		exit(1);
	}

	err = write(fd, CpuEmuCommDev.tx_fifo.buffer, CpuEmuCommDev.tx_fifo.size);
	if (err != CpuEmuCommDev.tx_fifo.size) {
		printf("write error:%s\n", CpuEmuCommDev.tx_fifo.path);
		exit(1);
	}
	CpuEmuCommDev.tx_fifo.fileoff += err;
	CpuEmuCommDev.tx_fifo.off = 0;
	CpuEmuCommDev.tx_fifo.size = 0;

	close(fd);
	return;
#endif
}

static void rx_fifo_sync(void)
{
#if 0
	int fd;
	int err;
	struct stat buf;

	fd = open(CpuEmuCommDev.rx_fifo.path, O_RDONLY|O_BINARY);
	if (fd < 0) {
		printf("file open error:%s\n", CpuEmuCommDev.rx_fifo.path);
		exit(1);
	}
	err = fstat(fd, &buf);
	if (err < 0) {
		printf("fstat error:%s\n", CpuEmuCommDev.rx_fifo.path);
		exit(1);
	}
	if (buf.st_size > CpuEmuCommDev.rx_fifo.fileoff) {
		err = lseek(fd, CpuEmuCommDev.rx_fifo.fileoff, SEEK_SET);
		if (err < 0) {
			printf("lseek error:%s\n", CpuEmuCommDev.rx_fifo.path);
			exit(1);
		}
		err = read(fd, CpuEmuCommDev.rx_fifo.buffer, CpuEmuCommDev.rx_fifo.max);
		if (err < 0) {
			printf("read error:%s\n", CpuEmuCommDev.rx_fifo.path);
			exit(1);
		}
		CpuEmuCommDev.rx_fifo.fileoff += err;
		CpuEmuCommDev.rx_fifo.off = 0;
		CpuEmuCommDev.rx_fifo.size = err;

	}
	close(fd);

	return;
#endif
}

static void tx_fifo_write(uint32 data)
{
	uint32 write_data = (0xFF & data);
	uint8 write_data8 = (uint8)write_data;

	if (CpuEmuCommDev.tx_fifo.size >= CpuEmuCommDev.tx_fifo.max) {
		tx_fifo_sync();
	}

	if (CpuEmuCommDev.tx_fifo.size < CpuEmuCommDev.tx_fifo.max) {
		CpuEmuCommDev.tx_fifo.buffer[CpuEmuCommDev.tx_fifo.off] = write_data8;
		CpuEmuCommDev.tx_fifo.off++;
		CpuEmuCommDev.tx_fifo.size++;
	}
	else {
		printf("ERROR:tx_fifo_write:empty buffer!!!\n");
	}
	return;
}

static void rx_fifo_read(void)
{
	uint8 data8 = 0;
	uint32 stat_data;

	if (CpuEmuCommDev.rx_fifo.off >= CpuEmuCommDev.rx_fifo.size) {
		rx_fifo_sync();
	}

	if (CpuEmuCommDev.rx_fifo.off < CpuEmuCommDev.rx_fifo.size) {
		data8 = CpuEmuCommDev.rx_fifo.buffer[CpuEmuCommDev.rx_fifo.off];
		(void)device_io_write32(CpuEmuCommDev.device, CPU_EMU_COMM_FIFO_RX_DATA_ADDR, data8);
		CpuEmuCommDev.rx_fifo.off++;
	}

	if (CpuEmuCommDev.rx_fifo.off >= CpuEmuCommDev.rx_fifo.size) {
		stat_data = 0;
	}
	else {
		stat_data = 1;
	}
	//update status register
	(void)device_io_write32(CpuEmuCommDev.device, CPU_EMU_COMM_FIFO_RX_STAT_ADDR, stat_data);

	return;
}

static void rx_fifo_read_status(void)
{
	uint32 stat_data;

	if (CpuEmuCommDev.rx_fifo.off >= CpuEmuCommDev.rx_fifo.size) {
		rx_fifo_sync();
	}

	if (CpuEmuCommDev.rx_fifo.off >= CpuEmuCommDev.rx_fifo.size) {
		stat_data = 0;
	}
	else {
		stat_data = 1;
	}
	//update status register
	(void)device_io_write32(CpuEmuCommDev.device, CPU_EMU_COMM_FIFO_RX_STAT_ADDR, stat_data);

	return;
}


