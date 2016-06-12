#ifndef _ADC_H_
#define _ADC_H_

#include "device/inc/device.h"

/*
 * A/Dコンバータ・モード・レジスタ 0（ ADAnM0）
 */
#define MPU_ADC_ADDR_AdA0M0					(0xFFFFF200U)

#define MPU_ADC_ADDR_AdA0M0_ADAnCE			(7U)
#define MPU_ADC_ADDR_AdA0M0_ADAnPS			(6U)
#define MPU_ADC_ADDR_AdA0M0_ADAnMD1			(5U)
#define MPU_ADC_ADDR_AdA0M0_ADAnMD0			(4U)
#define MPU_ADC_ADDR_AdA0M0_ADAnETS1		(3U)
#define MPU_ADC_ADDR_AdA0M0_ADAnETS0		(2U)
#define MPU_ADC_ADDR_AdA0M0_ADAnTMD			(1U)
#define MPU_ADC_ADDR_AdA0M0_ADAnEF			(0U)

/*
 * A/Dコンバータ・モード・レジスタ 1（ ADAnM1）
 */
#define MPU_ADC_ADDR_AdA0M1					(0xFFFFF201U)

/*
 * A/Dコンバータ・モード・レジスタ 2（ ADAnM2）
 */
#define MPU_ADC_ADDR_AdA0M2					(0xFFFFF203U)

/*
 * A/Dコンバータ・チャネル指定レジスタ 0（ ADAnS）
 */
#define MPU_ADC_ADDR_ADA0S					(0xFFFFF202U)

/*
 * A/D変換結果レジスタ m
 */
#define MPU_ADC_ADDR_ADA0CRm(pchannel)		((0xFFFFF210U) + ((pchannel) * 2U))

typedef struct {
	bool (*recv) (uint8 ch, uint16 *data);
} DeviceAdcOpType;

extern void device_adc_register_ops(void *adc, DeviceAdcOpType *ops);


#endif /* _ADC_H_ */
