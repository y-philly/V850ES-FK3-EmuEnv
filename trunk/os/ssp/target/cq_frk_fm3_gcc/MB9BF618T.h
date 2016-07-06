/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2012 Meika Sugimoto
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
*      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 */

#ifndef TOPPERS_MB9BF618T_H
#define TOPPERS_MB9BF618T_H

/*
 *  CORTEX-M3 CPUの依存部のインクルード
 */
#include "arm_m_gcc/arm_m.h"

/*
 *  プロセッサのエンディアン定義
 *    MB9BF618Tはリトルエンディアン
 */
#define SIL_ENDIAN_LITTLE


/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO   (16 + 48)

/*
 *  割込み優先度のビット幅
 */
#define TBITW_IPRI     (4)


/*
 *  割込みベクタ番号定義
 */
#define INT_FCS					(16)
#define INT_LVD					(17)
#define INT_SW_WDT				(18)
#define INT_MFT					(19)
#define INT_EXTI0TO7			(20)
#define INT_EXT_8TO31			(21)
#define INT_DUALTMR_QPRC		(22)
#define INT_MFS0_RX				(23)
#define INT_MFS0_TX				(24)
#define INT_MFS1_RX				(25)
#define INT_MFS1_TX				(26)
#define INT_MFS2_RX				(27)
#define INT_MFS2_TX				(28)
#define INT_MFS3_RX				(29)
#define INT_MFS3_TX				(30)
#define INT_MFS4_RX				(31)
#define INT_MFS4_TX				(32)
#define INT_MFS5_RX				(33)
#define INT_MFS5_TX				(34)
#define INT_MFS6_RX				(35)
#define INT_MFS6_TX				(36)
#define INT_MFS7_RX				(37)
#define INT_MFS7_TX				(38)
#define INT_PPG					(39)
#define INT_CLK_CNT				(40)
#define INT_AD0					(41)
#define INT_AD1					(42)
#define INT_AD2					(43)
#define INT_BASETMR_0TO7		(44)
#define INT_MFT_FREERUN			(45)
#define INT_MFT_INCAPTURE		(46)
#define INT_OUTCOMPARE			(47)
#define INT_CAN0_ETH0			(48)
#define INT_CAN1_ETH1			(49)
#define INT_USB0_FNC			(50)
#define INT_USB0_FNCHOST		(51)
#define INT_USB1_FNC			(52)
#define INT_USB1_FNCHOST		(53)
#define INT_DMA0				(54)
#define INT_DMA1				(55)
#define INT_DMA2				(56)
#define INT_DMA3				(57)
#define INT_DMA4				(58)
#define INT_DMA5				(59)
#define INT_DMA6				(60)
#define INT_DMA7				(61)
#define INT_BASETMR_8TO15		(62)
#define INT_RESERVE				(63)


/*
 *  割込み優先度ビット幅中のサブ優先度のビット幅
 */
#define TBITW_SUBIPRI  0


/* MB9BF618Tのペリフェラルレジスタ定義 */

/* BUS:AHB */
#define AHBPERIPH_BASE			(0x40000000)

#define FLASHIF_BASE			(AHBPERIPH_BASE + 0x0000)

/* BUS:APB0 */
#define APB0PERIPH_BASE			(0x40010000)

#define CLOCKRESET_BASE			(APB0PERIPH_BASE + 0x0000)
#define HW_WDT_BASE				(APB0PERIPH_BASE + 0x1000)
#define SW_WDT_BASE				(APB0PERIPH_BASE + 0x2000)
#define DUALTMR_BASE			(APB0PERIPH_BASE + 0x5000)

