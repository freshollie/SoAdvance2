#include "core.h"
#include "task.h"

#include "malloc_ewram.h"
#include "malloc_vram.h"

#include "lib/m4a.h"

#include "game/game.h"
#include "game/entity.h"
#include "game/stage/item_box.h"
#include "game/stage/entities_manager.h"

#include "game/interactables_0/platform_thin.h"
#include "game/interactables_0/044.h"
#include "game/interactables_0/grind_rail.h"
#include "game/interactables_0/platform_square.h"
#include "game/interactables_0/platform.h"
#include "game/interactables_0/ramp.h"
#include "game/interactables_0/slidy_ice.h"
#include "game/interactables_0/spring.h"
#include "game/interactables_0/clear_pipe.h"

#include "game/interactables_1/windmill.h"
#include "game/interactables_1/checkpoint.h"
#include "game/interactables_1/gapped_loop.h"
#include "game/interactables_1/stage_goal.h"
#include "game/interactables_1/corkscrew.h"
#include "game/interactables_1/corkscrew_3d.h"
#include "game/interactables_1/incline_ramp.h"
#include "game/interactables_1/toggle_player_layer.h"
#include "game/interactables_1/bouncy_bar.h"
#include "game/interactables_1/spring_bouncy.h"
#include "game/interactables_1/rotating_handle.h"
#include "game/interactables_1/019.h"
#include "game/interactables_1/spikes.h"
#include "game/interactables_1/booster.h"
#include "game/interactables_1/decoration.h"

#include "game/interactables_2/hot_crater/crane.h"
#include "game/interactables_2/hot_crater/dash_ring.h"
#include "game/interactables_2/hot_crater/wind_up_stick.h"
#include "game/interactables_2/hot_crater/floating_spring.h"
#include "game/interactables_2/hot_crater/turn_around_bar.h"
#include "game/interactables_2/hot_crater/hook_rail.h"

#include "game/interactables_2/music_plant/note_sphere.h"
#include "game/interactables_2/music_plant/pipe_horn.h"
#include "game/interactables_2/music_plant/keyboard.h"
#include "game/interactables_2/music_plant/note_block.h"
#include "game/interactables_2/music_plant/guitar_string.h"
#include "game/interactables_2/music_plant/german_flute.h"

#include "game/interactables_2/ice_paradise/069.h"
#include "game/interactables_2/ice_paradise/funnel_sphere.h"
#include "game/interactables_2/ice_paradise/half_pipe.h"
#include "game/interactables_2/ice_paradise/slowing_snow.h"

#include "game/interactables_2/sky_canyon/giant_propeller.h"
#include "game/interactables_2/sky_canyon/small_propeller.h"
#include "game/interactables_2/sky_canyon/small_windmill.h"
#include "game/interactables_2/sky_canyon/whirlwind.h"
#include "game/interactables_2/sky_canyon/085.h"

#include "game/interactables_2/tec_base/080.h"
#include "game/interactables_2/tec_base/light_bridge.h"
#include "game/interactables_2/tec_base/075.h"
#include "game/interactables_2/tec_base/078.h"
#include "game/interactables_2/tec_base/note_block.h"

#include "game/interactables_2/egg_utopia/094.h"
#include "game/interactables_2/egg_utopia/flying_handle.h"
#include "game/interactables_2/egg_utopia/gravity_toggle.h"
#include "game/interactables_2/egg_utopia/cannon.h"
#include "game/interactables_2/egg_utopia/095.h"
#include "game/interactables_2/egg_utopia/speeding_platform.h"
#include "game/interactables_2/egg_utopia/launcher.h"

#include "game/interactables_2/note_particle.h"
#include "game/interactables_2/105.h"
#include "game/interactables_2/special_ring.h"
#include "game/interactables_2/notification_ring_bonus.h"
#include "game/interactables_2/sky_canyon_init.h"

#include "game/mystery_item_box.h"

#include "game/enemies/kiki.h"
#include "game/enemies/kura_kura.h"
#include "game/enemies/flickey.h"
#include "game/enemies/kubinaga.h"
#include "game/enemies/hammerhead.h"
#include "game/enemies/bullet_buzzer.h"
#include "game/enemies/circus.h"
#include "game/enemies/yado.h"
#include "game/enemies/pen.h"
#include "game/enemies/gohla.h"
#include "game/enemies/bell.h"
#include "game/enemies/straw.h"
#include "game/enemies/kyura.h"
#include "game/enemies/balloon.h"
#include "game/enemies/mouse.h"
#include "game/enemies/koura.h"
#include "game/enemies/madillo.h"
#include "game/enemies/piko_piko.h"
#include "game/enemies/geji_geji.h"
#include "game/enemies/star.h"
#include "game/enemies/mon.h"
#include "game/enemies/buzzer.h"
#include "game/enemies/spinner.h"

