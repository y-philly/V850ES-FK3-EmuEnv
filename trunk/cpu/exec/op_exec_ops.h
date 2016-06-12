#ifndef _OP_EXEC_OPS_H_
#define _OP_EXEC_OPS_H_

/*
 * ���[�h����
 */
extern int op_exec_ldb(CpuManagerType *cpu);
extern int op_exec_ldhw(CpuManagerType *cpu);
extern int op_exec_sldb(CpuManagerType *cpu);
extern int op_exec_sldh(CpuManagerType *cpu);
extern int op_exec_sldw(CpuManagerType *cpu);

/*
 * �X�g�A����
 */
extern int op_exec_sstb(CpuManagerType *cpu);
extern int op_exec_ssth(CpuManagerType *cpu);
extern int op_exec_stb(CpuManagerType *cpu);
extern int op_exec_sthw(CpuManagerType *cpu);
extern int op_exec_sstw(CpuManagerType *cpu);

/*
 * ��Z����
 */
extern int op_exec_mulhi(CpuManagerType *cpu);
extern int op_exec_mulh_1(CpuManagerType *cpu);
extern int op_exec_mulh_2(CpuManagerType *cpu);

/*
 * ���Z����
 */

/*
 * �Z�p���Z����
 */
extern int op_exec_addi(CpuManagerType *cpu);
extern int op_exec_movea(CpuManagerType *cpu);
extern int op_exec_movhi(CpuManagerType *cpu);
extern int op_exec_mov_1(CpuManagerType *cpu);
extern int op_exec_mov_2(CpuManagerType *cpu);

extern int op_exec_add_1(CpuManagerType *cpu);
extern int op_exec_add_2(CpuManagerType *cpu);

extern int op_exec_sub(CpuManagerType *cpu);
extern int op_exec_subr(CpuManagerType *cpu);
extern int op_exec_setf(CpuManagerType *cpu);

/*
 * �O�a���Z����
 */
extern int op_exec_satsubi(CpuManagerType *cpu);
extern int op_exec_satadd_1(CpuManagerType *cpu);
extern int op_exec_satadd_2(CpuManagerType *cpu);

/*
 * �_�����Z����
 */
extern int op_exec_and(CpuManagerType *cpu);
extern int op_exec_andi(CpuManagerType *cpu);
extern int op_exec_ori(CpuManagerType *cpu);
extern int op_exec_or(CpuManagerType *cpu);
extern int op_exec_xori(CpuManagerType *cpu);
extern int op_exec_xor(CpuManagerType *cpu);
extern int op_exec_not(CpuManagerType *cpu);
extern int op_exec_sxh(CpuManagerType *cpu);
extern int op_exec_sxb(CpuManagerType *cpu);
extern int op_exec_shl_2(CpuManagerType *cpu);
extern int op_exec_shr_2(CpuManagerType *cpu);
extern int op_exec_sar_2(CpuManagerType *cpu);
extern int op_exec_shl_9(CpuManagerType *cpu);
extern int op_exec_shr_9(CpuManagerType *cpu);
extern int op_exec_sar_9(CpuManagerType *cpu);
extern int op_exec_tst(CpuManagerType *cpu);
extern int op_exec_zxb(CpuManagerType *cpu);
extern int op_exec_zxh(CpuManagerType *cpu);

/*
 * ���򖽗�
 */
extern int op_exec_jr(CpuManagerType *cpu);
extern int op_exec_jmp(CpuManagerType *cpu);
extern int op_exec_cmp_1(CpuManagerType *cpu);
extern int op_exec_cmp_2(CpuManagerType *cpu);
extern int op_exec_bcond(CpuManagerType *cpu);

/*
 * �r�b�g����
 */
extern int op_exec_tst1_8(CpuManagerType *cpu);
/*
 * ���ꖽ��
 */
extern int op_exec_diei(CpuManagerType *cpu);
extern int op_exec_ldsr(CpuManagerType *cpu);
extern int op_exec_stsr(CpuManagerType *cpu);
extern int op_exec_nop(CpuManagerType *cpu);
extern int op_exec_reti(CpuManagerType *cpu);
extern int op_exec_halt(CpuManagerType *cpu);
extern int op_exec_trap(CpuManagerType *cpu);



/*
 * �f�B�o�b�O�@�\�p����
 */

#include "dbg.h"


#endif /* _OP_EXEC_OPS_H_ */
