#ifndef _MPU_TYPES_H_
#define _MPU_TYPES_H_

#include "std_types.h"

typedef enum {
	GLOBAL_MEMORY = 0,
	LOCAL_MEMORY,
	PRIVATE_MEMORY,
	DEVICE
} MpuAddressRegionEnumType;


struct mpu_address_region_operation_type;
typedef struct {
	MpuAddressRegionEnumType					type;
	CoreIdType									core_id;
	uint32										start;
	uint32										size;
	uint8										*data;
	struct mpu_address_region_operation_type	*ops;
} MpuAddressRegionType;

typedef struct mpu_address_region_operation_type {
	Std_ReturnType (*get_data8) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
	Std_ReturnType (*get_data16) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
	Std_ReturnType (*get_data32) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);

	Std_ReturnType (*put_data8) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
	Std_ReturnType (*put_data16) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
	Std_ReturnType (*put_data32) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);

	Std_ReturnType (*get_pointer) (MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);
} MpuAddressRegionOperationType;

extern MpuAddressRegionOperationType	default_memory_operation;

#endif /* _MPU_TYPES_H_ */
