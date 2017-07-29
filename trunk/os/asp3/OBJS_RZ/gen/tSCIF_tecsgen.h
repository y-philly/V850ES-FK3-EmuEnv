/*
 * This file was automatically generated by tecsgen.
 * This file is not intended to be edited.
 */
#ifndef tSCIF_TECSGEN_H
#define tSCIF_TECSGEN_H

/*
 * celltype          :  tSCIF
 * global name       :  tSCIF
 * idx_is_id(actual) :  no(no)
 * singleton         :  no
 * has_CB            :  true
 * has_INIB          :  true
 * rom               :  yes
 * CB initializer    :  yes
 */

/* global header #_IGH_# */
#include "global_tecsgen.h"

/* signature header #_ISH_# */
#include "sSIOPort_tecsgen.h"
#include "siSIOCBR_tecsgen.h"
#include "siHandlerBody_tecsgen.h"

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* cell INIB type definition #_CIP_# */
typedef const struct tag_tSCIF_INIB {
    /* call port #_TCP_# */
    /* call port #_NEP_# */ 
    /* attribute(RO) #_ATO_# */ 
    uintptr_t      baseAddress;
    uint32_t       baudRate;
}  tSCIF_INIB;
/* cell CB type definition #_CCTPA_# */
typedef struct tag_tSCIF_CB {
    tSCIF_INIB  *_inib;
    /* call port #_TCP_# */
    /* call port #_NEP_# */ 
    /* var #_VA_# */ 
    bool_t         initialized;
}  tSCIF_CB;
extern tSCIF_CB  tSCIF_CB_tab[];

/* celltype IDX type #_CTIX_# */
typedef struct tag_tSCIF_CB *tSCIF_IDX;

/* prototype declaration of entry port function #_EPP_# */
/* sSIOPort */
void         tSCIF_eSIOPort_open(tSCIF_IDX idx);
void         tSCIF_eSIOPort_close(tSCIF_IDX idx);
bool_t       tSCIF_eSIOPort_putChar(tSCIF_IDX idx, char c);
int_t        tSCIF_eSIOPort_getChar(tSCIF_IDX idx);
void         tSCIF_eSIOPort_enableCBR(tSCIF_IDX idx, uint_t cbrtn);
void         tSCIF_eSIOPort_disableCBR(tSCIF_IDX idx, uint_t cbrtn);
/* siHandlerBody */
void         tSCIF_eiRxISR_main(tSCIF_IDX idx);
/* siHandlerBody */
void         tSCIF_eiTxISR_main(tSCIF_IDX idx);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

/* to get the definition of CB type of referenced celltype for optimization #_ICT_# */
#ifndef  TOPPERS_CB_TYPE_ONLY
#define  tSCIF_CB_TYPE_ONLY
#define TOPPERS_CB_TYPE_ONLY
#endif  /* TOPPERS_CB_TYPE_ONLY */
#include "tSIOPortGRPeachMain_tecsgen.h"
#ifdef  tSCIF_CB_TYPE_ONLY
#undef TOPPERS_CB_TYPE_ONLY
#endif /* tSCIF_CB_TYPE_ONLY */
#ifndef TOPPERS_CB_TYPE_ONLY

#define tSCIF_ID_BASE               (1)  /* ID Base  #_NIDB_# */
#define tSCIF_N_CELL                (1)  /*  number of cells  #_NCEL_# */

/* IDX validation macro #_CVI_# */
#define tSCIF_VALID_IDX(IDX) (1)


/* celll CB macro #_GCB_# */
#define tSCIF_GET_CELLCB(idx) (idx)

/* attr access  #_AAM_# */
#define tSCIF_ATTR_baseAddress( p_that )	((p_that)->_inib->baseAddress)
#define tSCIF_ATTR_baudRate( p_that )	((p_that)->_inib->baudRate)

#define tSCIF_GET_baseAddress(p_that)	((p_that)->_inib->baseAddress)
#define tSCIF_GET_baudRate(p_that)	((p_that)->_inib->baudRate)


