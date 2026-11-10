#include "msav_blocks_gen.h"
#include <stdint.h>
#include "msav_blocks_gen_priv.h"

msav_block__data_t msav_block__db[__MSAV_BLOCK_MAX__] =
{
    [MSAV_BLOCK_AIR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SPAWN] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REMOVE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REMOVE_ORE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CLIFF] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DEEPWATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TAINTED_WATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DEEP_TAINTED_WATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TAR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SLAG] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYOFLUID] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRATERS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CHARR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SAND] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARKSAND] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DIRT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MUD] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ICE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SNOW] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARKSAND_TAINTED_WATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SPACE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_EMPTY] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DACITE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RHYOLITE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RHYOLITE_CRATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ROUGH_RHYOLITE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REGOLITH] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_YELLOW_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_ICE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DENSE_RED_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ARKYCITE_FLOOR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ARKYIC_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REDMAT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BLUEMAT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_STONE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DIRT_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SPORE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ICE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DACITE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SPORE_PINE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SNOW_PINE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PINE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHRUBS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WHITE_TREE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WHITE_TREE_DEAD] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SPORE_CLUSTER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REDWEED] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PURBUSH] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_YELLOW_CORAL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RHYOLITE_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBON_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ARKYIC_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_YELLOW_STONE_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_STONE_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTALLINE_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_STONE_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BASALT_VENT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REGOLITH_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_YELLOW_STONE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RHYOLITE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBON_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_ICE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_FERRIC_STONE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BERYLLIC_STONE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ARKYIC_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTALLINE_STONE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_STONE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_DIAMOND_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_FERRIC_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_FERRIC_CRATERS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBON_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BERYLLIC_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTALLINE_STONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTAL_FLOOR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_YELLOW_STONE_PLATES] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ICE_SNOW] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SAND_WATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARKSAND_WATER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DUNE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SAND_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MOSS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SPORE_MOSS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHALE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHALE_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_GRASS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SALT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CORE_ZONE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHALE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SAND_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DACITE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SNOW_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BASALT_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBON_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_FERRIC_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BERYLLIC_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_YELLOW_STONE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ARKYIC_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTAL_CLUSTER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_VIBRANT_CRYSTAL_CLUSTER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTAL_BLOCKS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTAL_ORBS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRYSTALLINE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_ICE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RHYOLITE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RED_STONE_BOULDER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_FLOOR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_FLOOR_DAMAGED] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_FLOOR2] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_FLOOR3] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_FLOOR4] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_FLOOR5] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BASALT] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MAGMAROCK] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_HOTROCK] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SNOW_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SALT_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_PANEL1] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_PANEL2] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_PANEL3] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_PANEL4] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_PANEL5] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_PANEL6] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DARK_METAL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES1] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES2] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES3] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES4] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES5] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES6] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES7] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES8] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES9] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES10] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES11] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES12] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_TILES13] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_WALL1] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_WALL2] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_METAL_WALL3] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_COLORED_FLOOR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_COLORED_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CHARACTER_OVERLAY_GRAY] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CHARACTER_OVERLAY_WHITE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RUNE_OVERLAY] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CRUX_RUNE_OVERLAY] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PEBBLES] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TENDRILS] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_COPPER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_LEAD] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_SCRAP] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_COAL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_TITANIUM] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_THORIUM] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_BERYLLIUM] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_TUNGSTEN] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ORE_CRYSTAL_THORIUM] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WALL_ORE_THORIUM] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WALL_ORE_BERYLLIUM] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_GRAPHITIC_WALL] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WALL_ORE_GRAPHITE] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WALL_ORE_TUNGSTEN] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SILICON_SMELTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SILICON_CRUCIBLE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_KILN] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_GRAPHITE_PRESS] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_PLASTANIUM_COMPRESSOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_MULTI_PRESS] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_PHASE_WEAVER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SURGE_SMELTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_PYRATITE_MIXER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_BLAST_MIXER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_CRYOFLUID_MIXER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_MELTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_SEPARATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_DISASSEMBLER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_SPORE_PRESS] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_PULVERIZER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_INCINERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_COAL_CENTRIFUGE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_SILICON_ARC_FURNACE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_ELECTROLYZER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_OXIDATION_CHAMBER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_ATMOSPHERIC_CONCENTRATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x05
    },

    [MSAV_BLOCK_ELECTRIC_HEATER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SLAG_HEATER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_PHASE_HEATER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_HEAT_REDIRECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SMALL_HEAT_REDIRECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_HEAT_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SLAG_INCINERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBIDE_CRUCIBLE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SLAG_CENTRIFUGE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SURGE_CRUCIBLE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CYANOGEN_SYNTHESIZER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_PHASE_SYNTHESIZER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_HEAT_REACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_POWER_SOURCE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_POWER_VOID] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_ITEM_SOURCE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_ITEM_VOID] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_LIQUID_SOURCE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LIQUID_VOID] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_PAYLOAD_SOURCE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PAYLOAD_VOID] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ILLUMINATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_HEAT_SOURCE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_COPPER_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_COPPER_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TITANIUM_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TITANIUM_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PLASTANIUM_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PLASTANIUM_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_THORIUM_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_THORIUM_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DOOR] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_DOOR_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PHASE_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PHASE_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SURGE_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SURGE_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SCRAP_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SCRAP_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SCRAP_WALL_HUGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SCRAP_WALL_GIGANTIC] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_THRUSTER] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BERYLLIUM_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BERYLLIUM_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TUNGSTEN_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TUNGSTEN_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BLAST_DOOR] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REINFORCED_SURGE_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REINFORCED_SURGE_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBIDE_WALL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CARBIDE_WALL_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHIELDED_WALL] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_MENDER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_MEND_PROJECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_OVERDRIVE_PROJECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_OVERDRIVE_DOME] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_FORCE_PROJECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_SHOCK_MINE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TARGET_DUMMY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_RADAR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_BUILD_TOWER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REGEN_PROJECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SHOCKWAVE_TOWER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHIELD_PROJECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_LARGE_SHIELD_PROJECTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_SHIELD_BREAKER] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_TITANIUM_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_PLASTANIUM_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_ARMORED_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DISTRIBUTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_JUNCTION] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ITEM_BRIDGE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_PHASE_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SORTER] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_INVERTED_SORTER] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_OVERFLOW_GATE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_UNDERFLOW_GATE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_UNLOADER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_MASS_DRIVER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_DUCT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_ARMORED_DUCT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DUCT_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_OVERFLOW_DUCT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_UNDERFLOW_DUCT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DUCT_BRIDGE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DUCT_UNLOADER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SURGE_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SURGE_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_UNIT_CARGO_LOADER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_UNIT_CARGO_UNLOAD_POINT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_MECHANICAL_PUMP] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_ROTARY_PUMP] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_IMPULSE_PUMP] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_PULSE_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_PLATED_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LIQUID_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LIQUID_CONTAINER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LIQUID_TANK] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LIQUID_JUNCTION] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_BRIDGE_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_PHASE_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_REINFORCED_PUMP] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_REINFORCED_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_REINFORCED_LIQUID_JUNCTION] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_REINFORCED_BRIDGE_CONDUIT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_REINFORCED_LIQUID_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_REINFORCED_LIQUID_CONTAINER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_REINFORCED_LIQUID_TANK] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_COMBUSTION_GENERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_THERMAL_GENERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_STEAM_GENERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_DIFFERENTIAL_GENERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_RTG_GENERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_SOLAR_PANEL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_LARGE_SOLAR_PANEL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_THORIUM_REACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_IMPACT_REACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_BATTERY] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_BATTERY_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_POWER_NODE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_POWER_NODE_LARGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_SURGE_TOWER] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_DIODE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TURBINE_CONDENSER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_VENT_CONDENSER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x05
    },

    [MSAV_BLOCK_CHEMICAL_COMBUSTION_CHAMBER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_PYROLYSIS_GENERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_FLUX_REACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_NEOPLASIA_REACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_BEAM_NODE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_BEAM_TOWER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_BEAM_LINK] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_MECHANICAL_DRILL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x05
    },

    [MSAV_BLOCK_PNEUMATIC_DRILL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x05
    },

    [MSAV_BLOCK_LASER_DRILL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_BLAST_DRILL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_WATER_EXTRACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x06
    },

    [MSAV_BLOCK_OIL_EXTRACTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_CULTIVATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_CLIFF_CRUSHER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_LARGE_CLIFF_CRUSHER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_PLASMA_BORE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_LARGE_PLASMA_BORE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_IMPACT_DRILL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_ERUPTION_DRILL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_CORE_SHARD] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CORE_FOUNDATION] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CORE_NUCLEUS] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_VAULT] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CONTAINER] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CORE_BASTION] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CORE_CITADEL] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CORE_ACROPOLIS] =
    {
        .has_building = 0x03,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_REINFORCED_CONTAINER] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_REINFORCED_VAULT] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DUO] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SCATTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SCORCH] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_HAIL] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_ARC] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_WAVE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LANCER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_SWARMER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SALVO] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_FUSE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_RIPPLE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_CYCLONE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_FORESHADOW] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SPECTRE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_MELTDOWN] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_SEGMENT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_PARALLAX] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_TSUNAMI] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_BREACH] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DIFFUSE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SUBLIMATE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_TITAN] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_DISPERSE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_AFFLICT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_LUSTRE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SCATHE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SMITE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_MALIGN] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_GROUND_FACTORY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_AIR_FACTORY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_NAVAL_FACTORY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_ADDITIVE_RECONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MULTIPLICATIVE_RECONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_EXPONENTIAL_RECONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TETRATIVE_RECONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REPAIR_POINT] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_REPAIR_TURRET] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_TANK_FABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_SHIP_FABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_MECH_FABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_TANK_REFABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SHIP_REFABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MECH_REFABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PRIME_REFABRICATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TANK_ASSEMBLER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_SHIP_ASSEMBLER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_MECH_ASSEMBLER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x01
    },

    [MSAV_BLOCK_BASIC_ASSEMBLER_MODULE] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_UNIT_REPAIR_TOWER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PAYLOAD_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PAYLOAD_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REINFORCED_PAYLOAD_CONVEYOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REINFORCED_PAYLOAD_ROUTER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_PAYLOAD_MASS_DRIVER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_LARGE_PAYLOAD_MASS_DRIVER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x02
    },

    [MSAV_BLOCK_SMALL_DECONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_DECONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_CONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_LARGE_CONSTRUCTOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_PAYLOAD_LOADER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_PAYLOAD_UNLOADER] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x07
    },

    [MSAV_BLOCK_MESSAGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_SWITCH_BLOCK] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MICRO_PROCESSOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_LOGIC_PROCESSOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_HYPER_PROCESSOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x04
    },

    [MSAV_BLOCK_LARGE_LOGIC_DISPLAY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_LOGIC_DISPLAY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_TILE_LOGIC_DISPLAY] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MEMORY_CELL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_MEMORY_BANK] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_CANVAS] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_LARGE_CANVAS] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_REINFORCED_MESSAGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WORLD_PROCESSOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WORLD_CELL] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WORLD_MESSAGE] =
    {
        .has_building = 0x02,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_WORLD_SWITCH] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x00
    },

    [MSAV_BLOCK_LAUNCH_PAD] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_ADVANCED_LAUNCH_PAD] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    },

    [MSAV_BLOCK_LANDING_PAD] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x05
    },

    [MSAV_BLOCK_INTERPLANETARY_ACCELERATOR] =
    {
        .has_building = 0x01,
        .modules_bitmask = 0x03
    }

};

