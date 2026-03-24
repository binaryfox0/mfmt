#ifndef BLOCKDB_GEN_VARS_H
#define BLOCKDB_GEN_VARS_H

#include "bdb_arr.h"

typedef enum
{
    BDB__GEN_VAR_UNKNOWN = 0,
    BDB__GEN_VAR_BIT,
    BDB__GEN_VAR_BITS,
    __BDB__GEN_VAR_MAX__
} bdb__gen_var_kind_t;

typedef struct bdb__gen_var_desc
{
    const char *name;
    bdb__gen_var_kind_t kind;
    int offset;
    int size;
    struct bdb__gen_var_desc *parent;
    union
    {
        bdb__arr_view_t children;
    };
} bdb__gen_var_desc_t;

static inline bdb__gen_var_desc_t bdb__gen_var_bit(
        const char *name, 
        const int bit_idx
)
{
    return 
        (bdb__gen_var_desc_t){
            .name = name, .kind = BDB__GEN_VAR_BIT,
            .offset = bit_idx
        };
}

static inline bdb__gen_var_desc_t bdb__gen_var_bits(
        const char *out_name, /* the generated output name */
        const int offset,
        const int size,
        const bdb__arr_view_t bits_desc)
{
    return 
        (bdb__gen_var_desc_t){ 
            .name = out_name, .kind = BDB__GEN_VAR_BITS,
            .offset = offset, .size = size, 
            .children = bits_desc
        };
}

int bdb__gen_link_vars(
        const bdb__arr_view_t *vds
);

bdb__gen_var_desc_t* bdb__gen_var_desc_find(
        const bdb__arr_view_t *vds, 
        const char *str, 
        const size_t len
);

#endif
