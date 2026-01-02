#include "42170.h"
#include "3E160.h"
#include "56910.h"
#include "5AA90.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_3F3EF0);

extern void *D_8009A6B0_9B2B0;
extern void *D_8009A6C0_9B2C0;
extern void *D_8009A6D0_9B2D0;
extern void *D_8009A6E0_9B2E0;
extern void *D_8009A6F0_9B2F0;
extern void *D_8009A700_9B300;
extern void *D_8009A710_9B310;
extern void *D_8009A720_9B320;
extern void *D_8009A760_9B360;
extern void *D_8009A770_9B370;
extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;
extern Transform3D D_8009A8B0_9B4B0;
extern s32 gFrameCounter;

void cleanupStarEffect(void **);

typedef struct {
    u8 _pad0[0x9F0];
    s16 unk9F0[3];
    u8 _pad9F6[0x1C5];
    u8 unkBBB;
    u8 _padBBC[0x13];
    u8 unkBCF;
    u8 _padBD0[0x9];
    u8 unkBD9;
} Func43CA4Unk28;

typedef struct {
    void *assetData;
    loadAssetMetadata_arg sprite;
    u8 padding[0x2];
    Func43CA4Unk28 *unk24;
    Func43CA4Unk28 *player;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s16 animFrameIndex;
    s16 frameTimer;
    s16 displayTimer;
    s16 alphaPulseDir;
    s16 rotationAngle;
    u8 playSoundFlag;
    u8 immediateMode;
} StarEffectState;

typedef struct {
    u8 _pad0[0x1F0];   /* 0x00 */
    s32 pos1F0;        /* 0x1F0 */
    s32 pos1F4;        /* 0x1F4 */
    s32 pos1F8;        /* 0x1F8 */
    u8 _pad1FC[0x98C]; /* 0x1FC */
    s32 unkB88;        /* 0xB88 */
} FallingEffectPlayer;

typedef struct {
    s16 matrix[3][3];            /* 0x00 (0x12 bytes: 9 * 2), offset 0x0E is matrix[2][1] */
    u8 pad12[0x2];               /* 0x12 */
    Vec3i position;              /* 0x14 */
    void *displayData;           /* 0x20 */
    void *asset1;                /* 0x24 */
    void *asset2;                /* 0x28 */
    s32 unk2C;                   /* 0x2C */
    u8 pad30[0xC];               /* 0x30 */
    FallingEffectPlayer *player; /* 0x3C */
    s32 height;                  /* 0x40 */
    s32 playSound;               /* 0x44 */
} FallingEffectState;

typedef struct {
    s32 unk0;
    s32 displayList1;
    s32 displayList2;
    s32 displayList3;
} ShieldEffectDisplayConfig;

typedef struct {
    u8 matrix[0x20]; /* 0x00 - matrix data copied via memcpy */
    ShieldEffectDisplayConfig *displayConfig;
    void *displayAsset1;
    void *displayAsset2;
    s32 unk2C;
    s32 unk30;     /* 0x30 */
    u8 pad34[0x8]; /* 0x34 */
    Player *player;
    union {
        s32 full;
        struct {
            s16 hi;
            s16 lo;
        } half;
    } scale; /* 0x40 */
} ShieldEffectState;

typedef struct {
    DisplayListObject displayList;
    Player *player;
} ShieldEffectRenderState;

struct Func42D54Arg {
    u8 _pad0[0x4];
    loadAssetMetadata_arg unk4;
    void *unk20;
    Func43CA4Unk28 *unk24;
    Func43CA4Unk28 *unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    u8 _pad3E[0x4];
    u8 unk42;
};

void cleanupShieldEffect(ShieldEffectState *);
void renderShieldLayer1(ShieldEffectRenderState *);
void renderShieldLayer2(ShieldEffectRenderState *);
void renderShieldLayer3(ShieldEffectRenderState *);

extern s32 identityMatrix[];

typedef struct {
    u8 _pad0[0x4];
    loadAssetMetadata_arg sprite;
    u8 _pad20[0x4];
    Func43CA4Unk28 *unk24;
    u8 _pad28[0x4];
    s16 unk2C[2];
    s32 scale;
    u8 _pad34[0xE];
    u8 playSoundFlag;
} ExpandStarEffectState;

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    Vec3i unk14;   /* 0x14 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    void *unk3C;   /* 0x3C */
    s32 unk40;     /* 0x40 */
    s16 unk44;     /* 0x44 */
    s16 unk46;     /* 0x46 */
} Func43DC0State;

typedef struct {
    u8 pad0[0x14];            /* 0x00 */
    Vec3i position;           /* 0x14 */
    void *displayData;        /* 0x20 */
    void *asset1;             /* 0x24 */
    void *asset2;             /* 0x28 */
    s32 unk2C;                /* 0x2C */
    u8 pad30[0x0C];           /* 0x30 */
    Player *player;           /* 0x3C */
    s16 rotationMatrix[3][3]; /* 0x40 - 3x3 rotation matrix (0x12 bytes) */
    u8 pad52[2];              /* 0x52 - padding to 0x54 */
    s32 unk54;                /* 0x54 */
    s32 unk58;                /* 0x58 */
    s32 unk5C;                /* 0x5C */
    s32 playSound;            /* 0x60 */
} CrashEffectState;

typedef struct {
    u8 _pad0[0x3C]; /* 0x00 */
    s16 unk3C;      /* 0x3C */
} Func44538TaskMem;

typedef struct {
    u8 pad0[0x14];     /* 0x00 */
    Vec3i position;    /* 0x14 */
    void *displayData; /* 0x20 */
    void *asset1;      /* 0x24 */
    void *asset2;      /* 0x28 */
    s32 unk2C;         /* 0x2C */
    u8 pad30[0xC];     /* 0x30 */
    Player *player;    /* 0x3C */
    s16 unk40;         /* 0x40 */
    s16 scaleFactor;   /* 0x42 */
    u16 rotationAngle; /* 0x44 */
    s16 playSound;     /* 0x46 */
} LiftEffectState;

void cleanupFallingEffect(FallingEffectState *);

typedef struct {
    u8 _pad0[0x30];
    u8 unk30[0x14]; /* 0x30 */
    void *unk44;    /* 0x44 */
    u8 _pad48[0x14];
    u8 unk5C; /* 0x5C */
    u8 _pad5D[0x19];
    u8 unk76; /* 0x76 */
} Func43CA4GameState;

typedef struct {
    u8 _pad0[0xB44];
    u8 unkB44[0xC];   /* 0xB44 */
    u8 _padB50[0x24]; /* 0xB50 */
    s16 unkB74;       /* 0xB74 */
    u8 _padB76[0x58]; /* 0xB76 to 0xBCE */
    u8 unkBCE;        /* 0xBCE */
} Func44BBCPointerTarget;

typedef struct {
    u8 _pad0[0x14];                /* 0x00 */
    Vec3i unk14;                   /* 0x14 */
    void *unk20;                   /* 0x20 */
    void *unk24;                   /* 0x24 */
    void *unk28;                   /* 0x28 */
    s32 unk2C;                     /* 0x2C */
    u8 _pad30[0xC];                /* 0x30 */
    DisplayListObject unk3C;       /* 0x3C */
    DisplayListObject unk78;       /* 0x78 */
    Func44BBCPointerTarget *unkB4; /* 0xB4 */
    s32 unkB8;                     /* 0xB8 */
    s32 unkBC;                     /* 0xBC */
    s32 unkC0;                     /* 0xC0 */
    s16 unkC4;                     /* 0xC4 - counter */
    s16 unkC6;                     /* 0xC6 - value copied to unkB74 */
    u16 unkC8;                     /* 0xC8 */
    u16 unkCA;                     /* 0xCA */
} Func44BBCArg;

typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3i unk8;            /* 0x08 */
    u8 _pad14[0x14];       /* 0x14 */
    Func43CA4Unk28 *unk28; /* 0x28 */
    s16 unk2C[3];          /* 0x2C */
    u8 _pad32[0xE];        /* 0x32 */
    s16 unk40;             /* 0x40 */
    s16 unk42;             /* 0x42 */
} Func43CA4Arg;

