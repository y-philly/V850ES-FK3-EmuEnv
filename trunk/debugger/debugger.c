#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dbg.h"
#include "cpu.h"
#include "intc.h"
#include "device/inc/device.h"
#include "device/inc/serial.h"
#include "device/inc/can.h"
#include "device/inc/adc.h"
#include "hash.h"

#define HASH_ID_PC	0


#define SOFTWARE_BREAK_POINTS_NUM	100
typedef enum {
	DBG_MODE_CMD = 0,
	DBG_MODE_CONT
} DbgModeType;


typedef struct {
	CpuManagerType		*cpu;
	bool				view_mode;
	uint32				memory_ref_type;
	uint32				memory_ref_off;
	uint32				memory_ref_size;
	uint32 				break_cnt;
	uint32 				last_setted_breakp;
	uint32 				break_points[SOFTWARE_BREAK_POINTS_NUM];
	DbgModeType 		cmd_mode;
	DbgCmdType 			last_cmd;
	DeviceType		 	*device;
} DebugInfoType;

static DebugInfoType dbg_info;
static void del_break(DebugInfoType *dbg, uint32 break_addr);
static int set_break(DebugInfoType *dbg, uint32 break_addr);
static int set_intr(DebugInfoType *dbg, uint32 intno);
static int getline(char *line, int size);
static int getvalue16(char *str, int len, uint32 *value);
static int getvalue10(char *str, int len, uint32 *value);
static int parse_break(char *line, int len, DbgCmdType *cmd);
static int parse_print(char *line, int len, DbgCmdType *cmd);
static void debug_cpu_out(void);
static void debug_pc_out(void);
static void debug_mem_out(void);
static void debug_mem_print(void);
static void debug_stack_out(void);
static void debug_cov_out(void);
static void debug_prof_out(void);
static bool dbg_serial_recv(uint8 ch, uint8 *data);
static bool dbg_serial_send(uint8 ch, uint8 data);

static bool dbg_can_recv(uint32 *ch, uint32 *canid, uint32 *ex_canid, uint8 *data, uint8 *dlc);
static bool dbg_can_send(uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc);


static int dbg_serial_rcvin(char *str, int len, DbgCmdType *cmd);

static void dbg_do_serial_recvbuf(DbgCmdType *cmd);
static void dbg_do_can_recvbuf(DbgCmdType *cmd);

static void dbg_do_adc_cmd(DbgCmdType *cmd);
static bool dbg_adc_ada0_recv(uint8 ch, uint16 *data);
static bool dbg_adc_ada1_recv(uint8 ch, uint16 *data);
static int dbg_adc_parse(char *str, int len, DbgCmdType *cmd);

bool debug_is_view_mode(void)
{
	return dbg_info.view_mode;
}

bool is_debug_mode(void)
{
	return dbg_info.cmd_mode == DBG_MODE_CMD;
}

static DeviceAdcOpType dbg_adc_ops[2] = {
		{dbg_adc_ada0_recv},
		{dbg_adc_ada1_recv}
};

static DeviceSerialOpType dbg_serial_ops = {
		dbg_serial_recv,
		dbg_serial_send,
};
static DeviceCanOpType dbg_can_ops = {
		dbg_can_recv,
		dbg_can_send,
};


typedef struct {
	uint32 call_num;
	uint32 start_time;
	uint32 last_time;
	unsigned long long func_time;
	unsigned long long total_time;
} CpuProfileType;

static CpuProfileType *CpuProfile;

void debug_init(CpuManagerType *cpu, DeviceType *device)
{
	dbg_info.cpu = cpu;
	dbg_info.break_cnt = 1;
	dbg_info.cmd_mode = DBG_MODE_CMD;
	dbg_info.view_mode = TRUE;
	dbg_info.device = device;

	device_serial_register_ops(device->dev.serial, UDnCH0, &dbg_serial_ops);
	device_adc_register_ops(device->dev.adc, dbg_adc_ops);
	device_can_register_ops(device->dev.can, &dbg_can_ops);

	hash_init();
	CpuProfile = malloc(symbol_func_size * sizeof(CpuProfileType));
	memset(CpuProfile, 0, symbol_func_size * sizeof(CpuProfileType));
	return;
}


