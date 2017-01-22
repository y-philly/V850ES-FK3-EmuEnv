#include "elf_dwarf_struct_type.h"
#include "assert.h"
#include <string.h>

void elf_dwarf_build_struct_type(ElfDwarfDieType *die)
{
	uint32 size;
	int i;
	int j;
	DwarfDataStructType *obj;
	ElfDwarfAttributeType *attr;
	ElfDwarfAbbrevType *abbrev;
	DwAtType attr_type;
	ElfDwarfDieType *member;
	uint32 offset;
	Std_ReturnType err;

	if (die->abbrev_info->tag == DW_TAG_structure_type) {
		obj = dwarf_alloc_data_type(DATA_TYPE_STRUCT);
	}
	else {
		obj = dwarf_alloc_data_type(DATA_TYPE_UNION);
	}


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

	/*
	 * member
	 */
	for (i = 0; i < die->children->current_array_size; i++) {
		DwarfDataStructMember mem;
		member = (ElfDwarfDieType*)die->children->data[i];
		abbrev = (ElfDwarfAbbrevType *)member->abbrev_info;
		if (member->abbrev_info->tag != DW_TAG_member) {
			continue;
		}
		memset(&mem, 0, sizeof(mem));
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
				offset = elf_dwarf_info_get_value(abbrev->attribute_form->data[j], attr, &size);
				err = dwarf_get_real_type_offset(offset, &mem.ref_debug_info_offset);
				if (err == STD_E_OK) {
					mem.is_valid_ref_debug_info_offset = TRUE;
				}
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
		dwarf_add_struct_member(obj, &mem);
		//printf("mem=%s\n", mem.name);
	}

	obj->info.die = die;
	//printf("struct_type=%s off=0x%x\n", obj->info.typename, die->offset);
	//printf("struct_size=%u\n", obj->info.size);
	//TODO
	//printf("DW_AT_TYPE=0x%x\n", value);
	dwarf_register_data_type(&obj->info);
	return;
}

static void elf_dwarf_resolve_struct_union_member(DwarfDataStructType *struct_obj)
{
	int i;
	DwarfDataStructMember *obj;

	//printf("struct or union:%s\n", struct_obj->info.typename);
	for (i = 0; i < struct_obj->members->current_array_size; i++) {
		obj = (DwarfDataStructMember *)struct_obj->members->data[i];
		//printf("member %s ref=%p flag=%u;\n", obj->name, obj->ref, obj->is_valid_ref_debug_info_offset);
		if (obj->ref != NULL) {
			continue;
		}
		if (obj->is_valid_ref_debug_info_offset == FALSE) {
			continue;
		}
		obj->ref = elf_dwarf_get_data_type(obj->ref_debug_info_offset);
		if (obj->ref == NULL) {
			//printf("Not supported:unknown typeref(%s) debug_offset=0x%x\n", obj->ref->typename, obj->ref_debug_info_offset);
		}
		else {
			//printf("member %s %s;\n", obj->ref->typename, obj->name);
		}

	}
}

static void elf_dwarf_resolve_union(void)
{
	int i;
	ElfPointerArrayType	*my_types = dwarf_get_data_types(DATA_TYPE_UNION);
	DwarfDataStructType *obj;

	if (my_types == NULL) {
		return;
	}

	for (i = 0; i < my_types->current_array_size; i++) {
		obj = (DwarfDataStructType *)my_types->data[i];
		elf_dwarf_resolve_struct_union_member(obj);
	}
	return;
}
static void elf_dwarf_resolve_struct(void)
{
	int i;
	ElfPointerArrayType	*my_types = dwarf_get_data_types(DATA_TYPE_STRUCT);
	DwarfDataStructType *obj;

	if (my_types == NULL) {
		return;
	}

	for (i = 0; i < my_types->current_array_size; i++) {
		obj = (DwarfDataStructType *)my_types->data[i];
		elf_dwarf_resolve_struct_union_member(obj);
	}
	return;
}

void elf_dwarf_resolve_struct_type(void)
{
	elf_dwarf_resolve_struct();
	elf_dwarf_resolve_union();
	return;
}