#include "game/assets/compressed/entities.h"

#include "constants/zones.h"
#include "constants/songs.h"

// Unknown task
extern void CreateEnemyDefeatScore(s16, s16);

typedef struct Task *(*StagePreInitFunc)(void);
typedef void (*MapEntityInit)(MapEntity *, u16, u16, u8);

typedef struct {
    u32 unk0;
    u32 unk4; // h_regionCount
    u32 unk8; // v_regionCount
    u32 unkC[0]; // unknown size, offsets
} MapData; /* Unknown size */

typedef struct {
    MapData *unk0; // interactables
    MapData *unk4; // itemBoxes
    MapData *unk8; // enemies
    s32 unkC; // camX
    s32 unk10; // camY
    u8 unk14;
    struct Task *unk18; // preInitTask
} EntitesManager;

struct Range {
    s32 xLow, yLow;
    s32 xHigh, yHigh;
};

void sub_8008DCC(void);

void sub_80095FC(struct Task *);

const RLCompressed *const gSpritePosData_interactables[NUM_LEVEL_IDS] = {
    (void *)&zone1_act1_interactables,
    (void *)&zone1_act2_interactables,
    (void *)&zone1_boss_interactables,
    (void *)&zone1_act2_interactables,

    (void *)&zone2_act1_interactables,
    (void *)&zone2_act2_interactables,
    (void *)&zone2_boss_interactables,
    (void *)&zone2_act2_interactables,

    (void *)&zone3_act1_interactables,
    (void *)&zone3_act2_interactables,
    (void *)&zone3_boss_interactables,
    (void *)&zone3_act2_interactables,

    (void *)&zone4_act1_interactables,
    (void *)&zone4_act2_interactables,
    (void *)&zone4_boss_interactables,
    (void *)&zone4_act2_interactables,

    (void *)&zone5_act1_interactables,
    (void *)&zone5_act2_interactables,
    (void *)&zone5_boss_interactables,
    (void *)&zone5_act2_interactables,

    (void *)&zone6_act1_interactables,
    (void *)&zone6_act2_interactables,
    (void *)&zone6_boss_interactables,
    (void *)&zone6_act2_interactables,

    (void *)&zone7_act1_interactables,
    (void *)&zone7_act2_interactables,
    (void *)&zone7_boss_interactables,
    (void *)&zone7_act2_interactables,

    (void *)&zone8_act1_interactables,
    (void *)&zone8_act2_interactables,
    (void *)&zone8_boss_interactables,
    NULL,
    NULL,
    NULL,
};

const RLCompressed *const gSpritePosData_itemboxes[] = {
    (void *)&zone1_act1_itemboxes,
    (void *)&zone1_act2_itemboxes,
    (void *)&zone1_boss_itemboxes,
    (void *)&zone1_act2_itemboxes,

    (void *)&zone2_act1_itemboxes,
    (void *)&zone2_act2_itemboxes,
    (void *)&zone2_boss_itemboxes,
    (void *)&zone2_act2_itemboxes,

    (void *)&zone3_act1_itemboxes,
    (void *)&zone3_act2_itemboxes,
    (void *)&zone3_boss_itemboxes,
    (void *)&zone3_act2_itemboxes,

    (void *)&zone4_act1_itemboxes,
    (void *)&zone4_act2_itemboxes,
    (void *)&zone4_boss_itemboxes,
    (void *)&zone4_act2_itemboxes,

    (void *)&zone5_act1_itemboxes,
    (void *)&zone5_act2_itemboxes,
    (void *)&zone5_boss_itemboxes,
    (void *)&zone5_act2_itemboxes,

    (void *)&zone6_act1_itemboxes,
    (void *)&zone6_act2_itemboxes,
    (void *)&zone6_boss_itemboxes,
    (void *)&zone6_act2_itemboxes,

    (void *)&zone7_act1_itemboxes,
    (void *)&zone7_act2_itemboxes,
    (void *)&zone7_boss_itemboxes,
    (void *)&zone7_act2_itemboxes,

    (void *)&zone8_act1_itemboxes,
    (void *)&zone8_act2_itemboxes,
    (void *)&zone8_boss_itemboxes,
    NULL,
    NULL,
    NULL,
};
const RLCompressed *const gSpritePosData_enemies[] = {
    (void *)&zone1_act1_enemies,
    (void *)&zone1_act2_enemies,
    (void *)&zone1_boss_enemies,
    (void *)&zone1_act2_enemies,

    (void *)&zone2_act1_enemies,
    (void *)&zone2_act2_enemies,
    (void *)&zone2_boss_enemies,
    (void *)&zone2_act2_enemies,

    (void *)&zone3_act1_enemies,
    (void *)&zone3_act2_enemies,
    (void *)&zone3_boss_enemies,
    (void *)&zone3_act2_enemies,

    (void *)&zone4_act1_enemies,
    (void *)&zone4_act2_enemies,
    (void *)&zone4_boss_enemies,
    (void *)&zone4_act2_enemies,

    (void *)&zone5_act1_enemies,
    (void *)&zone5_act2_enemies,
    (void *)&zone5_boss_enemies,
    (void *)&zone5_act2_enemies,

    (void *)&zone6_act1_enemies,
    (void *)&zone6_act2_enemies,
    (void *)&zone6_boss_enemies,
    (void *)&zone6_act2_enemies,

    (void *)&zone7_act1_enemies,
    (void *)&zone7_act2_enemies,
    (void *)&zone7_boss_enemies,
    (void *)&zone7_act2_enemies,

    (void *)&zone8_act1_enemies,
    (void *)&zone8_act2_enemies,
    (void *)&zone8_boss_enemies,
    NULL,
    NULL,
    NULL,
};

