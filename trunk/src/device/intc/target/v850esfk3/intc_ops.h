#ifndef _INTC_OPS_H_
#define _INTC_OPS_H_

extern int intc_raise_intr(uint32 intno);
extern void intc_clr_nmi(TargetCoreType *cpu);
extern void intc_clr_currlvl_ispr(TargetCoreType *cpu);

#endif /* _INTC_OPS_H_ */
