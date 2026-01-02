#include "36B80.h"
#include "3E160.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6DE50.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *assetTable;
    void *indicatorAsset;
} PlayerIndicatorTask;

typedef struct {
    u8 padding[0x8];
    s16 unk8;
    s16 padding2;
    s16 unkC;
    s16 unkE;
} func_80040E00_41A00_arg;

extern s32 gFrameCounter;
extern s32 D_800907F8_913F8;
extern s32 D_800907EC_913EC[];
extern s16 identityMatrix[];

extern void func_80040870_41470(void);
extern void *D_80090860_91460;
extern void func_8004119C_41D9C(void);

typedef struct {
    Transform3D rotationMatrix; // 0x00-0x1F, with position at 0x14-0x1F
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad[0xC];
    Transform3D unk3C;
    void *unk5C;
    void *unk60;
    void *unk64;
    s32 unk68;
    u8 pad2[0xC];
    Transform3D unk78;
    s32 unk98;
    void *unk9C;
    void *unkA0;
    s32 unkA4;
    u8 pad3[0xC];
    s16 unkB4;
    s16 unkB6;
    s16 unkB8;
} func_80040B4C_4174C_arg;

void func_80040B4C_4174C(func_80040B4C_4174C_arg *);
void func_80040E00_41A00(func_80040E00_41A00_arg *);
void func_80040F34_41B34(PlayerIndicatorTask *);
void func_800413E0_41FE0(PlayerIndicatorTask *arg0);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    u8 _pad2[0x10];
    s32 unk38;
} func_8004083C_4143C_arg;

typedef struct {
    u8 _pad[0xBDE];
    u8 unkBDE;
} func_8004083C_4143C_struct24;

typedef struct {
    void *unk0;
} func_80040948_41548_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80040D48_41948_arg;

void func_80040D48_41948(func_80040D48_41948_arg *);

typedef struct {
    u8 high;
    u8 low;
} Bytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytesC;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytesE;

typedef struct {
    void *unk0;
    void *unk4;
    s16 unk8;
    s16 unkA;
    S16OrBytesC unkC;
    S16OrBytesE unkE;
    S16OrBytes unk10;
    s16 unk12;
} func_80040D80_41980_arg;

void func_80040E4C_41A4C(func_80040D80_41980_arg *);

void awaitPlayerIndicatorReady(func_8004083C_4143C_arg *arg0);
void cleanupPlayerIndicator(func_80040948_41548_arg *arg0);

void initPlayerIndicator(PlayerIndicatorTask *task) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    task->assetTable = load_3ECE40();
    task->indicatorAsset = (u8 *)&gameState->unk44->unk1380;
    setCleanupCallback(&cleanupPlayerIndicator);
    setCallbackWithContinue(&awaitPlayerIndicatorReady);
}

void awaitPlayerIndicatorReady(func_8004083C_4143C_arg *arg0) {
    func_8004083C_4143C_struct24 *player;

    player = (func_8004083C_4143C_struct24 *)arg0->unk24;
    if (player->unkBDE != 0) {
        arg0->unk38 = 6;
        setCallback(func_80040870_41470);
    }
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040870_41470);

