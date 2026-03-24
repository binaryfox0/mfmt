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

    [MSAV_BLOCK_BARRIER_PROJECTOR] =
    {
        .has_building = 0x00,
        .modules_bitmask = 0x00
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
        .has_building = 0x01,
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
        .has_building = 0x01,
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
