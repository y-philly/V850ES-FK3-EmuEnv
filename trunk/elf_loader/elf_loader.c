#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "elf_loader.h"

#define TARGET_ROM_SIZE	(1024*512)
#define TARGET_ROM_ADDR	(0x00000000)

static uint8 elf_rom_data[TARGET_ROM_SIZE];

static int elf_load(ElfLoaderType *loader);
static int elf_get_code(void *loader, uint32 pc, uint16 code[OP_DECODE_MAX]);

void ElfLoader_init(ElfLoaderType *loader, char *path, TargetRamType *ram)
{
	loader->file = path;
	loader->get_code = elf_get_code;
	loader->ram = ram;

	loader->rom.addr = TARGET_ROM_ADDR;
	loader->rom.size = TARGET_ROM_SIZE;
	loader->rom.data = elf_rom_data;

	(void)elf_load(loader);
	return;
}

static int elf_get_code(void *loader, uint32 pc, uint16 code[OP_DECODE_MAX])
{
	int off;
	uint16 *addrp;
	ElfLoaderType *p = (ElfLoaderType*)loader;
	/*
	 * とりあえず，プログラムはROMだけに存在するものとする．
	 */
	if (pc < p->rom.addr) {
		return -1;
	}
	if (pc > (p->rom.addr + p->rom.size)) {
		return -1;
	}
	if ((pc % 2) != 0) {
		//アライメント違反
		return -1;
	}
	off = pc - p->rom.addr;
	addrp = (uint16*)&(p->rom.data[off]);

	code[0] = addrp[0];
	code[1] = addrp[1];

	return 0;
}

static int elf_load(ElfLoaderType *loader)
{
	int fd;
	int err;
	int rlen = 0;
	int len = loader->rom.size;
	char *buffer = (char*)loader->rom.data;

	fd = open(loader->file, O_RDONLY | O_BINARY );
	if (fd < 0) {
		return -1;
	}

	lseek(fd, 0, SEEK_SET);
	while (rlen < len) {
		err = read(fd, (void*)buffer, len);
		if (err <= 0) {
			close(fd);
			if (rlen > 0U) {
				close(fd);
				return 0;
			}
			else {
				close(fd);
				return -1;
			}
		}
		rlen += err;
	}
	close(fd);
	return 0;
}
