#ifndef MFMTIO_ALLOC_PRIV_H
#define MFMTIO_ALLOC_PRIV_H

#include <stddef.h>

void *mfmtio__malloc(size_t size);
void *mfmtio__calloc(size_t count, size_t size);
void *mfmtio__realloc(void *ptr, size_t size);
void *mfmtio__strdup(const char *str);
void mfmtio__free(void *ptr);

#define malloc mfmtio__malloc
#define calloc mfmtio__calloc
#define realloc mfmtio__realloc
#define strdup mfmtio__strdup
#define free mfmtio__free

#endif
