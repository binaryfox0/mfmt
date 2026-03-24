#ifndef BLOCKDB_GEN_TREE_H
#define BLOCKDB_GEN_TREE_H

#include <stdint.h>
#include "bdb_arr.h"

#define bdb__concat(a, b) a##b
#define bdb__gen_tn_def_bits(type, name) \
    type name; \
    type bdb__concat(name, _mask)

typedef struct bdb__gen_tn
{
    char *name;
    char *parent_name;

    /*
     * !!! Info - The order of each bits is flexible - Info !!! *
     * bit 0: update == true
     * bit 1: destructible == true
    */
    bdb__gen_tn_def_bits(uint8_t, has_building);

    /*
     * !!! Warning - The order MUST be fixed - Warning !!! *
     * bit 0: items != null (hasItems == true)
     * bit 1: power != null (hasPower == true)
     * bit 2: liquids != null (hasLiquids == true)
     * bit 3: always on (compatibility with ConsumeModule)
     * bit 4: timeScale != 1f
     * bit 5: lastDisabler != null && lastDisabler.isValid()
     */
    bdb__gen_tn_def_bits(uint8_t, modules_bitmask);

    struct bdb__gen_tn *parent;
    bdb__arr_t children; /* bdb__gen_tn_t* */
} bdb__gen_tn_t;

int bdb__gen_build_tree(
        bdb__arr_t *classes /* bdb__gen_tn_t* */
);

int bdb__gen_merge_flags(
        const bdb__arr_view_t *vds, /* bdb__gen_var_desc_t */
        void *child,
        const void *parent
);

void bdb__gen_resolve_tree(
        bdb__gen_tn_t *tn,
        const bdb__arr_view_t *vds /* bdb__gen_var_desc_t */
);

#endif
