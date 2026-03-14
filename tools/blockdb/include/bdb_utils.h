#ifndef BLOCKDB_UTILS_H
#define BLOCKDB_UTILS_H

#include <stddef.h>

int bdb__read_file(
        const char *path, 
        char **out_buf, 
        long *out_size
);

/*
 * compare first l bytes of s1 with s2
 * s2 must be exactly length l
 */
int bdb__tokcmp(const char *s1, const char *s2, const size_t l);

char* bdb__tokdup(const char *s, const size_t l);

#endif
