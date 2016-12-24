#include "dbg_executor.h"
#include "concrete_executor/dbg_std_executor.h"
#include "front/parser/concrete_parser/dbg_std_parser.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpuemu_ops.h"
#include "dbg_log.h"
#include "assert.h"
#include "concrete_executor/target/dbg_target_serial.h"
#include "symbol_ops.h"
#include "cui/cui_ops.h"
#include "dbg_target_cpu.h"
#include <stdio.h>
#include <string.h>
#define SYMBOL_CANDIATE_NUM		5

void dbg_std_executor_parse_error(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;

	 printf("ERROR: %s :command not found\n", arg->original_str);
	 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "NG\n"));

	 return;
}

void dbg_std_executor_break(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorBreakType *parsed_args = (DbgCmdExecutorBreakType *)(arg->parsed_args);
	 uint32 func_len;
	 uint32 addr;
	 uint32 size;

	 if (parsed_args->type == DBG_CMD_BBREAK_SET_SYMBOL) {
		 func_len = strlen((char*)parsed_args->symbol.str);
		 if (symbol_get_func((char*)parsed_args->symbol.str, func_len, &addr, &size) < 0) {
			 printf("ERROR: not found symbol %s\n", parsed_args->symbol.str);
			 symbol_print_func((char*)parsed_args->symbol.str, SYMBOL_CANDIATE_NUM);
			 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "NG\n"));
		 }
		 else {
			 if (cpuctrl_set_break(addr, BREAK_POINT_TYPE_FOREVER) == TRUE) {
				 printf("break %s 0x%x\n", parsed_args->symbol.str, addr);
				 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
			 }
			 else {
				 printf("ERROR: can not break %s\n", parsed_args->symbol.str);
				 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "NG\n"));
			 }
		 }
	 }
	 else if (parsed_args->type == DBG_CMD_BBREAK_SET) {
		 if (cpuctrl_set_break(parsed_args->break_addr, BREAK_POINT_TYPE_FOREVER) == TRUE) {
			 printf("break 0x%x\n", parsed_args->break_addr);
			 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
		 }
		 else {
			 printf("ERROR: can not break 0x%x\n", parsed_args->break_addr);
			 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "NG\n"));
		 }
	 }
	 else if (parsed_args->type == DBG_CMD_BREAK_INFO) {
		 uint32 i;
		 uint32 addr;
		 for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
			 int funcid;
			 uint32 funcaddr;
			 if (cpuctrl_get_break(i, &addr) == TRUE) {
				 funcid = symbol_pc2funcid(addr, &funcaddr);
				 if (funcid >= 0) {
					 printf("[%u] 0x%x %s(+0x%x)\n", i, addr, symbol_funcid2funcname(funcid), addr - funcaddr);
				 }
				 else {
					 printf("[%u] 0x%x\n", i, addr);
				 }
			 }
		 }
		 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
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
			 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
		 }
	 }
	 else if (parsed_args->type == DBG_CMD_DELETE_ALL) {
		 cpuctrl_del_all_break(BREAK_POINT_TYPE_FOREVER);
		 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
	 }
	 return;
}

void dbg_std_executor_cont(void *executor)
{
	DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	DbgCmdExecutorContType *parsed_args = (DbgCmdExecutorContType *)(arg->parsed_args);

	cpuctrl_set_debug_mode(FALSE);
	if (parsed_args->type == DBG_CMD_CONT_ALL) {
		cpuctrl_set_cont_clocks(FALSE, 0);
		cputhr_control_dbg_wakeup_cpu();
	}
	else {
		cpuctrl_set_cont_clocks(TRUE, parsed_args->cont_clocks);
		cputhr_control_dbg_wakeup_cpu_and_wait_for_cpu_stopped();
	}
	CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
	return;
}

void dbg_std_executor_next(void *executor)
{
	bool org_view_mode = dbg_log_is_view_mode();
	cpuctrl_set_cont_clocks(FALSE, 0);

	cpuctrl_set_debug_mode(TRUE);
	dbg_log_set_view_mode(TRUE);
	dbg_log_set_print_mode(TRUE);
	cputhr_control_dbg_wakeup_cpu_and_wait_for_cpu_stopped();
	dbg_log_set_print_mode(FALSE);
	dbg_log_set_view_mode(org_view_mode);
	return;
}

void dbg_std_executor_return(void *executor)
{
	uint32 retaddr;
	CoreIdType core_id;

	cpuctrl_set_cont_clocks(FALSE, 0);

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
void dbg_std_executor_exit(void *executor)
{
	cpuctrl_set_debug_mode(TRUE);
	cputhr_control_dbg_waitfor_cpu_stopped();
	printf("Exit\n");
	CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "OK\n"));
	exit(1);
	return;
}

void dbg_std_executor_elaps(void *executor)
{
	CpuEmuElapsType elaps;
	cpuemu_get_elaps(&elaps);
	printf("clock = cpu %I64u intc %I64u\n", elaps.total_clocks, elaps.intr_clocks);
	CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "%I64u\n", elaps.total_clocks));
	return;
}
void dbg_std_executor_view(void *executor)
{
	bool view_mode = dbg_log_is_view_mode();

	if (view_mode == TRUE) {
		dbg_log_set_view_mode(FALSE);
		printf("VIEW_MODE=OFF\n");
		CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "VIEW_MODE=OFF\n"));
	}
	else {
		dbg_log_set_view_mode(TRUE);
		printf("VIEW_MODE=ON\n");
		CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "VIEW_MODE=ON\n"));
	}
	return;
}

