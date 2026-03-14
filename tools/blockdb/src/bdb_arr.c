#include "bdb_arr.h"

#include <stdlib.h>
#include <string.h>

int bdb__arr_append(bdb__arr_t *arr, const void *elem)
{
    if(arr->count >= arr->capacity)
    {
        size_t new_cap = 0;
        void *new_data = 0;

        new_cap = arr->capacity > 0 ? arr->capacity * 2 : 4;
        new_data = realloc(arr->data, arr->elem_size * new_cap);
        if(!new_data)
            return -1;

        arr->capacity = new_cap;
        arr->data = new_data;
    }

    memcpy((uint8_t*)arr->data + (arr->elem_size * arr->count), elem, arr->elem_size);
    arr->count++;

    return 0;
}

void bdb__arr_destroy(bdb__arr_t *arr)
{
    if(!arr)
        return;
    free(arr->data);
    *arr = (bdb__arr_t){0};
}

void bdb__arr_destroy_strings(bdb__arr_t *arr)
{
    if(!arr)
        return;
    for(size_t i = 0; i < arr->count; i++)
        free(bdb__arr_get(arr, char*, i));
    bdb__arr_destroy(arr);
}