typedef struct {
    s32 unk0;              /* 0x00 */
    void *unk4;            /* 0x04 */
    Vec3i unk8;            /* 0x08 */
    u8 _pad14[0x10];       /* 0x14 */
    Func43CA4Unk28 *unk24; /* 0x24 */
    Func43CA4Unk28 *unk28; /* 0x28 */
    s32 unk2C;             /* 0x2C */
    s32 unk30;             /* 0x30 */
    s32 unk34;             /* 0x34 */
    s32 unk38;             /* 0x38 */
    s32 unk3C;             /* 0x3C */
    s16 unk40;             /* 0x40 */
    s16 unk42;             /* 0x42 */
    u8 _pad44[0x2];        /* 0x44 */
    s16 unk46;             /* 0x46 */
    s16 unk48;             /* 0x48 */
    u8 _pad4A[0x2];        /* 0x4A */
    s32 unk4C;             /* 0x4C */
} Func4393CArg;

typedef struct {
    void *unk0;
    loadAssetMetadata_arg_base sprite;
    u8 _pad1C[0x2];
    u8 animFrameIndex;
    u8 _pad1F[0x1];
    void *unk20;
    void *unk24;
    Func43CA4Unk28 *player;
    s16 orbitOffset[3];
    u8 _pad32[0xA];
    s16 displayTimer;
    u8 _pad3E[0x2];
    u16 rotationAngle;
    u8 playSoundFlag;
} OrbitStarEffectState;

void func_80044578_45178(Func44BBCArg *);
void func_80044684_45284(Func44BBCArg *);
void func_800447D4_453D4(Func44BBCArg *);
void func_80044888_45488(Func44BBCArg *);
void func_80044990_45590(Func44BBCArg *);
void func_80044AB8_456B8(Func44BBCArg *);
void func_80044C38_45838(Func44BBCArg *);
void func_80044018_44C18(Func43DC0State *);
void func_80043E24_44A24(Func43DC0State *);
void func_80043F8C_44B8C(Func43DC0State *);
void contractStarEffect(StarEffectState *);
void func_80043CA4_448A4(Func43CA4Arg *);
void func_800439F4_445F4(Func4393CArg *);
void updateStarEffect(StarEffectState *);
void expandStarEffect(ExpandStarEffectState *);
void orbitStarEffect(OrbitStarEffectState *);
void updateFallingEffect(FallingEffectState *);
void animateFallingEffectDescent(FallingEffectState *);
void cleanupSparkleEffect(SparkleEffectState *);
void updateSparkleEffect(SparkleEffectState *);
void fadeOutSparkleEffect(SparkleEffectState *);

extern s8 starAnimFrameDurations[];
extern s8 starAnimFrameIndices[];
extern s32 D_80090964_91564;
extern s32 D_80090974_91574;
extern s32 D_8009093C_9153C;
extern s8 D_80090958_91558;
extern s8 D_8009095C_9155C;
extern s32 D_80090AA0_916A0;
extern s32 D_80090AAC_916AC;
extern s8 D_80090950_91550;

void initFallingEffect(FallingEffectState *arg0) {
    getCurrentAllocation();
    createXRotationMatrix(arg0->matrix, 0x800);
    arg0->displayData = &D_8009A6B0_9B2B0;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->height = 0x2000;
    arg0->unk2C = 0;
    arg0->playSound = 1;
    setCleanupCallback(cleanupFallingEffect);
    setCallbackWithContinue(updateFallingEffect);
}

