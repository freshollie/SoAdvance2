#include "trig.h"

#include "sakit/globals.h"

#include "game/stage/player.h"
#include "game/stage/camera.h"
#include "sakit/collision.h"

#include "game/bosses/common.h"

#include "game/cheese.h"

#include "lib/m4a.h"

#include "constants/songs.h"
#include "constants/player_transitions.h"

void sub_8049D1C(struct Task *);
void sub_80499D8(void);

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 unk4;
    u8 unk5;
    u8 unk6;
} UNK_80498CC;

static const u16 unused = 0;

const u16 gUnknown_080D87D8[] = {
    6400, 13888, 18016, 23104, 29152, 38080, 43091,
};

const u16 gUnknown_080D87E6[][2] = {
    { 5804, 177 },  { 13292, 177 }, { 17420, 177 }, { 22508, 177 },
    { 28585, 177 }, { 37484, 201 }, { 41600, 225 }, { 41600, 225 },
};

static const u16 unused2 = 0;

// const s32 gUnknown_080D8808[][2] = {
//     { 5376, -4512 },  { 12864, -5952 }, { 17088, -2688 }, { 22080, -3552 },
//     { 28128, -4512 }, { 37058, -7394 }, { 42240, -3648 },
// };

void sub_80498CC(u8 bossNum)
{
    struct Task *t = TaskCreate(sub_80499D8, 8, 0x4000, 0, sub_8049D1C);
    UNK_80498CC *unk8CC = TASK_DATA(t);
    unk8CC->unk4 = bossNum;
    unk8CC->unk5 = 0;
    unk8CC->unk6 = 0;
    unk8CC->unk0 = gUnknown_080D5964[unk8CC->unk4][0];
    unk8CC->unk2 = gUnknown_080D5964[unk8CC->unk4][1];

    gUnknown_03005440 = gUnknown_080D5964[unk8CC->unk4][0];
    gUnknown_030054BC = gUnknown_080D5964[unk8CC->unk4][1];

    if (bossNum != 0) {
        s32 x, y;
        Cheese *cheese;
        Player_DisableInputAndBossTimer();

        gPlayer.speedGroundX = 0x500;

        // ???
        x = gUnknown_080D87E6[bossNum - 1][0] - I(gPlayer.x);
        y = gUnknown_080D87E6[bossNum][1] - I(gPlayer.y);
        gPlayer.x += Q(x);
        gPlayer.y += Q(y);

        gCamera.x += x - 0x78;
        gCamera.y += y - 0x78;

        gCamera.unk20 += x - 0x78;
        gCamera.unk24 += y - 0x78;
        gCamera.unk10 += x - 0x78;
        gCamera.unk14 += y - 0x78;

        cheese = gCheese;
        if (cheese != NULL) {
            cheese->posX += Q(x);
            cheese->posY += Q(y);
        }
        unk8CC->unk4--;
    }
}

extern const s32 gUnknown_080D8808[][2];

