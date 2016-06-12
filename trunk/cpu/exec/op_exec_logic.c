#include "cpu.h"
#include "op_exec_ops.h"

/*
 * Format1
 */
int op_exec_or(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_or(&cpu->cpu, cpu->cpu.r[reg2], cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: OR r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;

}
int op_exec_zxb(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint8 data;
	uint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	data = (uint8)cpu->cpu.r[reg1];
	result = (uint32)data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ZXB r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1],  result));

	cpu->cpu.r[reg1] = result;

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_zxh(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint16 data;
	uint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	data = (uint16)cpu->cpu.r[reg1];
	result = (uint32)data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ZXH r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1],  result));

	cpu->cpu.r[reg1] = result;

	cpu->cpu.pc += 2;
	return 0;
}

int op_exec_tst(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_and(&cpu->cpu, cpu->cpu.r[reg2], cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: TST r%d(%d), r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2],  result));

	cpu->cpu.pc += 2;
	return 0;
}

int op_exec_and(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_and(&cpu->cpu, cpu->cpu.r[reg2], cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: AND r%d(%d), r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2],  result));
	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_not(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = ~cpu->cpu.r[reg1];
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: NOT r%d(%d), r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2],  result));

	cpu->cpu.r[reg2] = result;
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);
	CPU_CLR_OV(&cpu->cpu);

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_xor(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint32 reg2 = cpu->decoded_code.type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = cpu->cpu.r[reg2] ^ cpu->cpu.r[reg1];
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: XOR r%d(%d), r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2],  result));
	cpu->cpu.r[reg2] = result;

	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);
	CPU_CLR_OV(&cpu->cpu);

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_sxh(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint16 tmp;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	tmp = ((sint16)cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SXH r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1],  tmp));

	cpu->cpu.r[reg1] = (sint32)tmp;

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_sxb(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	uint8 tmp;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	tmp = ((sint8)cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SXB r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1],  tmp));
	cpu->cpu.r[reg1] = (sint32)tmp;

	cpu->cpu.pc += 2;
	return 0;
}

/*
 * Format2
 */
static void op_chk_and_set_shl_carry(CpuRegisterType *cpu, uint32 data, uint32 sh)
{
	if (sh > 0) {
		if ( data & (1 << (32 - sh)) ) {
			CPU_SET_CY(cpu);
		}
		else {
			CPU_CLR_CY(cpu);
		}
	}
	else {
		CPU_CLR_CY(cpu);
	}
	return;
}
static void op_chk_and_set_shr_carry(CpuRegisterType *cpu, uint32 data, uint32 sh)
{
	if (sh > 0) {
		if ( data & (1 << (sh - 1)) ) {
			CPU_SET_CY(cpu);
		}
		else {
			CPU_CLR_CY(cpu);
		}
	}
	else {
		CPU_CLR_CY(cpu);
	}
	return;
}

int op_exec_shl_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	uint32 imm_data;
	uint32 reg2_data;
	uint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	imm_data = OP_FORMAT2_IMM_ZERO_EXTEND(cpu->decoded_code.type2.imm);

	result = reg2_data << imm_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SHL imm5(%d),r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = (sint32)result;

	CPU_CLR_OV(&cpu->cpu);
	op_chk_and_set_shl_carry(&cpu->cpu, reg2_data, imm_data);
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_shr_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	uint32 imm_data;
	uint32 reg2_data;
	uint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	imm_data = OP_FORMAT2_IMM_ZERO_EXTEND(cpu->decoded_code.type2.imm);

	result = reg2_data >> imm_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SHR imm5(%d),r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = (sint32)result;

	CPU_CLR_OV(&cpu->cpu);
	op_chk_and_set_shr_carry(&cpu->cpu, reg2_data, imm_data);
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_sar_2(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	uint32 imm_data;
	sint32 reg2_data;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->cpu.r[reg2];
	imm_data = OP_FORMAT2_IMM_ZERO_EXTEND(cpu->decoded_code.type2.imm);

	result = reg2_data >> imm_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SAR imm5(%d),r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = (sint32)result;

	CPU_CLR_OV(&cpu->cpu);
	op_chk_and_set_shr_carry(&cpu->cpu, reg2_data, imm_data);
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);

	cpu->cpu.pc += 2;

	return 0;
}



/*
 * Format6
 */

int op_exec_andi(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	uint32 imm_data = op_zero_extend(16, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_andi(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ANDI imm5(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}

int op_exec_ori(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	uint32 imm_data = op_zero_extend(16, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_ori(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ORI imm5(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}

int op_exec_xori(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type6.reg1;
	uint32 reg2 = cpu->decoded_code.type6.reg2;
	uint32 imm_data = op_zero_extend(16, cpu->decoded_code.type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_xori(&cpu->cpu, imm_data, cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: XORI imm5(%d),r%d(%d) r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}

/*
 * Format9
 */
int op_exec_shl_9(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type9.reg2;
	uint32 reg1 = cpu->decoded_code.type9.gen;
	uint32 reg2_data;
	uint32 reg1_data;
	uint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	reg2_data = cpu->cpu.r[reg2];
	reg1_data = (cpu->cpu.r[reg1] & 0x0000001F);

	result = reg2_data << reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SHL r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = (sint32)result;

	CPU_CLR_OV(&cpu->cpu);
	op_chk_and_set_shl_carry(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);

	cpu->cpu.pc += 4;

	return 0;
}

int op_exec_shr_9(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type9.reg2;
	uint32 reg1 = cpu->decoded_code.type9.gen;
	uint32 reg2_data;
	uint32 reg1_data;
	uint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	reg2_data = cpu->cpu.r[reg2];
	reg1_data = (cpu->cpu.r[reg1] & 0x0000001F);

	result = reg2_data >> reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SHR r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));
	cpu->cpu.r[reg2] = (sint32)result;

	CPU_CLR_OV(&cpu->cpu);
	op_chk_and_set_shr_carry(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);

	cpu->cpu.pc += 4;

	return 0;
}

int op_exec_sar_9(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type9.reg2;
	uint32 reg1 = cpu->decoded_code.type9.gen;
	sint32 reg2_data;
	sint32 reg1_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	reg2_data = cpu->cpu.r[reg2];
	reg1_data = (cpu->cpu.r[reg1] & 0x0000001F);

	result = reg2_data >> reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SAR r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = (sint32)result;

	CPU_CLR_OV(&cpu->cpu);
	op_chk_and_set_shr_carry(&cpu->cpu, reg2_data, reg1_data);
	op_chk_and_set_zero(&cpu->cpu, cpu->cpu.r[reg2]);
	op_chk_and_set_sign(&cpu->cpu, cpu->cpu.r[reg2]);

	cpu->cpu.pc += 4;

	return 0;
}
