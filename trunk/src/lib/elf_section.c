#include "elf_section.h"
#include <stdio.h>

/*
 * セクション情報(内部向け)
 */
typedef struct {
	Elf32_Shdr *shdr;
	uint8		*shdata;
} ElfSectionHeaderType;

/*
 * ストリングテーブルのセクション情報(内部向け)
 */
typedef ElfSectionHeaderType ElfStringTableSectionType;

/*
 * シンボルのセクション情報(内部向け)
 */
typedef struct {
	Elf32_Shdr 					*shdr;
	Elf32_Sym					*sym;
	uint32						symbol_num;
	ElfStringTableSectionType	*string_table;
} ElfSymbolSectionType;

static ElfSymbolSectionType 		elf_symbol_section;
static ElfStringTableSectionType	elf_string_table_section;

Std_ReturnType elf_symbol_load(uint8 *elf_data)
{
	uint32 i;
	uint32 shdr_num;
	uint32 shdr_off;
	uint32 shdr_size;
	uint32 sec_shr_index;
	Elf32_Ehdr *hdr = (Elf32_Ehdr*)elf_data;
	Elf32_Shdr *shdr;

	sec_shr_index = hdr->e_shstrndx;
	shdr_num = hdr->e_shnum;
	shdr_off = hdr->e_shoff;
	shdr_size = hdr->e_shentsize;

	/*
	 * search section
	 */
	for (i = 0; i < shdr_num; i++) {
		shdr = (Elf32_Shdr *)&elf_data[(shdr_off + (i * shdr_size))];
		if (i == sec_shr_index) {
			continue;
		}

		if (shdr->sh_type == SHT_SYMTAB) {
			elf_symbol_section.shdr = shdr;
			elf_symbol_section.sym = (Elf32_Sym*)&elf_data[shdr->sh_offset];
			elf_symbol_section.symbol_num = shdr->sh_size / sizeof(Elf32_Sym);
			printf("ELF SYMBOL SECTION LOADED:index=%u\n", i);
			printf("ELF SYMBOL SECTION LOADED:sym_num=%u\n", elf_symbol_section.symbol_num);
		}
		else if (shdr->sh_type == SHT_STRTAB) {
			printf("ELF STRING TABLE SECTION LOADED:index=%u\n", i);
			elf_string_table_section.shdata = &elf_data[shdr->sh_offset];
			elf_string_table_section.shdr = shdr;
		}
	}

	if ((elf_symbol_section.shdr == NULL) || (elf_string_table_section.shdr == NULL)) {
		printf("ERROR: can not found symbol section...\n");
		return STD_E_INVALID;
	}
	elf_symbol_section.string_table = &elf_string_table_section;
	return STD_E_OK;
}

Std_ReturnType elfsym_get_symbol_num(uint32 *sym_num)
{
	if ((elf_symbol_section.shdr == NULL) || (elf_string_table_section.shdr == NULL)) {
		return STD_E_INVALID;
	}
	*sym_num = elf_symbol_section.symbol_num;
	return STD_E_OK;
}

Std_ReturnType elfsym_get_symbol(uint32 index, ElfSymbolType *elfsym)
{
	Elf32_Sym *sym;
	if ((elf_symbol_section.shdr == NULL) || (elf_string_table_section.shdr == NULL)) {
		return STD_E_INVALID;
	}
	sym = &elf_symbol_section.sym[index];
	elfsym->addr = sym->st_value;
	elfsym->size = sym->st_size;
	switch (sym->st_info) {
	case STT_OBJECT:
		elfsym->type = SYMBOL_TYPE_OBJECT;
		break;
	case STT_FUNC:
		elfsym->type = SYMBOL_TYPE_FUNC;
		break;
	case STT_NOTYPE:
		elfsym->type = SYMBOL_TYPE_NOTYPE;
		break;
	default:
		elfsym->type = SYMBOL_TYPE_NOSUP;
		break;
	}
	elfsym->name = (char*)&(elf_symbol_section.string_table->shdata[sym->st_name]);

	return STD_E_OK;
}

