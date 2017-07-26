/* cfg1_out.c */
#define TOPPERS_CFG1_OUT
#include "kernel/kernel_int.h"
#include "target_timer.h"
#include "tTask_tecsgen.h"
#include "tISR_tecsgen.h"
#include "tInitializeRoutine_tecsgen.h"
#include "tTerminateRoutine_tecsgen.h"
#include "sample1.h"

#ifdef INT64_MAX
  typedef int64_t signed_t;
  typedef uint64_t unsigned_t;
#else
  typedef int32_t signed_t;
  typedef uint32_t unsigned_t;
#endif

#include "target_cfg1_out.h"

#if defined(SIL_ENDIAN_BIG) && defined(SIL_ENDIAN_LITTLE)
#error Both SIL_ENDIAN_BIG and SIL_ENDIAN_LITTLE are defined.
#endif
#if !defined(SIL_ENDIAN_BIG) && !defined(SIL_ENDIAN_LITTLE)
#error Neither SIL_ENDIAN_BIG nor SIL_ENDIAN_LITTLE is defined.
#endif

const uint32_t TOPPERS_magic_number = 0x12345678;
const uint32_t TOPPERS_sizeof_signed_t = sizeof(signed_t);

const unsigned_t TOPPERS_cfg_CHAR_BIT = (unsigned_t)(CHAR_BIT);
const signed_t TOPPERS_cfg_SCHAR_MAX = (signed_t)(SCHAR_MAX);
const signed_t TOPPERS_cfg_SCHAR_MIN = (signed_t)(SCHAR_MIN);
const unsigned_t TOPPERS_cfg_UCHAR_MAX = (unsigned_t)(UCHAR_MAX);
const signed_t TOPPERS_cfg_CHAR_MAX = (signed_t)(CHAR_MAX);
const signed_t TOPPERS_cfg_CHAR_MIN = (signed_t)(CHAR_MIN);
const signed_t TOPPERS_cfg_SHRT_MAX = (signed_t)(SHRT_MAX);
const signed_t TOPPERS_cfg_SHRT_MIN = (signed_t)(SHRT_MIN);
const unsigned_t TOPPERS_cfg_USHRT_MAX = (unsigned_t)(USHRT_MAX);
const signed_t TOPPERS_cfg_INT_MAX = (signed_t)(INT_MAX);
const signed_t TOPPERS_cfg_INT_MIN = (signed_t)(INT_MIN);
const unsigned_t TOPPERS_cfg_UINT_MAX = (unsigned_t)(UINT_MAX);
const signed_t TOPPERS_cfg_LONG_MAX = (signed_t)(LONG_MAX);
const signed_t TOPPERS_cfg_LONG_MIN = (signed_t)(LONG_MIN);
const unsigned_t TOPPERS_cfg_ULONG_MAX = (unsigned_t)(ULONG_MAX);
#if defined(USE_EXTERNAL_ID)
const signed_t TOPPERS_cfg_USE_EXTERNAL_ID = (signed_t)(true);
#endif
#if defined(SIL_ENDIAN_BIG)
const signed_t TOPPERS_cfg_SIL_ENDIAN_BIG = (signed_t)(true);
#endif
#if defined(SIL_ENDIAN_LITTLE)
const signed_t TOPPERS_cfg_SIL_ENDIAN_LITTLE = (signed_t)(true);
#endif
const unsigned_t TOPPERS_cfg_TA_NULL = (unsigned_t)(TA_NULL);
const unsigned_t TOPPERS_cfg_TA_ACT = (unsigned_t)(TA_ACT);
const unsigned_t TOPPERS_cfg_TA_NOACTQUE = (unsigned_t)(TA_NOACTQUE);
const unsigned_t TOPPERS_cfg_TA_TPRI = (unsigned_t)(TA_TPRI);
const unsigned_t TOPPERS_cfg_TA_WMUL = (unsigned_t)(TA_WMUL);
const unsigned_t TOPPERS_cfg_TA_CLR = (unsigned_t)(TA_CLR);
const unsigned_t TOPPERS_cfg_TA_CEILING = (unsigned_t)(TA_CEILING);
const unsigned_t TOPPERS_cfg_TA_STA = (unsigned_t)(TA_STA);
const unsigned_t TOPPERS_cfg_TA_NONKERNEL = (unsigned_t)(TA_NONKERNEL);
const unsigned_t TOPPERS_cfg_TA_ENAINT = (unsigned_t)(TA_ENAINT);
const unsigned_t TOPPERS_cfg_TA_EDGE = (unsigned_t)(TA_EDGE);
const unsigned_t TOPPERS_cfg_TNFY_HANDLER = (unsigned_t)(TNFY_HANDLER);
const unsigned_t TOPPERS_cfg_TNFY_SETVAR = (unsigned_t)(TNFY_SETVAR);
const unsigned_t TOPPERS_cfg_TNFY_INCVAR = (unsigned_t)(TNFY_INCVAR);
const unsigned_t TOPPERS_cfg_TNFY_ACTTSK = (unsigned_t)(TNFY_ACTTSK);
const unsigned_t TOPPERS_cfg_TNFY_WUPTSK = (unsigned_t)(TNFY_WUPTSK);
const unsigned_t TOPPERS_cfg_TNFY_SIGSEM = (unsigned_t)(TNFY_SIGSEM);
const unsigned_t TOPPERS_cfg_TNFY_SETFLG = (unsigned_t)(TNFY_SETFLG);
const unsigned_t TOPPERS_cfg_TNFY_SNDDTQ = (unsigned_t)(TNFY_SNDDTQ);
const unsigned_t TOPPERS_cfg_TENFY_SETVAR = (unsigned_t)(TENFY_SETVAR);
const unsigned_t TOPPERS_cfg_TENFY_INCVAR = (unsigned_t)(TENFY_INCVAR);
const unsigned_t TOPPERS_cfg_TENFY_ACTTSK = (unsigned_t)(TENFY_ACTTSK);
const unsigned_t TOPPERS_cfg_TENFY_WUPTSK = (unsigned_t)(TENFY_WUPTSK);
const unsigned_t TOPPERS_cfg_TENFY_SIGSEM = (unsigned_t)(TENFY_SIGSEM);
const unsigned_t TOPPERS_cfg_TENFY_SETFLG = (unsigned_t)(TENFY_SETFLG);
const unsigned_t TOPPERS_cfg_TENFY_SNDDTQ = (unsigned_t)(TENFY_SNDDTQ);
const signed_t TOPPERS_cfg_TMIN_TPRI = (signed_t)(TMIN_TPRI);
const signed_t TOPPERS_cfg_TMAX_TPRI = (signed_t)(TMAX_TPRI);
const signed_t TOPPERS_cfg_TMIN_DPRI = (signed_t)(TMIN_DPRI);
const signed_t TOPPERS_cfg_TMAX_DPRI = (signed_t)(TMAX_DPRI);
const signed_t TOPPERS_cfg_TMIN_ISRPRI = (signed_t)(TMIN_ISRPRI);
const signed_t TOPPERS_cfg_TMAX_ISRPRI = (signed_t)(TMAX_ISRPRI);
const unsigned_t TOPPERS_cfg_TBIT_FLGPTN = (unsigned_t)(TBIT_FLGPTN);
const unsigned_t TOPPERS_cfg_TMAX_MAXSEM = (unsigned_t)(TMAX_MAXSEM);
const unsigned_t TOPPERS_cfg_TMAX_RELTIM = (unsigned_t)(TMAX_RELTIM);
const signed_t TOPPERS_cfg_TMIN_INTPRI = (signed_t)(TMIN_INTPRI);
const signed_t TOPPERS_cfg_TMAX_INTPRI = (signed_t)(TMAX_INTPRI);
const signed_t TOPPERS_cfg_TMIN_TSKID = (signed_t)(TMIN_TSKID);
const signed_t TOPPERS_cfg_TMIN_SEMID = (signed_t)(TMIN_SEMID);
const signed_t TOPPERS_cfg_TMIN_FLGID = (signed_t)(TMIN_FLGID);
const signed_t TOPPERS_cfg_TMIN_DTQID = (signed_t)(TMIN_DTQID);
const signed_t TOPPERS_cfg_TMIN_PDQID = (signed_t)(TMIN_PDQID);
const signed_t TOPPERS_cfg_TMIN_MTXID = (signed_t)(TMIN_MTXID);
const signed_t TOPPERS_cfg_TMIN_MPFID = (signed_t)(TMIN_MPFID);
const signed_t TOPPERS_cfg_TMIN_CYCID = (signed_t)(TMIN_CYCID);
const signed_t TOPPERS_cfg_TMIN_ALMID = (signed_t)(TMIN_ALMID);
#if defined(USE_TSKINICTXB)
const signed_t TOPPERS_cfg_USE_TSKINICTXB = (signed_t)(true);
#endif
#if defined(OMIT_INITIALIZE_INTERRUPT)
const signed_t TOPPERS_cfg_OMIT_INITIALIZE_INTERRUPT = (signed_t)(true);
#endif
#if defined(USE_INHINIB_TABLE)
const signed_t TOPPERS_cfg_USE_INHINIB_TABLE = (signed_t)(true);
#endif
#if defined(USE_INTINIB_TABLE)
const signed_t TOPPERS_cfg_USE_INTINIB_TABLE = (signed_t)(true);
#endif
#if defined(OMIT_INITIALIZE_EXCEPTION)
const signed_t TOPPERS_cfg_OMIT_INITIALIZE_EXCEPTION = (signed_t)(true);
#endif
#if defined(DEFAULT_ISTK)
const unsigned_t TOPPERS_cfg_DEFAULT_ISTK = (unsigned_t)(DEFAULT_ISTK);
#endif
#if defined(TARGET_TSKATR)
const unsigned_t TOPPERS_cfg_TARGET_TSKATR = (unsigned_t)(TARGET_TSKATR);
#else
const unsigned_t TOPPERS_cfg_TARGET_TSKATR = (unsigned_t)(0);
#endif
#if defined(TARGET_INTATR)
const unsigned_t TOPPERS_cfg_TARGET_INTATR = (unsigned_t)(TARGET_INTATR);
#else
const unsigned_t TOPPERS_cfg_TARGET_INTATR = (unsigned_t)(0);
#endif
#if defined(TARGET_INHATR)
const unsigned_t TOPPERS_cfg_TARGET_INHATR = (unsigned_t)(TARGET_INHATR);
#else
const unsigned_t TOPPERS_cfg_TARGET_INHATR = (unsigned_t)(0);
#endif
#if defined(TARGET_ISRATR)
const unsigned_t TOPPERS_cfg_TARGET_ISRATR = (unsigned_t)(TARGET_ISRATR);
#else
const unsigned_t TOPPERS_cfg_TARGET_ISRATR = (unsigned_t)(0);
#endif
#if defined(TARGET_EXCATR)
const unsigned_t TOPPERS_cfg_TARGET_EXCATR = (unsigned_t)(TARGET_EXCATR);
#else
const unsigned_t TOPPERS_cfg_TARGET_EXCATR = (unsigned_t)(0);
#endif
#if defined(TARGET_MIN_STKSZ)
const unsigned_t TOPPERS_cfg_TARGET_MIN_STKSZ = (unsigned_t)(TARGET_MIN_STKSZ);
#else
const unsigned_t TOPPERS_cfg_TARGET_MIN_STKSZ = (unsigned_t)(1);
#endif
#if defined(TARGET_MIN_ISTKSZ)
const unsigned_t TOPPERS_cfg_TARGET_MIN_ISTKSZ = (unsigned_t)(TARGET_MIN_ISTKSZ);
#else
const unsigned_t TOPPERS_cfg_TARGET_MIN_ISTKSZ = (unsigned_t)(1);
#endif
#if defined(CHECK_STKSZ_ALIGN)
const unsigned_t TOPPERS_cfg_CHECK_STKSZ_ALIGN = (unsigned_t)(CHECK_STKSZ_ALIGN);
#else
const unsigned_t TOPPERS_cfg_CHECK_STKSZ_ALIGN = (unsigned_t)(1);
#endif
#if defined(CHECK_INTPTR_ALIGN)
const unsigned_t TOPPERS_cfg_CHECK_INTPTR_ALIGN = (unsigned_t)(CHECK_INTPTR_ALIGN);
#else
const unsigned_t TOPPERS_cfg_CHECK_INTPTR_ALIGN = (unsigned_t)(1);
#endif
#if defined(CHECK_INTPTR_NONNULL)
const signed_t TOPPERS_cfg_CHECK_INTPTR_NONNULL = (signed_t)(true);
#endif
#if defined(CHECK_FUNC_ALIGN)
const unsigned_t TOPPERS_cfg_CHECK_FUNC_ALIGN = (unsigned_t)(CHECK_FUNC_ALIGN);
#else
const unsigned_t TOPPERS_cfg_CHECK_FUNC_ALIGN = (unsigned_t)(1);
#endif
#if defined(CHECK_FUNC_NONNULL)
const signed_t TOPPERS_cfg_CHECK_FUNC_NONNULL = (signed_t)(true);
#endif
#if defined(CHECK_STACK_ALIGN)
const unsigned_t TOPPERS_cfg_CHECK_STACK_ALIGN = (unsigned_t)(CHECK_STACK_ALIGN);
#else
const unsigned_t TOPPERS_cfg_CHECK_STACK_ALIGN = (unsigned_t)(1);
#endif
#if defined(CHECK_STACK_NONNULL)
const signed_t TOPPERS_cfg_CHECK_STACK_NONNULL = (signed_t)(true);
#endif
#if defined(CHECK_MPF_ALIGN)
const unsigned_t TOPPERS_cfg_CHECK_MPF_ALIGN = (unsigned_t)(CHECK_MPF_ALIGN);
#else
const unsigned_t TOPPERS_cfg_CHECK_MPF_ALIGN = (unsigned_t)(1);
#endif
#if defined(CHECK_MPF_NONNULL)
const signed_t TOPPERS_cfg_CHECK_MPF_NONNULL = (signed_t)(true);
#endif
const unsigned_t TOPPERS_cfg_sizeof_void_ptr = (unsigned_t)(sizeof(void*));
const unsigned_t TOPPERS_cfg_sizeof_uint_t = (unsigned_t)(sizeof(uint_t));
const unsigned_t TOPPERS_cfg_sizeof_size_t = (unsigned_t)(sizeof(size_t));
const unsigned_t TOPPERS_cfg_sizeof_intptr_t = (unsigned_t)(sizeof(intptr_t));
const unsigned_t TOPPERS_cfg_sizeof_ID = (unsigned_t)(sizeof(ID));
const unsigned_t TOPPERS_cfg_sizeof_FP = (unsigned_t)(sizeof(FP));
const unsigned_t TOPPERS_cfg_sizeof_INTNO = (unsigned_t)(sizeof(INTNO));
const unsigned_t TOPPERS_cfg_sizeof_INHNO = (unsigned_t)(sizeof(INHNO));
const unsigned_t TOPPERS_cfg_sizeof_EXCNO = (unsigned_t)(sizeof(EXCNO));
const unsigned_t TOPPERS_cfg_sizeof_TASK = (unsigned_t)(sizeof(TASK));
const unsigned_t TOPPERS_cfg_sizeof_TMEHDR = (unsigned_t)(sizeof(TMEHDR));
const unsigned_t TOPPERS_cfg_sizeof_ISR = (unsigned_t)(sizeof(ISR));
const unsigned_t TOPPERS_cfg_sizeof_INTHDR = (unsigned_t)(sizeof(INTHDR));
const unsigned_t TOPPERS_cfg_sizeof_EXCHDR = (unsigned_t)(sizeof(EXCHDR));
const unsigned_t TOPPERS_cfg_sizeof_INIRTN = (unsigned_t)(sizeof(INIRTN));
const unsigned_t TOPPERS_cfg_sizeof_TERRTN = (unsigned_t)(sizeof(TERRTN));
const unsigned_t TOPPERS_cfg_sizeof_NFYHDR = (unsigned_t)(sizeof(NFYHDR));
const unsigned_t TOPPERS_cfg_sizeof_TINIB = (unsigned_t)(sizeof(TINIB));
const unsigned_t TOPPERS_cfg_offsetof_TINIB_tskatr = (unsigned_t)(offsetof(TINIB,tskatr));
const unsigned_t TOPPERS_cfg_offsetof_TINIB_exinf = (unsigned_t)(offsetof(TINIB,exinf));
const unsigned_t TOPPERS_cfg_offsetof_TINIB_task = (unsigned_t)(offsetof(TINIB,task));
const unsigned_t TOPPERS_cfg_offsetof_TINIB_ipriority = (unsigned_t)(offsetof(TINIB,ipriority));
#if !defined(USE_TSKINICTXB)
const unsigned_t TOPPERS_cfg_offsetof_TINIB_stksz = (unsigned_t)(offsetof(TINIB,stksz));
#endif
#if !defined(USE_TSKINICTXB)
const unsigned_t TOPPERS_cfg_offsetof_TINIB_stk = (unsigned_t)(offsetof(TINIB,stk));
#endif
const unsigned_t TOPPERS_cfg_sizeof_SEMINIB = (unsigned_t)(sizeof(SEMINIB));
const unsigned_t TOPPERS_cfg_offsetof_SEMINIB_sematr = (unsigned_t)(offsetof(SEMINIB,sematr));
const unsigned_t TOPPERS_cfg_offsetof_SEMINIB_isemcnt = (unsigned_t)(offsetof(SEMINIB,isemcnt));
const unsigned_t TOPPERS_cfg_offsetof_SEMINIB_maxsem = (unsigned_t)(offsetof(SEMINIB,maxsem));
const unsigned_t TOPPERS_cfg_sizeof_FLGPTN = (unsigned_t)(sizeof(FLGPTN));
const unsigned_t TOPPERS_cfg_sizeof_FLGINIB = (unsigned_t)(sizeof(FLGINIB));
const unsigned_t TOPPERS_cfg_offsetof_FLGINIB_flgatr = (unsigned_t)(offsetof(FLGINIB,flgatr));
const unsigned_t TOPPERS_cfg_offsetof_FLGINIB_iflgptn = (unsigned_t)(offsetof(FLGINIB,iflgptn));
const unsigned_t TOPPERS_cfg_sizeof_DTQINIB = (unsigned_t)(sizeof(DTQINIB));
const unsigned_t TOPPERS_cfg_offsetof_DTQINIB_dtqatr = (unsigned_t)(offsetof(DTQINIB,dtqatr));
const unsigned_t TOPPERS_cfg_offsetof_DTQINIB_dtqcnt = (unsigned_t)(offsetof(DTQINIB,dtqcnt));
const unsigned_t TOPPERS_cfg_offsetof_DTQINIB_p_dtqmb = (unsigned_t)(offsetof(DTQINIB,p_dtqmb));
const unsigned_t TOPPERS_cfg_sizeof_PDQINIB = (unsigned_t)(sizeof(PDQINIB));
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_pdqatr = (unsigned_t)(offsetof(PDQINIB,pdqatr));
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_pdqcnt = (unsigned_t)(offsetof(PDQINIB,pdqcnt));
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_maxdpri = (unsigned_t)(offsetof(PDQINIB,maxdpri));
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_p_pdqmb = (unsigned_t)(offsetof(PDQINIB,p_pdqmb));
const unsigned_t TOPPERS_cfg_sizeof_MTXINIB = (unsigned_t)(sizeof(MTXINIB));
const unsigned_t TOPPERS_cfg_offsetof_MTXINIB_mtxatr = (unsigned_t)(offsetof(MTXINIB,mtxatr));
const unsigned_t TOPPERS_cfg_offsetof_MTXINIB_ceilpri = (unsigned_t)(offsetof(MTXINIB,ceilpri));
const unsigned_t TOPPERS_cfg_sizeof_MPFINIB = (unsigned_t)(sizeof(MPFINIB));
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_mpfatr = (unsigned_t)(offsetof(MPFINIB,mpfatr));
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_blkcnt = (unsigned_t)(offsetof(MPFINIB,blkcnt));
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_blksz = (unsigned_t)(offsetof(MPFINIB,blksz));
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_mpf = (unsigned_t)(offsetof(MPFINIB,mpf));
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_p_mpfmb = (unsigned_t)(offsetof(MPFINIB,p_mpfmb));
const unsigned_t TOPPERS_cfg_sizeof_CYCINIB = (unsigned_t)(sizeof(CYCINIB));
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cycatr = (unsigned_t)(offsetof(CYCINIB,cycatr));
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_exinf = (unsigned_t)(offsetof(CYCINIB,exinf));
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_nfyhdr = (unsigned_t)(offsetof(CYCINIB,nfyhdr));
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cyctim = (unsigned_t)(offsetof(CYCINIB,cyctim));
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cycphs = (unsigned_t)(offsetof(CYCINIB,cycphs));
const unsigned_t TOPPERS_cfg_sizeof_ALMINIB = (unsigned_t)(sizeof(ALMINIB));
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_almatr = (unsigned_t)(offsetof(ALMINIB,almatr));
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_exinf = (unsigned_t)(offsetof(ALMINIB,exinf));
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_nfyhdr = (unsigned_t)(offsetof(ALMINIB,nfyhdr));
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_sizeof_INHINIB = (unsigned_t)(sizeof(INHINIB));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_offset_INHINIB_inhno = (unsigned_t)(offsetof(INHINIB,inhno));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_offset_INHINIB_inhatr = (unsigned_t)(offsetof(INHINIB,inhatr));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_offset_INHINIB_int_entry = (unsigned_t)(offsetof(INHINIB,int_entry));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_sizeof_INTINIB = (unsigned_t)(sizeof(INTINIB));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_offset_INTINIB_intno = (unsigned_t)(offsetof(INTINIB,intno));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_offset_INTINIB_intatr = (unsigned_t)(offsetof(INTINIB,intatr));
#endif
#if !defined(OMIT_INITIALIZE_INTERRUPT)
const unsigned_t TOPPERS_cfg_offset_INTINIB_intpri = (unsigned_t)(offsetof(INTINIB,intpri));
#endif
#if !defined(OMIT_INITIALIZE_EXCEPTION)
const unsigned_t TOPPERS_cfg_sizeof_EXCINIB = (unsigned_t)(sizeof(EXCINIB));
#endif
#if !defined(OMIT_INITIALIZE_EXCEPTION)
const unsigned_t TOPPERS_cfg_offset_EXCINIB_excno = (unsigned_t)(offsetof(EXCINIB,excno));
#endif
#if !defined(OMIT_INITIALIZE_EXCEPTION)
const unsigned_t TOPPERS_cfg_offset_EXCINIB_excatr = (unsigned_t)(offsetof(EXCINIB,excatr));
#endif
#if !defined(OMIT_INITIALIZE_EXCEPTION)
const unsigned_t TOPPERS_cfg_offset_EXCINIB_exc_entry = (unsigned_t)(offsetof(EXCINIB,exc_entry));
#endif
#if defined(TARGET_RZA1H)
const signed_t TOPPERS_cfg_TARGET_RZA1H = (signed_t)(true);
#endif
#if defined(TARGET_RZA1L)
const signed_t TOPPERS_cfg_TARGET_RZA1L = (signed_t)(true);
#endif
const unsigned_t TOPPERS_cfg_TA_NEGEDGE = (unsigned_t)(TA_NEGEDGE);
const unsigned_t TOPPERS_cfg_TA_POSEDGE = (unsigned_t)(TA_POSEDGE);
const unsigned_t TOPPERS_cfg_TA_BOTHEDGE = (unsigned_t)(TA_BOTHEDGE);
const unsigned_t TOPPERS_cfg_INTNO_IRQ0 = (unsigned_t)(INTNO_IRQ0);
const unsigned_t TOPPERS_cfg_INTNO_IRQ7 = (unsigned_t)(INTNO_IRQ7);
const unsigned_t TOPPERS_cfg_TARGET_ARCH_ARM = (unsigned_t)(__TARGET_ARCH_ARM);
#if defined(USE_INTCFG_TABLE)
const signed_t TOPPERS_cfg_USE_INTCFG_TABLE = (signed_t)(true);
#endif
const unsigned_t TOPPERS_cfg_sizeof_TCB = (unsigned_t)(sizeof(TCB));
const unsigned_t TOPPERS_cfg_offsetof_TCB_p_tinib = (unsigned_t)(offsetof(TCB,p_tinib));
const unsigned_t TOPPERS_cfg_offsetof_TCB_sp = (unsigned_t)(offsetof(TCB,tskctxb.sp));
const unsigned_t TOPPERS_cfg_offsetof_TCB_pc = (unsigned_t)(offsetof(TCB,tskctxb.pc));
const unsigned_t TOPPERS_cfg_offsetof_T_EXCINF_cpsr = (unsigned_t)(offsetof(T_EXCINF,cpsr));


