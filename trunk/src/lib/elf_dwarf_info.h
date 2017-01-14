#ifndef _ELF_DWARF_INFO_H_
#define _ELF_DWARF_INFO_H_
#include "std_types.h"
#include "std_errno.h"
#include "elf_dwarf_util.h"
#include "elf_dwarf_abbrev.h"

typedef struct {
	uint32				abbrev_code;
	ElfDwarfAbbrevType	*abbrev_info;
	ElfPointerArrayType	*attribute;
} ElfDwarfDieType;

typedef struct {
	uint32	length;
	uint16	version;
	uint32	abbrev_offset;
	uint8	pointer_size;
} ElfDwarfCompilationUnitHeaderType;

extern Std_ReturnType elf_dwarf_info_load(uint8 *elf_data);
extern ElfDwarfCompilationUnitHeaderType *elf_dwarf_info_alloc_empty_ElfDwarfCompilationUnitHeader(void);

#endif /* _ELF_DWARF_INFO_H_ */