void updateFallingEffect(FallingEffectState *arg0) {
    s32 i;

    arg0->position.x = arg0->player->pos1F0;
    arg0->position.y = arg0->player->pos1F4;
    arg0->position.z = arg0->player->pos1F8;

    if ((arg0->player->unkB88 & 0x20) == 0) {
        setCallback(animateFallingEffectDescent);
    }

    if (arg0->playSound != 0) {
        arg0->playSound = 0;
        func_80056B7C_5777C(&arg0->position, 0xE);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void animateFallingEffectDescent(FallingEffectState *arg0) {
    Func43CA4GameState *allocation;
    s32 i;

    allocation = (Func43CA4GameState *)getCurrentAllocation();
    if (allocation->unk76 == 0) {
        arg0->height -= 0x200;
    }

    if (arg0->height == 0x200) {
        func_80069CF8_6A8F8();
    }

    arg0->matrix[2][1] = -arg0->height;
    arg0->position.y += 0xFFFF0000;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupFallingEffect(FallingEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

typedef struct {
    u8 _pad0[0x3C];
    void *player;
} FallingEffectTask;

void *createFallingEffect(void *arg0) {
    FallingEffectTask *task;

    task = (FallingEffectTask *)scheduleTask(&initFallingEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = arg0;
    }
    return task;
}

void updateShieldEffect(ShieldEffectState *);
void spawnShieldBurstEffect(Vec3i *);

void initShieldEffect(ShieldEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayConfig = (ShieldEffectDisplayConfig *)&D_8009A6C0_9B2C0;
    arg0->displayAsset1 = loadAsset_B7E70();
    arg0->displayAsset2 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->scale.full = 0x400;
    func_80056B7C_5777C(&arg0->player->worldPos, 0x13);
    setCleanupCallback(cleanupShieldEffect);
    setCallbackWithContinue(updateShieldEffect);
}

void updateShieldEffect(ShieldEffectState *arg0) {
    Vec3i effectPos;
    s32 scale;
    s16 scaleFactor;
    s32 i;

    if (!(arg0->player->unkB88 & 0x40)) {
        effectPos.x = arg0->player->worldPos.x;
        effectPos.y = arg0->player->worldPos.y + 0x100000;
        effectPos.z = arg0->player->worldPos.z;
        spawnShieldBurstEffect(&effectPos);
        func_80069CF8_6A8F8();
        return;
    }

    scale = arg0->scale.full;
    if (scale != 0x2000) {
        arg0->scale.full = scale + 0x400;
        memcpy(arg0, identityMatrix, 0x20);
        scaleFactor = arg0->scale.half.lo;
        scaleMatrix((Transform3D *)arg0, scaleFactor, scaleFactor, scaleFactor);
    }

    effectPos.x = arg0->player->worldPos.x;
    effectPos.y = arg0->player->worldPos.y;
    effectPos.z = arg0->player->worldPos.z;
    effectPos.y = effectPos.y + 0xFFFE0000;

    if (func_8005B9E4_5C5E4(&effectPos, 0x180000, 0x320000, arg0->player->unkBB8) != 0) {
        arg0->player->unkBCE |= 1;
    }
    arg0->unk30 = 0;

    for (i = 0; i < 4; i++) {
        if (arg0->displayConfig->displayList1 != 0) {
            debugEnqueueCallback((u16)i, 1, renderShieldLayer1, arg0);
        }
        if (arg0->displayConfig->displayList2 != 0) {
            debugEnqueueCallback((u16)i, 3, renderShieldLayer2, arg0);
        }
        if (arg0->displayConfig->displayList3 != 0) {
            debugEnqueueCallback((u16)i, 5, renderShieldLayer3, arg0);
        }
    }
}

void cleanupShieldEffect(ShieldEffectState *arg0) {
    arg0->displayAsset1 = freeNodeMemory(arg0->displayAsset1);
    arg0->displayAsset2 = freeNodeMemory(arg0->displayAsset2);
}

void scheduleShieldEffect(void *arg0) {
    ShieldEffectState *task;

    task = scheduleTask(&initShieldEffect, 0, 0, 0x63);
    if (task != NULL) {
        task->player = arg0;
    }
}

void renderShieldLayer1(ShieldEffectRenderState *arg0) {
    arg0->displayList.transform.translation.x = arg0->player->worldPos.x;
    arg0->displayList.transform.translation.y = arg0->player->worldPos.y;
    arg0->displayList.transform.translation.z = arg0->player->worldPos.z;
    func_800634E8_640E8(&arg0->displayList);
}

void renderShieldLayer2(ShieldEffectRenderState *arg0) {
    arg0->displayList.transform.translation.x = arg0->player->worldPos.x;
    arg0->displayList.transform.translation.y = arg0->player->worldPos.y;
    arg0->displayList.transform.translation.z = arg0->player->worldPos.z;
    func_80063534_64134(&arg0->displayList);
}

void renderShieldLayer3(ShieldEffectRenderState *arg0) {
    arg0->displayList.transform.translation.x = arg0->player->worldPos.x;
    arg0->displayList.transform.translation.y = arg0->player->worldPos.y;
    arg0->displayList.transform.translation.z = arg0->player->worldPos.z;
    func_80063580_64180(&arg0->displayList);
}

typedef struct {
    u8 pad0[0xD8];
    void *assetTable;
} BurstEffectState;

void setupBurstParticles(void);
void cleanupBurstEffect(BurstEffectState *);

void initBurstEffect(BurstEffectState *state) {
    state->assetTable = load_3ECE40();
    setCleanupCallback(cleanupBurstEffect);
    setCallbackWithContinue(setupBurstParticles);
}

INCLUDE_ASM("asm/nonmatchings/42170", setupBurstParticles);

INCLUDE_ASM("asm/nonmatchings/42170", updateBurstParticles);

void cleanupBurstEffect(BurstEffectState *state) {
    state->assetTable = freeNodeMemory(state->assetTable);
}

void spawnShieldBurstEffect(Vec3i *position) {
    u8 *task;
    s32 i;
    u8 *ptr;

    task = (u8 *)scheduleTask(initBurstEffect, 2, 0, 0xE7);
    if (task != NULL) {
        i = 0;
        ptr = task;
        do {
            memcpy(ptr + 4, position, 0xC);
            i++;
            ptr += 0x24;
        } while (i < 6);
        task[0xDC] = 0x2B;
        func_80056B7C_5777C(position, 0x16);
    }
}

void spawnBurstEffect(Vec3i *position) {
    u8 *task;
    s32 i;
    u8 *ptr;

    task = (u8 *)scheduleTask(initBurstEffect, 2, 0, 0xE7);
    if (task != NULL) {
        i = 0;
        ptr = task;
        do {
            memcpy(ptr + 4, position, 0xC);
            i++;
            ptr += 0x24;
        } while (i < 6);
        task[0xDC] = 0x2F;
        func_80056B7C_5777C(position, 0xE);
    }
}

void updateCrashEffect(CrashEffectState *);
void cleanupCrashEffect(CrashEffectState *);

void initCrashEffect(CrashEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayData = &D_8009A6D0_9B2D0;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->unk2C = 0;
    createYRotationMatrix((Transform3D *)arg0->rotationMatrix, 0xF800);
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk5C = 0;
    arg0->playSound = 1;
    setCleanupCallback(cleanupCrashEffect);
    setCallbackWithContinue(updateCrashEffect);
}

void updateCrashEffect(CrashEffectState *arg0) {
    Vec3i pos;
    s32 i;

    func_8006B084_6BC84(arg0->rotationMatrix, &arg0->player->unk3F8, arg0);

    if ((arg0->player->unkB88 & 0x80) == 0) {
        pos.x = arg0->player->worldPos.x;
        pos.y = arg0->player->worldPos.y + 0x100000;
        pos.z = arg0->player->worldPos.z;
        spawnBurstEffect(&pos);
        func_80069CF8_6A8F8();
    }

    if (arg0->playSound != 0) {
        arg0->playSound = 0;
        func_80056B7C_5777C(&arg0->position, 0x12);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupCrashEffect(CrashEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

void *spawnCrashEffect(void *arg0) {
    CrashEffectState *task;

    task = scheduleTask(&initCrashEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = arg0;
    }
    return task;
}

void initSparkleEffect(SparkleEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayData = &D_8009A6E0_9B2E0;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->scale = 0;
    arg0->rotation = 0;
    arg0->opacity = 0xFF;
    func_80056B7C_5777C(&arg0->position, 0x14);
    setCleanupCallback(cleanupSparkleEffect);
    setCallbackWithContinue(updateSparkleEffect);
}

void updateSparkleEffect(SparkleEffectState *arg0) {
    Func43CA4GameState *gameState;
    s32 temp;
    s32 diff;
    s32 i;
    s32 pad[5];

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        temp = arg0->scale;
        diff = 0x4000 - temp;
        if (diff < 0) {
            diff += 3;
        }
        arg0->scale = temp + (diff >> 2);
    }

    if (arg0->scale == 0) {
        arg0->scale = arg0->scale + 0x1000;
    }

    arg0->rotation += 0x300;
    createYRotationMatrix((Transform3D *)arg0, arg0->rotation);

    temp = arg0->scale;
    scaleMatrix((Transform3D *)arg0, temp, temp, temp);

    func_8005C250_5CE50((Vec3i *)&arg0->position, arg0->playerIndex, arg0->scale * 0xF0);

    arg0->alpha = (u8)arg0->opacity;

    i = 0;
    do {
        func_800639F8_645F8(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);

    if (arg0->scale >= 0x3C00) {
        setCallback(fadeOutSparkleEffect);
    }
}

void fadeOutSparkleEffect(SparkleEffectState *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    s32 pad[3];

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        arg0->opacity -= 0x18;
    }

    if (arg0->opacity < 0x18) {
        func_80069CF8_6A8F8();
        return;
    }

    arg0->rotation += 0x300;
    createYRotationMatrix((Transform3D *)arg0, arg0->rotation);
    scaleMatrix((Transform3D *)arg0, arg0->scale, arg0->scale, arg0->scale);
    arg0->alpha = (u8)arg0->opacity;

    for (i = 0; i < 4; i++) {
        func_800639F8_645F8(i, (DisplayListObject *)arg0);
    }
}

void cleanupSparkleEffect(SparkleEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

SparkleEffectState *spawnSparkleEffect(void *arg0) {
    SparkleEffectState *task;

    task = (SparkleEffectState *)scheduleTask(initSparkleEffect, 0, 0, 0xC8);
    if (task != NULL) {
        memcpy(&task->position, arg0, 0xC);
        task->playerIndex = -1;
    }
    return task;
}

SparkleEffectState *spawnSparkleEffectWithPlayer(void *arg0, s32 arg1) {
    SparkleEffectState *task;

    task = (SparkleEffectState *)scheduleTask(&initSparkleEffect, 0, 0, 0xC8);
    if (task != NULL) {
        memcpy(&task->position, arg0, 0xC);
        task->playerIndex = arg1;
    }
    return task;
}

void cleanupLiftEffect(LiftEffectState *);
void updateLiftEffect(LiftEffectState *);
void fadeOutLiftEffect(LiftEffectState *);

void initLiftEffect(LiftEffectState *state) {
    getCurrentAllocation();
    state->displayData = &D_8009A6F0_9B2F0;
    state->asset1 = loadAsset_B7E70();
    state->asset2 = loadAsset_216290();
    *(s32 *)&state->unk40 = 0x200;
    state->unk2C = 0;
    state->playSound = 1;
    setCleanupCallback(cleanupLiftEffect);
    setCallbackWithContinue(updateLiftEffect);
}

void updateLiftEffect(LiftEffectState *state) {
    Func43CA4GameState *gameState;
    Player *player;
    Vec3i *pos;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (*(s32 *)&state->unk40 != 0x2000) {
            *(s32 *)&state->unk40 += 0x200;
        }
    }

    state->rotationAngle += 0x300;
    createYRotationMatrix((Transform3D *)state, state->rotationAngle);
    scaleMatrix((Transform3D *)state, state->scaleFactor, state->scaleFactor, state->scaleFactor);
    player = state->player;
    pos = &state->position;
    memcpy(pos, &player->worldPos, 0xC);
    state->position.y += 0xFFEC0000;

    if (state->playSound != 0) {
        func_80056B7C_5777C(pos, 0x15);
        state->playSound = 0;
    }

    if ((state->player->unkB88 & 0x400) == 0) {
        setCallback(fadeOutLiftEffect);
    }

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        i++;
    } while (i < 4);
}

void fadeOutLiftEffect(LiftEffectState *state) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        *(s32 *)&state->unk40 -= 0x200;
    }

    if (*(s32 *)&state->unk40 == 0x200) {
        func_80069CF8_6A8F8();
    }

    state->rotationAngle += 0x12C;
    createYRotationMatrix((Transform3D *)state, state->rotationAngle);
    scaleMatrix((Transform3D *)state, state->scaleFactor, state->scaleFactor, state->scaleFactor);
    memcpy(&state->position, &state->player->worldPos, 0xC);
    state->position.y += 0xFFEC0000;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void cleanupLiftEffect(LiftEffectState *state) {
    state->asset1 = freeNodeMemory(state->asset1);
    state->asset2 = freeNodeMemory(state->asset2);
}

void *createLiftEffect(Player *player) {
    LiftEffectState *task;

    task = (LiftEffectState *)scheduleTask(initLiftEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = player;
    }
    return task;
}

typedef struct {
    u8 pad0[0x434];        /* 0x00 */
    s32 sourcePosition[3]; /* 0x434 - warp source position (12 bytes) */
} WarpEffectSource;

typedef struct {
    s16 matrix[3][3];         /* 0x00 (0x12 bytes: 9 * s16) */
    u8 pad12[0x2];            /* 0x12 */
    Vec3i position;           /* 0x14 */
    void *displayData;        /* 0x20 */
    void *asset1;             /* 0x24 */
    void *asset2;             /* 0x28 */
    s32 unk2C;                /* 0x2C */
    u8 pad30[0xC];            /* 0x30 */
    WarpEffectSource *source; /* 0x3C */
    Player *player;           /* 0x40 */
    s32 delayFrames;          /* 0x44 */
    s32 scale;                /* 0x48 */
    s32 height;               /* 0x4C */
    s32 velocity;             /* 0x50 */
} WarpEffectState;

typedef struct {
    u8 pad0[0x24];
    void *asset1;
    void *asset2;
} WarpEffectCleanupArg;

void cleanupWarpEffect(WarpEffectCleanupArg *);
void updateWarpEffect(WarpEffectState *);

void initWarpEffect(WarpEffectState *state) {
    if (state->delayFrames == 0) {
        getCurrentAllocation();
        state->displayData = &D_8009A700_9B300;
        state->asset1 = loadAsset_B7E70();
        state->asset2 = loadAsset_216290();
        state->scale = 0x400;
        state->unk2C = 0;
        state->height = 0x100000;
        state->velocity = 0;
        setCleanupCallback(cleanupWarpEffect);
        setCallbackWithContinue(updateWarpEffect);
    } else {
        state->delayFrames = state->delayFrames - 1;
    }
}

void finishWarpEffect(WarpEffectState *);
void descendWarpEffect(WarpEffectState *);

void updateWarpEffect(WarpEffectState *state) {
    GameState *gameState;
    s16 scale;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        if (state->scale != 0x2000) {
            state->scale += 0x400;
        }

        state->velocity += 0x20000;
        state->height += state->velocity;

        if (state->height > 0x500000) {
            setCallback(descendWarpEffect);
        }
    }

    createXRotationMatrix(state->matrix, 0);
    scale = (s16)state->scale;
    scaleMatrix((Transform3D *)state, scale, scale, scale);
    memcpy(&state->position, state->source->sourcePosition, 0xC);
    state->position.y += state->height;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void descendWarpEffect(WarpEffectState *state) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        state->player->unkBDE |= 4;
        state->velocity += 0xFFFE0000;
        state->height += state->velocity;

        if (state->height == 0x220000) {
            func_80058B30_59730(state->player);
            func_80056B7C_5777C(&state->position, 0x19);
        }

        if (state->height <= 0x10000) {
            state->velocity = 0x50000;
            setCallback(finishWarpEffect);
        }
    }

    memcpy(&state->position, &state->player->worldPos, 0xC);
    state->position.y += state->height;

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        i++;
    } while (i < 4);
}

void finishWarpEffect(WarpEffectState *state) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        state->velocity = state->velocity + (-0x8000);
        state->height = state->height + state->velocity;

        if (state->height < (s32)0xFFF00000 && state->velocity < 0) {
            func_80069CF8_6A8F8();
        }

        state->position.x = state->position.x + 0x20000;
        state->position.y = state->position.y + state->velocity;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void cleanupWarpEffect(WarpEffectCleanupArg *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

WarpEffectState *createWarpEffect(WarpEffectSource *source, Player *player, s16 delayFrames) {
    WarpEffectState *task;

    task = (WarpEffectState *)scheduleTask(initWarpEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->source = source;
        task->player = player;
        task->delayFrames = delayFrames;
    }
    return task;
}

void updateStarEffectAnimation(StarEffectState *arg0) {
    unsigned int new_var;
    arg0->frameTimer--;
    if ((arg0->frameTimer << 0x10) == 0) {
        new_var = 2;
        loadAssetMetadata(&arg0->sprite, arg0->assetData, starAnimFrameIndices[arg0->animFrameIndex * new_var]);
        arg0->frameTimer = starAnimFrameDurations[arg0->animFrameIndex * 2];
        arg0->animFrameIndex++;
        if (starAnimFrameDurations[arg0->animFrameIndex * new_var] == 0) {
            arg0->animFrameIndex = 0;
        }
    }
    if (arg0->alphaPulseDir != 0) {
        arg0->sprite.unk1A -= 0x10;
        if (arg0->sprite.unk1A == 0x40) {
            arg0->alphaPulseDir++;
        }
    } else {
        arg0->sprite.unk1A += 0x10;
        if (arg0->sprite.unk1A == 0xE0) {
            arg0->alphaPulseDir--;
        }
    }
}

void initStarEffect(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(cleanupStarEffect);
    setCallbackWithContinue(updateStarEffect);
}

void updateStarEffect(StarEffectState *arg0) {
    Func43CA4GameState *gameState;
    s16 startDelay;
    void *spriteBuffer;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    startDelay = arg0->frameTimer;

    if (startDelay == 0) {
        arg0->frameTimer = 1;
        arg0->alphaPulseDir = 0;
        arg0->sprite.unk1A = 0;
        spriteBuffer = gameState->unk44;
        arg0->unk30 = 0x200000;
        arg0->unk2C = 0;
        arg0->unk34 = 0;
        arg0->animFrameIndex = 0;
        arg0->playSoundFlag = 1;
        arg0->sprite.unk0 = (void *)((u8 *)spriteBuffer + 0xF00);
        updateStarEffectAnimation(arg0);

        if (arg0->immediateMode != 0) {
            arg0->player->unkBCF++;

            if (arg0->player->unkBBB == 0xC) {
                arg0->displayTimer = 0x1E;
            } else {
                arg0->displayTimer = 0x12C;
            }

            arg0->unk2C = 0x140000;
            arg0->unk30 = 0x190000;
            setCallbackWithContinue(orbitStarEffect);
        } else {
            setCallbackWithContinue(expandStarEffect);
        }
    } else {
        if (gameState->unk76 == 0) {
            arg0->frameTimer = startDelay - 1;
        }
    }
}

void expandStarEffect(ExpandStarEffectState *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        arg0->scale += 0x10000;
        if (arg0->scale > 0x2FFFFF) {
            setCallback(contractStarEffect);
        }

        transformVector(arg0->unk2C, arg0->unk24->unk9F0, &arg0->sprite.position);

        if (arg0->playSoundFlag != 0) {
            arg0->playSoundFlag = 0;
            func_80056B7C_5777C(&arg0->sprite.position, 0x1A);
        }

        updateStarEffectAnimation((StarEffectState *)arg0);
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &arg0->sprite);
    }
}