#line 10 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_1 = 1;
#line 10 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_iniatr_1 = (unsigned_t)(TA_NULL);

#line 11 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_2 = 2;
#line 11 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_teratr_2 = (unsigned_t)(TA_NULL);

#line 12 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_3 = 3;
#line 12 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_3 = (unsigned_t)(INTNO_TIMER);
#line 12 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_intatr_3 = (unsigned_t)(TA_ENAINT|INTATR_TIMER);
#line 12 "../arch/arm_gcc/rza1/chip_timer.cfg"
const signed_t TOPPERS_cfg_valueof_intpri_3 = (signed_t)(INTPRI_TIMER);

#line 13 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_4 = 4;
#line 13 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_inhno_4 = (unsigned_t)(INHNO_TIMER);
#line 13 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_inhatr_4 = (unsigned_t)(TA_NULL);

#ifdef TOPPERS_SUPPORT_OVRHDR

#line 19 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_5 = 5;
#line 19 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_iniatr_5 = (unsigned_t)(TA_NULL);

#line 20 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_6 = 6;
#line 20 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_teratr_6 = (unsigned_t)(TA_NULL);

#line 21 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_7 = 7;
#line 21 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_7 = (unsigned_t)(INTNO_OVRTIMER);
#line 21 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_intatr_7 = (unsigned_t)(TA_ENAINT|INTATR_OVRTIMER);
#line 21 "../arch/arm_gcc/rza1/chip_timer.cfg"
const signed_t TOPPERS_cfg_valueof_intpri_7 = (signed_t)(INTPRI_OVRTIMER);

