/* This file is generated from kernel_rename.def by genrename. */

/* This file is included only when kernel_rename.h has been included. */
#ifdef TOPPERS_KERNEL_RENAME_H
#undef TOPPERS_KERNEL_RENAME_H

/*
 *  startup.c
 */
#undef kerflg
#undef exit_kernel

/*
 *  task.c
 */
#undef runtsk_curpri
#undef runtsk_ipri
#undef ready_primap
#undef actque_bitmap
#undef reqflg
#undef disdsp
#undef get_ipri_self
#undef get_ipri
#undef bitmap_search_table
#undef bitmap_search
#undef primap_empty
#undef primap_test
#undef primap_search
#undef primap_set
#undef primap_clear
#undef search_schedtsk
#undef test_dormant
#undef initialize_task
#undef make_active
#undef run_task
#undef dispatcher

/*
 *  interrupt.c
 */
#undef initialize_interrupt

/*
 *  exception.c
 */
#undef initialize_exception


/*
 *  alarm.c
 */
#undef initialize_alarm
#undef call_almhdr

/*
 *  cyclic.c
 */
#undef initialize_cyclic
#undef call_cychdr

/*
 *  dataqueue.c
 */
#undef initialize_dataqueue
#undef send_data
#undef receive_data

/*
 *  eventflag.c
 */
#undef initialize_eventflag

/*
 *  time_event.c
 */
#undef current_time
#undef min_time
#undef initialize_time_event
#undef time_event_enqueue
#undef time_event_dequeue
#undef signal_time


/*
 *  kernel_cfg.c
 */
#undef tmax_tskid
#undef tinib_tskatr
#undef init_rdypmap
#undef tinib_exinf
#undef tinib_task
#undef tinib_epriority

#undef tmax_flgid
#undef flginib_atr
#undef flginib_iflgptn
#undef flgcb_flgptn

#undef tmax_dtqid
#undef dtqinib_atr
#undef dtqinib_size
#undef dtqinib_data
#undef dtqcb_count
#undef dtqcb_head
#undef dtqcb_tail

#undef tnum_inhno
#undef inhinib_inhno
#undef inhinib_inhatr
#undef inhinib_entry

#undef tnum_intno
#undef intinib_intno
#undef intinib_intatr
#undef intinib_intpri

#undef tnum_excno
#undef excinib_excno
#undef excinib_excatr
#undef excinib_entry

#undef tmax_cycid
#undef cycevtid_offset
#undef cycinib_cycact
#undef cycinib_exinf
#undef cycinib_cychdr
#undef cycinib_cyctim
#undef cycinib_cycphs
#undef cyccb_evttim
#undef cyccb_cycact

#undef tmax_almid
#undef almevtid_offset
#undef alminib_almhdr
#undef alminib_exinf
#undef almcb_almact

#undef tnum_tmevt_queue
#undef tmevt_queue
#undef tmevt_time
#undef tmevt_callback
#undef tmevt_arg

#undef stksz
#undef stk
#undef istkpt

#undef initialize_object
#undef call_inirtn
#undef call_terrtn

/*
 *  target_config.c
 */
#undef target_initialize
#undef target_exit


#ifdef TOPPERS_LABEL_ASM

/*
 *  startup.c
 */
#undef _kerflg
#undef _exit_kernel

/*
 *  task.c
 */
#undef _runtsk_curpri
#undef _runtsk_ipri
#undef _ready_primap
#undef _actque_bitmap
#undef _reqflg
#undef _disdsp
#undef _get_ipri_self
#undef _get_ipri
#undef _bitmap_search_table
#undef _bitmap_search
#undef _primap_empty
#undef _primap_test
#undef _primap_search
#undef _primap_set
#undef _primap_clear
#undef _search_schedtsk
#undef _test_dormant
#undef _initialize_task
#undef _make_active
#undef _run_task
#undef _dispatcher

/*
 *  interrupt.c
 */
#undef _initialize_interrupt

/*
 *  exception.c
 */
#undef _initialize_exception


/*
 *  alarm.c
 */
#undef _initialize_alarm
#undef _call_almhdr

/*
 *  cyclic.c
 */
#undef _initialize_cyclic
#undef _call_cychdr

/*
 *  dataqueue.c
 */
#undef _initialize_dataqueue
#undef _send_data
#undef _receive_data

/*
 *  eventflag.c
 */
#undef _initialize_eventflag

/*
 *  time_event.c
 */
#undef _current_time
#undef _min_time
#undef _initialize_time_event
#undef _time_event_enqueue
#undef _time_event_dequeue
#undef _signal_time


/*
 *  kernel_cfg.c
 */
#undef _tmax_tskid
#undef _tinib_tskatr
#undef _init_rdypmap
#undef _tinib_exinf
#undef _tinib_task
#undef _tinib_epriority

#undef _tmax_flgid
#undef _flginib_atr
#undef _flginib_iflgptn
#undef _flgcb_flgptn

#undef _tmax_dtqid
#undef _dtqinib_atr
#undef _dtqinib_size
#undef _dtqinib_data
#undef _dtqcb_count
#undef _dtqcb_head
#undef _dtqcb_tail

#undef _tnum_inhno
#undef _inhinib_inhno
#undef _inhinib_inhatr
#undef _inhinib_entry

#undef _tnum_intno
#undef _intinib_intno
#undef _intinib_intatr
#undef _intinib_intpri

#undef _tnum_excno
#undef _excinib_excno
#undef _excinib_excatr
#undef _excinib_entry

#undef _tmax_cycid
#undef _cycevtid_offset
#undef _cycinib_cycact
#undef _cycinib_exinf
#undef _cycinib_cychdr
#undef _cycinib_cyctim
#undef _cycinib_cycphs
#undef _cyccb_evttim
#undef _cyccb_cycact

#undef _tmax_almid
#undef _almevtid_offset
#undef _alminib_almhdr
#undef _alminib_exinf
#undef _almcb_almact

#undef _tnum_tmevt_queue
#undef _tmevt_queue
#undef _tmevt_time
#undef _tmevt_callback
#undef _tmevt_arg

#undef _stksz
#undef _stk
#undef _istkpt

#undef _initialize_object
#undef _call_inirtn
#undef _call_terrtn

/*
 *  target_config.c
 */
#undef _target_initialize
#undef _target_exit


#endif /* TOPPERS_LABEL_ASM */

#include "target_unrename.h"

#endif /* TOPPERS_KERNEL_RENAME_H */
