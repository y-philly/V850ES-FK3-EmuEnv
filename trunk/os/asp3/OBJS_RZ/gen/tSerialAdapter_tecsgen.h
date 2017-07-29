/*
 * This file was automatically generated by tecsgen.
 * This file is not intended to be edited.
 */
#ifndef tSerialAdapter_TECSGEN_H
#define tSerialAdapter_TECSGEN_H

/*
 * celltype          :  tSerialAdapter
 * global name       :  tSerialAdapter
 * idx_is_id(actual) :  no(no)
 * singleton         :  yes
 * has_CB            :  false
 * has_INIB          :  true
 * rom               :  yes
 * CB initializer    :  yes
 */

/* global header #_IGH_# */
#include "global_tecsgen.h"

/* signature header #_ISH_# */
#include "sSerialPort_tecsgen.h"

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* cell INIB type definition #_CIP_# */
typedef const struct tag_tSerialAdapter_INIB {
    /* call port #_TCP_# */
    struct tag_sSerialPort_VDES *const*cSerialPort;
    int_t n_cSerialPort;
}  tSerialAdapter_INIB;

/* CB not exist. CB corresponding to INIB #_DCI_# */
#define tSerialAdapter_CB_tab           tSerialAdapter_INIB_tab
#define tSerialAdapter_SINGLE_CELL_CB   tSerialAdapter_SINGLE_CELL_INIB
#define tSerialAdapter_CB               tSerialAdapter_INIB
#define tag_tSerialAdapter_CB           tag_tSerialAdapter_INIB

/* singleton cell CB prototype declaration #_SCP_# */
extern  tSerialAdapter_INIB  tSerialAdapter_SINGLE_CELL_INIB;


/* celltype IDX type #_CTIX_# */
typedef const struct tag_tSerialAdapter_INIB *tSerialAdapter_IDX;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_CB_TYPE_ONLY

/* call port array size macro #_NCPA_# */
#define N_CP_cSerialPort  (tSerialAdapter_SINGLE_CELL_INIB.n_cSerialPort)
#define NCP_cSerialPort   (tSerialAdapter_SINGLE_CELL_INIB.n_cSerialPort)

/* celll CB macro #_GCB_# */
#define tSerialAdapter_GET_CELLCB(idx) ((void *)0)
 /* call port function macro #_CPM_# */
#define tSerialAdapter_cSerialPort_open( subscript ) \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript]->VMT->open__T( \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript] )
#define tSerialAdapter_cSerialPort_close( subscript ) \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript]->VMT->close__T( \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript] )
#define tSerialAdapter_cSerialPort_read( subscript, buffer, length ) \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript]->VMT->read__T( \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript], (buffer), (length) )
#define tSerialAdapter_cSerialPort_write( subscript, buffer, length ) \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript]->VMT->write__T( \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript], (buffer), (length) )
#define tSerialAdapter_cSerialPort_control( subscript, ioControl ) \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript]->VMT->control__T( \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript], (ioControl) )
#define tSerialAdapter_cSerialPort_refer( subscript, pk_rpor ) \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript]->VMT->refer__T( \
	  tSerialAdapter_SINGLE_CELL_INIB.cSerialPort[subscript], (pk_rpor) )

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


/* cell CB macro (abbrev) #_GCBA_# */
#define GET_CELLCB(idx)  tSerialAdapter_GET_CELLCB(idx)

/* CELLCB type (abbrev) #_CCT_# */
#define CELLCB	tSerialAdapter_CB

/* celltype IDX type (abbrev) #_CTIXA_# */
#define CELLIDX	tSerialAdapter_IDX

/* call port function macro (abbrev) #_CPMA_# */
#define cSerialPort_open( subscript ) \
          tSerialAdapter_cSerialPort_open( subscript )
#define cSerialPort_close( subscript ) \
          tSerialAdapter_cSerialPort_close( subscript )
#define cSerialPort_read( subscript, buffer, length ) \
          tSerialAdapter_cSerialPort_read( subscript, buffer, length )
#define cSerialPort_write( subscript, buffer, length ) \
          tSerialAdapter_cSerialPort_write( subscript, buffer, length )
#define cSerialPort_control( subscript, ioControl ) \
          tSerialAdapter_cSerialPort_control( subscript, ioControl )
#define cSerialPort_refer( subscript, pk_rpor ) \
          tSerialAdapter_cSerialPort_refer( subscript, pk_rpor )



/* CB initialize macro #_CIM_# */
#define INITIALIZE_CB()
#define SET_CB_INIB_POINTER(i,p_that)\
	/* empty */
#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */

#endif /* tSerialAdapter_TECSGENH */