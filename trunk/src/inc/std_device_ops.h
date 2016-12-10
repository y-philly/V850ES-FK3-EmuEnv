#ifndef _STD_DEVICE_OPS_H_
#define _STD_DEVICE_OPS_H_

typedef struct {
	uint64 clock;
	uint64 intclock;//割込み処理で消費している時間
} DeviceClockType;

extern void device_init(CpuType *cpu, DeviceClockType *dev_clock);
extern void device_supply_clock(DeviceClockType *dev_clock);
/*
 * デバイスクロック参照
 */
extern void device_get_clock(DeviceClockType *dev_clock);

#endif /* _STD_DEVICE_OPS_H_ */
