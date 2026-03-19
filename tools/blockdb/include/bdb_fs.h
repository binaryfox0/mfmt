#ifndef BLOCKDB_FS_H
#define BLOCKDB_FS_H

#include "bdb_arr.h"

#ifdef _WIN32
#define BDB__PATH_SEP '\\'
#else
#define BDB__PATH_SEP '/'
#endif

char *bdb__fs_cat_path(const char *a, const char *b);
int bdb__fs_is_file(const char *path);
int bdb__fs_is_dir(const char *path);
int bdb__fs_collect_files(
        const char *path, 
        bdb__arr_t *out /* char* */
);

#endif
