#include "cpu.h"
#include "cpu_control/dbg_cpu_callback.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpuemu_ops.h"

void dbg_notify_cpu_clock_supply_start(const TargetCoreType *core)
{
	bool need_stop = FALSE;
	uint32 pc = cpu_get_pc(core);
	/*
	 * call callback
	 */
	//TODO


	if (cpuemu_cui_mode() == FALSE) {
		return;
	}
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
		cpuctrl_set_current_debugged_core(cpu_get_core_id(core));
		cpuctrl_set_debug_mode(TRUE);
		fflush(stdout);
		dbg_log_sync();
		/*
		 * return コマンド実行時の一時的なブレークポイントを削除する．
		 */
		cpuctrl_del_all_break(BREAK_POINT_TYPE_ONLY_ONCE);
		cputhr_control_cpu_wait();
	}
	return;
}

