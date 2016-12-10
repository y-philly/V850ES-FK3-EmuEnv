#include "cpu_control/dbg_cpu_control.h"


typedef struct {
	bool is_set;
	uint32 addr;
} DbgCpuCtrlBreakPointType;

DbgCpuCtrlBreakPointType dbg_cpuctrl_break_points[DBG_CPU_CONTROL_BREAK_SETSIZE] = {
		{ TRUE, 0x00 },
};
bool dbg_cpuctrl_dbg_mode = TRUE;

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


bool cpuctrl_set_break(uint32 addr)
{
	DbgCpuCtrlBreakPointType *bp;
	if (cpuctrl_is_break_point(addr) == TRUE) {
		return TRUE;
	}
	bp = search_free_break_point_space();
	if (bp != NULL) {
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

void cpuctrl_set_debug_mode(bool on)
{
	dbg_cpuctrl_dbg_mode = on;
	return;
}
