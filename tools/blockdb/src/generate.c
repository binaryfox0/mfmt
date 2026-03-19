#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

#include <tree_sitter/api.h>

#include "bdb_utils.h"
#include "bdb_log.h"
#include "bdb_fs.h"
#include "bdb_arr.h"

#include "gen_utils.h"
#include "gen_vars.h"
#include "gen_tree.h"
#include "gen_assign.h"
#include "gen_blocks.h"

#define ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))
extern const TSLanguage *tree_sitter_java(void);

static const bdb__gen_var_desc_t bdb__queried_vars[] =
{
    bdb__gen_var_desc("update",        1, 0, bdb__gen_tn_t, has_building),
    bdb__gen_var_desc("destructible",  1, 1, bdb__gen_tn_t, has_building),

    bdb__gen_var_desc("hasItems",      1, 0, bdb__gen_tn_t, modules_bitmask),
    bdb__gen_var_desc("hasPower",      1, 1, bdb__gen_tn_t, modules_bitmask),
    bdb__gen_var_desc("hasLiquids",    1, 2, bdb__gen_tn_t, modules_bitmask),
};

static int bdb__gen_parse_class(
        TSNode node,
        const char *src,
        bdb__arr_t *vds,
        bdb__arr_t *classes
)
{
    TSNode name_node = {0};
    TSNode super_node = {0};
    uint32_t s = 0;
    uint32_t e = 0;
    bdb__gen_tn_t *tn = 0;

    name_node = ts_node_child_by_field_name(node, "name", 4);
    super_node = ts_node_child_by_field_name(node, "superclass", 10);

    if(ts_node_is_null(name_node))
    {
        bdb__error("non-null node was expected");
        return -1;
    }

    tn = calloc(1, sizeof(*tn));
    if(!tn)
    {
        bdb__error_errno("failed to allocated memory for a tree node");
        return -1;
    }
    tn->children.elem_size = sizeof(void*);

    s = ts_node_start_byte(name_node);
    e = ts_node_end_byte(name_node);

    tn->name = bdb__tokdup(src + s, (size_t)(e - s));

    if(!ts_node_is_null(super_node))
    {
        TSNode type = {0};
        type = ts_node_named_child(super_node, 0);
        if(!ts_node_is_null(type))
        {
            if(!strcmp(ts_node_type(type), "generic_type"))
            {
                TSNode base = {0};
                base = ts_node_named_child(type, 0);
                if(!ts_node_is_null(base))
                    type = base;
            }

            s = ts_node_start_byte(type);
            e = ts_node_end_byte(type);

            tn->parent_name = bdb__tokdup(src + s, (size_t)(e - s));
        }
    }

    if(bdb__gen_scan_ctor(node, src, vds, tn) < 0) 
    {
        free(tn->name);
        free(tn->parent_name);
        free(tn);
        return -1;
    }

    bdb__arr_append(classes, &tn);

//    bdb__info("%s -> %s, has_building: 0x%02X, modules_bitmask; 0x%02X", 
//            tn->name, tn->parent_name, tn->has_building, tn->modules_bitmask);

    return 0;
}

static int bdb__gen_walk_ast(
        TSNode node, 
        const char *src,
        bdb__arr_t *vds,
        bdb__arr_t *classes
)
{
    const char *type = 0;
    uint32_t count = 0;

    type = ts_node_type(node);
    if(!strcmp(type, "class_declaration"))
    {
        if(bdb__gen_parse_class(node, src, vds, classes) < 0)
            return -1;
    }

    count = ts_node_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = {0};
        child = ts_node_child(node, i);
        if(bdb__gen_walk_ast(child, src, vds, classes) < 0)
            return -1;
    }
    return 0;
}


char* bdb__gen_camel_to_upper_underscore(
        const char *s,
        size_t l,
        const char *prefix)
{
    size_t prefix_len = 0;
    size_t out_len = 0;
    char *out = 0;

    if(!s)
        return 0;

    if(prefix)
        prefix_len = strlen(prefix);

    out_len = prefix_len;

    for(size_t i = 0; i < l; i++)
    {
        if(i > 0 && isupper((unsigned char)s[i]))
            out_len++;
        out_len++;
    }

    out = malloc(out_len + 1);
    if(!out)
        return 0;

    if(prefix_len)
        memcpy(out, prefix, prefix_len);

    size_t j = prefix_len;

    for(size_t i = 0; i < l; i++)
    {
        unsigned char c = (unsigned char)s[i];

        if(i > 0 && isupper(c))
            out[j++] = '_';

        out[j++] = (char)toupper(c);
    }

    out[j] = '\0';

    return out;
}

