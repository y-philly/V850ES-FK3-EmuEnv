#ifndef _INTC_OPS_H_
#define _INTC_OPS_H_

extern int intc_raise_intr(CoreType *cpu, uint32 intno);
extern bool intc_has_pending_intr(CoreType *cpu);
extern int intc_raise_pending_intr(CoreType *cpu);
extern void intc_clr_nmi(CoreType *cpu);
extern void intc_clr_currlvl_ispr(CoreType *cpu);

#endif /* _INTC_OPS_H_ */
