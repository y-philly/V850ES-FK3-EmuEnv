#include "cpu.h"
#include "cpu_control/dbg_cpu_callback.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpuemu_ops.h"
#include "cui/cui_ops.h"

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
	 * cont timeout check
	 * break point check
	 * debug mode check
	 */
	if (cpuctrl_is_timeout_cont_clocks(cpu_get_core_id(core)) == TRUE) {
		need_stop = TRUE;
		printf("CONT TIMEOUT\n");
		printf("[DBG>");
	}
	else if ((cpuctrl_is_break_point(pc) == TRUE)) {
		need_stop = TRUE;
		printf("HIT break:0x%x\n", pc);
		printf("[DBG>");
	}
	else if ((cpuctrl_is_debug_mode() == TRUE)) {
		need_stop = TRUE;
	}

	if (need_stop == TRUE) {
		CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "core[%u].pc = %x\n", cpu_get_core_id(core), pc));
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

