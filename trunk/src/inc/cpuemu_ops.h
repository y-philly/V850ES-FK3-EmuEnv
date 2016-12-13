#ifndef _CPUEMU_OPS_H_
#define _CPUEMU_OPS_H_

#include "std_types.h"
#include "cpu_config.h"
#include "target_cpu.h"

typedef struct {
	uint64	total_clocks;
	uint64	intr_clocks;
} CpuEmuElapsType;
extern void cpuemu_init(void *(*cpu_run)(void *));
extern void cpuemu_get_elaps(CpuEmuElapsType *elaps);
extern uint32 cpuemu_get_retaddr(CoreIdType core_id);
extern Std_ReturnType cpuemu_get_addr_pointer(uint32 addr, uint8 **data);
extern void cpuemu_get_register(CoreIdType core_id, TargetCoreType *cpu);

extern void *cpuemu_thread_run(void* arg);
extern bool cpuemu_cui_mode(void);
extern uint64 cpuemu_get_cpu_end_clock(void);
extern void cpuemu_set_cpu_end_clock(uint64 clock);

#endif /* _CPUEMU_OPS_H_ */
