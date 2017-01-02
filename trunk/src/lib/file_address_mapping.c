#include "file_address_mapping.h"
#include "elf_dwarf_line.h"
#include "assert.h"

typedef struct {
	KeyAddressType	key;
	ValueFileType	value;
} KeyValueMappingType;

static ElfPointerArrayType *key_value_mapping;
static void file_address_mapping_add(KeyAddressType *key, ValueFileType *value);
static void file_address_mapping_build(void);

void file_address_mapping_init(void)
{
	key_value_mapping = elf_array_alloc();
	file_address_mapping_build();

	return;
}
static void do_ext_build(ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine);
static void do_std_build(ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine);
static void do_spc_build(ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine);
static void add_map(KeyValueMappingType *map, ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine);

static void file_address_mapping_build(void)
{
	uint32 entry_inx;
	uint32 op_inx;
	ElfDwarfLineStateMachineRegisterType machine;
	ElfDwarfLineType *edl = elf_dwarf_line_get_ElfDwarfLine();

	printf("file_address_mapping_build:enter:size=%u\n", edl->entries->current_array_size);
	for (entry_inx = 0; entry_inx < edl->entries->current_array_size; entry_inx++) {
		ElfDwarfLineEntryType *entry = (ElfDwarfLineEntryType *)edl->entries->data[entry_inx];
		elf_dwarf_line_init_ElfDwarfLineStateMachineRegister(&machine, entry->header);

		printf("file_address_mapping_build:op_num=%u\n", entry->ops->current_array_size);
		for (op_inx = 0; op_inx < entry->ops->current_array_size; op_inx++) {
			ElfDwarfLineParsedOpCodeType *op = (ElfDwarfLineParsedOpCodeType *)entry->ops->data[op_inx];

			switch (op->type) {
			case OPTYPE_EXTENDED:
				do_ext_build(op, &machine);
				break;
			case OPTYPE_SPECIAL:
				do_spc_build(op, &machine);
				break;
			case OPTYPE_STANDARD:
				do_std_build(op, &machine);
				break;
			default:
				break;
			}
		}

	}
	printf("file_address_mapping_build:exit\n");
	return;
}

static void file_address_mapping_add(KeyAddressType *key, ValueFileType *value)
{
	KeyValueMappingType *obj = elf_obj_alloc(sizeof(KeyValueMappingType));
	obj->key = *key;
	obj->value = *value;
	elf_array_add_entry(key_value_mapping, obj);

	printf("0x%x - 0x%x : %s/%s %d\n",
			key->address_start, key->address_end,
			value->dir, value->file,
			value->line);
	return;
}

Std_ReturnType file_address_mapping_get(uint32 addr, ValueFileType *value)
{
	uint32 i;
	KeyValueMappingType *map;

	for (i = 0; i < key_value_mapping->current_array_size; i++) {
		map = (KeyValueMappingType *)key_value_mapping->data[i];
		if ((addr >= map->key.address_start) && (addr < map->key.address_end)) {
			*value = map->value;
			return STD_E_OK;
		}
	}
	return STD_E_NOENT;
}
Std_ReturnType file_address_mapping_get_last(KeyAddressType *key, ValueFileType *value)
{
	KeyValueMappingType *map;
	if (key_value_mapping->current_array_size == 0) {
		return STD_E_NOENT;
	}
	map = (KeyValueMappingType *)key_value_mapping->data[key_value_mapping->current_array_size - 1];
	*key = map->key;
	*value = map->value;
	return STD_E_OK;
}


static void do_ext_build(ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine)
{
	Std_ReturnType err;
	KeyAddressType key;
	ValueFileType value;

	//printf("do_ext_build:op->subtype=%d\n", op->subtype);
	switch (op->subtype) {
	case DW_LNE_end_sequence:
		err = file_address_mapping_get_last(&key, &value);
		if (err == STD_E_OK) {
			KeyValueMappingType map;
			map.key.address_start = key.address_end;
			map.key.address_end = machine->address;
			map.value.line = machine->line;
			add_map(&map, op, machine);
		}
		break;
	case DW_LNE_set_address:
		machine->address = op->args.extSetAddress.addr;
		break;
	case DW_LINE_define_file:
		ASSERT(0);
		break;
	case DW_LNE_set_discriminator:
		break;
	default:
		break;
	}
	return;
}
static void do_std_build(ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine)
{

	switch (op->subtype) {
	case DW_LNS_copy:
		break;
	case DW_LNS_advance_pc:
		machine->address += op->args.stdAdvancePc.advance_size;
		break;
	case DW_LNS_advance_line:
		machine->line = (((sint32)(machine->line)) + op->args.stdAdvanceLine.advance_line);
		break;
	case DW_LNS_set_file:
		machine->file = op->args.stdSetFile.file;
		break;
	case DW_LNS_set_column:
		break;
	case DW_LNS_negate_stmt:
		break;
	case DW_LNS_set_basic_block:
		break;
	case DW_LNS_const_add_pc:
		//TODO
		ASSERT(0);
		break;
	case DW_LNS_fixed_advance_pc:
		machine->address += op->args.stdFixedAdvancePc.fixed_advance_pc;
		break;
	default:
		break;
	}
	return;
}

static void add_map(KeyValueMappingType *map, ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine)
{
	uint32 file_inx;
	uint32 dir_inx;
	DwLineDefineFileType *dldf;
	char *file;
	char *dir;

	file_inx = machine->file  - 1;
	dldf = (DwLineDefineFileType*)op->hdr->file_names->data[file_inx];
	file = dldf->file;

	dir_inx = dldf->dir;
	if (dir_inx == 0) {
		//TODO current dir
		dir = ".";
	}
	else {
		dir = (char*)op->hdr->include_directories->data[dir_inx -1];
	}

	map->value.file = file;
	map->value.dir = dir;

	file_address_mapping_add(&map->key, &map->value);

	return;
}
static void do_spc_build(ElfDwarfLineParsedOpCodeType *op, ElfDwarfLineStateMachineRegisterType *machine)
{
	if (op->args.special.advance_addr != 0x0) {
		KeyValueMappingType map;

		map.key.address_start = machine->address;
		map.key.address_end = machine->address + op->args.special.advance_addr;
		map.value.line = machine->line;

		add_map(&map, op, machine);
	}
	machine->address = machine->address + op->args.special.advance_addr;
	machine->line = machine->line + op->args.special.advance_line;

	return;
}



