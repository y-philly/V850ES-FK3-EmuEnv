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
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("cont\n");
}
void dbg_std_executor_next(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("next\n");
}
void dbg_std_executor_return(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("return\n");
}
void dbg_std_executor_quit(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("quit\n");
	 exit(1);
}

void dbg_std_executor_elaps(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("elaps\n");
}
void dbg_std_executor_view(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 printf("view\n");
}
void dbg_std_executor_print(void *executor)
{
	 //TODO
	 DbgCmdExecutorType *arg = (DbgCmdExecutorType *)executor;
	 DbgCmdExecutorPrintType *parsed_args = (DbgCmdExecutorPrintType *)(arg->parsed_args);

	 if (parsed_args->type == DBG_CMD_PRINT_SYMBOL) {
		 printf("ERROR: not supported:print symbol(%s)\n", parsed_args->symbol.str);
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR) {
		 printf("ERROR: not supported:print addr(0x%x)\n", parsed_args->addr);
	 }
	 else if (parsed_args->type == DBG_CMD_PRINT_ADDR_SIZE) {
		 printf("print 0x%x %u\n", parsed_args->addr, parsed_args->size);
	 }
	 return;
}
