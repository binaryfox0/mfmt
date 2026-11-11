#ifndef MSAV_ENTITIES_GEN_H
#define MSAV_ENTITIES_GEN_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    int32_t id;
    const char *name;
    const char *const *aliases;
    size_t alias_count;
    const char *const *direct_components;
    size_t direct_component_count;
    const char *const *components;
    size_t component_count;
} msav_entity_info_t;

enum
{
    MSAV_ENTITY_UNIT_LEGACY_ALPHA = 0,
    MSAV_ENTITY_BLOCK_UNIT_UNIT = 2,
    MSAV_ENTITY_UNIT = 3,
    MSAV_ENTITY_MECH_UNIT = 4,
    MSAV_ENTITY_PAYLOAD_UNIT = 5,
    MSAV_ENTITY_BUILDING = 6,
    MSAV_ENTITY_BULLET = 7,
    MSAV_ENTITY_DECAL = 8,
    MSAV_ENTITY_EFFECT_STATE = 9,
    MSAV_ENTITY_FIRE = 10,
    MSAV_ENTITY_LAUNCH_CORE = 11,
    MSAV_ENTITY_PLAYER = 12,
    MSAV_ENTITY_PUDDLE = 13,
    MSAV_ENTITY_WEATHER_STATE = 14,
    MSAV_ENTITY_LAUNCH_PAYLOAD = 15,
    MSAV_ENTITY_UNIT_LEGACY_MONO = 16,
    MSAV_ENTITY_MECH_UNIT_LEGACY_NOVA = 17,
    MSAV_ENTITY_UNIT_LEGACY_POLY = 18,
    MSAV_ENTITY_MECH_UNIT_LEGACY_PULSAR = 19,
    MSAV_ENTITY_UNIT_WATER_MOVE = 20,
    MSAV_ENTITY_LEGS_UNIT_LEGACY_SPIROCT = 21,
    MSAV_ENTITY_PAYLOAD_UNIT_LEGACY_QUAD = 23,
    MSAV_ENTITY_LEGS_UNIT = 24,
    MSAV_ENTITY_PAYLOAD_UNIT_LEGACY_OCT = 26,
    MSAV_ENTITY_POS_TEAM = 28,
    MSAV_ENTITY_LEGS_UNIT_LEGACY_ARKYID = 29,
    MSAV_ENTITY_UNIT_LEGACY_BETA = 30,
    MSAV_ENTITY_UNIT_LEGACY_GAMMA = 31,
    MSAV_ENTITY_MECH_UNIT_LEGACY_QUASAR = 32,
    MSAV_ENTITY_LEGS_UNIT_LEGACY_TOXOPID = 33,
    MSAV_ENTITY_WORLD_LABEL = 35,
    MSAV_ENTITY_BUILDING_TETHER_PAYLOAD_UNIT = 36,
    MSAV_ENTITY_TIMED_KILL_UNIT = 39,
    MSAV_ENTITY_POWER_GRAPH_UPDATER = 42,
    MSAV_ENTITY_TANK_UNIT = 43,
    MSAV_ENTITY_ELEVATION_MOVE_UNIT = 45,
    MSAV_ENTITY_CRAWL_UNIT = 46,
    MSAV_ENTITY_TARGET_DUMMY_UNIT = 49
};

extern const msav_entity_info_t msav_entity_db[];
extern const size_t msav_entity_db_count;
const msav_entity_info_t *msav_entity_by_id(int32_t id);
const msav_entity_info_t *msav_entity_by_name(const char *name);

#endif
