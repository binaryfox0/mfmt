#ifndef AATLS_ALLOC_H
#define AATLS_ALLOC_H

#include <stddef.h>

typedef void *(*aatls_malloc_fn)(void *userdata, size_t size);
typedef void *(*aatls_realloc_fn)(void *userdata, void *ptr, size_t size);
typedef void (*aatls_free_fn)(void *userdata, void *ptr);

typedef struct
{
    aatls_malloc_fn malloc;
    aatls_realloc_fn realloc;
    aatls_free_fn free;
    void *userdata;
} aatls_alloc_t;

/* Pass NULL to restore the default allocator. Configure before using aatls and
 * keep it unchanged until all aatls-owned allocations have been released.
 */
int aatls_alloc_set(const aatls_alloc_t *alloc);

#endif