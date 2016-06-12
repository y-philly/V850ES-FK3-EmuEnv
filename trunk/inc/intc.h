#ifndef _INTC_H_
#define _INTC_H_

#include "cpu.h"
#include "reg.h"

extern void intc_init(CpuManagerType *cpu);
extern int intc_raise_intr(CpuManagerType *cpu, uint32 intno);
extern bool intc_has_pending_intr(CpuManagerType *cpu);
extern int intc_raise_pending_intr(CpuManagerType *cpu);

#define INTC_FECC_NMI				0x10
#define INTC_FECC_INTWDT2			0x20
#define INTC_NMINO_NMI			0
#define INTC_NMINO_INTWDT2		1
extern int intc_raise_nmi(CpuManagerType *cpu, uint32 nmino);


/*
 * �����ݐ�
 * �p�f�B���O���P���邽�߁C���ۂɂ�116�ł��邱�Ƃɒ��Ӂ��D
 * �������ݔԍ�53�܂ł́C�A�����Ă��邪�C
 * ��54�Ԗڂ̓p�f�B���O����Ă��邽�߁C�P����Ă���D
 */
#define INTC_NUM			117
#define INTC_INTNO_MAX		116
#define INTC_PINTNO_NOUSE	54

/*
 * 5.3.4 �����ݐ��䃌�W�X�^(xxICn)
 */
#define INTC_REG_ICN_BASE	0xFFFFF110
#define INTC_ICN_ISSET_IF(data)		( (((data) & (1 << 7)) == (1 << 7)) )
#define INTC_ICN_SET_IF(data)		( ((data) |=   (1 << 7)) )
#define INTC_ICN_CLR_IF(data)		( ((data) &=  ~(1 << 7)) )

#define INTC_ICN_ISSET_MK(data)		( (((data) & (1 << 6)) == (1 << 6)) )
#define INTC_ICN_SET_MK(data)		( ((data) |=  (1 << 6)) )
#define INTC_ICN_CLR_MK(data)		( ((data) &=  ~(1 << 6)) )

#define INTC_ICN_PR(data)			( ((data) & 0x07) )

static inline uint32 intc_intno2off(uint32 intno)
{
	uint32 off;
#if 0
	if (intno <= 53) {
		off = intno;
	}
	else {
		off = intno + 1;
	}
#else
	off = intno;
#endif
	return off;
}
static inline int intc_pintno2intno(int pintno)
{
#if 0
	if (pintno <= 53) {
		return pintno;
	}
	else {
		return pintno - 1;
	}
#else
	return pintno;
#endif
}
static inline uint32 intc_regaddr_icn(uint32 intno)
{
	uint32 regaddr;

	regaddr = INTC_REG_ICN_BASE + (intc_intno2off(intno) * 2);

	return regaddr;
}

/*
 * 5.1 �f�B�t�H�[���g�E�v���C�I���e�B
 */
#define INTC_DEFAULT_PRIORITY(intno)		intc_intno2off((intno))

/*
 * 5.1 ��O�R�[�h
 */
#define INTC_MASK_ECR_CODE_BASE				0x0080
#define INTC_MASK_ECR_CODE(intno)			( INTC_MASK_ECR_CODE_BASE + (intc_intno2off((intno)) *16) )

/*
 * 5.1 �n���h���E�A�h���X
 */
#define INTC_MASK_INTR_ADDR(intno)			INTC_MASK_ECR_CODE((intno))

/*
 * 5.3�D5 �����݃}�X�N�E���W�X�^(IMR0-IMR7)
 */
#define INTC_REG_IMR0	0xFFFFF100
#define INTC_REG_IMR0L	0xFFFFF100
#define INTC_REG_IMR0H	0xFFFFF101

#define INTC_REG_IMR1	0xFFFFF102
#define INTC_REG_IMR1L	0xFFFFF102
#define INTC_REG_IMR1H	0xFFFFF103

#define INTC_REG_IMR2	0xFFFFF104
#define INTC_REG_IMR2L	0xFFFFF104
#define INTC_REG_IMR2H	0xFFFFF105

#define INTC_REG_IMR3	0xFFFFF106
#define INTC_REG_IMR3L	0xFFFFF106
#define INTC_REG_IMR3H	0xFFFFF107

#define INTC_REG_IMR4	0xFFFFF108
#define INTC_REG_IMR4L	0xFFFFF108
#define INTC_REG_IMR4H	0xFFFFF109

#define INTC_REG_IMR5	0xFFFFF10A
#define INTC_REG_IMR5L	0xFFFFF10A
#define INTC_REG_IMR5H	0xFFFFF10B

#define INTC_REG_IMR6	0xFFFFF10C
#define INTC_REG_IMR6L	0xFFFFF10C
#define INTC_REG_IMR6H	0xFFFFF10D

#define INTC_REG_IMR7	0xFFFFF10E

/*
 * 5.3�D6 �C�����C���E�v���C�I���e�B�E���W�X�^(ISPR)
 */
#define INTC_REG_ISPR	0xFFFFF1FA
/*
 * �����݃R���g���[���Ǘ����
 */
#define INTC_NUM_INTLVL		8
#define INTC_MAX_INTLVL		0
#define INTC_MIN_INTLVL		7

typedef struct {
	bool 	intwdt2_hasreq;
	int		nmi_reqnum;
} NmiIntcStatusType;

typedef struct {
	NmiIntcStatusType 		nmi;
	/*
	 * ���ݎ��s���̊��荞�ݔԍ�
	 */
	int current_intno;

	int saved_intno_off;
	int saved_intno_stack[INTC_NUM_INTLVL];

	/*
	 * �����ݑҋ@���̊��荞�݂�����ꍇ�͂��̗D��x���ݒ肳��Ă���
	 * INTC_NUM_INTLVL�̏ꍇ�͑ҋ@�Ȃ��Ɣ��f����D
	 */
	uint8	is_waiting_lvl[INTC_NUM];
	uint32	waiting_lvl_num[INTC_NUM];
	uint32  waiting_int_num;
} IntcControlType;

extern IntcControlType intc_control;

#endif /* _INTC_H_ */
