#include "gen_assign.h"

#include "bdb_log.h"
#include "bdb_utils.h"
#include "gen_vars.h"

int bdb__gen_handle_assign(
        const bdb__arr_t *vds,
        const char *name,
        size_t name_len,
        TSNode right,
        const char *src,
        bdb__gen_tn_t *tn
)
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

int bdb__gen_handle_assign_expr(
        const bdb__arr_t *vds,
        TSNode left,
        TSNode right,
        const char *src,
        bdb__gen_tn_t *tn
)
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

int bdb__gen_scan_ctor_body(
        TSNode node, 
        const char *src, 
        const bdb__arr_t *vds,
        bdb__gen_tn_t *tn
)
{
    uint32_t count = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < count; i++)
    {
        TSNode stmt = {0};
        TSNode expr = {0};
        TSNode left = {0};
        TSNode right = {0};
        int ret = 0;

        stmt = ts_node_named_child(node, i);
        if (strcmp(ts_node_type(stmt), "expression_statement"))
            continue;

        expr = ts_node_named_child(stmt, 0);
        if (strcmp(ts_node_type(expr), "assignment_expression"))
            continue;

        left  = ts_node_child_by_field_name(expr, "left", 4);
        right = ts_node_child_by_field_name(expr, "right", 5);

        if (ts_node_is_null(left) || ts_node_is_null(right))
            continue;

        ret = bdb__gen_handle_assign_expr(vds, left, right, src, tn);
        if (ret < 0)
        {
            bdb__error("failed to process assignment");
            return -1;
        }
    }
    return 0;
}

int bdb__gen_scan_ctor(
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
