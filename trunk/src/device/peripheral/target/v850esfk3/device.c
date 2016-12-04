#include "inc/device.h"
#include "intc.h"
#include "intc_ops.h"
#include "bus.h"

#include <stdio.h>

void device_init(DeviceType *device)
{
	device->clock = 0;
	device->intclock = 0;
	device_init_intc(device, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_INTC]);
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

/*
 * TODO　デバイスがバスに依存するのはおかしいので，どこかのタイミングで削除したい．
 */
static int device_get_iomem(DeviceType *device, uint32 addr, uint32 **addrp)
{
	Std_ReturnType err;

	err = bus_get_pointer(CPU_CONFIG_CORE_ID_0, addr, (uint8**)addrp);
	if (err != STD_E_OK) {
		return -1;
	}

	return 0;
}


int device_io_write8(DeviceType *device,  uint32 addr, uint8 data)
{
	int ret;
	uint32 *addrp;
	uint8* datap;

	ret = device_get_iomem(device, addr, &addrp);
	if (ret < 0) {
		return -1;
	}
	datap = (uint8*)addrp;
	*datap = data;
	return 0;
}
int device_io_write16(DeviceType *device, uint32 addr, uint16 data)
{
	int ret;
	uint32 *addrp;
	uint16* datap;

	ret = device_get_iomem(device, addr, &addrp);
	if (ret < 0) {
		return -1;
	}
	datap = (uint16*)addrp;
	*datap = data;
	return 0;
}

int device_io_write32(DeviceType *device, uint32 addr, uint32 data)
{
	int ret;
	uint32 *addrp;
	uint32* datap;

	ret = device_get_iomem(device, addr, &addrp);
	if (ret < 0) {
		return -1;
	}
	datap = (uint32*)addrp;
	*datap = data;
	return 0;
}

int device_io_read8(DeviceType *device, uint32 addr, uint8 *data)
{
	int ret;
	uint32 *addrp;
	uint8* datap;

	ret = device_get_iomem(device, addr, &addrp);
	if (ret < 0) {
		return -1;
	}
	datap = (uint8*)addrp;
	*data = *datap;
	return 0;
}

int device_io_read16(DeviceType *device, uint32 addr, uint16 *data)
{
	int ret;
	uint32 *addrp;
	uint16* datap;

	ret = device_get_iomem(device, addr, &addrp);
	if (ret < 0) {
		return -1;
	}
	datap = (uint16*)addrp;
	*data = *datap;
	return 0;
}

int device_io_read32(DeviceType *device, uint32 addr, uint32 *data)
{
	int ret;
	uint32 *addrp;
	uint32* datap;

	ret = device_get_iomem(device, addr, &addrp);
	if (ret < 0) {
		return -1;
	}
	datap = (uint32*)addrp;
	*data = *datap;
	return 0;
}


#include "intc.h"

void device_raise_int(DeviceType *device, uint16 intno)
{
	intc_raise_intr(device->cpu, intno);
}


