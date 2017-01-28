#include "concrete_executor/util/dbg_print_data_type.h"
#include "dwarf/data_type/elf_dwarf_data_type.h"
#include "dwarf/data_type/elf_dwarf_base_type.h"
#include "dwarf/elf_section.h"
#include <stdio.h>

typedef struct {
	uint32 vaddr;
	uint32 current_addr;
	uint32 level;
} PrintControlType;

static bool print_any_data_type(PrintControlType *ctrl, DwarfDataType *obj, uint8 *top_addr, uint32 size);

static void print_base_type_address(uint8 *addr, uint32 size)
{
	uint32 value;
	value = elf_get_data32(addr, 0);
	printf("0x%x", value);
	return;
}
static void print_base_type_boolean(uint8 *addr, uint32 size)
{
	uint32 value = 0;
	switch (size) {
	case 1:
		value = elf_get_data8(addr, 0);
		break;
	case 2:
		value = elf_get_data16(addr, 0);
		break;
	case 4:
		value = elf_get_data32(addr, 0);
		break;
	default:
		break;
	}
	if (value == 0) {
		printf("FALSE");
	}
	else {
		printf("TRUE");
	}
	return;
}
static void print_base_type_signed(uint8 *addr, uint32 size)
{
	sint32 value = 0;
	switch (size) {
	case 1:
		value = ((sint8)elf_get_data8(addr, 0));
		break;
	case 2:
		value = ((sint16)elf_get_data16(addr, 0));
		break;
	case 4:
		value = ((sint32)elf_get_data32(addr, 0));
		break;
	default:
		break;
	}
	printf("%d", value);
	return;
}
static void print_base_type_unsigned(uint8 *addr, uint32 size)
{
	sint32 value = 0;
	switch (size) {
	case 1:
		value = ((uint8)elf_get_data8(addr, 0));
		break;
	case 2:
		value = ((uint16)elf_get_data16(addr, 0));
		break;
	case 4:
		value = ((uint32)elf_get_data32(addr, 0));
		break;
	default:
		break;
	}
	printf("%u", value);
	return;
}

static void print_base_type_data(DwAteType type, uint8 *addr, uint32 size)
{
	switch (type) {
	case DW_ATE_address:
		print_base_type_address(addr, size);
		break;
	case DW_ATE_boolean:
		print_base_type_boolean(addr, size);
		break;
	case DW_ATE_signed:
	case DW_ATE_signed_char:
		print_base_type_signed(addr, size);
		break;
	case DW_ATE_unsigned:
	case DW_ATE_unsigned_char:
		print_base_type_unsigned(addr, size);
		break;
	case DW_ATE_complex_float:
	case DW_ATE_float:
	default:
		printf("Unknown base type");
		break;
	}
	return;
}
static void print_space(PrintControlType *ctrl)
{
	int space_i;
	for (space_i = 0; space_i < (ctrl->level); space_i++) {
		printf("  ");
	}
	return;
}

static void print_struct_type_data(PrintControlType *ctrl, DwarfDataStructType *type, uint8 *top_addr, uint32 size)
{
	int i;
	printf("struct %s { \n", type->info.typename);
	for (i = 0; i < type->members->current_array_size; i++) {
		DwarfDataStructMember *memp = (DwarfDataStructMember *)type->members->data[i];
		print_space(ctrl);
		printf("%s = ", memp->name);
		if (memp->ref->type == DATA_TYPE_BASE) {
			DwarfDataBaseType *basetype = (DwarfDataBaseType *)memp->ref;
			print_base_type_data(basetype->encoding, &top_addr[memp->off], memp->ref->size);
			printf(" (%s:%u) @ 0x%x(0x%x)\n", memp->ref->typename, memp->ref->size, ctrl->current_addr + memp->off, memp->off);
		}
		else {
			uint32 org_addr = ctrl->current_addr;
			ctrl->current_addr += memp->off;
			(void)print_any_data_type(ctrl, memp->ref, &top_addr[memp->off], memp->ref->size);
			ctrl->current_addr = org_addr;
		}
	}
	print_space(ctrl);
	printf("}\n");
	return;
}
static bool print_any_data_type(PrintControlType *ctrl, DwarfDataType *obj, uint8 *top_addr, uint32 size)
{
	bool ret = FALSE;
	ctrl->level++;

	switch (obj->type) {
	case DATA_TYPE_BASE:
		break;
	case DATA_TYPE_ENUM:
		//TODO
		break;
	case DATA_TYPE_POINTER:
		//TODO
		break;
	case DATA_TYPE_TYPEDEF:
		//TODO
		break;
	case DATA_TYPE_STRUCT:
		print_struct_type_data(ctrl, (DwarfDataStructType *)obj, top_addr, size);
		ret = TRUE;
		break;
	case DATA_TYPE_ARRAY:
		//TODO
		break;
	case DATA_TYPE_UNION:
		//TODO
	default:
		break;
	}
	ctrl->level--;

	return ret;
}

bool print_variable_with_data_type(char *variable_name, uint32 vaddr, uint8 *top_addr, uint32 size)
{
	DwarfDataVariableType *variable;
	DwarfDataType *type;
	PrintControlType ctrl;

	variable = (DwarfDataVariableType *)dwarf_search_data_type(DATA_TYPE_VARIABLE, NULL, NULL, variable_name);
	if (variable == NULL || variable->ref == NULL) {
		return FALSE;
	}
	//printf("type(%u)=%s\n", variable->ref->type, variable->ref->typename);
	type = (DwarfDataType *)dwarf_search_data_type(variable->ref->type, NULL, NULL, variable->ref->typename);
	if (type == NULL) {
		return FALSE;
	}
	ctrl.vaddr = vaddr;
	ctrl.current_addr = vaddr;
	ctrl.level = 0;
	printf("vaddr=0x%x\n", vaddr);
	return print_any_data_type(&ctrl, type, top_addr, size);
}

