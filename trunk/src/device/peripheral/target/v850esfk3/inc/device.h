#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "std_types.h"

typedef struct {
	void *cpu;
	uint64 clock;
	uint64 intclock;//割込み処理で消費している時間

	struct {
		/*
		 * timer object
		 */
		void *timer;

		/*
		 * timer_m object
		 */
		void *timer_m;

		/*
		 * serial object
		 */
		void *serial;
		/*
		 * can object
		 */
		void *can;
		/*
		 * adc object
		 */
		void *adc;
		/*
		 * wdg object
		 */
		void *wdg;
	} dev;
} DeviceType;



/*
 * デバイス初期化関数
 */
extern void device_init(DeviceType *device);
extern void device_init_timer(DeviceType *device);
extern void device_init_timer_m(DeviceType *device);
extern void device_init_serial(DeviceType *device);
extern void device_init_can(DeviceType *device);
extern void device_init_adc(DeviceType *device);
extern void device_init_wdg(DeviceType *device);
extern void device_init_comm(DeviceType *device);

/*
 * デバイスクロック共有
 */
extern void device_supply_clock(DeviceType *device);
extern void device_supply_clock_timer(DeviceType *device);
extern void device_supply_clock_timer_m(DeviceType *device);
extern void device_supply_clock_serial(DeviceType *device);
extern void device_supply_clock_can(DeviceType *device);
extern void device_supply_clock_adc(DeviceType *device);
extern void device_supply_clock_wdg(DeviceType *device);
extern void device_supply_clock_comm(DeviceType *device);

extern int CpuEmuCommDevisProc1;

/*
 * デバイスクロック参照
 */
extern void device_print_clock(DeviceType *device);

/*
 * 割込み関数
 */
extern void device_raise_int(DeviceType *device, uint16 intno);

/*
 * メモリアクセス関数
 */
extern int device_io_write8(DeviceType *device, uint32 addr, uint8 data);
extern int device_io_write16(DeviceType *device, uint32 addr, uint16 data);
extern int device_io_write32(DeviceType *device, uint32 addr, uint32 data);

extern int device_io_read8(DeviceType *device, uint32 addr, uint8 *data);
extern int device_io_read16(DeviceType *device, uint32 addr, uint16 *data);
extern int device_io_read32(DeviceType *device, uint32 addr, uint32 *data);

#endif /* _DEVICE_H_ */
