#include "cpu.h"
#include "op_exec_ops.h"

/*
 * [�����Ȃ������Z]
 * ���Z�̏ꍇ A+B=D
 *  ��D<A�܂���D<B�̂Ƃ��L�����[����
 * ���Z�̏ꍇ A-B=D
 *  ��A<B�̏ꍇ�{���[����
 *
 *  �I�[�o�[�t���[�t���O(OF) �F �������ǂ����̉��Z���ʂ��ٕ���
 *  CF = ( A & B ) | ( A & CY) | ( B & CY )
 *�@�@where A=�I�y�����h1��MSB, B=�I�y�����h2��MSB, CY=MSB�ւ̌��オ��
 *
 *�@�@�I�[�o�[�t���[�t���O�̈Ӗ��́A���̉��Z���Ɣ퉉�Z���y�щ��Z���ʂ𕄍��t���Q�i���ƍl
 *  �������A���ʂ����r�b�g�̕����t���Q�i���ŕ\���ł���͈͂𒴂��Ă���Ƃ����Ӗ��ł���B
 *
 *  �L�����[�t���O(CF) �F MSB����̌��オ��
 *  OF = ( A & B & (~C) ) | ( (~A) & (~B) & C )
 *  where A=�I�y�����h1��MSB, B=�I�y�����h2��MSB, C=���Z���ʂ�MSB
 *
 *  ���̉��Z���Ɣ퉉�Z���𕄍������Q�i���ƍl���A���ʂ����������Q�i���Ƃ��đ�����ꍇ�A�����L
�@*  �����[�t���O���P�ɂȂ��Ă�����A����́u�����v�܂��́u��̌�����̎؂�v���������A���ʂ̂��r�b
�@*  �g�����Ő��w�I�Ȍ��ʂ��\���Ă��Ȃ����Ƃ��Ӗ�����B
 */

/*
 *  ���̉��Z���Ɣ퉉�Z���𕄍������Q�i���ƍl���A���ʂ����������Q�i���Ƃ��đ�����ꍇ�A�����L
�@*  �����[�t���O���P�ɂȂ��Ă�����A����́u�����v�܂��́u��̌�����̎؂�v���������A���ʂ̂��r�b
�@*  �g�����Ő��w�I�Ȍ��ʂ��\���Ă��Ȃ����Ƃ��Ӗ�����B
 */
int op_chk_and_set_carry(CpuRegisterType *cpu, uint32 a32, uint32 b32)
{
	uint64 tmp1 = a32;
	uint64 tmp2 = b32;
	uint64 result = tmp1 + tmp2;

	if (result > CPU_REG_UINT_MAX) {
		//printf("SET_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_SET_CY(cpu);
	}
	else {
		//printf("CLR_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_CLR_CY(cpu);
	}
	return 0;
}
int op_chk_and_set_borrow(CpuRegisterType *cpu, uint32 a32, uint32 b32)
{
	uint64 tmp1 = a32;
	uint64 tmp2 = b32;
	uint64 result = tmp1 - tmp2;

	if (result & 0x100000000ULL) {
		//printf("SET_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_SET_CY(cpu);
	}
	else {
		//printf("CLR_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_CLR_CY(cpu);
	}
	return 0;
}

/*
 *�@�@�I�[�o�[�t���[�t���O�̈Ӗ��́A���̉��Z���Ɣ퉉�Z���y�щ��Z���ʂ𕄍��t���Q�i���ƍl
 *  �������A���ʂ����r�b�g�̕����t���Q�i���ŕ\���ł���͈͂𒴂��Ă���Ƃ����Ӗ��ł���B
 */
int op_chk_and_set_overflow(CpuRegisterType *cpu, sint64 a64, sint64 b64)
{
	sint64 result = a64 + b64;

	if (result > (sint64)CPU_REG_PLUS_MAX) {
		CPU_SET_OV(cpu);
	}
	else if (result < (sint64)CPU_REG_MINUS_MAX) {
		CPU_SET_OV(cpu);
	}
	else {
		CPU_CLR_OV(cpu);
	}
	return 0;
}
int op_chk_and_set_zero(CpuRegisterType *cpu, sint32 result)
{
	if (result == 0) {
		CPU_SET_Z(cpu);
	}
	else {
		CPU_CLR_Z(cpu);
	}
	return 0;
}
int op_chk_and_set_sign(CpuRegisterType *cpu, sint32 result)
{
	if (result < 0) {
		CPU_SET_S(cpu);
	}
	else {
		CPU_CLR_S(cpu);
	}
	return 0;
}

/*
 * Format1
 */
