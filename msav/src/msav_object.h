#ifndef MSAV_OBJECT_H
#define MSAV_OBJECT_H

#include <stdint.h>

#include "msav_arr.h"

typedef enum
{
    MSAV__CTYPE_ITEM,
    MSAV__CTYPE_BLOCK,
    MSAV__CTYPE_MECH, /* Unused */
    MSAV__CTYPE_BULLET,
    MSAV__CTYPE_LIQUID,
    MSAV__CTYPE_STATUS,
    MSAV__CTYPE_UNIT,
    MSAV__CTYPE_WEATHER,
    MSAV__CTYPE_EFFECT, /* Unused */
    MSAV__CTYPE_SECTOR,
    MSAV__CTYPE_LOADOUT, /* Unused */
    MSAV__CTYPE_TYPEID, /* Unused */
    MSAV__CTYPE_ERROR,
    MSAV__CTYPE_PLANET,
    MSAV__CTYPE_AMMO, /* Unused */
    MSAV__CTYPE_TEAM,
    MSAV__CTYPE_UNIT_COMMAND,
    MSAV__CTYPE_UNIT_STANCE,
} msav__content_type_t;

typedef struct
{
    msav__content_type_t type;
    int32_t idx;
} msav__content_t;

typedef struct
{
    int32_t x;
    int32_t y;
} msav__point2_t;

typedef struct 
{
    float x;
    float y;
} msav__vec2_t;

typedef int8_t msav__bool;

typedef enum
{
    MSAV__TEAM_DERELICT,
    MSAV__TEAM_SHARDED,
    MSAV__TEAM_CRUX,
    MSAV__TEAM_MALIS,
    MSAV__TEAM_GREEN,
    MSAV__TEAM_BLUE,
    MSAV__TEAM_NEOPLASTIC
} msav__team_t;

typedef struct
{
    int32_t pos;
} msav__building;

typedef enum
{
    MSAV__LACCESS_TOTAL_ITEMS,
    MSAV__LACCESS_FIRST_ITEM,
    MSAV__LACCESS_TOTAL_LIQUIDS,
    MSAV__LACCESS_TOTAL_POWER,
    MSAV__LACCESS_ITEM_CAPACITY,
    MSAV__LACCESS_LIQUIDCAPACITY,
    MSAV__LACCESS_POWER_CAPACITY,
    MSAV__LACCESS_POWER_NET_STORED,
    MSAV__LACCESS_POWER_NET_CAPACITY,
    MSAV__LACCESS_POWER_NET_IN,
    MSAV__LACCESS_POWER_NET_OUT,
    MSAV__LACCESS_AMMO,
    MSAV__LACCESS_AMMO_CAPACITY,
    MSAV__LACCESS_CURRENT_AMMO_TYPE,
    MSAV__LACCESS_MEMORY_CAPACITY,
    MSAV__LACCESS_HEALTH,
    MSAV__LACCESS_MAX_HEALTH,
    MSAV__LACCESS_HEAT,
    MSAV__LACCESS_SHIELD,
    MSAV__LACCESS_ARMOR,
    MSAV__LACCESS_EFFICIENCY,
    MSAV__LACCESS_PROGRESS,
    MSAV__LACCESS_TIMESCALE,
    MSAV__LACCESS_ROTATION,
    MSAV__LACCESS_X,
    MSAV__LACCESS_Y,
    MSAV__LACCESS_VELOCITY_X,
    MSAV__LACCESS_VELOCITY_Y,
    MSAV__LACCESS_SHOOT_X,
    MSAV__LACCESS_SHOOT_Y,
    MSAV__LACCESS_CAMERA_X,
    MSAV__LACCESS_CAMERA_Y,
    MSAV__LACCESS_CAMERA_WIDTH,
    MSAV__LACCESS_CAMERA_HEIGHT,
    MSAV__LACCESS_DISPLAY_WIDTH,
    MSAV__LACCESS_DISPLAY_HEIGHT,
    MSAV__LACCESS_BUFFER_SIZE,
    MSAV__LACCESS_OPERATIONS,
    MSAV__LACCESS_SIZE,
    MSAV__LACCESS_SOLID,
    MSAV__LACCESS_DEAD,
    MSAV__LACCESS_RANGE,
    MSAV__LACCESS_SHOOTING,
    MSAV__LACCESS_BOOSTING,
    MSAV__LACCESS_MINE_X,
    MSAV__LACCESS_MINE_Y,
    MSAV__LACCESS_MINING,
    MSAV__LACCESS_BUILD_X,
    MSAV__LACCESS_BUILD_Y,
    MSAV__LACCESS_BUILDING,
    MSAV__LACCESS_BREAKING,
    MSAV__LACCESS_SPEED,
    MSAV__LACCESS_TEAM,
    MSAV__LACCESS_TYPE,
    MSAV__LACCESS_FLAG,
    MSAV__LACCESS_CONTROLLED,
    MSAV__LACCESS_CONTROLLER,
    MSAV__LACCESS_NAME,
    MSAV__LACCESS_PAYLOAD_COUNT,
    MSAV__LACCESS_PAYLOAD_TYPE,
    MSAV__LACCESS_TOTAL_PAYLOAD,
    MSAV__LACCESS_PAYLOAD_CAPACITY,
    MSAV__LACCESS_MAX_UNITS,
    MSAV__LACCESS_ID,
    MSAV__LACCESS_SELECTED_BLOCK,
    MSAV__LACCESS_SELECTED_ROTATION,

    MSAV__LACCESS_ENABLED,
    MSAV__LACCESS_SHOOT,
    MSAV__LACCESS_SHOOTP,
    MSAV__LACCESS_CONFIG,
    MSAV__LACCESS_COLOR
} msav__logic_access_t;

