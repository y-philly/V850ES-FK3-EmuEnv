#include "device.h"
#include "intc.h"
#include "intc_ops.h"
#include "bus.h"

#include <stdio.h>

void device_init(CpuType *cpu, DeviceClockType *dev_clock)
{
	dev_clock->clock = 0;
	dev_clock->intclock = 0;
	device_init_intc(cpu, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_INTC]);
	device_init_timer(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	device_init_timer_m(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	device_init_serial(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_SERIAL]);
	device_init_can( &mpu_address_map.map[MPU_ADDRESS_REGION_INX_CAN]);
	device_init_adc(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	//device_init_wdg(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	device_init_comm(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);

	return;
}

void device_supply_clock(DeviceClockType *dev_clock)
{

	device_supply_clock_intc(dev_clock);
	device_supply_clock_timer(dev_clock);
	device_supply_clock_timer_m(dev_clock);
	device_supply_clock_serial(dev_clock);
	device_supply_clock_can(dev_clock);
	device_supply_clock_adc(dev_clock);
	//device_supply_clock_wdg(dev_clock);
	device_supply_clock_comm(dev_clock);
	return;
}

void device_print_clock(DeviceClockType *dev_clock)
{
	printf("clock = cpu:%I64u intc:%I64u\n", dev_clock->clock, dev_clock->intclock);
	return;
}

int device_io_write8(MpuAddressRegionType *region,  uint32 addr, uint8 data)
{
	return region->ops->put_data8(region, CPU_CONFIG_CORE_ID_0, (addr & region->mask), data);
}
int device_io_write16(MpuAddressRegionType *region, uint32 addr, uint16 data)
{
	return region->ops->put_data16(region, CPU_CONFIG_CORE_ID_0, (addr & region->mask), data);
}

int device_io_write32(MpuAddressRegionType *region, uint32 addr, uint32 data)
{
	return region->ops->put_data32(region, CPU_CONFIG_CORE_ID_0, (addr & region->mask), data);
}

int device_io_read8(MpuAddressRegionType *region, uint32 addr, uint8 *data)
{
	return region->ops->get_data8(region, CPU_CONFIG_CORE_ID_0, (addr & region->mask), data);
}

int device_io_read16(MpuAddressRegionType *region, uint32 addr, uint16 *data)
{
	return region->ops->get_data16(region, CPU_CONFIG_CORE_ID_0, (addr & region->mask), data);
}

int device_io_read32(MpuAddressRegionType *region, uint32 addr, uint32 *data)
{
	return region->ops->get_data32(region, CPU_CONFIG_CORE_ID_0, (addr & region->mask), data);
}


#include "intc.h"

void device_raise_int(uint16 intno)
{
	intc_raise_intr(intno);
}


