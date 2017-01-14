#include "elf_dwarf_info.h"
#include "elf_section.h"
#include "assert.h"
#include <string.h>

#if 0
#include <stdio.h>
#define DBG_PRINTF(arg)	printf arg
#else
#define DBG_PRINTF(arg)
#endif

static char *debug_str = NULL;
static ElfPointerArrayType *compilation_unit_headers = NULL;

static ElfDwarfAttributeType *elf_dwarf_alloc_empty_ElfDwarfAttribute(void)
{
	return (ElfDwarfAttributeType *)elf_obj_alloc(sizeof(ElfDwarfAttributeType));
}
static ElfDwarfCompilationUnitHeaderType *elf_dwarf_alloc_empty_ElfDwarfCompilationUnitHeader(void)
{
	ElfDwarfCompilationUnitHeaderType *obj;

	obj = (ElfDwarfCompilationUnitHeaderType *)elf_obj_alloc(sizeof(ElfDwarfCompilationUnitHeaderType));

	obj->dies = elf_array_alloc();
	return obj;
}
static ElfDwarfDieType *elf_dwarf_alloc_empty_ElfDwarfDie(void)
{
	ElfDwarfDieType *obj;

	obj = (ElfDwarfDieType *)elf_obj_alloc(sizeof(ElfDwarfDieType));

	obj->attribute = elf_array_alloc();
	return obj;
}

static void elf_dwarf_info_parse_attr(ElfDwarfCompilationUnitHeaderType *cu, ElfDwarfDieType *die, uint8 *addr, DwFormType form, uint32 *size)
{
	ElfDwarfAttributeType *obj = NULL;

	*size = 0;
	switch (form) {
	case DW_FORM_none:
		break;
	case DW_FORM_addr:
		*size = cu->pointer_size;
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.addr = elf_get_data32(addr, 0);
		obj->typename = "DW_FORM_addr";
		break;
	case DW_FORM_block:
		ASSERT(0);
		break;
	case DW_FORM_block1:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.op.len = elf_get_data8(addr, 0);
		obj->encoded.op.ops = &addr[1];
		*size = obj->encoded.op.len + 1;
		obj->typename = "DW_FORM_block1";
		break;
	case DW_FORM_block2:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.op.len = elf_get_data16(addr, 0);
		obj->encoded.op.ops = &addr[2];
		*size = obj->encoded.op.len + 2;
		obj->typename = "DW_FORM_block2";
		break;
	case DW_FORM_block4:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.op.len = elf_get_data32(addr, 0);
		obj->encoded.op.ops = &addr[4];
		obj->typename = "DW_FORM_block4";
		*size = obj->encoded.op.len + 4;
		break;
	case DW_FORM_data1:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.const8 = elf_get_data8(addr, 0);
		obj->typename = "DW_FORM_data1";
		*size = 1;
		break;
	case DW_FORM_data2:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.const16 = elf_get_data16(addr, 0);
		obj->typename = "DW_FORM_data2";
		*size = 2;
		break;
	case DW_FORM_data4:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.const32 = elf_get_data32(addr, 0);
		obj->typename = "DW_FORM_data4";
		*size = 4;
		break;
	case DW_FORM_data8:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.const64 = elf_get_data64(addr, 0);
		obj->typename = "DW_FORM_data8";
		*size = 8;
		break;
	case DW_FORM_sdata:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.scont64 = elf_dwarf_decode_sleb128(addr, size);
		obj->typename = "DW_FORM_sdata";
		break;
	case DW_FORM_udata:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.const64 = elf_dwarf_decode_uleb128(addr, size);
		obj->typename = "DW_FORM_udata";
		break;
	case DW_FORM_string:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.string = (char*)addr;
		*size = strlen((char*)addr) + 1;
		obj->typename = "DW_FORM_string";
		break;
	case DW_FORM_strp:
	{
		uint32 off = elf_get_data32(addr, 0);
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.string = &debug_str[off];
		obj->typename = "DW_FORM_strp";
		*size = 4;
	}
		break;
	case DW_FORM_flag:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.flag = *addr;
		obj->typename = "DW_FORM_flag";
		*size = 1;
		break;
	case DW_FORM_ref_addr:
		ASSERT(0);
		break;
	case DW_FORM_ref1:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.ref8 = elf_get_data8(addr, 0);
		obj->typename = "DW_FORM_ref1";
		*size = 1;
		break;
	case DW_FORM_ref2:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.ref16 = elf_get_data16(addr, 0);
		obj->typename = "DW_FORM_ref2";
		*size = 2;
		break;
	case DW_FORM_ref4:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.ref32 = elf_get_data32(addr, 0);
		obj->typename = "DW_FORM_ref4";
		*size = 4;
		break;
	case DW_FORM_ref8:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.ref64 = elf_get_data64(addr, 0);
		obj->typename = "DW_FORM_ref8";
		*size = 8;
		break;
	case DW_FORM_ref_udata:
		ASSERT(0);
		break;
	case DW_FORM_indirect:
		ASSERT(0);
		break;
	case DW_FORM_sec_offset:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.sec_offset = elf_get_data32(addr, 0);
		obj->typename = "DW_FORM_sec_offset";
		*size = 4;
		break;
	case DW_FORM_exprloc:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.op.len = elf_dwarf_decode_uleb128(addr, size);
		obj->encoded.op.ops = &addr[*size];
		obj->typename = "DW_FORM_exprloc";
		*size += obj->encoded.op.len;
		break;
	case DW_FORM_flag_present:
		obj = elf_dwarf_alloc_empty_ElfDwarfAttribute();
		obj->encoded.flag = TRUE;
		obj->typename = "DW_FORM_flag_present";
		*size = 0;
		break;
	case DW_FORM_ref_sig8:
		ASSERT(0);
		break;
	default:
		ASSERT(0);
		break;
	}

	if (obj != NULL) {
		obj->type = form;
		elf_array_add_entry(die->attribute, obj);
	}
	return;
}

