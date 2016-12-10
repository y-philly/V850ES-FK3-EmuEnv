#ifndef _DBG_OPS_H_
#define _DBG_OPS_H_

#include "std_types.h"
#include "cpu_config.h"
#include "target_cpu.h"
#include "dbg_log.h"

extern void dbg_notify_cpu_clock_supply_start(const TargetCoreType *cpu);
extern void dbg_notify_cpu_stopped(const TargetCoreType *cpu);

#endif /* _DBG_OPS_H_ */
