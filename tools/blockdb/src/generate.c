#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>

#include <dirent.h>
#include <sys/stat.h>

#include <time.h>
#include <tree_sitter/api.h>

#include "bdb_utils.h"
#include "bdb_log.h"
#include "bdb_arr.h"

#define ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))

static char *bdb__path_cat(const char *a, const char *b)
{
    size_t len_a = 0;
    size_t len_b = 0;
    size_t need_sep = 0;
    size_t len = 0;
    char *out = 0;

    if(a)
        len_a = strlen(a);

    if(b)
        len_b = strlen(b);

    if(len_a && len_b && a[len_a - 1] != '/')
        need_sep = 1;

    len = len_a + need_sep + len_b;

    out = malloc(len);
    if(!out)
        return 0;

    if(len_a)
        memcpy(out, a, len_a);

    if(need_sep)
        out[len_a] = '/';

    if(len_b)
        memcpy(out + len_a + need_sep, b, len_b);

    out[len] = '\0';

    return out;
}

static int bdb__collect_files(const char *path, bdb__arr_t *out)
{
    DIR *dir = 0;
    struct dirent *entry = 0;
    char *full = 0;
    struct stat st = {0};

    dir = opendir(path);
    if(!dir)
    {
        bdb__error_errno("failed to open directory at \"%s\"", path);
        return -1;
    }

    while((entry = readdir(dir)) != 0)
    {
        if(
                !strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, "..")
          ) continue;

        full = bdb__path_cat(path, entry->d_name);
        if(!full)
        {
            bdb__error_errno("out of memory while creating file path");
            continue;
        }

        stat(full, &st);
        if(S_ISDIR(st.st_mode))
        {
            bdb__collect_files(full, out);
            free(full);
        }
        else
        {
            if(bdb__arr_append(out, &full) < 0)
            {
                bdb__error_errno("out of memory while appending file path into array");
                free(full);
                bdb__arr_destroy_strings(out);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

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
)
{
    if(!vds || !str)
        return 0;

    for(size_t i = 0; i < vds->count; i++)
    {
        if(!bdb__tokcmp(
                str, 
                bdb__arr_get(vds, bdb__gen_var_desc_t, i).name, 
                len
        )) {
            bdb__info("found %s", bdb__arr_get(vds, bdb__gen_var_desc_t, i).name);
            return &bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        }
    }

    return 0;
}

static int bdb__gen_handle_assign(
        const bdb__arr_t *vds,
        const char *name,
        size_t name_len,
        TSNode right,
        const char *src,
        bdb__gen_tn_t *tn)
{
    int val = -1;
    uint32_t rs = 0;
    uint32_t re = 0;
    size_t rlen = 0;

    bdb__gen_var_desc_t *vd = 0;
    uint8_t *target = 0;
    uint8_t *mask = 0;
    int byte_offset = 0;
    int bit_offset = 0;

    if(!vds || !name || !tn)
        return -1;

    vd = bdb__gen_var_desc_find(vds, name, name_len);
    if(!vd)
        return 0;

    if(!vd->is_bool)
    {
        bdb__error("variable \"%s\" is not supported", vd->name);
        bdb__info("support may be added in future version");
        return 0; /* unsupported for now */
    }

    rs = ts_node_start_byte(right);
    re = ts_node_end_byte(right);
    rlen = re - rs;

    if(!bdb__tokcmp(src + rs, "true", rlen))
        val = 1;
    else if(!bdb__tokcmp(src + rs, "false", rlen))
        val = 0;
    else
    {
        TSPoint p = {0};
        p = ts_node_start_point(right);
        bdb__error("invalid boolean value at %u, %u", p.row + 1, p.column + 1);
        return -1;
    }

    byte_offset = vd->bit_idx / 8;
    bit_offset = vd->bit_idx % 8;
    target = ((uint8_t*)tn) + vd->out_offset + byte_offset;
    mask = ((uint8_t*)tn) + vd->out_offset + byte_offset + vd->out_size;

    if(val)
        *target |= (1u << bit_offset);
    else
        *target &= ~(1u << bit_offset);
    *mask |= (1u << bit_offset);

    return 0;
}

static int bdb__gen_handle_assign_expr(
        const bdb__arr_t *vds,
        TSNode left,
        TSNode right,
        const char *src,
        bdb__gen_tn_t *tn)
{
    TSNode r = {0};

    if(!vds || !src || !tn)
        return -1;

    if(ts_node_is_null(left) || ts_node_is_null(right))
        return 0;

    r = right;

    /* resolve final RHS value (assignment is right-associative) */
    while(!strcmp(ts_node_type(r), "assignment_expression"))
    {
        TSNode inner_left = {0};
        TSNode inner_right = {0};

        inner_left = ts_node_child_by_field_name(r, "left", 4);
        inner_right = ts_node_child_by_field_name(r, "right", 5);

        if(ts_node_is_null(inner_left) || ts_node_is_null(inner_right))
            break;

        /* recurse to assign inner variable */
        if(bdb__gen_handle_assign_expr(vds, inner_left, inner_right, src, tn) < 0)
            return -1;

        r = inner_right;
    }

    /* assign outer variable using final RHS value */
    if(!strcmp(ts_node_type(left), "identifier"))
    {
        uint32_t s = ts_node_start_byte(left);
        uint32_t e = ts_node_end_byte(left);

        return bdb__gen_handle_assign(vds, src + s, e - s, r, src, tn);
    }
    else if(!strcmp(ts_node_type(left), "field_access"))
    {
        TSNode field = {0};
        uint32_t s = 0;
        uint32_t e = 0;

        field = ts_node_child_by_field_name(left, "field", 5);
        if(ts_node_is_null(field))
            return 0;

        s = ts_node_start_byte(field);
        e = ts_node_end_byte(field);

        return bdb__gen_handle_assign(vds, src + s, e - s, r, src, tn);
    }

    return 0;
}
// Scan a constructor body and handle all assignments
static int bdb__gen_scan_ctor_body(
        TSNode node, 
        const char *src, 
        const bdb__arr_t *vds,
        bdb__gen_tn_t *tn)
{
    uint32_t count = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < count; i++)
    {
        TSNode stmt = ts_node_named_child(node, i);
        if (strcmp(ts_node_type(stmt), "expression_statement"))
            continue;

        TSNode expr = ts_node_named_child(stmt, 0);
        if (strcmp(ts_node_type(expr), "assignment_expression"))
            continue;

        TSNode left  = ts_node_child_by_field_name(expr, "left", 4);
        TSNode right = ts_node_child_by_field_name(expr, "right", 5);

        if (ts_node_is_null(left) || ts_node_is_null(right))
            continue;

        int ret = bdb__gen_handle_assign_expr(vds, left, right, src, tn);
        if (ret < 0)
        {
            bdb__error("failed to process assignment");
            return -1;
        }
    }
    return 0;
}

static int bdb__gen_scan_ctor(
        TSNode node, 
        const char *src, 
        const bdb__arr_t *vds,
        bdb__gen_tn_t *tn
)
{
    TSNode body = {0};
    uint32_t count = 0;

    body = ts_node_child_by_field_name(node, "body", 4);
    if(ts_node_is_null(body))
    {
        bdb__error("non-null class's body node was expected");
        return -1;
    }

    count = ts_node_named_child_count(body);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = {0};
        TSNode cbody = {0};

        child = ts_node_named_child(body, i);

        if(strcmp(ts_node_type(child), "constructor_declaration"))
            continue;

        cbody = ts_node_child_by_field_name(child, "body", 4);

        if(!ts_node_is_null(cbody))
        {
            if(bdb__gen_scan_ctor_body(cbody, src, vds, tn) < 0)
                return -1;
        }
    }
    return 0;
}

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

    bdb__info("%s -> %s, has_building: 0x%02X, modules_bitmask; 0x%02X", 
            tn->name, tn->parent_name, tn->has_building, tn->modules_bitmask);

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

