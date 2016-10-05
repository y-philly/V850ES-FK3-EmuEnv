#include <stdio.h>

#include "intc.h"

IntcControlType intc_control;


void intc_init(CpuManagerType *cpu)
{
	int i;
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *xxIcn;
	uint16 *imr0_6;
	uint8 *imr7;
	uint8 *ispr;

	/*
	 * intc_controlの初期化
	 */
	/*
	 * NMI初期化
	 */
	intc_control.nmi.intwdt2_hasreq = FALSE;
	intc_control.nmi.nmi_reqnum = 0;


	/*
	 * マスカブル割り込み初期化
	 */
	for (i = 0; i < INTC_NUM; i++) {
		intc_control.is_waiting_lvl[i] = INTC_NUM_INTLVL;
		intc_control.waiting_lvl_num[i] = 0;
	}
	intc_control.current_intno = -1;
	intc_control.waiting_int_num = 0;

	intc_control.saved_intno_off = 0;
	for (i = 0; i < INTC_NUM_INTLVL; i++) {
		intc_control.saved_intno_stack[i] = 0;
	}
	/*
	 * xxIcnの初期化
	 */
	for (i = 0; i < INTC_NUM; i++) {
		regaddr = intc_regaddr_icn(i);
		cpu_memget_addrp(cpu, regaddr, &pregaddr);
		xxIcn = (uint8*)pregaddr;

		*xxIcn = 0x47;
	}

	/*
	 * IMR0-IMR7の初期化
	 */
	cpu_memget_addrp(cpu, INTC_REG_IMR0, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR1, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR2, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR3, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR4, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR5, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR6, &pregaddr);
	imr0_6 = (uint16*)pregaddr;
	*imr0_6 = 0xFFFF;

	cpu_memget_addrp(cpu, INTC_REG_IMR7, &pregaddr);
	imr7 = (uint8*)pregaddr;
	*imr7 = 0x1F;

	/*
	 * ISPRの初期化
	 */
	cpu_memget_addrp(cpu, INTC_REG_ISPR, &pregaddr);
	ispr = (uint8*)pregaddr;
	*ispr = 0x00;

	return;
}
static void set_wait_intno(int intno, uint32 lvl)
{
	if (intc_control.is_waiting_lvl[intno] == INTC_NUM_INTLVL) {
		intc_control.is_waiting_lvl[intno] = lvl;
		intc_control.waiting_lvl_num[intno] = 1;
		intc_control.waiting_int_num++;
	}
}
static void clr_wait_intno(int intno)
{
	intc_control.waiting_lvl_num[intno]--;
	//printf("clr_wait_intno:intno=%d lvl_num=%d\n", intno, intc_control.waiting_lvl_num[intno]);
	if (intc_control.waiting_lvl_num[intno] == 0) {
		intc_control.is_waiting_lvl[intno] = INTC_NUM_INTLVL;
	}
	intc_control.waiting_int_num--;
}
int is_masked(CpuManagerType *cpu, uint32 intno)
{
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *xxIcn;
	uint8 wdata;

	regaddr = intc_regaddr_icn(intno);
	cpu_memget_addrp(cpu, regaddr, &pregaddr);
	xxIcn = (uint8*)pregaddr;

	wdata = *xxIcn;
	return INTC_ICN_ISSET_MK(wdata);
}