void set_force_break(void)
{
	dbg_info.cmd_mode = DBG_MODE_CMD;
	return;
}

int debugger_is_break(void)
{
	DebugInfoType *dbg = &dbg_info;
	int i;
	for (i = 0; i < dbg->break_cnt; i++) {
		//printf("break_points[%d]=0x%x\n", i, dbg->break_points[i]);
		//fflush(stdout);
		if (dbg->cpu->cpu.pc == dbg->break_points[i]) {
			dbg->cmd_mode = DBG_MODE_CMD;
			return TRUE;
		}
	}
	return FALSE;
}
static void del_break(DebugInfoType *dbg, uint32 break_addr)
{
	int i;
	for (i = 0; i < dbg->break_cnt; i++) {
		//printf("break_points[%d]=0x%x\n", i, dbg->break_points[i]);
		//fflush(stdout);
		if (break_addr == dbg->break_points[i]) {
			dbg->break_points[i] = 0;
			dbg->cmd_mode = DBG_MODE_CMD;
		}
	}
	return;
}
static int set_break(DebugInfoType *dbg, uint32 break_addr)
{
	if (dbg->last_setted_breakp == (SOFTWARE_BREAK_POINTS_NUM -1)) {
		/*
		 * 0�Ԗڂ͏��������ł��Ȃ����̂Ƃ��Ĉ���(���Z�b�g�����݂͏�Ƀu���[�N�Ώ�)
		 */
		dbg->last_setted_breakp = 1;
	}
	else {
		dbg->last_setted_breakp = dbg->break_cnt;
		dbg->break_cnt++;
	}

	dbg->break_points[dbg->last_setted_breakp] = break_addr;
	//printf("break_points[%d]=0x%x\n", dbg->last_setted_breakp, dbg->break_points[dbg->last_setted_breakp]);
	//fflush(stdout);
	return 0;
}
static int set_intr(DebugInfoType *dbg, uint32 intno)
{
	printf("INT:%d\n", intno);
	return intc_raise_intr(dbg->cpu, intno);
}


/*
 * �P�s�擾����D
 * ���s�R�[�h�͊܂܂Ȃ��D
 */
static int getline(char *line, int size)
{
	int n = 0;
	char c;
	while (TRUE) {
		c = fgetc(stdin);
		if (c < 0 || c == '\n') {
			break;
		}
		line[n] = c;
		n++;
	}
	return n;
}


static int getvalue16(char *str, int len, uint32 *value)
{
	char *p;
	int p_len = len -1;
	long long ret;

	if (p_len == 0) {
		//�P�����݂̂Ȃ�΃X�L�b�v
		return -1;
	}
	//2�����ڍs�𐔒l�Ƃ݂Ȃ��D
	p = &str[1];
	p[p_len] = '\0';

	errno = 0;
	ret = strtoull(p, NULL, 16);
	*value = (uint32)ret;
	//printf("%s errno=%d addr=0x%x\n", str, errno, *value);
	if (errno != 0) {
		return -1;
	}
	return 0;
}

static int getvalue10(char *str, int len, uint32 *value)
{
	char *p;
	int p_len = len -1;
	long ret;

	if (p_len == 0) {
		//�P�����݂̂Ȃ�΃X�L�b�v
		return -1;
	}
	//2�����ڍs�𐔒l�Ƃ݂Ȃ��D
	p = &str[1];
	p[p_len] = '\0';

	errno = 0;
	ret = strtol(p, NULL, 10);
	*value = ret;
	//printf("errno=%d addr=%d\n", errno, *value);
	if (errno != 0) {
		return -1;
	}
	return 0;
}


static int parse_break(char *line, int len, DbgCmdType *cmd)
{
	int res;
	uint32 addr;
	uint32 size;

	if (len == 0) {
		return -1;
	}
	res = symbol_get_func(&line[1], len -1, &addr, &size);
	if (res == 0) {
		cmd->id = DBG_CMD_ID_BREAK;
		cmd->addr = addr;
		return 0;
	}

	res = getvalue16(line, len, &addr);
	if (res == 0) {
		cmd->id = DBG_CMD_ID_BREAK;
		cmd->addr = addr;
		return 0;
	}

	return -1;
}

