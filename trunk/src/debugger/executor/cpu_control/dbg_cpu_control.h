#ifndef _DBG_CPU_CONTROL_H_
#define _DBG_CPU_CONTROL_H_

#include "std_types.h"

#define DBG_CPU_CONTROL_BREAK_SETSIZE	128

extern bool cpuctrl_get_break(uint32 index, uint32 *addrp);
extern bool cpuctrl_is_break_point(uint32 addr);
extern bool cpuctrl_is_debug_mode(void);

extern void cpuctrl_set_current_debugged_core(CoreIdType core_id);
extern void cpuctrl_clr_current_debugged_core(void);
extern bool cpuctrl_get_current_debugged_core(CoreIdType *core_id);

extern bool cpuctrl_set_break(uint32 addr);
extern bool cpuctrl_del_break(uint32 index);
extern void cpuctrl_set_debug_mode(bool on);

#endif /* _DBG_CPU_CONTROL_H_ */
