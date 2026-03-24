#ifndef MSAV_BLOCKS_GEN_PRIV_H
#define MSAV_BLOCKS_GEN_PRIV_H

#include "msav_blocks_gen.h"
typedef struct
{
    uint8_t has_building;
    uint8_t modules_bitmask;
} msav_block__data_t;

extern msav_block__data_t msav_block__db[__MSAV_BLOCK_MAX__];

#endif