static int parse_print(char *line, int len, DbgCmdType *cmd)
{
	int res;
	uint32 addr;
	uint32 size;

	cmd->id = DBG_CMD_ID_PMEM;
	res = symbol_get_gl(&line[1], len -1, &addr, &size);
	if (res == 0) {
		dbg_info.memory_ref_off = addr;
		dbg_info.memory_ref_size = size;
		dbg_info.memory_ref_type = size;
		printf("o=0x%x, s=0x%d\n", addr, size);
	}
	return 0;
}

/*
 * b <�A�h���X�Ԓn(hex)>�F�u���[�N�ݒ肷��D
 * b <�A�h���X�Ԓn(hex)>�F�u���[�N��������D
 * i <�����ݔԍ�>		�F�����݃g���K����D
 * s <�T�C�Y>			�F�������Q�ƃT�C�Y
 * o <�I�t�Z�b�g>		�F�I�t�Z�b�g
 * v				�F���s�������߃R�[�h��\������(�g�O���ݒ�Ƃ���)
 * n				�F�X�e�b�v���s����D
 * c				�F���̖��߈ڍs��A�����s����D
 *
 */
void debugger_getcmd(DbgCmdType *cmd)
{
	char line[1024];
	int n;
	int res;
	uint32 addr;
	uint32 intno;

	cmd->id = DBG_CMD_ID_UNKNOWN;
	n = getline(line, 1024);
	//printf("debugger_get_cmd:\n");
	//fflush(stdout);
	if (n == 0) {
		*cmd = dbg_info.last_cmd;
		return;
	}
	cmd->cmd_char = line[0];
	switch (cmd->cmd_char) {
	case 'C':
		cmd->id = DBG_CMD_ID_CAN_RCVIN;
		break;
	case 'S':
		res = dbg_serial_rcvin(&line[1], n-1, cmd);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_SERIAL_IN;
		}
		break;
	case 'A':
		res = dbg_adc_parse(&line[1], n-1, cmd);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_ADC;
		}
		break;
	case 'p':
		parse_print(&line[1], n-1, cmd);
		break;
	case 't':
		res = getvalue10(&line[1], n-1, &cmd->size);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_PMEMTYPE;
		}
		break;
	case 'b':
		parse_break(&line[1], n -1, cmd);
		break;
	case 'd':
		res = getvalue16(&line[1], n-1, &addr);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_DELETE_BREAK;
			cmd->addr = addr;
		}
		break;
	case 'o':
		res = getvalue16(&line[1], n-1, &cmd->off);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_MEMORY_REF_OFF;
		}
		break;
	case 's':
		res = getvalue10(&line[1], n-1, &cmd->size);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_MEMORY_REF_SIZE;
		}
		break;
	case 'i':
		res = getvalue10(&line[1], n-1, &intno);
		if (res == 0) {
			cmd->id = DBG_CMD_ID_INTR;
			cmd->intno = intno;
		}
		break;
	case 'c':
		cmd->id = DBG_CMD_ID_CONT;
		break;
	case 'v':
		cmd->id = DBG_CMD_ID_VIEW;
		break;
	case 'n':
		cmd->id = DBG_CMD_ID_NEXT;
		break;
	default:
		break;
	}
	return;
}


