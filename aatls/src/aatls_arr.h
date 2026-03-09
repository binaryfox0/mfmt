#ifndef AATLS_ARR_H
#define AATLS_ARR_H

#include <stdint.h>

typedef struct {
    void **ptr;
    int32_t *size;
    int32_t *cap;
    int32_t stride;
} aatls__arr;

int aatls__arr_ensure(
        const aatls__arr *arr,
        const int32_t idx
);

int aatls__arr_validate(
        const aatls__arr *arr
);

int aatls__arr_push(
        const aatls__arr *arr, 
        const void *elem
);

int aatls__arr_insert(
        const aatls__arr *arr,
        const void *elem,
        const int32_t idx
);

#endif