#line 22 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_8 = 8;
#line 22 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_inhno_8 = (unsigned_t)(INHNO_OVRTIMER);
#line 22 "../arch/arm_gcc/rza1/chip_timer.cfg"
const unsigned_t TOPPERS_cfg_valueof_inhatr_8 = (unsigned_t)(TA_NULL);

#endif

#line 2 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_9 = 9;
#define TSKID_tTask_LogTask_Task	(<>)
#line 2 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_9 = (unsigned_t)(TA_ACT);
#line 2 "./gen/tecsgen.cfg"
const signed_t TOPPERS_cfg_valueof_itskpri_9 = (signed_t)(3);
#line 2 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_stksz_9 = (unsigned_t)(4096);

#line 3 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_10 = 10;
#define SEMID_tSemaphore_SerialPort1_ReceiveSemaphore	(<>)
#line 3 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_10 = (unsigned_t)(TA_NULL);
#line 3 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_isemcnt_10 = (unsigned_t)(0);
#line 3 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_maxsem_10 = (unsigned_t)(1);

#line 4 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_11 = 11;
#define SEMID_tSemaphore_SerialPort1_SendSemaphore	(<>)
#line 4 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_11 = (unsigned_t)(TA_NULL);
#line 4 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_isemcnt_11 = (unsigned_t)(1);
#line 4 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_maxsem_11 = (unsigned_t)(1);

