#ifndef GUARD_SAKIT_GLOBALS_H
#define GUARD_SAKIT_GLOBALS_H

#include "multi_sio.h"
#include "core.h"

#define GAME_MODE_SINGLE_PLAYER    0
#define GAME_MODE_TIME_ATTACK      1
#define GAME_MODE_BOSS_TIME_ATTACK 2

#define GAME_MODE_MULTI_PLAYER               3
#define GAME_MODE_TEAM_PLAY                  4
#define GAME_MODE_MULTI_PLAYER_COLLECT_RINGS 5

#define IS_SINGLE_PLAYER                                                                                                                   \
    ((gGameMode == GAME_MODE_SINGLE_PLAYER) || (gGameMode == GAME_MODE_TIME_ATTACK) || (gGameMode == GAME_MODE_BOSS_TIME_ATTACK))

#define IS_MULTI_PLAYER (!(IS_SINGLE_PLAYER))

#define STAGE_FLAG__CLEAR              0x0000
#define STAGE_FLAG__ACT_START          0x0001 // Turns timer off, likely other effects?
#define STAGE_FLAG__2                  0x0002
#define STAGE_FLAG__TIMER_REVERSED     0x0004 // Timer gets lower on every tick, and player loses life on 0:00:00.
#define STAGE_FLAG__10                 0x0010
#define STAGE_FLAG__DISABLE_PAUSE_MENU 0x0020
#define STAGE_FLAG__DEMO_RUNNING       0x0040 // TODO: Check accuracy of name
#define STAGE_FLAG__GRAVITY_INVERTED   0x0080
#define STAGE_FLAG__100                0x0100 // Set during stage's "loading screen"
#define STAGE_FLAG__TURN_OFF_TIMER     0x0200
#define STAGE_FLAG__TURN_OFF_HUD       0x0400

#define DIFFICULTY_NORMAL 0
#define DIFFICULTY_EASY   1

#define MAX_PLAYER_NAME_LENGTH 6

#define GRAVITY_IS_INVERTED (gStageFlags & STAGE_FLAG__GRAVITY_INVERTED)

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u16 fadeoutSpeed;
} MusicManagerState; /* size: 8 */

typedef struct {
    s32 squarePlayerDistance;
    u16 angle;
} HomingTarget;

// Common RoomEvent variables
#define ROOMEVENT_BASE u8 type

// RoomEvent opaque struct
// See 'multiplayer_event_mgr.h' for more information on this
typedef struct {
    ROOMEVENT_BASE;

    u8 opaque[7];
} RoomEvent; /* 0x8 */

#define CHEESE_DISTANCE_MAX 200
typedef struct {
    /* 0x00 */ s32 squarePlayerDistance;
    /* 0x04 */ struct Task *task;
} CheeseTarget; /* size: unknown (but >= 0x8?) */

// Incomplete
extern u8 gDemoPlayCounter;
extern u8 gGameMode;

extern s8 gCurrentLevel;
extern s8 gSelectedCharacter;

extern u8 gMultiplayerLanguage;

// Sometimes loaded as s16, but as u16 most of the time.
// If you encounter it being loaded as s16, please cast it.
extern u16 gRingCount;

extern MusicManagerState gMusicManagerState;

// Time that is being used to score the player (or in Time Attack)
extern u32 gCourseTime;

// Collected Special Rings in current stage?
extern u8 gSpecialRingCount;

// TODO: Types not checked yet!
extern s32 gUnknown_030054E0;
extern s32 gUnknown_030054FC;

extern u16 gBossCameraClampYLower;
extern u16 gBossCameraClampYUpper;
extern u8 gRandomItemBox;
extern u8 gUnknown_030053E0;

extern u8 gUnknown_0300543C;
extern struct Task *gEntitiesManagerTask;

extern u8 gDestroySpotlights;

extern u8 gRoomEventQueueSendPos;

// "Extra State" (see above #defines for states)
// TODO: Find better name. Put somewhere else?
extern u16 gStageFlags;
extern u16 gUnknown_0300544C;

extern u8 gDifficultyLevel;

extern s8 gTrappedAnimalVariant;

extern u8 gBossIndex; // TODO: Boss ID in XX-Stage? But it's used in checkpointc.c ...
extern u8 gUnknown_030054F8;

// Incremented by 1 every frame if the game is not paused.
// Starts before the stage-timer that is used for scores does.
extern u32 gStageTime;
extern u32 gMPStageStartFrameCount;

extern u32 gCheckpointTime; // Checkpoint timer?

extern u8 gRoomEventQueueWritePos;

extern u8 gBossRingsRespawnCount;
extern bool8 gBossRingsShallRespawn;
extern bool8 gUnknown_030055BC;

