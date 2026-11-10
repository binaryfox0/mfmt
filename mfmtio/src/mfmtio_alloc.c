#include <mfmtio_alloc.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void *mfmtio__malloc_default(void *userdata, size_t size)
{
    (void)userdata;
    return malloc(size);
}

static void *mfmtio__realloc_default(void *userdata, void *ptr, size_t size)
{
    (void)userdata;
    return realloc(ptr, size);
}

static void mfmtio__free_default(void *userdata, void *ptr)
{
    (void)userdata;
    free(ptr);
}

static mfmtio_alloc_t mfmtio__alloc = {
    .malloc = mfmtio__malloc_default,
    .realloc = mfmtio__realloc_default,
    .free = mfmtio__free_default,
    .userdata = NULL
};

int mfmtio_alloc_set(const mfmtio_alloc_t *alloc)
{
    if(!alloc)
    {
        mfmtio__alloc = (mfmtio_alloc_t){
            .malloc = mfmtio__malloc_default,
            .realloc = mfmtio__realloc_default,
            .free = mfmtio__free_default,
            .userdata = NULL
        };
        return 0;
    }

    if(!alloc->malloc || !alloc->realloc || !alloc->free)
        return -1;

    mfmtio__alloc = *alloc;
    return 0;
}

void *mfmtio__malloc(size_t size)
{
    void *ptr = mfmtio__alloc.malloc(mfmtio__alloc.userdata, size);
    return ptr;
}

void *mfmtio__calloc(size_t count, size_t size)
{
    size_t total = 0;
    void *ptr = NULL;

    if(count != 0 && size > SIZE_MAX / count)
        return NULL;

    total = count * size;
    ptr = mfmtio__alloc.malloc(mfmtio__alloc.userdata, total);
    if(!ptr && total != 0)
        return NULL;
    if(ptr)
        memset(ptr, 0, total);
    return ptr;
}

void *mfmtio__realloc(void *ptr, size_t size)
{
    void *new_ptr = mfmtio__alloc.realloc(mfmtio__alloc.userdata, ptr, size);
    return new_ptr;
}

void *mfmtio__strdup(const char *str)
{
    size_t size = 0;
    char *copy = NULL;

    if(!str)
        return NULL;

    size = strlen(str) + 1;
    copy = mfmtio__malloc(size);
    if(copy)
        memcpy(copy, str, size);
    return copy;
}

void mfmtio__free(void *ptr)
{
    if(ptr)
        mfmtio__alloc.free(mfmtio__alloc.userdata, ptr);
}
