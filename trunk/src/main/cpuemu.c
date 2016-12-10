#include "cpu.h"
#include "cpuemu_ops.h"
#include "bus.h"
#include "std_device_ops.h"
#include "std_cpu_ops.h"

static DeviceClockType cpuemu_dev_clock;

void cpuemu_init(void)
{
	cpu_init();
	device_init(&virtual_cpu, &cpuemu_dev_clock);
	return;
}

void cpuemu_get_elaps(CpuEmuElapsType *elaps)
{
	elaps->total_clocks = cpuemu_dev_clock.clock;
	elaps->intr_clocks = cpuemu_dev_clock.intclock;
	return;
}

uint32 cpuemu_get_retaddr(CoreIdType core_id)
{
	return cpu_get_return_addr((const TargetCoreType *)&virtual_cpu.cores[core_id]);
}

Std_ReturnType cpuemu_get_addr_pointer(uint32 addr, uint8 **data)
{
	return bus_get_pointer(CPU_CONFIG_CORE_ID_0, addr, data);
}


void cpuemu_get_register(CoreIdType core_id, TargetCoreType *cpu)
{
	*cpu = virtual_cpu.cores[core_id].core;
	return;
}
