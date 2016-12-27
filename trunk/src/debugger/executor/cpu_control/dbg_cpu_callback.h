#ifndef _DBG_CPU_CALLBACK_H_
#define _DBG_CPU_CALLBACK_H_

#include "std_types.h"
#include "cpu_config.h"
#include "target_cpu.h"
#include "dbg_log.h"

extern void dbg_notify_cpu_clock_supply_start(const TargetCoreType *core);
extern void dbg_notify_cpu_clock_supply_end(const TargetCoreType *core);


#endif /* _DBG_CPU_CALLBACK_H_ */
