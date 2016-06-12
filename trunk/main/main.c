#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "cpu.h"
#include "cpu_types.h"
#include "elf_loader.h"
#include "op_dec.h"
#include "cpu_types.h"
#include "op_exec.h"
#include "intc.h"
#include "dbg.h"
#include "device/inc/device.h"


static ElfLoaderType loader;
static DeviceType DeviceManager;

static void *cpu_run(void *arg);
static pthread_mutex_t my_mutex;
static pthread_cond_t my_cv;
static pthread_cond_t dbg_cv;
static volatile bool cpu_is_running = FALSE;

static void cpu_check_point(void);

static void cpu_check_point(void)
{
	pthread_mutex_lock(&my_mutex);
	debugger_save();
	if (debugger_is_break()) {
		set_force_break();
		printf("HIT break:0x%x\n", CpuManager.cpu.pc);
		fflush(stdout);
	}
	if (is_debug_mode()) {
		debugger_callback();
		cpu_is_running = FALSE;
		pthread_cond_signal(&dbg_cv);
		//printf("wait for debugger end..\n");
		//fflush(stdout);

		//Sleep(1000);

		pthread_cond_wait(&my_cv, &my_mutex);
	}
	cpu_is_running = TRUE;
	pthread_mutex_unlock(&my_mutex);
	//printf("wakedup from debugger..\n");
	//fflush(stdout);

	return;
}
static void cpu_stop(void)
{
	//printf("cpu_stop:enter\n");
	//fflush(stdout);
	set_force_break();
#if 1
	pthread_mutex_lock(&my_mutex);
	while (cpu_is_running) {
		set_force_break();
		//printf("wait_for_cpu_stop\n");
		//fflush(stdout);
		pthread_cond_wait(&dbg_cv, &my_mutex);
	}
	pthread_mutex_unlock(&my_mutex);
#else
	while (cpu_is_running == TRUE) {
		//printf("wait_for_cpu_stop\n");
		//fflush(stdout);
		Sleep(1000);
	}
#endif
	//printf("ENTER DEBUG MODE>\n");
	fflush(stdout);
	return;
}
static void cpu_resume(void)
{
	pthread_mutex_lock(&my_mutex);
	pthread_cond_signal(&my_cv);
	pthread_mutex_unlock(&my_mutex);
	return;
}
void debug_cmd(void)
{
	DbgCmdType cmd;
	int need_recv_cmd = TRUE;
	cmd.id = DBG_CMD_ID_UNKNOWN;

	while (need_recv_cmd == TRUE) {
		/*
		 * DBG_MODE_CMD
		 */
		if (is_debug_mode()) {
			printf("@DBG>");
		}
		else {
			printf("@CPU>");
		}
		fflush(stdout);

		//端末からの受信待ち
		debugger_getcmd(&cmd);

		if (cmd.id != DBG_CMD_ID_CONT) {
			//cpu_runスレッドが動作している場合は，cpu_runスレッドの停止待ち
			cpu_stop();
			//printf("$Enter DEBUG MODE\n");
			//デバッグ処理を実行する．
		}
		need_recv_cmd = debugger_docmd(&cmd);

	}
	cpu_resume();
	return;
}

int main(int argc, const char *argv[])
{
	char *path;
	pthread_t thread;

	if (argc != 2) {
		printf("Usage: %s binary\n", argv[0]);
		return 1;
	}
	path = (char*)argv[1];


	cpu_init(&loader);


	ElfLoader_init(&loader,
			path,
			&CpuManager.ram);

#if 1
	/*
	 * 暫定対処：クロック初期化
	 */
	{
		uint32 addr = 0xFFFFF6C2; /* OSTC */
		uint32 *addrp;
		uint8 *regp;

		cpu_memget_addrp(&CpuManager, addr, &addrp);
		regp = (uint8*)addrp;
		*regp = 0x01;
	}
#endif


	intc_init(&CpuManager);
	DeviceManager.cpu = &CpuManager;
	DeviceManager.io_memory1 = &CpuManager.io_mem1;
	DeviceManager.io_memory2 = &CpuManager.io_mem2;
	device_init(&DeviceManager);

	debug_init(&CpuManager, &DeviceManager);
	debugger_exec_op_bufinit(".\\log.txt");

	//cpu_runスレッドを起動する．
	pthread_create(&thread , NULL , cpu_run , NULL);
	pthread_mutex_init(&my_mutex, NULL);
	pthread_cond_init(&my_cv, NULL);
	pthread_cond_init(&dbg_cv, NULL);



	while (TRUE) {

		debug_cmd();

	}

	return 0;
}


static void* cpu_run(void *arg)
{
	int ret;

	while (TRUE) {

		//デバッグ状態チェック
		//デバッグ状態である場合は待ち状態になる．
		cpu_check_point();

		/*
		 * デバイス実行
		 */
		device_supply_clock(&DeviceManager);

		if (CpuManager.is_halt == TRUE) {
			continue;
		}
		/*
		 * ペンディング中の割り込みを発生させる
		 */
		intc_raise_pending_intr(&CpuManager);

		/*
		 * CPU命令を実行する
		 */
		ret = cpu_exec();
		if (ret < 0) {
			printf("CPU Exception!!\n");
			fflush(stdout);
			set_force_break();
#if 0
			cpu_illegal_opcode_trap(&CpuManager);
#endif
		}
	}

	return NULL;
}
