#include "front/parser/dbg_parser_config.h"
#include "front/parser/concrete_parser/dbg_std_parser.h"

DbgCmdParserTableType dbg_cmd_parser_std_table[DBG_CMD_STD_ID_NUM] = {
		{ dbg_parse_break, },
};
