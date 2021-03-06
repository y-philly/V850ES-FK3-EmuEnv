#include "device/inc/serial.h"
#include "cpu.h"
#include "reg.h"
#include <stdio.h>

typedef struct {
	uint16 id;
	uint16 fd;
	uint32 count;
	uint32 bitrate;
	uint32 count_base;
	bool   is_send_data;
	uint8 send_data;
	DeviceSerialOpType *ops;
	DeviceType *device;
} SerialDeviceType;

static SerialDeviceType SerialDevice[UDnChannelNum];
static void serial_set_str(bool enable);
static bool serial_isset_str_ssf(void);
static void serial_set_str_ssf(void);

#define CLOCK_PER_SEC	10000000U	/* 10MHz */

void device_init_serial(DeviceType *device)
{
	int i = 0;

	for (i = 0; i < UDnChannelNum; i++) {
		SerialDevice[i].id = i;
//		SerialDevice[i].fd = 2;
		SerialDevice[i].fd = 1;
		SerialDevice[i].is_send_data = FALSE;
		SerialDevice[i].count = 0;
		SerialDevice[i].bitrate = 38400; /* bit/sec */
//		SerialDevice[i].count_base = CLOCK_PER_SEC / (SerialDevice[i].bitrate / 8);
		SerialDevice[i].count_base = 1;
		SerialDevice[i].ops = NULL;
		SerialDevice[i].device = device;
	}
	device->dev.serial = &SerialDevice;

	return;
}
void device_do_serial(DeviceType *device, SerialDeviceType *serial)
{
	uint8 data;
	bool ret;
	serial->count++;

	if (serial->ops == NULL) {
		return;
	}
	if ((serial->count % serial->count_base) != 0) {
		return;
	}
	/*
	 * 受信データチェック：存在している場合は，割り込みを上げる．
	 */
	if (serial_isset_str_ssf() == FALSE) {
		ret = serial->ops->recv(serial->id, &data);
		if (ret) {
			serial_set_str_ssf();
			//受信データをセットする．
			(void)device_io_write8(device, UDnRX(serial->id), data);
			//受信割込みを上げる
			device_raise_int(device, INTNO_INTUD0R);
		}
	}

	/*
	 * 送信データチェック：存在している場合は，データ転送する．
	 */
	if (serial->is_send_data) {
		ret = serial->ops->send(serial->id, serial->send_data);
		//送信割込みを上げる
		serial_set_str(FALSE);
		device_raise_int(device, INTNO_INTUD0T);
		serial->is_send_data = FALSE;
	}


	return;
}

void device_supply_clock_serial(DeviceType *device)
{
	int i = 0;

	for (i = 0; i < UDnChannelNum; i++) {
		if ((device->clock % SerialDevice[i].fd) != 0) {
			continue;
		}
		device_do_serial(device, &SerialDevice[i]);
	}
	return;
}


void serial_hook_update_reg8(CpuManagerType *cpu, uint32 regaddr, uint8 data)
{
	if (regaddr == UDnTX(UDnCH0)) {
		SerialDevice[UDnCH0].is_send_data = TRUE;
		SerialDevice[UDnCH0].send_data = data;
		serial_set_str(TRUE);
		printf("%c", data);
		fflush(stdout);
	}
}

void device_serial_register_ops(void *serial, uint8 ch, DeviceSerialOpType *ops)
{
	SerialDevice[ch].ops = ops;
	return;
}

static void serial_set_str(bool enable)
{
	uint8 str;
	(void)device_io_read8(SerialDevice[UDnCH0].device, UDnSTR(UDnCH0), &str);
	if (enable) {
		str |= 0x80;
	}
	else {
		str &= ~0x80;
	}
	(void)device_io_write8(SerialDevice[UDnCH0].device, UDnSTR(UDnCH0), str);
	return;
}
static bool serial_isset_str_ssf(void)
{
	uint8 str;
	(void)device_io_read8(SerialDevice[UDnCH0].device, UDnSTR(UDnCH0), &str);
	return ((str & 0x10) == 0x10);
}
static void serial_set_str_ssf(void)
{
	uint8 str;
	(void)device_io_read8(SerialDevice[UDnCH0].device, UDnSTR(UDnCH0), &str);
	str |= 0x10;
	(void)device_io_write8(SerialDevice[UDnCH0].device, UDnSTR(UDnCH0), str);
	return;
}