typedef struct
{
    const char *prefix;
    const char *type_name;
    char sep;
    size_t name_offset; /* offsetof(struct, name) */
} bdb__gen_name_fmt_t;

#define BDB__INDENT 4
static inline const char*
bdb__get_name_from_offset(const void *elem, size_t offset)
{
    const char **ptr = 0;

    if(!elem)
        return 0;

    ptr = (const char**)((const char*)elem + offset);
    return *ptr;
}

static inline void
bdb__fprint_upper_path(FILE *file, const char *str, char sep)
{
    const char *p = 0;

    if(!file || !str)
        return;

    for(p = str; *p; p++)
    {
        if(*p == '.')
            fputc(sep, file);
        else
            fputc(toupper((unsigned char)*p), file);
    }
}

static void
bdb__fprint_prefixed_upper(FILE *file,
                           const char *prefix,
                           const char *name,
                           char sep)
{
    const char *p = 0;
    size_t i = 0;

    if(!file || !name)
        return;

    if(prefix)
    {
        for(i = 0; prefix[i]; i++)
            fputc(toupper((unsigned char)prefix[i]), file);
    }

    for(p = name; *p; p++)
    {
        if(p != name && isupper((unsigned char)*p))
            fputc(sep, file);

        fputc(toupper((unsigned char)*p), file);
    }
}

int bdb__gen_emit_header(
        const char *path,
        const bdb__arr_t *arr,
        const bdb__gen_name_fmt_t *fmt)
{
    FILE *file = 0;
    const char *fname = 0;
    size_t i = 0;

    if(!path || !arr || !fmt)
        return -1;

    file = fopen(path, "w");
    if(!file)
    {
        bdb__error_errno("failed to open output header path");
        return -1;
    }

    fname = strrchr(path, BDB__PATH_SEP);
    fname = fname ? fname + 1 : path;

    /* include guard */
    fprintf(file, "#ifndef ");
    bdb__fprint_upper_path(file, fname, fmt->sep);
    fputc('\n', file);

    fprintf(file, "#define ");
    bdb__fprint_upper_path(file, fname, fmt->sep);
    fputc('\n', file);

    fputs("\ntypedef enum\n{\n", file);

    for(i = 0; i < arr->count; i++)
    {
        const char *name = *(const char**)((uint8_t*)arr->data + i * arr->elem_size + fmt->name_offset);

        if(!name)
            continue;

        fprintf(file, "%*s", BDB__INDENT, "");

        bdb__fprint_prefixed_upper(
                file,
                fmt->prefix,
                name,
                fmt->sep);

        if(i < arr->count - 1)
            fputc(',', file);

        fputc('\n', file);
    }

    fprintf(file, "} %s;\n\n#endif\n",
            fmt->type_name ? fmt->type_name : "");

    fclose(file);
    return 0;
}

int bdb__gen_generate(
        const char *out_path,
        const char *file_name,
        bdb__arr_t *blocks, /* bdb__gen_tn_t */
        const bdb__gen_name_fmt_t *spec
)
{
    static const bdb__gen_name_fmt_t fmt = {
        .prefix = "msav_block_",
        .type_name = "msav_block_t",
        .sep = '_',
        .name_offset = offsetof(bdb__gen_tn_t, name)
    };

    char *header_path = 0;
    char *src_path = 0;

    header_path = bdb__sprintf("%s%cinclude%c%s.h", out_path, BDB__PATH_SEP, BDB__PATH_SEP, file_name);
    src_path = bdb__sprintf("%s%csrc%c%s.c", out_path, BDB__PATH_SEP,BDB__PATH_SEP, file_name);
    if(!header_path || !src_path)
        goto cleanup;

    bdb__gen_emit_header(header_path, blocks, &fmt);

cleanup:
    free(header_path);
    free(src_path);
    return 0;
}

