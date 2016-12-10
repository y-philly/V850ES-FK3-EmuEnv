#include "dbg_executor.h"
#include "concrete_executor/dbg_std_executor.h"
#include "front/parser/concrete_parser/dbg_std_parser.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpuemu_ops.h"
#include "dbg_log.h"
#include "assert.h"
#include <stdio.h>

void dbg_std_executor_parse_error(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;

	 printf("ERROR: %s :command not found\n", arg->original_str);
	 return;
}

void dbg_std_executor_break(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorBreakType *parsed_args = (DbgCmdExecutorBreakType *)(arg->parsed_args);

	 if (parsed_args->type == DBG_CMD_BBREAK_SET) {
		 if (cpuctrl_set_break(parsed_args->break_addr) == TRUE) {
			 printf("break 0x%x\n", parsed_args->break_addr);
		 }
		 else {
			 printf("ERROR: can not break 0x%x\n", parsed_args->break_addr);
		 }
	 }
	 else if (parsed_args->type == DBG_CMD_BREAK_INFO) {
		 uint32 i;
		 uint32 addr;
		 for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
			 if (cpuctrl_get_break(i, &addr) == TRUE) {
				 printf("[%u] 0x%x\n", i, addr);
			 }
		 }
	 }
	 return;
}

void dbg_std_executor_delete(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorDeleteType *parsed_args = (DbgCmdExecutorDeleteType *)(arg->parsed_args);

	 if (parsed_args->type == DBG_CMD_DELETE_ONE) {
		 if (cpuctrl_del_break(parsed_args->delete_break_no) == FALSE) {
			 printf("ERROR: can not delete %u\n", parsed_args->delete_break_no);
		 }
	 }
	 else if (parsed_args->type == DBG_CMD_DELETE_ALL) {
		 uint32 i;
		 for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
			(void)cpuctrl_del_break(i);
		 }
	 }
	 return;
}

void dbg_std_executor_cont(void *executor)
{
	cpuctrl_set_debug_mode(FALSE);
	cputhr_control_dbg_wakeup_cpu();
	return;
}

void dbg_std_executor_next(void *executor)
{
	cpuctrl_set_debug_mode(TRUE);
	cputhr_control_dbg_wakeup_cpu_and_wait_for_cpu_stopped();
	return;
}

void dbg_std_executor_return(void *executor)
{
	uint32 retaddr;
	CoreIdType core_id;

	if (cpuctrl_get_current_debugged_core(&core_id) == TRUE) {
		retaddr = cpuemu_get_retaddr(core_id);
		 if (cpuctrl_set_break(retaddr) == TRUE) {
			 printf("break 0x%x\n", retaddr);
		 }
		 else {
			 printf("ERROR: can not break 0x%x\n", retaddr);
		 }
		cpuctrl_set_debug_mode(FALSE);
		cputhr_control_dbg_wakeup_cpu();
	}
	else {
		ASSERT(0);
	}

	return;
}

void dbg_std_executor_quit(void *executor)
{
	cpuctrl_set_debug_mode(TRUE);
	cputhr_control_dbg_waitfor_cpu_stopped();
}

void dbg_std_executor_elaps(void *executor)
{
	CpuEmuElapsType elaps;
	cpuemu_get_elaps(&elaps);
	printf("clock = cpu:%I64u intc:%I64u\n", elaps.total_clocks, elaps.intr_clocks);
	return;
}
void dbg_std_executor_view(void *executor)
{
	bool view_mode = dbg_log_is_view_mode();

	if (view_mode == TRUE) {
		dbg_log_set_view_mode(FALSE);
		printf("VIEW_MODE=OFF\n");
	}
	else {
		dbg_log_set_view_mode(TRUE);
		printf("VIEW_MODE=ON\n");
	}
	return;
}
void dbg_std_executor_print(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorPrintType *parsed_args = (DbgCmdExecutorPrintType *)(arg->parsed_args);

	 if (parsed_args->type == DBG_CMD_PRINT_SYMBOL) {
		 printf("ERROR: not supported:print symbol(%s)\n", parsed_args->symbol.str);
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR) {
		 printf("ERROR: not supported:print addr(0x%x)\n", parsed_args->addr);
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR_SIZE) {
		 printf("print 0x%x %u\n", parsed_args->addr, parsed_args->size);
	 }
	 return;
}