static void debug_cpu_out(void)
{
	/*
	 * show cpu
	 */
	int fd;
	char buffer[1024];
	int i;
	fd = open(".\\cpuinfo.txt", O_CREAT | O_TRUNC |O_WRONLY|O_BINARY, 00777);
	sprintf(buffer, "PC		0x%x\n", dbg_info.cpu->cpu.pc);
	write(fd, buffer, strlen(buffer));
	for (i = 0; i < 32; i++) {
		sprintf(buffer, "R%d		0x%x\n", i, dbg_info.cpu->cpu.r[i]);
		write(fd, buffer, strlen(buffer));
	}
	sprintf(buffer, "EIPC	0x%x\n", dbg_info.cpu->cpu.eipc);
	write(fd, buffer, strlen(buffer));
	sprintf(buffer, "EIPSW	0x%x\n", dbg_info.cpu->cpu.eipsw);
	write(fd, buffer, strlen(buffer));
	sprintf(buffer, "ECR		0x%x\n", dbg_info.cpu->cpu.ecr);
	write(fd, buffer, strlen(buffer));
	sprintf(buffer, "PSW		0x%x\n", dbg_info.cpu->cpu.psw);
	write(fd, buffer, strlen(buffer));
	sprintf(buffer, "FEPC	0x%x\n", dbg_info.cpu->cpu.fepc);
	write(fd, buffer, strlen(buffer));
	sprintf(buffer, "FEPSW	0x%x\n", dbg_info.cpu->cpu.fepsw);
	write(fd, buffer, strlen(buffer));
	sprintf(buffer, "CTBP	0x%x\n", dbg_info.cpu->cpu.ctbp);
	write(fd, buffer, strlen(buffer));

	close(fd);
	return;
}

static void debug_pc_out(void)
{
	fprintf(stderr, "%x\n", dbg_info.cpu->cpu.pc);
	fflush(stderr);
	return;
}

static void debug_mem_out(void)
{
	/*
	 * show mem
	 */
	int fd;
	uint32 *regaddr = NULL;
	char *buffer;

	if (dbg_info.memory_ref_size == 0) {
		return;
	}
	cpu_memget_addrp(&CpuManager, dbg_info.memory_ref_off, &regaddr);
	if (regaddr == NULL) {
		printf("debug_mem_out:not found memory(off=0x%x)\n", dbg_info.memory_ref_off);
		return;
	}

	fd = open(".\\meminfo.bin", O_CREAT | O_TRUNC |O_WRONLY|O_BINARY, 00777);
	buffer = (char*)regaddr;

	write(fd, buffer, dbg_info.memory_ref_size);

	close(fd);

	return;
}

static void debug_mem_print(void)
{
	int i;
	/*
	 * show mem
	 */
	uint32 *regaddr = NULL;
	uint16 *regaddr16 = NULL;
	uint8 *regaddr8 = NULL;
	uint32 reftype;

	if (dbg_info.memory_ref_size == 0) {
		return;
	}
	cpu_memget_addrp(&CpuManager, dbg_info.memory_ref_off, &regaddr);
	if (regaddr == NULL) {
		printf("debug_mem_out:not found memory(off=0x%x)\n", dbg_info.memory_ref_off);
		return;
	}
	switch (dbg_info.memory_ref_type) {
	case 1:
		reftype = 1;
		break;
	case 2:
		reftype = 2;
		break;
	case 4:
		reftype = 4;
		break;
	default:
		reftype = 1;
		break;
	}

	for (i = 0; i < dbg_info.memory_ref_size/reftype; i++) {
		if (reftype == 2) {
			regaddr16 = ((uint16*)regaddr) + i;
			printf("0x%x: 0x%02x\n", dbg_info.memory_ref_off + (i * 2) + 0, regaddr16[0]);
		}
		else if (reftype == 4) {
			printf("0x%x: 0x%04x\n", dbg_info.memory_ref_off + (i * 4), *(regaddr + i));
		}
		else {
			regaddr8 = (uint8*)regaddr;
			printf("0x%x: 0x%01x\n", dbg_info.memory_ref_off + (i * 1) + 0, *(regaddr8 + i));
		}
	}
	fflush(stdout);
	return;
}

