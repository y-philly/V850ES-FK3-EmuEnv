#ifndef _ELF_LOADER_H_
#define _ELF_LOADER_H_

#include "std_types.h"
#include "op_dec_types.h"


typedef struct {
	char *file;
	TargetRamType *ram;
	TargetRomType rom;
	int (*get_code) (void *loader, uint32 pc, uint16 code[OP_DECODE_MAX]);
} ElfLoaderType;

extern void ElfLoader_init(ElfLoaderType *loader, char *path, TargetRamType *ram);

#endif /* _ELF_LOADER_H_ */
