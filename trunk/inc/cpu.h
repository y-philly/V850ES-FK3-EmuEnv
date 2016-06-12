#ifndef _CPU_H_
#define _CPU_H_

#include "std_types.h"
#include "cpu_types.h"
#include "reg.h"

extern CpuManagerType CpuManager;
extern void cpu_init(ElfLoaderType *loader);
extern void cpu_reset(CpuManagerType *cpu);
extern int cpu_exec(void);
extern void cpu_illegal_opcode_trap(CpuManagerType *cpu);

#endif /* _CPU_H_ */