void bdb__gen_build_tree(bdb__arr_t *classes)
{
    for (size_t i = 0; i < classes->count; i++)
    {
        bdb__gen_tn_t *c = bdb__arr_get(classes, bdb__gen_tn_t*, i);
        bdb__gen_tn_t *parent = 0;

        if (!c->parent_name)
            continue;

        for (size_t j = 0; j < classes->count; j++)
        {
            bdb__gen_tn_t *c2 = bdb__arr_get(classes, bdb__gen_tn_t*, j);
            if (!strcmp(c2->name, c->parent_name))
            {
                parent = c2;
                break;
            }
        }

        if (!parent)
        {
            parent = calloc(1, sizeof(*parent));
            if(!parent)
                continue;
            bdb__arr_append(classes, &parent);
            parent->name = strdup(c->parent_name);
            parent->children.elem_size = sizeof(void*);
        }

        c->parent = parent;
        bdb__arr_append(&parent->children, &c);
    }
}

static void bdb__gen_merge_flags(
        const bdb__arr_t *vds,
        bdb__gen_tn_t *child,
        const bdb__gen_tn_t *parent)
{
    if(!vds || !child || !parent)
        return;

    for(size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t *vd = &bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        uint8_t *child_val = 0;
        uint8_t *child_mask = 0;
        const uint8_t *parent_val = 0;

        int byte_offset = 0;
        int bit_offset = 0;
        uint8_t bit_mask = 0;

        if(!vd->is_bool)
        {
            bdb__error("variable \"%s\" is not supported", vd->name);
            bdb__info("support may be added in future version");
            continue;
        }

        byte_offset = vd->bit_idx / 8;
        bit_offset  = vd->bit_idx % 8;

        child_val  = ((uint8_t*)child)  + vd->out_offset + byte_offset;
        child_mask = ((uint8_t*)child)  + vd->out_offset + vd->out_size + byte_offset;
        parent_val = ((uint8_t*)parent) + vd->out_offset + byte_offset;
        bit_mask = 1u << bit_offset;

        if(!(*child_mask & bit_mask))
        {
            if(*parent_val & bit_mask)
                *child_val |= bit_mask;
            else
                *child_val &= ~bit_mask;
        }
    }
}



