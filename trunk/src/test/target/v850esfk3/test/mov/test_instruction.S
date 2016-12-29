#include "test_data.h"
#include "test_reg.h"

/*
 * （ 1） MOV reg1, reg2
 */

.section	".text" , "ax"
.align	4
.global	_do_test_mov1_1
.type   _do_test_mov1_1, @function
_do_test_mov1_1:
	add		-8, sp
	st.w	REG_IN, 0[sp]
	st.w	REG_OUT, 4[sp]

	/*
	 * テストコード
	 */
	mov		UINT32_MAX, REG_IN
	mov		REG_IN, REG_OUT
	cmp		REG_IN, REG_OUT
	cmov	NZ, -1, r0, r10

	ld.w	0[sp], REG_IN
	ld.w	4[sp], REG_OUT
	add		8, sp
	jmp		[lp]
.size		_do_test_mov1_1, .-_do_test_mov1_1

/*
	bne	_test_fail
_do_test_mov1_2:
	mov UINT32_MIN, REG_IN
	mov REG_IN, REG_OUT
	cmp REG_IN, REG_OUT
	bne	_test_fail
_do_test_mov1_3:
	mov SINT32_MAX, REG_IN
	mov REG_IN, REG_OUT
	cmp REG_IN, REG_OUT
	bne	_test_fail
_do_test_mov1_4:
	mov SINT32_MIN, REG_IN
	mov REG_IN, REG_OUT
	cmp REG_IN, REG_OUT
	bne	_test_fail
*/
	/*
	 * （ 2） MOV imm5, reg2
	 */
/*
_do_test_mov2_1:
	mov IMM5_SIGN_MAX, REG_OUT
	cmp IMM5_SIGN_MAX, REG_OUT

	bne	_test_fail
_do_test_mov2_2:
	mov IMM5_SIGN_MAX_MINUS_1, REG_OUT
	cmp IMM5_SIGN_MAX_MINUS_1, REG_OUT
	bne	_test_fail
_do_test_mov2_3:
	mov IMM5_ZERO, REG_OUT
	cmp IMM5_ZERO, REG_OUT
	bne	_test_fail
_do_test_mov2_4:
	mov IMM5_SIGN_MIN, REG_OUT
	cmp IMM5_SIGN_MIN, REG_OUT
	bne	_test_fail
_do_test_mov2_5:
	mov IMM5_SIGN_MIN_MINUS_1, REG_OUT
	cmp IMM5_SIGN_MIN_MINUS_1, REG_OUT
	bne	_test_fail
	br	_test_done
*/