static int get_maxpri_itno(CpuManagerType *cpu)
{
	int i;
	int next_intno = -1;
	uint32 tmp_lvl;
	uint32 minlvl;

	if (intc_control.waiting_int_num == 0) {
		return -1;
	}

	if (intc_control.current_intno >= 0) {
		/*
		 * 現在実行中のものよりも高い優先度をもつものを取得する
		 */
		minlvl = (((uint32)intc_control.is_waiting_lvl[intc_control.current_intno]) << 16) | ((INTC_DEFAULT_PRIORITY(intc_control.current_intno) << 0));
	}
	else {
		/*
		 * もっとも高い優先度をもつものを取得する
		 */
		minlvl = 0xFFFFFFFF;
	}
	for (i = 0; i < INTC_NUM; i++) {
		if (intc_control.is_waiting_lvl[i] == INTC_NUM_INTLVL) {
			continue;
		}
		if (intc_control.current_intno == i) {
			/*
			 * すでに実行中のものはチェック対象外．
			 */
			continue;
		}
		/*
		 * マスクされているものは対象外とする
		 */
		if (is_masked(cpu, i) == TRUE) {
			continue;
		}
		/*
		 * ディフォールトの優先度と同時比較するため，32ビットで評価する．
		 * ※ディフォールトの優先度は割り込み優先度よりも低い評価となるため，下位ビットに設定する
		 * ※値が小さい方が優先度が高いことに注意
		 *     優先度　　デフォルト優先度
		 * 最小：    0    |       0
		 * 最大：    7    |     115
		 *
		 */
		tmp_lvl = (((uint32)intc_control.is_waiting_lvl[i]) << 16) | ((INTC_DEFAULT_PRIORITY(i) << 0));

		//printf("tmplvl=%d minlvl=%d next_intno=%d intc_control.is_waiting_lvl[%d]=%d\n",
		//		tmp_lvl, minlvl, next_intno, i, intc_control.is_waiting_lvl[i]);
		if (tmp_lvl < minlvl) {
			minlvl = tmp_lvl;
			next_intno = i;
		}
	}
	return next_intno;
}

static void intc_raise(CpuManagerType *cpu, uint32 intno)
{
	uint16 wdata16;
	uint8 *ispr;
	uint32 *pregaddr;
	uint8 lvl;

	cpu_memget_addrp(cpu, INTC_REG_ISPR, &pregaddr);
	ispr = (uint8*)pregaddr;

	lvl = intc_control.is_waiting_lvl[intno];

	/*
	 * PSW.NP
	 */
	if (CPU_ISSET_NP(&cpu->cpu) == TRUE) {
		//printf("can not raise INT(%d) because NP is set\n", intno);
		return;
	}
	/*
	 * PSW.ID
	 */
	if (CPU_ISSET_ID(&cpu->cpu) == TRUE) {
		//printf("can not raise INT(%d) because ID is set\n", intno);
		return;
	}

	/*
	 * マスカブル割り込み要求発生
	 */
	if (intc_control.current_intno != -1) {
		intc_control.saved_intno_stack[intc_control.saved_intno_off] = intc_control.current_intno;
		intc_control.saved_intno_off++;
	}

	intc_control.current_intno = intno;

	cpu->cpu.eipc = cpu->cpu.pc;
	cpu->cpu.eipsw = cpu->cpu.psw;
	wdata16 = (cpu->cpu.ecr & 0xFF00);
	wdata16 |= INTC_MASK_ECR_CODE(intno);
	cpu->cpu.ecr = wdata16;

	CPU_SET_ID(&cpu->cpu);
	CPU_CLR_EP(&cpu->cpu);
	*ispr = (*ispr) | (1 << lvl);

	cpu->cpu.pc = INTC_MASK_INTR_ADDR(intno);

	if (intno >= 100) {
		//printf("RAISED INT(%d):waiting_num=%d\n", intno, intc_control.waiting_lvl_num[intno]);
	}
	return;
}

