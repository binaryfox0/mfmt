#ifndef AATLS_ALLOC_PRIV_H
#define AATLS_ALLOC_PRIV_H

#include <stddef.h>

void *aatls__malloc(size_t size);
void *aatls__calloc(size_t count, size_t size);
void *aatls__realloc(void *ptr, size_t size);
char *aatls__strdup(const char *str);
void aatls__free(void *ptr);

#endif