static void debug_stack_out(void)
{
	/*
	 * show stack
	 */
	//uint32 stack_addr = CpuManager.cpu.r[3];
	uint32 stack_addr = 0x03ff8a58;//TODO ���񏑂�������͖̂ʓ|�Ȃ̂ŁC�΍􂪕K�v�D
	uint32 start_addr;
	uint32 *regaddr;
	uint32 size = 512;
	char buffer[1024];
	int fd;
	int i;

	if (stack_addr == 0) {
		return;
	}

	cpu_memget_addrp(&CpuManager, stack_addr, &regaddr);
	if (regaddr == NULL) {
		return;
	}

	fd = open(".\\stack.txt", O_CREAT | O_TRUNC |O_WRONLY|O_BINARY, 00777);

	//sprintf(buffer, "STACK_ADDR		0x%x\n", stack_addr);
	//write(fd, buffer, strlen(buffer));

	/*
	 * �X�^�b�N�̓A�h���X�Ԓn�������������ɉ��т邽��
	 * �����������珇�Ԃɕ\������D
	 */
	start_addr = (uint32)regaddr;
	regaddr = (uint32*)start_addr;
	for (i = 0; i <= size/4; i++) {
		sprintf(buffer, "0x%x		0x%x\n", stack_addr - (i*4), *regaddr);
		regaddr--;
		write(fd, buffer, strlen(buffer));
	}
	close(fd);

	return;
}
void debug_cov_out(void)
{
	int fd;
	HashValueType *value;
	char buffer[1024];
	fd = open(".\\cov.csv", O_CREAT | O_TRUNC |O_WRONLY|O_BINARY, 00777);

	value = hash_first(HASH_ID_PC);
	while (value != NULL) {
		//printf("%u,\n", value->rawdata);
		sprintf(buffer, "0x%x,\n", value->rawdata);
		write(fd, buffer, strlen(buffer));
		value = hash_next(HASH_ID_PC);
	}
	//fflush(stdout);
	close(fd);
	return;
}
void debug_prof_out(void)
{
	int fd;
	int i;
	char buffer[1024];
	fd = open(".\\prof.csv", O_CREAT | O_TRUNC |O_WRONLY|O_BINARY, 00777);

	sprintf(buffer, "func, call_num, func_clocks, total_clocks,\n");
	write(fd, buffer, strlen(buffer));
	for (i = 0; i < symbol_func_size; i++) {

		if (CpuProfile[i].call_num == 1) {
			CpuProfile[i].total_time =
					(CpuProfile[i].last_time - CpuProfile[i].start_time);
		}

		sprintf(buffer, "%s, %u, %I64u, %I64u,\n",
				symbol_funcid2funcname(i),
				CpuProfile[i].call_num,
				CpuProfile[i].func_time,
				CpuProfile[i].total_time);
		write(fd, buffer, strlen(buffer));
	}

	close(fd);
	return;
}

int debugger_docmd(DbgCmdType *cmd)
{
	int need_recv_cmd = FALSE;
	switch (cmd->id) {
	case DBG_CMD_ID_NEXT:
		dbg_info.cmd_mode = DBG_MODE_CMD;
		break;
	case DBG_CMD_ID_CONT:
		dbg_info.cmd_mode = DBG_MODE_CONT;
		//printf("$Enter CPU MODE\n");
		fflush(stdout);
		break;
	case DBG_CMD_ID_INTR:
		if (cmd->intno != -1) {
			set_intr(&dbg_info, cmd->intno);
		}
		else {
			printf("NMI\n");
			intc_raise_nmi(dbg_info.cpu, INTC_NMINO_NMI);
		}
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_BREAK:
		set_break(&dbg_info, cmd->addr);
		printf("setted break point:0x%x\n", cmd->addr);
		fflush(stdout);
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_DELETE_BREAK:
		del_break(&dbg_info, cmd->addr);
		printf("deleted break point:0x%x\n", cmd->addr);
		fflush(stdout);
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_MEMORY_REF_OFF:
		dbg_info.memory_ref_off = cmd->off;
		printf("off=0x%x size=%d\n", dbg_info.memory_ref_off, dbg_info.memory_ref_size);
		fflush(stdout);
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_MEMORY_REF_SIZE:
		dbg_info.memory_ref_size = cmd->size;
		dbg_info.memory_ref_type = cmd->size;
		printf("off=0x%x size=%d\n", dbg_info.memory_ref_off, dbg_info.memory_ref_size);
		fflush(stdout);
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_PMEM:
		debug_mem_print();
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_PMEMTYPE:
		dbg_info.memory_ref_type = cmd->size;
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_VIEW:
		if (dbg_info.view_mode == FALSE) {
			printf("view mode=ON\n");
			dbg_info.view_mode = TRUE;
		}
		else {
			printf("view mode=OFF\n");
			dbg_info.view_mode = FALSE;
		}
		fflush(stdout);
		need_recv_cmd = TRUE;
		break;
	case DBG_CMD_ID_SERIAL_IN:
		need_recv_cmd = TRUE;
		dbg_do_serial_recvbuf(cmd);
		break;
	case DBG_CMD_ID_CAN_RCVIN:
		need_recv_cmd = TRUE;
		dbg_do_can_recvbuf(cmd);
		break;
	case DBG_CMD_ID_ADC:
		need_recv_cmd = TRUE;
		dbg_do_adc_cmd(cmd);
		break;
	case DBG_CMD_ID_UNKNOWN:
	default:
		printf("Unknown command:%c\n", cmd->cmd_char);
		fflush(stdout);
		need_recv_cmd = TRUE;
		break;
	}
	dbg_info.last_cmd = *cmd;
	return need_recv_cmd;
}
/*
 * Can
 */

