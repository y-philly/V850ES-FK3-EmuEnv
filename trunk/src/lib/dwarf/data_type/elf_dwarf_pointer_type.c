#include "elf_dwarf_pointer_type.h"
#include "assert.h"

void elf_dwarf_build_pointer_type(ElfDwarfDieType *die)
{
	int i;
	uint32 size;
	DwarfDataPointerType *obj = dwarf_alloc_data_type(DATA_TYPE_POINTER);
	ElfDwarfAttributeType *attr;
	ElfDwarfAbbrevType *abbrev;
	DwAtType attr_type;
	uint32 offset;

	obj->info.typename = "*";

	for (i = 0; i < die->attribute->current_array_size; i++) {
		abbrev = (ElfDwarfAbbrevType *)die->abbrev_info;
		attr = (ElfDwarfAttributeType*)die->attribute->data[i];
		attr_type = abbrev->attribute_name->data[i];
		switch (attr_type) {
		case DW_AT_byte_size:
			obj->info.size = elf_dwarf_info_get_value(abbrev->attribute_form->data[i], attr, &size);
			break;
		case DW_AT_type:
			offset = elf_dwarf_info_get_value(abbrev->attribute_form->data[i], attr, &size);
			obj->ref_debug_info_offset = dwarf_get_real_type_offset(offset);
			break;
		default:
			ASSERT(0);
		}
	}
	obj->info.die = die;

	dwarf_register_data_type(&obj->info);

	return;
}

void elf_dwarf_resolve_pointer_type(void)
{
	int i;
	ElfPointerArrayType	*my_types = dwarf_get_data_types(DATA_TYPE_POINTER);
	DwarfDataPointerType *obj;

	for (i = 0; i < my_types->current_array_size; i++) {
		obj = (DwarfDataPointerType *)my_types->data[i];
		if (obj->ref != NULL) {
			continue;
		}
		obj->ref = elf_dwarf_get_data_type(obj->ref_debug_info_offset);
		if (obj->ref == NULL) {
			printf("Not supported:unknown typeref(%s) debug_offset=0x%x\n", obj->info.typename, obj->ref_debug_info_offset);
		}
		else {
			printf("pointer %s %s\n", obj->ref->typename, obj->info.typename);
		}
	}
	return;
}