typedef struct
{
    int32_t id;
} msav__unit_t;

typedef enum
{
    MSAV__UNIT_COMMAND_MOVE,
    MSAV__UNIT_COMMAND_REPAIR,
    MSAV__UNIT_COMMAND_REBUILD,
    MSAV__UNIT_COMMAND_ASSIST,
    MSAV__UNIT_COMMAND_MINE,
    MSAV__UNIT_COMMAND_BOOST,
    MSAV__UNIT_COMMAND_ENTER_PAYLOAD,
    MSAV__UNIT_COMMAND_LOAD_UNITS,
    MSAV__UNIT_COMMAND_LOAD_BLOCKS,
    MSAV__UNIT_COMMAND_UNLOAD_PAYLOAD,
    MSAV__UNIT_COMMAND_LOOP_PAYLOAD
} msav__unit_command_t;

typedef enum
{
    MSAV__OBJ_NULL,
    MSAV__OBJ_INT,
    MSAV__OBJ_LONG,
    MSAV__OBJ_FLOAT,
    MSAV__OBJ_STRING,
    MSAV__OBJ_CONTENT,
    MSAV__OBJ_INTSEQ,
    MSAV__OBJ_POINT2,
    MSAV__OBJ_POINT2_ARRAY,
    MSAV__OBJ_TECH_NODE,
    MSAV__OBJ_BOOL,
    MSAV__OBJ_DOUBLE,
    MSAV__OBJ_BUILDING,
    MSAV__OBJ_LACCESS,
    MSAV__OBJ_BYTES,
    MSAV__OBJ_UNIT_COMMAND_NULL,
    MSAV__OBJ_BOOL_ARRAY,
    MSAV__OBJ_UNIT,
    MSAV__OBJ_VEC2_ARRAY,
    MSAV__OBJ_VEC2,
    MSAV__OBJ_TEAM,
    MSAV__OBJ_INT_ARRAY,
    MSAV__OBJ_OBJECT_ARRAY,
    MSAV__OBJ_UNIT_COMMAND
} msav__obj_type_t;

typedef struct 
{
    msav__obj_type_t type;
    union
    {
        int32_t i;
        int64_t l;
        float f;
        char *s;
        msav__content_t c;
        msav__arr_t is; /* int32_t */
        msav__point2_t p;
        msav__arr_t pa; /* msav__point2_t */
        msav__content_t tn; /* May be merged into c */
        double d;
        msav__building bd;
        msav__bool b;
        msav__arr_t ba; /* msav__bool */
        msav__unit_t u;
        msav__arr_t va; /* msav__vec2_t */
        msav__vec2_t v;
        msav__team_t t;
        msav__arr_t ia; /* int32_t */
        msav__arr_t oa; /* msav__obj_t */
        msav__unit_command_t uc;

        msav__arr_t ga; /* generic array */
    };
} msav__obj_t;

typedef struct msav__rstream msav__rstream_t;
int msav__obj_read(msav__rstream_t *stream, msav__obj_t *obj);
void msav__obj_destroy(msav__obj_t *obj);

#endif
