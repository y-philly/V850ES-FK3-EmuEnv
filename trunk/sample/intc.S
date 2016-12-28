#include "v850asm.inc"

	.section	".text" , "ax"
	.align	4

.macro INT_EXC_SAVEREG_ISR
	addi	-80, sp, ep

	st.w	R31, 8[ep]
	st.w	R30, 12[ep]
	st.w	R19, 16[ep]
	st.w	R18, 20[ep]
	st.w	R17, 24[ep]
	st.w	R16, 28[ep]
	st.w	R15, 32[ep]
	st.w	R14, 36[ep]
	st.w	R13, 40[ep]
	st.w	R12, 44[ep]
	st.w	R11, 48[ep]
	st.w	R10, 52[ep]
	st.w	R9, 56[ep]
	st.w	R8, 60[ep]
	st.w	R7, 64[ep]
	st.w	R6, 68[ep]
	st.w	R5, 72[ep]
	st.w	R1, 76[ep]
.endm

.macro INT_EXC_RESTOREREG
	ld.w	76[ep], R1
	ld.w	72[ep], R5
	ld.w	68[ep], R6
	ld.w	64[ep], R7
	ld.w	60[ep], R8
	ld.w	56[ep], R9
	ld.w	52[ep], R10
	ld.w	48[ep], R11
	ld.w	44[ep], R12
	ld.w	40[ep], R13
	ld.w	36[ep], R14
	ld.w	32[ep], R15
	ld.w	28[ep], R16
	ld.w	24[ep], R17
	ld.w	20[ep], R18
	ld.w	16[ep], R19
	ld.w	12[ep], R30
	ld.w	8[ep], R31

	addi	80, ep, sp
.endm


	.globl	_default_int_handler
_default_int_handler:

	reti

	.global __interrupt_22
	.type	__interrupt_22,@function
__interrupt_22:
	INT_EXC_SAVEREG_ISR

	stsr	eipc, R19
	st.w	R19, 0[ep]
	stsr	eipsw, R19
	st.w	R19, 4[ep]

	ei

	jarl _api_1sec_cycle, lp

	di

	ld.w	0[ep], R1
	ldsr	R1, 0
	ld.w	4[ep], R1
	ldsr	R1, 1

	INT_EXC_RESTOREREG

	reti

	.globl	__interrupt_36
	.type	__interrupt_36,@function
__interrupt_36:
	INT_EXC_SAVEREG_ISR

	stsr	eipc, R19
	st.w	R19, 0[ep]
	stsr	eipsw, R19
	st.w	R19, 4[ep]

	ei

	jarl _api_receive_serial, lp

	di

	ld.w	0[ep], R1
	ldsr	R1, 0
	ld.w	4[ep], R1
	ldsr	R1, 1

	INT_EXC_RESTOREREG

	reti
