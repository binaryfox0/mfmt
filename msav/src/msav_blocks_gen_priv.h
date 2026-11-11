#ifndef MSAV_BLOCKS_GEN_PRIV_H
#define MSAV_BLOCKS_GEN_PRIV_H

#include "msav_blocks_gen.h"

typedef struct
{
    uint8_t has_building;
    uint8_t modules_bitmask;
} msav__block_data_t;

extern msav__block_data_t msav__block_db[MSAV_BLOCK_MAX];

#endif
