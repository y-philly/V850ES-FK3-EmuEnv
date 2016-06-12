#ifndef _TIMER_H_
#define _TIMER_H_

#include "device/inc/device.h"

/*************************************************
 * 16�r�b�g�^�C�}�^�C�x���g�J�E���^AA(TAA)
 *************************************************/

#define TAAnChannelNum			UINT_C(8)
#define TAAnCH0					UINT_C(0)
#define TAAnCH1					UINT_C(1)
#define TAAnCH2					UINT_C(2)
#define TAAnCH3					UINT_C(3)
#define TAAnCH4					UINT_C(4)
#define TAAnCH5					UINT_C(5)
#define TAAnCH6					UINT_C(6)
#define TAAnCH7					UINT_C(7)

/*
 * TAAn���䃌�W�X�^0
 */
#define TAAnCTL0_BASE			UINT_C(0xFFFFF590)
#define TAAnCTL0(CH)			(TAAnCTL0_BASE + ((CH) * 16U))
/*
 * TAAn���䃌�W�X�^1
 */
#define TAAnCTL1_BASE			UINT_C(0xFFFFF591)
#define TAAnCTL1(CH)			(TAAnCTL1_BASE + ((CH) * 16U))


/*
 * TAAn �L���v�`���^�R���y�A�E���W�X�^ 0�i TAAnCCR0�j
 */
#define TAAnCCR0_BASE			UINT_C(0xFFFFF596)
#define TAAnCCR0(CH)			(TAAnCCR0_BASE + ((CH) * 16U))

/*
 * TAAn �L���v�`���^�R���y�A�E���W�X�^ 1�i TAAnCCR1�j
 */
#define TAAnCCR1_BASE			UINT_C(0xFFFFF598)
#define TAAnCCR1(CH)			(TAAnCCR1_BASE + ((CH) * 16U))

/*
 * TAAn�J�E���^�E���[�h�E�o�b�t�@�E���W�X�^
 */
#define TAAnCNT_BASE			UINT_C(0xFFFFF59A)
#define TAAnCNT(CH)				(TAAnCNT_BASE + ((CH) * 16U))

/*
 * TAAn �I�v�V�����E���W�X�^ 0�i TAAnOPT0�j
 */
#define TAAnOPT0_BASE			UINT_C(0xFFFFF595)
#define TAAnOPT0(CH)			(TAAnOPT0_BASE + ((CH) * 16U))

/*
 * TAAn �I�v�V�����E���W�X�^ 1�i TAAnOPT1�j
 */
#define TAA1OPT1				UINT_C(0xFFFFF5AD)
#define TAA3OPT1				UINT_C(0xFFFFF5CD)
#define TAA6OPT1				UINT_C(0xFFFFF5FD)



#endif /* _TIMER_H_ */
