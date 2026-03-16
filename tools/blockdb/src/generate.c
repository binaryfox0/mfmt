#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

#include <time.h>
#include <tree_sitter/api.h>

#include "bdb_utils.h"
#include "bdb_log.h"
#include "bdb_fs.h"
#include "bdb_arr.h"

#include "gen_vars.h"
#include "gen_tree.h"
#include "gen_assign.h"

#define ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))


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

/* find class_declaration with given name */
static TSNode bdb__gen_find_class(
        TSNode node,
        const char *src,
        const char *class_name)
{
    uint32_t count = 0;

    if(!src || !class_name)
        return (TSNode){0};

    if(!strcmp(ts_node_type(node), "class_declaration"))
    {
        TSNode name_node = {0};
        uint32_t s = 0;
        uint32_t e = 0;

        name_node = ts_node_child_by_field_name(node, "name", 4);
        if(!ts_node_is_null(name_node))
        {
            s = ts_node_start_byte(name_node);
            e = ts_node_end_byte(name_node);

            if(!bdb__tokcmp(src + s, class_name, (size_t)(e - s)))
                return node;
        }
    }

    count = ts_node_child_count(node);

    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = {0};
        TSNode found = {0};

        child = ts_node_child(node, i);
        found = bdb__gen_find_class(child, src, class_name);

        if(!ts_node_is_null(found))
            return found;
    }

    return (TSNode){0};
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

void bdb__gen_process_block_names(
        TSNode node, 
        const char *src,
        const char *prefix,
        bdb__arr_t *names
)
{
    uint32_t count = 0;
    if(ts_node_is_null(node))
        return;
        
    count = ts_node_named_child_count(node);

    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = ts_node_named_child(node, i);
        uint32_t s = 0; 
        uint32_t e = 0;
        char *dup_name = 0;

        if(strcmp(ts_node_type(child), "variable_declarator"))
            continue;

        TSNode name_node = ts_node_child_by_field_name(child, "name", 4);
        if(ts_node_is_null(name_node))
            continue;

        s = ts_node_start_byte(name_node);
        e = ts_node_end_byte(name_node);

        dup_name = bdb__tokdup(src + s, (size_t)(e - s));
        if(!dup_name)
            continue;
        bdb__arr_append(names, &dup_name);
    }
}

int bdb__gen_check_existence(
        TSNode identifier,
        const char *src,
        const bdb__arr_t *names)
{
    uint32_t s = 0;
    uint32_t e = 0;

    if(!src || !names)
        return 0;

    if(strcmp(ts_node_type(identifier), "identifier"))
        return 0;
            
    s = ts_node_start_byte(identifier);
    e = ts_node_end_byte(identifier);
    
    for(size_t i = 0; i < names->count; i++)
    {
        if(bdb__tokcmp(src + s, bdb__arr_get(names, char*, i), (size_t)(e - s)))
            return 1;
    }
    bdb__error("identifier \"%*s\" was not found in declarators", (int)(e - s), src + s);
    return 0;
}

size_t bdb__gen_find_tn(
        const bdb__arr_t *classes,
        const char *str,
        const size_t len
)
{
    if(!classes || !len)
        return classes->count;

    for(size_t i = 0; i < classes->count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(classes, bdb__gen_tn_t*, i);
        if(!bdb__tokcmp(str, tn->name, len))
            return i;
    }
    return classes->count;
}

int bdb__gen_handle_object_creation(
        TSNode node,
        const char *src,
        const bdb__arr_t *classes,
        bdb__arr_t *attribs

)
{
    TSNode type_node = {0};
    uint32_t s = 0;
    uint32_t e = 0;
    size_t found_idx = 0;

    if(ts_node_is_null(node) || strcmp(ts_node_type(node), "object_creation_expression"))
        return -1;
    
    type_node = ts_node_child_by_field_name(node, "type", 4);
    
    s = ts_node_start_byte(type_node);
    e = ts_node_end_byte(type_node);
    found_idx = bdb__gen_find_tn(classes, src + s, (size_t)(e - s));
    if(found_idx == classes->count)
    {
        bdb__error("type \"%.*s\" was not found in type list", (int)(e - s), src + s);
        return -1;
    }
    
    bdb__info("type \"%.*s\" was found at %zu", (int)(e - s), src + s, found_idx);
    return 0;
}

void bdb__gen_process_load_method(
        TSNode node,
        const char *src,
        bdb__arr_t *names, /* char* */
        const bdb__arr_t *classes, /* bdb__gen_tn_t* */
        bdb__arr_t *attribs
)
{
    TSNode body = {0};
    uint32_t count = 0;

    body = ts_node_child_by_field_name(node, "body", 4);
    if(ts_node_is_null(body))
    {
        bdb__error("non-null body node was expected from method_declaration");
        return;
    }

    count = ts_node_child_count(body);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode stmt = {0};
        TSNode expr = {0};
        TSNode left = {0};
        TSNode right = {0};
        bdb__gen_tn_t *found = 0;
        
        stmt = ts_node_named_child(body, i);
        if(ts_node_is_null(stmt) || strcmp(ts_node_type(stmt), "expression_statement"))
            continue;

        expr = ts_node_child(stmt, 0);
        if(ts_node_is_null(expr) || strcmp(ts_node_type(expr), "assignment_expression"))
            continue;

        left = ts_node_child_by_field_name(expr, "left", 4);
        right = ts_node_child_by_field_name(expr, "right", 5);
        if(ts_node_is_null(left) || ts_node_is_null(right))
            continue;

        if(!bdb__gen_check_existence(left, src, names))
        {
            uint32_t s = 0;
            uint32_t e = 0;
    
            s = ts_node_start_byte(left);
            e = ts_node_end_byte(left);
            bdb__error("variable \"%*s\" was not found in variable list", (int)(e - s), src + s);

            continue;
        }

        if(!bdb__gen_handle_object_creation(right, src, classes, attribs))
        {
        }

    }
}

