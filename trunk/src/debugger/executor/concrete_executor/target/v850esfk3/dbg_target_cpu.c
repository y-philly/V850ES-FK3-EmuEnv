#include "dbg_target_cpu.h"
#include "cpu.h"

#include <stdio.h>

void dbg_target_print_cpu(void)
{
	int i;
	printf("PC		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.pc);
	for (i = 0; i < 32; i++) {
		printf("R%d		0x%x\n", i, virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.r[i]);
	}
	printf("EIPC		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.eipc);
	printf("EIPSW		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.eipsw);
	printf("ECR		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.ecr);
	printf("PSW		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.psw);
	printf("FEPC 		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.fepc);
	printf("FEPSW 		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.fepsw);
	printf("CTBP		0x%x\n", virtual_cpu.cores[CPU_CONFIG_CORE_ID_0].core.reg.ctbp);

	return;
}
