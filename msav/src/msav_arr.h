#ifndef MSAV_ARR_H
#define MSAV_ARR_H

#include <stddef.h>

typedef struct
{
    void *data;
    size_t elem_size;
    size_t capacity;
    size_t count;
} msav__arr_t;

#define msav__arr_at(arr, type, idx) (((type*)(arr)->data)[(idx)])

int msav__arr_create(
        msav__arr_t *arr, 
        const size_t elem_size, 
        const size_t initial_size
);

void msav__arr_destroy(
        msav__arr_t *arr
);

#endif
