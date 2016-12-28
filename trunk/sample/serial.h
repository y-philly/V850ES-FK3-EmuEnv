#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "types.h"

extern void serial_init(void);
extern void serial_write(uint8 data);
extern void serial_read(uint8* data);

#endif /* _SERIAL_H_ */
