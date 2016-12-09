#include "option/option.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
static CmdOptionType cmd_option;

static int cmd_atoi(char *arg, uint64 *out)
{
	char *endptr;
	long long ret64;
	/*
	 * 10進数チェック
	 */
	errno = 0;
	ret64 = strtoll((const char*)arg, &endptr, 10);
	if ((errno == 0) && (*endptr == '\0')) {
		*out = ret64;
		return 0;
	}
	return -1;
}

static int file_load(CmdOptionType *opt)
{
	size_t ret;
	FILE *fp;
	int fd;
    struct stat st;
    int err;

    fp = fopen(opt->filepath, "rb");
    if (fp == NULL) {
		fprintf(stderr, "ERROR can not open %s\n", opt->filepath);
		return -1;
    }
    fd = fileno(fp);

    err = fstat(fd, &st);
    if (err < 0) {
		fprintf(stderr, "ERROR can not fstat %s\n", opt->filepath);
	    fclose(fp);
    	return -1;
    }
    opt->filedata_len = st.st_size;
    ret = fread(opt->filedata, st.st_size, 1, fp);
    if (ret == 0) {
		fprintf(stderr, "ERROR can not fread %s\n", opt->filepath);
	    fclose(fp);
    	return -1;
    }
    fclose(fp);
    return 0;
}

CmdOptionType *parse_args(int argc, const char* argv[])
{
	  int i;
	  int opt;

	  cmd_option.fifopath = NULL;
	  cmd_option.filepath = NULL;
	  cmd_option.is_binary_data = FALSE;
	  cmd_option.is_interaction = FALSE;
	  cmd_option.timeout = 0;

	  while ((opt = getopt(argc, argv, "ibt:p:")) != -1) {
		  switch (opt) {
		  case 'i':
	    	cmd_option.is_interaction = TRUE;
	        break;
	      case 'b':
		    cmd_option.is_binary_data = TRUE;
	        break;
	      case 't':
	    	if (cmd_atoi(optarg, &cmd_option.timeout) < 0) {
		        printf("error! -t %s\n", optarg);
	    		return NULL;
	    	}
	        break;
	      case 'p':
	    	memcpy(cmd_option.buffer_fifopath, optarg, strlen(optarg));
	    	cmd_option.buffer_fifopath[strlen(optarg)] = '\0';
	        cmd_option.fifopath = cmd_option.buffer_fifopath;
	        break;
	      default:
	        printf("error! \'%c\' \'%c\'\n", opt, optopt);
	        return NULL;
	    }
	  }
#if 0
	  printf("i = %d\n", cmd_option.is_interaction);
	  printf("b = %d\n", cmd_option.is_binary_data);
	  printf("t = %llu\n", cmd_option.timeout);
	  printf("p = %s\n", (cmd_option.fifopath != NULL) ? cmd_option.fifopath : "NULL");
#endif

	  memcpy(cmd_option.buffer_filepath, argv[optind], strlen(argv[optind]));
	  cmd_option.buffer_filepath[strlen(argv[optind])] = '\0';
      cmd_option.filepath = cmd_option.buffer_filepath;

      if (file_load(&cmd_option) < 0) {
    	  return NULL;
      }
	  return &cmd_option;
}
