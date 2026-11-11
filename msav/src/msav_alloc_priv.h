#ifndef MSAV_ALLOC_PRIV_H
#define MSAV_ALLOC_PRIV_H

#include <stddef.h>

void *msav__malloc(size_t size);
void *msav__calloc(size_t count, size_t size);
void *msav__realloc(void *ptr, size_t size);
void *msav__strdup(const char *str);
void msav__free(void *ptr);

#endif
