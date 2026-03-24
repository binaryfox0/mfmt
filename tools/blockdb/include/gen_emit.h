#ifndef BLOCKDB_GEN_EMIT_H
#define BLOCKDB_GEN_EMIT_H

#include <stddef.h>
#include <bdb_arr.h>

typedef struct
{
    const char *root_path;
    const char *include_dir;
    const char *src_dir;
    const char *stem;
    const char *header_ext;
    const char *source_ext;
} bdb_gen_path_config_t;

typedef struct
{
    const char *prefix;
    const char *type_name;
    char sep;
    size_t name_offset; /* offsetof(struct, name) */
} bdb_gen_name_fmt_t;

int bdb_gen_emit_code(
        const bdb_gen_path_config_t *pc,
        const bdb__arr_t *arr,
        const bdb__arr_view_t *vds, /* bdb_gen_var_desc_t */
        const bdb_gen_name_fmt_t *fmt
);

#endif

