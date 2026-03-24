#ifndef BLOCKDB_ARR_H
#define BLOCKDB_ARR_H

#include <stddef.h>

#define bdb__arr_get(arr, type, idx) \
    (((type*)(arr)->data)[(idx)])
#define bdb__arr_get_base(arr, idx) \
    ((uint8_t*)(arr)->data + (arr)->elem_size * (idx))

typedef struct
{
    void *data;
    size_t elem_size;
    size_t count;
} bdb__arr_view_t;

typedef struct
{
    void *data;
    size_t elem_size;
    size_t capacity;
    size_t count;
} bdb__arr_t;

int bdb__arr_ensure(bdb__arr_t *arr, const size_t idx);
int bdb__arr_append(bdb__arr_t *arr, const void *elem);
void bdb__arr_destroy(bdb__arr_t *arr);
void bdb__arr_destroy_strings(bdb__arr_t *arr);

#endif
