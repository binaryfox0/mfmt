#include "aatls_arr.h"

#include <stdlib.h>
#include <string.h>

#define AATLS__MIN(a, b) ((a) < (b) ? (a) : (b))

int aatls__arr_ensure(
        const aatls__arr *arr,
        const int32_t idx
)
{
    int32_t new_cap = 0;
    void *new_ptr = 0;

    if(*arr->cap > idx)
        return 0;
    
    new_cap = *arr->cap ? *arr->cap * 2 : 4;
    while(new_cap <= idx)
        new_cap *= 2;

    new_ptr = realloc(*arr->ptr, new_cap * arr->stride);
    if(!new_ptr)
        return -1;

    *arr->ptr = new_ptr;
    *arr->cap = new_cap;
    
    return 0;
}

int aatls__arr_validate(
        const aatls__arr *arr
)
{
    if(
        arr->size < 0 ||
        *arr->cap < 0 ||
        *arr->size > *arr->cap ||
        (*arr->cap > 0 && !*arr->ptr) ||
        (*arr->cap == 0 && *arr->ptr)
    ) return -1;

    return 0;
}

int aatls__arr_push(
        const aatls__arr *arr, 
        const void *elem
)
{
    if(aatls__arr_ensure(arr, *arr->size) < 0)
        return -1;

    memcpy(
            (uint8_t*)*arr->ptr + *arr->size * arr->stride, 
            elem, arr->stride
    );
    (*arr->size)++;
    return 0;
}

int aatls__arr_insert(
        const aatls__arr *arr,
        const void *elem,
        const int32_t idx)
{
    int32_t ins_idx = 0;
    if(aatls__arr_ensure(arr, *arr->size) < 0)
        return -1;

    ins_idx = AATLS__MIN(*arr->size, idx);
    memmove(
            (uint8_t*)*arr->ptr + (ins_idx + 1) * arr->stride,
            (uint8_t*)*arr->ptr + ins_idx * arr->stride,
            (*arr->size - ins_idx) * arr->stride
    );
    memcpy((uint8_t*)*arr->ptr + ins_idx * arr->stride, 
            elem, arr->stride);
    return 0;
}