void contractStarEffect(StarEffectState *state) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        updateStarEffectAnimation(state);
        transformVector((s16 *)&state->unk2C, state->unk24->unk9F0, &state->sprite.position);

        if (state->sprite.unk1A == 0x40) {
            if (state->player->unkBD9 != 0) {
                state->unk2C = 0x300000;
                state->unk30 = 0x300000;
            } else {
                state->unk2C = 0x140000;
                state->unk30 = 0x190000;
            }

            if (state->player->unkBBB == 0xC) {
                state->displayTimer = 0x1E;
            } else {
                state->displayTimer = 0x12C;
            }

            state->player->unkBCF++;
            state->playSoundFlag = 1;
            setCallback(orbitStarEffect);
        }
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &state->sprite);
    }
}

void orbitStarEffect(OrbitStarEffectState *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    s32 pad;
    Vec3i rotated;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        updateStarEffectAnimation((StarEffectState *)arg0);
        arg0->rotationAngle += 0x100;
        rotateVectorY(arg0->orbitOffset, arg0->rotationAngle, &rotated);
        transformVector((s16 *)&rotated, arg0->player->unk9F0, &arg0->sprite.position);
        if (arg0->playSoundFlag != 0) {
            arg0->playSoundFlag = 0;
            func_80056B7C_5777C(&arg0->sprite.position, 0x1A);
        }
        if (arg0->displayTimer != 0) {
            arg0->displayTimer--;
        } else if (arg0->animFrameIndex == 0x40) {
            arg0->player->unkBCF--;
            func_80069CF8_6A8F8();
        }
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, (loadAssetMetadata_arg *)&arg0->sprite);
    }
}

