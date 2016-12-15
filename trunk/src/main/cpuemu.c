#include "cpu.h"
#include "cpuemu_ops.h"
#include "bus.h"
#include "std_device_ops.h"
#include "std_cpu_ops.h"
#include "dbg_log.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpu_control/dbg_cpu_callback.h"
#include "token.h"
#include "file.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
 #include <windows.h>
#include <fcntl.h>

static DeviceClockType cpuemu_dev_clock;
static bool cpuemu_is_cui_mode = FALSE;
static uint64 cpuemu_cpu_end_clock = -1LLU;

bool cpuemu_cui_mode(void)
{
	return cpuemu_is_cui_mode;
}

void cpuemu_init(void *(*cpu_run)(void *))
{
	dbg_log_init("./log.txt");
	cpu_init();
	device_init(&virtual_cpu, &cpuemu_dev_clock);
	cputhr_control_init();
	if (cpu_run != NULL) {
		cpuemu_is_cui_mode = TRUE;
		cputhr_control_start(cpu_run);
	}
	else {
		cpuemu_is_cui_mode = FALSE;
	}
	return;
}

uint64 cpuemu_get_cpu_end_clock(void)
{
	return cpuemu_cpu_end_clock;
}
void cpuemu_set_cpu_end_clock(uint64 clock)
{
	cpuemu_cpu_end_clock = clock;
	return;
}

void cpuemu_get_elaps(CpuEmuElapsType *elaps)
{
	elaps->total_clocks = cpuemu_dev_clock.clock;
	elaps->intr_clocks = cpuemu_dev_clock.intclock;
	return;
}

uint32 cpuemu_get_retaddr(CoreIdType core_id)
{
	return cpu_get_return_addr((const TargetCoreType *)&virtual_cpu.cores[core_id]);
}

Std_ReturnType cpuemu_get_addr_pointer(uint32 addr, uint8 **data)
{
	return bus_get_pointer(CPU_CONFIG_CORE_ID_0, addr, data);
}


void cpuemu_get_register(CoreIdType core_id, TargetCoreType *cpu)
{
	*cpu = virtual_cpu.cores[core_id].core;
	return;
}


void *cpuemu_thread_run(void* arg)
{
	CoreIdType i;
	Std_ReturnType err;

	while (TRUE) {
		if (cpuemu_dev_clock.clock >= cpuemu_get_cpu_end_clock()) {
			dbg_log_sync();
			printf("EXIT for timeout(%I64u).\n", cpuemu_dev_clock.clock);
			exit(1);
		}


		/**
		 * デバイス実行実行
		 */
		device_supply_clock(&cpuemu_dev_clock);

		/**
		 * CPU 実行
		 */
		for (i = 0; i < CPU_CONFIG_CORE_NUM; i++) {
			/**
			 * CPU 実行開始通知
			 */
			dbg_notify_cpu_clock_supply_start(&virtual_cpu.cores[i].core);

			err = cpu_supply_clock(i);
			if (err != STD_E_OK) {
				printf("CPU(pc=0x%x) Exception!!\n", cpu_get_pc(&virtual_cpu.cores[i].core));
				fflush(stdout);
				cpuctrl_set_force_break();
	#if 0
				cpu_illegal_opcode_trap(&CpuManager);
	#endif
			}
		}


		//Sleep(100);

	}
	return NULL;
}


typedef struct {
	TokenStringType	folder_path;
	TokenStringType rx_path;
	TokenStringType tx_path;
} CpuEmuFifoFileType;

static const TokenStringType fifo_tx_string = {
		.len = 2,
		.str = { 'T', 'X', '\0' },
};
static const TokenStringType fifo_rx_string = {
		.len = 2,
		.str = { 'R', 'X', '\0' },
};


static CpuEmuFifoFileType cpuemu_fifo_file;
static FileType cpuemu_fifocfg;

