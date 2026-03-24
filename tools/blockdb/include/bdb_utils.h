#ifndef BLOCKDB_UTILS_H
#define BLOCKDB_UTILS_H

#include <stddef.h>

int bdb__read_file(
        const char *path, 
        char **out_buf, 
        long *out_size
);

int bdb__confirm(
        const char *label, 
        const int default_ans
);

#endif