/* var access macro #_VAM_# */
#define tSCIF_VAR_initialized(p_that)	((p_that)->initialized)

#define tSCIF_GET_initialized(p_that)	((p_that)->initialized)
#define tSCIF_SET_initialized(p_that,val)	((p_that)->initialized=(val))

 /* call port function macro #_CPM_# */
#define tSCIF_ciSIOCBR_readySend( p_that ) \
	  tSIOPortGRPeachMain_eiSIOCBR_readySend( \
	   &tSIOPortGRPeachMain_CB_tab[0] )
#define tSCIF_ciSIOCBR_readyReceive( p_that ) \
	  tSIOPortGRPeachMain_eiSIOCBR_readyReceive( \
	   &tSIOPortGRPeachMain_CB_tab[0] )

#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* prototype declaration of entry port function (referenced when VMT useless optimise enabled) #_EPSP_# */
/* eiRxISR */
void           tSCIF_eiRxISR_main_skel( const struct tag_siHandlerBody_VDES *epd);
/* eiTxISR */
void           tSCIF_eiTxISR_main_skel( const struct tag_siHandlerBody_VDES *epd);

#ifndef TOPPERS_CB_TYPE_ONLY

#endif /* TOPPERS_CB_TYPE_ONLY */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_CB_TYPE_ONLY

/* IDX validation macro (abbrev.) #_CVIA_# */
#define VALID_IDX(IDX)  tSCIF_VALID_IDX(IDX)


/* cell CB macro (abbrev) #_GCBA_# */
#define GET_CELLCB(idx)  tSCIF_GET_CELLCB(idx)

/* CELLCB type (abbrev) #_CCT_# */
#define CELLCB	tSCIF_CB

/* celltype IDX type (abbrev) #_CTIXA_# */
#define CELLIDX	tSCIF_IDX


/* attr access macro (abbrev) #_AAMA_# */
#define ATTR_baseAddress     tSCIF_ATTR_baseAddress( p_cellcb )
#define ATTR_baudRate        tSCIF_ATTR_baudRate( p_cellcb )


/* var access macro (abbrev) #_VAMA_# */
#define VAR_initialized      tSCIF_VAR_initialized( p_cellcb )

/* call port function macro (abbrev) #_CPMA_# */
#define ciSIOCBR_readySend( ) \
          ((void)p_cellcb, tSCIF_ciSIOCBR_readySend( p_cellcb ))
#define ciSIOCBR_readyReceive( ) \
          ((void)p_cellcb, tSCIF_ciSIOCBR_readyReceive( p_cellcb ))




/* entry port function macro (abbrev) #_EPM_# */
#define eSIOPort_open    tSCIF_eSIOPort_open
#define eSIOPort_close   tSCIF_eSIOPort_close
#define eSIOPort_putChar tSCIF_eSIOPort_putChar
#define eSIOPort_getChar tSCIF_eSIOPort_getChar
#define eSIOPort_enableCBR tSCIF_eSIOPort_enableCBR
#define eSIOPort_disableCBR tSCIF_eSIOPort_disableCBR
#define eiRxISR_main     tSCIF_eiRxISR_main
#define eiTxISR_main     tSCIF_eiTxISR_main

/* iteration code (FOREACH_CELL) #_FEC_# */
#define FOREACH_CELL(i,p_cb)   \
    for( (i) = 0; (i) < tSCIF_N_CELL; (i)++ ){ \
       (p_cb) = &tSCIF_CB_tab[i];

#define END_FOREACH_CELL   }

/* CB initialize macro #_CIM_# */
#define INITIALIZE_CB(p_that)\
	(p_that)->initialized = false;
#define SET_CB_INIB_POINTER(i,p_that)\
	(p_that)->_inib = &tSCIF_INIB_tab[(i)];

#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */

#endif /* tSCIF_TECSGENH */