#ifndef _TIMER_M_H_
#define _TIMER_M_H_

#include "device/inc/device.h"

/*
 * TMM0�R���y�A�E���W�X�^ 0�i TM0CMP0�j
 */
#define MPU_TMM_ADDR_TM0CMP0				(0xFFFFF694U)

/*
 * TMM0���䃌�W�X�^ 0�i TM0CTL0�j
 */
#define MPU_TMM_ADDR_TM0CTL0				(0xFFFFF690U)

#define MPU_TMM_ADDR_TM0CTL0_TM0CE			(7U)
#define MPU_TMM_ADDR_TM0CTL0_TM0CKS2		(2U)
#define MPU_TMM_ADDR_TM0CTL0_TM0CKS1		(1U)
#define MPU_TMM_ADDR_TM0CTL0_TM0CKS0		(0U)

#endif /* _TIMER_M_H_ */
