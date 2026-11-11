#include <msav_alloc.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void *msav__malloc_default(void *userdata, size_t size)
{
    (void)userdata;
    return malloc(size);
}

static void *msav__realloc_default(void *userdata, void *ptr, size_t size)
{
    (void)userdata;
    return realloc(ptr, size);
}

static void msav__free_default(void *userdata, void *ptr)
{
    (void)userdata;
    free(ptr);
}

static msav_alloc_t msav__alloc = {
    .malloc = msav__malloc_default,
    .realloc = msav__realloc_default,
    .free = msav__free_default,
    .userdata = NULL
};

int msav_alloc_set(const msav_alloc_t *alloc)
{
    if(!alloc)
    {
        msav__alloc = (msav_alloc_t){
            .malloc = msav__malloc_default,
            .realloc = msav__realloc_default,
            .free = msav__free_default,
            .userdata = NULL
        };
        return 0;
    }

    if(!alloc->malloc || !alloc->realloc || !alloc->free)
        return -1;

    msav__alloc = *alloc;
    return 0;
}

void *msav__malloc(size_t size)
{
    void *ptr = msav__alloc.malloc(msav__alloc.userdata, size);
    return ptr;
}

void *msav__calloc(size_t count, size_t size)
{
    size_t total = 0;
    void *ptr = NULL;

    if(count != 0 && size > SIZE_MAX / count)
        return NULL;

    total = count * size;
    ptr = msav__alloc.malloc(msav__alloc.userdata, total);
    if(!ptr && total != 0)
        return NULL;
    if(ptr)
        memset(ptr, 0, total);
    return ptr;
}

void *msav__realloc(void *ptr, size_t size)
{
    void *new_ptr = msav__alloc.realloc(msav__alloc.userdata, ptr, size);
    return new_ptr;
}

void *msav__strdup(const char *str)
{
    size_t size = 0;
    char *copy = NULL;

    if(!str)
        return NULL;

    size = strlen(str) + 1;
    copy = msav__malloc(size);
    if(copy)
        memcpy(copy, str, size);
    return copy;
}

void msav__free(void *ptr)
{
    if(ptr)
        msav__alloc.free(msav__alloc.userdata, ptr);
}
