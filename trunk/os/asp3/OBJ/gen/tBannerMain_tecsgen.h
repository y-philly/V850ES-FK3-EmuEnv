/*
 * This file was automatically generated by tecsgen.
 * This file is not intended to be edited.
 */
#ifndef tBannerMain_TECSGEN_H
#define tBannerMain_TECSGEN_H

/*
 * celltype          :  tBannerMain
 * global name       :  tBannerMain
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
#include "sRoutineBody_tecsgen.h"

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* cell INIB type definition #_CIP_# */
typedef const struct tag_tBannerMain_INIB {
    /* call port #_NEP_# */ 
    /* attribute(RO) #_ATO_# */ 
    char*          targetName;
    char*          copyrightNotice;
}  tBannerMain_INIB;

/* CB not exist. CB corresponding to INIB #_DCI_# */
#define tBannerMain_CB_tab           tBannerMain_INIB_tab
#define tBannerMain_SINGLE_CELL_CB   tBannerMain_SINGLE_CELL_INIB
#define tBannerMain_CB               tBannerMain_INIB
#define tag_tBannerMain_CB           tag_tBannerMain_INIB

/* singleton cell CB prototype declaration #_SCP_# */
extern  tBannerMain_INIB  tBannerMain_SINGLE_CELL_INIB;


/* celltype IDX type #_CTIX_# */
typedef const struct tag_tBannerMain_INIB *tBannerMain_IDX;

/* prototype declaration of entry port function #_EPP_# */
/* sRoutineBody */
void         tBannerMain_eBannerInitialize_main();
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_CB_TYPE_ONLY


/* celll CB macro #_GCB_# */
#define tBannerMain_GET_CELLCB(idx) ((void *)0)

/* attr access  #_AAM_# */
#define tBannerMain_ATTR_targetName	(tBannerMain_SINGLE_CELL_INIB.targetName)
#define tBannerMain_ATTR_copyrightNotice	(tBannerMain_SINGLE_CELL_INIB.copyrightNotice)

#define tBannerMain_GET_targetName()	(tBannerMain_SINGLE_CELL_INIB.targetName)
#define tBannerMain_GET_copyrightNotice()	(tBannerMain_SINGLE_CELL_INIB.copyrightNotice)



#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* prototype declaration of entry port function (referenced when VMT useless optimise enabled) #_EPSP_# */

#ifndef TOPPERS_CB_TYPE_ONLY

#endif /* TOPPERS_CB_TYPE_ONLY */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_CB_TYPE_ONLY


/* cell CB macro (abbrev) #_GCBA_# */
#define GET_CELLCB(idx)  tBannerMain_GET_CELLCB(idx)

/* CELLCB type (abbrev) #_CCT_# */
#define CELLCB	tBannerMain_CB

/* celltype IDX type (abbrev) #_CTIXA_# */
#define CELLIDX	tBannerMain_IDX


/* attr access macro (abbrev) #_AAMA_# */
#define ATTR_targetName      tBannerMain_ATTR_targetName
#define ATTR_copyrightNotice tBannerMain_ATTR_copyrightNotice





/* entry port function macro (abbrev) #_EPM_# */
#define eBannerInitialize_main tBannerMain_eBannerInitialize_main

/* CB initialize macro #_CIM_# */
#define INITIALIZE_CB()
#define SET_CB_INIB_POINTER(i,p_that)\
	/* empty */
#endif /* TOPPERS_CB_TYPE_ONLY */

#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */

#endif /* tBannerMain_TECSGENH */
