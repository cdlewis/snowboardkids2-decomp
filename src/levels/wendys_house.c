#include "../3E160.h"
#include "56910.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} D_800BB930_B5B40_type;
extern D_800BB930_B5B40_type D_800BB930_B5B40[];

typedef struct {
    u8 _pad[0x5C];
    u8 memoryPoolId;
} AllocB5668;

typedef struct {
    u8 _pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad1[0xC];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u16 unk48;
    u16 unk4A;
    s16 unk4C;
    s16 unk4E;
} func_800BB458_B5668_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB420_B5630_arg;

typedef struct {
    s16 matrix[6];
    u8 _pad[0x8];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad2[0xC];
    s16 unk3C;
    u8 _pad3[0x2];
    s32 unk40;
    s32 unk44;
    s32 unk48;
} func_800BB2B0_arg;

void func_800BB5B0_B57C0(func_800BB458_B5668_arg *arg0);
void func_800BB458_B5668(func_800BB458_B5668_arg *arg0);
void func_800BB7B8_B59C8(func_800BB420_B5630_arg *arg0);
void func_800BB420_B5630(func_800BB420_B5630_arg *arg0);
void func_800BB350_B5560(func_800BB2B0_arg *arg0);

void func_800BB2B0_B54C0(func_800BB2B0_arg *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    arg0->unk20 = (void *)((u32)func_80055E68_56A68(gameState->memoryPoolId) + 0xA0);
    arg0->unk24 = func_80055DC4_569C4(gameState->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(gameState->memoryPoolId);
    arg0->unk14 = 0x03E90000;
    arg0->unk18 = 0x1D500000;
    arg0->unk1C = 0xF8460000;
    arg0->unk2C = 0;
    arg0->unk3C = 0;
    arg0->unk48 = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0xFE6A0000;
    setCleanupCallback(func_800BB420_B5630);
    setCallback(func_800BB350_B5560);
}

void func_800BB350_B5560(func_800BB2B0_arg *arg0) {
    s32 i;
    s32 temp[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->unk3C += 0x40;
    }

    createRotationMatrixYZ(arg0->matrix, (u16)(approximateSin(arg0->unk3C) >> 4), 0xF800);
    transformVector((s16 *)&arg0->unk40, arg0->matrix, temp);
    func_8005B730_5C330(temp, 0x500000, 0x100000, -0x800);

    if (gameState->gamePaused == 0) {
        if ((arg0->unk3C & 0xFFF) == 0) {
            func_80056B7C_5777C(temp, 0x27);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BB420_B5630(func_800BB420_B5630_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BB458_B5668(func_800BB458_B5668_arg *arg0) {
    AllocB5668 *gameState;
    void *temp;
    s32 randVal;
    s32 diff;
    gameState = (AllocB5668 *)getCurrentAllocation();
    arg0->unk24 = func_80055DC4_569C4(gameState->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(gameState->memoryPoolId);
    arg0->unk2C = 0;
    temp = func_80055E68_56A68(gameState->memoryPoolId);
    randVal = (randA() & 1) << 4;
    arg0->unk20 = temp + ((randVal) + 0xB0);
    arg0->unk14 = 0x225BCB0C;
    arg0->unk18 = D_800BB930_B5B40[((s16)arg0->unk4C)].unk4 + 0x1E8000;
    arg0->unk1C = 0xF14F9599;
    diff = D_800BB930_B5B40[arg0->unk4C].unk0 - arg0->unk14;
    if (diff < 0) {
        diff += 0x3F;
    }
    arg0->unk3C = diff >> 6;
    diff = D_800BB930_B5B40[arg0->unk4C].unk8 - arg0->unk1C;
    if (diff < 0) {
        diff += 0x3F;
    }
    arg0->unk44 = 6;
    arg0->unk44 = diff >> arg0->unk44;
    arg0->unk40 = 0x180000;
    arg0->unk48 = atan2Fixed(-arg0->unk3C, -arg0->unk44);
    arg0->unk4E = 0;
    setCleanupCallback(func_800BB7B8_B59C8);
    setCallback(func_800BB5B0_B57C0);
}

void func_800BB5B0_B57C0(func_800BB458_B5668_arg *arg0) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused != 0) {
        goto end;
    }

    switch (arg0->unk4E) {
        case 0:
            arg0->unk14 += arg0->unk3C;
            arg0->unk18 += arg0->unk40;
            arg0->unk1C += arg0->unk44;
            arg0->unk40 += (s32)0xFFFF4000;
            if (arg0->unk40 < (s32)0xFFE80000) {
                arg0->unk40 = 0xC0000;
                arg0->unk4E += 1;
                func_80056B7C_5777C(&arg0->unk14, 0x28);
            }
            arg0->unk4A += 0x100;
            for (i = 0; i < gameState->numPlayers; i++) {
                if (func_8005B400_5C000(&gameState->players[i], (Vec3i *)&arg0->unk14, 0x180000) != 0) {
                    func_80058B30_59730(&gameState->players[i]);
                }
            }
            break;
        case 1:
            arg0->unk14 += arg0->unk3C;
            arg0->unk18 += arg0->unk40;
            arg0->unk1C += arg0->unk44;
            for (i = 0; i < gameState->numPlayers; i++) {
                isPlayerInRangeAndPull((Vec3i *)&arg0->unk14, 0x180000, &gameState->players[i]);
            }
            arg0->unk40 += (s32)0xFFFF4000;
            if (arg0->unk40 < (s32)0xFFE20000) {
                func_80069CF8_6A8F8();
            }
            arg0->unk4A += 0x100;
            break;
    }

end:
    createCombinedRotationMatrix(arg0, arg0->unk4A, arg0->unk48);
    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BB7B8_B59C8(func_800BB420_B5630_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} func_800BB7F0_B5A00_arg;

typedef struct {
    u8 pad[0x4C];
    s16 unk4C;
} Task;

void func_800BB828_B5A38(func_800BB7F0_B5A00_arg *);

void func_800BB7F0_B5A00(func_800BB7F0_B5A00_arg *arg0) {
    arg0->unk2 = 0x50;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk6 = 0x28;
    setCallback(func_800BB828_B5A38);
}

void func_800BB828_B5A38(func_800BB7F0_B5A00_arg *arg0) {
    Task *task;
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        if (arg0->unk2 == 0) {
            arg0->unk0++;
            if (arg0->unk0 == 3) {
                arg0->unk0 = 0;
            }
            task = scheduleTask(func_800BB458_B5668, 0, 0, 0x5F);
            if (task != NULL) {
                task->unk4C = arg0->unk0;
            }
            arg0->unk2 = 0x50;
        } else {
            arg0->unk2--;
        }

        if (arg0->unk6 == 0) {
            arg0->unk4++;
            if (arg0->unk4 == 3) {
                arg0->unk4 = 0;
            }
            task = scheduleTask(func_800BB458_B5668, 0, 0, 0x5F);
            if (task != NULL) {
                task->unk4C = arg0->unk4 + 3;
            }
            arg0->unk6 = 0x50;
        } else {
            arg0->unk6--;
        }
    }
}