#line 5 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_12 = 12;
#line 5 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_12 = (unsigned_t)(INTNO_SCIF2_RXI);
#line 5 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_intatr_12 = (unsigned_t)(TA_NULL);
#line 5 "./gen/tecsgen.cfg"
const signed_t TOPPERS_cfg_valueof_intpri_12 = (signed_t)(-4);

#line 6 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_13 = 13;
#line 6 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_13 = (unsigned_t)(INTNO_SCIF2_TXI);
#line 6 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_intatr_13 = (unsigned_t)(TA_NULL);
#line 6 "./gen/tecsgen.cfg"
const signed_t TOPPERS_cfg_valueof_intpri_13 = (signed_t)(-4);

#line 8 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_14 = 14;
#define ISRID_tISR_SIOPortTarget1_RxISRInstance	(<>)
#line 8 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_isratr_14 = (unsigned_t)(TA_NULL);
#line 8 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_14 = (unsigned_t)(INTNO_SCIF2_RXI);
#line 8 "./gen/tecsgen.cfg"
const signed_t TOPPERS_cfg_valueof_isrpri_14 = (signed_t)(1);

#line 9 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_15 = 15;
#define ISRID_tISR_SIOPortTarget1_TxISRInstance	(<>)
#line 9 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_isratr_15 = (unsigned_t)(TA_NULL);
#line 9 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_15 = (unsigned_t)(INTNO_SCIF2_TXI);
#line 9 "./gen/tecsgen.cfg"
const signed_t TOPPERS_cfg_valueof_isrpri_15 = (signed_t)(1);

