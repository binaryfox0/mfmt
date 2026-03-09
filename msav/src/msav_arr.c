#include "msav_arr.h"

#include <stdlib.h>
#include <string.h>

int msav__arr_create(
        msav__arr_t *arr, 
        const size_t elem_size, 
        const size_t initial_size
)
{
    if(!arr)
        return -1;

    if(initial_size == 0)
        return 0;

    arr->data = malloc(elem_size * initial_size);
    if(!arr->data)
        return -1;

    arr->capacity = initial_size;
    arr->count = initial_size;
    arr->elem_size = elem_size;

    return 0;
}

void msav__arr_destroy(
        msav__arr_t *arr
)
{
    if(!arr)
        return;
    free(arr->data);
    *arr = (msav__arr_t){0};
}
