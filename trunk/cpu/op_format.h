#ifndef _OP_FORMAT_H_
#define _OP_FORMAT_H_

#include "std_types.h"

#define OP_CODE_FORMAT_NUM	13
typedef enum {
	OP_CODE_FORMAT_1 = 0,
	OP_CODE_FORMAT_2,
	OP_CODE_FORMAT_3,
	OP_CODE_FORMAT_4,
	OP_CODE_FORMAT_5,
	OP_CODE_FORMAT_6,
	OP_CODE_FORMAT_7,
	OP_CODE_FORMAT_8,
	OP_CODE_FORMAT_9,
	OP_CODE_FORMAT_10,
	OP_CODE_FORMAT_11,
	OP_CODE_FORMAT_12,
	OP_CODE_FORMAT_13,
	OP_CODE_FORMAT_UNKNOWN,
} OpCodeFormatId;

typedef struct {
	OpCodeFormatId id;
} OpCode2FormatType;

#define OP_CODE_NUM		(16)
#define OP_CODE_NUM		(16)

extern OpCodeFormatId OpCode2FormatId(uint8 opcode, uint8 subcode);



/*
 * reg-reg���ߌ`��
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType1;
/*
 * imm-reg���ߌ`��
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint32 imm;		/* 4-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType2;
/*
 * �������򖽗ߌ`��
 */
typedef struct {
	uint16 opcode;	/* 10-7 */
	uint32 disp;	/* 15-11, 6-4 */
	uint16 cond;	/* 3-0 */
} OpCodeFormatType3;
/*
 * ���[�h�^�X�g�A����16�r�b�g�`��
 */
typedef struct {
	uint16 opcode;	/* 10-7 */
	uint32 disp;	/* 6-1 */
	uint16 reg2;	/* 15-11 */
	uint32 gen;		/* 0 */
} OpCodeFormatType4_1;
typedef struct {
	uint16 opcode;	/* 10-4 */
	uint32 disp;	/* 3-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType4_2;

/*
 * �W�����v���ߌ`��
 */
typedef struct {
	uint16 opcode;	/* 10-6 */
	uint32 disp;	/* 5-0, 31-17 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType5;
/*
 * 3�I�y�����h���ߌ`��
 */
typedef struct {
	uint16 opcode;	/* 10-4 */
	uint32 imm;		/* 31-16 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType6;

/*
 * ���[�h�^�X�g�A����32�r�b�g�`��
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint32 disp;	/* 31-17 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
	uint32 gen;		/* 16 */
} OpCodeFormatType7;

/*
 * �r�b�g���얽�ߌ`��
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 sub;		/* 15-14 */
	uint16 bit;		/* 13-11 */
	uint16 reg1;	/* 4-0 */
	uint32 disp;	/* 31-16 */
} OpCodeFormatType8;
/*
 * �g�����ߌ`��1
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 sub;		/* 26-21 */
	uint16 gen;		/* 4-0 */
	uint16 reg2;	/* 15-11 */
	uint16 rfu2;		/* 31-27 */
	uint16 rfu1;		/* 20-17 */
} OpCodeFormatType9;
/*
 * �g�����ߌ`��2
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 gen1;	/* 15-13 */
	uint32 gen2;	/* 4-0 */
	uint16 sub;		/* 26-21 */
	uint16 rfu1;		/* 12-11*/
	uint16 rfu2;		/* 20-17 */
	uint16 rfu3;		/* 31-27 */
} OpCodeFormatType10;
/*
 * �g�����ߌ`��3
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
	uint16 reg3;	/* 31-27 */
	uint16 sub1;	/* 26-21 */
	uint16 sub2;	/* 17 */
	uint16 rfu;		/* 20-18 */
} OpCodeFormatType11;
/*
 * �g�����ߌ`��4
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 reg2;	/* 15-11 */
	uint16 reg3;	/* 31-27 */
	uint32 imml;		/* 4-0 */
	uint32 immh;		/* 22-18 */
	uint16 sub1;	/* 26-23 */
	uint16 sub2;	/* 17 */
} OpCodeFormatType12;
/*
 * �X�^�b�N���얽�ߌ`��1
 */
typedef struct {
	uint16 opcode;	/* 10-6 */
	uint32 imm;		/* 5-1 */
	uint16 list;	/* 0, 31-21 */
	uint16 gen;		/* 20-16 */
	uint16 rfu;		/* 15-11 */
} OpCodeFormatType13;
#endif /* _OP_FORMAT_H_ */
