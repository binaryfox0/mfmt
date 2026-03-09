#include "msav_blocks.h"

#include <stdint.h>

enum
{
    MSAV__MODULE_ITEM           = (1 << 0),
    MSAV__MODULE_POWER          = (1 << 1),
    MSAV__MODULE_LIQUID         = (1 << 2),
    MSAV__MODULE_CONSUME        = (1 << 3), /* compatibility, */
    MSAV__MODULE_TIME_SCALE     = (1 << 4),
    MSAV__MODULE_LAST_DISABLER  = (1 << 5)
};

typedef enum
{
    MSAV__BOOL_UPDATE = (1 << 0),
    MSAV__BOOL_DESTRUCTIBLE = (1 << 1)
} msav__db_bool_t;



struct
{
} msav__blocks_db[__MSAV_BLOCK_END__] = 
{
};
