#include "inc/device.h"
#include "intc.h"
#include "intc_ops.h"
#include "bus.h"

#include <stdio.h>

void device_init(CpuType *cpu, DeviceType *device)
{
	device->clock = 0;
	device->intclock = 0;
	device_init_intc(cpu, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_INTC]);
	device_init_timer(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	device_init_timer_m(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	device_init_serial(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_SERIAL]);
	device_init_can(device,  &mpu_address_map.map[MPU_ADDRESS_REGION_INX_CAN]);
	device_init_adc(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	//device_init_wdg(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);
	device_init_comm(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_PH0]);

	return;
}

void device_supply_clock(DeviceType *device)
{
	device->clock++;
	if (intc_control.current_intno != -1) {
		device->intclock++;
	}

	device_supply_clock_timer(device);
	device_supply_clock_timer_m(device);
	device_supply_clock_serial(device);
	device_supply_clock_can(device);
	device_supply_clock_adc(device);
	//device_supply_clock_wdg(device);
	device_supply_clock_comm(device);
	return;
}

void device_print_clock(DeviceType *device)
{
	printf("clock = cpu:%I64u intc:%I64u\n", device->clock, device->intclock);
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


