#include <stdlib.h>

#include "../cpu_exec/op_exec_ops.h"
#include "cpu.h"


/*
 * Format4
 */
int op_exec_sstb(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code.type4_1.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = cpu->decoded_code.type4_1.disp;
	disp = (disp << 1) | cpu->decoded_code.type4_1.gen;
	disp = op_zero_extend(7, disp);
	addr = cpu->cpu.r[reg1] + disp;

	cpu_memget_addrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SST.B r%d(0x%x), disp7(0x%x) r%d(0x%x):0x%x\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], disp, reg2, cpu->cpu.r[reg2], (uint8)cpu->cpu.r[reg2]));

	*((uint8*)addrp) =	(uint8)cpu->cpu.r[reg2];
	intc_hook_update_reg8(cpu, addr, *((uint8*)addrp));
	serial_hook_update_reg8(cpu, addr, *((uint8*)addrp));

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_ssth(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code.type4_1.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}


	disp = cpu->decoded_code.type4_1.disp;
	disp = (disp << 1) | cpu->decoded_code.type4_1.gen;
	disp = op_zero_extend(7, disp);
	disp = disp << 1;
	addr = cpu->cpu.r[reg1] + disp;

	cpu_memget_addrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SST.H r%d(0x%x), disp8(0x%x) r%d(0x%x):0x%x\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], disp, reg2, cpu->cpu.r[reg2], (uint16)cpu->cpu.r[reg2]));
	*((uint16*)addrp) =	(uint16)cpu->cpu.r[reg2];
	intc_hook_update_reg16(cpu, addr, *((uint16*)addrp));
	can_hook_update_reg16(cpu, addr, *((uint16*)addrp));

	cpu->cpu.pc += 2;
	return 0;
}

int op_exec_sstw(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code.type4_1.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = cpu->decoded_code.type4_1.disp;
	disp = op_zero_extend(6, disp);
	disp = disp << 2;
	addr = cpu->cpu.r[reg1] + disp;

	cpu_memget_addrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SST.W r%d(0x%x), disp7(0x%x) r%d(0x%x):0x%x\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1], disp, reg2, cpu->cpu.r[reg2], (uint32)cpu->cpu.r[reg2]));

	comm_hook_update_reg32(cpu, addr, cpu->cpu.r[reg2]);

	*((uint32*)addrp) =	cpu->cpu.r[reg2];



	cpu->cpu.pc += 2;
	return 0;
}

/*
 * Format7
 */

int op_exec_sthw(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code.type7.reg1;
	uint32 reg2 = cpu->decoded_code.type7.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}


	if (cpu->decoded_code.type7.gen == 0x00) {
		//ST.H
		disp = op_sign_extend(15, (cpu->decoded_code.type7.disp << 1) );
		addr = cpu->cpu.r[reg1] + disp;
		cpu_memget_addrp(cpu, addr, &addrp);
		if (addrp == NULL) {
			return -1;
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ST.H r%d(0x%x), disp16(%d) r%d(0x%x):0x%x\n", cpu->cpu.pc, reg2, cpu->cpu.r[reg2], disp, reg1, cpu->cpu.r[reg1], (sint16)cpu->cpu.r[reg2]));
		*((sint16*)addrp) = (sint16)cpu->cpu.r[reg2];
		intc_hook_update_reg16(cpu, addr, *((sint16*)addrp));
		can_hook_update_reg16(cpu, addr, *((uint16*)addrp));
	}
	else {
		//ST.W
		disp = op_sign_extend(15, (cpu->decoded_code.type7.disp << 1) );
		addr = cpu->cpu.r[reg1] + disp;
		cpu_memget_addrp(cpu, addr, &addrp);
		if (addrp == NULL) {
			return -1;
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ST.W r%d(0x%x), disp16(%d) r%d(0x%x):0x%x\n", cpu->cpu.pc, reg2, cpu->cpu.r[reg2], disp, reg1, cpu->cpu.r[reg1], (sint32)cpu->cpu.r[reg2]));

		comm_hook_update_reg32(cpu, addr, cpu->cpu.r[reg2]);


		*((sint32*)addrp) = cpu->cpu.r[reg2];
	}

	cpu->cpu.pc += 4;
	return 0;
}


int op_exec_stb(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code.type7.reg1;
	uint32 reg2 = cpu->decoded_code.type7.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(15, (cpu->decoded_code.type7.disp << 1) | cpu->decoded_code.type7.gen);
	addr = cpu->cpu.r[reg1] + disp;

	cpu_memget_addrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ST.B r%d(0x%x), disp16(%d) r%d(0x%x):0x%x\n", cpu->cpu.pc, reg2, cpu->cpu.r[reg2], disp, reg1, cpu->cpu.r[reg1], (uint8)cpu->cpu.r[reg2]));
	//ST.B
	*((sint8*)addrp) = (sint8)cpu->cpu.r[reg2];
	intc_hook_update_reg8(cpu, addr, *((sint8*)addrp));
	serial_hook_update_reg8(cpu, addr, *((uint8*)addrp));

	cpu->cpu.pc += 4;
	return 0;
}
