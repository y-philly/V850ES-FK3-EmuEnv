#include "test_serial.h"
#include "test_data.h"
#include "v850asm.inc"

	.section	".text" , "ax"
	.align	4
	.globl	_start
_start:
	Lea _stack_data, r3
	addi 1024, sp r3
	br	 _main


	.align	4
	.globl	_default_int_handler
	.type	_default_int_handler,@function
_default_int_handler:
	br	_default_int_handler

	
