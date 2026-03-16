#ifndef BLOCKDB_GEN_VARS_H
#define BLOCKDB_GEN_VARS_H

#include "bdb_arr.h"

typedef struct
{
    const char *name;
    int is_bool;
    int bit_idx;
    int out_offset;
    int out_size;
} bdb__gen_var_desc_t;

#define bdb__gen_var_desc(name_in, is_bool_in, bit_idx_in, type, member) \
    (bdb__gen_var_desc_t) { \
        .name = (name_in), \
        .is_bool = (is_bool_in), \
        .bit_idx = (bit_idx_in), \
        .out_offset = offsetof(type, member), \
        .out_size = sizeof(((type*)0)->member) \
    }

bdb__gen_var_desc_t* bdb__gen_var_desc_find(
        const bdb__arr_t *vds, 
        const char *str, 
        const size_t len
);

#endif