static void print_memory(uint32 vaddr, uint8 *top_addr, uint32 size)
{
	uint32 i;
	printf("size=%u byte\n", size);
	for (i = 0; i < size; i++) {
		printf("%4u 0x%x 0x%x\n", i, (vaddr + i), *(top_addr + i));
	}
	CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "%x\n", *top_addr));
	return;
}
static void print_memory_type(uint32 vaddr, uint8 *top_addr, uint32 size)
{
	if (size == 2) {
		uint16 *data = (uint16*)top_addr;
		printf("size=%u byte\n", size);
		printf("0x%x 0x%x\n", (vaddr), *(data));
		CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "%x\n",  *(data)));
	}
	else if (size == 4) {
		uint32 *data = (uint32*)top_addr;
		printf("size=%u byte\n", size);
		printf("0x%x 0x%x\n", (vaddr), *(data));
		CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "%x\n",  *(data)));
	}
	else {
		print_memory(vaddr, top_addr, size);
	}
	return;
}

void dbg_std_executor_print(void *executor)
{
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorPrintType *parsed_args = (DbgCmdExecutorPrintType *)(arg->parsed_args);
	 uint8 *data;
	 uint32 gl_len;
	 uint32 addr;
	 uint32 size;

	 if (parsed_args->type == DBG_CMD_PRINT_SYMBOL) {
		 gl_len = strlen((char*)parsed_args->symbol.str);
		 if (symbol_get_gl((char*)parsed_args->symbol.str, gl_len, &addr, &size) < 0) {
			 printf("ERROR: not found symbol %s\n", parsed_args->symbol.str);
			 symbol_print_gl((char*)parsed_args->symbol.str, SYMBOL_CANDIATE_NUM);
			 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "NG\n"));
		 }
		 else {
			 cpuemu_get_addr_pointer(addr, &data);
			 print_memory_type(addr, data, size);
		 }
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR) {
		 printf("ERROR: not supported:print addr(0x%x)\n", parsed_args->addr);
		 CUI_PRINTF((CPU_PRINT_BUF(), CPU_PRINT_BUF_LEN(), "NG\n"));
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR_SIZE) {
		 cpuemu_get_addr_pointer(parsed_args->addr, &data);
		 print_memory_type(parsed_args->addr, data, parsed_args->size);
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

void dbg_std_executor_info_cpu(void *executor)
{
	dbg_target_print_cpu();
	return;
}

void dbg_std_executor_func_trace(void *executor)
{
	int i;
	uint32 funcid;
	uint32 funcpcoff;
	char *funcname;
	DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	DbgCmdExecutorFuncTraceType *parsed_args = (DbgCmdExecutorFuncTraceType *)(arg->parsed_args);

	for (i = (parsed_args->bt_number - 1); i >= 0; i--) {
		funcname = cpuctrl_get_func_log_trace_info(i, &funcpcoff, &funcid);
		if (funcname == NULL) {
			break;
		}
		printf("[%3u] <%03u> %s\n", i, funcpcoff, funcname);
	}

	return;
}

static void print_stack_data(uint32 addr)
{
	 uint32 funcaddr;
	 int funcid;
	 uint32 gladdr;
	 int glid;
	 uint32 *datap;
	 uint32 data;


	 cpuemu_get_addr_pointer(addr, (uint8**)&datap);
	 data = *datap;

	 funcid = symbol_pc2funcid(data, &funcaddr);
	 if (funcid >= 0) {
		printf("						0x%x 0x%x %s(+0x%x)", addr, data, symbol_funcid2funcname(funcid), data - funcaddr);
	 }
	 else {
		glid = symbol_addr2glid(data, &gladdr);
		if (glid >= 0) {
			printf("						0x%x 0x%x %s(+0x%x)", addr, data, symbol_glid2glname(glid), data - gladdr);
		}
		else {
			printf("						0x%x 0x%x", addr, data);
		}
	 }
	printf("\n");
	return;
}
void dbg_std_executor_back_trace(void *executor)
{
	int i;
	uint32 prev_sp = -1;
	uint32 sp;
	Std_ReturnType err;
	uint32 gl_num = symbol_get_gl_num();
	int inx;

	for (inx = 0; inx < gl_num; inx++) {
		i = DBG_STACK_LOG_SIZE - 1;
		if (cpuctrl_get_stack_pointer(inx, 0, &sp) == STD_E_OK) {
			prev_sp = -1;
			printf("*************************************\n");
			while (i > 0) {
				err = cpuctrl_get_stack_pointer(inx, i, &sp);
				if (err == STD_E_OK) {
					if (prev_sp != -1) {
						uint32 addr_inx;
						uint32 addr = sp;
						uint32 num = (sp - prev_sp)/4;
#if 1
						for (addr_inx = 0; addr_inx < num; addr_inx++) {
							print_stack_data(addr);
							addr -= 4;
						}
#else
						printf("addr_num=%u sp=%u prev_sp=%u\n", num, sp, prev_sp);
#endif
					}
					prev_sp = sp;
					printf(" %-30s [%4u] 0x%08x\n", symbol_glid2glname(inx), i, sp);
					}
				i--;
			}
			printf("\n");
		}
	}


	return;
}
void dbg_std_executor_profile(void *executor)
{
	uint32 funcnum;
	uint32 funcid;
	char *funcname;
	CpuProfileType profile;

	funcnum = symbol_get_func_num();
	printf("%-50s %-15s %-15s %-15s\n", "funcname", "call_num", "func_time", "total_time");
	for (funcid = 0; funcid < funcnum; funcid++) {
		cpuctrl_profile_get(funcid, &profile);
		if (profile.call_num == 0) {
			continue;
		}
		funcname = symbol_funcid2funcname(funcid);
		printf("%-50s %-15I64u %-15I64u %-15I64u\n",
				funcname, profile.call_num,
				profile.func_time/profile.call_num, profile.total_time/profile.call_num);
	}

	return;
}
