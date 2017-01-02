#ifndef _FILE_ADDRESS_MAPPING_H_
#define _FILE_ADDRESS_MAPPING_H_

#include "std_types.h"
#include "std_errno.h"

typedef struct {
	uint32	address_start;
	uint32	address_end;
} KeyAddressType;

typedef struct {
	char	*dir;
	char	*file;
	uint32	line;
} ValueFileType;

extern void file_address_mapping_init(void);
extern Std_ReturnType file_address_mapping_get(uint32 addr, ValueFileType *value);
extern Std_ReturnType file_address_mapping_get_last(KeyAddressType *key, ValueFileType *value);

#endif /* _FILE_ADDRESS_MAPPING_H_ */