int op_exec_sub(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	reg1_data = cpu->cpu.r[reg1];
	result = reg2_data - reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SUB r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = result;


	op_chk_and_set_borrow(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_overflow(&cpu->cpu, reg2_data, -reg1_data);
	op_chk_and_set_zero(&cpu->cpu, result);
	op_chk_and_set_sign(&cpu->cpu, result);

	cpu->cpu.pc += 2;
	return 0;
}

int op_exec_subr(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	reg1_data = cpu->cpu.r[reg1];
	result = reg1_data - reg2_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SUBR r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = result;

	op_chk_and_set_borrow(&cpu->cpu, reg1_data, reg2_data);
	op_chk_and_set_overflow(&cpu->cpu, reg1_data, -reg2_data);
	op_chk_and_set_zero(&cpu->cpu, result);
	op_chk_and_set_sign(&cpu->cpu, result);

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_cmp_1(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	reg1_data = cpu->cpu.r[reg1];
	result = reg2_data - reg1_data;

	op_chk_and_set_borrow(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_overflow(&cpu->cpu, reg2_data, -reg1_data);
	op_chk_and_set_zero(&cpu->cpu, result);
	op_chk_and_set_sign(&cpu->cpu, result);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: CMP r%d(%d),r%d(%d):%d:psw=0x%x\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result, cpu->cpu.psw));

	cpu->cpu.pc += 2;
	return 0;
}

int op_exec_add_1(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	reg1_data = cpu->cpu.r[reg1];
	result = reg2_data + reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADD r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	op_chk_and_set_carry(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_overflow(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_zero(&cpu->cpu, result);
	op_chk_and_set_sign(&cpu->cpu, result);

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_mov_1(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOV r%d(%d),r%d(%d)\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2]));

	cpu->cpu.r[reg2] = cpu->cpu.r[reg1];

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_mulh_1(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint16 tmp1;
	sint16 tmp2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	tmp1 = (sint16)cpu->cpu.r[reg1];
	tmp2 = (sint16)cpu->cpu.r[reg2];

	result = (sint32)tmp1 * (sint32)tmp2;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULH r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;
}
/*
 * Format2
 */
int op_exec_cmp_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	sint32 imm_data;
	sint32 reg2_data;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code.type2.imm);
	result = reg2_data - imm_data;

	op_chk_and_set_borrow(&cpu->cpu, reg2_data, imm_data);
	op_chk_and_set_overflow(&cpu->cpu, reg2_data, -imm_data);
	op_chk_and_set_zero(&cpu->cpu, result);
	op_chk_and_set_sign(&cpu->cpu, result);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: CMP imm5(%d),r%d(%d):%d:psw=0x%x\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result, cpu->cpu.psw));

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_add_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	sint32 imm_data;
	sint32 reg2_data;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code.type2.imm);
	result = reg2_data + imm_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADD imm5(%d),r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;
	op_chk_and_set_carry(&cpu->cpu, reg2_data, imm_data);
	op_chk_and_set_overflow(&cpu->cpu, reg2_data, imm_data);
	op_chk_and_set_zero(&cpu->cpu, result);
	op_chk_and_set_sign(&cpu->cpu, result);

	cpu->cpu.pc += 2;

	return 0;
}
int op_exec_mov_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	sint32 imm_data;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code.type2.imm);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOV imm5(%d),r%d(%d)\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2]));
	cpu->cpu.r[reg2] = imm_data;

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_mulh_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	sint32 imm_data;
	sint16 tmp2;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code.type2.imm);
	tmp2 = (sint16)cpu->cpu.r[reg2];
	result = imm_data * ((sint32)tmp2);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULH imm5(%d),r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;
}

/*
 * Format6
 */
int op_exec_addi(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_addi(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADDI imm5(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}
int op_exec_movea(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_movea(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOVEA imm16(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}
int op_exec_movhi(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_movhi(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOVHI imm16(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}

int op_exec_mulhi(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_mulhi(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULHI imm16(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}

/*
 * Format9
 */
static int setf_chk_cond(CpuManagerType *cpu, uint16 cond)
{
	int ret = FALSE;
	uint16 flg_s = CPU_ISSET_S(&cpu->cpu);
	uint16 flg_ov = CPU_ISSET_OV(&cpu->cpu);
	uint16 flg_z = CPU_ISSET_Z(&cpu->cpu);
	uint16 flg_cy = CPU_ISSET_CY(&cpu->cpu);
	uint16 flg_sat = CPU_ISSET_SAT(&cpu->cpu);

	switch (cond) {
	case 0b0000: //V
		if (flg_ov == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0001: //C/L
		if (flg_cy == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0010: //Z
		if (flg_z == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0011: //NH
		if ((flg_cy | flg_z) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0100: //S/N
		if ((flg_s) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0101: //T
		ret = TRUE;
		break;
	case 0b0110: //LT
		if ((flg_s ^ flg_ov) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0111: //LE
		if (((flg_s ^ flg_ov) | flg_z) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b1000: //NV
		if (flg_ov == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1001: //NC/NL
		if (flg_cy == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1010: //NZ
		if (flg_z == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1011: //H
		if ((flg_cy | flg_z) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1100: //NS/P
		if ((flg_s) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1101: //SA
		if ((flg_sat) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1110: //GE
		if ((flg_s ^ flg_ov) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1111: //GT
		if (((flg_s ^ flg_ov) | flg_z) == FALSE) {
			ret = TRUE;
		}
		break;
	default:
		break;
	}

	return ret;
}
int op_exec_setf(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type9.reg2;
	uint16 cond = cpu->decoded_code.type9.gen;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	if (setf_chk_cond(cpu, cond) == TRUE) {
		result = 1;
	}
	else {
		result = 0;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SETF cond(0x%x),r%d(%d):%d\n", cpu->cpu.pc, cond, reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;

	return 0;
}
