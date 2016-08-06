#ifndef _REG_H_
#define _REG_H_



extern void intc_clr_currlvl_ispr(CpuManagerType *cpu);
extern void intc_clr_nmi(CpuManagerType *cpu);

extern void intc_hook_update_reg8(CpuManagerType *cpu, uint32 regaddr, uint8 data);
extern void serial_hook_update_reg8(CpuManagerType *cpu, uint32 regaddr, uint8 data);

extern void intc_hook_update_reg16(CpuManagerType *cpu, uint32 regaddr, uint16 data);

extern uint32* can_get_reg16_addr(CpuManagerType *cpu, uint32 regaddr);
extern void can_hook_update_reg16(CpuManagerType *cpu, uint32 regaddr, uint16 data);

/*
 * 内部関数
 */
extern void cpu_memget_addrp(CpuManagerType *cpu, uint32 addr, uint32 **addrp);
extern void cpu_memget_raddrp(CpuManagerType *cpu, uint32 addr, uint32 **addrp);

#endif /* _REG_H_ */