#line 11 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_16 = 16;
#line 11 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_iniatr_16 = (unsigned_t)(TA_NULL);

#line 13 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_static_api_17 = 17;
#line 13 "./gen/tecsgen.cfg"
const unsigned_t TOPPERS_cfg_valueof_teratr_17 = (unsigned_t)(TA_NULL);

#line 9 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_18 = 18;
#define TASK1	(<>)
#line 9 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_18 = (unsigned_t)(TA_NULL);
#line 9 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_itskpri_18 = (signed_t)(MID_PRIORITY);
#line 9 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_stksz_18 = (unsigned_t)(STACK_SIZE);

#line 10 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_19 = 19;
#define TASK2	(<>)
#line 10 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_19 = (unsigned_t)(TA_NULL);
#line 10 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_itskpri_19 = (signed_t)(MID_PRIORITY);
#line 10 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_stksz_19 = (unsigned_t)(STACK_SIZE);

#line 11 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_20 = 20;
#define TASK3	(<>)
#line 11 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_20 = (unsigned_t)(TA_NULL);
#line 11 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_itskpri_20 = (signed_t)(MID_PRIORITY);
#line 11 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_stksz_20 = (unsigned_t)(STACK_SIZE);

#line 12 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_21 = 21;
#define MAIN_TASK	(<>)
#line 12 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_21 = (unsigned_t)(TA_ACT);
#line 12 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_itskpri_21 = (signed_t)(MAIN_PRIORITY);
#line 12 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_stksz_21 = (unsigned_t)(STACK_SIZE);

