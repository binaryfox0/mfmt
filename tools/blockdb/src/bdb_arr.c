#include "bdb_arr.h"

#include <stdlib.h>
#include <string.h>

#define BDB__INITIAL_ARR_CAP 4
int bdb__arr_ensure(bdb__arr_t *arr, const size_t idx)
{
    size_t new_cap = 0;
    void *new_data = 0;

    if(!arr)
        return -1;

    if(idx < arr->capacity)
    {
        if(idx >= arr->count)
            arr->count = idx + 1;

        return 0;
    }

    new_cap = arr->capacity;

    if(new_cap == 0)
        new_cap = BDB__INITIAL_ARR_CAP;

    while(idx >= new_cap)
        new_cap *= 2;

    new_data = realloc(arr->data, arr->elem_size * new_cap);
    if(!new_data)
        return -1;

    arr->data = new_data;
    arr->capacity = new_cap;
    arr->count = idx + 1;

    return 0;
}

int bdb__arr_append(bdb__arr_t *arr, const void *elem)
{
    if(arr->count >= arr->capacity)
    {
        size_t new_cap = 0;
        void *new_data = 0;

        new_cap = arr->capacity > 0 ? arr->capacity * 2 : BDB__INITIAL_ARR_CAP;
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
