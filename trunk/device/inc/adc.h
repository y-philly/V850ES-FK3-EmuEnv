#ifndef _ADC_H_
#define _ADC_H_

#include "device/inc/device.h"

/*
 * A/D�R���o�[�^
 */
#define MPU_ADC_ADA_NUM		(2U)
#define MPU_ADC_ADA0		(0U)
#define MPU_ADC_ADA1		(1U)

/*
 * A/D�R���o�[�^�E���[�h�E���W�X�^ 0�i ADAnM0�j
 */
#define MPU_ADC_ADDR_AdAnM0_BASE			(0xFFFFF200U)
#define MPU_ADC_ADDR_AdAnM0(cntl)			(MPU_ADC_ADDR_AdAnM0_BASE + ((cntl) * 0x40))

#define MPU_ADC_ADDR_AdAnM0_ADAnCE			(7U)
#define MPU_ADC_ADDR_AdAnM0_ADAnPS			(6U)
#define MPU_ADC_ADDR_AdAnM0_ADAnMD1			(5U)
#define MPU_ADC_ADDR_AdAnM0_ADAnMD0			(4U)
#define MPU_ADC_ADDR_AdAnM0_ADAnETS1		(3U)
#define MPU_ADC_ADDR_AdAnM0_ADAnETS0		(2U)
#define MPU_ADC_ADDR_AdAnM0_ADAnTMD			(1U)
#define MPU_ADC_ADDR_AdAnM0_ADAnEF			(0U)

/*
 * A/D�R���o�[�^�E���[�h�E���W�X�^ 1�i ADAnM1�j
 */
#define MPU_ADC_ADDR_AdAnM1_BASE			(0xFFFFF201U)
#define MPU_ADC_ADDR_AdAnM1(cntl)			(MPU_ADC_ADDR_AdAnM1_BASE + ((cntl) * 0x40))

/*
 * A/D�R���o�[�^�E���[�h�E���W�X�^ 2�i ADAnM2�j
 */
#define MPU_ADC_ADDR_AdAnM2_BASE			(0xFFFFF203U)
#define MPU_ADC_ADDR_AdAnM2(cntl)			(MPU_ADC_ADDR_AdAnM2_BASE + ((cntl) * 0x40))

/*
 * A/D�R���o�[�^�E�`���l���w�背�W�X�^ 0�i ADAnS�j
 */
#define MPU_ADC_ADDR_ADAnS_BASE				(0xFFFFF202U)
#define MPU_ADC_ADDR_ADAnS(cntl)			(MPU_ADC_ADDR_AdAnS_BASE + ((cntl) * 0x40))
/*
 * A/D�ϊ����ʃ��W�X�^ m
 */
#define MPU_ADC_ADDR_ADAnCRm_BASE				(0xFFFFF210U)
#define MPU_ADC_ADDR_ADAnCRm(cntl,pchannel)		((MPU_ADC_ADDR_ADAnCRm_BASE + ((cntl) * 0x2)) + ((pchannel) * 2U))

typedef struct {
	bool (*recv) (uint8 ch, uint16 *data);
} DeviceAdcOpType;

extern void device_adc_register_ops(void *adc, DeviceAdcOpType *ops);


#endif /* _ADC_H_ */