#line 13 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_22 = 22;
#define EXC_TASK	(<>)
#line 13 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_22 = (unsigned_t)(TA_NULL);
#line 13 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_itskpri_22 = (signed_t)(EXC_PRIORITY);
#line 13 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_stksz_22 = (unsigned_t)(STACK_SIZE);

#line 14 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_23 = 23;
#define CYCHDR1	(<>)
#line 14 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_cycatr_23 = (unsigned_t)(TA_NULL);
#line 14 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_nfymode_23 = (unsigned_t)(TNFY_HANDLER);
#line 14 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_cyctim_23 = (unsigned_t)(2000000);
#line 14 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_cycphs_23 = (unsigned_t)(0);

#line 15 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_24 = 24;
#define ALMHDR1	(<>)
#line 15 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_almatr_24 = (unsigned_t)(TA_NULL);
#line 15 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_nfymode_24 = (unsigned_t)(TNFY_HANDLER);

#ifdef INTNO1

#line 17 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_25 = 25;
#line 17 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_25 = (unsigned_t)(INTNO1);
#line 17 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_intatr_25 = (unsigned_t)(INTNO1_INTATR);
#line 17 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_intpri_25 = (signed_t)(INTNO1_INTPRI);

#line 18 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_26 = 26;
#define INTNO1_ISR	(<>)
#line 18 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_isratr_26 = (unsigned_t)(TA_NULL);
#line 18 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_intno_26 = (unsigned_t)(INTNO1);
#line 18 "../sample/sample1.cfg"
const signed_t TOPPERS_cfg_valueof_isrpri_26 = (signed_t)(1);

#endif

#ifdef CPUEXC1

#line 21 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_static_api_27 = 27;
#line 21 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_excno_27 = (unsigned_t)(CPUEXC1);
#line 21 "../sample/sample1.cfg"
const unsigned_t TOPPERS_cfg_valueof_excatr_27 = (unsigned_t)(TA_NULL);

#endif

