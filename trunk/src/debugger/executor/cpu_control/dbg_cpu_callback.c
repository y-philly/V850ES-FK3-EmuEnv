#include "cpu.h"
#include "cpu_control/dbg_cpu_callback.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpu_control/dbg_cpu_control.h"

void dbg_notify_cpu_clock_supply_start(const TargetCoreType *core)
{
	bool need_stop = FALSE;
	uint32 pc = cpu_get_pc(core);
	/*
	 * call callback
	 */
	//TODO


	/*
	 * break point check
	 * debug mode check
	 */
	if ((cpuctrl_is_break_point(pc) == TRUE)) {
		need_stop = TRUE;
		printf("HIT break:0x%x\n", pc);
		printf("[DBG>");
	}
	else if ((cpuctrl_is_debug_mode() == TRUE)) {
		need_stop = TRUE;
	}

	if (need_stop == TRUE) {
		cpuctrl_set_debug_mode(TRUE);
		fflush(stdout);
		cputhr_control_cpu_wait();
	}
	return;
}