/* BUS:APB1 */
#define APB1PERIPH_BASE			(0x40020000)
#define MFT0_BASE				(APB1PERIPH_BASE + 0x0000)
#define MFT1_BASE				(APB1PERIPH_BASE + 0x1000)
#define PPG_BASE				(APB1PERIPH_BASE + 0x4000)
#define BASETMR_BASE			(APB1PERIPH_BASE + 0x5000)
#define QUADCNT_BASE			(APB1PERIPH_BASE + 0x5000)
#define ADCONV_BASE				(APB1PERIPH_BASE + 0x7000)
#define HSPDCR_TRIM_BASE		(APB1PERIPH_BASE + 0xE000)

/* BUS:APB2 */
#define APB2PERIPH_BASE			(0x40030000)
#define EXTI_BASE				(APB2PERIPH_BASE + 0x0000)
#define INTREQ_BASE				(APB2PERIPH_BASE + 0x1000)
#define GPIO_BASE				(APB2PERIPH_BASE + 0x3000)
#define LVD_BASE				(APB2PERIPH_BASE + 0x5000)
#define DSTBY_MODE_BASE			(APB2PERIPH_BASE + 0x5800)
#define USBCLK_BASE				(APB2PERIPH_BASE + 0x6000)
#define CAN_PRESCL_BASE			(APB2PERIPH_BASE + 0x7000)
#define MFS_BASE				(APB2PERIPH_BASE + 0x8000)
#define CRC_BASE				(APB2PERIPH_BASE + 0x9000)
#define WATCHCNT_BASE			(APB2PERIPH_BASE + 0xA000)
#define RTC_BASE				(APB2PERIPH_BASE + 0xB000)
#define EXTBUS_BASE				(APB2PERIPH_BASE + 0xF000)

/* BUS:AHB */
#define USB_CH0_BASE			(AHBPERIPH_BASE + 0x40000)
#define USB_CH1_BASE			(AHBPERIPH_BASE + 0x50000)
#define DMAC_BASE				(AHBPERIPH_BASE + 0x60000)
#define CAN_CH0_BASE			(AHBPERIPH_BASE + 0x62000)
#define CAN_CH1_BASE			(AHBPERIPH_BASE + 0x63000)
#define ETH0_BASE				(AHBPERIPH_BASE + 0x64000)
#define ETHSYS_BASE				(AHBPERIPH_BASE + 0x66000)
#define ETH1_BASE				(AHBPERIPH_BASE + 0x67000)

#define FLASHIF_WORK_BASE		(0x200E0000)

/* クロック/リセット関連のレジスタ */
#define SCM_CTL					(CLOCKRESET_BASE + 0x0000)
#define SCM_STR					(CLOCKRESET_BASE + 0x0004)
#define STB_CTL					(CLOCKRESET_BASE + 0x0008)	/* 4byte access */
#define RST_STR					(CLOCKRESET_BASE + 0x000C)	/* 2byte access */
#define BSC_PSR					(CLOCKRESET_BASE + 0x0010)
#define APBC0_PSR				(CLOCKRESET_BASE + 0x0014)
#define APBC1_PSR				(CLOCKRESET_BASE + 0x0018)
#define APBC2_PSR				(CLOCKRESET_BASE + 0x001C)
#define TTC_PSR					(CLOCKRESET_BASE + 0x0028)
#define CSW_PSR					(CLOCKRESET_BASE + 0x0030)
#define PSW_TMR					(CLOCKRESET_BASE + 0x0034)
#define PLL_CTL1				(CLOCKRESET_BASE + 0x0038)
#define PLL_CTL2				(CLOCKRESET_BASE + 0x003C)
#define INT_ENR					(CLOCKRESET_BASE + 0x0060)
#define INT_STR					(CLOCKRESET_BASE + 0x0064)
#define INT_CLR					(CLOCKRESET_BASE + 0x0068)

/* SCM_CTL , SCM_STRのビット割当て */
#define SCM_CTL_RCS_HIGHCR		(0 << 5)
#define SCM_CTL_RCS_MAINCLK		(1 << 5)
#define SCM_CTL_RCS_PLLCLK		(2 << 5)
#define SCM_CTL_RCS_LOWCR		(4 << 5)
#define SCM_CTL_RCS_SUBCLK		(5 << 5)
#define SCM_CTL_RCS_MASK		(7 << 5)

