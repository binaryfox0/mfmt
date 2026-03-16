#include "gen_vars.h"

#include "bdb_utils.h"

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
            return &bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        }
    }

    return 0;
}
