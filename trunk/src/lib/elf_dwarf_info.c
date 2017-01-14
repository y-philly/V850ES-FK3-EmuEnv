#include "elf_dwarf_info.h"
#include "elf_section.h"
#include "assert.h"
#include <string.h>

#if 1
#include <stdio.h>
#define DBG_PRINTF(arg)	printf arg
#else
#define DBG_PRINTF(arg)
#endif

static char *debug_str = NULL;

static DwFormEncodingType elf_dwarf_info_attrform_enctype(ElfDwarfCompilationUnitHeaderType *cu, uint8 *addr, DwFormType form, uint32 *size)
{
	DwFormEncodingType ret = DW_FORM_ENC_UNKNOWN;
	*size = 0;
	switch (form) {
	case DW_FORM_addr:
		ret = DW_FORM_ENC_ADDRESS;
		*size = cu->pointer_size;
		//printf("addr=0x%x\n", elf_get_data32(addr, 0));
		break;
	case DW_FORM_block:
		ASSERT(0);
		ret = DW_FORM_ENC_BLOCK;
		break;
	case DW_FORM_block1:
		*size = elf_get_data8(addr, 0) + 1;
		ret = DW_FORM_ENC_BLOCK;
		break;
	case DW_FORM_block2:
		*size = elf_get_data16(addr, 0) + 2;
		ret = DW_FORM_ENC_BLOCK;
		break;
	case DW_FORM_block4:
		*size = elf_get_data32(addr, 0) + 4;
		ret = DW_FORM_ENC_BLOCK;
		break;
	case DW_FORM_data1:
		*size = 1;
		ret = DW_FORM_ENC_CONSTANT;
		//printf("data1=0x%x\n", elf_get_data8(addr, 0));
		break;
	case DW_FORM_data2:
		*size = 2;
		ret = DW_FORM_ENC_CONSTANT;
		//printf("data2=0x%x\n", elf_get_data16(addr, 0));
		break;
	case DW_FORM_data4:
		*size = 4;
		ret = DW_FORM_ENC_CONSTANT;
		//printf("data4=0x%x\n", elf_get_data32(addr, 0));
		break;
	case DW_FORM_data8:
		*size = 8;
		ret = DW_FORM_ENC_CONSTANT;
		break;
	case DW_FORM_sdata:
		ASSERT(0);
		break;
	case DW_FORM_udata:
		ASSERT(0);
		ret = DW_FORM_ENC_CONSTANT;
		break;
	case DW_FORM_string:
		*size = strlen((char*)addr) + 1;
		ret = DW_FORM_ENC_STRING;
		//printf("string=%s\n", (char*)addr);
		break;
	case DW_FORM_strp://TODO
		*size = 4;
		{
			uint32 off = elf_get_data32(addr, 0);
			//printf("indirect string=%s\n", &debug_str[off]);
		}
		ret = DW_FORM_ENC_STRING;
		break;
	case DW_FORM_flag:
		*size = 1;
		ret = DW_FORM_ENC_FLAG;
		break;
	case DW_FORM_ref_addr:
		ASSERT(0);
		ret = DW_FORM_ENC_REFERENCE;
		break;
	case DW_FORM_ref1:
		*size = 1;
		ret = DW_FORM_ENC_REFERENCE;
		break;
	case DW_FORM_ref2:
		*size = 2;
		ret = DW_FORM_ENC_REFERENCE;
		break;
	case DW_FORM_ref4:
		*size = 4;
		ret = DW_FORM_ENC_REFERENCE;
		break;
	case DW_FORM_ref8:
		*size = 8;
		ret = DW_FORM_ENC_REFERENCE;
		break;
	case DW_FORM_ref_udata:
		ASSERT(0);
		ret = DW_FORM_ENC_REFERENCE;
		break;
	case DW_FORM_indirect:
		ASSERT(0);
		ret = DW_FORM_ENC_INDIRECT;
		break;
	default:
		break;
	}
	return ret;
}

Std_ReturnType elf_dwarf_info_load(uint8 *elf_data)
{
	uint8 *section_data;
	Std_ReturnType err;
	uint32 section_size;
	uint32 current_size = 0;
	uint32	header_size;
	ElfDwarfCompilationUnitHeaderType	cuh;
	ElfDwarfAbbrevType *entry;
	ElfDwarfAbbrevType *top;
	uint32 size;
	uint32 entry_size;
	uint8 *addr;
	uint32 code;
	int i;

	err = elf_section_get(elf_data, SECTION_DWARF_STR_NAME, &debug_str, &section_size);
	if (err != STD_E_OK) {
		return err;
	}

	err = elf_section_get(elf_data, SECTION_DWARF_INFO_NAME, &section_data, &section_size);
	if (err != STD_E_OK) {
		return err;
	}
	DBG_PRINTF(("**section_size=%u\n", section_size));

	header_size = (4 + 2 + 4 + 1);
	while (current_size < section_size) {
		DBG_PRINTF(("**current_size=0x%x ******\n", current_size));

		cuh.length = elf_get_data32(&section_data[current_size], 0);
		cuh.version = elf_get_data16(&section_data[current_size], 4);
		cuh.abbrev_offset = elf_get_data32(&section_data[current_size], 6);
		cuh.pointer_size = elf_get_data8(&section_data[current_size], 10);

		top = elf_dwarf_abbrev_get(cuh.abbrev_offset);
		ASSERT(top != NULL);

		DBG_PRINTF(("**length=0x%x\n", cuh.length));
		DBG_PRINTF(("**version=0x%x\n", cuh.version));
		DBG_PRINTF(("**offset=0x%x\n", cuh.abbrev_offset));
		DBG_PRINTF(("**pointer_size=0x%x\n", cuh.pointer_size));
		DBG_PRINTF(("**top=0x%x\n", top));

		entry_size = header_size;
		while (entry_size < (cuh.length + 4)) {
			code = elf_dwarf_decode_uleb128(&section_data[current_size + entry_size], &size);
			DBG_PRINTF(("<%x>    entry_code=0x%x\n", current_size + entry_size, code));
			entry_size += size;
			if (code == 0x00) {
				continue;
			}
			entry = elf_dwarf_abbrev_get_from_code(top, code);
			ASSERT(entry != NULL);
			DBG_PRINTF(("    code=0x%x tag=0x%x\n", entry->code, entry->tag));
			for (i = 0; i < entry->attribute_name->current_array_size; i++) {
				addr = &section_data[current_size + entry_size];
				DBG_PRINTF(("<%x>    name=0x%x form=0x%x form_type=%u\n",
						current_size + entry_size,
						entry->attribute_name->data[i],
						entry->attribute_form->data[i],
						elf_dwarf_info_attrform_enctype(&cuh, addr, entry->attribute_form->data[i], &size)));
				entry_size += size;
			}
			DBG_PRINTF(("    entry_size=0x%x\n", entry_size));
		}
		ASSERT(entry_size == (cuh.length + 4));
		current_size += entry_size;

	}
	return STD_E_OK;
}
