#ifndef GUARD_STAGE_PLAYER_H
#define GUARD_STAGE_PLAYER_H

#include "sakit/player.h"

#include "game/parameters/characters.h"

// Actual type of 'type8029A28' currently unknown, rename once it is
typedef s32 type8029A28;

void Player_SetMovestate_IsInScriptedSequence(void);
void Player_ClearMovestate_IsInScriptedSequence(void);

void InitializePlayer(Player *p);
void sub_80218E4(Player *);
void sub_8023B5C(Player *, s8);
void sub_8023260(Player *);
void sub_80232D0(Player *);
void sub_8023610(Player *);
void PlayerCB_8025318(Player *p);
void PlayerCB_80261D8(Player *p);
void sub_8027EF0(Player *p);
void sub_8028204(Player *p);
void sub_80282EC(Player *p);
void sub_80283C4(Player *p);
void sub_8029C84(Player *p);
void sub_8029FA4(Player *p);

void Player_DisableInputAndBossTimer(void);
void Player_DisableInputAndBossTimer_FinalBoss(void);

s32 sub_8029B88(Player *player, u8 *p1, int *out);
s32 sub_8029AC0(Player *player, u8 *p1, s32 *out);
s32 sub_8029B0C(Player *player, u8 *p1, s32 *out);

type8029A28 sub_8029A28(Player *player, u8 *p1, type8029A28 *out);
type8029A28 sub_8029A74(Player *player, u8 *p1, type8029A28 *out);

bool32 sub_8029E6C(Player *);

#define GET_CHARACTER_ANIM(player)                                                      \
    (player->anim - gPlayerCharacterIdleAnims[player->character])

#define PLAYERFN_SET(proc)          gPlayer.callback = proc;
#define PLAYERFN_CALL(proc, player) proc(player);
#define PLAYERFN_SET_AND_CALL(proc, player)                                             \
    {                                                                                   \
        PLAYERFN_SET(proc);                                                             \
        PLAYERFN_CALL(proc, player);                                                    \
    }

#define PLAYERFN_SET_SHIFT_OFFSETS(player, x, y)                                        \
    {                                                                                   \
        player->unk16 = x;                                                              \
        player->unk17 = y;                                                              \
    }
#define PLAYERFN_CHANGE_SHIFT_OFFSETS(player, x, y)                                     \
    {                                                                                   \
        sub_8023B5C(player, y);                                                         \
        PLAYERFN_SET_SHIFT_OFFSETS(player, x, y)                                        \
    }

extern const u16 gUnknown_080D6736[][2];

#endif // GUARD_STAGE_PLAYER_H