const MapEntityInit gSpriteInits_Interactables[] = {
    CreateEntity_Toggle_PlayerLayer,
    CreateEntity_Toggle_PlayerLayer,
    CreateEntity_Platform,
    CreateEntity_Spring_Normal_Up,
    CreateEntity_Spring_Normal_Down,
    CreateEntity_Spring_Normal_Right,
    CreateEntity_Spring_Normal_Left,
    CreateEntity_Spring_Big_UpLeft,
    CreateEntity_Spring_Big_UpRight,
    CreateEntity_Spring_Big_DownRight,
    CreateEntity_Spring_Big_DownLeft,
    CreateEntity_Spring_Small_UpRight,
    CreateEntity_Spring_Small_UpLeft,
    CreateEntity_CommonThinPlatform,
    CreateEntity_Decoration,
    CreateEntity_Ramp,
    CreateEntity_Booster,
    CreateEntity_InclineRamp,
    CreateEntity_BouncySpring,
    CreateEntity_019,
    CreateEntity_RotatingHandle,
    CreateEntity_GappedLoop_Start,
    CreateEntity_GappedLoop_End,
    CreateEntity_Spikes_Up,
    CreateEntity_Spikes_Down,
    CreateEntity_Spikes_LeftRight,
    CreateEntity_Spikes_LeftRight,
    CreateEntity_Spikes_HidingUp,
    CreateEntity_Spikes_HidingDown,
    CreateEntity_Corkscrew_Start,
    CreateEntity_Corkscrew_End,
    CreateEntity_BouncyBar,
    CreateEntity_GrindRail_Start,
    CreateEntity_GrindRail_StartAir,
    CreateEntity_GrindRail_EndGround,
    CreateEntity_GrindRail_End_ForcedJump,
    CreateEntity_GrindRail_EndAlternate,
    CreateEntity_GrindRail_EndAir,
    CreateEntity_GrindRail_EndGround_Left,
    CreateEntity_GrindRail_EndAir_Left,
    CreateEntity_Corkscrew3D_Start,
    CreateEntity_Corkscrew3D_End,
    CreateEntity_StageGoal,
    CreateEntity_Checkpoint,
    CreateEntity_044,
    CreateEntity_Toggle_Checkpoint,
    CreateEntity_Toggle_StageGoal,
    CreateEntity_WindUpStick,
    CreateEntity_048,
    CreateEntity_HookRail_Start,
    CreateEntity_HookRail_End,
    CreateEntity_Windmill,
    CreateEntity_TurnAroundBar,
    CreateEntity_Crane,
    CreateEntity_DashRing,
    CreateEntity_FloatingSpring_Up,
    CreateEntity_Note_Sphere,
    CreateEntity_Note_Block,
    CreateEntity_GuitarString,
    CreateEntity_Keyboard_Vertical,
    CreateEntity_Keyboard_Horizontal_PushLeft,
    CreateEntity_Keyboard_Horizontal_PushRight,
    CreateEntity_GermanFlute,
    CreateEntity_FrenchHorn_Entry,
    CreateEntity_PipeInstrument_Entry,
    CreateEntity_SlowingSnow,
    CreateEntity_FunnelSphere,
    CreateEntity_HalfPipe_End,
    CreateEntity_HalfPipe_Start,
    CreateEntity_069,
    CreateEntity_NoteBlock,
    CreateEntity_044,
    CreateEntity_ClearPipe_Start,
    CreateEntity_ClearPipe_End,
    CreateEntity_LightBridge,
    CreateEntity_075_0,
    CreateEntity_075_1,
    CreateEntity_075_2,
    CreateEntity_078,
    CreateEntity_079,
    CreateEntity_080,
    CreateEntity_Platform_Square,
    CreateEntity_GiantPropeller,
    CreateEntity_SlidyIce,
    CreateEntity_SmallSpinnyWindmill,
    CreateEntity_085,
    CreateEntity_Whirlwind_0,
    CreateEntity_SmallPropeller_Left,
    CreateEntity_SmallPropeller_Right,
    CreateEntity_SmallPropeller_Left_Periodic,
    CreateEntity_SmallPropeller_Right_Periodic,
    CreateEntity_Launcher_Left_GDown,
    CreateEntity_Launcher_Right_GDown,
    CreateEntity_Cannon,
    CreateEntity_094,
    CreateEntity_095,
    CreateEntity_FlyingHandle,
    CreateEntity_SpeedingPlatform,
    CreateEntity_Toggle_Gravity__Down,
    CreateEntity_Toggle_Gravity__Up,
    CreateEntity_Toggle_Gravity__Toggle,
    CreateEntity_Launcher_Left_GUp,
    CreateEntity_Launcher_Right_GUp,
    CreateEntity_SpecialRing,
    CreateEntity_Whirlwind_1,
    CreateEntity_Interactable105,
};