void cleanupStarEffect(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnStarEffect(void *arg0, void *arg1, s16 arg2) {
    StarEffectTask *task;

    task = (StarEffectTask *)scheduleTask(initStarEffect, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->player = arg1;
        task->frameTimer = arg2;
        task->immediateMode = 0;
    }
}

StarEffectTask *spawnStarEffectImmediate(void *arg0) {
    StarEffectTask *task;

    task = (StarEffectTask *)scheduleTask(initStarEffect, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->player = arg0;
        task->frameTimer = 0;
        task->immediateMode = 1;
    }
    return task;
}

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} Func432D8Arg;

void func_800432D8_43ED8(Func432D8Arg *);
void func_80044CA4_458A4(Func432D8Arg *);

typedef struct {
    u8 pad0[0x18];           /* 0x00 */
    s32 unk18;               /* 0x18 */
    u8 pad1C[0x4];           /* 0x1C */
    void *unk20;             /* 0x20 */
    void *unk24;             /* 0x24 */
    void *unk28;             /* 0x28 */
    s32 unk2C;               /* 0x2C */
    u8 pad30[0xC];           /* 0x30 */
    DisplayListObject unk3C; /* 0x3C */
    Player *unk78;           /* 0x78 */
    s32 unk7C;               /* 0x7C */
    u16 unk80;               /* 0x80 */
    u16 unk82;               /* 0x82 */
    s16 unk84;               /* 0x84 */
} Func43018State;

void func_8004309C_43C9C(Func43018State *arg0);
void func_8004320C_43E0C(Func43018State *arg0);

void func_80043018_43C18(Func43018State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A710_9B310;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk3C.unk20 = (DisplayLists *)&D_8009A720_9B320;
    arg0->unk2C = 0;
    arg0->unk3C.unk2C = 0;
    arg0->unk84 = 0x200;
    arg0->unk3C.unk24 = arg0->unk24;
    arg0->unk3C.unk28 = arg0->unk28;
    setCleanupCallback(func_800432D8_43ED8);
    setCallbackWithContinue(func_8004309C_43C9C);
}

void func_8004309C_43C9C(Func43018State *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    Transform3D matrix;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, &arg0->unk78->unk3F8, arg0);
    scaleMatrix((Transform3D *)arg0, arg0->unk84, arg0->unk84, arg0->unk84);

    arg0->unk82 += 0x300;
    createZRotationMatrix(&matrix, arg0->unk82);

    matrix.translation.x = 0;
    matrix.translation.y = 0xBB333;
    matrix.translation.z = 0xFFEA0000;

    func_8006B084_6BC84(&matrix, arg0, &arg0->unk3C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, (&arg0->unk3C));
    }

    if (arg0->unk78->unkB84 & 0x80000) {
        arg0->unk78->unkB9A = 0;
    }

    if (arg0->unk78->unkB9A != 0) {
        if (gameState->unk76 == 0) {
            arg0->unk78->unkB9A--;
            if (arg0->unk78->unkB9A == 0) {
                if (arg0->unk78->unkBBB == 0x10) {
                    arg0->unk78->unkB9A++;
                }
            }
            if (arg0->unk84 != 0x2000) {
                arg0->unk84 += 0x200;
            }
        }
    } else {
        arg0->unk78->unkBD0 = 0;
        arg0->unk7C = 0x40000;
        setCallback(func_8004320C_43E0C);
    }
}

void func_8004320C_43E0C(Func43018State *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    Transform3D matrix;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        arg0->unk7C -= 0x8000;
        if (arg0->unk7C <= (s32)0xFFF80000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 += arg0->unk7C;
        createZRotationMatrix(&matrix, arg0->unk82);
        matrix.translation.x = 0;
        matrix.translation.y = 0xBB333;
        matrix.translation.z = 0xFFEA0000;
        func_8006B084_6BC84(&matrix, arg0, &arg0->unk3C);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, &arg0->unk3C);
    }
}

void func_800432D8_43ED8(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void *func_80043310_43F10(Player *arg0) {
    Func43018State *task;

    task = (Func43018State *)scheduleTask(func_80043018_43C18, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        if (arg0->unkB84 & 2) {
            task->unk80 = 0x1000;
        }
    }
    return task;
}

extern void *D_8009A730_9B330;
extern void *D_8009A740_9B340;
extern void *D_8009A750_9B350;
void func_8004367C_4427C(Func432D8Arg *);
void func_800433EC_43FEC(Func43374State *);
void func_800435F0_441F0(Func43374State *);

void func_80043374_43F74(Func43374State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A730_9B330;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk82 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(func_8004367C_4427C);
    setCallbackWithContinue(func_800433EC_43FEC);
}

void func_800433EC_43FEC(Func43374State *arg0) {
    s32 pad[8];
    GameState *allocation;
    s32 *ptr;
    s32 i;
    Player *player;
    Player *player2;
    s16 scale;

    allocation = (GameState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, &arg0->unk78->unk3F8, arg0);
    scale = arg0->unk82;
    scaleMatrix((Transform3D *)arg0, scale, scale, scale);

    ptr = &D_8009A8A4_9B4A4;
    *ptr = 0;
    D_8009A8A8_9B4A8 = 0x9CCCC;
    D_8009A8AC_9B4AC = 0xFFE44CCD;

    func_8006B084_6BC84(ptr - 5, arg0, arg0->unk3C);

    if (gFrameCounter & 1) {
        arg0->unk5C = &D_8009A740_9B340;
    } else {
        arg0->unk5C = &D_8009A750_9B350;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        if (arg0->unk82 == 0x2000) {
            enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0->unk3C);
        }
    }

    player = arg0->unk78;
    if (player->unkBC9 == 3) {
        if (player->unkB9A != 0) {
            if (player->unkB9A < 0x3C) {
                player->unkB9A = 0x3C;
            }
        }
        player = arg0->unk78;
    }

    if (player->unkB84 & 0x80000) {
        player->unkB9A = 0;
    }

    player2 = arg0->unk78;
    if (player2->unkB9A != 0) {
        if (allocation->gamePaused == 0) {
            if (arg0->unk82 == 0x2000) {
                player2->unkB9A--;
                player = arg0->unk78;
                if (player->unkB9A == 0 && player->unkBBB == 0x11) {
                    player->unkB9A++;
                }
            } else {
                arg0->unk82 = arg0->unk82 + 0x200;
                if (arg0->unk82 == 0x2000) {
                    arg0->unk78->unkBD0 += 2;
                }
            }
        }
    } else {
        player2->unkBD0 = 0;
        arg0->unk7C = 0x40000;
        setCallback(func_800435F0_441F0);
    }
}

