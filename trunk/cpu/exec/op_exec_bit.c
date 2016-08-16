#include "cpu.h"
#include "op_exec_ops.h"



/*
 * Format8
 */
int op_exec_tst1_8(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type8.reg1;
	sint32 disp16 = cpu->decoded_code.type8.disp;
	sint32 bit3 = cpu->decoded_code.type8.bit;
	uint32 addr;
	uint32 *addrp;
	uint8 bit;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}

	addr = cpu->cpu.r[reg1] + disp16;

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	bit = *((uint8*)addrp);
	if ((bit & (1 << bit3)) == (1 << bit3)) {
		CPU_CLR_Z(&cpu->cpu);
	}
	else {
		CPU_SET_Z(&cpu->cpu);
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: TST1 bit#3(%d), disp16(%d),r%d(0x%x):psw=0x%x\n", cpu->cpu.pc, bit3, disp16, reg1, cpu->cpu.r[reg1], cpu->cpu.psw));

	cpu->cpu.pc += 4;

	return 0;
}
int op_exec_set1_8(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type8.reg1;
	sint32 disp16 = cpu->decoded_code.type8.disp;
	sint32 bit3 = cpu->decoded_code.type8.bit;
	uint32 addr;
	uint32 *addrp;
	uint8 *bitp;
	uint8 org_bit;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}

	addr = cpu->cpu.r[reg1] + disp16;

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}

	bitp = (uint8*)addrp;
	org_bit = *bitp;

	*bitp |= (1 << bit3);

	if (((org_bit) & (1 << bit3)) == (1 << bit3)) {
		CPU_CLR_Z(&cpu->cpu);
	}
	else {
		CPU_SET_Z(&cpu->cpu);
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SET1 bit#3(%d), disp16(%d), addr=0x%x r%d(0x%x):psw=0x%x, bit=0x%x\n", cpu->cpu.pc, bit3, disp16, addr, reg1, cpu->cpu.r[reg1], cpu->cpu.psw, *bitp));

	cpu->cpu.pc += 4;

	return 0;
}

int op_exec_clr1_8(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type8.reg1;
	sint32 disp16 = cpu->decoded_code.type8.disp;
	sint32 bit3 = cpu->decoded_code.type8.bit;
	uint32 addr;
	uint32 *addrp;
	uint8 *bitp;
	uint8 org_bit;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}

	addr = cpu->cpu.r[reg1] + disp16;

	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}

	bitp = (uint8*)addrp;
	org_bit = *bitp;

	*bitp &= ~(1 << bit3);

	if (((org_bit) & (1 << bit3)) == (1 << bit3)) {
		CPU_CLR_Z(&cpu->cpu);
	}
	else {
		CPU_SET_Z(&cpu->cpu);
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: CLR1 bit#3(%d), disp16(%d), addr=0x%x r%d(0x%x):psw=0x%x, bit=0x%x\n", cpu->cpu.pc, bit3, disp16, addr, reg1, cpu->cpu.r[reg1], cpu->cpu.psw, *bitp));

	cpu->cpu.pc += 4;

	return 0;
}

/*
 * Format9
 */
int op_exec_set1_9(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type9.gen;
	sint32 reg2 = cpu->decoded_code.type9.reg2;
	uint32 addr;
	uint32 *addrp;
	uint8 *bitp;
	uint8 org_bit;
	uint8 bit3;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	/*
	 * （ 2） adr ← GR [reg1]
	 * Zフラグ ← Not (Load-memory-bit (adr, reg2) )
	 * Store-memory-bit (adr, reg2, 1)
	 */

	addr = cpu->cpu.r[reg1];
	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}

	bitp = (uint8*)addrp;
	org_bit = *bitp;

	bit3 = (cpu->cpu.r[reg2] & 0x07);
	*bitp |= (1 << bit3);

	if (((org_bit) & (1 << bit3)) == (1 << bit3)) {
		CPU_CLR_Z(&cpu->cpu);
	}
	else {
		CPU_SET_Z(&cpu->cpu);
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
			"0x%x: SET1 bit#3(%d), addr=0x%x r%d(0x%x) r%d(0x%x):psw=0x%x, bit=0x%x\n",
			cpu->cpu.pc,
			bit3, addr, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2],cpu->cpu.psw, *bitp));

	cpu->cpu.pc += 4;

	return 0;
}
int op_exec_clr1_9(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type9.gen;
	sint32 reg2 = cpu->decoded_code.type9.reg2;
	uint32 addr;
	uint32 *addrp;
	uint8 *bitp;
	uint8 org_bit;
	uint8 bit3;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	/*
	 * （ 2） adr ← GR [reg1]
	 * Zフラグ ← Not (Load-memory-bit (adr, reg2) )
	 * Store-memory-bit (adr, reg2, 0)
	 */

	addr = cpu->cpu.r[reg1];
	cpu_memget_raddrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}

	bitp = (uint8*)addrp;
	org_bit = *bitp;

	bit3 = (cpu->cpu.r[reg2] & 0x07);
	*bitp &= ~(1 << bit3);

	if (((org_bit) & (1 << bit3)) == (1 << bit3)) {
		CPU_CLR_Z(&cpu->cpu);
	}
	else {
		CPU_SET_Z(&cpu->cpu);
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
			"0x%x: CLR1 bit#3(%d), addr=0x%x r%d(0x%x) r%d(0x%x):psw=0x%x, bit=0x%x\n",
			cpu->cpu.pc,
			bit3, addr, reg1, cpu->cpu.r[reg1], reg2, cpu->cpu.r[reg2],cpu->cpu.psw, *bitp));

	cpu->cpu.pc += 4;

	return 0;
}