extern struct Task *gMultiplayerPlayerTasks[4];
extern s8 gMultiplayerCharacters[4];
extern s8 gUnknown_030054B4[4];
extern u8 gUnknown_030054B8;

extern u8 gMultiplayerMissingHeartbeats[4];
extern u8 gActiveCollectRingEffectCount;

extern u8 gMultiplayerUnlockedCharacters;
extern u8 gMultiplayerUnlockedLevels;
extern u32 gMultiplayerIds[MULTI_SIO_PLAYERS_MAX];
extern u16 gMultiplayerNames[MULTI_SIO_PLAYERS_MAX][MAX_PLAYER_NAME_LENGTH];

extern u32 gMultiplayerPseudoRandom;

extern s32 gLevelScore;
extern u8 gNumLives;
extern u8 gUnknown_030054B0;

extern HomingTarget gHomingTarget;

extern u8 gMultiplayerConnections;

// Only set after the player passed it, used to determine extra score
extern s32 gStageGoalX;

extern u8 gUnknown_03005428[4];
extern u8 gMultiplayerCharRings[MULTI_SIO_PLAYERS_MAX];

extern RoomEvent gRoomEventQueue[16];

extern CheeseTarget gCheeseTarget;

extern u8 gUnknown_030055D0[4];

#if 0
extern u8 gNewInputCountersIndex;
extern u8 gNewInputCounters[128];
#else

struct InputCounters {
    u8 unk0, unk1, unk2, unk3;
};

extern u8 gNewInputCountersIndex;
extern struct InputCounters gNewInputCounters[32];
#endif

extern u8 gUnknown_030055D8;

#if (GAME == GAME_SA1)
#define SET_LIVES_A(lives) (lives)
#else
#define SET_LIVES_A(lives)                                                                                                                 \
    ({                                                                                                                                     \
        if ((lives) > 255)                                                                                                                 \
            (lives) = 255;                                                                                                                 \
        gNumLives = (lives);                                                                                                               \
    })
#endif

// Sometimes this variant was used
#define SET_LIVES_B(lives)                                                                                                                 \
    ({                                                                                                                                     \
        if (lives > 255) {                                                                                                                 \
            gNumLives = 255;                                                                                                               \
        } else {                                                                                                                           \
            gNumLives = lives;                                                                                                             \
        };                                                                                                                                 \
    });

#define INCREMENT_SCORE_BASE(incVal, restartMusic, SetLives)                                                                               \
    {                                                                                                                                      \
        s32 divResA, divResB;                                                                                                              \
        s32 oldScore = gLevelScore;                                                                                                        \
        gLevelScore += incVal;                                                                                                             \
                                                                                                                                           \
        divResA = Div(gLevelScore, 50000);                                                                                                 \
        divResB = Div(oldScore, 50000);                                                                                                    \
                                                                                                                                           \
        if ((divResA != divResB) && (gGameMode == GAME_MODE_SINGLE_PLAYER)) {                                                              \
            u16 lives = divResA - divResB;                                                                                                 \
            lives += gNumLives;                                                                                                            \
                                                                                                                                           \
            SetLives(lives);                                                                                                               \
                                                                                                                                           \
            if (restartMusic)                                                                                                              \
                gMusicManagerState.unk3 = 0x10 | 0x0;                                                                                      \
        }                                                                                                                                  \
    }

#define INCREMENT_SCORE_A(incVal) INCREMENT_SCORE_BASE(incVal, FALSE, SET_LIVES_A)
#define INCREMENT_SCORE(incVal)   INCREMENT_SCORE_BASE(incVal, TRUE, SET_LIVES_A)
#define INCREMENT_SCORE_B(incVal) INCREMENT_SCORE_BASE(incVal, TRUE, SET_LIVES_B)

#define INCREMENT_RINGS(incVal)                                                                                                            \
    {                                                                                                                                      \
        s32 prevLives, newLives;                                                                                                           \
        s32 oldRings = gRingCount;                                                                                                         \
        gRingCount += incVal;                                                                                                              \
                                                                                                                                           \
        if (!(IS_EXTRA_STAGE(gCurrentLevel))) {                                                                                            \
            newLives = Div(gRingCount, 100);                                                                                               \
            prevLives = Div(oldRings, 100);                                                                                                \
                                                                                                                                           \
            if ((newLives != prevLives) && (gGameMode == GAME_MODE_SINGLE_PLAYER)) {                                                       \
                u16 lives = gNumLives + 1;                                                                                                 \
                                                                                                                                           \
                SET_LIVES_A(lives);                                                                                                        \
                                                                                                                                           \
                gMusicManagerState.unk3 = 0x10 | 0x0;                                                                                      \
            }                                                                                                                              \
        }                                                                                                                                  \
    }

#endif // GUARD_SAKIT_GLOBALS_H