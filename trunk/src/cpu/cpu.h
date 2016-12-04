#ifndef _CPU_H_
#define _CPU_H_

#include "std_types.h"
#include "cpu_config.h"
#include "target_cpu.h"

typedef struct {
	TargetCoreType		core;
} CpuCoreType;

typedef struct {
	CpuCoreType			cores[CPU_CONFIG_CORE_NUM];
} CpuType;

extern CpuType	virtual_cpu;


#endif /* _CPU_H_ */