void bdb__gen_process_block_fields(
        TSNode class_node,
        const char *src,
        const char *prefix,
        bdb__arr_t *names,
        const bdb__arr_t *classes,
        bdb__arr_t *attribs)
{
    TSNode body = {0};
    uint32_t count = 0;

    if(ts_node_is_null(class_node))
        return;

    body = ts_node_child_by_field_name(class_node, "body", 4);
    if(ts_node_is_null(body))
        return;

    count = ts_node_named_child_count(body);

    for(uint32_t i = 0; i < count; i++)
    {
        TSNode field = ts_node_named_child(body, i);

        if(!strcmp(ts_node_type(field), "field_declaration"))
            bdb__gen_process_block_names(field, src, prefix, names);
        if(!strcmp(ts_node_type(field), "method_declaration"))
        {
            TSNode name_node = {0};
            uint32_t s = 0;
            uint32_t e = 0;

            name_node = ts_node_child_by_field_name(field, "name", 4); 
            if(ts_node_is_null(name_node))
            {
                bdb__error("expected non-null name node from method_declaration");
                continue;
            }
            
            s = ts_node_start_byte(name_node);
            e = ts_node_end_byte(name_node);

            if(!bdb__tokcmp(src + s, "load", (size_t)(e - s)))
                bdb__gen_process_load_method(field, src, names, classes, attribs);
        }

    }
}
    
extern const TSLanguage *tree_sitter_java(void);
void generate_command(void *args)
{
    int ret = 0;
    const char *mindustry_root = *(const char**)args;
    char *blocks_dir_path = 0;
    char *blocks_java_path = 0;

    bdb__arr_t file_list = { .elem_size = sizeof(void*) };
    char *src = 0; 
    long src_len = 0;

    TSParser *parser = 0;
    TSTree *tree = 0;
    TSNode node = {0};

    bdb__arr_t classes = { .elem_size = sizeof(void*) };
    bdb__arr_t vds = {
        .count = ARRSZ(bdb__queried_vars),
        .capacity = ARRSZ(bdb__queried_vars),
        .elem_size = sizeof(bdb__queried_vars[0]),
        .data = (void*)bdb__queried_vars
    };

    TSNode blocks_node = {0};
    bdb__arr_t block_names = { .elem_size = sizeof(void*) };

    blocks_dir_path = bdb__fs_cat_path(mindustry_root, "core/src/mindustry/world/blocks");  
    blocks_java_path = bdb__fs_cat_path(mindustry_root, "core/src/mindustry/content/Blocks.java");
    if(!blocks_dir_path || !blocks_java_path)
    {
        free(blocks_java_path);
        free(blocks_dir_path);
        return;
    }

    if(!bdb__fs_is_dir(blocks_dir_path)) {
        bdb__info("\"%s\" was not pointed to a directory", blocks_dir_path);
        ret = -1;
    }

    if(!bdb__fs_is_file(blocks_java_path)) {
        bdb__info("\"%s\" was not pointed to a file", blocks_java_path);
        ret = -1;
    }

    if(ret < 0)
    {
        free(blocks_java_path);
        free(blocks_dir_path);
        return;
    }

    ret = bdb__fs_collect_files(blocks_dir_path, &file_list);
    free(blocks_dir_path);
    if(ret < 0)
        return;

    parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());

    for(size_t i = 0; i < file_list.count; i++)
    {
        char *path = bdb__arr_get(&file_list, char*, i);
        //info("parsing \"%s\"", path);
        ret = bdb__read_file(path, &src, &src_len);
        if(ret < 0)
            continue;

        tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);     
        node = ts_tree_root_node(tree);
        ret = bdb__gen_walk_ast(node, src, &vds, &classes);
        if(ret < 0)
        {
            bdb__error("an error was found while parsing \"%s\", skipped.", path);
            continue;
        }

        ts_tree_delete(tree);
        free(src);
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

    
    ret = bdb__read_file(blocks_java_path, &src, &src_len);
    free(blocks_java_path);
    if(ret < 0)
        return;

    tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
    node = ts_tree_root_node(tree);

    blocks_node = bdb__gen_find_class(node, src, "Blocks");
    bdb__gen_process_block_fields(blocks_node, src, "MSAV_BLOCK_", &block_names, &classes, 0);
    
    for(size_t i = 0; i < block_names.count; i++)
        bdb__info("%s", bdb__arr_get(&block_names, char*, i));

    bdb__arr_destroy_strings(&block_names);
    
    for(size_t i = 0; i < classes.count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(&classes, bdb__gen_tn_t*, i);
        free(tn->name);
        free(tn->parent_name);
        bdb__arr_destroy(&tn->children);
        free(tn);
    }

    ts_tree_delete(tree);
    ts_parser_delete(parser);
    free(src);
    bdb__arr_destroy(&classes);
}
