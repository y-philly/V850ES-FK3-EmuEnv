#include "elf_dwarf_data_type.h"
#include "elf_dwarf_info.h"
#include "elf_dwarf_base_type.h"
#include "elf_dwarf_struct_type.h"
#include "elf_dwarf_typedef_type.h"
#include "assert.h"
#include <string.h>

static ElfPointerArrayType	*dwarf_data_type_set[DATA_TYPE_NUM] = {
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
};

static void parse_array_type(ElfDwarfDieType *die);
static void parse_pointer_type(ElfDwarfDieType *die);

typedef  void (*parse_func_table_t)(ElfDwarfDieType *die);

static parse_func_table_t parse_func_table[DATA_TYPE_NUM] = {
		elf_dwarf_build_base_type,
		elf_dwarf_build_struct_type,
		parse_array_type,
		parse_pointer_type,
		elf_dwarf_build_typedef_type
};

static DwarfDataEnumType get_dataType(DwTagType tag)
{
	DwarfDataEnumType ret = DATA_TYPE_NUM;
	switch (tag) {
	case DW_TAG_array_type:
		ret = DATA_TYPE_ARRAY;
		break;
	case DW_TAG_pointer_type:
		ret = DATA_TYPE_POINTER;
		break;
	case DW_TAG_base_type:
		ret = DATA_TYPE_BASE;
		break;
	case DW_TAG_structure_type:
		ret = DATA_TYPE_STRUCT;
		break;
	case DW_TAG_typedef:
		ret = DATA_TYPE_TYPEDEF;
		break;
	default:
		break;
	}
	return ret;
}


static void parse_array_type(ElfDwarfDieType *die)
{
	//printf("array_type\n");
	return;
}
static void parse_pointer_type(ElfDwarfDieType *die)
{
	//printf("pointer_type\n");

	return;
}


static void dwarf_search_die_recursive(ElfDwarfDieType *die)
{
	int i_childs;
	ElfDwarfDieType *child;
	DwarfDataEnumType type;

	if (die->attribute == NULL) {
		return;
	}
	type = get_dataType(die->abbrev_info->tag);
	if (type != DATA_TYPE_NUM) {
		//printf("die(%u)=0x%x\n", die->level, die->abbrev_info->tag);
		parse_func_table[type](die);
	}
	if (die->children == NULL) {
		return;
	}
	for (i_childs = 0; i_childs < die->children->current_array_size; i_childs++) {
		child = die->children->data[i_childs];
		dwarf_search_die_recursive(child);
	}
	return;
}

static void build_types(void)
{
	int i_cu;
	int i_die;
	ElfDwarfCompilationUnitHeaderType	*cu;
	ElfDwarfDieType						*die;

	ElfPointerArrayType *compilation_unit_set = elf_dwarf_info_get();

	for (i_cu = 0; i_cu < compilation_unit_set->current_array_size; i_cu++) {
		cu = (ElfDwarfCompilationUnitHeaderType *)compilation_unit_set->data[i_cu];
		//printf("cu->offset=0x%x\n", cu->offset);
		if (cu->dies == NULL) {
			continue;
		}
		for (i_die = 0; i_die < cu->dies->current_array_size; i_die++) {
			die = (ElfDwarfDieType *)cu->dies->data[i_die];
			if (die->parent != NULL) {
				continue;
			}
			//printf("die->abbrev_code=%u\n", die->abbrev_code);
			//printf("die->children=0x%p\n", die->children);
			dwarf_search_die_recursive(die);
		}
	}

	return;
}

static void resolve_reference(void)
{
	return;
}

/*
 * 1) build definition
 * 2) resolve reference
 */
void dwarf_build_data_type_set(void)
{
	build_types();
	resolve_reference();
}

void *dwarf_alloc_data_type(DwarfDataEnumType type)
{
	uint32 size;
	DwarfDataType *obj;

	switch (type) {
	case DATA_TYPE_BASE:
		size = sizeof(DwarfDataBaseType);
		break;
	case DATA_TYPE_STRUCT:
		size = sizeof(DwarfDataStructType);
		break;
	case DATA_TYPE_ARRAY:
		size = sizeof(DwarfDataArrayType);
		break;
	case DATA_TYPE_POINTER:
		size = sizeof(DwarfDataPointerType);
		break;
	case DATA_TYPE_TYPEDEF:
		size = sizeof(DwarfDataTypedefType);
		break;
	default:
		ASSERT(0);
		break;
	}
	obj = (DwarfDataType *)elf_obj_alloc(size);
	obj->type = type;
	return obj;
}

void *dwarf_search_data_type(DwarfDataEnumType type, char *dirname, char *filename, char *typename)
{
	int i;
	int dirlen = strlen(dirname);
	int filelen = strlen(filename);
	int typelen = strlen(typename);

	if (type >= DATA_TYPE_NUM) {
		return NULL;
	}
	if (dwarf_data_type_set[type] == NULL) {
		return NULL;
	}

	for (i = 0; i < dwarf_data_type_set[type]->current_array_size; i++) {
		int len;
		DwarfDataType *entry = (DwarfDataType *)dwarf_data_type_set[type]->data[i];

		len = strlen(entry->typename);
		if (typelen != len) {
			continue;
		}
		if (strncmp(typename, entry->typename, len) == 0) {
			continue;
		}
		len = strlen(entry->dirname);
		if (dirlen != len) {
			continue;
		}
		if (strncmp(dirname, entry->dirname, len) == 0) {
			continue;
		}
		len = strlen(entry->filename);
		if (filelen != len) {
			continue;
		}
		if (strncmp(filename, entry->filename, len) == 0) {
			continue;
		}

		return entry;
	}
	return NULL;
}
void dwarf_register_data_type(DwarfDataType *entry)
{
	if (entry->type >= DATA_TYPE_NUM) {
		return;
	}
	if (dwarf_data_type_set[entry->type] == NULL) {
		dwarf_data_type_set[entry->type] = elf_array_alloc();
	}

	elf_array_add_entry(dwarf_data_type_set[entry->type], entry);
	return;
}


void dwarf_add_struct_member(DwarfDataStructType *obj, char *name, uint32 off, DwarfDataType *ref)
{
	DwarfDataStructMember *member;

	member = (DwarfDataStructMember*)elf_obj_alloc(sizeof(DwarfDataStructMember));
	member->name = name;
	member->off = off;
	member->ref = ref;
	if (obj->members == NULL) {
		obj->members = elf_array_alloc();
	}

	elf_array_add_entry(obj->members, member);

	return;
}
