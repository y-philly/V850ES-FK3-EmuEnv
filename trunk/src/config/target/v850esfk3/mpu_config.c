#include "mpu.h"
#include "cpu_config.h"

#define MPU_ADDRESS_REGION_SIZE_INX_0	(1024U * 512U)
#define MPU_ADDRESS_REGION_SIZE_INX_1	(1024U * 60U)
#define MPU_ADDRESS_REGION_SIZE_INX_2	((6U * 2U) + 1U)
#define MPU_ADDRESS_REGION_SIZE_INX_3	(1024U * 4U)
#define MPU_ADDRESS_REGION_SIZE_INX_4	(1024U * 12U)

static uint8 memory_data_0[MPU_ADDRESS_REGION_SIZE_INX_0];
static uint8 memory_data_1[MPU_ADDRESS_REGION_SIZE_INX_1];
static uint8 memory_data_2[MPU_ADDRESS_REGION_SIZE_INX_2];
static uint8 memory_data_3[MPU_ADDRESS_REGION_SIZE_INX_3];
static uint8 memory_data_4[MPU_ADDRESS_REGION_SIZE_INX_4];

MpuAddressMapType mpu_address_map = {
		.map = {
				/*
				 * INDEX 0:ROM
				 */
				{
						.type		= GLOBAL_MEMORY,
						.core_id	= CPU_CONFIG_CORE_ID_NONE,
						.start		= 0x00000000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_0,
						.data		= memory_data_0,
						.ops		= &default_memory_operation
				},
				/*
				 * INDEX 1:RAM
				 */
				{
						.type		= GLOBAL_MEMORY,
						.core_id	= CPU_CONFIG_CORE_ID_NONE,
						.start		= 0x03FF0000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_1,
						.data		= memory_data_1,
						.ops		= &default_memory_operation
				},
				/*
				 * INDEX 2:DEVICE(割込みコントローラ：IMR0～IMR7)
				 */
				{
						.type		= DEVICE,
						.core_id	= CPU_CONFIG_CORE_ID_NONE,
						.start		= 0xFFFFF100,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_2,
						.data		= memory_data_2,
						.ops		= NULL	//TODO
				},
				/*
				 * INDEX 3:DEVICE(内蔵周辺I/O領域)
				 */
				{
						.type		= DEVICE,
						.core_id	= CPU_CONFIG_CORE_ID_NONE,
						.start		= 0x03FFF000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_3,
						.data		= memory_data_3,
						.ops		= NULL	//TODO
				},
				/*
				 * INDEX 4:DEVICE(プログラマブル周辺I/O領域)
				 */
				{
						.type		= DEVICE,
						.core_id	= CPU_CONFIG_CORE_ID_NONE,
						.start		= 0x03FEC000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_4,
						.data		= memory_data_4,
						.ops		= NULL	//TODO
				},
		}
};
