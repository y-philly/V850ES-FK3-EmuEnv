#include <stdlib.h>
#include "cpu.h"
#include "op_exec_ops.h"

void cpu_memget_addrp(CpuManagerType *cpu, uint32 addr, uint32 **addrp)
{
	uint32 off;

	if ((addr >= cpu->loader->rom.addr) &&
			(addr <= (cpu->loader->rom.addr + cpu->loader->rom.size))) {
		/*
		 * ROM
		 */
		off = addr - cpu->loader->rom.addr;
		*addrp = (uint32*)&cpu->loader->rom.data[off];
	}
	else if ((addr >= cpu->loader->ram->addr) &&
			(addr <= (cpu->loader->ram->addr + cpu->loader->ram->size))) {
		/*
		 * RAM
		 */
		off = addr - cpu->loader->ram->addr;
		*addrp = (uint32*)&cpu->loader->ram->data[off];
	}
	else {
		uint32 devaddr = addr & 0x03FFFFFF;

		if  ((devaddr >= cpu->io_mem1.addr) &&
				(devaddr <= (cpu->io_mem1.addr + cpu->io_mem1.size))) {
			off = devaddr - cpu->io_mem1.addr;
			DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "###0x%x:loadaddr(0x%x)=%s\n", cpu->cpu.pc, addr, addr2devregname(addr)));
			*addrp = (uint32*)&cpu->io_mem1.data[off];
		}
		else if  ((devaddr >= cpu->io_mem2.addr) &&
				(devaddr <= (cpu->io_mem2.addr + cpu->io_mem2.size))) {
			off = devaddr - cpu->io_mem2.addr;
			*addrp = (uint32*)&cpu->io_mem2.data[off];
		}
		else {
			/*
			 * invalid memory
			 */
			*addrp = NULL;
			printf("0x%x:Invalid memory:not found memory(off=0x%x, ramaddr_start=0x%x, ramaddr_end=0x%x)\n",
					cpu->cpu.pc, addr,
					cpu->loader->ram->addr, (cpu->loader->ram->addr + cpu->loader->ram->size));
		}

	}
	return;
}

void cpu_memget_raddrp(CpuManagerType *cpu, uint32 addr, uint32 **addrp)
{
	uint32 off;

	if ((addr >= cpu->loader->rom.addr) &&
			(addr <= (cpu->loader->rom.addr + cpu->loader->rom.size))) {
		/*
		 * ROM
		 */
		off = addr - cpu->loader->rom.addr;
		*addrp = (uint32*)&cpu->loader->rom.data[off];
	}
	else if ((addr >= cpu->loader->ram->addr) &&
			(addr <= (cpu->loader->ram->addr + cpu->loader->ram->size))) {
		/*
		 * RAM
		 */
		off = addr - cpu->loader->ram->addr;
		*addrp = (uint32*)&cpu->loader->ram->data[off];
	}
	else {
		uint32 devaddr = addr & 0x03FFFFFF;

		if  ((devaddr >= cpu->io_mem1.addr) &&
				(devaddr <= (cpu->io_mem1.addr + cpu->io_mem1.size))) {
			off = devaddr - cpu->io_mem1.addr;
			DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "###0x%x:loadaddr(0x%x)=%s\n", cpu->cpu.pc, addr, addr2devregname(addr)));
			*addrp = (uint32*)&cpu->io_mem1.data[off];
		}
		else if  ((devaddr >= cpu->io_mem2.addr) &&
				(devaddr <= (cpu->io_mem2.addr + cpu->io_mem2.size))) {
			uint32 *spc_canaddr;
			spc_canaddr = can_get_reg16_addr(cpu, addr);
			if (spc_canaddr != NULL) {
				*addrp = spc_canaddr;
			}
			else {
				off = devaddr - cpu->io_mem2.addr;
				*addrp = (uint32*)&cpu->io_mem2.data[off];
			}
		}
		else {
			/*
			 * invalid memory
			 */
			*addrp = NULL;
			printf("Invalid memory:not found memory(off=0x%x)\n", addr);
		}

	}
	return;
}

/*
 * Format4
 */
int op_exec_sldb(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	sint32 ret;
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

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	ret = *((sint8*)addrp);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.B disp7(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], ret));

	cpu->cpu.r[reg2] = ret;

	cpu->cpu.pc += 2;
	return 0;
}
int op_exec_sldh(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	sint32 ret;
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

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	ret = *((sint16*)addrp);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.H disp8(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], ret));

	cpu->cpu.r[reg2] = ret;

	cpu->cpu.pc += 2;
	return 0;
}

int op_exec_sldw(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	uint32 ret;
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

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	ret = *((uint32*)addrp);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.W disp8(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], ret));

	cpu->cpu.r[reg2] = ret;

	cpu->cpu.pc += 2;
	return 0;
}
/*
 * Format7
 */

int op_exec_ldb(CpuManagerType *cpu)
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

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.B disp16(%d),r%d(0x%x), r%d(0x%x) addr=0x%x:0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], addr, *((sint8*)addrp)));

	cpu->cpu.r[reg2] = *((sint8*)addrp);

	cpu->cpu.pc += 4;
	return 0;
}

int op_exec_ldbu(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code.type7.reg1;
	uint32 reg2 = cpu->decoded_code.type7.reg2;
	sint32 disp_bit;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp_bit = (cpu->decoded_code.type7.opcode & 0x0001);
	disp = op_sign_extend(15, (cpu->decoded_code.type7.disp << 1) | disp_bit);

	addr = cpu->cpu.r[reg1] + disp;

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.BU disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], *((uint8*)addrp)));

	cpu->cpu.r[reg2] = *((uint8*)addrp);

	cpu->cpu.pc += 4;
	return 0;
}
int op_exec_ldhw(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	sint32 ret;
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
		//LD.H
		disp = op_sign_extend(15, (cpu->decoded_code.type7.disp << 1) );
		addr = cpu->cpu.r[reg1] + disp;
		cpu_memget_raddrp(cpu, addr, &addrp);
		if (addrp == NULL) {
			return -1;
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.H disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], *((sint16*)addrp)));
		ret = *((sint16*)addrp);
	}
	else {
		//LD.W
		disp = op_sign_extend(15, (cpu->decoded_code.type7.disp << 1) );
		addr = cpu->cpu.r[reg1] + disp;
		cpu_memget_raddrp(cpu, addr, &addrp);
		if (addrp == NULL) {
			return -1;
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.W disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], *((sint32*)addrp)));
		ret = *((sint32*)addrp);
	}
	cpu->cpu.r[reg2] = ret;

	cpu->cpu.pc += 4;
	return 0;
}

int op_exec_ldhu(CpuManagerType *cpu)
{
	uint32 addr;
	uint32 *addrp;
	uint32 ret;
	uint32 disp;
	uint32 reg1 = cpu->decoded_code.type7.reg1;
	uint32 reg2 = cpu->decoded_code.type7.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_zero_extend(15, (cpu->decoded_code.type7.disp << 1) );
	addr = cpu->cpu.r[reg1] + disp;
	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.HU disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->cpu.pc, disp, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2], *((sint16*)addrp)));
	ret = *((uint16*)addrp);

	cpu->cpu.r[reg2] = ret;

	cpu->cpu.pc += 4;
	return 0;
}
