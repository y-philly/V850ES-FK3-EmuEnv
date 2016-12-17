#include "cpuemu_config.h"
#include "symbol_ops.h"
#include "std_types.h"
#include <stdio.h>
#include <string.h>

static uint32 symbol_func_size = 0;
static uint32 symbol_gl_size = 0;
static DbgSymbolType symbol_func[CPUEMU_CONFIG_FUNC_SYMBOL_TABLE_NUM];
static DbgSymbolType symbol_gl[CPUEMU_CONFIG_OBJECT_SYMBOL_TABLE_NUM];


int symbol_gl_add(DbgSymbolType *sym)
{
	if (symbol_gl_size >= CPUEMU_CONFIG_OBJECT_SYMBOL_TABLE_NUM) {
		return -1;
	}
	symbol_gl[symbol_gl_size] = *sym;
	symbol_gl_size++;
	return 0;
}

int symbol_func_add(DbgSymbolType *sym)
{
	if (symbol_func_size >= CPUEMU_CONFIG_FUNC_SYMBOL_TABLE_NUM) {
		return -1;
	}
	symbol_func[symbol_func_size] = *sym;
	symbol_func_size++;
	return 0;
}


int symbol_get_func(char *funcname, uint32 func_len, uint32 *addrp, uint32 *size)
{
	int i;
	uint32 len;

	for (i = 0; i < symbol_func_size; i++) {
		len = strlen(symbol_func[i].name);

		if (func_len != len) {
			continue;
		}
		if (strncmp(funcname, symbol_func[i].name, func_len) != 0) {
			continue;
		}
		*addrp = symbol_func[i].addr;
		*size = symbol_func[i].size;
		return 0;
	}
	return -1;
}
char * symbol_pc2func(uint32 pc)
{
	int i;

	for (i = 0; i < symbol_func_size; i++) {
		if (pc < symbol_func[i].addr) {
			continue;
		}
		if (pc >= (symbol_func[i].addr + symbol_func[i].size)) {
			continue;
		}
		return  symbol_func[i].name;
	}
	return NULL;
}

int symbol_pc2funcid(uint32 pc, uint32 *funcaddr)
{
	int i;

	for (i = 0; i < symbol_func_size; i++) {
		if (pc < symbol_func[i].addr) {
			continue;
		}
		if (pc >= (symbol_func[i].addr + symbol_func[i].size)) {
			continue;
		}
		*funcaddr = symbol_func[i].addr;
		return  i;
	}
	return -1;
}
char * symbol_funcid2funcname(int id)
{
	return symbol_func[id].name;
}
uint32 symbol_uncid2funcaddr(int id)
{
	return symbol_func[id].addr;
}


int symbol_get_gl(char *gl_name, uint32 gl_len, uint32 *addrp, uint32 *size)
{
	int i;
	uint32 len;

	for (i = 0; i < symbol_gl_size; i++) {
		len = strlen(symbol_gl[i].name);

		if (gl_len != len) {
			continue;
		}
		if (strncmp(gl_name, symbol_gl[i].name, gl_len) != 0) {
			continue;
		}
		*addrp = symbol_gl[i].addr;
		*size = symbol_gl[i].size;
		return 0;
	}
	return -1;
}

void symbol_print_gl(char *gl_name, uint32 show_num)
{
	int i;
	uint32 len;
	uint32 gl_len;

	gl_len = strlen(gl_name);
	for (i = 0; i < symbol_gl_size; i++) {
		len = strlen(symbol_gl[i].name);
		if (len < gl_len) {
			continue;
		}

		if (strncmp(gl_name, symbol_gl[i].name, gl_len) != 0) {
			continue;
		}
		if (show_num > 0) {
			printf("candidate %s\n", symbol_gl[i].name);
			show_num--;
		}
		else {
			break;
		}
	}
	return;
}
