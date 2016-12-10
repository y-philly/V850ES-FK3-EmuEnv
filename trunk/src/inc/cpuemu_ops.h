#ifndef _CPUEMU_OPS_H_
#define _CPUEMU_OPS_H_

#include "std_types.h"
#include "cpu_config.h"
#include "target_cpu.h"

typedef struct {
	uint64	total_clocks;
	uint64	intr_clocks;
} CpuEmuElapsType;
extern void cpuemu_init(void);
extern void cpuemu_get_elaps(CpuEmuElapsType *elaps);
extern uint32 cpuemu_get_retaddr(CoreIdType core_id);
extern Std_ReturnType cpuemu_get_addr_pointer(uint32 addr, uint8 **data);
extern void cpuemu_get_register(CoreIdType core_id, TargetCoreType *cpu);

#endif /* _CPUEMU_OPS_H_ */
