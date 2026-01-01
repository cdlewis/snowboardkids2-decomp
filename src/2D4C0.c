#include "297B0.h"
#include "2C8F0.h"
#include "2DD40.h"
#include "B040.h"
#include "common.h"
#include "gamestate.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk0;
    Transform3D unk4;
    /* 0x24 */ u8 pad2[0x8];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u16 unk2E;
    /* 0x30 */ u8 pad3[0x10];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad3b[0x8];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad4[0x8];
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 pad5[0x5];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
} Func8002CB88Arg_arg0;

typedef struct RareEventIdleState RareEventIdleState;
struct RareEventIdleState {
    /* 0x00 */ Func8002CB88Arg_arg0 elements[2];
    /* 0xC8 */ void (*callback)(RareEventIdleState *);
    /* 0xCC */ u16 effectDelay[1];
    /* 0xCE */ u16 effectDuration;
    /* 0xD0 */ u8 pad9[0x5];
    /* 0xD5 */ u8 npcCount;
};

extern u16 D_8009ADE0_9B9E0;

typedef struct Func8002CD3CArg Func8002CD3CArg;
struct Func8002CD3CArg {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad0[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad2[0x1C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad3[0x8];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad3b[0x10];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad5[0x1];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ u8 pad6[0x3C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ u8 pad7[0x20];
    /* 0xC8 */ void (*unkC8)(Func8002CD3CArg *);
    /* 0xCC */ u16 unkCC;
    /* 0xCE */ u16 unkCE;
    /* 0xD0 */ u8 pad12[0x5];
    /* 0xD5 */ u8 unkD5;
};

extern u8 identityMatrix[];
extern u16 D_8008EF70_8FB70[];

void updateStoryMapRareEventIdle(RareEventIdleState *);

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002C8C0_2D4C0);

void updateStoryMapRareEventWave(RareEventIdleState *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CB88Arg_arg0 *ptr;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->npcCount; i++) {
        ptr = &arg0->elements[i];

        if (ptr->unk50 != 0x19) {
            if (ptr->unk50 == 0x1A) {
                ptr->unk62 = 0;
            }
        } else {
            if (ptr->unk62 != 0) {
                ptr->unk50 = 0x1A;
            }
        }

        updateStoryMapNpcModel((Func297D8Arg *)ptr);
        allocation->unk408[i] = ptr->unk4.translation.x;
        allocation->unk410[i] = ptr->unk4.translation.z;
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->callback = updateStoryMapRareEventWave;
        setCallback(updateStoryMapNpcDialogue);
    }
}

void initStoryMapRareEventIdle(RareEventIdleState *arg0) {
    GameState *allocation;
    s32 vec3[3];
    s32 i;

    allocation = getCurrentAllocation();
    vec3[2] = 0;
    vec3[0] = 0;
    vec3[1] = 0x280000;

    for (i = 0; i < arg0->npcCount; i++) {
        memcpy(&arg0->elements[i].unk4, identityMatrix, sizeof(Transform3D));
        arg0->elements[i].unk62 = 0;
        arg0->effectDelay[i] = 0;
        if (i == 0) {
            arg0->elements[0].unk4.translation.x = 0xFFF40000;
            arg0->elements[0].unk4.translation.z = 0xFFB80000;
        } else {
            arg0->elements[1].unk4.translation.x = 0x180000;
            arg0->elements[1].unk4.translation.z = 0xFFB80000;
        }

        arg0->elements[i].unk2C = 0x800 + i * 0x1000;
        arg0->elements[i].unk50 = 0x13;
        arg0->elements[i].unk52 = 0x13;
        arg0->elements[i].unk2E = 0x800 + i * 0x1000;

        createYRotationMatrix(&arg0->elements[i].unk4, arg0->elements[i].unk2C);

        memcpy(&arg0->elements[i].unk40, vec3, 0xC);

        if (i == 0) {
            arg0->elements[i].unk44 = 0x290000;
            arg0->elements[i].unk40 += 0xFFF70000;
        } else {
            arg0->elements[i].unk44 = 0x240000;
            arg0->elements[i].unk40 += 0x60000;
        }

        setupStoryMapNpcModel((Func297D8Arg *)&arg0->elements[i]);
        allocation->unk408[i] = arg0->elements[i].unk4.translation.x;
        allocation->unk410[i] = arg0->elements[i].unk4.translation.z;
        allocation->unk418[i] = D_8008EF70_8FB70[arg0->elements[i].unk5C];
    }

    arg0->effectDelay[0] = (randB() & 0x1F) + 0x28;
    setCallback(updateStoryMapRareEventIdle);
}

void updateStoryMapRareEventIdle(RareEventIdleState *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CB88Arg_arg0 *ptr;
    s32 temp_a0;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->npcCount; i++) {
        ptr = &arg0->elements[i];

        switch (ptr->unk50) {
            case 0x13:
                if (ptr->unk62 != 0) {
                    ptr->unk50 = 0x14;
                }
                break;
            case 0x14:
                ptr->unk62 = 0;
                break;
        }

        updateStoryMapNpcModel((Func297D8Arg *)ptr);
        allocation->unk408[i] = ptr->unk4.translation.x;
        allocation->unk410[i] = ptr->unk4.translation.z;
    }

