#ifndef BLOCKDB_GEN_BLOCKS_H
#define BLOCKDB_GEN_BLOCKS_H

#include <tree_sitter/api.h>
#include "bdb_arr.h"

int bdb__gen_parse_block_class(
        TSNode class_node,
        const char *src,
        const bdb__arr_t *classes,
        bdb__arr_t *vds,
        bdb__arr_t *instances
);

#endif
