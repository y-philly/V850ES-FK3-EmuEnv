#include "elf_dwarf_struct_type.h"
#include "assert.h"

void elf_dwarf_build_struct_type(ElfDwarfDieType *die)
{
	uint32 size;
	int i;
	int j;
	DwarfDataStructType *obj = dwarf_alloc_data_type(DATA_TYPE_STRUCT);
	ElfDwarfAttributeType *attr;
	ElfDwarfAbbrevType *abbrev;
	DwAtType attr_type;
	uint32 value;
	ElfDwarfDieType *member;

	//printf("struct_type\n");
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
			value = elf_dwarf_info_get_value(abbrev->attribute_form->data[i], attr, &size);
			break;
		case DW_AT_byte_size:
			obj->info.size = elf_dwarf_info_get_value(abbrev->attribute_form->data[i], attr, &size);
			break;
		case DW_AT_sibling:
		case DW_AT_decl_file:
		case DW_AT_decl_line:
			break;
		default:
			ASSERT(0);
		}
	}

	for (i = 0; i < die->children->current_array_size; i++) {
		DwarfDataStructMember mem;
		member = (ElfDwarfDieType*)die->children->data[i];
		abbrev = (ElfDwarfAbbrevType *)member->abbrev_info;
		if (member->abbrev_info->tag != DW_TAG_member) {
			continue;
		}
		for (j = 0; j < member->attribute->current_array_size; j++) {
			attr = (ElfDwarfAttributeType*)member->attribute->data[j];
			attr_type = abbrev->attribute_name->data[j];
			//printf("member name=0x%x form=%s\n", attr_type, attr->typename);
			switch (attr_type) {
			case DW_AT_name:
				mem.name = attr->encoded.string;
				break;
			case DW_AT_type:
				//value = elf_dwarf_info_get_value(abbrev->attribute_form->data[j], attr, &size);
				break;
			case DW_AT_byte_size:
				break;
			case DW_AT_data_member_location:
			case DW_AT_decl_file:
			case DW_AT_decl_line:
				break;
			default:
				ASSERT(0);
			}
		}
		dwarf_add_struct_member(obj, mem.name, 0, NULL);
		//printf("mem=%s\n", mem.name);
	}

	obj->info.die = die;
	//printf("struct_type=%s\n", obj->info.typename);
	//printf("struct_size=%u\n", obj->info.size);
	//TODO
	//printf("DW_AT_TYPE=0x%x\n", value);
	dwarf_register_data_type(&obj->info);
	return;
}

