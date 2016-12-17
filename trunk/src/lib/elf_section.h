#ifndef _ELF_SECTION_H_
#define _ELF_SECTION_H_

#include "loader/elf.h"
#include "std_errno.h"

extern Std_ReturnType elf_symbol_load(uint8 *elf_data);
extern Std_ReturnType elfsym_get_symbol_num(uint32 *sym_num);

typedef enum {
	SYMBOL_TYPE_OBJECT,
	SYMBOL_TYPE_FUNC,
	SYMBOL_TYPE_NOTYPE,
	SYMBOL_TYPE_NOSUP,
} ElfSymbolEnumType;

typedef struct {
	char* 				name;
	uint32 				size;
	uint32 				addr;
	ElfSymbolEnumType	type;
} ElfSymbolType;
extern Std_ReturnType elfsym_get_symbol(uint32 index, ElfSymbolType *elfsym);

#endif /* _ELF_SECTION_H_ */