const MapEntityInit gSpriteInits_Enemies[] = {
    CreateEntity_Mon,      CreateEntity_Kiki,         CreateEntity_Buzzer,
    CreateEntity_Gohla,    CreateEntity_KuraKura,     CreateEntity_Kubinaga,
    CreateEntity_PikoPiko, CreateEntity_Bell,         CreateEntity_Yado,
    CreateEntity_Circus,   CreateEntity_Koura,        CreateEntity_Madillo,
    CreateEntity_Straw,    CreateEntity_Hammerhead,   CreateEntity_Spinner,
    CreateEntity_Mouse,    CreateEntity_Pen,          CreateEntity_GejiGeji,
    CreateEntity_Balloon,  CreateEntity_Flickey,      CreateEntity_Kyura,
    CreateEntity_Star,     CreateEntity_BulletBuzzer,
};

#define NUN_ENEMY_DEFEAT_SCORES 5
const u16 enemyDefeatScores[NUN_ENEMY_DEFEAT_SCORES] = {
    100, 200, 400, 800, 1000,
};

const MapEntityInit gUnknown_080D502C[] = {
    CreateEntity_Toggle_PlayerLayer,
    CreateEntity_Toggle_PlayerLayer,
    CreateEntity_Spring_Normal_Up,
    CreateEntity_Spring_Normal_Down,
    CreateEntity_Spring_Normal_Right,
    CreateEntity_Spring_Normal_Left,
    CreateEntity_Spring_Big_UpLeft,
    CreateEntity_Spring_Big_UpRight,
    CreateEntity_Spring_Big_DownLeft,
    CreateEntity_Spring_Big_DownRight,
    CreateEntity_Spring_Small_UpLeft,
    CreateEntity_Spring_Small_UpRight,
    CreateEntity_Ramp,
    CreateEntity_Spikes_Up,
    CreateEntity_Spikes_Up,
    CreateEntity_Spikes_Up,
    CreateEntity_Spikes_Up,
    CreateEntity_Spikes_Up,
    CreateEntity_Spikes_Up,
    CreateEntity_GrindRail_Start,
    CreateEntity_GrindRail_StartAir,
    CreateEntity_GrindRail_EndGround,
    CreateEntity_GrindRail_End_ForcedJump,
    CreateEntity_GrindRail_EndAlternate,
    CreateEntity_GrindRail_EndAir,
    CreateEntity_GrindRail_EndGround_Left,
    CreateEntity_GrindRail_EndAir_Left,
    CreateEntity_MysteryItemBox,
    CreateEntity_8080368,
};

const StagePreInitFunc gSpriteTileInits_PreStageEntry[] = {
    NULL,        NULL,        NULL, NULL, // Leaf Forest
    NULL,        NULL,        NULL, NULL, // Hot Crater
    NULL,        NULL,        NULL, NULL, // Music Plant
    NULL,        NULL,        NULL, NULL, // Ice Paradise
    sub_80807CC, sub_80807CC, NULL, NULL, // Sky Canyon
    NULL,        NULL,        NULL, NULL, // Techno Base
    NULL,        NULL,        NULL, NULL, // Egg Utopia
    NULL,        NULL,        NULL, NULL, // Final Zone
    NULL,        NULL,
};