static Std_ReturnType parse_fifopath(char *buffer, uint32 len)
{
	Std_ReturnType err;
	TokenContainerType token_container;

	err = token_split(&token_container, (uint8*)buffer, len);
	if (err != STD_E_OK) {
		goto errdone;
	}
	err = STD_E_INVALID;
	if (token_container.num != 2) {
		goto errdone;
	}
	if (token_container.array[0].type != TOKEN_TYPE_STRING) {
		goto errdone;
	}
	else if (token_container.array[1].type != TOKEN_TYPE_STRING) {
		goto errdone;
	}

	if (token_strcmp(&fifo_tx_string, &token_container.array[0].body.str) == TRUE) {
		if (cpuemu_fifo_file.tx_path.len > 0) {
			printf("ERROR: INVALID parameter number of TX >= 2\n");
			return STD_E_INVALID;
		}
		cpuemu_fifo_file.tx_path = cpuemu_fifo_file.folder_path;
		if (token_merge(&cpuemu_fifo_file.tx_path, &token_container.array[1].body.str) == FALSE) {
			printf("ERROR: INVALID filename is too long: %s\n", token_container.array[1].body.str.str);
			return STD_E_INVALID;
		}
	}
	else if (token_strcmp(&fifo_rx_string, &token_container.array[0].body.str) == TRUE) {
		if (cpuemu_fifo_file.rx_path.len > 0) {
			printf("ERROR: INVALID parameter number of RX >= 2\n");
			return STD_E_INVALID;
		}
		cpuemu_fifo_file.rx_path = cpuemu_fifo_file.folder_path;
		if (token_merge(&cpuemu_fifo_file.rx_path, &token_container.array[1].body.str) == FALSE) {
			printf("ERROR: INVALID filename is too long: %s\n", token_container.array[1].body.str.str);
			return STD_E_INVALID;
		}
	}

	return STD_E_OK;
errdone:
	printf("ERROR: Invalid parameter. Format should b {TX|RX} <fifo name>\n");
	return err;
}

/*
 * 出力
 * ・fifoファイル配置フォルダパス
 * ・tx fifo ファイル配置パス
 * ・rx fifo ファイル配置パス
 */
Std_ReturnType cpuemu_set_comm_fifocfg(const char* fifocfg)
{
	uint32 len;
	Std_ReturnType err;
	char buffer[4096];
	bool ret;

	ret = token_string_set(&cpuemu_fifocfg.filepath, fifocfg);
	if (ret == FALSE) {
		return STD_E_INVALID;
	}

	ret = file_ropen(&cpuemu_fifocfg);
	if (ret == FALSE) {
		return STD_E_NOENT;
	}

	//fifoファイル配置フォルダパス
	len = file_get_parent_folder_pathlen(fifocfg);
	memcpy(cpuemu_fifo_file.folder_path.str, fifocfg, len);
	cpuemu_fifo_file.folder_path.str[len] = '\0';
	cpuemu_fifo_file.folder_path.len = len;

	err = STD_E_INVALID;
	//fifo ファイル配置パス取得(1回目)
	len = file_getline(&cpuemu_fifocfg, buffer, 4096);
	if (len > 0) {
		err = parse_fifopath(buffer, len);
		if (err != STD_E_OK) {
			goto errdone;
		}
	}
	else {
		printf("ERROR: can not found data on %s...\n", fifocfg);
		goto errdone;
	}

	//fifo ファイル配置パス取得(2回目)
	len = file_getline(&cpuemu_fifocfg, buffer, 4096);
	if (len > 0) {
		err = parse_fifopath(buffer, len);
		if (err != STD_E_OK) {
			goto errdone;
		}
	}
	else {
		printf("ERROR: can not found data on %s...\n", fifocfg);
		goto errdone;
	}

	err = STD_E_INVALID;
	if (file_exist(cpuemu_get_comm_rx_fifo()) == FALSE) {
		printf("ERROR: can not found fifo file %s...\n", cpuemu_get_comm_rx_fifo());
		goto errdone;
	}
	if (file_exist(cpuemu_get_comm_tx_fifo()) == FALSE) {
		printf("ERROR: can not found fifo file %s...\n", cpuemu_get_comm_rx_fifo());
		goto errdone;
	}

	printf("RX fifo:%s\n", cpuemu_get_comm_rx_fifo());
	printf("TX fifo:%s\n", cpuemu_get_comm_tx_fifo());

	file_close(&cpuemu_fifocfg);
	return STD_E_OK;
errdone:
	file_close(&cpuemu_fifocfg);
	return err;
}

const char* cpuemu_get_comm_rx_fifo(void)
{
	return (const char*)cpuemu_fifo_file.rx_path.str;
}

const char* cpuemu_get_comm_tx_fifo(void)
{
	return (const char*)cpuemu_fifo_file.tx_path.str;
}
