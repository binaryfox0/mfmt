#include "msav_entities_gen.h"

#include <string.h>

static const char *const msav_entity_0_aliases[] = { "-unit-legacy-alpha", "alpha" };
static const char *const msav_entity_0_direct[] = { "UnitComp" };
static const char *const msav_entity_0_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_1_aliases[] = { "-block-unit-unit", "block" };
static const char *const msav_entity_1_direct[] = { "BlockUnitComp", "UnitComp" };
static const char *const msav_entity_1_components[] = { "BlockUnitComp", "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_2_aliases[] = { "-unit", "antumbra", "avert", "eclipse", "flare", "horizon", "obviate", "zenith" };
static const char *const msav_entity_2_direct[] = { "UnitComp" };
static const char *const msav_entity_2_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_3_aliases[] = { "-mech-unit", "crawler", "dagger", "fortress", "mace", "reign", "scepter", "vela" };
static const char *const msav_entity_3_direct[] = { "MechComp", "UnitComp" };
static const char *const msav_entity_3_components[] = { "BuilderComp", "DrawComp", "ElevationMoveComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MechComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_4_aliases[] = { "-payload-unit", "disrupt", "emanate", "evoke", "incite", "mega", "quell" };
static const char *const msav_entity_4_direct[] = { "PayloadComp", "UnitComp" };
static const char *const msav_entity_4_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PayloadComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_5_aliases[] = { "-building" };
static const char *const msav_entity_5_direct[] = { "BuildingComp" };
static const char *const msav_entity_5_components[] = { "BuildingComp", "EntityComp", "HealthComp", "PosComp", "TeamComp", "TimerComp" };
static const char *const msav_entity_6_aliases[] = { "-bullet" };
static const char *const msav_entity_6_direct[] = { "BulletComp" };
static const char *const msav_entity_6_components[] = { "BulletComp", "DamageComp", "DrawComp", "EntityComp", "HitboxComp", "OwnerComp", "PosComp", "ShielderComp", "TeamComp", "TimedComp", "TimerComp" };
static const char *const msav_entity_7_aliases[] = { "-decal" };
static const char *const msav_entity_7_direct[] = { "DecalComp" };
static const char *const msav_entity_7_components[] = { "DecalComp", "DrawComp", "EntityComp", "PosComp", "RotComp", "TimedComp" };
static const char *const msav_entity_8_aliases[] = { "-effect-state" };
static const char *const msav_entity_8_direct[] = { "ChildComp", "EffectStateComp" };
static const char *const msav_entity_8_components[] = { "ChildComp", "DrawComp", "EffectStateComp", "EntityComp", "PosComp", "RotComp", "TimedComp" };
static const char *const msav_entity_9_aliases[] = { "-fire" };
static const char *const msav_entity_9_direct[] = { "FireComp" };
static const char *const msav_entity_9_components[] = { "DrawComp", "EntityComp", "FireComp", "PosComp", "SyncComp", "TimedComp" };
static const char *const msav_entity_10_aliases[] = { "-launch-core" };
static const char *const msav_entity_10_direct[] = { "LaunchCoreComp" };
static const char *const msav_entity_10_components[] = { "DrawComp", "EntityComp", "LaunchCoreComp", "PosComp", "TimedComp" };
static const char *const msav_entity_11_aliases[] = { "-player" };
static const char *const msav_entity_11_direct[] = { "PlayerComp" };
static const char *const msav_entity_11_components[] = { "DrawComp", "EntityComp", "PlayerComp", "PosComp", "SyncComp", "TimerComp" };
static const char *const msav_entity_12_aliases[] = { "-puddle" };
static const char *const msav_entity_12_direct[] = { "PuddleComp" };
static const char *const msav_entity_12_components[] = { "DrawComp", "EntityComp", "PosComp", "PuddleComp", "SyncComp" };
static const char *const msav_entity_13_aliases[] = { "-weather-state" };
static const char *const msav_entity_13_direct[] = { "WeatherStateComp" };
static const char *const msav_entity_13_components[] = { "DrawComp", "EntityComp", "PosComp", "SyncComp", "WeatherStateComp" };
static const char *const msav_entity_14_aliases[] = { "-launch-payload" };
static const char *const msav_entity_14_direct[] = { "LaunchPayloadComp" };
static const char *const msav_entity_14_components[] = { "DrawComp", "EntityComp", "LaunchPayloadComp", "PosComp", "TeamComp", "TimedComp" };
static const char *const msav_entity_15_aliases[] = { "-unit-legacy-mono", "mono" };
static const char *const msav_entity_15_direct[] = { "UnitComp" };
static const char *const msav_entity_15_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_16_aliases[] = { "-mech-unit-legacy-nova", "nova" };
static const char *const msav_entity_16_direct[] = { "MechComp", "UnitComp" };
static const char *const msav_entity_16_components[] = { "BuilderComp", "DrawComp", "ElevationMoveComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MechComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_17_aliases[] = { "-unit-legacy-poly", "poly" };
static const char *const msav_entity_17_direct[] = { "UnitComp" };
static const char *const msav_entity_17_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_18_aliases[] = { "-mech-unit-legacy-pulsar", "pulsar" };
static const char *const msav_entity_18_direct[] = { "MechComp", "UnitComp" };
static const char *const msav_entity_18_components[] = { "BuilderComp", "DrawComp", "ElevationMoveComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MechComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_19_aliases[] = { "-unit-water-move", "aegires", "bryde", "cyerce", "minke", "navanax", "omura", "oxynoe", "retusa", "risso", "sei" };
static const char *const msav_entity_19_direct[] = { "UnitComp", "WaterMoveComp" };
static const char *const msav_entity_19_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WaterMoveComp", "WeaponsComp" };
static const char *const msav_entity_20_aliases[] = { "-legs-unit-legacy-spiroct", "spiroct" };
static const char *const msav_entity_20_direct[] = { "LegsComp", "UnitComp" };
static const char *const msav_entity_20_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "LegsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_21_aliases[] = { "-payload-unit-legacy-quad", "quad" };
static const char *const msav_entity_21_direct[] = { "PayloadComp", "UnitComp" };
static const char *const msav_entity_21_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PayloadComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_22_aliases[] = { "-legs-unit", "anthicus", "atrax", "cleroi", "collaris", "corvus", "merui", "tecta" };
static const char *const msav_entity_22_direct[] = { "LegsComp", "UnitComp" };
static const char *const msav_entity_22_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "LegsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_23_aliases[] = { "-payload-unit-legacy-oct", "oct" };
static const char *const msav_entity_23_direct[] = { "PayloadComp", "UnitComp" };
static const char *const msav_entity_23_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PayloadComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_24_aliases[] = { "-pos-team" };
static const char *const msav_entity_24_direct[] = { "TeamComp" };
static const char *const msav_entity_24_components[] = { "EntityComp", "PosComp", "TeamComp" };
static const char *const msav_entity_25_aliases[] = { "-legs-unit-legacy-arkyid", "arkyid" };
static const char *const msav_entity_25_direct[] = { "LegsComp", "UnitComp" };
static const char *const msav_entity_25_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "LegsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_26_aliases[] = { "-unit-legacy-beta", "beta" };
static const char *const msav_entity_26_direct[] = { "UnitComp" };
static const char *const msav_entity_26_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_27_aliases[] = { "-unit-legacy-gamma", "gamma" };
static const char *const msav_entity_27_direct[] = { "UnitComp" };
static const char *const msav_entity_27_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_28_aliases[] = { "-mech-unit-legacy-quasar", "quasar" };
static const char *const msav_entity_28_direct[] = { "MechComp", "UnitComp" };
static const char *const msav_entity_28_components[] = { "BuilderComp", "DrawComp", "ElevationMoveComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MechComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_29_aliases[] = { "-legs-unit-legacy-toxopid", "toxopid" };
static const char *const msav_entity_29_direct[] = { "LegsComp", "UnitComp" };
static const char *const msav_entity_29_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "LegsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_30_aliases[] = { "-world-label" };
static const char *const msav_entity_30_direct[] = { "WorldLabelComp" };
static const char *const msav_entity_30_components[] = { "DrawComp", "EntityComp", "PosComp", "SyncComp", "WorldLabelComp" };
static const char *const msav_entity_31_aliases[] = { "-building-tether-payload-unit", "assembly-drone", "assemblyDrone", "manifold" };
static const char *const msav_entity_31_direct[] = { "BuildingTetherComp", "PayloadComp", "UnitComp" };
static const char *const msav_entity_31_components[] = { "BuilderComp", "BuildingTetherComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PayloadComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_32_aliases[] = { "-timed-kill-unit", "missile" };
static const char *const msav_entity_32_direct[] = { "TimedKillComp", "UnitComp" };
static const char *const msav_entity_32_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "TimedKillComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_33_aliases[] = { "-power-graph-updater" };
static const char *const msav_entity_33_direct[] = { "PowerGraphUpdaterComp" };
static const char *const msav_entity_33_components[] = { "EntityComp", "PowerGraphUpdaterComp" };
static const char *const msav_entity_34_aliases[] = { "-tank-unit", "conquer", "locus", "precept", "stell", "vanquish" };
static const char *const msav_entity_34_direct[] = { "TankComp", "UnitComp" };
static const char *const msav_entity_34_components[] = { "BuilderComp", "DrawComp", "ElevationMoveComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TankComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_35_aliases[] = { "-elevation-move-unit", "elude" };
static const char *const msav_entity_35_direct[] = { "ElevationMoveComp", "UnitComp" };
static const char *const msav_entity_35_components[] = { "BuilderComp", "DrawComp", "ElevationMoveComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_36_aliases[] = { "-crawl-unit", "latum", "renale" };
static const char *const msav_entity_36_direct[] = { "CrawlComp", "UnitComp" };
static const char *const msav_entity_36_components[] = { "BuilderComp", "CrawlComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };
static const char *const msav_entity_37_aliases[] = { "-target-dummy-unit", "dummy" };
static const char *const msav_entity_37_direct[] = { "TargetDummyComp", "UnitComp" };
static const char *const msav_entity_37_components[] = { "BuilderComp", "DrawComp", "EntityComp", "HealthComp", "HitboxComp", "ItemsComp", "MinerComp", "PhysicsComp", "PosComp", "RotComp", "ShieldComp", "StatusComp", "SyncComp", "TargetDummyComp", "TeamComp", "UnitComp", "VelComp", "WeaponsComp" };

const msav_entity_info_t msav_entity_db[] =
{
    { 0, "UnitLegacyAlpha", msav_entity_0_aliases, 2, msav_entity_0_direct, 1, msav_entity_0_components, 17 },
    { 2, "BlockUnitUnit", msav_entity_1_aliases, 2, msav_entity_1_direct, 2, msav_entity_1_components, 18 },
    { 3, "Unit", msav_entity_2_aliases, 8, msav_entity_2_direct, 1, msav_entity_2_components, 17 },
    { 4, "MechUnit", msav_entity_3_aliases, 8, msav_entity_3_direct, 2, msav_entity_3_components, 19 },
    { 5, "PayloadUnit", msav_entity_4_aliases, 7, msav_entity_4_direct, 2, msav_entity_4_components, 18 },
    { 6, "Building", msav_entity_5_aliases, 1, msav_entity_5_direct, 1, msav_entity_5_components, 6 },
    { 7, "Bullet", msav_entity_6_aliases, 1, msav_entity_6_direct, 1, msav_entity_6_components, 11 },
    { 8, "Decal", msav_entity_7_aliases, 1, msav_entity_7_direct, 1, msav_entity_7_components, 6 },
    { 9, "EffectState", msav_entity_8_aliases, 1, msav_entity_8_direct, 2, msav_entity_8_components, 7 },
    { 10, "Fire", msav_entity_9_aliases, 1, msav_entity_9_direct, 1, msav_entity_9_components, 6 },
    { 11, "LaunchCore", msav_entity_10_aliases, 1, msav_entity_10_direct, 1, msav_entity_10_components, 5 },
    { 12, "Player", msav_entity_11_aliases, 1, msav_entity_11_direct, 1, msav_entity_11_components, 6 },
    { 13, "Puddle", msav_entity_12_aliases, 1, msav_entity_12_direct, 1, msav_entity_12_components, 5 },
    { 14, "WeatherState", msav_entity_13_aliases, 1, msav_entity_13_direct, 1, msav_entity_13_components, 5 },
    { 15, "LaunchPayload", msav_entity_14_aliases, 1, msav_entity_14_direct, 1, msav_entity_14_components, 6 },
    { 16, "UnitLegacyMono", msav_entity_15_aliases, 2, msav_entity_15_direct, 1, msav_entity_15_components, 17 },
    { 17, "MechUnitLegacyNova", msav_entity_16_aliases, 2, msav_entity_16_direct, 2, msav_entity_16_components, 19 },
    { 18, "UnitLegacyPoly", msav_entity_17_aliases, 2, msav_entity_17_direct, 1, msav_entity_17_components, 17 },
    { 19, "MechUnitLegacyPulsar", msav_entity_18_aliases, 2, msav_entity_18_direct, 2, msav_entity_18_components, 19 },
    { 20, "UnitWaterMove", msav_entity_19_aliases, 11, msav_entity_19_direct, 2, msav_entity_19_components, 18 },
    { 21, "LegsUnitLegacySpiroct", msav_entity_20_aliases, 2, msav_entity_20_direct, 2, msav_entity_20_components, 18 },
    { 23, "PayloadUnitLegacyQuad", msav_entity_21_aliases, 2, msav_entity_21_direct, 2, msav_entity_21_components, 18 },
    { 24, "LegsUnit", msav_entity_22_aliases, 8, msav_entity_22_direct, 2, msav_entity_22_components, 18 },
    { 26, "PayloadUnitLegacyOct", msav_entity_23_aliases, 2, msav_entity_23_direct, 2, msav_entity_23_components, 18 },
    { 28, "PosTeam", msav_entity_24_aliases, 1, msav_entity_24_direct, 1, msav_entity_24_components, 3 },
    { 29, "LegsUnitLegacyArkyid", msav_entity_25_aliases, 2, msav_entity_25_direct, 2, msav_entity_25_components, 18 },
    { 30, "UnitLegacyBeta", msav_entity_26_aliases, 2, msav_entity_26_direct, 1, msav_entity_26_components, 17 },
    { 31, "UnitLegacyGamma", msav_entity_27_aliases, 2, msav_entity_27_direct, 1, msav_entity_27_components, 17 },
    { 32, "MechUnitLegacyQuasar", msav_entity_28_aliases, 2, msav_entity_28_direct, 2, msav_entity_28_components, 19 },
    { 33, "LegsUnitLegacyToxopid", msav_entity_29_aliases, 2, msav_entity_29_direct, 2, msav_entity_29_components, 18 },
    { 35, "WorldLabel", msav_entity_30_aliases, 1, msav_entity_30_direct, 1, msav_entity_30_components, 5 },
    { 36, "BuildingTetherPayloadUnit", msav_entity_31_aliases, 4, msav_entity_31_direct, 3, msav_entity_31_components, 19 },
    { 39, "TimedKillUnit", msav_entity_32_aliases, 2, msav_entity_32_direct, 2, msav_entity_32_components, 18 },
    { 42, "PowerGraphUpdater", msav_entity_33_aliases, 1, msav_entity_33_direct, 1, msav_entity_33_components, 2 },
    { 43, "TankUnit", msav_entity_34_aliases, 6, msav_entity_34_direct, 2, msav_entity_34_components, 19 },
    { 45, "ElevationMoveUnit", msav_entity_35_aliases, 2, msav_entity_35_direct, 2, msav_entity_35_components, 18 },
    { 46, "CrawlUnit", msav_entity_36_aliases, 3, msav_entity_36_direct, 2, msav_entity_36_components, 18 },
    { 49, "TargetDummyUnit", msav_entity_37_aliases, 2, msav_entity_37_direct, 2, msav_entity_37_components, 18 }
};

const size_t msav_entity_db_count = sizeof(msav_entity_db) / sizeof(msav_entity_db[0]);

const msav_entity_info_t *msav_entity_by_id(int32_t id)
{
    for(size_t i = 0; i < msav_entity_db_count; i++)
        if(msav_entity_db[i].id == id) return &msav_entity_db[i];
    return NULL;
}

const msav_entity_info_t *msav_entity_by_name(const char *name)
{
    if(!name) return NULL;
    for(size_t i = 0; i < msav_entity_db_count; i++)
    {
        const msav_entity_info_t *entity = &msav_entity_db[i];
        if(!strcmp(entity->name, name)) return entity;
        for(size_t j = 0; j < entity->alias_count; j++)
            if(!strcmp(entity->aliases[j], name)) return entity;
    }
    return NULL;
}
