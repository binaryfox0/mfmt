#ifndef MFMTIO_ALLOC_H
#define MFMTIO_ALLOC_H

#include <stddef.h>

typedef void *(*mfmtio_malloc_fn)(void *userdata, size_t size);
typedef void *(*mfmtio_realloc_fn)(void *userdata, void *ptr, size_t size);
typedef void (*mfmtio_free_fn)(void *userdata, void *ptr);

typedef struct
{
    mfmtio_malloc_fn malloc;
    mfmtio_realloc_fn realloc;
    mfmtio_free_fn free;
    void *userdata;
} mfmtio_alloc_t;

/* Pass NULL to restore the default allocator. Configure before using the libraries
 * and keep it unchanged until all library-owned allocations have been released.
 * Buffers returned by library APIs must be released with this allocator's free
 * callback and userdata.
 */
int mfmtio_alloc_set(const mfmtio_alloc_t *alloc);

#endif