void generate_command(void *args)
{
    const char *mindustry_root = *(const char**)args;
    const char *out_path = *((const char**)args + 1);

    char *blocks_dir_path = 0;
    char *blocks_java_path = 0;

    char *src = 0;
    long src_len = 0;

    TSParser *parser = 0;
    TSTree *tree = 0;
    TSNode root = {0};

    bdb__arr_t file_list = {0};
    bdb__arr_t classes = {0};
    bdb__arr_t blocks = {0};

    bdb__arr_t vds = {
        .count = ARRSZ(bdb__queried_vars),
        .capacity = ARRSZ(bdb__queried_vars),
        .elem_size = sizeof(bdb__queried_vars[0]),
        .data = (void*)bdb__queried_vars
    };

    int ret = 0;

    file_list.elem_size = sizeof(void*);
    classes.elem_size   = sizeof(void*);
    blocks.elem_size    = sizeof(bdb__gen_tn_t);

    blocks_dir_path  = bdb__fs_cat_path(mindustry_root, "core/src/mindustry/world/blocks");
    blocks_java_path = bdb__fs_cat_path(mindustry_root, "core/src/mindustry/content/Blocks.java");

    if(!blocks_dir_path || !blocks_java_path)
        goto cleanup;

    if(!bdb__fs_is_dir(blocks_dir_path))
    {
        bdb__info("\"%s\" is not a directory", blocks_dir_path);
        goto cleanup;
    }

    if(!bdb__fs_is_file(blocks_java_path))
    {
        bdb__info("\"%s\" is not a file", blocks_java_path);
        goto cleanup;
    }

    if(bdb__fs_collect_files(blocks_dir_path, &file_list) < 0)
        goto cleanup;

    free(blocks_dir_path);
    blocks_dir_path = 0;

    /* ===== init parser ===== */
    parser = ts_parser_new();
    if(!parser)
        goto cleanup;

    ts_parser_set_language(parser, tree_sitter_java());

    for(size_t i = 0; i < file_list.count; i++)
    {
        char *path = bdb__arr_get(&file_list, char*, i);

        if(bdb__read_file(path, &src, &src_len) < 0)
            continue;

        tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
        root = ts_tree_root_node(tree);

        if(bdb__gen_walk_ast(root, src, &vds, &classes) < 0)
        {
            bdb__error("failed parsing \"%s\"", path);
        }

        ts_tree_delete(tree);
        tree = 0;

        free(src);
        src = 0;
    }

    bdb__arr_destroy_strings(&file_list);

    bdb__gen_build_tree(&classes);

    for(size_t i = 0; i < classes.count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(&classes, bdb__gen_tn_t*, i);
        if(tn->parent)
            continue;

        bdb__gen_resolve_tree(tn, &vds);
    }

    if(bdb__read_file(blocks_java_path, &src, &src_len) < 0)
        goto cleanup;

    tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
    root = ts_tree_root_node(tree);

    TSNode blocks_node = bdb__gen_find_class(root, src, "Blocks");

    bdb__gen_parse_block_class(blocks_node, src, &classes, &vds, &blocks);

    bdb__gen_generate(out_path, "msav_block_gen", &blocks, 0);
//    for(size_t i = 0; i < blocks.count; i++)
//    {
//        bdb__gen_tn_t *tn = &bdb__arr_get(&blocks, bdb__gen_tn_t, i);
//
//        bdb__info("%zu: %s", i, tn->name);
//        bdb__info("hb: 0x%02X, mb: 0x%02X",
//                  tn->has_building,
//                  tn->modules_bitmask);
//
//        free(tn->name);
//    }

cleanup:

    /* ===== cleanup classes ===== */
    for(size_t i = 0; i < classes.count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(&classes, bdb__gen_tn_t*, i);
        if(!tn) continue;

        free(tn->name);
        free(tn->parent_name);
        bdb__arr_destroy(&tn->children);
        free(tn);
    }

    /* ===== cleanup parser ===== */
    if(tree)   ts_tree_delete(tree);
    if(parser) ts_parser_delete(parser);

    /* ===== cleanup buffers ===== */
    free(src);
    free(blocks_java_path);
    free(blocks_dir_path);

    bdb__arr_destroy(&classes);
    bdb__arr_destroy(&blocks);
}
