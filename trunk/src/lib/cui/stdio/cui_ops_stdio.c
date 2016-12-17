#include "cui/stdio/cui_ops_stdio.h"
#include "cui/cui_ops.h"

static FileOpType cui_fileop_stdio = {
	.read_fd  = 0,
	.write_fd = 1,
};

void cui_ops_stdio_init(void)
{
	cui_fileop_register(&cui_fileop_stdio);
	return;
}
