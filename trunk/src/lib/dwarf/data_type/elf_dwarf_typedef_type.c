#include "elf_dwarf_typedef_type.h"
#include "assert.h"

void elf_dwarf_build_typedef_type(ElfDwarfDieType *die)
{
	int i;
	uint32 size;
	DwarfDataTypedefType *obj = dwarf_alloc_data_type(DATA_TYPE_TYPEDEF);
	ElfDwarfAttributeType *attr;
	ElfDwarfAbbrevType *abbrev;
	DwAtType attr_type;

	//printf("typedef_type\n");
	for (i = 0; i < die->attribute->current_array_size; i++) {
		abbrev = (ElfDwarfAbbrevType *)die->abbrev_info;
		attr = (ElfDwarfAttributeType*)die->attribute->data[i];
		attr_type = abbrev->attribute_name->data[i];
		//printf("name=0x%x form=%s\n", attr_type, attr->typename);
		switch (attr_type) {
		case DW_AT_name:
			obj->info.typename = attr->encoded.string;
			break;
		case DW_AT_type:
			obj->ref_debug_info_offset = elf_dwarf_info_get_value(abbrev->attribute_form->data[i], attr, &size);
			break;
		case DW_AT_decl_file:
		case DW_AT_decl_line:
			break;
		default:
			ASSERT(0);
		}
	}
	obj->info.die = die;
	//printf("typedef=%s\n", obj->info.typename);

	dwarf_register_data_type(&obj->info);

	return;
}
