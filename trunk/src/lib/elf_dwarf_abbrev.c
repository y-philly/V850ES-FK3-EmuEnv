#include "elf_dwarf_abbrev.h"
#include "elf_section.h"
#include <stdio.h>

#if 0
#define DBG_PRINTF(arg)	printf arg
#else
#define DBG_PRINTF(arg)
#endif

static ElfPointerArrayType *elf_dwarf_abbrev_array = NULL;

static void print_ElfDwarfAbbrev(ElfDwarfAbbrevType *entry)
{
	int i;

	printf("******************\n");
	printf("code=0x%x\n", entry->code);
	printf("tag=0x%x\n", entry->tag);
	printf("childs=0x%x\n", entry->child);

	for (i = 0; i < entry->attribute_name->current_array_size; i++) {
		printf("%u : name=0x%x form=0x%x\n", i, entry->attribute_name->data[i], entry->attribute_form->data[i]);
	}

	return;
}

Std_ReturnType elf_dwarf_abbrev_load(uint8 *elf_data)
{
	uint8 *section_data;
	Std_ReturnType err;
	uint32 section_size;
	uint32 current_size = 0;
	uint32 entry_size;
	uint32 attr_name;
	uint32 attr_form;
	uint32 code;
	ElfDwarfAbbrevType *entry;

	err = elf_section_get(elf_data, SECTION_DWARF_ABBREV_NAME, &section_data, &section_size);
	if (err != STD_E_OK) {
		return err;
	}
	DBG_PRINTF(("**section_size=%u\n", section_size));

	elf_dwarf_abbrev_array = elf_array_alloc();

	while (current_size < section_size) {
		code = elf_dwarf_decode_uleb128(&section_data[current_size], &entry_size);
		DBG_PRINTF(("**current_size=0x%x code=0x%x******\n", current_size, code));
		if (code == 0) {
			current_size += entry_size;
			continue;
		}
		current_size += entry_size;
		/*
		 * header
		 */
		entry = elf_dwarf_abbrev_alloc_empty_ElfDwarfAbbrev();

		/*
		 * set abbrev code
		 */
		entry->code = code;
		/*
		 * set tag
		 */
		entry->tag = elf_dwarf_decode_uleb128(&section_data[current_size], &entry_size);
		current_size += entry_size;

		/*
		 * childs
		 */
		entry->child = section_data[current_size];
		current_size += 1;

		/*
		 * set attribute
		 */
		do {
			attr_name = elf_dwarf_decode_uleb128(&section_data[current_size], &entry_size);
			current_size += entry_size;
			attr_form = elf_dwarf_decode_uleb128(&section_data[current_size], &entry_size);
			current_size += entry_size;

			dwarf_uint32_array_add_entry(entry->attribute_name, attr_name);
			dwarf_uint32_array_add_entry(entry->attribute_form, attr_form);
		} while (attr_name != 0);

		print_ElfDwarfAbbrev(entry);
		elf_array_add_entry(elf_dwarf_abbrev_array, entry);
	}
	DBG_PRINTF(("END\n"));

	return STD_E_OK;
}

ElfDwarfAbbrevType *elf_dwarf_abbrev_alloc_empty_ElfDwarfAbbrev(void)
{
	ElfDwarfAbbrevType *obj;

	obj = elf_obj_alloc(sizeof(ElfDwarfAbbrevType));
	obj->attribute_name = dwarf_uint32_array_alloc();
	obj->attribute_form = dwarf_uint32_array_alloc();
	return obj;
}
