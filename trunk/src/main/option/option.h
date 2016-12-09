#ifndef _OPTION_H_
#define _OPTION_H_

#include "std_types.h"

typedef struct {
	bool	is_interaction;
	uint64	timeout;
	bool	is_binary_data;
	char	*fifopath;
	char	*filepath;
	char	buffer_fifopath[4096];
	char	buffer_filepath[4096];
} CmdOptionType;

extern CmdOptionType *parse_args(int argc, const char* argv[]);


#endif /* _OPTION_H_ */
