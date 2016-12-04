#ifndef _INTC_OPS_H_
#define _INTC_OPS_H_

extern int intc_raise_intr(CpuManagerType *cpu, uint32 intno);
extern bool intc_has_pending_intr(CpuManagerType *cpu);
extern int intc_raise_pending_intr(CpuManagerType *cpu);
extern void intc_clr_nmi(CpuManagerType *cpu);
extern void intc_clr_currlvl_ispr(CpuManagerType *cpu);

#endif /* _INTC_OPS_H_ */
