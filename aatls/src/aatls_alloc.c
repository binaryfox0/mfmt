#include <aatls_alloc.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void *aatls__malloc_default(void *userdata, size_t size)
{
    (void)userdata;
    return malloc(size);
}

static void *aatls__realloc_default(void *userdata, void *ptr, size_t size)
{
    (void)userdata;
    return realloc(ptr, size);
}

static void aatls__free_default(void *userdata, void *ptr)
{
    (void)userdata;
    free(ptr);
}

static aatls_alloc_t aatls__alloc = {
    .malloc = aatls__malloc_default,
    .realloc = aatls__realloc_default,
    .free = aatls__free_default,
    .userdata = NULL
};

int aatls_alloc_set(const aatls_alloc_t *alloc)
{
    if(!alloc)
    {
        aatls__alloc = (aatls_alloc_t){
            .malloc = aatls__malloc_default,
            .realloc = aatls__realloc_default,
            .free = aatls__free_default,
            .userdata = NULL
        };
        return 0;
    }

    if(!alloc->malloc || !alloc->realloc || !alloc->free)
        return -1;

    aatls__alloc = *alloc;
    return 0;
}

void *aatls__malloc(size_t size)
{
    return aatls__alloc.malloc(aatls__alloc.userdata, size);
}

void *aatls__calloc(size_t count, size_t size)
{
    size_t total = 0;
    void *ptr = NULL;

    if(count != 0 && size > SIZE_MAX / count)
        return NULL;

    total = count * size;
    ptr = aatls__alloc.malloc(aatls__alloc.userdata, total);
    if(!ptr && total != 0)
        return NULL;
    if(ptr)
        memset(ptr, 0, total);
    return ptr;
}

void *aatls__realloc(void *ptr, size_t size)
{
    return aatls__alloc.realloc(aatls__alloc.userdata, ptr, size);
}

char *aatls__strdup(const char *str)
{
    size_t size = 0;
    char *copy = NULL;

    if(!str)
        return NULL;

    size = strlen(str) + 1;
    copy = aatls__malloc(size);
    if(copy)
        memcpy(copy, str, size);
    return copy;
}

void aatls__free(void *ptr)
{
    if(ptr)
        aatls__alloc.free(aatls__alloc.userdata, ptr);
}