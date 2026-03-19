#include "gen_tree.h"

#include <stdlib.h>
#include <string.h>

#include "bdb_log.h"
#include "gen_vars.h"

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

int bdb__gen_merge_flags(
        const bdb__arr_t *vds,
        bdb__gen_tn_t *child,
        const bdb__gen_tn_t *parent
)
{
    if (!vds || !child || !parent)
        return -1;

    uint8_t *child_base  = (uint8_t *)child;
    const uint8_t *parent_base = (const uint8_t *)parent;

    for (size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t *vd =
            &bdb__arr_get(vds, bdb__gen_var_desc_t, i);

        if (!vd->is_bool)
        {
            bdb__error("variable \"%s\" is not supported", vd->name);
            continue;
        }

        /* bit location */
        const int byte_off = vd->bit_idx >> 3;
        const int bit_off  = vd->bit_idx & 7;
        const uint8_t mask = (uint8_t)(1u << bit_off);

        /* layout:
           [value bytes][mask bytes]
        */
        uint8_t *child_val =
            child_base + vd->out_offset + byte_off;

        uint8_t *child_written =
            child_base + vd->out_offset + vd->out_size + byte_off;

        const uint8_t *parent_val =
            parent_base + vd->out_offset + byte_off;

        /* skip if child explicitly set */
        if (*child_written & mask)
            continue;

        /* inherit from parent */
        if (*parent_val & mask)
            *child_val |= mask;
        else
            *child_val &= (uint8_t)~mask;
    }

    return 0;
}

void bdb__gen_resolve_tree(
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