void cleanupPlayerIndicator(func_80040948_41548_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void spawnPlayerIndicatorTask(void *cleanupArg) {
    Node *task = scheduleTask(&initPlayerIndicator, 0U, 0U, 0xF0U);
    if (task != NULL) {
        task->cleanupCallback = cleanupArg;
    }
}

void func_800409B4_415B4(func_80040B4C_4174C_arg *obj) {
    s32 tempMatrix[8];
    s32 worldPos[4];
    s32 *transformMatrix;
    GameState *gameState;
    D_80090F90_91B90_item *spawnData;
    u16 trackAngle;

    gameState = (GameState *)getCurrentAllocation();
    spawnData = func_80055D10_56910(gameState->memoryPoolId);
    obj->unk20 = (void *)((u8 *)func_80055E68_56A68(gameState->memoryPoolId) + 0x50);
    obj->unk24 = func_80055DC4_569C4(gameState->memoryPoolId);
    obj->unk28 = func_80055DF8_569F8(gameState->memoryPoolId);
    obj->unk2C = 0;
    trackAngle = func_800625A4_631A4((u8 *)gameState + 0x30, worldPos);
    createYRotationMatrix(&obj->rotationMatrix, (trackAngle + spawnData->unk8) & 0xFFFF);
    rotateVectorY(&D_800907EC_913EC, trackAngle + spawnData->unk8, &obj->rotationMatrix.translation);
    obj->rotationMatrix.translation.x = obj->rotationMatrix.translation.x + spawnData->unk0;
    obj->rotationMatrix.translation.z = obj->rotationMatrix.translation.z + spawnData->unk4;
    obj->rotationMatrix.translation.y = worldPos[1];
    obj->unk5C = (void *)((u8 *)func_80055E68_56A68(gameState->memoryPoolId) + 0x60);
    transformMatrix = tempMatrix;
    obj->unk60 = obj->unk24;
    obj->unk64 = obj->unk28;
    obj->unk68 = obj->unk2C;
    memcpy(transformMatrix, identityMatrix, 0x20);
    transformMatrix[6] = 0x180000;
    func_8006B084_6BC84(transformMatrix, obj, &obj->unk3C);
    obj->unk9C = obj->unk24;
    obj->unkA0 = obj->unk28;
    obj->unkA4 = obj->unk2C;
    transformMatrix[6] = 0x160000;
    transformMatrix[7] = 0xA3333;
    func_8006B084_6BC84(transformMatrix, obj, &obj->unk78);
    obj->unkB4 = 0;
    obj->unkB6 = 0;
    setCleanupCallback(func_80040D48_41948);
    setCallback(func_80040B4C_4174C);
}

void func_80040B4C_4174C(func_80040B4C_4174C_arg *arg0) {
    Transform3D sp10;
    Transform3D *s0;
    GameState *s2;
    s32 i;

    s2 = (GameState *)getCurrentAllocation();

    switch (arg0->unkB6) {
        case 0:
            if (s2->unk63 != 3) {
                goto block_else;
            }
            arg0->unkB6++;
            func_80056B7C_5777C(&arg0->rotationMatrix.translation, 0xA);
            /* fallthrough */
        case 1:
            if (s2->gamePaused == 0) {
                arg0->unkB4 -= 0x80;
            }
            if (arg0->unkB4 == -0x800) {
                arg0->unkB8 = 0xA;
                arg0->unkB6++;
            }
            createZRotationMatrix(&sp10, arg0->unkB4);
            sp10.translation.x = 0;
            sp10.translation.y = 0xC0000;
            sp10.translation.z = 0;
            func_8006B084_6BC84(&sp10, arg0, &arg0->unk3C);
            break;
        case 2:
            if (s2->gamePaused == 0) {
                arg0->unkB8--;
            }
            if (arg0->unkB8 == 0) {
                arg0->unkB6++;
            }
            break;
        case 3:
            if (s2->gamePaused == 0) {
                arg0->unkB4 += 0x100;
            }
            if (arg0->unkB4 == 0) {
                arg0->unkB6++;
            }
            s0 = &arg0->unk3C;
            createZRotationMatrix(s0, arg0->unkB4);
            createZRotationMatrix(&sp10, arg0->unkB4);
            sp10.translation.x = 0;
            sp10.translation.y = 0xC0000;
            sp10.translation.z = 0;
            func_8006B084_6BC84(&sp10, arg0, s0);
            break;
        case 4:
            if (s2->unk63 != 3) {
                arg0->unkB6 = 0;
            }
            break;
    }

    if (s2->unk63 == 3) {
        arg0->unk98 = (s32)func_80055E68_56A68(s2->memoryPoolId) + 0x70;
    } else {
    block_else:
        arg0->unk98 = (s32)func_80055E68_56A68(s2->memoryPoolId) + 0x80;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->unk3C);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->unk78);
    }
}

