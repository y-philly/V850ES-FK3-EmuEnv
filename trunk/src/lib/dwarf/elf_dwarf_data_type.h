#ifndef _ELF_DWARF_DATA_TYPE_H_
#define _ELF_DWARF_DATA_TYPE_H_

#include "std_types.h"
#include "elf_dwarf_util.h"

typedef enum {
	DATA_TYPE_BASE,
	DATA_TYPE_STRUCT,
	DATA_TYPE_ARRAY,
	DATA_TYPE_POINTER,
	DATA_TYPE_TYPEDEF,
} DwarfDataEnumType;

extern void *dwarf_alloc_data_type(DwarfDataEnumType type);
extern void *dwarf_search_data_type(DwarfDataEnumType type, char *filename, char *typename);

typedef struct {
	DwarfDataEnumType	type;
	char				*name;
	uint32				size;
} DwarfDataType;

/*
 * ex. uint32
 *
 * info.type = DATA_TYPE_BASE
 * info.name = "uint32"
 * info.size = 4
 */
typedef struct {
	DwarfDataType	info;
} DwarfDataBaseType;

/*
 * ex. uint32 *
 *
 * info.type = DATA_TYPE_POINTER
 * info.name = NULL
 * info.size = 4
 * info.ref = (ref of uint32)
 */
typedef struct {
	DwarfDataType	info;
	DwarfDataType	*ref;
} DwarfDataPointerType;


/*
 * ex. typedef struct a a_t;
 *
 * info.type = DATA_TYPE_TYPEDEF
 * info.name = "a_t"
 * info.size = sizeof (struct a)
 * info.ref = (ref of struct a )
 */
typedef struct {
	DwarfDataType	info;
	DwarfDataType	*ref;
} DwarfDataTypedefType;


typedef struct {
	DwarfDataType		info;
	ElfPointerArrayType	*members;
} DwarfDataTypedefType;

typedef struct {
	char				*name;
	uint32				off;
	DwarfDataType		*ref;
} DwarfDataStructMember;

typedef struct {
	DwarfDataType			info;
	DwarfDataType			*ref;
	uint32					dimension;
	DwarfUint32ArrayType	*element_num;
} DwarfDataArrayType;
#endif /* _ELF_DWARF_DATA_TYPE_H_ */
