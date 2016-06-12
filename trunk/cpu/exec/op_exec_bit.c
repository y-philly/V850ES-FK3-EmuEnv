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

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}

	addr = cpu->cpu.r[reg1] + disp16;

	cpu_memget_addrp(cpu, addr, &addrp);
	if (addrp == NULL) {
		return -1;
	}
	if (((*addrp) & (1 << bit3)) == (1 << bit3)) {
		CPU_CLR_Z(&cpu->cpu);
	}
	else {
		CPU_SET_Z(&cpu->cpu);
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: TST1 bit#3(%d), disp16(%d),r%d(0x%x):psw=0x%x\n", cpu->cpu.pc, bit3, disp16, reg1, cpu->cpu.r[reg1], cpu->cpu.psw));

	cpu->cpu.pc += 4;

	return 0;
}
