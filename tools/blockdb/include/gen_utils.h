#ifndef BLOCKDB_GEN_UTILS_H
#define BLOCKDB_GEN_UTILS_H

#include <tree_sitter/api.h>

#define BDB__STRINGIFY(c) (char[2]){(c), 0}

TSNode bdb__gen_find_class(
        TSNode node,
        const char *src,
        const char *class_name
);

/*
 * compare first l bytes of s1 with s2
 * s2 must be exactly length l
 */
int bdb__tokcmp(const char *s1, const char *s2, const size_t l);

char* bdb__tokdup(const char *s, const size_t l);
char* bdb__sprintf(const char *fmt, ...);

#endif
