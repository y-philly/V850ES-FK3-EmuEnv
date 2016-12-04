#ifndef _OP_EXEC_OPS_H_
#define _OP_EXEC_OPS_H_

#include "cpu_register.h"
#include "cpu_common/cpu_ops.h"

/*
 * ロード命令
 */
extern int op_exec_ldb(CoreType *cpu);
extern int op_exec_ldbu(CoreType *cpu);
extern int op_exec_ldhu(CoreType *cpu);
extern int op_exec_ldhw(CoreType *cpu);
extern int op_exec_sldb(CoreType *cpu);
extern int op_exec_sldbu(CoreType *cpu);
extern int op_exec_sldh(CoreType *cpu);
extern int op_exec_sldhu(CoreType *cpu);
extern int op_exec_sldw(CoreType *cpu);

/*
 * ストア命令
 */
extern int op_exec_sstb(CoreType *cpu);
extern int op_exec_ssth(CoreType *cpu);
extern int op_exec_stb(CoreType *cpu);
extern int op_exec_sthw(CoreType *cpu);
extern int op_exec_sstw(CoreType *cpu);

/*
 * 乗算命令
 */
extern int op_exec_mulhi(CoreType *cpu);
extern int op_exec_mulh_1(CoreType *cpu);
extern int op_exec_mulh_2(CoreType *cpu);
extern int op_exec_mulu_12(CoreType *cpu);
extern int op_exec_mul_12(CoreType *cpu);

/*
 * 除算命令
 */

/*
 * 算術演算命令
 */
extern int op_exec_addi(CoreType *cpu);
extern int op_exec_movea(CoreType *cpu);
extern int op_exec_mov_6(CoreType *cpu);
extern int op_exec_movhi(CoreType *cpu);
extern int op_exec_mov_1(CoreType *cpu);
extern int op_exec_mov_2(CoreType *cpu);

extern int op_exec_add_1(CoreType *cpu);
extern int op_exec_add_2(CoreType *cpu);

extern int op_exec_sub(CoreType *cpu);
extern int op_exec_subr(CoreType *cpu);
extern int op_exec_setf(CoreType *cpu);

extern int op_exec_div(CoreType *cpu);
extern int op_exec_divu(CoreType *cpu);
extern int op_exec_divhu(CoreType *cpu);
extern int op_exec_divh_11(CoreType *cpu);
extern int op_exec_divh_1(CoreType *cpu);
extern int op_exec_mul(CoreType *cpu);
extern int op_exec_mulu(CoreType *cpu);

extern int op_exec_cmov_11(CoreType *cpu);
extern int op_exec_cmov_12(CoreType *cpu);

/*
 * 飽和演算命令
 */
extern int op_exec_satsubi(CoreType *cpu);
extern int op_exec_satadd_1(CoreType *cpu);
extern int op_exec_satsub_1(CoreType *cpu);
extern int op_exec_satadd_2(CoreType *cpu);

/*
 * 論理演算命令
 */
extern int op_exec_and(CoreType *cpu);
extern int op_exec_andi(CoreType *cpu);
extern int op_exec_ori(CoreType *cpu);
extern int op_exec_or(CoreType *cpu);
extern int op_exec_xori(CoreType *cpu);
extern int op_exec_xor(CoreType *cpu);
extern int op_exec_not(CoreType *cpu);
extern int op_exec_sxh(CoreType *cpu);
extern int op_exec_sxb(CoreType *cpu);
extern int op_exec_shl_2(CoreType *cpu);
extern int op_exec_shr_2(CoreType *cpu);
extern int op_exec_sar_2(CoreType *cpu);
extern int op_exec_shl_9(CoreType *cpu);
extern int op_exec_shr_9(CoreType *cpu);
extern int op_exec_sar_9(CoreType *cpu);
extern int op_exec_tst(CoreType *cpu);
extern int op_exec_zxb(CoreType *cpu);
extern int op_exec_zxh(CoreType *cpu);

/*
 * 分岐命令
 */
extern int op_exec_jr(CoreType *cpu);
extern int op_exec_jmp(CoreType *cpu);
extern int op_exec_cmp_1(CoreType *cpu);
extern int op_exec_cmp_2(CoreType *cpu);
extern int op_exec_bcond(CoreType *cpu);

/*
 * ビット命令
 */
extern int op_exec_tst1_8(CoreType *cpu);
extern int op_exec_set1_8(CoreType *cpu);
extern int op_exec_clr1_8(CoreType *cpu);
extern int op_exec_not1_8(CoreType *cpu);


extern int op_exec_tst1_9(CoreType *cpu);
extern int op_exec_set1_9(CoreType *cpu);
extern int op_exec_clr1_9(CoreType *cpu);
extern int op_exec_not1_9(CoreType *cpu);

/*
 * 特殊命令
 */
extern int op_exec_diei(CoreType *cpu);
extern int op_exec_ldsr(CoreType *cpu);
extern int op_exec_stsr(CoreType *cpu);
extern int op_exec_nop(CoreType *cpu);
extern int op_exec_reti(CoreType *cpu);
extern int op_exec_halt(CoreType *cpu);
extern int op_exec_trap(CoreType *cpu);
extern int op_exec_switch(CoreType *cpu);
extern int op_exec_prepare(CoreType *cpu);
extern int op_exec_dispose(CoreType *cpu);



/*
 * ディバッグ機能用命令
 */

#include "dbg.h"


#endif /* _OP_EXEC_OPS_H_ */