int intc_raise_intr(CpuManagerType *cpu, uint32 intno)
{
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *xxIcn;
	uint8 wdata;
	uint8 lvl;

	regaddr = intc_regaddr_icn(intno);
	cpu_memget_addrp(cpu, regaddr, &pregaddr);
	xxIcn = (uint8*)pregaddr;

	/*
	 * xxIF = 1
	 */
	wdata = *xxIcn;
	INTC_ICN_SET_IF(wdata);
	*xxIcn = wdata;
	lvl = INTC_ICN_PR(*xxIcn);

	/*
	 * 管理上，すべての割り込み要求はwaitingに入れておく．
	 */
	set_wait_intno(intno, lvl);

	cpu->is_halt = FALSE;

	return 0;
}
void intc_clr_currlvl_ispr(CpuManagerType *cpu)
{
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *ispr;
	uint8 *xxIcn;
	uint8 lvl;
	uint8 wdata;

	if (intc_control.current_intno == -1) {
		return;
	}

	cpu_memget_addrp(cpu, INTC_REG_ISPR, &pregaddr);
	ispr = (uint8*)pregaddr;

	regaddr = intc_regaddr_icn(intc_control.current_intno);
	cpu_memget_addrp(cpu, regaddr, &pregaddr);
	xxIcn = (uint8*)pregaddr;


	lvl = intc_control.is_waiting_lvl[intc_control.current_intno];
	*ispr = (*ispr) & ~(1 << lvl);

	/*
	 * ハードウェアマニュアル上は，Ｉｃｎのリセット契機が明記されていないが，
	 * 割込み終了契機でリセットすることにした．
	 */
	wdata = *xxIcn;
	INTC_ICN_CLR_IF(wdata);
	*xxIcn = wdata;


	/*
	 * 割込み終了タイミングでしか呼び出されないため，現在の割り込み情報はクリアする．
	 */
	clr_wait_intno(intc_control.current_intno);

	if (intc_control.saved_intno_off > 0) {
		intc_control.saved_intno_off--;
		intc_control.current_intno = intc_control.saved_intno_stack[intc_control.saved_intno_off];
	}
	else {
		intc_control.current_intno = -1;
	}
	return;
}

static bool can_raise_inwtdt2(CpuManagerType *cpu)
{
	return CPU_ISSET_NP(&cpu->cpu);
}
static void raise_intwdt2(CpuManagerType *cpu)
{
	cpu->is_halt = FALSE;

	cpu->cpu.fepc = 0x0;
	cpu->cpu.fepsw = cpu->cpu.psw;

	cpu->cpu.ecr |= (INTC_FECC_INTWDT2 << 16);

	CPU_SET_NP(&cpu->cpu);
	CPU_CLR_EP(&cpu->cpu);
	CPU_SET_ID(&cpu->cpu);

	cpu->cpu.pc = INTC_FECC_INTWDT2;

	//printf("RAISED INTWDT2\n");
	return;
}
static void raise_nmi(CpuManagerType *cpu)
{
	if (CPU_ISSET_NP(&cpu->cpu) == TRUE) {
		return;
	}
	cpu->is_halt = FALSE;

	cpu->cpu.fepc = cpu->cpu.pc;
	cpu->cpu.fepsw = cpu->cpu.psw;

	cpu->cpu.ecr |= (INTC_FECC_NMI << 16);

	CPU_SET_NP(&cpu->cpu);
	CPU_CLR_EP(&cpu->cpu);
	CPU_SET_ID(&cpu->cpu);


	cpu->cpu.pc = INTC_FECC_NMI;

	//printf("RAISED NMI\n");
	return;
}

/*
 *
 * 現在実行中およびペンディング中の割込みを全てチェックして，
 * 最高優先度のものを実行する．
 */
int intc_raise_pending_intr(CpuManagerType *cpu)
{
	/*
	 * INTWDT2割込みチェック
	 */
	if (intc_control.nmi.intwdt2_hasreq == TRUE) {
		if (can_raise_inwtdt2(cpu) == FALSE) {
			/*
			 * 新たな要求受付は禁止するため処理終了．
			 */
			return 0;
		}
		raise_intwdt2(cpu);
		return 0;
	}
	/*
	 * NMI割込みチェック
	 */
	if (intc_control.nmi.nmi_reqnum > 0) {
		raise_nmi(cpu);
		/*
		 * NMI割込みありの場合は，マスカブル割り込み受付は禁止するため処理終了．
		 */
		return 0;
	}

	/*
	 * マスカブル割り込みチェック
	 */
	int maxlvl_intno;

	maxlvl_intno = get_maxpri_itno(cpu);

	if (maxlvl_intno < 0) {
		return 0;
	}
	if (maxlvl_intno == intc_control.current_intno) {
		return 0;
	}

	intc_raise(cpu, maxlvl_intno);
	return 0;
}
bool intc_has_pending_intr(CpuManagerType *cpu)
{
	int maxlvl_intno;

	maxlvl_intno = get_maxpri_itno(cpu);

	if (maxlvl_intno < 0) {
		return FALSE;
	}
	return TRUE;
}