void CreateStageEntitiesManager(void)
{
    void *decompBuf;
    struct Task *t;
    EntitesManager *em;
    if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
        t = TaskCreate(sub_8008DCC, sizeof(EntitesManager), 0x2000, 0, sub_80095FC);
    } else {
        t = TaskCreate(sub_8008DCC, sizeof(EntitesManager), 0x2000, 0, NULL);
    }

    em = TaskGetStructPtr(t);

    if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
        decompBuf = EwramMalloc(gSpritePosData_interactables[gCurrentLevel]->size);
        RLUnCompWram(gSpritePosData_interactables[gCurrentLevel], decompBuf);
        em->unk0 = decompBuf;

        decompBuf = EwramMalloc(gSpritePosData_itemboxes[gCurrentLevel]->size);
        RLUnCompWram(gSpritePosData_itemboxes[gCurrentLevel], decompBuf);
        em->unk4 = decompBuf;

        decompBuf = EwramMalloc(gSpritePosData_enemies[gCurrentLevel]->size);
        RLUnCompWram(gSpritePosData_enemies[gCurrentLevel], decompBuf);
        em->unk8 = decompBuf;

        em->unk18 = NULL;

        if (gSpriteTileInits_PreStageEntry[gCurrentLevel]) {
            em->unk18 = gSpriteTileInits_PreStageEntry[gCurrentLevel]();
        }
    } else {
        decompBuf = (void *)EWRAM_START + 0x3F000;
        RLUnCompWram(*(void **)((void *)EWRAM_START + 0x3300C), decompBuf);
        em->unk0 = decompBuf;
    }

    em->unkC = gCamera.x;
    em->unk10 = gCamera.y;
    em->unk14 = 1;
    gEntitesManagerTask = t;
}

static inline MapEntity *ReadMe(void *data, u32 r6)
{
    register u32 offset asm("r0") = r6 - 8;
    return data + offset;
}

// https://decomp.me/scratch/Co5bs
// logic is correct. Compiling differently for some reason
NONMATCH("asm/non_matching/sub_80089CC.inc", void sub_80089CC())
{
    if (!(gUnknown_03005424 & 2)) {
        u32 pos;

        u32 regionY, regionX;

        struct Range range;
        u32 h_regionCount, v_regionCount;

        EntitesManager *em = TaskGetStructPtr(gCurTask);
        u32 *interactables;
        u32 *itemBoxPositions;
        u32 *enemyPositions;

        interactables = (u32 *)em->unk0;
        if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
            enemyPositions = em->unk8->unkC;
            itemBoxPositions = em->unk4->unkC;
        }

        interactables++;
        h_regionCount = (u16)*interactables++;
        v_regionCount = (u16)*interactables++;

        range.xLow = gCamera.x - 0x80;
        range.xHigh = gCamera.x + 0x170;

        range.yLow = gCamera.y - 0x80;
        range.yHigh = gCamera.y + 0x120;

        if (range.xLow < 0) {
            range.xLow = 0;
        }
        if (range.yLow < 0) {
            range.yLow = 0;
        }
        if (range.xHigh < 0) {
            range.xHigh = 0;
        }
        if (range.yHigh < 0) {
            range.yHigh = 0;
        }

        if ((u32)range.xLow >= Q_24_8(h_regionCount)) {
            range.xLow = Q_24_8(h_regionCount) - 1;
        }
        if ((u32)range.yLow >= Q_24_8(v_regionCount)) {
            range.yLow = Q_24_8(v_regionCount) - 1;
        }

        if ((u32)range.xHigh >= Q_24_8(h_regionCount)) {
            range.xHigh = Q_24_8(h_regionCount) - 1;
        }

        if ((u32)range.yHigh >= Q_24_8(v_regionCount)) {
            range.yHigh = Q_24_8(v_regionCount) - 1;
        }

        regionY = Q_24_8_TO_INT(range.yLow);
        while (Q_24_8(regionY) < (u32)range.yHigh && regionY < v_regionCount) {

            regionX = Q_24_8_TO_INT(range.xLow);
            while (Q_24_8(regionX) < (u32)range.xHigh && regionX < h_regionCount) {
                u32 posY;
                u32 i;
                register u32 r6 asm("r6");
                MapEntity *me;
                u32 sp28;

                sp28 = ({
                    u32 r0 = h_regionCount * regionY;
                    r0 *= 4;
                    r0 += (u32)interactables;
                    r0;
                });
                if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
                    r6 = *(u32 *)(sp28 + (regionX * 4));
                    if (r6 != 0) {
                        me = ReadMe(interactables, r6);
                        for (i = 0; (s8)me->x != -1; me++, i++) {
                            if ((s8)me->x >= -2) {
                                s32 x = TO_WORLD_POS(me->x, regionX);
                                s32 y = TO_WORLD_POS(me->y, regionY);
                                if (x >= range.xLow && x <= range.xHigh
                                    && y >= range.yLow && y <= range.yHigh) {
                                    gSpriteInits_Interactables[me->index](me, regionX,
                                                                          regionY, i);
                                }
                            }
                        }
                    }

                    r6 = *(u32 *)({
                        u32 r0 = h_regionCount * regionY;
                        r0 *= 4;
                        r0 += (u32)itemBoxPositions;
                        r0 += (regionX * 4);
                        r0;
                    });

                    if (r6 != 0) {
                        MapEntitySmall *me
                            = (MapEntitySmall *)ReadMe(itemBoxPositions, r6);
                        for (i = 0; (s8)me->x != -1; me++, i++) {
                            if ((s8)me->x >= -2) {
                                s32 x = TO_WORLD_POS(me->x, regionX);
                                s32 y = TO_WORLD_POS(me->y, regionY);
                                if (x >= range.xLow && x <= range.xHigh
                                    && y >= range.yLow && y <= range.yHigh) {
                                    CreateEntity_ItemBox((void *)me, regionX, regionY,
                                                         i);
                                }
                            }
                        }
                    }

                    sp28 = ({
                        u32 r0 = h_regionCount * regionY;
                        r0 *= 4;
                        r0 += (u32)enemyPositions;
                        r0 += (regionX * 4);
                        r0;
                    });
                    r6 = *(u32 *)sp28;
                    if (r6 != 0) {
                        me = ReadMe(enemyPositions, r6);
                        for (i = 0; (s8)me->x != -1; me++, i++) {

                            if ((s8)me->x >= -2) {
                                s32 x = TO_WORLD_POS(me->x, regionX);
                                s32 y = TO_WORLD_POS(me->y, regionY);
                                if (x >= range.xLow && x <= range.xHigh
                                    && y >= range.yLow && y <= range.yHigh) {
                                    gSpriteInits_Enemies[me->index](me, regionX, regionY,
                                                                    i);
                                }
                            }
                        }
                    }

                } else {
                    r6 = *(u32 *)(sp28 + (regionX * 4));
                    if (r6 != 0) {
                        me = ReadMe(interactables, r6);
                        for (i = 0; (s8)me->x != -1; me++, i++) {
                            if ((s8)me->x >= -2) {
                                s32 x = TO_WORLD_POS(me->x, regionX);
                                s32 y = TO_WORLD_POS(me->y, regionY);
                                if (x >= range.xLow && x <= range.xHigh
                                    && y >= range.yLow && y <= range.yHigh) {
                                    gUnknown_080D502C[me->index](me, regionX, regionY,
                                                                 i);
                                }
                            }
                        }
                    }
                }
                regionX++;
            }
            regionY++;
        }
        em->unkC = gCamera.x;
        em->unk10 = gCamera.y;
        em->unk14 = 0;
        gCurTask->main = sub_8008DCC;
    }
}
END_NONMATCH