static void bdb__gen_resolve_tree(
        bdb__gen_tn_t *tn,
        const bdb__arr_t *vds
)
{
    if(tn->parent)
        bdb__gen_merge_flags(vds, tn, tn->parent);

    if(tn->children.count == 0)
    {
        bdb__info("%s -> %s, has_building: 0x%02X, modules_bitmask; 0x%02X", 
                tn->name, tn->parent_name, tn->has_building, tn->modules_bitmask);
        return;
    }

    for(size_t i = 0; i < tn->children.count; i++)
    {
        bdb__gen_tn_t *c = bdb__arr_get(&tn->children, bdb__gen_tn_t*, i);
        bdb__gen_resolve_tree(c, vds);
    }
}

extern const TSLanguage *tree_sitter_java(void);
void generate_command(void *args)
{
    const char *mindustry_root = *(const char**)args;
    char *blocks_class_path = 0;
    char *blocks_java_path = 0;

    bdb__arr_t file_list = { .elem_size = sizeof(void*) };
    TSParser *parser = 0;
    bdb__arr_t classes = { .elem_size = sizeof(void*) };
    bdb__arr_t vds = {
        .count = ARRSZ(bdb__queried_vars),
        .capacity = ARRSZ(bdb__queried_vars),
        .elem_size = sizeof(bdb__queried_vars[0]),
        .data = (void*)bdb__queried_vars
    };

    blocks_class_path = bdb__path_cat(mindustry_root, "core/src/mindustry/world/blocks");  

    if(bdb__collect_files(blocks_class_path, &file_list) < 0)
    {
        free(blocks_class_path);
        return;
    }
    free(blocks_class_path);

    parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());

    for(size_t i = 0; i < file_list.count; i++)
    {
        char *path = 0;
        char *src = 0; 
        long src_len = 0;

        TSTree *tree = 0;
        TSNode node = {0};
    
        path = bdb__arr_get(&file_list, char*, i);
        //info("parsing \"%s\"", path);
        if(bdb__read_file(path, &src, &src_len) < 0)
            continue;

        tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
       
        node = ts_tree_root_node(tree);
        if(bdb__gen_walk_ast(node, src, &vds, &classes) < 0)
        {
            bdb__error("an error was found while parsing \"%s\", skipped.", path);
            continue;
        }

        ts_tree_delete(tree);
        free(src);
    }

    ts_parser_delete(parser);
    bdb__arr_destroy_strings(&file_list);

    bdb__gen_build_tree(&classes);
    for(size_t i = 0; i < classes.count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(&classes, bdb__gen_tn_t*, i);
        if(tn->parent)
            continue;
        bdb__gen_resolve_tree(tn, &vds);
    }

    for(size_t i = 0; i < classes.count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(&classes, bdb__gen_tn_t*, i);
        free(tn->name);
        free(tn->parent_name);
        bdb__arr_destroy(&tn->children);
        free(tn);
    }
    
    blocks_java_path = bdb__path_cat(mindustry_root, "core/src/mindustry/content/Blocks.java");  

    bdb__arr_destroy(&classes);
}
