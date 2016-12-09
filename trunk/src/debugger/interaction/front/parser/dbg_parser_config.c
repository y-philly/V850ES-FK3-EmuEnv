#include "front/parser/dbg_parser_config.h"
#include "front/parser/concrete_parser/dbg_std_parser.h"

DbgCmdParserTableType dbg_cmd_parser_std_table[DBG_CMD_STD_ID_NUM] = {
		{ dbg_parse_break, },
		{ dbg_parse_delete, },
		{ dbg_parse_cont, },
		{ dbg_parse_elaps, },
		{ dbg_parse_next, },
		{ dbg_parse_return, },
		{ dbg_parse_view, },
		{ dbg_parse_print, },
		{ dbg_parse_quit, },
};
