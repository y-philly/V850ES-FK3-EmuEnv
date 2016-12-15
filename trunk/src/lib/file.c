#include "file.h"
#include "string.h"
#include <stdio.h>
#include <sys/stat.h>

bool file_exist(const char *path)
{
	struct stat st;
	int err;

	err = stat(path, &st);
	if (err < 0) {
		return FALSE;
	}
	return TRUE;
}

uint32 file_get_parent_folder_pathlen(const char *filepath)
{
	uint32 retlen = 0;
	int i;
	int len = strlen(filepath);

	for (i = (len -1); i >= 0; i--) {
		if (filepath[i] != '/') {
			continue;
		}
		retlen = i + 1;
		break;
	}
	return retlen;
}

bool file_load(FileType *file)
{
	size_t ret;
	FILE *fp;
	int fd;
    struct stat st;
    int err;

    fp = fopen((char*)file->filepath.str, "rb");
    if (fp == NULL) {
		printf("ERROR can not open %s\n", file->filepath.str);
		return FALSE;
    }
    fd = fileno(fp);

    err = fstat(fd, &st);
    if (err < 0) {
		printf("ERROR can not fstat %s\n", file->filepath.str);
	    fclose(fp);
		return FALSE;
    }
    file->size = st.st_size;
    if (file->size > MAX_FILE_SIZE) {
		printf("ERROR file size too large %s size=%u\n", file->filepath.str, file->size);
	    fclose(fp);
		return FALSE;
    }

    ret = fread(file->buffer, st.st_size, 1, fp);
    if (ret == 0) {
		printf("ERROR can not fread %s\n", file->filepath.str);
	    fclose(fp);
    	return FALSE;
    }
    fclose(fp);
    return TRUE;
}

bool file_ropen(FileType *file)
{
	file->fp = fopen((char*)file->filepath.str, "rb");
	if (file->fp == NULL) {
		printf("file open error:%s\n", file->filepath.str);
		return FALSE;
	}
	return TRUE;
}

uint32 file_getline(FileType *file, char *line, int size)
{
	uint32 n = 0;
	char c;
	while (TRUE) {
		c = fgetc(file->fp);
		if (c < 0 || c == '\n') {
			break;
		}
		line[n] = c;
		n++;
	}
	return n;
}
void file_close(FileType *file)
{
	if (file->fp != NULL) {
		fclose(file->fp);
		file->fp = NULL;
	}
	return;
}