#define SCM_CTL_MOSCE			(1 << 1)
#define SCM_CTL_SOSCE			(1 << 3)
#define SCM_CTL_PLLE			(1 << 4)
#define SCM_STR_MORDY			(1 << 1)
#define SCM_STR_SOSCE			(1 << 3)
#define SCM_STR_PLRDY			(1 << 4)

/* クロック分周比設定値 */
#define BSC_CLK_DIV_1			(0)
#define BSC_CLK_DIV_2			(1)
#define BSC_CLK_DIV_3			(2)
#define BSC_CLK_DIV_4			(3)
#define BSC_CLK_DIV_6			(4)
#define BSC_CLK_DIV_8			(5)
#define BSC_CLK_DIV_16			(6)

/* APBクロック制御設定値 */
#define APBC_EN					(1 << 7)
#define APBC_RESET				(1 << 4)

/* クロック分周比設定値 */
#define APB_CLK_DIV_1			(0)
#define APB_CLK_DIV_2			(1)
#define APB_CLK_DIV_4			(2)
#define APB_CLK_DIV_8			(3)


/* クロック関連割込みのビット割当て */
#define INT_CLL_MCS				(1 << 0)
#define INT_CLL_SCS				(1 << 1)
#define INT_CLL_PCS				(1 << 2)
#define INT_CLL_FCS				(1 << 5)

/* CSW_TMRのビット割当て */
#define PSW_TMR_MOWT_CONF0		(0 << 0)		/* 2^1  / FCRH */
#define PSW_TMR_MOWT_CONF1		(1 << 0)		/* 2^5 / FCRH */
#define PSW_TMR_MOWT_CONF2		(2 << 0)		/* 2^6 / FCRH */
#define PSW_TMR_MOWT_CONF3		(3 << 0)		/* 2^7 / FCRH */
#define PSW_TMR_MOWT_CONF4		(4 << 0)		/* 2^8 / FCRH */
#define PSW_TMR_MOWT_CONF5		(5 << 0)		/* 2^9  / FCRH */
#define PSW_TMR_MOWT_CONF6		(6 << 0)		/* 2^10 / FCRH */
#define PSW_TMR_MOWT_CONF7		(7 << 0)		/* 2^11 / FCRH */
#define PSW_TMR_MOWT_CONF8		(8 << 0)		/* 2^12 / FCRH */
#define PSW_TMR_MOWT_CONF9		(9 << 0)		/* 2^13 / FCRH */
#define PSW_TMR_MOWT_CONF10		(10 << 0)		/* 2^14 / FCRH */
#define PSW_TMR_MOWT_CONF11		(11 << 0)		/* 2^16 / FCRH */
#define PSW_TMR_MOWT_CONF12		(12 << 0)		/* 2^17 / FCRH */
#define PSW_TMR_MOWT_CONF13		(13 << 0)		/* 2^19 / FCRH */
#define PSW_TMR_MOWT_CONF14		(14 << 0)		/* 2^21 / FCRH */
#define PSW_TMR_MOWT_CONF15		(15 << 0)		/* 2^23 / FCRH */

/* PSW_TMRのビット割当て */
#define PSW_TMR_PINC_MO			(0 << 4)
#define PSW_TMR_PINC_CR			(1 << 4)
#define PSW_TMR_POWT_CONF0		(0 << 0)		/* 2^9  / FCRH */
#define PSW_TMR_POWT_CONF1		(1 << 0)		/* 2^10 / FCRH */
#define PSW_TMR_POWT_CONF2		(2 << 0)		/* 2^11 / FCRH */
#define PSW_TMR_POWT_CONF3		(3 << 0)		/* 2^12 / FCRH */
#define PSW_TMR_POWT_CONF4		(4 << 0)		/* 2^13 / FCRH */
#define PSW_TMR_POWT_CONF5		(5 << 0)		/* 2^14 / FCRH */
#define PSW_TMR_POWT_CONF6		(6 << 0)		/* 2^15 / FCRH */
#define PSW_TMR_POWT_CONF7		(7 << 0)		/* 2^16 / FCRH */

