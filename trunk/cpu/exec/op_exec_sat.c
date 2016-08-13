#include "cpu.h"
#include "op_exec_ops.h"

/*
 * Format1
 */
int op_exec_satadd_1(CpuManagerType *cpu)
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
	result = op_satadd(&cpu->cpu, cpu->cpu.r[reg2], cpu->cpu.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATADD r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_satsub_1(CpuManagerType *cpu)
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
	result = op_satadd(&cpu->cpu, cpu->cpu.r[reg2], -((sint32)cpu->cpu.r[reg1]));
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATSUB r%d(%d),r%d(%d):%d\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;
}

/*
 * Format2
 */
int op_exec_satadd_2(CpuManagerType *cpu)
{
	sint32 imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code.type2.imm);
	uint32 reg2 = cpu->decoded_code.type2.reg2;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->cpu, cpu->cpu.r[reg2], imm_data);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATADD imm5(%d),r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 2;
	return 0;
}



/*
 * Format6
 */

int op_exec_satsubi(CpuManagerType *cpu)
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

	result = op_satadd(&cpu->cpu, cpu->cpu.r[reg1], -imm_data);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATSUBI imm16(%d), r%d(%d), r%d(%d):%d\n", cpu->cpu.pc, imm_data, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], result));

	cpu->cpu.r[reg2] = result;

	cpu->cpu.pc += 4;
	return 0;
}
