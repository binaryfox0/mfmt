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
        const bdb__arr_view_t *vds,
        void *child,
        const void *parent
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

        if (vd->kind != BDB__GEN_VAR_BITS)
        {
            bdb__error("variable \"%s\" is not supported", vd->name);
            continue;
        }

        for(size_t j = 0; j < vd->children.count; j++)
        {
            bdb__gen_var_desc_t *cn =
                &bdb__arr_get(&vd->children, bdb__gen_var_desc_t, j);
            /* bit location */
            int byte_off = 0;
            int bit_off = 0;
            uint8_t mask = 0;
            uint8_t *child_val = 0;
            uint8_t *child_written = 0;
            const uint8_t *parent_val = 0;

            byte_off = cn->offset >> 3;
            bit_off  = cn->offset & 7;
            mask = (uint8_t)(1u << bit_off);
            /* layout:
               [value bytes][mask bytes]
            */
            child_val =
                child_base + cn->parent->offset + byte_off;
            child_written =
                child_val + cn->parent->size;

            parent_val =
                parent_base + cn->parent->offset + byte_off;

            /* skip if child explicitly set */
            if (*child_written & mask)
                continue;

            /* inherit from parent */
            if (*parent_val & mask)
                *child_val |= mask;
            else
                *child_val &= (uint8_t)~mask;
        }
    }

    return 0;
}

void bdb__gen_resolve_tree(
        bdb__gen_tn_t *tn,
        const bdb__arr_view_t *vds
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
