#include "device/inc/wdg.h"
#include "intc.h"
#include <stdio.h>

typedef struct {
	uint16 state;
	uint32 cnt;
} WdgDeviceType;

static WdgDeviceType WdgDevice;

void device_init_wdg(DeviceType *device)
{
	device->dev.wdg = &WdgDevice;
	WdgDevice.state = 0;
	WdgDevice.cnt = 0;
	return;
}

void device_supply_clock_wdg(DeviceType *device)
{
	uint16 reg16;
	/*
	 * �J�E���g�C���N�������g
	 */
	WdgDevice.cnt++;

	/*
	 * �^�C���A�E�g�m�F
	 */
	if (WdgDevice.cnt >= DVICE_WDG_TIMEOUT) {
		intc_raise_nmi(NULL, INTC_NMINO_NMI);
	}

	/*
	 * �p���X�L���m�F
	 */
	(void)device_io_read16(device, 0xFFFFF404, &reg16);
	reg16 = reg16 & DVICE_WDG_PORT_BIT;
	if (WdgDevice.state != reg16) {
		WdgDevice.cnt = 0;
		WdgDevice.state = reg16;
	}

	return;
}
