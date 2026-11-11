#ifndef MFMTIO_ALLOC_H
#define MFMTIO_ALLOC_H

#include <stddef.h>

typedef void *(*msav_malloc_fn)(void *userdata, size_t size);
typedef void *(*msav_realloc_fn)(void *userdata, void *ptr, size_t size);
typedef void (*msav_free_fn)(void *userdata, void *ptr);

typedef struct
{
    msav_malloc_fn malloc;
    msav_realloc_fn realloc;
    msav_free_fn free;
    void *userdata;
} msav_alloc_t;

/* Pass NULL to restore the default allocator. Configure before using the libraries
 * and keep it unchanged until all library-owned allocations have been released.
 * Buffers returned by library APIs must be released with this allocator's free
 * callback and userdata.
 */
int msav_alloc_set(const msav_alloc_t *alloc);

#endif