int intc_raise_nmi(CpuManagerType *cpu, uint32 nmino)
{
	if (nmino == INTC_NMINO_INTWDT2) {
		intc_control.nmi.intwdt2_hasreq = TRUE;
	}
	else {
		intc_control.nmi.nmi_reqnum++;
	}
	return 0;
}

void intc_clr_nmi(CpuManagerType *cpu)
{
	if (intc_control.nmi.intwdt2_hasreq == TRUE) {
		cpu_reset(cpu);
	}
	else {
		intc_control.nmi.nmi_reqnum--;
	}
	return;
}

static int get_imrno(uint32 regaddr, uint32**imraddr)
{
	int imrno = -1;
	*imraddr = (uint32*)(regaddr & 0x03FFFFFF);
	switch (regaddr) {
	case INTC_REG_IMR0:
		imrno = 0;
		break;
	case INTC_REG_IMR1:
		imrno = 1;
		break;
	case INTC_REG_IMR2:
		imrno = 2;
		break;
	case INTC_REG_IMR3:
		imrno = 3;
		break;
	case INTC_REG_IMR4:
		imrno = 4;
		break;
	case INTC_REG_IMR5:
		imrno = 5;
		break;
	case INTC_REG_IMR6:
		imrno = 6;
		break;
	case INTC_REG_IMR7:
		imrno = 7;
		break;
	default:
		break;
	}
	return imrno;
}

void intc_hook_update_reg8(CpuManagerType *cpu, uint32 regaddr, uint8 data)
{
	int imrno;
	uint32 *imraddr;
	uint8 *xxICn;
	uint32 regaddr32;
	uint32 *pregaddr32;
	int i;
	int pintno;
	int intno;

	imrno = get_imrno(regaddr, &imraddr);
	if (imrno < 0) {
		return;
	}
	pintno = imrno * 16;
	for (i = 0; i < 8; i++, pintno++) {
		if (pintno == INTC_PINTNO_NOUSE) {
			continue;
		}
		intno = intc_pintno2intno(pintno);
		if (intno > INTC_INTNO_MAX) {
			continue;
		}
		regaddr32 = intc_regaddr_icn(intno);
		cpu_memget_addrp(cpu, regaddr32, &pregaddr32);
		xxICn = (uint8*)pregaddr32;
		if (data & 1 << i) {
			*xxICn |= (1 << 6);
			//printf("xxIcn(intno=%d):set\n", intno);
		}
		else {
			*xxICn &= ~(1 << 6);
			//printf("xxIcn(intno=%d):clear\n", intno);
		}
	}
	return;
}
void intc_hook_update_reg16(CpuManagerType *cpu, uint32 regaddr, uint16 data)
{
	int imrno;
	uint32 *imraddr;
	uint8 *xxICn;
	uint32 regaddr32;
	uint32 *pregaddr32;
	int i;
	int pintno;
	int intno;

	imrno = get_imrno(regaddr, &imraddr);
	if (imrno < 0) {
		return;
	}
	pintno = imrno * 16;
	for (i = 0; i < 16; i++, pintno++) {
		if (pintno == INTC_PINTNO_NOUSE) {
			continue;
		}
		intno = intc_pintno2intno(pintno);
		if (intno > INTC_INTNO_MAX) {
			continue;
		}
		regaddr32 = intc_regaddr_icn(intno);
		cpu_memget_addrp(cpu, regaddr32, &pregaddr32);
		xxICn = (uint8*)pregaddr32;
		if (data & 1 << i) {
			*xxICn |= (1 << 6);
			//printf("xxIcn(intno=%d):set\n", intno);
		}
		else {
			*xxICn &= ~(1 << 6);
			//printf("xxIcn(intno=%d):clear\n", intno);
		}
	}
	return;
}