Std_ReturnType elf_dwarf_info_load(uint8 *elf_data)
{
	uint8 *section_data;
	Std_ReturnType err;
	uint32 section_size;
	uint32 current_size = 0;
	uint32	header_size;
	ElfDwarfCompilationUnitHeaderType	*cu;
	ElfDwarfAbbrevType *entry;
	ElfDwarfAbbrevType *top;
	uint32 size;
	uint32 entry_size;
	uint8 *addr;
	uint32 code;
	ElfDwarfDieType *die;
	int i;

	compilation_unit_headers = elf_array_alloc();

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

		cu = elf_dwarf_alloc_empty_ElfDwarfCompilationUnitHeader();

		cu->length = elf_get_data32(&section_data[current_size], 0);
		cu->version = elf_get_data16(&section_data[current_size], 4);
		cu->abbrev_offset = elf_get_data32(&section_data[current_size], 6);
		cu->pointer_size = elf_get_data8(&section_data[current_size], 10);

		top = elf_dwarf_abbrev_get(cu->abbrev_offset);
		ASSERT(top != NULL);

		DBG_PRINTF(("**length=0x%x\n", cu->length));
		DBG_PRINTF(("**version=0x%x\n", cu->version));
		DBG_PRINTF(("**offset=0x%x\n", cu->abbrev_offset));
		DBG_PRINTF(("**pointer_size=0x%x\n", cu->pointer_size));
		DBG_PRINTF(("**top=0x%x\n", top));

		entry_size = header_size;
		while (entry_size < (cu->length + 4)) {
			code = elf_dwarf_decode_uleb128(&section_data[current_size + entry_size], &size);
			DBG_PRINTF(("<%x>    entry_code=0x%x\n", current_size + entry_size, code));
			entry_size += size;
			if (code == 0x00) {
				continue;
			}
			entry = elf_dwarf_abbrev_get_from_code(top, code);
			ASSERT(entry != NULL);
			die = elf_dwarf_alloc_empty_ElfDwarfDie();
			die->abbrev_code = code;
			die->abbrev_info = entry;

			DBG_PRINTF(("    code=0x%x tag=0x%x\n", entry->code, entry->tag));
			for (i = 0; i < entry->attribute_name->current_array_size; i++) {
				addr = &section_data[current_size + entry_size];
				elf_dwarf_info_parse_attr(cu, die, addr, entry->attribute_form->data[i], &size);

				DBG_PRINTF(("<%x>    name=0x%x form=0x%x\n",
						current_size + entry_size,
						entry->attribute_name->data[i],
						entry->attribute_form->data[i]));
				entry_size += size;
			}

			elf_array_add_entry(cu->dies, die);
			DBG_PRINTF(("    entry_size=0x%x\n", entry_size));
		}
		ASSERT(entry_size == (cu->length + 4));
		current_size += entry_size;

		elf_array_add_entry(compilation_unit_headers, cu);
	}
	return STD_E_OK;
}
