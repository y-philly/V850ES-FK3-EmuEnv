#include "op_dec.h"

typedef struct {
	int (*decode) (uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
} OpDecoderType;


static int OpDecode1(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode2(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode3(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode4(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode5(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode6(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode7(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode8(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode9(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode10(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode11(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode12(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static int OpDecode13(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code);
static OpDecoderType OpDecoder[OP_CODE_FORMAT_NUM] = {
	{ OpDecode1 },
	{ OpDecode2 },
	{ OpDecode3 },
	{ OpDecode4 },
	{ OpDecode5 },
	{ OpDecode6 },
	{ OpDecode7 },
	{ OpDecode8 },
	{ OpDecode9 },
	{ OpDecode10 },
	{ OpDecode11 },
	{ OpDecode12 },
	{ OpDecode13 },
};

/*
 *
 */
int OpDecode(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint16 opcode =  ( (code[0] >> 5) & 0x3F );
	uint16 subcode = ( (code[1] >> 5) & 0x3F );
	OpCodeFormatId id;

	//printf("code0=0x%x\n", code[0]);
	//printf("code1=0x%x\n", code[1]);
	//printf("opcode=0x%x\n", opcode);
	id = OpCode2FormatId(opcode, subcode);
	if (id == OP_CODE_FORMAT_UNKNOWN) {
		return -1;
	}
	if (id == OP_CODE_FORMAT_5) {
		/*
		 * この時点でFORMAT5と判断された場合，
		 * FORMAT7 or FORMAT13判定が必要となる．
		 */
		/*
		 * 16bit != 0 の場合はFORMAT7 or FORMAT13
		 */
		if ((code[1] & 0x0001) != 0) {
			/*
			 * reg2 != 0 の場合はFORMAT7
			 * reg2 == 0 の場合はFORMAT13
			 */
			uint16 reg2 = (code[0] >> 11U);
			if (reg2 != 0U) {
				id = OP_CODE_FORMAT_7;
			}
			else {
				id = OP_CODE_FORMAT_13;
			}
		}
	}
	else if (id == OP_CODE_FORMAT_6) {
		/*
		 * この時点でFORMAT6と判断された場合，
		 * FORMAT13判定が必要となる．
		 */
		/*
		 * opcode=0b11001X かつ　reg2==0 の場合はFORMAT13
		 */
		uint16 opcode5 = opcode >> 1U;
		uint16 reg2 = (code[0] >> 11U);
		if ((opcode5 == 0b1101) && reg2 == 0U) {
			id = OP_CODE_FORMAT_13;
		}
	}
	decoded_code->type_id = id;
	return OpDecoder[id].decode(code, decoded_code);
}
static int OpDecode1(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	decoded_code->type1.opcode = ( (code[0] >> 5) & 0x003F );
	decoded_code->type1.reg1   = ( (code[0]) & 0x001F );
	decoded_code->type1.reg2   = ( (code[0] >> 11) & 0x001F );
	return 0;
}
static int OpDecode2(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	decoded_code->type2.opcode = ( (code[0] >> 5) & 0x003F );
	decoded_code->type2.imm    = ( (code[0]) & 0x001F );
	decoded_code->type2.reg2   = ( (code[0] >> 11) & 0x001F );
	return 0;
}

static int OpDecode3(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp1 =  ( (code[0] >> 11) & 0x001F ); /* 5bits */
	uint32 tmp2 =  ( (code[0] >>  4) & 0x0007 ); /* 3bits */

	decoded_code->type3.opcode = ( (code[0] >> 7) & 0x000F );
	decoded_code->type3.disp   = ( (tmp1 << 3) | (tmp2) );
	decoded_code->type3.cond   = ( (code[0]) & 0x000F );
	return 0;
}

static int OpDecode4(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	/* 4_1 */
	decoded_code->type4_1.opcode = ( (code[0] >> 7)  & 0x000F );
	decoded_code->type4_1.disp   = ( (code[0] >> 1)  & 0x3F );
	decoded_code->type4_1.reg2   = ( (code[0] >> 11) & 0x001F );
	decoded_code->type4_1.gen    = ( (code[0] >>  0) & 0x0001 );

	/* 4_2 */
	decoded_code->type4_2.opcode = ( (code[0] >> 4)  & 0x007F );
	decoded_code->type4_2.disp   = ( (sint8)(code[0] >> 0)  & 0x0F );
	decoded_code->type4_2.reg2   = ( (code[0] >> 11) & 0x001F );

	return 0;
}

static int OpDecode5(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp1 = ( (code[0] >> 0) & 0x003F ); /* 6bits */;
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type5.opcode = ( (code[0] >> 6)  & 0x001F );
	decoded_code->type5.reg2   = ( (code[0] >> 11) & 0x001F );
	decoded_code->type5.disp = ( (tmp1 << 16) | tmp2 );
	return 0;
}

static int OpDecode6(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type6.opcode = ( (code[0] >> 5)  & 0x003F );
	decoded_code->type6.reg2   = ( (code[0] >> 11) & 0x001F );
	decoded_code->type6.reg1   = ( code[0] & 0x001F );
	decoded_code->type6.imm   = tmp2;

	return 0;
}

static int OpDecode7(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type7.opcode = ( (code[0] >> 5)  & 0x003F );
	decoded_code->type7.reg2   = ( (code[0] >> 11) & 0x001F );
	decoded_code->type7.reg1   = ( code[0] & 0x001F );
	decoded_code->type7.disp   = ( (tmp2 >> 1) & 0x7FFF );
	decoded_code->type7.gen    = ( tmp2 & 0x0001 );

	return 0;
}

static int OpDecode8(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type8.opcode = ( (code[0] >> 5)  & 0x003F );
	decoded_code->type8.sub   = ( (code[0] >> 14) & 0x0003 );
	decoded_code->type8.bit   = ( (code[0] >> 11) & 0x0007 );
	decoded_code->type8.reg1   = ( code[0] & 0x001F );
	decoded_code->type8.disp   = tmp2;

	return 0;
}

static int OpDecode9(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint16 tmp1 = ( (code[0] >> 0) & 0xFFFF );/* 16bits */;
	uint16 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type9.opcode = ( (tmp1 >> 5)  & 0x003F );
	decoded_code->type9.reg2   = ( (tmp1 >> 11) & 0x001F );
	decoded_code->type9.gen    = ( tmp1 & 0x001F );
	decoded_code->type9.rfu1   = ( (tmp2 >>  1) & 0x000F );
	decoded_code->type9.rfu2   = ( (tmp2 >> 11) & 0x001F );
	decoded_code->type9.sub    = ( (tmp2 >>  5) & 0x003F );

	return 0;
}

static int OpDecode10(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type10.opcode = ( (code[0] >> 5)  & 0x003F );
	decoded_code->type10.rfu1   = ( (code[0] >> 11) & 0x0003 );
	decoded_code->type10.gen1   = ( (code[0] >> 13) & 0x0007 );
	decoded_code->type10.gen2   = ( (code[0] >> 0)  & 0x001F );

	decoded_code->type10.rfu2   = ( (tmp2 >> 11) & 0x001F );
	decoded_code->type10.rfu3   = ( (tmp2 >>  1) & 0x000F );
	decoded_code->type10.sub    = ( (tmp2 >>  5) & 0x003F );

	return 0;
}

static int OpDecode11(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type11.opcode = ( (code[0] >> 5)  & 0x003F );
	decoded_code->type11.reg2   = ( (code[0] >> 11) & 0x001F );
	decoded_code->type11.reg1   = ( (code[0] >>  0) & 0x001F );

	decoded_code->type11.rfu    = ( (tmp2 >>  2) & 0x0007 );
	decoded_code->type11.reg3   = ( (tmp2 >> 11) & 0x001F );
	decoded_code->type11.sub1   = ( (tmp2 >>  5) & 0x003F );
	decoded_code->type11.sub2   = ( (tmp2 >>  1) & 0x0001 );

	return 0;
}

static int OpDecode12(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;

	decoded_code->type12.opcode = ( (code[0] >> 5)  & 0x003F );
	decoded_code->type12.reg2   = ( (code[0] >> 11) & 0x001F );
	decoded_code->type12.imml    = ( (code[0] >>  0) & 0x001F );

	decoded_code->type12.reg3   = ( (tmp2 >> 11) & 0x001F );
	decoded_code->type12.sub1   = ( (tmp2 >>  7) & 0x000F );
	decoded_code->type12.sub2   = ( (tmp2 >>  1) & 0x0001 );
	decoded_code->type12.immh   = ( (tmp2 >>  2) & 0x001F );
	return 0;
}

static int OpDecode13(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code)
{
	uint32 tmp2 = ( (code[1] >> 0) & 0xFFFF );/* 16bits */;
	uint16 t1 = ((code[0]) & 0x0001);
	uint16 t2 = ((code[1] >> 5) & 0x07FF);

	decoded_code->type13.opcode = ( (code[0] >> 6)  & 0x001F );
	decoded_code->type13.rfu    = ( (code[0] >> 11) & 0x001F );
	decoded_code->type13.imm    = ( (code[0] >>  1) & 0x001F );

	decoded_code->type13.gen   = ( (tmp2 >> 0) & 0x001F );
	decoded_code->type13.list   = ( t1 << 11 ) | t2;
	return 0;
}

