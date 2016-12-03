#ifndef _BUS_H_
#define _BUS_H_

#include "std_types.h"
#include "std_errno.h"
#include "mpu_ops.h"

/*
 * データ取得するための操作関数群
 */
static inline Std_ReturnType bus_get_data8(CoreIdType core_id, uint32 addr, uint8 *data)
{
	return mpu_get_data8(core_id, addr, data);
}
static inline Std_ReturnType bus_get_data16(CoreIdType core_id, uint32 addr, uint16 *data)
{
	return mpu_get_data16(core_id, addr, data);
}
static inline Std_ReturnType bus_get_data32(CoreIdType core_id, uint32 addr, uint32 *data)
{
	return mpu_get_data32(core_id, addr, data);
}

/*
 * データ転送するための操作関数群
 */
static inline Std_ReturnType bus_put_data8(CoreIdType core_id, uint32 addr, uint8 data)
{
	return mpu_put_data8(core_id, addr, data);
}
static inline Std_ReturnType bus_put_data16(CoreIdType core_id, uint32 addr, uint16 data)
{
	return mpu_put_data16(core_id, addr, data);
}
static inline Std_ReturnType bus_put_data32(CoreIdType core_id, uint32 addr, uint32 data)
{
	return mpu_put_data32(core_id, addr, data);
}

/*
 * 注意：本APIは基本的には使用しないこと．
 *
 * 性能向上を目的として使用する場合は，
 * ROM/RAM領域のメモリに限り，当該領域のポインタ変数を本関数を使用して取得する．
 */
static inline Std_ReturnType bus_get_pointer(CoreIdType core_id, uint32 addr, uint8 **data)
{
	return mpu_get_pointer(core_id, addr, data);
}

#endif /* _BUS_H_ */
