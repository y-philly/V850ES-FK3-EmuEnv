#include "cpu.h"
#include "op_exec_ops.h"

/*
 * Format1
 */
int op_exec_jmp(CpuManagerType *cpu)
{
	uint32 reg1 = cpu->decoded_code.type1.reg1;
	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JMP r%d(0x%x)\n", cpu->cpu.pc, reg1, cpu->cpu.r[reg1]));
	cpu->cpu.pc = cpu->cpu.r[reg1];
	return 0;
}


/*
 * Format3
 */
int op_exec_bcond(CpuManagerType *cpu)
{
	uint16 cond = cpu->decoded_code.type3.cond;
	uint16 is_br = FALSE;
	uint16 flg_s = CPU_ISSET_S(&cpu->cpu);
	uint16 flg_ov = CPU_ISSET_OV(&cpu->cpu);
	uint16 flg_z = CPU_ISSET_Z(&cpu->cpu);
	uint16 flg_cy = CPU_ISSET_CY(&cpu->cpu);
	uint16 flg_sat = CPU_ISSET_SAT(&cpu->cpu);
	uint32 disp_u;
	sint32 disp;

	disp_u = cpu->decoded_code.type3.disp << 1;
	disp = op_sign_extend(8, disp_u);//1�r�b�g�V�t�g�����̂ŁC7��8�ƂȂ����D

	switch (cond) {
	/*
	 * �����t������
	 */
	case 0b1110:	//BGE
		if ((flg_s ^ flg_ov) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b1111:	//BGT
		if (((flg_s ^ flg_ov) | flg_z ) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b0111:	//BLE
		if (((flg_s ^ flg_ov) | flg_z ) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b0110:	//BLT
		if ((flg_s ^ flg_ov) == TRUE) {
			is_br = TRUE;
		}
		break;

	/*
	 * ���������Ȃ�����
	 */
	case 0b1011:	//BH
		if ((flg_cy | flg_z) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b0001:	//BL
/*	case 0b0001: */	//BC
		if ((flg_cy) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b0011:	//BNH
		if ((flg_cy | flg_z) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b1001:	//BNL
/*	case 0b1001: */	//BNC
		if ((flg_cy) == FALSE) {
			is_br = TRUE;
		}
		break;

	/*
	 * ����
	 */
	case 0b0010:	//BE
/*	case 0b0010: */	//BZ
		if ((flg_z) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b1010:	//BNZ
		if ((flg_z) == FALSE) {
			is_br = TRUE;
		}
		break;

	/*
	 * ���̑�
	 */
	case 0b0100:	//BN
		if ((flg_s) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b1000:	//BNV
		if ((flg_ov) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b1100:	//BP
		if ((flg_s) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b0101:	//BR
		is_br = TRUE;
		break;
	case 0b1101:	//BSA
		if ((flg_sat) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b0000:	//BV
		if ((flg_ov) == TRUE) {
			is_br = TRUE;
		}
		break;
	default:
		break;
	}
	if (is_br == TRUE) {
		sint32 pc = cpu->cpu.pc;
		pc = pc + disp;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: B cond(0x%x):0x%x\n", cpu->cpu.pc, cond, pc));
		cpu->cpu.pc = pc;
	}
	else {
		sint32 pc = cpu->cpu.pc + 2;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: Bcond(0x%x):0x%x\n", cpu->cpu.pc, cond, pc));
		cpu->cpu.pc = pc;
	}
	return 0;
}

/*
 * Format5
 */
int op_exec_jr(CpuManagerType *cpu)
{
	uint32 reg2 = cpu->decoded_code.type5.reg2;
	sint32 pc = (sint32)cpu->cpu.pc;
	sint32 disp;

	if (reg2 > 0) {
		cpu->cpu.r[reg2] = cpu->cpu.pc + 4;
	}
	disp = op_sign_extend(21, cpu->decoded_code.type5.disp);
	pc += disp;

	if (reg2 == 0) {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JR disp22(%d):0x%x\n", cpu->cpu.pc, disp, pc));
	}
	else {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JARL disp22(%d):0x%x r%u(0x%x)\n", cpu->cpu.pc, disp, pc, reg2, cpu->cpu.r[reg2]));
	}

	cpu->cpu.pc = pc;
	return 0;
}
