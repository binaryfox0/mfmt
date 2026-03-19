#include "gen_blocks.h"

#include "bdb_log.h"
#include "gen_tree.h"
#include "gen_assign.h"
#include "gen_utils.h"

#define BDB_NOT_FOUND (size_t)-1
size_t bdb__gen_find_variables(
        TSNode identifier,
        const char *src,
        const bdb__arr_t *blocks)
{
    uint32_t s = 0;
    uint32_t e = 0;

    if(!src || !blocks)
        return BDB_NOT_FOUND;

    if(strcmp(ts_node_type(identifier), "identifier"))
        return BDB_NOT_FOUND;
            
    s = ts_node_start_byte(identifier);
    e = ts_node_end_byte(identifier);

    for(size_t i = 0; i < blocks->count; i++)
    {
        if(!bdb__tokcmp(src + s, 
                    bdb__arr_get(blocks, bdb__gen_tn_t, i).name, 
                    (size_t)(e - s)))
            return i;
    }
    bdb__error("identifier \"%*s\" was not found in declarators", (int)(e - s), src + s);
    return BDB_NOT_FOUND;
}

bdb__gen_tn_t* bdb__gen_find_tn(
        const bdb__arr_t *classes,
        const char *str,
        const size_t len
)
{
    if(!classes || !len)
        return 0;

    for(size_t i = 0; i < classes->count; i++)
    {
        bdb__gen_tn_t *tn = bdb__arr_get(classes, bdb__gen_tn_t*, i);
        if(!bdb__tokcmp(str, tn->name, len))
            return tn;
    }
    return 0;
}

TSNode bdb__gen_node_child_from_type(
        TSNode node,
        const char *type
)
{
    uint32_t count = 0;
    if(ts_node_is_null(node) || !type)
        return (TSNode){0};
    count = ts_node_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = ts_node_child(node, i);
        if(ts_node_is_null(child))
            continue;
        if(!strcmp(ts_node_type(child), type))
            return child;
    }
    return (TSNode){0};
}

static int bdb__gen_collect_block_variables(
        TSNode field_decl,
        const char *src,
        bdb__arr_t *instances
)
{
    uint32_t count = 0;

    if (ts_node_is_null(field_decl) || !src || !instances)
        return -1;

    count = ts_node_named_child_count(field_decl);
    for (uint32_t i = 0; i < count; i++)
    {
        TSNode declarator = {0};
        bdb__gen_tn_t inst = {0};
        uint32_t s = 0; 
        uint32_t e = 0;
        char *name = 0;

        declarator = ts_node_named_child(field_decl, i);
        if (strcmp(ts_node_type(declarator), "variable_declarator"))
            continue;

        TSNode name_node =
            ts_node_child_by_field_name(declarator, "name", 4);

        if (ts_node_is_null(name_node))
            continue;

        s = ts_node_start_byte(name_node);
        e = ts_node_end_byte(name_node);
        size_t len = (size_t)(e - s);

        name = bdb__tokdup(src + s, len);
        if (!name)
        {
            bdb__error("failed to allocate variable name");
            return -1;
        }

        inst.name = name;
        if (bdb__arr_append(instances, &inst) < 0)
        {
            free(name);
            return -1;
        }
    }

    return 0;
}

int bdb__gen_parse_anonymous_class_body(
        TSNode class_body,
        const char *src,
        const bdb__arr_t *vds, /* bdb__gen_var_desc_t */
        const bdb__gen_tn_t *base_type,
        bdb__gen_tn_t *instance
)
{
    TSNode ctor_block = {0};

    if (ts_node_is_null(class_body) || !src || !vds || !base_type || !instance)
        return -1;

    ctor_block = bdb__gen_node_child_from_type(class_body, "block");
    if (ts_node_is_null(ctor_block))
    {
        bdb__error("anonymous class missing constructor block");
        return -1;
    }

    if (bdb__gen_scan_ctor_body(ctor_block, src, vds, instance) < 0)
        return -1;

    if (bdb__gen_merge_flags(vds, instance, base_type) < 0)
        return -1;

    return 0;
}

