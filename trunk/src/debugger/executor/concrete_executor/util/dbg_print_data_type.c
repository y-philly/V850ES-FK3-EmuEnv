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

static bool print_any_data_type(PrintControlType *ctrl, DwarfDataType *obj, uint8 *top_addr, uint32 size, uint32 off);
static void print_ref_typename(PrintControlType *ctrl, DwarfDataType *type);

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

static void print_base_type_data(PrintControlType *ctrl, DwarfDataBaseType *obj, uint8 *addr, uint32 size, uint32 off)
{
	switch (obj->encoding) {
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
	printf(" (%s:%u) @ 0x%x(0x%x)\n", obj->info.typename, size, ctrl->current_addr + off, off);
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

static void print_struct_type_data(PrintControlType *ctrl, DwarfDataStructType *type, uint8 *top_addr, uint32 size, uint32 off)
{
	int i;

	printf("struct %s { \n", type->info.typename);
	for (i = 0; i < type->members->current_array_size; i++) {
		DwarfDataStructMember *memp = (DwarfDataStructMember *)type->members->data[i];
		print_space(ctrl);
		printf("%s = ", memp->name);
		if (memp->ref->type == DATA_TYPE_BASE) {
			DwarfDataBaseType *basetype = (DwarfDataBaseType *)memp->ref;
			print_base_type_data(ctrl, basetype, &top_addr[memp->off], memp->ref->size, memp->off);
		}
		else {
			uint32 org_addr = ctrl->current_addr;
			ctrl->current_addr += memp->off;
			(void)print_any_data_type(ctrl, memp->ref, &top_addr[memp->off], memp->ref->size, memp->off);
			ctrl->current_addr = org_addr;
		}
	}
	print_space(ctrl);
	printf("}\n");
	return;
}
static void print_typedef_type_data(PrintControlType *ctrl, DwarfDataTypedefType *type, uint8 *top_addr, uint32 size, uint32 off)
{
	printf("(typedef %s )", type->info.typename);
	print_any_data_type(ctrl, type->ref, top_addr, size, off);
	return;
}

static void print_ref_base_typename(PrintControlType *ctrl, DwarfDataBaseType *type)
{
	printf("%s", type->info.typename);
	return;
}
static void print_ref_enum_typename(PrintControlType *ctrl, DwarfDataEnumulatorType *type)
{
#if 0
	int i;
	printf("enum %s {\n", type->info.typename);
	for (i = 0; i < type->members->current_array_size; i++) {
		DwarfDataEnumMember *memp = (DwarfDataEnumMember *)type->members->data[i];
		print_space(ctrl);
		printf("%s\n", memp->name);
	}
	printf("}");
#else
	printf("enum %s", type->info.typename);
#endif
	return;
}

static void print_ref_pointer_typename(PrintControlType *ctrl, DwarfDataPointerType *type)
{
	print_ref_typename(ctrl, type->ref);
	printf(" *");
	return;
}
static void print_ref_typedef_typename(PrintControlType *ctrl, DwarfDataTypedefType *type)
{
	printf("%s", type->info.typename);
	return;
}
static void print_ref_struct_typename(PrintControlType *ctrl, DwarfDataStructType *type)
{
	printf("struct %s", type->info.typename);
	return;
}

static void print_ref_typename(PrintControlType *ctrl, DwarfDataType *type)
{
	switch (type->type) {
	case DATA_TYPE_BASE:
		print_ref_base_typename(ctrl, (DwarfDataBaseType *)type);
		break;
	case DATA_TYPE_ENUM:
		print_ref_enum_typename(ctrl, (DwarfDataEnumulatorType *)type);
		break;
	case DATA_TYPE_POINTER:
		print_ref_pointer_typename(ctrl, (DwarfDataPointerType *)type);
		break;
	case DATA_TYPE_TYPEDEF:
		print_ref_typedef_typename(ctrl, (DwarfDataTypedefType *)type);
		break;
	case DATA_TYPE_STRUCT:
		print_ref_struct_typename(ctrl, (DwarfDataStructType *)type);
		break;
	case DATA_TYPE_ARRAY:
		//TODO
		break;
	case DATA_TYPE_UNION:
		break;
	default:
		break;
	}
	return;
}

static void print_pointer_type_data(PrintControlType *ctrl, DwarfDataPointerType *type, uint8 *top_addr, uint32 size, uint32 off)
{
	printf("(");
	print_ref_typename(ctrl, (DwarfDataType *)type);
	printf(")");
	printf(" 0x%x", elf_get_data32(top_addr, 0));
	printf("  @ 0x%x(0x%x)\n", ctrl->current_addr + off, off);
	return;
}

static void print_enum_type_data(PrintControlType *ctrl, DwarfDataEnumulatorType *type, uint8 *top_addr, uint32 size, uint32 off)
{
	int i;
	DwarfDataEnumMember *memp = NULL;
	uint32 value;

	switch (size) {
	case 1:
		value = elf_get_data8(top_addr, 0);
		break;
	case 2:
		value = elf_get_data16(top_addr, 0);
		break;
	case 4:
		value = elf_get_data32(top_addr, 0);
		break;
	default:
		printf("unknown enum\n");
		return;
	}

	for (i = 0; i < type->members->current_array_size; i++) {
		memp = (DwarfDataEnumMember *)type->members->data[i];
		if (memp->const_value == value) {
			printf("%s.%s(%d)", type->info.typename, memp->name, memp->const_value);
			printf("  @ 0x%x(0x%x)\n", ctrl->current_addr + off, off);
			return;
		}
	}
	printf("unknown enum\n");

	return;
}

static bool print_any_data_type(PrintControlType *ctrl, DwarfDataType *obj, uint8 *top_addr, uint32 size, uint32 off)
{
	bool ret = FALSE;

	switch (obj->type) {
	case DATA_TYPE_BASE:
		print_base_type_data(ctrl, (DwarfDataBaseType *)obj, top_addr, size, off);
		ret = TRUE;
		break;
	case DATA_TYPE_ENUM:
		print_enum_type_data(ctrl, (DwarfDataEnumulatorType *)obj, top_addr, size, off);
		break;
	case DATA_TYPE_POINTER:
		print_pointer_type_data(ctrl, (DwarfDataPointerType *)obj, top_addr, size, off);
		ret = TRUE;
		break;
	case DATA_TYPE_TYPEDEF:
		print_typedef_type_data(ctrl, (DwarfDataTypedefType *)obj, top_addr, size, off);
		ret = TRUE;
		break;
	case DATA_TYPE_STRUCT:
		ctrl->level++;
		print_struct_type_data(ctrl, (DwarfDataStructType *)obj, top_addr, size, off);
		ctrl->level--;
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
	//printf("%s 0x%x ref=0x%x\n", variable->info.typename, variable->info.die->offset, variable->ref->die->offset);
	type = (DwarfDataType *)dwarf_search_data_type_from_die(variable->ref->type, variable->ref->die->offset);
	if (type == NULL) {
		return FALSE;
	}
	ctrl.vaddr = vaddr;
	ctrl.current_addr = vaddr;
	ctrl.level = 0;
	printf("%s = ", variable_name);
	return print_any_data_type(&ctrl, type, top_addr, size, 0);
}

