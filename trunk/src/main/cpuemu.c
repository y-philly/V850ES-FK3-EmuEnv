#include "cpu.h"
#include "cpuemu_ops.h"
#include "bus.h"
#include "std_device_ops.h"

void cpuemu_get_elaps(CpuEmuElapsType *elaps)
{
	DeviceClockType dev_clock;

	device_print_clock(&dev_clock);
	elaps->total_clocks = dev_clock.clock;
	elaps->intr_clocks = dev_clock.intclock;
	return;
}

uint32 cpuemu_get_retaddr(CoreIdType core_id)
{
	return cpu_get_return_addr(&virtual_cpu.cores[core_id]);
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
