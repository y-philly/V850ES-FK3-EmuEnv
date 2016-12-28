#include "types.h"
#include "timer.h"
#include "serial.h"
#include "api.h"

uint8 stack_top[1024];
uint8 global_x = 0xFF;
uint8 global_y;

int main(void)
{
//	uint16 count;


	timer_init();
	serial_init();
	timer_start();

	serial_write(65);

	while (1) {
		api_bg();
	}
	return 0;
}
