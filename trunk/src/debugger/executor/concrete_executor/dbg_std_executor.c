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
		 printf("break 0x%x\n", parsed_args->break_addr);
	 }
	 else if (parsed_args->type == DBG_CMD_BREAK_INFO) {
		 //TODO
		 printf("info break\n");
	 }
	 return;
 }

 void dbg_std_executor_delete(void *executor)
 {
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorDeleteType *parsed_args = (DbgCmdExecutorDeleteType *)(arg->parsed_args);

	 if (parsed_args->type == DBG_CMD_DELETE_ONE) {
		 //TODO
		 printf("delete %u\n", parsed_args->delete_break_no);
	 }
	 else if (parsed_args->type == DBG_CMD_DELETE_ALL) {
		 //TODO
		 printf("delete all\n");
	 }
	 return;

 }

 void dbg_std_executor_cont(void *executor)
 {
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("cont\n");
 }
