#ifndef BLOCKDB_GEN_ASSIGN_H
#define BLOCKDB_GEN_ASSIGN_H

#include <tree_sitter/api.h>

#include "bdb_arr.h"
#include "gen_tree.h"

int bdb__gen_handle_assign(
        const bdb__arr_t *vds,
        const char *name,
        size_t name_len,
        TSNode right,
        const char *src,
        bdb__gen_tn_t *tn
);

int bdb__gen_handle_assign_expr(
        const bdb__arr_t *vds,
        TSNode left,
        TSNode right,
        const char *src,
        bdb__gen_tn_t *tn
);


int bdb__gen_scan_ctor_body(
        TSNode node, 
        const char *src, 
        const bdb__arr_t *vds,
        bdb__gen_tn_t *tn
);

int bdb__gen_scan_ctor(
        TSNode node, 
        const char *src, 
        const bdb__arr_t *vds,
        bdb__gen_tn_t *tn
);

#endif
