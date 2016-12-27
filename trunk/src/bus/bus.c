#include "bus.h"

typedef struct {
	BusAccessType		access_type;
	BusAccessSizeType	access_sizetype;
	union {
		uint8	data8;
		uint16	data16;
		uint32	data32;
	} access;
} BusAccessLogType;

BusAccessLogType bus_access_log;

void bus_access_set_log(BusAccessType type, BusAccessSizeType sizetype, uint32 access_data)
{
	bus_access_log.access_type = type;
	bus_access_log.access_sizetype = sizetype;

	if (type != BUS_ACCESS_TYPE_WRITE) {
		return;
	}
	switch (sizetype) {
	case BUS_ACCESS_SIZE_TYPE8:
		bus_access_log.access.data8 = (uint8)access_data;
		break;
	case BUS_ACCESS_SIZE_TYPE16:
		bus_access_log.access.data16 = (uint16)access_data;
		break;
	case BUS_ACCESS_SIZE_TYPE32:
		bus_access_log.access.data32 = (uint32)access_data;
		break;
	default:
		break;
	}
}

void bus_access_get_log(BusAccessType *type, BusAccessSizeType *sizetype, uint32 *access_data)
{
	*type = bus_access_log.access_type;
	*sizetype = bus_access_log.access_sizetype;
	switch (bus_access_log.access_sizetype) {
	case BUS_ACCESS_SIZE_TYPE8:
		*access_data = bus_access_log.access.data8;
		break;
	case BUS_ACCESS_SIZE_TYPE16:
		*access_data = bus_access_log.access.data16;
		break;
	case BUS_ACCESS_SIZE_TYPE32:
		*access_data = bus_access_log.access.data32;
		break;
	default:
		break;
	}
}