static const char *const msav_block__names[__MSAV_BLOCK_MAX__] =
{
    [MSAV_BLOCK_AIR] = "air",
    [MSAV_BLOCK_SPAWN] = "spawn",
    [MSAV_BLOCK_REMOVE_WALL] = "removeWall",
    [MSAV_BLOCK_REMOVE_ORE] = "removeOre",
    [MSAV_BLOCK_CLIFF] = "cliff",
    [MSAV_BLOCK_DEEPWATER] = "deepwater",
    [MSAV_BLOCK_WATER] = "water",
    [MSAV_BLOCK_TAINTED_WATER] = "taintedWater",
    [MSAV_BLOCK_DEEP_TAINTED_WATER] = "deepTaintedWater",
    [MSAV_BLOCK_TAR] = "tar",
    [MSAV_BLOCK_SLAG] = "slag",
    [MSAV_BLOCK_CRYOFLUID] = "cryofluid",
    [MSAV_BLOCK_STONE] = "stone",
    [MSAV_BLOCK_CRATERS] = "craters",
    [MSAV_BLOCK_CHARR] = "charr",
    [MSAV_BLOCK_SAND] = "sand",
    [MSAV_BLOCK_DARKSAND] = "darksand",
    [MSAV_BLOCK_DIRT] = "dirt",
    [MSAV_BLOCK_MUD] = "mud",
    [MSAV_BLOCK_ICE] = "ice",
    [MSAV_BLOCK_SNOW] = "snow",
    [MSAV_BLOCK_DARKSAND_TAINTED_WATER] = "darksandTaintedWater",
    [MSAV_BLOCK_SPACE] = "space",
    [MSAV_BLOCK_EMPTY] = "empty",
    [MSAV_BLOCK_DACITE] = "dacite",
    [MSAV_BLOCK_RHYOLITE] = "rhyolite",
    [MSAV_BLOCK_RHYOLITE_CRATER] = "rhyoliteCrater",
    [MSAV_BLOCK_ROUGH_RHYOLITE] = "roughRhyolite",
    [MSAV_BLOCK_REGOLITH] = "regolith",
    [MSAV_BLOCK_YELLOW_STONE] = "yellowStone",
    [MSAV_BLOCK_RED_ICE] = "redIce",
    [MSAV_BLOCK_RED_STONE] = "redStone",
    [MSAV_BLOCK_DENSE_RED_STONE] = "denseRedStone",
    [MSAV_BLOCK_ARKYCITE_FLOOR] = "arkyciteFloor",
    [MSAV_BLOCK_ARKYIC_STONE] = "arkyicStone",
    [MSAV_BLOCK_REDMAT] = "redmat",
    [MSAV_BLOCK_BLUEMAT] = "bluemat",
    [MSAV_BLOCK_STONE_WALL] = "stoneWall",
    [MSAV_BLOCK_DIRT_WALL] = "dirtWall",
    [MSAV_BLOCK_SPORE_WALL] = "sporeWall",
    [MSAV_BLOCK_ICE_WALL] = "iceWall",
    [MSAV_BLOCK_DACITE_WALL] = "daciteWall",
    [MSAV_BLOCK_SPORE_PINE] = "sporePine",
    [MSAV_BLOCK_SNOW_PINE] = "snowPine",
    [MSAV_BLOCK_PINE] = "pine",
    [MSAV_BLOCK_SHRUBS] = "shrubs",
    [MSAV_BLOCK_WHITE_TREE] = "whiteTree",
    [MSAV_BLOCK_WHITE_TREE_DEAD] = "whiteTreeDead",
    [MSAV_BLOCK_SPORE_CLUSTER] = "sporeCluster",
    [MSAV_BLOCK_REDWEED] = "redweed",
    [MSAV_BLOCK_PURBUSH] = "purbush",
    [MSAV_BLOCK_YELLOW_CORAL] = "yellowCoral",
    [MSAV_BLOCK_RHYOLITE_VENT] = "rhyoliteVent",
    [MSAV_BLOCK_CARBON_VENT] = "carbonVent",
    [MSAV_BLOCK_ARKYIC_VENT] = "arkyicVent",
    [MSAV_BLOCK_YELLOW_STONE_VENT] = "yellowStoneVent",
    [MSAV_BLOCK_RED_STONE_VENT] = "redStoneVent",
    [MSAV_BLOCK_CRYSTALLINE_VENT] = "crystallineVent",
    [MSAV_BLOCK_STONE_VENT] = "stoneVent",
    [MSAV_BLOCK_BASALT_VENT] = "basaltVent",
    [MSAV_BLOCK_REGOLITH_WALL] = "regolithWall",
    [MSAV_BLOCK_YELLOW_STONE_WALL] = "yellowStoneWall",
    [MSAV_BLOCK_RHYOLITE_WALL] = "rhyoliteWall",
    [MSAV_BLOCK_CARBON_WALL] = "carbonWall",
    [MSAV_BLOCK_RED_ICE_WALL] = "redIceWall",
    [MSAV_BLOCK_FERRIC_STONE_WALL] = "ferricStoneWall",
    [MSAV_BLOCK_BERYLLIC_STONE_WALL] = "beryllicStoneWall",
    [MSAV_BLOCK_ARKYIC_WALL] = "arkyicWall",
    [MSAV_BLOCK_CRYSTALLINE_STONE_WALL] = "crystallineStoneWall",
    [MSAV_BLOCK_RED_STONE_WALL] = "redStoneWall",
    [MSAV_BLOCK_RED_DIAMOND_WALL] = "redDiamondWall",
    [MSAV_BLOCK_FERRIC_STONE] = "ferricStone",
    [MSAV_BLOCK_FERRIC_CRATERS] = "ferricCraters",
    [MSAV_BLOCK_CARBON_STONE] = "carbonStone",
    [MSAV_BLOCK_BERYLLIC_STONE] = "beryllicStone",
    [MSAV_BLOCK_CRYSTALLINE_STONE] = "crystallineStone",
    [MSAV_BLOCK_CRYSTAL_FLOOR] = "crystalFloor",
    [MSAV_BLOCK_YELLOW_STONE_PLATES] = "yellowStonePlates",
    [MSAV_BLOCK_ICE_SNOW] = "iceSnow",
    [MSAV_BLOCK_SAND_WATER] = "sandWater",
    [MSAV_BLOCK_DARKSAND_WATER] = "darksandWater",
    [MSAV_BLOCK_DUNE_WALL] = "duneWall",
    [MSAV_BLOCK_SAND_WALL] = "sandWall",
    [MSAV_BLOCK_MOSS] = "moss",
    [MSAV_BLOCK_SPORE_MOSS] = "sporeMoss",
    [MSAV_BLOCK_SHALE] = "shale",
    [MSAV_BLOCK_SHALE_WALL] = "shaleWall",
    [MSAV_BLOCK_GRASS] = "grass",
    [MSAV_BLOCK_SALT] = "salt",
    [MSAV_BLOCK_CORE_ZONE] = "coreZone",
    [MSAV_BLOCK_SHALE_BOULDER] = "shaleBoulder",
    [MSAV_BLOCK_SAND_BOULDER] = "sandBoulder",
    [MSAV_BLOCK_DACITE_BOULDER] = "daciteBoulder",
    [MSAV_BLOCK_BOULDER] = "boulder",
    [MSAV_BLOCK_SNOW_BOULDER] = "snowBoulder",
    [MSAV_BLOCK_BASALT_BOULDER] = "basaltBoulder",
    [MSAV_BLOCK_CARBON_BOULDER] = "carbonBoulder",
    [MSAV_BLOCK_FERRIC_BOULDER] = "ferricBoulder",
    [MSAV_BLOCK_BERYLLIC_BOULDER] = "beryllicBoulder",
    [MSAV_BLOCK_YELLOW_STONE_BOULDER] = "yellowStoneBoulder",
    [MSAV_BLOCK_ARKYIC_BOULDER] = "arkyicBoulder",
    [MSAV_BLOCK_CRYSTAL_CLUSTER] = "crystalCluster",
    [MSAV_BLOCK_VIBRANT_CRYSTAL_CLUSTER] = "vibrantCrystalCluster",
    [MSAV_BLOCK_CRYSTAL_BLOCKS] = "crystalBlocks",
    [MSAV_BLOCK_CRYSTAL_ORBS] = "crystalOrbs",
    [MSAV_BLOCK_CRYSTALLINE_BOULDER] = "crystallineBoulder",
    [MSAV_BLOCK_RED_ICE_BOULDER] = "redIceBoulder",
    [MSAV_BLOCK_RHYOLITE_BOULDER] = "rhyoliteBoulder",
    [MSAV_BLOCK_RED_STONE_BOULDER] = "redStoneBoulder",
    [MSAV_BLOCK_METAL_FLOOR] = "metalFloor",
    [MSAV_BLOCK_METAL_FLOOR_DAMAGED] = "metalFloorDamaged",
    [MSAV_BLOCK_METAL_FLOOR2] = "metalFloor2",
    [MSAV_BLOCK_METAL_FLOOR3] = "metalFloor3",
    [MSAV_BLOCK_METAL_FLOOR4] = "metalFloor4",
    [MSAV_BLOCK_METAL_FLOOR5] = "metalFloor5",
    [MSAV_BLOCK_BASALT] = "basalt",
    [MSAV_BLOCK_MAGMAROCK] = "magmarock",
    [MSAV_BLOCK_HOTROCK] = "hotrock",
    [MSAV_BLOCK_SNOW_WALL] = "snowWall",
    [MSAV_BLOCK_SALT_WALL] = "saltWall",
    [MSAV_BLOCK_DARK_PANEL1] = "darkPanel1",
    [MSAV_BLOCK_DARK_PANEL2] = "darkPanel2",
    [MSAV_BLOCK_DARK_PANEL3] = "darkPanel3",
    [MSAV_BLOCK_DARK_PANEL4] = "darkPanel4",
    [MSAV_BLOCK_DARK_PANEL5] = "darkPanel5",
    [MSAV_BLOCK_DARK_PANEL6] = "darkPanel6",
    [MSAV_BLOCK_DARK_METAL] = "darkMetal",
    [MSAV_BLOCK_METAL_TILES1] = "metalTiles1",
    [MSAV_BLOCK_METAL_TILES2] = "metalTiles2",
    [MSAV_BLOCK_METAL_TILES3] = "metalTiles3",
    [MSAV_BLOCK_METAL_TILES4] = "metalTiles4",
    [MSAV_BLOCK_METAL_TILES5] = "metalTiles5",
    [MSAV_BLOCK_METAL_TILES6] = "metalTiles6",
    [MSAV_BLOCK_METAL_TILES7] = "metalTiles7",
    [MSAV_BLOCK_METAL_TILES8] = "metalTiles8",
    [MSAV_BLOCK_METAL_TILES9] = "metalTiles9",
    [MSAV_BLOCK_METAL_TILES10] = "metalTiles10",
    [MSAV_BLOCK_METAL_TILES11] = "metalTiles11",
    [MSAV_BLOCK_METAL_TILES12] = "metalTiles12",
    [MSAV_BLOCK_METAL_TILES13] = "metalTiles13",
    [MSAV_BLOCK_METAL_WALL1] = "metalWall1",
    [MSAV_BLOCK_METAL_WALL2] = "metalWall2",
    [MSAV_BLOCK_METAL_WALL3] = "metalWall3",
    [MSAV_BLOCK_COLORED_FLOOR] = "coloredFloor",
    [MSAV_BLOCK_COLORED_WALL] = "coloredWall",
    [MSAV_BLOCK_CHARACTER_OVERLAY_GRAY] = "characterOverlayGray",
    [MSAV_BLOCK_CHARACTER_OVERLAY_WHITE] = "characterOverlayWhite",
    [MSAV_BLOCK_RUNE_OVERLAY] = "runeOverlay",
    [MSAV_BLOCK_CRUX_RUNE_OVERLAY] = "cruxRuneOverlay",
    [MSAV_BLOCK_PEBBLES] = "pebbles",
    [MSAV_BLOCK_TENDRILS] = "tendrils",
    [MSAV_BLOCK_ORE_COPPER] = "oreCopper",
    [MSAV_BLOCK_ORE_LEAD] = "oreLead",
    [MSAV_BLOCK_ORE_SCRAP] = "oreScrap",
    [MSAV_BLOCK_ORE_COAL] = "oreCoal",
    [MSAV_BLOCK_ORE_TITANIUM] = "oreTitanium",
    [MSAV_BLOCK_ORE_THORIUM] = "oreThorium",
    [MSAV_BLOCK_ORE_BERYLLIUM] = "oreBeryllium",
    [MSAV_BLOCK_ORE_TUNGSTEN] = "oreTungsten",
    [MSAV_BLOCK_ORE_CRYSTAL_THORIUM] = "oreCrystalThorium",
    [MSAV_BLOCK_WALL_ORE_THORIUM] = "wallOreThorium",
    [MSAV_BLOCK_WALL_ORE_BERYLLIUM] = "wallOreBeryllium",
    [MSAV_BLOCK_GRAPHITIC_WALL] = "graphiticWall",
    [MSAV_BLOCK_WALL_ORE_GRAPHITE] = "wallOreGraphite",
    [MSAV_BLOCK_WALL_ORE_TUNGSTEN] = "wallOreTungsten",
    [MSAV_BLOCK_SILICON_SMELTER] = "siliconSmelter",
    [MSAV_BLOCK_SILICON_CRUCIBLE] = "siliconCrucible",
    [MSAV_BLOCK_KILN] = "kiln",
    [MSAV_BLOCK_GRAPHITE_PRESS] = "graphitePress",
    [MSAV_BLOCK_PLASTANIUM_COMPRESSOR] = "plastaniumCompressor",
    [MSAV_BLOCK_MULTI_PRESS] = "multiPress",
    [MSAV_BLOCK_PHASE_WEAVER] = "phaseWeaver",
    [MSAV_BLOCK_SURGE_SMELTER] = "surgeSmelter",
    [MSAV_BLOCK_PYRATITE_MIXER] = "pyratiteMixer",
    [MSAV_BLOCK_BLAST_MIXER] = "blastMixer",
    [MSAV_BLOCK_CRYOFLUID_MIXER] = "cryofluidMixer",
    [MSAV_BLOCK_MELTER] = "melter",
    [MSAV_BLOCK_SEPARATOR] = "separator",
    [MSAV_BLOCK_DISASSEMBLER] = "disassembler",
    [MSAV_BLOCK_SPORE_PRESS] = "sporePress",
    [MSAV_BLOCK_PULVERIZER] = "pulverizer",
    [MSAV_BLOCK_INCINERATOR] = "incinerator",
    [MSAV_BLOCK_COAL_CENTRIFUGE] = "coalCentrifuge",
    [MSAV_BLOCK_SILICON_ARC_FURNACE] = "siliconArcFurnace",
    [MSAV_BLOCK_ELECTROLYZER] = "electrolyzer",
    [MSAV_BLOCK_OXIDATION_CHAMBER] = "oxidationChamber",
    [MSAV_BLOCK_ATMOSPHERIC_CONCENTRATOR] = "atmosphericConcentrator",
    [MSAV_BLOCK_ELECTRIC_HEATER] = "electricHeater",
    [MSAV_BLOCK_SLAG_HEATER] = "slagHeater",
    [MSAV_BLOCK_PHASE_HEATER] = "phaseHeater",
    [MSAV_BLOCK_HEAT_REDIRECTOR] = "heatRedirector",
    [MSAV_BLOCK_SMALL_HEAT_REDIRECTOR] = "smallHeatRedirector",
    [MSAV_BLOCK_HEAT_ROUTER] = "heatRouter",
    [MSAV_BLOCK_SLAG_INCINERATOR] = "slagIncinerator",
    [MSAV_BLOCK_CARBIDE_CRUCIBLE] = "carbideCrucible",
    [MSAV_BLOCK_SLAG_CENTRIFUGE] = "slagCentrifuge",
    [MSAV_BLOCK_SURGE_CRUCIBLE] = "surgeCrucible",
    [MSAV_BLOCK_CYANOGEN_SYNTHESIZER] = "cyanogenSynthesizer",
    [MSAV_BLOCK_PHASE_SYNTHESIZER] = "phaseSynthesizer",
    [MSAV_BLOCK_HEAT_REACTOR] = "heatReactor",
    [MSAV_BLOCK_POWER_SOURCE] = "powerSource",
    [MSAV_BLOCK_POWER_VOID] = "powerVoid",
    [MSAV_BLOCK_ITEM_SOURCE] = "itemSource",
    [MSAV_BLOCK_ITEM_VOID] = "itemVoid",
    [MSAV_BLOCK_LIQUID_SOURCE] = "liquidSource",
    [MSAV_BLOCK_LIQUID_VOID] = "liquidVoid",
    [MSAV_BLOCK_PAYLOAD_SOURCE] = "payloadSource",
    [MSAV_BLOCK_PAYLOAD_VOID] = "payloadVoid",
    [MSAV_BLOCK_ILLUMINATOR] = "illuminator",
    [MSAV_BLOCK_HEAT_SOURCE] = "heatSource",
    [MSAV_BLOCK_COPPER_WALL] = "copperWall",
    [MSAV_BLOCK_COPPER_WALL_LARGE] = "copperWallLarge",
    [MSAV_BLOCK_TITANIUM_WALL] = "titaniumWall",
    [MSAV_BLOCK_TITANIUM_WALL_LARGE] = "titaniumWallLarge",
    [MSAV_BLOCK_PLASTANIUM_WALL] = "plastaniumWall",
    [MSAV_BLOCK_PLASTANIUM_WALL_LARGE] = "plastaniumWallLarge",
    [MSAV_BLOCK_THORIUM_WALL] = "thoriumWall",
    [MSAV_BLOCK_THORIUM_WALL_LARGE] = "thoriumWallLarge",
    [MSAV_BLOCK_DOOR] = "door",
    [MSAV_BLOCK_DOOR_LARGE] = "doorLarge",
    [MSAV_BLOCK_PHASE_WALL] = "phaseWall",
    [MSAV_BLOCK_PHASE_WALL_LARGE] = "phaseWallLarge",
    [MSAV_BLOCK_SURGE_WALL] = "surgeWall",
    [MSAV_BLOCK_SURGE_WALL_LARGE] = "surgeWallLarge",
    [MSAV_BLOCK_SCRAP_WALL] = "scrapWall",
    [MSAV_BLOCK_SCRAP_WALL_LARGE] = "scrapWallLarge",
    [MSAV_BLOCK_SCRAP_WALL_HUGE] = "scrapWallHuge",
    [MSAV_BLOCK_SCRAP_WALL_GIGANTIC] = "scrapWallGigantic",
    [MSAV_BLOCK_THRUSTER] = "thruster",
    [MSAV_BLOCK_BERYLLIUM_WALL] = "berylliumWall",
    [MSAV_BLOCK_BERYLLIUM_WALL_LARGE] = "berylliumWallLarge",
    [MSAV_BLOCK_TUNGSTEN_WALL] = "tungstenWall",
    [MSAV_BLOCK_TUNGSTEN_WALL_LARGE] = "tungstenWallLarge",
    [MSAV_BLOCK_BLAST_DOOR] = "blastDoor",
    [MSAV_BLOCK_REINFORCED_SURGE_WALL] = "reinforcedSurgeWall",
    [MSAV_BLOCK_REINFORCED_SURGE_WALL_LARGE] = "reinforcedSurgeWallLarge",
    [MSAV_BLOCK_CARBIDE_WALL] = "carbideWall",
    [MSAV_BLOCK_CARBIDE_WALL_LARGE] = "carbideWallLarge",
    [MSAV_BLOCK_SHIELDED_WALL] = "shieldedWall",
    [MSAV_BLOCK_MENDER] = "mender",
    [MSAV_BLOCK_MEND_PROJECTOR] = "mendProjector",
    [MSAV_BLOCK_OVERDRIVE_PROJECTOR] = "overdriveProjector",
    [MSAV_BLOCK_OVERDRIVE_DOME] = "overdriveDome",
    [MSAV_BLOCK_FORCE_PROJECTOR] = "forceProjector",
    [MSAV_BLOCK_SHOCK_MINE] = "shockMine",
    [MSAV_BLOCK_TARGET_DUMMY] = "targetDummy",
    [MSAV_BLOCK_RADAR] = "radar",
    [MSAV_BLOCK_BUILD_TOWER] = "buildTower",
    [MSAV_BLOCK_REGEN_PROJECTOR] = "regenProjector",
    [MSAV_BLOCK_SHOCKWAVE_TOWER] = "shockwaveTower",
    [MSAV_BLOCK_SHIELD_PROJECTOR] = "shieldProjector",
    [MSAV_BLOCK_LARGE_SHIELD_PROJECTOR] = "largeShieldProjector",
    [MSAV_BLOCK_SHIELD_BREAKER] = "shieldBreaker",
    [MSAV_BLOCK_CONVEYOR] = "conveyor",
    [MSAV_BLOCK_TITANIUM_CONVEYOR] = "titaniumConveyor",
    [MSAV_BLOCK_PLASTANIUM_CONVEYOR] = "plastaniumConveyor",
    [MSAV_BLOCK_ARMORED_CONVEYOR] = "armoredConveyor",
    [MSAV_BLOCK_DISTRIBUTOR] = "distributor",
    [MSAV_BLOCK_JUNCTION] = "junction",
    [MSAV_BLOCK_ITEM_BRIDGE] = "itemBridge",
    [MSAV_BLOCK_PHASE_CONVEYOR] = "phaseConveyor",
    [MSAV_BLOCK_SORTER] = "sorter",
    [MSAV_BLOCK_INVERTED_SORTER] = "invertedSorter",
    [MSAV_BLOCK_ROUTER] = "router",
    [MSAV_BLOCK_OVERFLOW_GATE] = "overflowGate",
    [MSAV_BLOCK_UNDERFLOW_GATE] = "underflowGate",
    [MSAV_BLOCK_UNLOADER] = "unloader",
    [MSAV_BLOCK_MASS_DRIVER] = "massDriver",
    [MSAV_BLOCK_DUCT] = "duct",
    [MSAV_BLOCK_ARMORED_DUCT] = "armoredDuct",
    [MSAV_BLOCK_DUCT_ROUTER] = "ductRouter",
    [MSAV_BLOCK_OVERFLOW_DUCT] = "overflowDuct",
    [MSAV_BLOCK_UNDERFLOW_DUCT] = "underflowDuct",
    [MSAV_BLOCK_DUCT_BRIDGE] = "ductBridge",
    [MSAV_BLOCK_DUCT_UNLOADER] = "ductUnloader",
    [MSAV_BLOCK_SURGE_CONVEYOR] = "surgeConveyor",
    [MSAV_BLOCK_SURGE_ROUTER] = "surgeRouter",
    [MSAV_BLOCK_UNIT_CARGO_LOADER] = "unitCargoLoader",
    [MSAV_BLOCK_UNIT_CARGO_UNLOAD_POINT] = "unitCargoUnloadPoint",
    [MSAV_BLOCK_MECHANICAL_PUMP] = "mechanicalPump",
    [MSAV_BLOCK_ROTARY_PUMP] = "rotaryPump",
    [MSAV_BLOCK_IMPULSE_PUMP] = "impulsePump",
    [MSAV_BLOCK_CONDUIT] = "conduit",
    [MSAV_BLOCK_PULSE_CONDUIT] = "pulseConduit",
    [MSAV_BLOCK_PLATED_CONDUIT] = "platedConduit",
    [MSAV_BLOCK_LIQUID_ROUTER] = "liquidRouter",
    [MSAV_BLOCK_LIQUID_CONTAINER] = "liquidContainer",
    [MSAV_BLOCK_LIQUID_TANK] = "liquidTank",
    [MSAV_BLOCK_LIQUID_JUNCTION] = "liquidJunction",
    [MSAV_BLOCK_BRIDGE_CONDUIT] = "bridgeConduit",
    [MSAV_BLOCK_PHASE_CONDUIT] = "phaseConduit",
    [MSAV_BLOCK_REINFORCED_PUMP] = "reinforcedPump",
    [MSAV_BLOCK_REINFORCED_CONDUIT] = "reinforcedConduit",
    [MSAV_BLOCK_REINFORCED_LIQUID_JUNCTION] = "reinforcedLiquidJunction",
    [MSAV_BLOCK_REINFORCED_BRIDGE_CONDUIT] = "reinforcedBridgeConduit",
    [MSAV_BLOCK_REINFORCED_LIQUID_ROUTER] = "reinforcedLiquidRouter",
    [MSAV_BLOCK_REINFORCED_LIQUID_CONTAINER] = "reinforcedLiquidContainer",
    [MSAV_BLOCK_REINFORCED_LIQUID_TANK] = "reinforcedLiquidTank",
    [MSAV_BLOCK_COMBUSTION_GENERATOR] = "combustionGenerator",
    [MSAV_BLOCK_THERMAL_GENERATOR] = "thermalGenerator",
    [MSAV_BLOCK_STEAM_GENERATOR] = "steamGenerator",
    [MSAV_BLOCK_DIFFERENTIAL_GENERATOR] = "differentialGenerator",
    [MSAV_BLOCK_RTG_GENERATOR] = "rtgGenerator",
    [MSAV_BLOCK_SOLAR_PANEL] = "solarPanel",
    [MSAV_BLOCK_LARGE_SOLAR_PANEL] = "largeSolarPanel",
    [MSAV_BLOCK_THORIUM_REACTOR] = "thoriumReactor",
    [MSAV_BLOCK_IMPACT_REACTOR] = "impactReactor",
    [MSAV_BLOCK_BATTERY] = "battery",
    [MSAV_BLOCK_BATTERY_LARGE] = "batteryLarge",
    [MSAV_BLOCK_POWER_NODE] = "powerNode",
    [MSAV_BLOCK_POWER_NODE_LARGE] = "powerNodeLarge",
    [MSAV_BLOCK_SURGE_TOWER] = "surgeTower",
    [MSAV_BLOCK_DIODE] = "diode",
    [MSAV_BLOCK_TURBINE_CONDENSER] = "turbineCondenser",
    [MSAV_BLOCK_VENT_CONDENSER] = "ventCondenser",
    [MSAV_BLOCK_CHEMICAL_COMBUSTION_CHAMBER] = "chemicalCombustionChamber",
    [MSAV_BLOCK_PYROLYSIS_GENERATOR] = "pyrolysisGenerator",
    [MSAV_BLOCK_FLUX_REACTOR] = "fluxReactor",
    [MSAV_BLOCK_NEOPLASIA_REACTOR] = "neoplasiaReactor",
    [MSAV_BLOCK_BEAM_NODE] = "beamNode",
    [MSAV_BLOCK_BEAM_TOWER] = "beamTower",
    [MSAV_BLOCK_BEAM_LINK] = "beamLink",
    [MSAV_BLOCK_MECHANICAL_DRILL] = "mechanicalDrill",
    [MSAV_BLOCK_PNEUMATIC_DRILL] = "pneumaticDrill",
    [MSAV_BLOCK_LASER_DRILL] = "laserDrill",
    [MSAV_BLOCK_BLAST_DRILL] = "blastDrill",
    [MSAV_BLOCK_WATER_EXTRACTOR] = "waterExtractor",
    [MSAV_BLOCK_OIL_EXTRACTOR] = "oilExtractor",
    [MSAV_BLOCK_CULTIVATOR] = "cultivator",
    [MSAV_BLOCK_CLIFF_CRUSHER] = "cliffCrusher",
    [MSAV_BLOCK_LARGE_CLIFF_CRUSHER] = "largeCliffCrusher",
    [MSAV_BLOCK_PLASMA_BORE] = "plasmaBore",
    [MSAV_BLOCK_LARGE_PLASMA_BORE] = "largePlasmaBore",
    [MSAV_BLOCK_IMPACT_DRILL] = "impactDrill",
    [MSAV_BLOCK_ERUPTION_DRILL] = "eruptionDrill",
    [MSAV_BLOCK_CORE_SHARD] = "coreShard",
    [MSAV_BLOCK_CORE_FOUNDATION] = "coreFoundation",
    [MSAV_BLOCK_CORE_NUCLEUS] = "coreNucleus",
    [MSAV_BLOCK_VAULT] = "vault",
    [MSAV_BLOCK_CONTAINER] = "container",
    [MSAV_BLOCK_CORE_BASTION] = "coreBastion",
    [MSAV_BLOCK_CORE_CITADEL] = "coreCitadel",
    [MSAV_BLOCK_CORE_ACROPOLIS] = "coreAcropolis",
    [MSAV_BLOCK_REINFORCED_CONTAINER] = "reinforcedContainer",
    [MSAV_BLOCK_REINFORCED_VAULT] = "reinforcedVault",
    [MSAV_BLOCK_DUO] = "duo",
    [MSAV_BLOCK_SCATTER] = "scatter",
    [MSAV_BLOCK_SCORCH] = "scorch",
    [MSAV_BLOCK_HAIL] = "hail",
    [MSAV_BLOCK_ARC] = "arc",
    [MSAV_BLOCK_WAVE] = "wave",
    [MSAV_BLOCK_LANCER] = "lancer",
    [MSAV_BLOCK_SWARMER] = "swarmer",
    [MSAV_BLOCK_SALVO] = "salvo",
    [MSAV_BLOCK_FUSE] = "fuse",
    [MSAV_BLOCK_RIPPLE] = "ripple",
    [MSAV_BLOCK_CYCLONE] = "cyclone",
    [MSAV_BLOCK_FORESHADOW] = "foreshadow",
    [MSAV_BLOCK_SPECTRE] = "spectre",
    [MSAV_BLOCK_MELTDOWN] = "meltdown",
    [MSAV_BLOCK_SEGMENT] = "segment",
    [MSAV_BLOCK_PARALLAX] = "parallax",
    [MSAV_BLOCK_TSUNAMI] = "tsunami",
    [MSAV_BLOCK_BREACH] = "breach",
    [MSAV_BLOCK_DIFFUSE] = "diffuse",
    [MSAV_BLOCK_SUBLIMATE] = "sublimate",
    [MSAV_BLOCK_TITAN] = "titan",
    [MSAV_BLOCK_DISPERSE] = "disperse",
    [MSAV_BLOCK_AFFLICT] = "afflict",
    [MSAV_BLOCK_LUSTRE] = "lustre",
    [MSAV_BLOCK_SCATHE] = "scathe",
    [MSAV_BLOCK_SMITE] = "smite",
    [MSAV_BLOCK_MALIGN] = "malign",
    [MSAV_BLOCK_GROUND_FACTORY] = "groundFactory",
    [MSAV_BLOCK_AIR_FACTORY] = "airFactory",
    [MSAV_BLOCK_NAVAL_FACTORY] = "navalFactory",
    [MSAV_BLOCK_ADDITIVE_RECONSTRUCTOR] = "additiveReconstructor",
    [MSAV_BLOCK_MULTIPLICATIVE_RECONSTRUCTOR] = "multiplicativeReconstructor",
    [MSAV_BLOCK_EXPONENTIAL_RECONSTRUCTOR] = "exponentialReconstructor",
    [MSAV_BLOCK_TETRATIVE_RECONSTRUCTOR] = "tetrativeReconstructor",
    [MSAV_BLOCK_REPAIR_POINT] = "repairPoint",
    [MSAV_BLOCK_REPAIR_TURRET] = "repairTurret",
    [MSAV_BLOCK_TANK_FABRICATOR] = "tankFabricator",
    [MSAV_BLOCK_SHIP_FABRICATOR] = "shipFabricator",
    [MSAV_BLOCK_MECH_FABRICATOR] = "mechFabricator",
    [MSAV_BLOCK_TANK_REFABRICATOR] = "tankRefabricator",
    [MSAV_BLOCK_SHIP_REFABRICATOR] = "shipRefabricator",
    [MSAV_BLOCK_MECH_REFABRICATOR] = "mechRefabricator",
    [MSAV_BLOCK_PRIME_REFABRICATOR] = "primeRefabricator",
    [MSAV_BLOCK_TANK_ASSEMBLER] = "tankAssembler",
    [MSAV_BLOCK_SHIP_ASSEMBLER] = "shipAssembler",
    [MSAV_BLOCK_MECH_ASSEMBLER] = "mechAssembler",
    [MSAV_BLOCK_BASIC_ASSEMBLER_MODULE] = "basicAssemblerModule",
    [MSAV_BLOCK_UNIT_REPAIR_TOWER] = "unitRepairTower",
    [MSAV_BLOCK_PAYLOAD_CONVEYOR] = "payloadConveyor",
    [MSAV_BLOCK_PAYLOAD_ROUTER] = "payloadRouter",
    [MSAV_BLOCK_REINFORCED_PAYLOAD_CONVEYOR] = "reinforcedPayloadConveyor",
    [MSAV_BLOCK_REINFORCED_PAYLOAD_ROUTER] = "reinforcedPayloadRouter",
    [MSAV_BLOCK_PAYLOAD_MASS_DRIVER] = "payloadMassDriver",
    [MSAV_BLOCK_LARGE_PAYLOAD_MASS_DRIVER] = "largePayloadMassDriver",
    [MSAV_BLOCK_SMALL_DECONSTRUCTOR] = "smallDeconstructor",
    [MSAV_BLOCK_DECONSTRUCTOR] = "deconstructor",
    [MSAV_BLOCK_CONSTRUCTOR] = "constructor",
    [MSAV_BLOCK_LARGE_CONSTRUCTOR] = "largeConstructor",
    [MSAV_BLOCK_PAYLOAD_LOADER] = "payloadLoader",
    [MSAV_BLOCK_PAYLOAD_UNLOADER] = "payloadUnloader",
    [MSAV_BLOCK_MESSAGE] = "message",
    [MSAV_BLOCK_SWITCH_BLOCK] = "switchBlock",
    [MSAV_BLOCK_MICRO_PROCESSOR] = "microProcessor",
    [MSAV_BLOCK_LOGIC_PROCESSOR] = "logicProcessor",
    [MSAV_BLOCK_HYPER_PROCESSOR] = "hyperProcessor",
    [MSAV_BLOCK_LARGE_LOGIC_DISPLAY] = "largeLogicDisplay",
    [MSAV_BLOCK_LOGIC_DISPLAY] = "logicDisplay",
    [MSAV_BLOCK_TILE_LOGIC_DISPLAY] = "tileLogicDisplay",
    [MSAV_BLOCK_MEMORY_CELL] = "memoryCell",
    [MSAV_BLOCK_MEMORY_BANK] = "memoryBank",
    [MSAV_BLOCK_CANVAS] = "canvas",
    [MSAV_BLOCK_LARGE_CANVAS] = "largeCanvas",
    [MSAV_BLOCK_REINFORCED_MESSAGE] = "reinforcedMessage",
    [MSAV_BLOCK_WORLD_PROCESSOR] = "worldProcessor",
    [MSAV_BLOCK_WORLD_CELL] = "worldCell",
    [MSAV_BLOCK_WORLD_MESSAGE] = "worldMessage",
    [MSAV_BLOCK_WORLD_SWITCH] = "worldSwitch",
    [MSAV_BLOCK_LAUNCH_PAD] = "launchPad",
    [MSAV_BLOCK_ADVANCED_LAUNCH_PAD] = "advancedLaunchPad",
    [MSAV_BLOCK_LANDING_PAD] = "landingPad",
    [MSAV_BLOCK_INTERPLANETARY_ACCELERATOR] = "interplanetaryAccelerator",
};

const char *msav_block_name(msav_block_t id)
{
    if ((unsigned int)id >= (unsigned int)__MSAV_BLOCK_MAX__)
        return 0;
    return msav_block__names[id];
}
