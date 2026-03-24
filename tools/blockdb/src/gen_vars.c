#include "gen_vars.h"

#include "bdb_log.h"
#include "bdb_arr.h"
#include "gen_utils.h"

int bdb__gen_link_vars(
        const bdb__arr_view_t *vds
)
{
    if(!vds)
        return -1;

    for(size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t *vd = &bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        if(vd->kind != BDB__GEN_VAR_BITS)
            continue;

        if(vd->children.count == 0 || !vd->children.data)
        {
            bdb__warn("\"%s\" have no children", vd->name);
            continue;
        }

        for(size_t j = 0; j < vd->children.count; j++)
        {
            bdb__gen_var_desc_t *child = &bdb__arr_get(&vd->children, bdb__gen_var_desc_t, j);
            if(child->kind != BDB__GEN_VAR_BIT)
            {
                bdb__error("expected \"%s\" to be bit", child->name);
                return -1;
            }
            if(child->offset < 0 || child->offset >= vd->size * 8)
            {
                bdb__error("invalid bit offset: %d, min: 0, max: %d", child->offset, vd->size * 8 - 1);
                return -1;
            }
            child->parent = vd;
        }
    }
    return 0;
}

bdb__gen_var_desc_t* bdb__gen_var_desc_find(
        const bdb__arr_view_t *vds, 
        const char *str, 
        const size_t len
)
{
    if(!vds || !str)
        return 0;

    for(size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t vd = bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        if(vd.kind == BDB__GEN_VAR_BITS)
        {
            bdb__gen_var_desc_t *found = bdb__gen_var_desc_find(&vd.children, str, len);
            if(found)
                return found;
        } else {
            if(!bdb__tokcmp(
                    str, 
                    vd.name, 
                    len
            )) {
                return &bdb__arr_get(vds, bdb__gen_var_desc_t, i);
            }
        }
    }

    return 0;
}