void func_80040D48_41948(func_80040D48_41948_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80040D80_41980(func_80040D80_41980_arg *arg0) {
    if (arg0->unkA == 0) {
        arg0->unk0 = NULL;
        arg0->unk4 = NULL;
        setCallback(&func_80040E00_41A00);
    } else {
        arg0->unk4 = loadTextRenderAsset(1);
        arg0->unk0 = loadAsset_34F7E0();
        arg0->unk10.asS16 = 0x80;
        arg0->unk12 = -8;
        setCallback(&func_80040E4C_41A4C);
    }
    setCleanupCallback(&func_80040F34_41B34);
}

const char pushStartButtonText[] = "PUSH START BUTTON";

void func_80040E00_41A00(func_80040E00_41A00_arg *arg0) {
    if (gFrameCounter & 8) {
        enqueueTextRender(-0x44, arg0->unk8, 0, (u8 *)&pushStartButtonText, (s32)arg0->unkC, (s32)arg0->unkE);
    }
}

void func_80040E4C_41A4C(func_80040D80_41980_arg *arg0) {
    func_80035260_35E60(
        arg0->unk4,
        (void *)&D_800907F8_913F8,
        -0x68,
        arg0->unk8,
        0xFF,
        0xFF,
        0,
        arg0->unkC.asBytes.low,
        arg0->unkE.asBytes.low
    );

    arg0->unk10.asS16 += arg0->unk12;

    if (arg0->unk10.asS16 == 0x20) {
        arg0->unk12 = 8;
    }

    if (arg0->unk10.asS16 == 0x80) {
        arg0->unk12 = -8;
    }

    func_8006D4B8_6E0B8(
        arg0->unk0,
        -0x68,
        arg0->unk8,
        0xD,
        2,
        0,
        arg0->unk10.asBytes.low,
        0xFF,
        0,
        0,
        0xC0,
        arg0->unkC.asBytes.low,
        arg0->unkE.asBytes.low
    );
}

void func_80040F34_41B34(PlayerIndicatorTask *task) {
    task->assetTable = freeNodeMemory(task->assetTable);
    task->indicatorAsset = freeNodeMemory(task->indicatorAsset);
}

extern u8 gConnectedControllerMask;

void func_80040F6C_41B6C(s32 arg0, s16 arg1, u8 arg2, u8 arg3, s16 arg4, s16 arg5) {
    func_80040D80_41980_arg *task;

    if (((s16)arg0 == 0) || !(gConnectedControllerMask & 1)) {
        task = (func_80040D80_41980_arg *)scheduleTask(&func_80040D80_41980, arg2, arg3, 0xC8);
        if (task != NULL) {
            if (gConnectedControllerMask & 1) {
                task->unkA = 0;
            } else {
                task->unkA = 1;
            }
            task->unk8 = arg1;
            task->unkC.asS16 = arg4;
            task->unkE.asS16 = arg5;
        }
    }
}

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    u8 unkC[12];
    s16 unk18;
    s16 unk1A;
} func_80041010_41C10_arg;

void func_8004106C_41C6C(func_80041010_41C10_arg *arg0);

void func_80041010_41C10(func_80041010_41C10_arg *arg0) {
    s16 temp;

    arg0->unk0 = func_80055D7C_5697C(0xB);
    temp = arg0->unk1A;
    arg0->unk4 = allocateNodeMemory((temp * 7) * 8);
    setCleanupCallback(&func_800413E0_41FE0);
    setCallbackWithContinue(&func_8004106C_41C6C);
}

void func_8004106C_41C6C(func_80041010_41C10_arg *arg0) {
    s32 pad[2];
    s32 offset;
    s32 i;
    s32 constant;
    void *extPtr;

    do {
        (void)pad;
        i = 0;
        if (arg0->unk1A > 0) {
            constant = 0x40000;
            extPtr = &D_80090860_91460;
            offset = 0;
            do {
                i++;
                *(s32 *)(offset + (s32)arg0->unk4 + 0x20) = (randA() & 0xFF) << 17;
                *(s32 *)(offset + (s32)arg0->unk4 + 0x24) = (randA() & 0xFF) << 17;
                *(s32 *)(offset + (s32)arg0->unk4 + 0x28) = (randA() & 0xFF) << 17;
                *(s32 *)(offset + (s32)arg0->unk4 + 0x2C) = constant;
                *(s32 *)(offset + (s32)arg0->unk4 + 0x30) = -((randA() & 0xFF) << 8) - constant;
                *(s32 *)(offset + (s32)arg0->unk4 + 0x34) = 0;
                loadAssetMetadata((loadAssetMetadata_arg *)(arg0->unk4 + offset), arg0->unk0, 0);
                *(void **)(offset + (s32)arg0->unk4) = extPtr;
                offset += 0x38;
            } while (i < arg0->unk1A);
        }
    } while (0);
    memcpy(arg0->unkC, (u8 *)arg0->unk8 + 0x134, 0xC);
    setCallbackWithContinue(&func_8004119C_41D9C);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004119C_41D9C);

void func_800413E0_41FE0(PlayerIndicatorTask *task) {
    task->assetTable = freeNodeMemory(task->assetTable);
    task->indicatorAsset = freeNodeMemory(task->indicatorAsset);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041418_42018);

typedef struct {
    u8 _pad[0x8];
    void *freeNext;
    u8 _pad2[0xC];
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
} func_80041518_42118_Node;

void func_80041518_42118(void *arg0) {
    func_80041518_42118_Node *task;

    task = (func_80041518_42118_Node *)scheduleTask(&func_80041010_41C10, 0, 0, 0xF0);

    if (task != NULL) {
        task->unk18 = 0;
        task->freeNext = arg0;
        task->unk1A = 0x64;
        task->unk1C = 0;
    }
}