// https://decomp.me/scratch/pOpmU
NONMATCH("asm/non_matching/sub_8008DCC.inc", void sub_8008DCC())
{
    if (!(gUnknown_03005424 & 2)) {
        u32 pos;

        u16 regionX, regionY;
        u32 temp, temp2, temp3, temp4;
        s32 temp5;
        MapEntity *me;
        u32 i;
        register u32 r6 asm("r6");
        u32 sp28;

        struct Range range1;
        struct Range range2;

        u32 h_regionCount, v_regionCount;

        EntitesManager *em = TaskGetStructPtr(gCurTask);

        if (em->unk14 != 0) {
            sub_80089CC();
            return;
        }
        if ((gCamera.x - em->unkC >= 0 ? gCamera.x - em->unkC : em->unkC - gCamera.x)
            > 248) {
            sub_80089CC();
            return;
        }
        if ((gCamera.y - em->unk10 >= 0 ? gCamera.y - em->unk10 : em->unk10 - gCamera.y)
            > 208) {
            sub_80089CC();
            return;
        }
        {
            u32 *interactables;
            u32 *itemBoxPositions;
            u32 *enemyPositions;

            interactables = (u32 *)em->unk0;
            if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
                enemyPositions = em->unk8->unkC;
                itemBoxPositions = em->unk4->unkC;
            }

            interactables++;
            h_regionCount = (u16)*interactables++;
            v_regionCount = (u16)*interactables++;

            if (gCamera.x > em->unkC) {
                range1.xLow = em->unkC + 0x170;
                range1.xHigh = gCamera.x + 0x170;
            } else {
                range1.xLow = gCamera.x - 0x80;
                range1.xHigh = em->unkC - 0x80;
            }

            if (em->unk14 != 0) {
                sub_80089CC();
                return;
            }

            range1.yLow = gCamera.y - 0x80;
            range1.yHigh = gCamera.y + 0x120;

            if (gCamera.y > em->unk10) {
                range2.yLow = em->unk10 + 0x120;
                range2.yHigh = gCamera.y + 0x120;
            } else {
                range2.yLow = gCamera.y - 0x80;
                range2.yHigh = em->unk10 - 0x80;
            }

            range2.xLow = gCamera.x - 0x80;
            range2.xHigh = gCamera.x + 0x170;

            if ((s32)range1.xLow < 0) {
                range1.xLow = 0;
            }
            if ((s32)range1.yLow < 0) {
                range1.yLow = 0;
            }
            if ((s32)range1.xHigh < 0) {
                range1.xHigh = 0;
            }

            if ((s32)range1.yHigh < 0) {
                range1.yHigh = 0;
            }

            temp2 = range1.xLow;
            temp = Q_24_8(h_regionCount);
            if (temp2 >= temp) {
                range1.xLow = temp - 1;
            }

            temp3 = range1.yLow;
            temp4 = Q_24_8(v_regionCount);

            if (temp3 >= temp4) {
                range1.yLow = temp4 - 1;
            }

            if ((u32)range1.xHigh >= temp) {
                range1.xHigh = temp - 1;
            }

            if (range1.yHigh >= temp4) {
                range1.yHigh = temp4 - 1;
            }

            if (range2.xLow < 0) {
                range2.xLow = 0;
            }

            if (range2.yLow < 0) {
                range2.yLow = 0;
            }

            if ((s32)range2.xHigh < 0) {
                range2.xHigh = 0;
            }

            if ((s32)range2.yHigh < 0) {
                range2.yHigh = 0;
            }

            if ((u32)range2.xLow >= temp) {
                range2.xLow = temp - 1;
            }

            if ((u32)range2.yLow >= temp4) {
                range2.yLow = temp4 - 1;
            }

            if ((u32)range2.xHigh >= temp) {
                range2.xHigh = temp - 1;
            }

            if ((u32)range2.yHigh >= temp4) {
                range2.yHigh = temp4 - 1;
            }

            if (gCamera.x != em->unkC && range1.xLow != range1.xHigh
                && range1.yLow != range1.yHigh) {
                regionY = Q_24_8_TO_INT(range1.yLow);

                while (Q_24_8(regionY) < range1.yHigh && regionY < v_regionCount) {
                    regionX = Q_24_8_TO_INT((s32)range1.xLow);
                    while (Q_24_8(regionX) < range1.xHigh && regionX < h_regionCount) {

                        sp28 = ({
                            u32 r0 = h_regionCount * regionY;
                            r0 *= 4;
                            r0 += (u32)interactables;
                            r0;
                        });

                        if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
                            r6 = *(u32 *)(sp28 + (regionX * 4));
                            if (r6 != 0) {
                                me = ReadMe(interactables, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range1.xLow
                                            && x <= (s32)range1.xHigh
                                            && y >= (s32)range1.yLow
                                            && y <= (s32)range1.yHigh) {
                                            gSpriteInits_Interactables[me->index](
                                                me, regionX, regionY, i);
                                        }
                                    }
                                }
                            }

                            r6 = *(u32 *)({
                                u32 r0 = h_regionCount * regionY;
                                r0 *= 4;
                                r0 += (u32)itemBoxPositions;
                                r0 += (regionX * 4);
                                r0;
                            });

                            if (r6 != 0) {
                                MapEntitySmall *me
                                    = (MapEntitySmall *)ReadMe(itemBoxPositions, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range1.xLow
                                            && x <= (s32)range1.xHigh
                                            && y >= (s32)range1.yLow
                                            && y <= (s32)range1.yHigh) {
                                            CreateEntity_ItemBox((MapEntity *)me,
                                                                 regionX, regionY, i);
                                        }
                                    }
                                }
                            }

                            sp28 = ({
                                u32 r0 = h_regionCount * regionY;
                                r0 *= 4;
                                r0 += (u32)enemyPositions;
                                r0 += (regionX * 4);
                                r0;
                            });
                            r6 = *(u32 *)sp28;
                            if (r6 != 0) {
                                me = ReadMe(enemyPositions, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range1.xLow
                                            && x <= (s32)range1.xHigh
                                            && y >= (s32)range1.yLow
                                            && y <= (s32)range1.yHigh) {
                                            gSpriteInits_Enemies[me->index](me, regionX,
                                                                            regionY, i);
                                        }
                                    }
                                }
                            }

                        } else {
                            r6 = *(u32 *)(sp28 + (regionX * 4));
                            if (r6 != 0) {
                                me = ReadMe(interactables, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range1.xLow
                                            && x <= (s32)range1.xHigh
                                            && y >= (s32)range1.yLow
                                            && y <= (s32)range1.yHigh) {
                                            gUnknown_080D502C[me->index](me, regionX,
                                                                         regionY, i);
                                        }
                                    }
                                }
                            }
                        }
                        regionX++;
                    }
                    regionY++;
                }
            }
            if (((gCamera.y != em->unk10) && (range2.yLow != range2.yHigh))
                && (range2.xLow != range2.xHigh)) {
                regionY = Q_24_8_TO_INT((s32)range2.yLow);

                while (Q_24_8(regionY) < range2.yHigh && regionY < v_regionCount) {
                    regionX = Q_24_8_TO_INT((s32)range2.xLow);
                    while (Q_24_8(regionX) < range2.xHigh && regionX < h_regionCount) {

                        sp28 = ({
                            u32 r0 = h_regionCount * regionY;
                            r0 *= 4;
                            r0 += (u32)interactables;
                            r0;
                        });

                        if (gGameMode != GAME_MODE_MULTI_PLAYER_COLLECT_RINGS) {
                            r6 = *(u32 *)(sp28 + (regionX * 4));
                            if (r6 != 0) {
                                me = ReadMe(interactables, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range2.xLow
                                            && x <= (s32)range2.xHigh
                                            && y >= (s32)range2.yLow
                                            && y <= (s32)range2.yHigh) {
                                            gSpriteInits_Interactables[me->index](
                                                me, regionX, regionY, i);
                                        }
                                    }
                                }
                            }

                            r6 = *(u32 *)({
                                u32 r0 = h_regionCount * regionY;
                                r0 *= 4;
                                r0 += (u32)itemBoxPositions;
                                r0 += (regionX * 4);
                                r0;
                            });

                            if (r6 != 0) {
                                MapEntitySmall *me
                                    = (MapEntitySmall *)ReadMe(itemBoxPositions, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range2.xLow
                                            && x <= (s32)range2.xHigh
                                            && y >= (s32)range2.yLow
                                            && y <= (s32)range2.yHigh) {
                                            CreateEntity_ItemBox((MapEntity *)me,
                                                                 regionX, regionY, i);
                                        }
                                    }
                                }
                            }

                            sp28 = ({
                                u32 r0 = h_regionCount * regionY;
                                r0 *= 4;
                                r0 += (u32)enemyPositions;
                                r0 += (regionX * 4);
                                r0;
                            });
                            r6 = *(u32 *)sp28;
                            if (r6 != 0) {
                                me = ReadMe(enemyPositions, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range2.xLow
                                            && x <= (s32)range2.xHigh
                                            && y >= (s32)range2.yLow
                                            && y <= (s32)range2.yHigh) {
                                            gSpriteInits_Enemies[me->index](me, regionX,
                                                                            regionY, i);
                                        }
                                    }
                                }
                            }

                        } else {
                            r6 = *(u32 *)(sp28 + (regionX * 4));
                            if (r6 != 0) {
                                me = ReadMe(interactables, r6);
                                for (i = 0; (s8)me->x != -1; me++, i++) {
                                    if ((s8)me->x >= -2) {
                                        s32 x = TO_WORLD_POS(me->x, regionX);
                                        s32 y = TO_WORLD_POS(me->y, regionY);
                                        if (x >= (s32)range2.xLow
                                            && x <= (s32)range2.xHigh
                                            && y >= (s32)range2.yLow
                                            && y <= (s32)range2.yHigh) {
                                            gUnknown_080D502C[me->index](me, regionX,
                                                                         regionY, i);
                                        }
                                    }
                                }
                            }
                        }
                        regionX++;
                    }
                    regionY++;
                }
            }
            em->unkC = gCamera.x;
            em->unk10 = gCamera.y;
        }
    }
}
END_NONMATCH

