#ifndef _DBG_CPU_CONTROL_H_
#define _DBG_CPU_CONTROL_H_

#include "std_types.h"

#define DBG_CPU_CONTROL_BREAK_SETSIZE	128
#define DBG_FUNCLOG_TRACE_SIZE			1024

extern bool cpuctrl_get_break(uint32 index, uint32 *addrp);
extern bool cpuctrl_is_break_point(uint32 addr);
extern bool cpuctrl_is_debug_mode(void);

extern void cpuctrl_set_current_debugged_core(CoreIdType core_id);
extern void cpuctrl_clr_current_debugged_core(void);
extern bool cpuctrl_get_current_debugged_core(CoreIdType *core_id);

extern void cpuctrl_set_cont_clocks(bool is_timeout, uint64 cont_clocks);
extern bool cpuctrl_is_timeout_cont_clocks(CoreIdType core_id);

typedef enum {
	BREAK_POINT_TYPE_FOREVER,
	BREAK_POINT_TYPE_ONLY_ONCE,
} BreakPointEumType;
extern bool cpuctrl_set_break(uint32 addr, BreakPointEumType type);
extern bool cpuctrl_del_break(uint32 index);
extern void cpuctrl_del_all_break(BreakPointEumType type);
extern void cpuctrl_set_debug_mode(bool on);
extern void cpuctrl_set_force_break(void);


extern void cpuctrl_set_func_log_trace(uint32 pc);
extern char *cpuctrl_get_func_log_trace_info(uint32 bt_number, uint32 *funcpcoff, uint32 *funcid);

extern void cpuctrl_init(void);
/*
 * profile機能
 */
typedef struct {
	uint64 call_num;
	uint64 start_time;
	uint64 last_time;
	uint64 func_time;
	uint64 total_time;
} CpuProfileType;
extern void cpuctrl_profile_collect(uint32 pc);
extern void cpuctrl_profile_get(uint32 funcid, CpuProfileType *profile);

/*
 * 関数フレーム記録
 */
#define DBG_STACK_LOG_SIZE				1024
#define DBG_STACK_NUM					1024
extern void cpuctrl_set_stack_pointer(uint32 sp);
extern Std_ReturnType cpuctrl_get_stack_pointer(int glid, uint32 bt_number, uint32 *sp);

#endif /* _DBG_CPU_CONTROL_H_ */