/* PLL_CTL1 , PLL_CTL2のビット割当て */
#define PLL_CTL1_PLLK(n)		(((n) - 1) << 4)
#define PLL_CTL1_PLLM(n)		(((n) - 1) << 0)
#define PLL_CTL2_PLLN(n)		(((n) - 1) << 0)

/* WDT */
#define WDG_CTL					(HW_WDT_BASE + 0x0008)
#define WDG_LCK					(HW_WDT_BASE + 0x0C00)

#define WDG_RESDIS				(0 << 1)
#define WDG_RESENA				(1 << 1)
#define WDG_RESINTDIS			(0 << 0)
#define WDG_RESINTENA			(1 << 0)

#define WDG_UNLOCK_VAL			(0x1ACCE551)

/* MFS(Multi Functional Serial */
#define MFS_BASEADDR(ch)		(MFS_BASE + ((ch) * 0x0100))

#define MFS_SMR(ch)				(MFS_BASEADDR((ch)) + 0x00)
#define MFS_SCR(ch)				(MFS_BASEADDR((ch)) + 0x01)
#define MFS_ESCR(ch)			(MFS_BASEADDR((ch)) + 0x04)
#define MFS_SSR(ch)				(MFS_BASEADDR((ch)) + 0x05)
#define MFS_RDRTDR(ch)			(MFS_BASEADDR((ch)) + 0x08)	/* 2byte access */
#define MFS_BGR0(ch)			(MFS_BASEADDR((ch)) + 0x0C)
#define MFS_BGR1(ch)			(MFS_BASEADDR((ch)) + 0x0D)
#define MFS_FCR0(ch)			(MFS_BASEADDR((ch)) + 0x14)
#define MFS_FCR1(ch)			(MFS_BASEADDR((ch)) + 0x15)

/* GPIO */
#define BITMAP_0				(1 << 0)
#define BITMAP_1				(1 << 1)
#define BITMAP_2				(1 << 2)
#define BITMAP_3				(1 << 3)
#define BITMAP_4				(1 << 4)
#define BITMAP_5				(1 << 5)
#define BITMAP_6				(1 << 6)
#define BITMAP_7				(1 << 7)

/* #define GPIO_BASE				(APB2PERIPH_BASE + 0x3000) */
#define PFR0					(GPIO_BASE + 0x0000)
#define PFR1					(GPIO_BASE + 0x0004)
#define PFR2					(GPIO_BASE + 0x0008)
#define PFR3					(GPIO_BASE + 0x000C)
#define PFR4					(GPIO_BASE + 0x0010)
#define PFR5					(GPIO_BASE + 0x0014)
#define PFR6					(GPIO_BASE + 0x0018)
#define PFR7					(GPIO_BASE + 0x001C)
#define PFR8					(GPIO_BASE + 0x0020)
#define PFR9					(GPIO_BASE + 0x0024)
#define PFRA					(GPIO_BASE + 0x0028)
#define PFRB					(GPIO_BASE + 0x002C)
#define PFRC					(GPIO_BASE + 0x0030)
#define PFRD					(GPIO_BASE + 0x0034)
#define PFRE					(GPIO_BASE + 0x0038)
#define PFRF					(GPIO_BASE + 0x003C)

