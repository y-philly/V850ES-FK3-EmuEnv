#ifndef _OPTION_H_
#define _OPTION_H_

#include "std_types.h"

typedef struct {
	bool	is_interaction;
	uint64	timeout;
	bool	is_binary_data;
	char	*fifocfgpath;
	char	*filepath;
	char	buffer_fifopath[4096];
	char	buffer_filepath[4096];
	uint32	filedata_len;
	char	filedata[1024*1024*10];
} CmdOptionType;

extern CmdOptionType *parse_args(int argc, const char* argv[]);


#endif /* _OPTION_H_ */
