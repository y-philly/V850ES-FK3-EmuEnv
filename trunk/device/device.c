#include "device/inc/device.h"
#include <stdio.h>

void device_init(DeviceType *device)
{
	device->clock = 0;
	device_init_timer(device);
	device_init_timer_m(device);
	device_init_serial(device);
	device_init_can(device);
	device_init_adc(device);
	device_init_wdg(device);

	return;
}

void device_supply_clock(DeviceType *device)
{
	device->clock++;

	device_supply_clock_timer(device);
	device_supply_clock_timer_m(device);
	device_supply_clock_serial(device);
	device_supply_clock_can(device);
	device_supply_clock_adc(device);
	device_supply_clock_wdg(device);
	return;
}

void device_print_clock(DeviceType *device)
{
	printf("clock = %I64u\n", device->clock);
	return;
}

static int device_get_iomem(DeviceType *device, uint32 addr, uint32 **addrp)
{
	uint32 off;

	/*
	 * �f�o�C�X�������̈�`�F�b�N
	 */
	uint32 devaddr = addr & 0x03FFFFFF;

	if  ((devaddr >= device->io_memory1->addr) &&
			(devaddr <= (device->io_memory1->addr + device->io_memory1->size))) {
		off = devaddr - device->io_memory1->addr;
		*addrp = (uint32*)&device->io_memory1->data[off];
	}
	else if  ((devaddr >= device->io_memory2->addr) &&
			(devaddr <= (device->io_memory2->addr + device->io_memory2->size))) {
		off = devaddr - device->io_memory2->addr;
		*addrp = (uint32*)&device->io_memory2->data[off];
	}
	else {
		/*
		 * invalid memory
		 */
		*addrp = NULL;
		printf("Invalid memory:not found memory(off=0x%x)\n", addr);
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


