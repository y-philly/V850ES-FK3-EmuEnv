#ifndef _ADC_H_
#define _ADC_H_

#include "device/inc/device.h"

/*
 * A/D�R���o�[�^�E���[�h�E���W�X�^ 0�i ADAnM0�j
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
 * A/D�R���o�[�^�E���[�h�E���W�X�^ 1�i ADAnM1�j
 */
#define MPU_ADC_ADDR_AdA0M1					(0xFFFFF201U)

/*
 * A/D�R���o�[�^�E���[�h�E���W�X�^ 2�i ADAnM2�j
 */
#define MPU_ADC_ADDR_AdA0M2					(0xFFFFF203U)

/*
 * A/D�R���o�[�^�E�`���l���w�背�W�X�^ 0�i ADAnS�j
 */
#define MPU_ADC_ADDR_ADA0S					(0xFFFFF202U)

/*
 * A/D�ϊ����ʃ��W�X�^ m
 */
#define MPU_ADC_ADDR_ADA0CRm(pchannel)		((0xFFFFF210U) + ((pchannel) * 2U))

typedef struct {
	bool (*recv) (uint8 ch, uint16 *data);
} DeviceAdcOpType;

extern void device_adc_register_ops(void *adc, DeviceAdcOpType *ops);


#endif /* _ADC_H_ */
