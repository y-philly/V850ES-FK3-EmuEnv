#include "cpu_control/dbg_cpu_control.h"
#include "cpu_config.h"
#include "symbol_ops.h"

typedef struct {
	bool 				is_set;
	BreakPointEumType	type;
	uint32 				addr;
} DbgCpuCtrlBreakPointType;

DbgCpuCtrlBreakPointType dbg_cpuctrl_break_points[DBG_CPU_CONTROL_BREAK_SETSIZE] = {
		{ TRUE, 0x00 },
};
bool dbg_cpuctrl_dbg_mode = TRUE;

typedef struct {
	bool is_stopped;
	CoreIdType core_id;
} DbgCpuStoppedCoreType;
DbgCpuStoppedCoreType dbg_cpu_stopped_core;

typedef struct {
	bool	is_timeout;
	uint64 	cont_clocks;
} DbgCpuContType;

static DbgCpuContType dbg_cpu_cont[CPU_CONFIG_CORE_NUM];


typedef struct {
	uint32	current;
	uint32	lognum;
	uint32	funcid[DBG_FUNCLOG_TRACE_SIZE];
	uint32	funcoff[DBG_FUNCLOG_TRACE_SIZE];
	uint32	funcpc[DBG_FUNCLOG_TRACE_SIZE];
	char 	*funcname[DBG_FUNCLOG_TRACE_SIZE];
} DbgFuncLogTraceType;

static DbgFuncLogTraceType dbg_func_log_trace;


void cpuctrl_set_func_log_trace(uint32 pc)
{
	uint32 inx;
	uint32 next;
	uint32 funcpc;
	char *funcname;
	int funcid;

	funcid = symbol_pc2funcid(pc, &funcpc);
	if (funcid < 0) {
		return;
	}
	funcname = symbol_funcid2funcname(funcid);

	if (dbg_func_log_trace.lognum > 0) {
		inx = dbg_func_log_trace.current;
		if (dbg_func_log_trace.funcpc[inx] == funcpc) {
			return;
		}
		next = inx + 1;
		if (next >= DBG_FUNCLOG_TRACE_SIZE) {
			next = 0;
		}
	} else {
		next = 0;
	}

	dbg_func_log_trace.current = next;
	dbg_func_log_trace.funcid[next] = funcid;
	dbg_func_log_trace.funcoff[next] = pc - funcpc;
	dbg_func_log_trace.funcname[next] = funcname;
	dbg_func_log_trace.funcpc[next] = funcpc;
	if (dbg_func_log_trace.lognum < DBG_FUNCLOG_TRACE_SIZE) {
		dbg_func_log_trace.lognum++;
	}
	return;
}

char *cpuctrl_get_func_log_trace_info(uint32 bt_number, uint32 *funcpcoff, uint32 *funcid)
{
	int off;
	if (bt_number >= DBG_FUNCLOG_TRACE_SIZE) {
		return NULL;
	}
	if (bt_number >= dbg_func_log_trace.lognum) {
		return NULL;
	}

	if (dbg_func_log_trace.current >= bt_number) {
		off = dbg_func_log_trace.current - bt_number;
	}
	else {
		off = DBG_FUNCLOG_TRACE_SIZE - (bt_number - dbg_func_log_trace.current);
	}
	*funcid = dbg_func_log_trace.funcid[off];
	*funcpcoff = dbg_func_log_trace.funcoff[off];
	return dbg_func_log_trace.funcname[off];

}

void cpuctrl_set_cont_clocks(bool is_timeout, uint64 cont_clocks)
{
	dbg_cpu_cont[dbg_cpu_stopped_core.core_id].is_timeout = is_timeout;
	dbg_cpu_cont[dbg_cpu_stopped_core.core_id].cont_clocks = cont_clocks;
	return;
}
bool cpuctrl_is_timeout_cont_clocks(CoreIdType core_id)
{
	if (dbg_cpu_cont[core_id].is_timeout == TRUE) {
		if (dbg_cpu_cont[core_id].cont_clocks > 1) {
			dbg_cpu_cont[core_id].cont_clocks--;
			return FALSE;
		}
		return TRUE;
	}
	else {
		return FALSE;

	}
}

