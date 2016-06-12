#include "cpu_types.h"

#include "op_exec.h"
#include "op_code.h"
#include "op_exec_ops.h"

static int OpExec1(CpuManagerType *cpu);
static int OpExec2(CpuManagerType *cpu);
static int OpExec3(CpuManagerType *cpu);
static int OpExec4(CpuManagerType *cpu);
static int OpExec5(CpuManagerType *cpu);
static int OpExec6(CpuManagerType *cpu);
static int OpExec7(CpuManagerType *cpu);
static int OpExec8(CpuManagerType *cpu);
static int OpExec9(CpuManagerType *cpu);
static int OpExec10(CpuManagerType *cpu);
static int OpExec11(CpuManagerType *cpu);
static int OpExec12(CpuManagerType *cpu);
static int OpExec13(CpuManagerType *cpu);

int OpExec(CpuManagerType *cpu)
{
	int ret = -1;
	switch (cpu->decoded_code.type_id) {
	case OP_CODE_FORMAT_1:
		ret = OpExec1(cpu);
		break;
	case OP_CODE_FORMAT_2:
		ret = OpExec2(cpu);
		break;
	case OP_CODE_FORMAT_3:
		ret = OpExec3(cpu);
		break;
	case OP_CODE_FORMAT_4:
		ret = OpExec4(cpu);
		break;
	case OP_CODE_FORMAT_5:
		ret = OpExec5(cpu);
		break;
	case OP_CODE_FORMAT_6:
		ret = OpExec6(cpu);
		break;
	case OP_CODE_FORMAT_7:
		ret = OpExec7(cpu);
		break;
	case OP_CODE_FORMAT_8:
		ret = OpExec8(cpu);
		break;
	case OP_CODE_FORMAT_9:
		ret = OpExec9(cpu);
		break;
	case OP_CODE_FORMAT_10:
		ret = OpExec10(cpu);
		break;
	case OP_CODE_FORMAT_11:
		ret = OpExec11(cpu);
		break;
	case OP_CODE_FORMAT_12:
		ret = OpExec12(cpu);
		break;
	case OP_CODE_FORMAT_13:
		ret = OpExec13(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec1(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type1.opcode) {
	case OP_CODE_ADD_1:
		ret = op_exec_add_1(cpu);
		break;
	case OP_CODE_AND:
		ret = op_exec_and(cpu);
		break;
	case OP_CODE_CMP_1:
		ret = op_exec_cmp_1(cpu);
		break;
	case OP_CODE_JMP:
		ret = op_exec_jmp(cpu);
		break;
	case OP_CODE_MOV_1:
		if (cpu->decoded_code.type1.reg2 > 0) {
			ret = op_exec_mov_1(cpu);
		}
		else {
			ret = op_exec_nop(cpu);
		}
		break;
	case OP_CODE_MULH_1:
		if (cpu->decoded_code.type1.reg2 > 0) {
			//OP_CODE_MULH_1
			ret = op_exec_mulh_1(cpu);
		}
		else {
			//OP_CODE_SXH
			ret = op_exec_sxh(cpu);
		}
		break;
	case OP_CODE_NOT:
		ret = op_exec_not(cpu);
		break;
	case OP_CODE_SATADD_1:
/*	case OP_CODE_ZXH: */
		if (cpu->decoded_code.type1.reg2 > 0) {
			//SATADD
			ret = op_exec_satadd_1(cpu);
		}
		else {
			//ZXH
			ret = op_exec_zxh(cpu);
		}
		break;
	case OP_CODE_SATSUB:
/* 	case OP_CODE_SXB: */
		if (cpu->decoded_code.type1.reg2 > 0) {
			//OP_CODE_SATSUB
			//TODO
		}
		else {
			ret = op_exec_sxb(cpu);
		}
		break;
	case OP_CODE_SATSUBR:
/*	case OP_CODE_ZXB: */
		if (cpu->decoded_code.type1.reg2 > 0) {
			//SATSUBR
			//TODO
		}
		else {
			ret = op_exec_zxb(cpu);
		}
		break;
	case OP_CODE_SUB:
		ret = op_exec_sub(cpu);
		break;
	case OP_CODE_SUBR:
		ret = op_exec_subr(cpu);
		break;
	case OP_CODE_XOR:
		ret = op_exec_xor(cpu);
		break;
	case OP_CODE_OR:
		ret = op_exec_or(cpu);
		break;
	case OP_CODE_TST:
		ret = op_exec_tst(cpu);
		break;
	default:
		break;
	}
	return ret;
}
static int OpExec2(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type2.opcode) {
	case OP_CODE_ADD_2:
		ret = op_exec_add_2(cpu);
		break;
	case OP_CODE_CMP_2:
		ret = op_exec_cmp_2(cpu);
		break;
	case OP_CODE_MOV_2:
		ret = op_exec_mov_2(cpu);
		break;
	case OP_CODE_MULH_2:
		ret = op_exec_mulh_2(cpu);
		break;
	case OP_CODE_SAR_2:
		ret = op_exec_sar_2(cpu);
		break;
	case OP_CODE_SATADD_2:
		ret = op_exec_satadd_2(cpu);
		break;
	case OP_CODE_SHL_2:
		ret = op_exec_shl_2(cpu);
		break;
	case OP_CODE_SHR_2:
		ret = op_exec_shr_2(cpu);
		break;
	default:
		break;
	}
	return ret;
}
static int OpExec3(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type3.opcode) {
	case OP_CODE_BCOND:
		ret = op_exec_bcond(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec4(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type4_1.opcode) {
	case OP_CODE_SLDB:
		ret = op_exec_sldb(cpu);
		break;
	case OP_CODE_SLDH:
		ret = op_exec_sldh(cpu);
		break;
	case OP_CODE_SLDW:
/*	case OP_CODE_SSTW: */
		if (cpu->decoded_code.type4_1.gen == 0x00) {
			//SLDW
			ret = op_exec_sldw(cpu);
		}
		else {
			//SSTW
			ret = op_exec_sstw(cpu);
		}
		break;
	case OP_CODE_SSTB:
		ret = op_exec_sstb(cpu);
		break;
	case OP_CODE_SSTH:
		ret = op_exec_ssth(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec5(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type5.opcode) {
	case OP_CODE_JARL:
/*	case OP_CODE_JR: */
		ret = op_exec_jr(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec6(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type6.opcode) {
	case OP_CODE_ADDI:
		ret = op_exec_addi(cpu);
		break;
	case OP_CODE_ANDI:
		ret = op_exec_andi(cpu);
		break;
	case OP_CODE_MOVEA:
/*	case OP_CODE_MOV_6: */
		//MOVE_6 not spported
		ret = op_exec_movea(cpu);
		break;
	case OP_CODE_MOVHI:
		ret = op_exec_movhi(cpu);
		break;
	case OP_CODE_MULHI:
		ret = op_exec_mulhi(cpu);
		break;
	case OP_CODE_ORI:
		ret = op_exec_ori(cpu);
		break;
	case OP_CODE_SATSUBI:
		ret = op_exec_satsubi(cpu);
		break;
	case OP_CODE_XORI:
		ret = op_exec_xori(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec7(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type7.opcode) {
	case OP_CODE_LDB:
		ret = op_exec_ldb(cpu);
		break;
	case OP_CODE_LDH:
/*	case OP_CODE_LDW: */
		ret = op_exec_ldhw(cpu);
		break;
	case OP_CODE_STB:
		ret = op_exec_stb(cpu);
		break;
	case OP_CODE_STH:
/*	case OP_CODE_STW: */
		ret = op_exec_sthw(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec8(CpuManagerType *cpu)
{
	int ret = -1;
#if 1
	//TODO
	ret = op_exec_tst1_8(cpu);
#endif
	return ret;
}

static int OpExec9(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type9.sub) {
	case SOP_CODE_LDSR:
		ret = op_exec_ldsr(cpu);
		break;
	case SOP_CODE_SAR_9:
		ret = op_exec_sar_9(cpu);
		break;
	case SOP_CODE_SETF_9:
		ret = op_exec_setf(cpu);
		break;
	case SOP_CODE_SHL_9:
		ret = op_exec_shl_9(cpu);
		break;
	case SOP_CODE_SHR_9:
		ret = op_exec_shr_9(cpu);
		break;
	case SOP_CODE_STSR:
		ret = op_exec_stsr(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec10(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type10.sub) {
	case SOP_CODE_DI:
/*	case SOP_CODE_EI: */
		ret = op_exec_diei(cpu);
		break;
	case SOP_CODE_HALT:
		ret = op_exec_halt(cpu);
		break;
	case SOP_CODE_RETI:
		ret = op_exec_reti(cpu);
		break;
	case SOP_CODE_TRAP:
		ret = op_exec_trap(cpu);
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec11(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type11.sub1) {
	case SOP_CODE_MUL_11:
		//TODO
		break;
	default:
		break;
	}
	return ret;
}

static int OpExec12(CpuManagerType *cpu)
{
	int ret = -1;

	switch (cpu->decoded_code.type10.sub) {
	case SOP_CODE_MUL_12:
		//TODO
		break;
	default:
		break;
	}

	return ret;
}

static int OpExec13(CpuManagerType *cpu)
{
	return -1;
}


