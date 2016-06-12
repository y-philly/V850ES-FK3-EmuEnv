#include "device/inc/adc.h"

typedef enum {
	ADC_MODE_STOP,
	ADC_MODE_RUN
} AdcModeType;

typedef enum {
	ADC_CHANNEL_0 = 0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
	ADC_CHANNEL_8,
	ADC_CHANNEL_9,
	ADC_CHANNEL_10,
	ADC_CHANNEL_11,
	ADC_CHANNEL_12,
	ADC_CHANNEL_13,
	ADC_CHANNEL_14,
	ADC_CHANNEL_15,
	ADC_CHANNEL_NUM
} AdcChannelType;

typedef struct {
	uint16 cnt;
	AdcModeType mode;
	uint16 intno;
	uint16 conv_interval_clock; /* clock */
	uint16 adc_data[ADC_CHANNEL_NUM];
	DeviceAdcOpType *ops;
} AdcDeviceType;

static AdcDeviceType AdcDevice;

void device_init_adc(DeviceType *device)
{
	int i;

	device->dev.adc = &AdcDevice;

	AdcDevice.cnt = 0;
	AdcDevice.mode = ADC_MODE_STOP;
	AdcDevice.intno = 42;
	AdcDevice.conv_interval_clock = 160U;

	for (i = 0; i < ADC_CHANNEL_NUM; i++) {
		AdcDevice.adc_data[i] = 0U;
	}
	return;
}

void device_adc_register_ops(void *adc, DeviceAdcOpType *ops)
{
	AdcDevice.ops = ops;
	return;
}

void device_supply_clock_adc(DeviceType *device)
{
	int i;
	uint8 data;

	(void)device_io_read8(device, MPU_ADC_ADDR_AdA0M0, &data);
	if ((data & (1U << MPU_ADC_ADDR_AdA0M0_ADAnCE)) == 0U) {
		/*
		 * AD•ÏŠ·“®ì’âŽ~
		 */
		AdcDevice.mode = ADC_MODE_STOP;
		return;
	}
	if ((data & (1U << MPU_ADC_ADDR_AdA0M0_ADAnPS)) == 0U) {
		/*
		 * AD“dŒ¹OFF
		 */
		AdcDevice.mode = ADC_MODE_STOP;
		return;
	}
	if (AdcDevice.mode == ADC_MODE_STOP) {
		AdcDevice.mode = ADC_MODE_RUN;
		(void)device_io_write8(device, MPU_ADC_ADDR_AdA0M0, (data | MPU_ADC_ADDR_AdA0M0_ADAnEF));
	}
	if (AdcDevice.cnt < AdcDevice.conv_interval_clock) {
		AdcDevice.cnt++;
		return;
	}


	if (AdcDevice.ops != NULL) {
		for (i = 0; i < ADC_CHANNEL_NUM; i++) {
			AdcDevice.ops->recv(i, &AdcDevice.adc_data[i]);
			(void)device_io_write16(device, MPU_ADC_ADDR_ADA0CRm(i), AdcDevice.adc_data[i]);
		}
	}

	data &= ~(1U << MPU_ADC_ADDR_AdA0M0_ADAnEF);
	data &= ~(1U << MPU_ADC_ADDR_AdA0M0_ADAnCE);
	(void)device_io_write8(device, MPU_ADC_ADDR_AdA0M0, data);
	AdcDevice.cnt = 0;

	device_raise_int(device, AdcDevice.intno);
	return;
}