    if (arg0->effectDelay[0] != 0) {
        arg0->effectDelay[0]--;
        if (arg0->effectDelay[0] == 0) {
            temp_a0 = randB() & 0x1F;
            arg0->effectDuration = (u8)temp_a0 + 0x18;
            if (D_8009ADE0_9B9E0 & 1) {
                arg0->elements[0].unk44 = 0x290000;
                spawnSpriteEffectEx(
                    (SceneModel *)arg0->elements[0].unk0,
                    0,
                    8,
                    (s16)(arg0->effectDuration - 4),
                    &arg0->elements[0].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0
                );
            } else {
                arg0->effectDuration = (u8)temp_a0 + 0x2C;
                arg0->elements[0].unk44 = 0x290000;
                spawnSpriteEffectEx(
                    (SceneModel *)arg0->elements[0].unk0,
                    0,
                    0x3E,
                    (s16)(arg0->effectDuration - 4),
                    &arg0->elements[0].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0x400
                );
                spawnSpriteEffectEx(
                    (SceneModel *)arg0->elements[1].unk0,
                    0,
                    0x3E,
                    (s16)arg0->effectDuration,
                    &arg0->elements[1].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0x1F00
                );
            }
        }
    } else {
        arg0->effectDuration--;
        if (arg0->effectDuration == 0) {
            arg0->effectDelay[0] = (randB() & 0x1F) + 0x28;
        }
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->callback = updateStoryMapRareEventIdle;
        allocation->unk42E = 1;
        arg0->effectDelay[0] = (randB() & 0x1F) + 0x28;
        arg0->effectDuration = 0;
        setCallback(updateStoryMapNpcDialogue);
    }
}

void initStoryMapRareEventMagicShow(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 effectOffset[3];
    s32 i;

    gameState = getCurrentAllocation();
    arg0->unkD2 = 0;
    arg0->elements[0].unk5A = 0;
    effectOffset[2] = 0;
    effectOffset[0] = 0;
    effectOffset[1] = 0x2C0000;

    for (i = 0; i < arg0->unkD5; i++) {
        memcpy(&arg0->elements[i].matrix, identityMatrix, sizeof(Transform3D));
        arg0->elements[i].unk62 = 0;

        if (i == 0) {
            arg0->elements[0].matrix.translation.x = 0xFFF50000;
            arg0->elements[0].matrix.translation.z = 0xFFB30000;
            arg0->elements[0].unk50 = 0;
            memcpy(&arg0->elements[0].unk40, effectOffset, 0xC);
            setAnimationIndex(arg0->elements[0].model, -1);
        } else {
            arg0->elements[1].matrix.translation.x = 0x1B0000;
            arg0->elements[1].matrix.translation.z = 0xFFB30000;
            arg0->elements[1].unk50 = 0x1B;
            memcpy(&arg0->elements[1].unk40, effectOffset, 0xC);
            setItemDisplayEnabled(arg0->elements[1].model, 1);
            setAnimationIndex(arg0->elements[1].model, 4);
        }

        arg0->elements[i].rotation = 0x800 + i * 0x1000;
        arg0->elements[i].unk2E = 0x800 + i * 0x1000;
        arg0->elements[i].unk52 = arg0->elements[i].unk50;
        createYRotationMatrix(&arg0->elements[i].matrix, arg0->elements[i].rotation);
        setupStoryMapNpcModel(&arg0->elements[i]);
        gameState->unk408[i] = arg0->elements[i].matrix.translation.x;
        gameState->unk410[i] = arg0->elements[i].matrix.translation.z;
        gameState->unk418[i] = D_8008EF70_8FB70[arg0->elements[i].unk5C];
    }

    setCallback(updateStoryMapRareEventMagicShow);
}
