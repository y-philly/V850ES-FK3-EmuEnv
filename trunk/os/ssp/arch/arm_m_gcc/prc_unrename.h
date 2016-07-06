/* This file is generated from prc_rename.def by genrename. */

/* This file is included only when prc_rename.h has been included. */
#ifdef TOPPERS_PRC_RENAME_H
#undef TOPPERS_PRC_RENAME_H

/*
 *  prc_config.c
 */
#undef lock_flag
#undef saved_iipm
#undef intnest

#undef set_exc_int_priority
#undef enable_exc
#undef disable_exc
#undef prc_initialize
#undef prc_terminate
#undef x_config_int
#undef default_exc_handler
#undef default_int_handler

/*
 *  prc_support.S
 */
#undef exc_entry
#undef int_entry
#undef ret_exc
#undef ret_int
#undef svc_handler
#undef start_dispatch
#undef call_exit_kernel

/*
 *  kernel_cfg.c
 */
#undef exc_tbl
#undef bitpat_cfgint
#undef vector_table

#ifdef TOPPERS_LABEL_ASM

/*
 *  prc_config.c
 */
#undef _lock_flag
#undef _saved_iipm
#undef _intnest

#undef _set_exc_int_priority
#undef _enable_exc
#undef _disable_exc
#undef _prc_initialize
#undef _prc_terminate
#undef _x_config_int
#undef _default_exc_handler
#undef _default_int_handler

/*
 *  prc_support.S
 */
#undef _exc_entry
#undef _int_entry
#undef _ret_exc
#undef _ret_int
#undef _svc_handler
#undef _start_dispatch
#undef _call_exit_kernel

/*
 *  kernel_cfg.c
 */
#undef _exc_tbl
#undef _bitpat_cfgint
#undef _vector_table

#endif /* TOPPERS_LABEL_ASM */


#endif /* TOPPERS_PRC_RENAME_H */
