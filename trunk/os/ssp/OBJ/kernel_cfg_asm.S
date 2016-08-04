/*
 *	CPU例外/割込みハンドラテーブルの定義
 *
 *	各ハンドラに16byteずつ命令を配置可能．
 */


#include "v850asm.inc"
#define TOPPERS_LABEL_ASM
#define TOPPERS_MACRO_ONLY
#include "../kernel/kernel_impl.h"


	.section .vectors , "ax"
	.align	4


	.globl	interrupt
	.globl	_default_exc_handler
	.globl	_default_int_handler

_reset_handler:
	.globl _start
	jr	_start
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_1:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_2:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop


_exc_handler_3:
	jr	_goto_default_exc_handler
	nop
	nop
	nop
	nop
	nop
	nop
_exc_handler_4:
	.globl	exc_handler_handler
	jr	exc_handler_handler
	nop
	nop
	nop
	nop
	nop
	nop
_exc_handler_5:
	jr	_goto_default_exc_handler
	nop
	nop
	nop
	nop
	nop
	nop
_exc_handler_6:
	jr	_goto_default_exc_handler
	nop
	nop
	nop
	nop
	nop
	nop
security_id:
	.long	0xFFFFFFFF
	.long	0xFFFFFFFF
	.long	0xFFFFFFFF
	.long	0xFFFFFFFF

_int_handler_8:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_9:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_10:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_11:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_12:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_13:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_14:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_15:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_16:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_17:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_18:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_19:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_20:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_21:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_22:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_23:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_24:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_25:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_26:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_27:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_28:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_29:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_30:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_31:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_32:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_33:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_34:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_35:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_36:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_37:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_38:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_39:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_40:
	.globl	target_timer_handler_handler
	jr	__kernel_target_timer_handler_40
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_41:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_42:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_43:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_44:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_45:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_46:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_47:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_48:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_49:
	.globl	_kernel_inthdr_49_handler
	jr	__kernel__kernel_inthdr_49_49
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_50:
	.globl	_kernel_inthdr_50_handler
	jr	__kernel__kernel_inthdr_50_50
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_51:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_52:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_53:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_54:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_55:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_56:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_57:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_58:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_59:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_60:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_61:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_62:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int_handler_63:
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop


	.section .text , "ax"
	.align 4

	.globl	_exc_handler
exc_handler_handler:
	addi	-80 , sp , sp
	st.w	r10 , 52[sp]
	st.w	r11 , 48[sp]
	st.w	r12 , 44[sp]
	Lea	_exc_handler , r10
	addi	80 , sp , r11
	Lea	4 , r12
	jr		exception

	.globl	_target_timer_handler
	.globl	__kernel_target_timer_handler_40
__kernel_target_timer_handler_40:
	addi	-80 , sp , sp
	st.w	r10 , 52[sp]
	st.w	r11 , 48[sp]
	st.w	r12 , 44[sp]
	Lea	_target_timer_handler , r10
	mov	6 , r11
	Lea	40 , r12
	jr		interrupt

	.globl	__kernel_inthdr_49
	.globl	__kernel__kernel_inthdr_49_49
__kernel__kernel_inthdr_49_49:
	addi	-80 , sp , sp
	st.w	r10 , 52[sp]
	st.w	r11 , 48[sp]
	st.w	r12 , 44[sp]
	Lea	__kernel_inthdr_49 , r10
	mov	4 , r11
	Lea	49 , r12
	jr		interrupt

	.globl	__kernel_inthdr_50
	.globl	__kernel__kernel_inthdr_50_50
__kernel__kernel_inthdr_50_50:
	addi	-80 , sp , sp
	st.w	r10 , 52[sp]
	st.w	r11 , 48[sp]
	st.w	r12 , 44[sp]
	Lea	__kernel_inthdr_50 , r10
	mov	4 , r11
	Lea	50 , r12
	jr		interrupt



_goto_default_int_handler:
	addi	-80 , sp , sp
	st.w	r10 , 0[sp]
	Lea	_default_int_handler , r10
	jr		interrupt


_goto_default_exc_handler:
	addi	-80 , sp , sp
	st.w	r10 , 0[sp]
	Lea	_default_exc_handler , r10
	jr		exception
	.end


