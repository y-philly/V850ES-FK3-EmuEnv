#include "v850asm.inc"

	.section	".text" , "ax"
	.align	4
	.globl	_start
_start:
	di
	
	Lea     _stack_top, r3
	addi    1024, sp r3
	Lea     __tp, tp

	Lea     __bss_start, r10
	Lea     __bss_end, r11
_bssloop:	
	st.w    r0, 0[r10]
	addi    4, r10, r10
	cmp     r10, r11
	bne     _bssloop

_globalinit:	
	Lea    __data_start, r10
	Lea    _edata, r11
	Lea    __idata_start, r12

_globalinit_loop:
	ld.w    0[r12], r13
	st.w    r13, 0[r10]
	addi    4, r12, r12
	addi    4, r10, r10
	cmp     r10, r11
	bne     _globalinit_loop
	
	ei
	
	br _main
	
	
