#include "dbg_executor.h"
#include "concrete_executor/dbg_std_executor.h"
#include "front/parser/concrete_parser/dbg_std_parser.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpuemu_ops.h"
#include "dbg_log.h"
#include "assert.h"
#include "concrete_executor/target/dbg_target_serial.h"
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
		 if (cpuctrl_set_break(parsed_args->break_addr, BREAK_POINT_TYPE_FOREVER) == TRUE) {
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
		 cpuctrl_del_all_break(BREAK_POINT_TYPE_FOREVER);
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
		 if (cpuctrl_set_break(retaddr, BREAK_POINT_TYPE_ONLY_ONCE) == TRUE) {
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
	printf("clock = cpu %I64u intc %I64u\n", elaps.total_clocks, elaps.intr_clocks);
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

static void print_memory(uint32 vaddr, uint8 *top_addr, uint32 size)
{
	uint32 i;
	for (i = 0; i < size; i++) {
		printf("0x%x 0x%x\n", (vaddr + i), *(top_addr + i));
	}
	return;
}

void dbg_std_executor_print(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorPrintType *parsed_args = (DbgCmdExecutorPrintType *)(arg->parsed_args);
	 uint8 *data;

	 if (parsed_args->type == DBG_CMD_PRINT_SYMBOL) {
		 printf("ERROR: not supported:print symbol(%s)\n", parsed_args->symbol.str);
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR) {
		 printf("ERROR: not supported:print addr(0x%x)\n", parsed_args->addr);
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR_SIZE) {
		 cpuemu_get_addr_pointer(parsed_args->addr, &data);
		 print_memory(parsed_args->addr, data, parsed_args->size);
	 }
	 return;
}

void dbg_std_executor_serialin(void *executor)
{
	uint32 i;
	Std_ReturnType err;
	DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	DbgCmdExecutorSerialInType *parsed_args = (DbgCmdExecutorSerialInType *)(arg->parsed_args);

	for (i = 0; i < parsed_args->input.len; i++) {
		err = dbg_serial_in(parsed_args->channel, parsed_args->input.str[i]);
		if (err != STD_E_OK) {
			printf("ERROR:can not put serial data:%c\n", parsed_args->input.str[i]);
		}
	}

	return;
}