/*
 * Serial
 */
static uint32 serial_rcv_len = 0;
static uint32 serial_rcv_off = 0;
static char serial_rcv_buffer[1024];

static int dbg_serial_rcvin(char *str, int len, DbgCmdType *cmd)
{
	int i;
	char *p;
	int p_len = len -1;

	if (p_len == 0) {
		//�P�����݂̂Ȃ�΃X�L�b�v
		return -1;
	}
	//2�����ڍs���V���A�����͕�����Ƃ݂Ȃ��D
	p = &str[1];
	p[p_len] = '\0';

	cmd->serial_buflen = 0;
	for (i = 0; i < p_len + 1; i++) {
		cmd->serial_buffer[i] = p [i];
		cmd->serial_buflen++;
	}

	return 0;
}
/*
 * A 0:-1:256 0�R���g���[���̃`�����l���S��
 * A 0:1:256�@�@0�R���g���[���̂P�`���l��
 */
static int dbg_adc_parse(char *str, int len, DbgCmdType *cmd)
{
	int i;
	char *p;
	int p_len = len -1;
	char *p_cntl;
	int len_cntl;
	char *p_ch;
	int len_ch;
	char *p_val;
	int len_val;
	int colon_count = 0;
	int res;

	if (p_len == 0) {
		//�P�����݂̂Ȃ�΃X�L�b�v
		return -1;
	}
	//2�����ڍs���V���A�����͕�����Ƃ݂Ȃ��D
	p = &str[1];
	p[p_len] = '\0';

	/*
	 * ":" ��T���C"\0"�ɕύX����
	 */
	len_cntl = -1;
	len_ch = -1;
	len_val = 0;
	p_cntl = p;
	for (i = 0; i < p_len; i++) {
		if (len_ch != -1) {
			len_val++;
		}
		else if (p[i] == ':') {
			p[i] = '\0';
			if (colon_count == 0) {
				len_cntl = i;
				p_ch = &p[i+1];
				colon_count++;
			}
			else if (colon_count == 1) {
				len_ch = i;
				p_val = &p[i+1];
				colon_count++;
			}
		}
	}
	if (len_cntl == -1 || len_ch == -1 || len_val == 0) {
		return -1;
	}

	res = getvalue10(--p_cntl, len_cntl + 1, (uint32*)&cmd->adc_cntl);
	//printf("p_ch=%c len_ch=%d res=%d\n", *p_ch, len_ch, res);
	fflush(stdout);
	if (res < 0) {
		return -1;
	}
	res = getvalue10(--p_ch, len_ch + 1, (uint32*)&cmd->adc_ch);
	//printf("p_ch=%c len_ch=%d res=%d\n", *p_ch, len_ch, res);
	fflush(stdout);
	if (res < 0) {
		return -1;
	}
	res = getvalue10(--p_val, len_val + 1, &cmd->adc_data);
	//printf("res=%d\n", res);
	fflush(stdout);
	if (res < 0) {
		return -1;
	}

	return 0;
}
void dbg_do_serial_recvbuf(DbgCmdType *cmd)
{
	int i;
	serial_rcv_len = 0;
	serial_rcv_off = 0;
	printf("SERIAL_ACCEPT:");
	for (i = 0; i < cmd->serial_buflen; i++) {
		serial_rcv_buffer[i] = cmd->serial_buffer[i];
		serial_rcv_len++;
		printf("%c", serial_rcv_buffer[i]);
	}
	printf("\n");
	fflush(stdout);
	return;
}

