/*
 * This file was automatically generated by tecsgen.
 * This file is not intended to be edited.
 */
#ifndef tTerminateRoutine_TECSGEN_H
#define tTerminateRoutine_TECSGEN_H

/*
 * celltype          :  tTerminateRoutine
 * global name       :  tTerminateRoutine
 * idx_is_id(actual) :  no(no)
 * singleton         :  no
 * has_CB            :  false
 * has_INIB          :  0
 * rom               :  yes
 * CB initializer    :  yes
 */

/* global header #_IGH_# */
#include "global_tecsgen.h"

/* signature header #_ISH_# */
#include "sRoutineBody_tecsgen.h"

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* cell INIB type definition #_CIP_# */
typedef const struct tag_tTerminateRoutine_INIB {
    /* call port #_TCP_# */
}  tTerminateRoutine_INIB;

/* CB not exist. CB corresponding to INIB #_DCI_# */
#define tTerminateRoutine_CB_tab           tTerminateRoutine_INIB_tab
#define tTerminateRoutine_SINGLE_CELL_CB   tTerminateRoutine_SINGLE_CELL_INIB
#define tTerminateRoutine_CB               tTerminateRoutine_INIB
#define tag_tTerminateRoutine_CB           tag_tTerminateRoutine_INIB

extern tTerminateRoutine_CB  tTerminateRoutine_CB_tab[];

/* celltype IDX type #_CTIX_# */
typedef const struct tag_tTerminateRoutine_INIB *tTerminateRoutine_IDX;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

/* to get the definition of CB type of referenced celltype for optimization #_ICT_# */
#ifndef  TOPPERS_CB_TYPE_ONLY
#define  tTerminateRoutine_CB_TYPE_ONLY
#define TOPPERS_CB_TYPE_ONLY
#endif  /* TOPPERS_CB_TYPE_ONLY */
#include "tLogTaskMain_tecsgen.h"
#ifdef  tTerminateRoutine_CB_TYPE_ONLY
#undef TOPPERS_CB_TYPE_ONLY
#endif /* tTerminateRoutine_CB_TYPE_ONLY */
#ifndef TOPPERS_CB_TYPE_ONLY

#define tTerminateRoutine_ID_BASE        (1)  /* ID Base  #_NIDB_# */
#define tTerminateRoutine_N_CELL        (1)  /*  number of cells  #_NCEL_# */

/* IDX validation macro #_CVI_# */
#define tTerminateRoutine_VALID_IDX(IDX) (1)


/* celll CB macro #_GCB_# */
#define tTerminateRoutine_GET_CELLCB(idx) (idx)
 /* call port function macro #_CPM_# */
#define tTerminateRoutine_cTerminateRoutineBody_main( p_that ) \
	  tLogTaskMain_eLogTaskTerminate_main( \
	    )

#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef TOPPERS_CB_TYPE_ONLY

#endif /* TOPPERS_CB_TYPE_ONLY */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_CB_TYPE_ONLY

/* IDX validation macro (abbrev.) #_CVIA_# */
#define VALID_IDX(IDX)  tTerminateRoutine_VALID_IDX(IDX)


/* cell CB macro (abbrev) #_GCBA_# */
#define GET_CELLCB(idx)  tTerminateRoutine_GET_CELLCB(idx)

/* CELLCB type (abbrev) #_CCT_# */
#define CELLCB	tTerminateRoutine_CB

/* celltype IDX type (abbrev) #_CTIXA_# */
#define CELLIDX	tTerminateRoutine_IDX

/* call port function macro (abbrev) #_CPMA_# */
#define cTerminateRoutineBody_main( ) \
          ((void)p_cellcb, tTerminateRoutine_cTerminateRoutineBody_main( p_cellcb ))



/* iteration code (FOREACH_CELL) #_FEC_# */
#define FOREACH_CELL(i,p_cb)   \
    for( (i) = 0; (i) < tTerminateRoutine_N_CELL; (i)++ ){ \
       (p_cb) = &tTerminateRoutine_CB_tab[i];

#define END_FOREACH_CELL   }

/* CB initialize macro #_CIM_# */
#define INITIALIZE_CB(p_that)	(void)(p_that);
#define SET_CB_INIB_POINTER(i,p_that)\
	/* empty */
#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */

#endif /* tTerminateRoutine_TECSGENH */