void sub_8009530(s16 x, s16 y)
{
    u32 old;
    u32 temp1;
    u32 temp2;
    m4aSongNumStart(SE_ITEM_BOX);
    old = gUnknown_03005450;
    gUnknown_03005450 += enemyDefeatScores[gPlayer.defeatScoreIndex];
    temp1 = Div(gUnknown_03005450, 50000);
    temp2 = Div(old, 50000);
    if (temp1 != temp2 && gGameMode == GAME_MODE_SINGLE_PLAYER) {
        u16 numLives = (temp1 - temp2);
        numLives += gNumLives;
        if (numLives > 255) {
            gNumLives = 255;
        } else {
            gNumLives = numLives;
        }
        gUnknown_030054A8.unk3 = 0x10;
    }

    CreateEnemyDefeatScore(x, y);

    // NOTE: This should be (ARRAY_COUNT(enemyDefeatScores) - 1)
    //       But padding makes it (6-1) instead of (5-1),
    //       hence the macro.
    if (gPlayer.defeatScoreIndex < (NUN_ENEMY_DEFEAT_SCORES - 1)) {
        gPlayer.defeatScoreIndex++;
    }
}

void TaskDestructor_80095E8(struct Task *t)
{
    Sprite_Entity *s = TaskGetStructPtr(t);
    VramFree(s->displayed.graphics.dest);
}

void sub_80095FC(struct Task *t)
{
    EntitesManager *em = TaskGetStructPtr(t);
    EwramFree(em->unk0);
    EwramFree(em->unk4);
    EwramFree(em->unk8);
    gEntitesManagerTask = NULL;
}