void func_800435F0_441F0(Func43374State *arg0) {
    GameState *gameState;
    s32 i;
    s32 pad[7];

    gameState = (GameState *)getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        arg0->unk7C -= 0x8000;
        if (arg0->unk7C <= (s32)0xFFF80000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 += arg0->unk7C;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_8004367C_4427C(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

Func43374State *func_800436B4_442B4(Player *arg0) {
    Func43374State *task;

    task = (Func43374State *)scheduleTask(func_80043374_43F74, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        if (arg0->unkB84 & 2) {
            task->unk80 = 0x1000;
        }
    }
    return task;
}

s32 func_80043718_44318(void *arg0_void, void *arg1_void) {
    Func4393CArg *arg0 = (Func4393CArg *)arg0_void;
    u8 *arg1 = (u8 *)arg1_void;
    s8 check;

    arg0->unk42--;

    if (arg0->unk42 != 0) {
        return 0;
    }

    loadAssetMetadata((void *)((s32)arg0 + 4), (void *)arg0->unk0, (s8)arg1[arg0->unk40 * 2 + 1]);

    arg0->unk42 = (s8)arg1[arg0->unk40 * 2];
    arg0->unk40++;

    check = arg1[arg0->unk40 * 2];
    if (check != 0) {
        return 0;
    }

    arg0->unk40--;
    return 1;
}

void func_800437C4_443C4(Func4393CArg *arg0, s8 *arg1) {
    s16 temp;

    temp = arg0->unk42 - 1;
    arg0->unk42 = temp;

    if ((temp << 16) == 0) {
        loadAssetMetadata((loadAssetMetadata_arg *)((s32)arg0 + 4), (void *)arg0->unk0, arg1[arg0->unk40 * 2 + 1]);
        arg0->unk42 = arg1[arg0->unk40 * 2];
        temp = (u16)arg0->unk40 + 1;
        arg0->unk40 = temp;
        if (arg1[temp * 2] == 0) {
            arg0->unk40 = 0;
        }
    }
}

void func_80043D30_44930(void **);
void func_8004393C_4453C(Func4393CArg *);
void func_800438A0_444A0(Func4393CArg *);
void func_80043AB4_446B4(Func4393CArg *);

void func_80043860_44460(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(func_80043D30_44930);
    setCallbackWithContinue(func_800438A0_444A0);
}

void func_800438A0_444A0(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s16 temp;
    void *ptr;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    temp = arg0->unk42;

    if (temp == 0) {
        arg0->unk42 = 1;
        arg0->unk46 = 0;
        ptr = (void *)((u8 *)gameState->unk44 + 0xF40);
        arg0->unk30 = 0x300000;
        arg0->unk2C = 0;
        arg0->unk34 = 0;
        arg0->unk40 = 0;
        arg0->unk4C = 1;
        arg0->unk4 = ptr;
        func_80043718_44318(arg0, &D_8009093C_9153C);
        setCallbackWithContinue(func_8004393C_4453C);
    } else {
        if (gameState->unk76 == 0) {
            arg0->unk42 = temp - 1;
        }
    }
}

void func_8004393C_4453C(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_8009093C_9153C) != 0) {
            arg0->unk38 = 0;
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            setCallback(func_800439F4_445F4);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->unk24->unk9F0, &arg0->unk8);

    if (arg0->unk4C != 0) {
        arg0->unk4C = 0;
        func_80056B7C_5777C(&arg0->unk8, 0x12);
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_800439F4_445F4(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        func_800437C4_443C4(arg0, &D_80090950_91550);
        arg0->unk38 = arg0->unk38 + 0x8000;
        arg0->unk30 = arg0->unk30 + arg0->unk38;
        if (arg0->unk30 > 0x600000) {
            arg0->unk48 = 0;
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            setCallback(func_80043AB4_446B4);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->unk24->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_80043C00_44800(Func43CA4Arg *);

void func_80043AB4_446B4(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s32 sinVal;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 != 0) {
        goto transform_and_loop;
    }

    arg0->unk48 = arg0->unk48 + 0x80;
    sinVal = approximateSin(arg0->unk48);
    arg0->unk30 = 0x700000 - (sinVal * 3 << 8);

    if (arg0->unk48 < 0x800) {
        func_800437C4_443C4(arg0, &D_80090958_91558);
    } else if (arg0->unk48 == 0x800) {
        arg0->unk40 = 0;
        arg0->unk42 = 1;
        arg0->unk3C = func_80059AC4_5A6C4((Player *)arg0->unk28);
        func_80059A48_5A648((Player *)arg0->unk28, -arg0->unk3C);
        func_800437C4_443C4(arg0, &D_8009095C_9155C);
    } else {
        func_800437C4_443C4(arg0, &D_8009095C_9155C);
    }

    if (arg0->unk48 == 0xC00) {
        arg0->unk40 = 0;
        arg0->unk42 = 1;
        func_80059A48_5A648((Player *)arg0->unk24, arg0->unk3C);
        setCallback(func_80043C00_44800);
    }

transform_and_loop:
    transformVector((s16 *)&arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    if (arg0->unk48 == 0x80) {
        func_80056B7C_5777C(&arg0->unk8, 0x1C);
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_80043C00_44800(Func43CA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_80090964_91564) != 0) {
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            func_80056B7C_5777C(&arg0->unk8, 0x12);
            setCallback(func_80043CA4_448A4);
        }
    }

    transformVector(arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_80043CA4_448A4(Func43CA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_80090974_91574) != 0) {
            func_80069CF8_6A8F8();
        }
    }

    transformVector(arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_80043D30_44930(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_80043D5C_4495C(void *arg0, void *arg1, s16 arg2) {
    Func4393CArg *task;

    task = (Func4393CArg *)scheduleTask(func_80043860_44460, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->unk28 = arg1;
        task->unk42 = arg2;
    }
}

void func_80043DC0_449C0(Func43DC0State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A760_9B360;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk46 = 0x200;
    setCleanupCallback(func_80044018_44C18);
    setCallbackWithContinue(func_80043E24_44A24);
}

void func_80043E24_44A24(Func43DC0State *arg0) {
    Func43CA4GameState *allocation;
    Player *player;
    Player *temp_player;
    u16 count;
    u16 new_count;
    s32 i;

    allocation = (Func43CA4GameState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk44);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, (u8 *)arg0->unk3C + 0x3F8, arg0);

    if (arg0->unk46 == 0x200) {
        func_80056B7C_5777C(&arg0->unk14, 0x1D);
    }

    if (arg0->unk46 != 0x2000) {
        arg0->unk46 += 0x200;
    }

    scaleMatrix((Transform3D *)arg0, arg0->unk46, arg0->unk46, arg0->unk46);

    if (gFrameCounter & 4) {
        arg0->unk20 = &D_8009A760_9B360;
    } else {
        arg0->unk20 = &D_8009A770_9B370;
    }

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);

    player = arg0->unk3C;
    if (player->unkB84 & 0x80000) {
        player->unkBA6 = 0;
    }

    temp_player = arg0->unk3C;
    count = temp_player->unkBA6;
    if (count != 0) {
        if (allocation->unk76 == 0) {
            temp_player->unkBA6 = count - 1;
            player = arg0->unk3C;
            new_count = player->unkBA6;
            if (new_count == 0) {
                if (player->unkBBB == 0x11) {
                    player->unkBA6 = new_count + 1;
                }
            }
        }
    } else {
        temp_player->unkBD1 = 0;
        arg0->unk40 = 0x40000;
        setCallback(func_80043F8C_44B8C);
    }
}

void func_80043F8C_44B8C(Func43DC0State *arg0) {
    GameState *state;
    s32 i;
    s32 pad[8];

    state = getCurrentAllocation();
    if (state->gamePaused == 0) {
        arg0->unk40 = arg0->unk40 - 0x8000;
        if ((s32)0xFFF80000 >= arg0->unk40) {
            func_80069CF8_6A8F8();
        }
        arg0->unk14.y = arg0->unk14.y + arg0->unk40;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80044018_44C18(Func43DC0State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void *func_80044050_44C50(Player *arg0) {
    Func43DC0State *task;

    task = scheduleTask(func_80043DC0_449C0, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
        task->unk44 = 0;
        if (arg0->unkB84 & 2) {
            task->unk44 = 0x1000;
        }
    }
    return task;
}

extern s32 D_80090980_91580[];
extern s32 D_80090984_91584[];
extern s32 D_80090988_91588[];
extern s32 D_80090994_91594[];
extern s32 D_80090996_91596[];

typedef struct {
    u8 _pad0[0x5C];
    u8 unk5C;
} Func44CB4Allocation;

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    s32 unk14[3];  /* 0x14 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    s16 unk3C;     /* 0x3C */
} Func44CB4State;

void func_80044500_45100(Func432D8Arg *);
void func_800441A4_44DA4(Func44CB4State *);

void func_800440B4_44CB4(Func44CB4State *arg0) {
    Func44CB4Allocation *allocation;
    s32 offset;

    allocation = getCurrentAllocation();
    offset = arg0->unk3C * 9;
    createCombinedRotationMatrix(arg0, *(u16 *)&D_80090994_91594[offset], *(u16 *)&D_80090996_91596[offset]);
    memcpy(&arg0->unk14, &D_80090988_91588[arg0->unk3C * 9], 0xC);
    arg0->unk20 = (void *)(D_80090980_91580[arg0->unk3C * 9] + (D_80090984_91584[arg0->unk3C * 9] << 4));
    arg0->unk24 = func_80055DC4_569C4(allocation->unk5C);
    arg0->unk28 = func_80055DF8_569F8(allocation->unk5C);
    arg0->unk2C = 0;
    setCleanupCallback(func_80044500_45100);
    setCallbackWithContinue(func_800441A4_44DA4);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800441A4_44DA4);

void func_80044500_45100(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80044538_45138(s16 arg0) {
    Func44538TaskMem *task = (Func44538TaskMem *)scheduleTask(func_800440B4_44CB4, 0, 0, 0x32);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_80044578_45178(Func44BBCArg *arg0) {
    Transform3D matrix;
    s32 sinVal;
    s32 i;

    arg0->unkCA = arg0->unkCA + 0x100;
    sinVal = approximateSin((s16)arg0->unkCA);
    createZRotationMatrix(&matrix, (sinVal >> 5) & 0xFFFF);

    matrix.translation.x = 0xFFF7490A;
    matrix.translation.y = 0xFFF98007;
    matrix.translation.z = 0xCB326;
    func_8006B084_6BC84(&matrix, arg0, &arg0->unk3C);

    sinVal = approximateSin(arg0->unkCA);
    createZRotationMatrix(&matrix, (-(sinVal >> 5)) & 0xFFFF);

    matrix.translation.x = 0x8B6F6;
    matrix.translation.y = 0xFFF98007;
    matrix.translation.z = 0xCB326;
    func_8006B084_6BC84(&matrix, arg0, &arg0->unk78);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, &arg0->unk3C);
        enqueueDisplayListWithFrustumCull(i, &arg0->unk78);
    }
}

void func_80044684_45284(Func44BBCArg *arg0) {
    Vec3i posOutput;
    Vec3i transformOutput;
    Func43CA4GameState *allocation;
    u16 rotation;
    D_80090F90_91B90_item *item;
    s32 temp_unk18;

    allocation = (Func43CA4GameState *)getCurrentAllocation();
    rotation = func_800625A4_631A4(&allocation->unk30, &posOutput) + 0x800;
    item = func_80055D10_56910(allocation->unk5C);

    arg0->unk20 = (u8 *)func_80055E68_56A68(allocation->unk5C) + 0x10;

    arg0->unk24 = func_80055DC4_569C4(allocation->unk5C);
    arg0->unk28 = func_80055DF8_569F8(allocation->unk5C);
    arg0->unk2C = 0;

    arg0->unk3C.unk20 = (DisplayLists *)((u8 *)func_80055E68_56A68(allocation->unk5C) + 0x90);
    arg0->unk3C.unk2C = 0;
    arg0->unk3C.unk24 = arg0->unk24;
    arg0->unk3C.unk28 = arg0->unk28;

    arg0->unk78.unk20 = (DisplayLists *)((u8 *)func_80055E68_56A68(allocation->unk5C) + 0xA0);
    arg0->unk78.unk2C = 0;
    arg0->unk78.unk24 = arg0->unk24;
    arg0->unk78.unk28 = arg0->unk28;

    arg0->unkC8 = rotation + item->unk8;
    createYRotationMatrix((Transform3D *)arg0, arg0->unkC8);

    arg0->unkC6 = 0;
    transformVector2(&D_80090AA0_916A0, arg0, &transformOutput);

    arg0->unk14.x = item->unk0 + transformOutput.x;
    arg0->unk14.z = item->unk4 + transformOutput.z;
    temp_unk18 = posOutput.y + transformOutput.y;
    arg0->unkC4 = 0x30;
    arg0->unk14.y = temp_unk18;

    setCleanupCallback(func_80044CA4_458A4);
    setCallbackWithContinue(func_800447D4_453D4);
}

void func_800447D4_453D4(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState;
    Vec3i output;

    gameState = getCurrentAllocation();
    if (gameState->unk76 == 0) {
        transformVector2(&D_80090AAC_916AC, arg0, &output);
        arg0->unk14.x = arg0->unk14.x + output.x;
        arg0->unk14.z = arg0->unk14.z + output.z;
        memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
        arg0->unkB4->unkB74 = arg0->unkC6;
        if (arg0->unkC4 != 0) {
            arg0->unkC4 = arg0->unkC4 - 1;
        } else {
            arg0->unkC4 = 0xB4;
            setCallback(func_80044888_45488);
        }
    }

    func_80044578_45178(arg0);
}

void func_80044888_45488(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState;
    Vec3i output;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 != 0) {
        goto end;
    }

    {
        s16 temp = arg0->unkC6;
        if (temp != -0x400) {
            arg0->unkC6 = temp - 0x10;
        }
    }

    createCombinedRotationMatrix(arg0, arg0->unkC6, arg0->unkC8);
    transformVector2(&D_80090AAC_916AC, arg0, &output);

    arg0->unk14.x = arg0->unk14.x + output.x;
    arg0->unk14.y = arg0->unk14.y + output.y;
    arg0->unk14.z = arg0->unk14.z + output.z;

    memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
    arg0->unkB4->unkB74 = arg0->unkC6;

    {
        s16 temp = arg0->unkC4;
        if (temp != 0) {
            s16 temp2 = temp - 1;
            arg0->unkC4 = temp2;
            if (temp2 == 0x12) {
                arg0->unkB4->unkBCE |= 8;
            }
        } else {
            setCallback(func_80044990_45590);
        }
    }

end:
    func_80044578_45178(arg0);
}

void func_80044990_45590(Func44BBCArg *arg0) {
    D_80090F90_91B90_item *temp_v0;
    Func44BBCPointerTarget *temp_a1;
    Vec3i output;
    Vec3i input;
    s32 i;
    s32 temp_unk14;

    temp_v0 = func_80055D10_56910(((Func43CA4GameState *)getCurrentAllocation())->unk5C);
    temp_a1 = arg0->unkB4;
    temp_a1->unkBCE |= 2;
    arg0->unkC6 = -0x300;
    arg0->unkC4 = 0x5A;
    arg0->unkC8 = 0x1000;
    createCombinedRotationMatrix(arg0, (u16)arg0->unkC6, 0x1000);
    arg0->unk14.x = temp_v0->unkC.x + 0xFFD00000;
    arg0->unk14.y = temp_v0->unkC.y;
    temp_unk14 = temp_v0->unkC.z;
    i = 0;
    arg0->unkC0 = 0;
    arg0->unkBC = 0;
    arg0->unkB8 = 0;
    arg0->unk14.z = temp_unk14 + 0x200000;
    input.x = 0;
    input.y = 0;
    input.z = 0;
    do {
        arg0->unkC0 += 0x2000;
        i++;
        input.z += arg0->unkC0;
    } while (i < 0x5A);
    transformVector2(&input, arg0, &output);
    arg0->unk14.x -= output.x;
    arg0->unk14.y -= output.y;
    arg0->unk14.z -= output.z;
    setCallbackWithContinue(func_80044AB8_456B8);
}

void func_80044BBC_457BC(Func44BBCArg *);

void func_80044AB8_456B8(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState;
    Vec3i output;

    gameState = getCurrentAllocation();
    if (gameState->unk76 != 0) {
        goto end;
    }

    if (arg0->unkC0 > 0x20000) {
        goto skip_rotation;
    }

    {
        s16 temp = arg0->unkC6;
        if (temp != 0) {
            arg0->unkC6 = temp + 0x40;
        }
    }

    createCombinedRotationMatrix(arg0, arg0->unkC6, arg0->unkC8);

skip_rotation:
    transformVector2(&arg0->unkB8, arg0, &output);
    arg0->unk14.x = arg0->unk14.x + output.x;
    arg0->unk14.y = arg0->unk14.y + output.y;
    arg0->unk14.z = arg0->unk14.z + output.z;
    memcpy(arg0->unkB4->unkB44, &arg0->unk14, 0xC);
    arg0->unkB4->unkB74 = arg0->unkC6;

    if (arg0->unkC0 != 0) {
        arg0->unkC0 = arg0->unkC0 - 0x2000;
    } else {
        arg0->unkC4 = 4;
        arg0->unkB4->unkBCE |= 4;
        setCallback(func_80044BBC_457BC);
    }

end:
    func_80044578_45178(arg0);
}

void func_80044BBC_457BC(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState = (Func43CA4GameState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->unk76 == 0) {
        arg0->unkC4--;
        if (arg0->unkC4 == 0) {
            setCallback(func_80044C38_45838);
        }
        memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
        arg0->unkB4->unkB74 = arg0->unkC6;
    }

    func_80044578_45178(arg0);
}

void func_80044C38_45838(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState = (Func43CA4GameState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->unk76 == 0) {
        arg0->unkBC -= 0x8000;
        if (arg0->unkBC < (s32)0xFFF00000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk14.y += arg0->unkBC;
    }

    func_80044578_45178(arg0);
}

void func_80044CA4_458A4(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 _pad0[0xB4];
    void *unkB4;
} Func44CDCTaskMem;

void func_80044CDC_458DC(void *arg0) {
    Func44CDCTaskMem *task;

    task = (Func44CDCTaskMem *)scheduleTask(func_80044684_45284, 0, 0, 0x32);
    if (task != NULL) {
        task->unkB4 = arg0;
    }
}

extern void *D_80090AB8_916B8[];
extern void *D_80090ABC_916BC[];
extern s32 D_80090AC0_916C0[];

typedef struct {
    s8 active;      /* 0x00 - 1 when active, set to 0 when processed */
    s8 type;        /* 0x01 - 0 for primary item, non-zero for secondary */
    u16 itemId;     /* 0x02 - item ID to give to the player */
    Vec3i position; /* 0x04 - world position for player detection */
} ItemTriggerEntry; /* size: 0x10 */

typedef struct {
    void *unk0;              /* 0x00 */
    void *unk4;              /* 0x04 */
    ItemTriggerEntry *items; /* 0x08 - array of item trigger entries */
    void *unkC;              /* 0x0C */
    s32 *unk10;              /* 0x10 */
    s16 unk14;               /* 0x14 */
    s16 numItems;            /* 0x16 - number of item trigger entries */
} Func44D1CArg;

void func_80044DB0_459B0(Func44D1CArg *);
void func_80044EC4_45AC4(Func44D1CArg *);

typedef struct Func45010Arg Func45010Arg;
void func_80045010_45C10(Func45010Arg *);

void func_80044D1C_4591C(Func44D1CArg *arg0) {
    s16 idx;

    arg0->unkC = loadCompressedData(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    idx = arg0->unk14;
    arg0->unk10 = loadCompressedData(D_80090AB8_916B8[idx * 3], D_80090ABC_916BC[idx * 3], D_80090AC0_916C0[idx * 3]);
    arg0->unk0 = NULL;
    setCleanupCallback(func_80045010_45C10);
    setCallback(func_80044DB0_459B0);
}

extern void func_80045054_45C54(Func44D1CArg *);

void func_80044DB0_459B0(Func44D1CArg *arg0) {
    s32 i;
    s32 *ptr;
    ItemTriggerEntry *entries;
    s32 pad[2];
    s32 one;
    s32 offset;

    arg0->unk4 = (void *)((u8 *)((GameState *)getCurrentAllocation())->unk44 + 0xF80);
    arg0->items = (ItemTriggerEntry *)((s8 *)arg0->unk10 + *arg0->unk10);
    entries = *(ItemTriggerEntry *volatile *)&arg0->items;
    arg0->numItems = 0;

    if (entries->active >= 0) {
        do {
            arg0->numItems++;
        } while (entries[arg0->numItems].active >= 0);
    }

    i = 0;
    arg0->unk0 = allocateNodeMemory(arg0->numItems << 6);

    if (arg0->numItems > 0) {
        one = 1;
        ptr = &D_8009A8A4_9B4A4;
        do {
            offset = i << 4;
            *((s8 *)(offset + (s32)arg0->items)) = one;
            memcpy(ptr, (s8 *)(offset + (s32)arg0->items + 4), 0xC);
            func_8006BFB8_6CBB8(ptr - 5, (u8 *)arg0->unk0 + (i << 6));
            i++;
        } while (i < arg0->numItems);
    }

    setCallback(func_80044EC4_45AC4);
}

/**
 * Processes item trigger entries to give items to nearby players.
 *
 * For each active entry in arg0->items:
 * - Finds a player within range of the entry's position
 * - If player found (and not a boss), gives them the item:
 *   - Type 0 (primary): Sets unkBD2 to itemId, unkBD3 to 3 (or 9 in special mode)
 *   - Type 1 (secondary): Sets unkBD4 to itemId
 * - Marks entry as inactive and plays pickup sound
 */
void func_80044EC4_45AC4(Func44D1CArg *arg0) {
    s32 i;
    GameState *gameState;
    ItemTriggerEntry *entry;
    Player *player;
    ItemTriggerEntry *entryForItem;
    u8 itemFlags;
    s32 offset;
    u16 itemId;
    s32 pad[2];
    s32 three;
    s32 nine;

    gameState = (GameState *)getCurrentAllocation();
    i = 0;
    if (arg0->numItems > 0) {
        three = 3;
        nine = 9;
        offset = 0;
        do {
            entry = (ItemTriggerEntry *)(offset + (s32)arg0->items);
            if (entry->active != 0) {
                player = (Player *)func_8005B24C_5BE4C(&entry->position, -1, 0x100000);
                if (player != NULL && player->unkBD9 == 0) {
                    entryForItem = (ItemTriggerEntry *)(offset + (s32)arg0->items);
                    if (entryForItem->type == 0) {
                        itemId = entryForItem->itemId;
                        player->unkBD3 = three;
                        player->unkBD2 = (u8)itemId;
                        if (gameState->unk7A == three) {
                            player->unkBD3 = nine;
                        }
                        itemFlags = player->unkBD8 | 1;
                    } else {
                        itemFlags = player->unkBD8 | 2;
                        player->unkBD4 = (u8)entryForItem->itemId;
                    }
                    player->unkBD8 = itemFlags;
                    ((ItemTriggerEntry *)(offset + (s32)arg0->items))->active = 0;
                    func_80056B7C_5777C((Vec3i *)((s8 *)((s32)arg0->items + offset) + 4), 8);
                }
            }
            i++;
            offset += 0x10;
        } while (i < arg0->numItems);
        i = 0;
    }
    do {
        debugEnqueueCallback((u16)i, 4, &func_80045054_45C54, arg0);
        i++;
    } while (i < 4);
}

struct Func45010Arg {
    void *unk0;   /* 0x00 */
    u8 pad4[0x8]; /* 0x04 */
    void *unkC;   /* 0x0C */
    void *unk10;  /* 0x10 */
};

void func_80045010_45C10(Func45010Arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80045054_45C54);

typedef struct {
    u8 _pad0[0x14];
    s16 unk14;
} Func45434TaskMem;

void func_80045434_46034(s16 arg0) {
    Func45434TaskMem *task;

    task = (Func45434TaskMem *)scheduleTask(func_80044D1C_4591C, 0, 0, 0xD3);
    if (task != NULL) {
        task->unk14 = arg0;
    }
}
