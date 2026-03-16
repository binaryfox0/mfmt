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

void bdb__gen_merge_flags(
        const bdb__arr_t *vds,
        bdb__gen_tn_t *child,
        const bdb__gen_tn_t *parent
)
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
