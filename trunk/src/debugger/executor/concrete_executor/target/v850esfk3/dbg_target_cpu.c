#include "dbg_target_cpu.h"
#include "cpu.h"
#include "symbol_ops.h"

#include <stdio.h>


static void print_register(const char* regname, uint32 addr)
{
	 uint32 funcaddr;
	 int funcid;
	 uint32 gladdr;
	 int glid;

	 funcid = symbol_pc2funcid(addr, &funcaddr);
	 if (funcid >= 0) {
		printf("%s		0x%x %s(+0x%x)\n", regname, addr, symbol_funcid2funcname(funcid), addr - funcaddr);
	 }
	 else {
		glid = symbol_addr2glid(addr, &gladdr);
		if (glid >= 0) {
			printf("%s		0x%x %s(+0x%x)\n", regname, addr, symbol_glid2glname(glid), addr - gladdr);
		}
		else {
			printf("%s		0x%x\n", regname, addr);
		}
	 }
	return;
}

void dbg_target_print_cpu(void)
{
	int i;
	char buffer[128];
	uint32 pc = virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.pc;

	 print_register("PC", pc);
	 for (i = 0; i < 32; i++) {
		 sprintf(buffer, "R%d", i);
		 //printf("R%d		0x%x\n", i, virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.r[i]);
		 print_register(buffer, virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.r[i]);
	 }
	 print_register("EIPC", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.eipc);
	 printf("EIPSW		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.eipsw);
	 printf("ECR		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.ecr);
	 printf("PSW		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.psw);
	 print_register("EIPC", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.fepc);
	 printf("FEPSW 		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.fepsw);
	 printf("CTBP		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.ctbp);

	return;
}
