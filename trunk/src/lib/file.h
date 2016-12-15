#ifndef _FILE_H_
#define _FILE_H_

#include "std_types.h"
#include "token.h"

extern bool file_exist(const char *path);

#define MAX_FILE_SIZE	(1024 * 1024 * 5)
typedef struct {
	TokenStringType filepath;
	void			*fp;
	uint32			size;
	uint8			buffer[MAX_FILE_SIZE];
} FileType;
extern bool file_load(FileType *file);

extern uint32 file_get_parent_folder_pathlen(const char *filepath);

extern bool file_ropen(FileType *file);
extern uint32 file_getline(FileType *file, char *line, int size);
extern void file_close(FileType *file);

#endif /* _FILE_H_ */