int bdb__gen_parse_object_creation(
        TSNode node,
        const char *src,
        const bdb__arr_t *classes, /* bdb__gen_tn_t* */
        const bdb__arr_t *vds, /* bdb__gen_var_desc_t */
        size_t var_idx,
        bdb__arr_t *instances
)
{
    uint32_t count = 0;
    bdb__gen_tn_t *base_type = 0;

    if (ts_node_is_null(node) || !src || !classes || !vds || !instances)
        return -1;

    count = ts_node_child_count(node);

    for (uint32_t i = 0; i < count; i++)
    {
        TSNode child = ts_node_child(node, i);
        if (ts_node_is_null(child))
            continue;

        const char *type = ts_node_type(child);

        if (!strcmp(type, "type_identifier"))
        {
            uint32_t s = ts_node_start_byte(child);
            uint32_t e = ts_node_end_byte(child);

            base_type = bdb__gen_find_tn(classes, src + s, (size_t)(e - s));
            if (!base_type)
            {
                bdb__error("type \"%.*s\" not found",
                           (int)(e - s), src + s);
                return -1;
            }
        }
        else if (!strcmp(type, "class_body"))
        {
            if (!base_type)
            {
                bdb__error("class_body without type_identifier");
                return -1;
            }

            bdb__gen_tn_t *instance =
                &bdb__arr_get(instances, bdb__gen_tn_t, var_idx);

            if (bdb__gen_parse_anonymous_class_body(
                    child, src, vds, base_type, instance) < 0)
                return -1;
        }
    }

    return 0;
}

int bdb__gen_parse_load_method(
        TSNode method,
        const char *src,
        const bdb__arr_t *classes, /* bdb__gen_tn_t* */
        const bdb__arr_t *vds, /* bdb__gen_var_desc_t */
        bdb__arr_t *instances /* bdb__gen_tn_t */
)
{
    TSNode body = {0};
    uint32_t count = 0;

    if (ts_node_is_null(method) || !src || !classes || !vds || !instances)
        return -1;

    body = ts_node_child_by_field_name(method, "body", 4);
    if (ts_node_is_null(body))
    {
        bdb__error("method missing body");
        return -1;
    }

    count = ts_node_child_count(body);

    for (uint32_t i = 0; i < count; i++)
    {
        TSNode stmt = ts_node_named_child(body, i);
        if (ts_node_is_null(stmt) ||
            strcmp(ts_node_type(stmt), "expression_statement"))
            continue;

        TSNode expr = ts_node_child(stmt, 0);
        if (ts_node_is_null(expr) ||
            strcmp(ts_node_type(expr), "assignment_expression"))
            continue;

        TSNode left  = ts_node_child_by_field_name(expr, "left", 4);
        TSNode right = ts_node_child_by_field_name(expr, "right", 5);

        if (ts_node_is_null(left) || ts_node_is_null(right))
            continue;

        if (strcmp(ts_node_type(right), "object_creation_expression"))
            continue;

        size_t var_idx = bdb__gen_find_variables(left, src, instances);
        if (var_idx == BDB_NOT_FOUND)
        {
            uint32_t s = ts_node_start_byte(left);
            uint32_t e = ts_node_end_byte(left);

            bdb__error("variable \"%.*s\" not found",
                       (int)(e - s), src + s);
            continue; /* recoverable */
        }

        if (bdb__gen_parse_object_creation(
                right, src, classes, vds, var_idx, instances) < 0)
        {
            bdb__error("failed to parse object creation");
            return -1; /* propagate */
        }
    }

    return 0;
}

int bdb__gen_parse_block_class(
        TSNode class_node,
        const char *src,
        const bdb__arr_t *classes,
        bdb__arr_t *vds,
        bdb__arr_t *instances
)
{
    TSNode body = {0};
    uint32_t count = 0;

    if (ts_node_is_null(class_node) || !src || !classes || !vds || !instances)
        return -1;

    body = ts_node_child_by_field_name(class_node, "body", 4);
    if (ts_node_is_null(body))
        return -1;

    count = ts_node_named_child_count(body);

    for (uint32_t i = 0; i < count; i++)
    {
        TSNode node = ts_node_named_child(body, i);
        const char *type = ts_node_type(node);

        if (!strcmp(type, "field_declaration"))
        {
            if (bdb__gen_collect_block_variables(node, src, instances) < 0)
                return -1;
        }
        else if (!strcmp(type, "method_declaration"))
        {
            TSNode name_node =
                ts_node_child_by_field_name(node, "name", 4);

            if (ts_node_is_null(name_node))
            {
                bdb__error("method missing name");
                continue;
            }

            uint32_t s = ts_node_start_byte(name_node);
            uint32_t e = ts_node_end_byte(name_node);

            if (!bdb__tokcmp(src + s, "load", (size_t)(e - s)))
            {
                if (bdb__gen_parse_load_method(
                        node, src, classes, vds, instances) < 0)
                    return -1;
            }
        }
    }

    return 0;
}