void sub_80499D8(void)
{
    UNK_80498CC *unk8CC = TASK_DATA(gCurTask);
#ifndef NON_MATCHING
    register s32 r5 asm("r5");
    register s32 r1 asm("r1");
    register s32 r0 asm("r0");
    register s32 r6 asm("r6");
#else
    s32 r5, r1, r0, r6;
#endif
    s32 r4;

    if (unk8CC->unk4 <= 6) {
        if (unk8CC->unk4 == 6) {
            gUnknown_03005440 = gUnknown_080D5964[7][0];
            gUnknown_030054BC = gUnknown_080D5964[7][1];
            if (gPlayer.x < Q(42960) && gPlayer.x > Q(gUnknown_080D8808[6][0] + 0x1E)) {
                gPlayer.moveState |= MOVESTATE_IGNORE_INPUT;
                gPlayer.speedGroundX = 0x500;
                gPlayer.unk5E = 0;
                gPlayer.unk5C = 0;
                gPlayer.rotation = 0;
                if (I(gPlayer.x) - 0x78 != gCamera.x) {
                    if (I(gPlayer.x) - 0x78 > gCamera.x) {
                        gCamera.unk20++;
                        gCamera.x++;
                        gCamera.unk10++;
                    } else {
                        gCamera.unk20--;
                        gCamera.x--;
                        gCamera.unk10--;
                    }
                }
            } else if (gPlayer.x < Q(42700)
                       && gPlayer.x > Q(gUnknown_080D8808[6][0] + 0x14)) {
                if (gActiveBossTask == NULL && !(gStageFlags & 0x100)) {
                    gPlayer.checkpointTime = gCourseTime;
                    CreateZoneBoss(7);
                    gStageFlags &= ~EXTRA_STATE__2;
                }
            } else {
                r5 = gPlayer.x;
                if (r5 > Q(42960)) {
                    gPlayer.moveState &= ~MOVESTATE_8000000;
                    gPlayer.speedGroundX = 0;
                    gPlayer.transition = 1;
                    unk8CC->unk4++;
                    unk8CC->unk6 = 1;
                } else if (gActiveBossTask != NULL && r5 < Q(42300)) {
                    r1 = gUnknown_080D8808[unk8CC->unk4][0];
                    if (r1 >= 0) {
                        r0 = Q(r1);
                        if (r5 >= r0) {
                            r4 = gUnknown_080D8808[unk8CC->unk4][1];
                            r6 = Q(r4);
                            r0 = r5 + r6;
                            gPlayer.x = r0;
                            gUnknown_030054FC = r6;
                            sub_8039F50(r6, unk8CC->unk4);
                            gBossRingsShallRespawn = 1;
                            gCamera.x += r4;
                            gCamera.unk20 += r4;
                            gCamera.unk10 += r4;
                            if (gCheese != NULL) {
                                gCheese->posX += r6;
                            }
                        }
                    }
                }
            }
        } else if (unk8CC->unk4 <= 5) {
            if (gCamera.unk10 > gUnknown_080D87D8[unk8CC->unk4]) {
                gBossRingsShallRespawn = 1;
                gBossRingsRespawnCount = 10;
                gPlayer.checkpointTime = gCourseTime;
                CreateZoneBoss(++unk8CC->unk4);
                gStageFlags &= ~EXTRA_STATE__2;
            }

            if (gUnknown_080D8808[unk8CC->unk4][0] >= 0 && gActiveBossTask != NULL
                && gPlayer.x >= Q(gUnknown_080D8808[unk8CC->unk4][0])) {
                r4 = gUnknown_080D8808[unk8CC->unk4][1];
                r5 = Q(r4);
                gPlayer.x += r5;
                gUnknown_030054FC = r5;
                sub_8039F50(r5, unk8CC->unk4);
                gBossRingsShallRespawn = 1;
                gCamera.x += r4;
                gCamera.unk20 += r4;
                gCamera.unk10 += r4;
                if (gCheese != NULL) {
                    gCheese->posX += r5;
                }
            }
        }
    }

    if (unk8CC->unk5 < 7 && gCamera.unk10 > gUnknown_080D87D8[unk8CC->unk5] - 600) {
        if (unk8CC->unk5 == 6) {
            if (unk8CC->unk6 == 0) {
                gCamera.unkC = 1;
                gCamera.unk8 = 0x500;
            }
            gUnknown_030054B0 = 1;
        } else {
            unk8CC->unk5++;
        }
    }

    if (unk8CC->unk0 != gUnknown_080D5964[unk8CC->unk5][0]) {
        if (unk8CC->unk0 < gUnknown_080D5964[unk8CC->unk5][0]) {
            unk8CC->unk0++;
        } else {
            unk8CC->unk0--;
        }
    }

    if (unk8CC->unk2 != gUnknown_080D5964[unk8CC->unk5][1]) {
        if (unk8CC->unk2 < gUnknown_080D5964[unk8CC->unk5][1]) {
            unk8CC->unk2++;
        } else {
            unk8CC->unk2--;
        }
    }

    gUnknown_03005440 = unk8CC->unk0;
    gUnknown_030054BC = unk8CC->unk2;
}

void sub_8049D1C(struct Task *t) { }