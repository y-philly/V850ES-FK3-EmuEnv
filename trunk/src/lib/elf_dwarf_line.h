#ifndef _ELF_DWARF_LINE_H_
#define _ELF_DWARF_LINE_H_

#include "loader/elf.h"
#include "elf_dwarf_util.h"
#include "std_errno.h"

#define STD_OP_CODE_MAX	256
typedef struct {
	char		*filename;
	uint32		dir;
	uint32		time;
	uint32		size;
} ElfDwarfLineEntryHeaderFileType;

typedef struct {
	uint32 					total_length;
	uint16 					version;
	uint32 					prologue_length;
	uint8  					minimum_instruction_length;
	uint8  					default_is_stmt;
	sint8  					line_base;
	uint8  					line_range;
	uint8  					opcode_base;
	uint8  					standard_opcode_lengths[STD_OP_CODE_MAX];
	ElfPointerArrayType		*include_directories;
	ElfPointerArrayType		*file_names;
} ElfDwarfLineEntryHeaderType;

extern Std_ReturnType elf_dwarf_line_load(uint8 *elf_data);

typedef enum {
	OPTYPE_EXTENDED	= 0,
	OPTYPE_STANDARD,
	OPTYPE_SPECIAL,
} ElfDwarfLineOpType;

typedef struct {
	uint32 advance_addr;
	sint32 advance_line;
} DwLineSpecialType;


typedef enum {
	DW_LNE_end_sequence = 1,
	DW_LNE_set_address,
	DW_LINE_define_file,
	DW_LINE_unknown,
} ElfDwarfLineOpExtendedType;

typedef struct {
	uint32 addr;
} DwLineSetAddressType;
typedef struct {
	char 	*file;
	uint32	dir;
	uint32	time;
	uint32	size;
} DwLineDefineFileType;

typedef enum {
	DW_LNS_copy = 1,
	DW_LNS_advance_pc,
	DW_LNS_advance_line,
	DW_LNS_set_file,
	DW_LNS_set_column,
	DW_LNS_negate_stmt,
	DW_LNS_set_basic_block,
	DW_LNS_const_add_pc,
	DW_LNS_fixed_advance_pc,
} ElfDwarfLineOpStandardType;

typedef struct {
	uint32 advance_size;
} DwLnsAdvancePcType;

typedef struct {
	sint32 advance_line;
} DwLnsAdvanceLineType;

typedef struct {
	uint32 file;
} DwLnsSetFileType;

typedef struct {
	uint32 column;
} DwLnsSetColumnType;

typedef struct {
	uint32 fixed_advance_pc;
} DwLnsFixedAdvancePcType;

typedef struct {
	ElfDwarfLineEntryHeaderType		*hdr;
	uint32 							size;
	ElfDwarfLineOpType				type;
	int								subtype;
	union {
		DwLineSetAddressType		extSetAddress;
		DwLineDefineFileType		extDefineFile;
		DwLnsAdvancePcType			stdAdvancePc;
		DwLnsAdvanceLineType		stdAdvanceLine;
		DwLnsSetFileType			stdSetFile;
		DwLnsSetColumnType			stdSetColumn;
		DwLnsFixedAdvancePcType		stdFixedAdvancePc;
		DwLineSpecialType			special;
	} args;
} ElfDwarfLineParsedOpCodeType;


typedef struct {
	ElfDwarfLineEntryHeaderType		*header;
	ElfPointerArrayType				*ops;
} ElfDwarfLineEntryType;


typedef struct {
	ElfPointerArrayType				*entries;
} ElfDwarfLineType;

extern ElfDwarfLineType *elf_dwarf_line_alloc_empty_ElfDwarfLine(void);
extern ElfDwarfLineEntryHeaderType *elf_dwarf_line_alloc_empty_ElfDwarfLineEntryHeader(void);
extern ElfDwarfLineEntryType *elf_dwarf_line_alloc_empty_ElfDwarfLineEntry(void);
extern ElfDwarfLineParsedOpCodeType *elf_dwarf_line_alloc_empty_ElfDwarfLineParsedOpCode(void);

#endif /* _ELF_DWARF_LINE_H_ */