static bool dbg_serial_recv(uint8 ch, uint8 *data)
{
	if (serial_rcv_len > 0) {
		*data = serial_rcv_buffer[serial_rcv_off];
		//printf("dbg_serial_recv:%c\n", *data);
		serial_rcv_len--;
		serial_rcv_off++;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

static bool dbg_serial_send(uint8 ch, uint8 data)
{
#if 0
	int fd;
	int err;
	fd = open(".\\serial_out.txt", O_APPEND |O_WRONLY|O_BINARY);
	if (fd < 0) {
		printf("dbg_serial_send:open err=%d\n", errno);
		fflush(stdout);
		return FALSE;
	}
	err = write(fd, &data, 1);
	if (err <= 0) {
		printf("dbg_serial_send:write err=%d\n", errno);
		fflush(stdout);
		return FALSE;
	}
	close(fd);
	//printf("dbg_serial_send:write OK=%c\n", data);
	fflush(stdout);
#endif
	return TRUE;
}

typedef struct {
	bool is_rcv;
	uint32 ch;
	uint32 canid;
	uint32 ex_canid;
	uint8 data[8U];
	uint8 dlc;
} DbgCmdCanRcvDataType;

static DbgCmdCanRcvDataType dbg_can_rcvinfo;
void dbg_do_can_recvbuf(DbgCmdType *cmd)
{
	int fd;
	int err;
	char data[1024];
	DbgCmdCanRcvDataType *can_data;

	fd = open(".\\can.txt", O_RDONLY|O_BINARY);
	if (fd < 0) {
		printf("dbg_do_can_recvbuf:open err=%d\n", errno);
		fflush(stdout);
		return;
	}
	err = read(fd, &data, 1024);
	if (err <= 0) {
		printf("dbg_do_can_recvbuf:read err=%d\n", errno);
		fflush(stdout);
		return;
	}
	can_data = (DbgCmdCanRcvDataType *)data;

	dbg_can_rcvinfo = *can_data;
	close(fd);
	return;
}

bool dbg_can_recv(uint32 *ch, uint32 *canid, uint32 *ex_canid, uint8 *data, uint8 *dlc)
{
	int i;
	if (dbg_can_rcvinfo.is_rcv) {
		*ch = dbg_can_rcvinfo.ch;
		*canid = dbg_can_rcvinfo.canid;
		*ex_canid = dbg_can_rcvinfo.ex_canid;
		*dlc = dbg_can_rcvinfo.dlc;
		dbg_can_rcvinfo.is_rcv = FALSE;

		printf("\n");
		printf("##RCV_CAN::ch=%u\n", *ch);
		printf("##RCV_CNA:canid=0x%x\n", *canid);
		printf("##RCV_CAN:ex_canid=0x%x\n", *ex_canid);
		printf("##RCV_CNA:dlc=%u\n", *dlc);
		printf("##");
		for (i = 0; i < *dlc; i++) {
			data[i] = dbg_can_rcvinfo.data[i];
			printf("0x%02x ", data[i]);
		}
		printf("\n");
		return TRUE;
	}
	return FALSE;
}
bool dbg_can_send(uint32 ch, uint32 msg_id, uint8 *data, uint8 dlc)
{
	uint8 i;
	printf("\n");
	printf("#########################CAN DEBUG INFO########################\n");
	printf("##ch=%u\n", ch);
	printf("##msg_id=%u\n", msg_id);
	printf("##dlc=%u\n", dlc);
	printf("##");
	for (i = 0U; i < dlc; i++) {
		printf("%u:0x%02x ", i, data[i]);
	}
	printf("\n");
	printf("#########################CAN DEBUG INFO########################\n");
	return TRUE;
}

/*
 * ADC
 */
static uint16 adc_ada0_data[9];
static uint16 adc_ada1_data[10];
static void do_adc_cmd(uint8 ch, uint8 ch_num, uint16 *data_buf, uint16 data)
{
	int i;
	if (ch == -1) {
		for (i = 0; i < ch_num; i++) {
			data_buf[i] = data;
		}
	}
	else if (ch < ch_num) {
		data_buf[ch] = data;
		//printf("SET:ch=%u adc_data=%u\n", cmd->adc_ch, adc_data[cmd->adc_ch]);
		fflush(stdout);
	}

	return;
}

static void dbg_do_adc_cmd(DbgCmdType *cmd)
{

	if (cmd->adc_cntl == 0) {
		do_adc_cmd(cmd->adc_ch, 9, adc_ada0_data, (uint16)cmd->adc_data);
	}
	else if (cmd->adc_cntl == 1) {
		do_adc_cmd(cmd->adc_ch, 10, adc_ada1_data, (uint16)cmd->adc_data);
	}

	return;
}

static bool dbg_adc_ada0_recv(uint8 ch, uint16 *data)
{
		if (ch < 9) {
			*data = adc_ada0_data[ch];
			//printf("REF:ch=%u adc_data=%u\n", ch, adc_data[ch]);
			fflush(stdout);
		}
	return TRUE;
}

static bool dbg_adc_ada1_recv(uint8 ch, uint16 *data)
{
		if (ch < 10) {
			*data = adc_ada1_data[ch];
			//printf("REF:ch=%u adc_data=%u\n", ch, adc_data[ch]);
			fflush(stdout);
		}
	return TRUE;
}

void debugger_callback(void)
{
	debug_pc_out();
	debug_cpu_out();
	debug_mem_out();
	debug_stack_out();
	debug_cov_out();
	debug_prof_out();
	return;
}

DbgExecOpBufferType DbgExecOpBuffer;

void debugger_exec_op_bufinit(char *filepath)
{

	int fd;
	fd = open(filepath, O_CREAT | O_TRUNC |O_WRONLY | O_BINARY, 0777);
	if (fd < 0) {
		printf("debugger_exec_op_bufinit:open err=%d\n", errno);
		fflush(stdout);
		exit(1);
	}
	DbgExecOpBuffer.fd = fd;
	DbgExecOpBuffer.filepath = filepath;
	DbgExecOpBuffer.count = 0;
	return;
}
void debugger_exec_op_bufsync(void)
{
	int i;
	int err;

	if (dbg_info.cmd_mode != DBG_MODE_CMD) {
		if (DbgExecOpBuffer.count < DBG_BUFP_MAX_CNT) {
			return;
		}
	}
	for (i = 0; i < DbgExecOpBuffer.count; i++) {
		err = write(DbgExecOpBuffer.fd, DbgExecOpBuffer.buf[i].p, DbgExecOpBuffer.buf[i].write_len);
		if (err <= 0) {
			printf("debugger_exec_op_bufsync:write err=%d\n", errno);
			fflush(stdout);
			exit(1);
		}
	}
	err = close(DbgExecOpBuffer.fd);
	if (err < 0) {
		printf("debugger_exec_op_bufsync:close err=%d\n", errno);
		fflush(stdout);
		exit(1);
	}
	int fd;
	fd = open(DbgExecOpBuffer.filepath, O_APPEND |O_WRONLY | O_BINARY);
	if (fd < 0) {
		printf("debugger_exec_op_bufsync:open err=%d\n", errno);
		fflush(stdout);
		exit(1);
	}
	DbgExecOpBuffer.fd = fd;
	DbgExecOpBuffer.count = 0;
	return;
}


void debugger_save(void)
{
	HashValueType value;
	uint32 funcid;
	uint32 funcaddr;

	value.rawdata = dbg_info.cpu->cpu.pc;
	hash_add(HASH_ID_PC, &value);

	funcid = symbol_pc2funcid(value.rawdata, &funcaddr);
	if (funcid == -1) {
		return;
	}
	if (funcaddr == value.rawdata) {
		CpuProfile[funcid].call_num++;
		if (CpuProfile[funcid].last_time > 0) {
			CpuProfile[funcid].total_time +=
					(CpuProfile[funcid].last_time - CpuProfile[funcid].start_time);
		}
		CpuProfile[funcid].start_time = dbg_info.device->clock;
	}
	CpuProfile[funcid].func_time++;
	CpuProfile[funcid].last_time = dbg_info.device->clock;

	return;
}

