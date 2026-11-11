#ifndef BLOCKDB_GEN_ASSIGN_H
#define BLOCKDB_GEN_ASSIGN_H

#include <tree_sitter/api.h>

#include "bdb_arr.h"

int bdb__gen_handle_assign(
        const bdb__arr_view_t *vds,
        const char *name,
        size_t name_len,
        TSNode right,
        const char *src,
        void *out
);

int bdb__gen_handle_assign_expr(
        const bdb__arr_view_t *vds,
        TSNode left,
        TSNode right,
        const char *src,
        void *out
);


int bdb__gen_scan_ctor_body(
        const bdb__arr_view_t *vds,
        TSNode node, 
        const char *src, 
        void *out
);

int bdb__gen_scan_ctor(
        const bdb__arr_view_t *vds,
        TSNode node, 
        const char *src, 
        void *out
);

#endif