#define PDDR0					(GPIO_BASE + 0x0200)
#define PDDR1					(GPIO_BASE + 0x0204)
#define PDDR2					(GPIO_BASE + 0x0208)
#define PDDR3					(GPIO_BASE + 0x020C)
#define PDDR4					(GPIO_BASE + 0x0210)
#define PDDR5					(GPIO_BASE + 0x0214)
#define PDDR6					(GPIO_BASE + 0x0218)
#define PDDR7					(GPIO_BASE + 0x021C)
#define PDDR8					(GPIO_BASE + 0x0220)
#define PDDR9					(GPIO_BASE + 0x0224)
#define PDDRA					(GPIO_BASE + 0x0228)
#define PDDRB					(GPIO_BASE + 0x022C)
#define PDDRC					(GPIO_BASE + 0x0230)
#define PDDRD					(GPIO_BASE + 0x0234)
#define PDDRE					(GPIO_BASE + 0x0238)
#define PDDRF					(GPIO_BASE + 0x023C)

#define PDIR0					(GPIO_BASE + 0x0300)
#define PDIR1					(GPIO_BASE + 0x0304)
#define PDIR2					(GPIO_BASE + 0x0308)
#define PDIR3					(GPIO_BASE + 0x030C)
#define PDIR4					(GPIO_BASE + 0x0310)
#define PDIR5					(GPIO_BASE + 0x0314)
#define PDIR6					(GPIO_BASE + 0x0318)
#define PDIR7					(GPIO_BASE + 0x031C)
#define PDIR8					(GPIO_BASE + 0x0320)
#define PDIR9					(GPIO_BASE + 0x0324)
#define PDIRA					(GPIO_BASE + 0x0328)
#define PDIRB					(GPIO_BASE + 0x032C)
#define PDIRC					(GPIO_BASE + 0x0330)
#define PDIRD					(GPIO_BASE + 0x0334)
#define PDIRE					(GPIO_BASE + 0x0338)
#define PDIRF					(GPIO_BASE + 0x033C)

#define PDOR0					(GPIO_BASE + 0x0400)
#define PDOR1					(GPIO_BASE + 0x0404)
#define PDOR2					(GPIO_BASE + 0x0408)
#define PDOR3					(GPIO_BASE + 0x040C)
#define PDOR4					(GPIO_BASE + 0x0410)
#define PDOR5					(GPIO_BASE + 0x0414)
#define PDOR6					(GPIO_BASE + 0x0418)
#define PDOR7					(GPIO_BASE + 0x041C)
#define PDOR8					(GPIO_BASE + 0x0420)
#define PDOR9					(GPIO_BASE + 0x0424)
#define PDORA					(GPIO_BASE + 0x0428)
#define PDORB					(GPIO_BASE + 0x042C)
#define PDORC					(GPIO_BASE + 0x0430)
#define PDORD					(GPIO_BASE + 0x0434)
#define PDORE					(GPIO_BASE + 0x0438)
#define PDORF					(GPIO_BASE + 0x043C)

#define EPFR0					(GPIO_BASE + 0x0600)
#define EPFR1					(GPIO_BASE + 0x0604)
#define EPFR2					(GPIO_BASE + 0x0608)
#define EPFR3					(GPIO_BASE + 0x060C)
#define EPFR4					(GPIO_BASE + 0x0610)
#define EPFR5					(GPIO_BASE + 0x0614)
#define EPFR6					(GPIO_BASE + 0x0618)
#define EPFR7					(GPIO_BASE + 0x061C)
#define EPFR8					(GPIO_BASE + 0x0620)
#define EPFR9					(GPIO_BASE + 0x0624)
#define EPFRA					(GPIO_BASE + 0x0628)
#define EPFRB					(GPIO_BASE + 0x062C)
#define EPFRC					(GPIO_BASE + 0x0630)
#define EPFRD					(GPIO_BASE + 0x0634)
#define EPFRE					(GPIO_BASE + 0x0638)
#define EPFRF					(GPIO_BASE + 0x063C)

#define PCR0					(GPIO_BASE + 0x0100)
#define PCR1					(GPIO_BASE + 0x0104)


#define ADE						(GPIO_BASE + 0x0500)

#endif /* TOPPERS_MB9BF618T_H */
