

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

#include "v850es_fk3_emu_env.h"


#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */


#define TARGET_PUTC_PORTID	(1)
#define TARGET_PUTC_BAUD	(38400)

#if TARGET_PUTC_PORTID == 1
	#define TARGET_FPUTC_UAnSTR	UA0STR
	#define TARGET_FPUTC_UAnTX	UA0TX
#elif TARGET_PUTC_PORTID == 2
	#define TARGET_FPUTC_UAnSTR	UA1STR
	#define TARGET_FPUTC_UAnTX	UA1TX
#elif TARGET_PUTC_PORTID == 3
	#define TARGET_FPUTC_UAnSTR	UA2STR
	#define TARGET_FPUTC_UAnTX	UA2TX
#else
	#error "TARGET_PUTC_BAUD must set [1 - 3]."
#endif


#if TARGET_PUTC_BAUD == 38400
	#define TARGET_FPUTC_UAnCTL1_SETTING		BAUD_38400_UA0CTL1
	#define TARGET_FPUTC_UAnCTL2_SETTING		BAUD_38400_UA0CTL2
#elif TARGET_TARGET_FPUTC_PUTC_BAUD == 19200
	#define TARGET_FPUTC_UAnCTL1_SETTING		BAUD_19200_UA0CTL1
	#define TARGET_FPUTC_UAnCTL2_SETTING		BAUD_19200_UA0CTL2
#elif TARGET_PUTC_BAUD == 9600
	#define TARGET_FPUTC_UAnCTL1_SETTING		BAUD_9600_UA0CTL1
	#define TARGET_FPUTC_UAnCTL2_SETTING		BAUD_9600_UA0CTL2
#else
	#error "TARGET_PUTC_BAUD must set 38400 , 19200 , or 9600."
#endif





#define DEFAULT_ISTKSZ    0x2000U

#define DEFAULT_ISTK      (void *)(DEFAULT_STK_TOP - DEFAULT_ISTKSZ)

#ifndef TOPPERS_MACRO_ONLY


extern void target_initialize(void);


extern void target_exit(void);

#endif /* TOPPERS_MACRO_ONLY */


#include "prc_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */
