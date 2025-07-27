#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x24];
    s32 *unk24;
    s32 *unk28;
} func_800BBA60_B7CA0_arg;

typedef struct {
    Mat3x3 matrix;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 PAD[0xC];
    s16 unk3C;
    s16 unk3E;
} func_800BB808_B7A48_arg;

typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4[3];
} D_800BBB90_Entry;

extern D_800BBB90_Entry D_800BBB90_B7DD0[];

typedef struct {
    s8 pad_0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    s8 pad_30[0xC];
    s32 unk3C;
    s8 pad_40[0x10];
    s8 unk50[0x0C];
    void *unk5C;
    void *unk60;
    void *unk64;
    void *unk68;
    s8 pad_6C[0xC];
    s32 unk78;
    s8 pad_7C[0x10];
    s32 unk8C;
    s8 pad_90[0x4];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s16 unkA4;
    s16 unkA6;
    s16 unkA8;
} TrackHazard;

typedef struct {
    Node n;
    u8 padding[0x7A];
    s16 unkA6;
} NodeWithPayload;

typedef struct {
    u8 padding[0x14];
    u8 padding2[0xC];
    s32 unk20;
    u8 padding3[0x18];
    u16 unk3C;
    s16 unk3E;
} func_800BB8B8_B7AF8_arg;

extern s32 func_800AFF44_9FE04(void);
extern s32 D_8009A8A4_9B4A4;
extern void createCombinedRotationMatrix(void *, u16, u16);
extern void func_8006B084_6BC84(void *, void *, void *);
extern s32 func_8005C60C_5D20C(void *a0, s32 a1, Player *a2);
extern void func_800589A0_595A0(Player *player);

void func_800BB468_B76A8(TrackHazard *arg0);
void func_800BB658_B7898(TrackHazard *arg0);
void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0);
void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *);
void func_800BB71C_B795C(TrackHazard *);
void func_800BB8B8_B7AF8(func_800BB8B8_B7AF8_arg *arg0);
void func_800547E0_553E0(s32, s32);

void func_800BB2B0_B74F0(TrackHazard *arg0) {
    GameState *gameState;
    s32 temp;
    u16 var_v1;
    func_80055E68_56A68_result *result;

    gameState = (GameState *)getCurrentAllocation();
    temp = func_800AFF44_9FE04();

    arg0->unkA6 += (temp & 1);
    result = func_80055E68_56A68(gameState->memoryPoolId);
    arg0->unk20 = &result->unk90;
    result = func_80055E68_56A68(gameState->memoryPoolId);
    arg0->unk5C = &result->unkA0;
    arg0->unk24 = func_80055DC4_569C4(gameState->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(gameState->memoryPoolId);
    arg0->unk2C = 0;
    arg0->unk98 = 0;
    arg0->unk9C = 0;
    arg0->unkA0 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    arg0->unk68 = arg0->unk2C;
    memcpy(&arg0->unk8C, &D_800BBB90_B7DD0[arg0->unkA6].unk4, 0xC);
    createCombinedRotationMatrix(&arg0->unk78, D_800BBB90_B7DD0[arg0->unkA6].unk0, D_800BBB90_B7DD0[arg0->unkA6].unk2);
    arg0->unkA4 = 0;

    setCleanupCallback((void (*)(void *))func_800BB7D0_B7A10);
    setCallback((void (*)(void *))func_800BB468_B76A8);
}

void func_800BB3B8_B75F8(TrackHazard *arg0) {
    s32 matrix[8]; // this should be matrix[9] but it causes stack issues
    s32 i;

    memcpy(&D_8009A8A4_9B4A4, &arg0->unk98, 0xC);
    func_8006B084_6BC84(&D_8009A8A4_9B4A4 - 5, &arg0->unk78, arg0);
    createXRotationMatrix(&matrix, arg0->unkA4);

    matrix[6] = 0x3b333;
    matrix[5] = 0;
    matrix[7] = 0x170000;

    func_8006B084_6BC84(&matrix[0], arg0, &arg0->unk3C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, arg0);
        enqueueDisplayListWithFrustumCull(i, &arg0->unk3C);
    }
}

void func_800BB468_B76A8(TrackHazard *arg0) {
    GameState *gs;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 flag;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    flag = 0;

    for (i = 0; i < gs->numPlayers; i++) {
        sp20 = gs->players[i].worldPosX - arg0->unk8C;
        sp28 = gs->players[i].worldPosZ - arg0->unk94;
        if (((0x27FFFFE >= ((u32)sp20) + 0x13FFFFF) & (0x13FFFFF >= sp28)) == 0) {
            continue;
        }

        if ((s32)0xFF600000 < sp28) {
            flag = 1;
            break;
        }
    }

    if (flag != 0) {
        if (gs->gamePaused == 0) {
            if (arg0->unk9C != 0x60000) {
                arg0->unk9C += 0x20000;
            }

            if (arg0->unkA4 != (-0x600)) {
                arg0->unkA4 -= 0x100;
            }
        }

        func_800BB3B8_B75F8(arg0);

        if (gs->gamePaused == 0) {
            for (i = 0; i < gs->numPlayers; i++) {
                if (func_8005C60C_5D20C(&arg0->unk50, 0x12A000, &gs->players[i]) != 0) {
                    if (func_8005C60C_5D20C(&arg0->unk50, 0x1E3000, &gs->players[i]) != 0) {
                        func_800589A0_595A0(&gs->players[i]);
                        func_80056B7C_5777C(&arg0->unk50, 0x2A);
                        setCallback((void (*)(void *))func_800BB658_B7898);
                    }
                }
            }
        }
    } else {
        if (gs->gamePaused == 0) {
            if (arg0->unk9C > 0) {
                arg0->unk9C += 0xFFFE0000;
            }

            if (arg0->unk9C < 0) {
                arg0->unk9C += 0x20000;
            }

            if (arg0->unkA4 != 0) {
                arg0->unkA4 += 0x100;
            }
        }

        func_800BB3B8_B75F8(arg0);
    }
}

void func_800BB658_B7898(TrackHazard *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    if (arg0->unk9C != 0xFFF00000) {
        if (gs->PAD_6[0x17] == 0) {
            arg0->unk9C = arg0->unk9C - 0x8000;
        }
    } else {
        arg0->unkA8 = 0x12C;
        setCallback((void (*)(void *))&func_800BB71C_B795C);
    }
    arg0->unkA4 = 0;
    func_800BB3B8_B75F8(arg0);

    for (i = 0; i < gs->numPlayers; i++) {
        func_8005C60C_5D20C(&arg0->unk50, 0x12A000, &gs->players[i]);
    }
}

void func_800BB71C_B795C(TrackHazard *arg0) {
    u8 temp;

    if (((GameState *)getCurrentAllocation())->PAD_6[0x17] == 0) {
        arg0->unkA8--;
    }

    if (!arg0->unkA8) {
        temp = func_800AFF44_9FE04() & 1;
        arg0->unkA6 = temp + (arg0->unkA6 & 0xFE);
        memcpy(&arg0->unk8C, &D_800BBB90_B7DD0[arg0->unkA6].unk4, 0xC);
        createCombinedRotationMatrix(
            &arg0->unk78,
            D_800BBB90_B7DD0[arg0->unkA6].unk0,
            D_800BBB90_B7DD0[arg0->unkA6].unk2
        );
        setCallback((void (*)(void *))&func_800BB468_B76A8);
    }
}

void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = (s32 *)freeGameStateMemory(arg0->unk24);
    arg0->unk28 = (s32 *)freeGameStateMemory(arg0->unk28);
}

void func_800BB808_B7A48(func_800BB808_B7A48_arg *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->unk24 = func_80055DC4_569C4(gs->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(gs->memoryPoolId);
    arg0->unk2C = 0;
    createYRotationMatrix(&arg0->matrix, 0x6C0);
    arg0->unk14 = 0xDD196FEA;
    arg0->unk18 = 0x0ABD4CA3;
    arg0->unk1C = 0xE270649E;
    arg0->unk3C = 0x12C;
    arg0->unk3E = 0;
    arg0->unk20 = (s32)(func_80055E68_56A68(gs->memoryPoolId)) + 0xB0;
    setCleanupCallback((void (*)(void *))&func_800BBA60_B7CA0);
    setCallback((void (*)(void *))&func_800BB8B8_B7AF8);
}

void func_800BB8B8_B7AF8(func_800BB8B8_B7AF8_arg *arg0) {
    GameState *gameState;
    s16 state;
    s32 i;
    u8 numPlayers;
    s32 randVal;

    gameState = getCurrentAllocation();
    state = arg0->unk3E;

    switch (state) {
        case 0:
            if (gameState->gamePaused == 0) {
                arg0->unk3C--;
                if ((arg0->unk3C << 16) == 0) {
                    arg0->unk3E++;
                }
            }
            break;
        case 1:
            if (gameState->gamePaused == 0) {
                numPlayers = gameState->numPlayers;

                for (i = 0; i < numPlayers; i++) {
                    if ((u32)gameState->players[i].unkB94 - 0x60 < 6) {
                        arg0->unk20 = (s32)func_80055E68_56A68(gameState->memoryPoolId) + 0xC0;
                        randVal = (u8)func_800AFF44_9FE04();
                        randVal = randVal - 0x60;
                        i = randVal << 1;
                        i = i + randVal;
                        i = i + 0x6C0;
                        func_800547E0_553E0(i, (((u8)func_800AFF44_9FE04()) << 12) | 0x100000);
                        func_80056B7C_5777C((void *)((u8 *)arg0 + 0x14), 0x23);
                        arg0->unk3C = 0x18;
                        arg0->unk3E++;
                        break;
                    }
                }
            }
            break;
        case 2:
            if (gameState->gamePaused == 0) {
                arg0->unk3C--;
                if ((arg0->unk3C << 16) == 0) {
                    arg0->unk3E = 0;
                    arg0->unk20 = (s32)func_80055E68_56A68(gameState->memoryPoolId) + 0xB0;
                    arg0->unk3C = 0x14;
                }
            }
            break;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, arg0);
    }
}

void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = (s32 *)freeGameStateMemory(arg0->unk24);
    arg0->unk28 = (s32 *)freeGameStateMemory(arg0->unk28);
}

void func_800BBA98(void) {
    s32 i;
    NodeWithPayload *temp;

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 0;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 2;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 4;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 6;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 8;
    }

    scheduleTask(&func_800BB808_B7A48, 0, 0, 0xC8);
}
