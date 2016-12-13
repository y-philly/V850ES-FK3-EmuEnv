#include "front/parser/dbg_parser.h"
#include "front/parser/lib/dbg_parser_lib.h"
#include "loader/loader.h"
#include "option/option.h"
#include "cpu_control/dbg_cpu_control.h"
#include "cpu_control/dbg_cpu_thread_control.h"
#include "cpuemu_ops.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

static int cui_getline(char *line, int size)
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
		len = cui_getline(buffer, 1024);
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
 * -p<fifofile path>	対抗ECU通信経路指定
 * 	・あり：対抗ECUとの通信あり(FIFO)
 * 	・なし：シングルECU構成
 */
int main(int argc, const char *argv[])
{
	CmdOptionType *opt;

	opt = parse_args(argc, argv);

	if (opt == NULL) {
		return -1;
	}

	if (opt->is_binary_data) {
		binary_load((uint8*)opt->filedata, 0U, opt->filedata_len);
	}
	else {
		elf_load((uint8*)opt->filedata);
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
