#include "dbg_executor.h"
#include "concrete_executor/dbg_std_executor.h"
#include "front/parser/concrete_parser/dbg_std_parser.h"
#include <stdio.h>

 void dbg_std_executor_parse_error(void *executor)
 {
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;

	 printf("ERROR: %s :command not found\n", arg->original_str);
	 return;
 }

 void dbg_std_executor_break(void *executor)
 {
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorBreakType *parsed_args = (DbgCmdExecutorBreakType *)(arg->parsed_args);

	 if (parsed_args->type == DBG_CMD_BBREAK_SET) {
		 //TODO
		 printf("break command:0x%x\n", parsed_args->break_addr);
	 }
	 else if (parsed_args->type == DBG_CMD_BREAK_INFO) {
		 //TODO
		 printf("break command:info break\n");
	 }

 }
