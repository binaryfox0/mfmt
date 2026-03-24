#include "gen_cmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>
#include <tree_sitter/api.h>

#include "bdb_utils.h"
#include "bdb_log.h"
#include "bdb_fs.h"
#include "bdb_ui.h"
#include "bdb_arr.h"

#include "gen_utils.h"
#include "gen_vars.h"
#include "gen_tree.h"
#include "gen_assign.h"
#include "gen_blocks.h"
#include "gen_emit.h"

#define ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define MEMBER_SIZE(t, a) (sizeof(((t*)0)->a))
extern const TSLanguage *tree_sitter_java(void);

static int bdb__gen_parse_class(
        TSNode node,
        const char *src,
        const bdb__arr_view_t *vds,
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

    if(bdb__gen_scan_ctor(vds, node, src, tn) < 0) 
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
        const bdb__arr_view_t *vds,
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

static int bdb__gen_prepare_paths(
        const char *root,
        char **blocks_dir,
        char **blocks_java)
{
    *blocks_dir  = bdb__fs_cat_path(root, "core/src/mindustry/world/blocks");
    *blocks_java = bdb__fs_cat_path(root, "core/src/mindustry/content/Blocks.java");

    if(!*blocks_dir || !*blocks_java)
        return -1;

    if(!bdb__fs_is_dir(*blocks_dir))
    {
        bdb__info("\"%s\" is not a directory", *blocks_dir);
        return -1;
    }

    if(!bdb__fs_is_file(*blocks_java))
    {
        bdb__info("\"%s\" is not a file", *blocks_java);
        return -1;
    }

    return 0;
}
static int bdb__gen_parse_block_classes(
        TSParser *parser,
        const char *path,
        const bdb__arr_view_t *vds,
        bdb__arr_t *classes
)
{
    bdb__arr_t file_list = { .elem_size = sizeof(void*) };
    char *src = 0;
    long src_len = 0;
    TSTree *tree = 0;

    bdb__info("collecting block files");
    if(bdb__fs_collect_files(path, &file_list) < 0)
        return -1;

    bdb__info("parsing block classes");
    for(size_t i = 0; i < file_list.count; i++)
    {
        char *path = bdb__arr_get(&file_list, char*, i);

        bdb_ui_progress(i + 1, file_list.count);
        if(bdb__read_file(path, &src, &src_len) < 0)
            continue;

        tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
        TSNode root = ts_tree_root_node(tree);

        if(bdb__gen_walk_ast(root, src, vds, classes) < 0)
            bdb__error("failed parsing \"%s\"", path);

        ts_tree_delete(tree);
        tree = 0;

        free(src);
        src = 0;
    }
    bdb__arr_destroy_strings(&file_list);
    bdb_ui_progress_end();

    return 0;
}

static void bdb__gen_resolve_classes(
        bdb__arr_t *classes,
        const bdb__arr_view_t *vds)
{
    bdb__gen_build_tree(classes);
    for(size_t i = 0; i < classes->count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(classes, bdb__gen_tn_t*, i);

        if(tn->parent)
            continue;

        bdb__gen_resolve_tree(tn, vds);
    }
}

static int bdb__gen_parse_blocks_java(
        TSParser *parser,
        const char *path,
        const bdb__arr_view_t *vds,
        bdb__arr_t *classes,
        bdb__arr_t *blocks
)
{
    char *src = 0;
    long src_len = 0;
    TSTree *tree = 0;

    if(bdb__read_file(path, &src, &src_len) < 0)
        return -1;

    tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
    TSNode root = ts_tree_root_node(tree);

    TSNode blocks_node = bdb__gen_find_class(root, src, "Blocks");

    bdb__gen_parse_block_class(blocks_node, src, classes, vds, blocks);

    ts_tree_delete(tree);
    free(src);

    return 0;
}

void bdb_gen_main(void *args)
{
    bdb_gen_args_t *data = args;
    static const bdb_gen_name_fmt_t fmt = 
    {
        .prefix = "msav_block",
        .type_name = "msav_block_t",
        .sep = '_',
        .name_offset = offsetof(bdb__gen_tn_t, name)
    };

    bdb_gen_path_config_t pc = {0};

    const bdb__gen_var_desc_t hb_desc[] = {
        bdb__gen_var_bit("update",       0),
        bdb__gen_var_bit("destructible", 1),
    };
    const bdb__gen_var_desc_t mb_desc[] = {
        bdb__gen_var_bit("hasItems",   0),
        bdb__gen_var_bit("hasPower",   1),
        bdb__gen_var_bit("hasLiquids", 2),
    };
    const bdb__gen_var_desc_t var_desc[] =
    {
        bdb__gen_var_bits(
                "has_building", 
                offsetof(bdb__gen_tn_t, has_building), 
                MEMBER_SIZE(bdb__gen_tn_t, has_building),
                (bdb__arr_view_t){
                    .count = ARRSZ(hb_desc),
                    .elem_size = sizeof(hb_desc[0]),
                    .data = (void*)hb_desc,
                }
        ),
        bdb__gen_var_bits(
                "modules_bitmask", 
                offsetof(bdb__gen_tn_t, modules_bitmask), 
                MEMBER_SIZE(bdb__gen_tn_t, modules_bitmask),
                (bdb__arr_view_t){
                    .count = ARRSZ(mb_desc),
                    .elem_size = sizeof(mb_desc[0]),
                    .data = (void*)mb_desc,
                }
        )
    };
    
    const bdb__arr_view_t vds = {
        .count = ARRSZ(var_desc),
        .elem_size = sizeof(var_desc[0]),
        .data = (void*)var_desc
    };

    const char *mindustry_root = *(const char**)args;
    const char *out_path = *((const char**)args + 1);

    char *blocks_dir_path = 0;
    char *blocks_java_path = 0;

    TSParser *parser = 0;

    bdb__arr_t classes = { .elem_size = sizeof(void*) };
    bdb__arr_t blocks = { .elem_size = sizeof(bdb__gen_tn_t) };

    if(bdb__gen_prepare_paths(
                data->mindustry_root,
                &blocks_dir_path,
                &blocks_java_path
    ) < 0) goto cleanup;

    parser = ts_parser_new();
    if(!parser)
    {
        bdb__error("failed to create tree-sitter parser");
        goto cleanup;
    }
    ts_parser_set_language(parser, tree_sitter_java());
    bdb__gen_link_vars(&vds);
    if(bdb__gen_parse_block_classes(
                parser, 
                blocks_dir_path, 
                &vds, 
                &classes
    ) < 0) goto cleanup;

    bdb__gen_resolve_classes(&classes, &vds);

    if(bdb__gen_parse_blocks_java(
                parser, 
                blocks_java_path, 
                &vds, 
                &classes, 
                &blocks
    ) < 0) goto cleanup;

    pc = (bdb_gen_path_config_t) {
        .root_path   = data->root_path ? data->root_path : ".",
        .include_dir = data->include_dir ? data->include_dir : "include",
        .src_dir     = data->src_dir ? data->src_dir : "src",
        .stem        = data->stem ? data->stem : "msav_blocks_gen",
        .header_ext  = data->header_ext ? data->header_ext : "h",
        .source_ext  = data->source_ext ? data->source_ext : "c"
    };
    bdb_gen_emit_code(&pc, &blocks, &vds, &fmt);

cleanup:
    for(size_t i = 0; i < classes.count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(&classes, bdb__gen_tn_t*, i);
        if(!tn) continue;

        free(tn->name);
        free(tn->parent_name);
        bdb__arr_destroy(&tn->children);
        free(tn);
    }

    if(parser) ts_parser_delete(parser);

    free(blocks_java_path);
    free(blocks_dir_path);

    bdb__arr_destroy(&classes);
    bdb__arr_destroy(&blocks);
}