void cpuctrl_set_current_debugged_core(CoreIdType core_id)
{
	dbg_cpu_stopped_core.is_stopped = TRUE;
	dbg_cpu_stopped_core.core_id = core_id;
	return;
}
bool cpuctrl_get_current_debugged_core(CoreIdType *core_id)
{
	if (dbg_cpu_stopped_core.is_stopped == TRUE) {
		*core_id = dbg_cpu_stopped_core.core_id;
		return TRUE;
	}
	return FALSE;
}
void cpuctrl_clr_current_debugged_core(void)
{
	dbg_cpu_stopped_core.is_stopped = FALSE;
	return;
}


static DbgCpuCtrlBreakPointType *search_free_break_point_space(void)
{
	uint32 i;
	for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
		if (dbg_cpuctrl_break_points[i].is_set == FALSE) {
			return &dbg_cpuctrl_break_points[i];
		}
	}
	return NULL;
}

static DbgCpuCtrlBreakPointType *search_break_point(uint32 addr)
{
	uint32 i;
	for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
		if (dbg_cpuctrl_break_points[i].is_set == FALSE) {
			continue;
		}
		else if (dbg_cpuctrl_break_points[i].addr == addr) {
			return &dbg_cpuctrl_break_points[i];
		}
	}
	return NULL;
}

static DbgCpuCtrlBreakPointType *search_break_point_with_type(uint32 addr, BreakPointEumType type)
{
	uint32 i;
	for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
		if (dbg_cpuctrl_break_points[i].is_set == FALSE) {
			continue;
		}
		else if (dbg_cpuctrl_break_points[i].type != type) {
			continue;
		}
		else if (dbg_cpuctrl_break_points[i].addr == addr) {
			return &dbg_cpuctrl_break_points[i];
		}
	}
	return NULL;
}

bool cpuctrl_get_break(uint32 index, uint32 *addrp)
{
	if (index >= DBG_CPU_CONTROL_BREAK_SETSIZE) {
		return FALSE;
	}
	if (dbg_cpuctrl_break_points[index].is_set == FALSE) {
		return FALSE;
	}
	*addrp = dbg_cpuctrl_break_points[index].addr;
	return TRUE;
}

bool cpuctrl_is_break_point(uint32 addr)
{
	if (search_break_point(addr) != NULL) {
		return TRUE;
	}
	return FALSE;
}

bool cpuctrl_is_debug_mode(void)
{
	return (dbg_cpuctrl_dbg_mode == TRUE);
}

bool cpuctrl_set_break(uint32 addr, BreakPointEumType type)
{
	DbgCpuCtrlBreakPointType *bp;
	if (search_break_point_with_type(addr, type) != NULL) {
		return TRUE;
	}
	bp = search_free_break_point_space();
	if (bp != NULL) {
		bp->type = type;
		bp->is_set = TRUE;
		bp->addr = addr;
		return TRUE;
	}
	return FALSE;
}


bool cpuctrl_del_break(uint32 index)
{
	if (index > 0) {
		dbg_cpuctrl_break_points[index].is_set = FALSE;
		return TRUE;
	}
	return FALSE;
}
void cpuctrl_del_all_break(BreakPointEumType type)
{
	 uint32 i;
	 for (i = 0; i < DBG_CPU_CONTROL_BREAK_SETSIZE; i++) {
		 if (dbg_cpuctrl_break_points[i].type == type) {
			 cpuctrl_del_break(i);
		 }
	 }
	 return;
}
void cpuctrl_set_debug_mode(bool on)
{
	dbg_cpuctrl_dbg_mode = on;
	return;
}
void cpuctrl_set_force_break(void)
{
	cpuctrl_set_debug_mode(TRUE);

	return;
}
