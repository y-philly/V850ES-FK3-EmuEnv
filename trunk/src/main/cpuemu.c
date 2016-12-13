#include "cpu.h"
#include "cpuemu_ops.h"
#include "bus.h"
#include "std_device_ops.h"
#include "std_cpu_ops.h"
#include "dbg_log.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpu_control/dbg_cpu_callback.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
 #include <windows.h>

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

