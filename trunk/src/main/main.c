#include "front/parser/dbg_parser.h"
#include "loader/loader.h"
#include "option/option.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpuemu_ops.h"
#include "cui/cui_ops.h"
#include "cui/stdio/cui_ops_stdio.h"
#include "cui/udp/cui_ops_udp.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include<windows.h>
#include "winsock_wrapper/winsock_wrapper.h"


static void do_cui(void)
{
	DbgCmdExecutorType *res;
	bool is_dbgmode;
	char buffer[1024];
	int len;

	while (TRUE) {
		is_dbgmode = cpuctrl_is_debug_mode();
		printf("%s", (is_dbgmode == TRUE) ? "[DBG>" : "[CPU>");
		fflush(stdout);
retry:
		len = cui_getline(buffer, 1024);
		if (len < 0) {
			cui_close();
			Sleep(1000);
			goto retry;
		}
		buffer[len] = '\0';
		res = dbg_parse((uint8*)buffer, (uint32)len);

		if (res != NULL) {
			res->run(res);
		}
	}
}


/*
 * コマンドオプション仕様
 * -i		インタラクションモード
 * 	・あり：インタラクションモード
 * 	・なし：バックグラウンド実行モード
 * -t<time>	終了時間(単位：clock)
 * 	・あり：終了時間
 * 	・なし：無制限
 * -b	入力ファイル形式
 * 	・あり：バイナリデータ
 * 	・なし：ELFファイル
 * -p<fifo config file path>
 * 	・あり：対抗ECUとの通信あり
 * 	・なし：シングルECU構成
 */
int main(int argc, const char *argv[])
{
	CmdOptionType *opt;

	winsock_init();
	//cui_ops_stdio_init();
	cui_ops_udp_init();

	opt = parse_args(argc, argv);
	if (opt == NULL) {
		return -1;
	}
	if (opt->fifocfgpath != NULL) {
		Std_ReturnType err;
		err = cpuemu_set_comm_fifocfg(opt->fifocfgpath);
		if (err != STD_E_OK) {
			return -1;
		}
	}

	if (opt->is_binary_data) {
		binary_load((uint8*)opt->load_file.buffer, 0U, opt->load_file.size);
	}
	else {
		elf_load((uint8*)opt->load_file.buffer);
		if (cpuemu_symbol_set() != STD_E_OK) {
			return -1;
		}
	}

	if (opt->is_interaction == TRUE) {
		cpuemu_init(cpuemu_thread_run);
		do_cui();
	}
	else {
		cpuemu_init(NULL);
		cpuemu_set_cpu_end_clock(opt->timeout);
		(void)cpuemu_thread_run(NULL);
	}

	return 0;
}
