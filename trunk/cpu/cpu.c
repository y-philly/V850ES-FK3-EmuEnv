#include <string.h>
#include "cpu.h"
#include "op_dec.h"
#include "op_exec.h"

CpuManagerType CpuManager;

#define TARGET_RAM_ADDR	(0x03FF7000)
#define TARGET_RAM_SIZE	(1024*32)
static uint8 cpu_ram_data[TARGET_RAM_SIZE];

#define TARGET_IOMEM1_ADDR	(0x03FFF000)
#define TARGET_IOMEM1_SIZE	(1024*4)
static uint8 cpu_iomem1_data[TARGET_IOMEM1_SIZE];

#define TARGET_IOMEM2_ADDR	(0x03FEC000)
#define TARGET_IOMEM2_SIZE	(1024*12)
static uint8 cpu_iomem2_data[TARGET_IOMEM2_SIZE];

static uint8 cpu_iomem2_rdata[TARGET_IOMEM2_SIZE];

void cpu_init(ElfLoaderType *loader)
{
	memset(&CpuManager, 0, sizeof(CpuManager));

	cpu_reset(&CpuManager);

	CpuManager.loader = loader;
	CpuManager.ram.addr = TARGET_RAM_ADDR;
	CpuManager.ram.size = TARGET_RAM_SIZE;
	CpuManager.ram.data = cpu_ram_data;

	CpuManager.io_mem1.addr = TARGET_IOMEM1_ADDR;
	CpuManager.io_mem1.size = TARGET_IOMEM1_SIZE;
	CpuManager.io_mem1.data = cpu_iomem1_data;

	CpuManager.io_mem2.addr = TARGET_IOMEM2_ADDR;
	CpuManager.io_mem2.size = TARGET_IOMEM2_SIZE;
	CpuManager.io_mem2.data = cpu_iomem2_data;

	CpuManager.io_mem2r.addr = TARGET_IOMEM2_ADDR;
	CpuManager.io_mem2r.size = TARGET_IOMEM2_SIZE;
	CpuManager.io_mem2r.data = cpu_iomem2_rdata;

	return;
}

void cpu_reset(CpuManagerType *cpu)
{
	cpu->cpu.pc = 0x00;
	cpu->cpu.r[0] = 0;

	cpu->cpu.eipc = 0;
	cpu->cpu.eipsw = 0;
	cpu->cpu.fepc = 0;
	cpu->cpu.fepsw = 0;
	cpu->cpu.ecr = 0;
	cpu->cpu.psw = 0x20;
	cpu->cpu.ctbp = 0;
	CpuManager.is_halt = FALSE;
	return;
}

int cpu_exec(void)
{
	int ret;

	ElfLoaderType *loader = CpuManager.loader;

	/*
	 * 命令取得する
	 */
	loader->get_code(loader, CpuManager.cpu.pc, CpuManager.current_code);

	/*
	 * デコード
	 */
	ret = OpDecode(CpuManager.current_code, &CpuManager.decoded_code);
	if (ret < 0) {
		return ret;
	}

	/*
	 * 命令実行
	 */
	ret = OpExec(&CpuManager);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

void cpu_illegal_opcode_trap(CpuManagerType *cpu)
{
	uint32 eicc;
	uint32 ecr;

	eicc = 0x60;
	cpu->cpu.eipc = cpu->cpu.pc - 4;
	cpu->cpu.eipsw = cpu->cpu.psw;
	ecr = cpu->cpu.ecr;
	ecr = ecr & 0x00FF;
	ecr |= (eicc << 16);
	cpu->cpu.ecr = ecr;
	CPU_SET_NP(&cpu->cpu);
	CPU_SET_EP(&cpu->cpu);
	CPU_SET_ID(&cpu->cpu);
	cpu->cpu.pc = 0x60;

	return;
}
