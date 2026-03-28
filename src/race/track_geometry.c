#include "race/track_geometry.h"
#include "D_800AFE8C_A71FC_type.h"
#include "animation/easing_state.h"
#include "audio/audio.h"
#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "story/npc_dialogue.h"
#include "story/rare_event_npcs.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

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

extern u16 gNpcCollisionRadii[];

void updateStoryMapRareEventIdle(RareEventIdleState *);
void updateStoryMapRareEventWave(RareEventIdleState *);
void updateStoryMapRareEventJuggling(Func2E024Arg *arg0);
void updateStoryMapRareEventSledding(Func2E024Arg *);
void updateStoryMapRareEventSkating(Func2E024Arg *);

void initStoryMapRareEventWave(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 vec3[3];
    s32 i;
    Func297D8Arg *elements;

    allocation = getCurrentAllocation();
    vec3[2] = 0;
    vec3[0] = 0;
    vec3[1] = 0x2C0000;

    elements = arg0->elements;
    for (i = 0; i < arg0->unkD5; i++) {
        memcpy(&elements[i].matrix, &identityMatrix, sizeof(Transform3D));
        elements[i].unk62 = 0;

        if (i == 0) {
            arg0->elements[0].matrix.translation.x = -0x60000;
            arg0->elements[0].matrix.translation.z = -0x4A0000;
            setAnimationIndex(arg0->elements[0].model, 1);
        } else {
            arg0->elements[1].matrix.translation.x = 0x1A0000;
            arg0->elements[1].matrix.translation.z = -0x4A0000;
            setAnimationIndex(arg0->elements[1].model, 1);
        }

        elements[i].rotation = i * 0x1000;
        elements[i].rotation = 0x800 + elements[i].rotation;
        elements[i].unk50 = 0x19;
        elements[i].unk52 = 0x19;
        elements[i].unk2E = 0x800 + i * 0x1000;

        createYRotationMatrix(&elements[i].matrix, elements[i].rotation);

        memcpy(&elements[i].unk40, vec3, 0xC);

        if (i != 0) {
            elements[i].unk40 += 0x30000;
        } else {
            arg0->elements[0].unk40 += -0x60000;
        }

        spawnSpriteEffectEx(elements[i].model, 0, 0x29, -1, &elements[i].unk40, 0x10000, 0, 2, 0, 0);
        setupStoryMapNpcModel(&elements[i]);
        allocation->npcPosX[i] = elements[i].matrix.translation.x;
        allocation->npcPosZ[i] = elements[i].matrix.translation.z;
        allocation->npcCollisionRadius[i] = gNpcCollisionRadii[elements[i].unk5C];
    }

    setCallback(updateStoryMapRareEventWave);
}

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
        allocation->npcPosX[i] = ptr->unk4.translation.x;
        allocation->npcPosZ[i] = ptr->unk4.translation.z;
    }

    if (allocation->unk42A == 0x11) {
        prepareStoryMapNpcDialogue(arg0);
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
        memcpy(&arg0->elements[i].unk4, &identityMatrix, sizeof(Transform3D));
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
        allocation->npcPosX[i] = arg0->elements[i].unk4.translation.x;
        allocation->npcPosZ[i] = arg0->elements[i].unk4.translation.z;
        allocation->npcCollisionRadius[i] = gNpcCollisionRadii[arg0->elements[i].unk5C];
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
        allocation->npcPosX[i] = ptr->unk4.translation.x;
        allocation->npcPosZ[i] = ptr->unk4.translation.z;
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
        prepareStoryMapNpcDialogue(arg0);
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
        memcpy(&arg0->elements[i].matrix, &identityMatrix, sizeof(Transform3D));
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
        gameState->npcPosX[i] = arg0->elements[i].matrix.translation.x;
        gameState->npcPosZ[i] = arg0->elements[i].matrix.translation.z;
        gameState->npcCollisionRadius[i] = gNpcCollisionRadii[arg0->elements[i].unk5C];
    }

    setCallback(updateStoryMapRareEventMagicShow);
}

void updateStoryMapRareEventMagicShow(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func297D8Arg *ptr;
    u16 temp;
    s32 pad[4];

    (void)pad;
    allocation = getCurrentAllocation();

    switch (arg0->unkD2) {
        case 0:
            temp = arg0->elements[0].unk5A + 1;
            arg0->elements[0].unk5A = temp;
            if ((s16)temp == 0x3C) {
                arg0->unkD2 = 1;
                arg0->elements[0].unk5A = 0;
                arg0->elements[1].unk50 = 0x1C;
                setAnimationIndex(arg0->elements[1].model, 3);
                spawnSpriteEffectEx(arg0->elements[0].model, 0, 6, 0xF, &arg0->elements[0].unk40, 0x10000, 0, 2, 0, 0);
                spawnSpriteEffectEx(arg0->elements[1].model, 0, 6, 0xF, &arg0->elements[1].unk40, 0x10000, 0, 2, 0, 0);
            }
            break;

        case 1:
            if (arg0->elements[1].unk62 != 0) {
                arg0->elements[1].unk62 = -1;
                arg0->unkD2 = 2;
            }
            break;

        case 2:
            temp = arg0->elements[0].unk5A + 1;
            arg0->elements[0].unk5A = temp;
            if ((s16)temp == 0x14) {
                arg0->elements[1].unk62 = 0;
                arg0->elements[1].unk50 = 0x1E;
                setAnimationIndex(arg0->elements[1].model, 2);
                arg0->elements[0].unk50 = 0x17;
                setAnimationIndex(arg0->elements[0].model, 2);
                arg0->unkD2 = 3;
                arg0->elements[0].unk5A = 0;
            }
            break;

        case 3:
            if (arg0->elements[1].unk62 != 0) {
                arg0->elements[1].unk50 = 0x1F;
                arg0->elements[0].unk50 = 0x18;
                arg0->unkD2 = 4;
            }
            break;

        case 4:
            arg0->elements[0].unk62 = 0;
            arg0->elements[1].unk62 = 0;
            if (getFreeNodeCount(2) == 0x14) {
                arg0->elements[0].unk40 = (s32)0xFFF90000;
                arg0->elements[0].unk44 = 0x220000;
                spawnSpriteEffectEx(
                    arg0->elements[0].model,
                    0,
                    0x24,
                    -1,
                    &arg0->elements[0].unk40,
                    0x10000,
                    1,
                    2,
                    1,
                    0
                );
                arg0->elements[1].unk40 = 0x90000;
                arg0->elements[1].unk44 = 0x260000;
                spawnSpriteEffectEx(
                    arg0->elements[1].model,
                    0,
                    0x24,
                    -1,
                    &arg0->elements[1].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0
                );
            } else if (getFreeNodeCount(2) == 0x13) {
                arg0->elements[0].unk40 = (s32)0xFFF90000;
                arg0->elements[0].unk44 = 0x220000;
                spawnSpriteEffectEx(
                    arg0->elements[0].model,
                    0,
                    0x24,
                    -1,
                    &arg0->elements[0].unk40,
                    0x10000,
                    1,
                    2,
                    1,
                    0
                );
            }
            break;
    }

    ptr = arg0->elements;
    for (i = 0; i < 2; i++) {
        updateStoryMapNpcModel(&ptr[i]);
        allocation->npcPosX[i] = ptr[i].matrix.translation.x;
        allocation->npcPosZ[i] = ptr[i].matrix.translation.z;
    }

    if (allocation->unk42A == 0x11) {
        if (arg0->unkD2 == 4) {
            allocation->unk42E = 2;
        } else {
            allocation->unk42E = 1;
        }
        prepareStoryMapNpcDialogue(arg0);
        arg0->callback = updateStoryMapRareEventMagicShow;
        setCallback(updateStoryMapNpcDialogue);
    }
}

void initStoryMapRareEventJuggling(Func2E024Arg *container) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *characters;

    gameState = getCurrentAllocation();
    characters = container->elements;

    for (i = 0; i < container->unkD5; i++) {
        memcpy(&characters[i].matrix, &identityMatrix, sizeof(Transform3D));
        characters[i].unk62 = 0;
        container->unkCC[i] = 0;

        if (i == 0) {
            container->elements[0].matrix.translation.x = -0x80000;
            container->elements[0].matrix.translation.z = -0x4E0000;
            setAnimationIndex(container->elements[0].model, 1);
            container->elements[0].unk50 = 0x11;
            container->elements[0].unk44 = 0x260000;
            container->elements[0].unk5E = 0;
            container->elements[0].unk40 = 0;
            container->elements[0].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[0].model,
                0,
                0x29,
                -1,
                &container->elements[0].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        } else {
            container->elements[1].matrix.translation.x = 0x1C0000;
            container->elements[1].matrix.translation.z = -0x4E0000;
            setAnimationIndex(container->elements[1].model, 2);
            container->elements[1].unk50 = 0x1E;
            container->elements[1].unk5E = 1;
            container->elements[1].unk40 = 0x40000;
            container->elements[1].unk44 = 0x280000;
            container->elements[1].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[1].model,
                0,
                0x24,
                -1,
                &container->elements[1].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        }

        characters[i].rotation = 0x800 + i * 0x1000;
        characters[i].unk2E = 0x800 + i * 0x1000;
        characters[i].unk52 = characters[i].unk50;
        createYRotationMatrix(&characters[i].matrix, characters[i].rotation);
        setupStoryMapNpcModel(&characters[i]);
        gameState->npcPosX[i] = characters[i].matrix.translation.x;
        gameState->npcPosZ[i] = characters[i].matrix.translation.z;
        gameState->npcCollisionRadius[i] = gNpcCollisionRadii[characters[i].unk5C];
    }

    setCallback(updateStoryMapRareEventJuggling);
}

void updateStoryMapRareEventJuggling(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *elements;
    s32 state;

    gameState = getCurrentAllocation();
    elements = arg0->elements;

    for (i = 0; i < arg0->unkD5; i++) {
        state = elements[i].unk5E;

        if (state < 0) {
            goto cont;
        }

        if (state >= 2) {
            if (state >= 4) {
                goto cont;
            }
            goto case23;
        }

        if (elements[i].unk62 == 0) {
            goto cont;
        }
        if (i == 0) {
            arg0->elements[0].unk50 = 0x12;
            arg0->elements[0].unk5E = 2;
            goto cont;
        }
        arg0->elements[1].unk50 = 0x1F;
        arg0->elements[1].unk5E = 3;
        goto cont;

    case23:
        elements[i].unk62 = 0;
        if (getFreeNodeCount(2) == 0x14) {
            spawnSpriteEffectEx(arg0->elements[0].model, 0, 0x29, -1, &arg0->elements[0].unk40, 0x10000, 0, 2, 0, 0);
            spawnSpriteEffectEx(arg0->elements[1].model, 0, 0x24, -1, &arg0->elements[1].unk40, 0x10000, 0, 2, 0, 0);
        }

    cont:
        updateStoryMapNpcModel(&elements[i]);
        gameState->npcPosX[i] = elements[i].matrix.translation.x;
        gameState->npcPosZ[i] = elements[i].matrix.translation.z;
    }

    if (gameState->unk42A == 0x11) {
        gameState->unk42E = 1;
        arg0->unkCC[0] = 1;
        arg0->unkCC[1] = 1;
        prepareStoryMapNpcDialogue(arg0);
        arg0->callback = updateStoryMapRareEventJuggling;
        setCallback(updateStoryMapNpcDialogue);
    }
}

void initStoryMapRareEventSledding(Func2E024Arg *arg0) {
    Transform3D localMatrix;
    Transform3D worldMatrix;
    s32 effectOffset[3];
    GameState *allocation;
    Transform3D *localMatrixPtr;
    s32 i;
    s32 radius;
    s32 negRadius;
    s32 result;
    s32 angle;
    Transform3D *new_var;
    s32 sinVal;

    allocation = (GameState *)getCurrentAllocation();
    effectOffset[0] = 0xFFFD0000;
    effectOffset[2] = 0;
    effectOffset[1] = 0x240000;

    for (i = 0; i < arg0->unkD5; i++) {
        new_var = &worldMatrix;
        localMatrixPtr = &localMatrix;

        memcpy(&worldMatrix, &identityMatrix, sizeof(Transform3D));
        memcpy(&localMatrix, new_var, sizeof(Transform3D));
        memcpy(&arg0->elements[i].matrix, localMatrixPtr, sizeof(Transform3D));

        angle = 0xB98;
        if (i == 1) {
            radius = 0x4400;
            arg0->elements[1].unk5A = 0x22;
        } else {
            radius = 0x5200;
            arg0->elements[i].unk5A = 0x1A;
        }

        arg0->unkCC[i] = 0;
        angle = (s16)angle;
        sinVal = approximateSin(angle);
        result = sinVal * (-radius);
        if (result < 0) {
            result += 0x1FFF;
        }

        arg0->elements[i].matrix.translation.x = (result >> 13) << 8;
        result = approximateCos(angle) * (-radius);
        if (result <= (0 - 1)) {
            result += 0x1FFF;
        }

        arg0->elements[i].matrix.translation.z = (result >> 13) << 8;
        arg0->elements[i].unk5E = 0;

        memcpy(&arg0->elements[i].unk40, effectOffset, 0xC);

        arg0->elements[i].unk50 = 9;
        arg0->elements[i].unk52 = 9;
        arg0->elements[i].rotation = 0x800;
        arg0->elements[i].unk2E = 0x800;

        memcpy(&worldMatrix.translation, &arg0->elements[i].matrix.translation, 0xC);

        createYRotationMatrix(&localMatrix, arg0->elements[i].rotation);
        createYRotationMatrix(
            &worldMatrix,
            atan2Fixed(worldMatrix.translation.x, angle = worldMatrix.translation.z) & 0xFFFF
        );
        func_8006B084_6BC84(&localMatrix, new_var, &arg0->elements[i].matrix);
        setupStoryMapNpcModel(&arg0->elements[i]);
        allocation->npcPosX[i] = arg0->elements[i].matrix.translation.x;
        allocation->npcPosZ[i] = arg0->elements[i].matrix.translation.z;
        allocation->npcCollisionRadius[i] = gNpcCollisionRadii[arg0->elements[i].unk5C];
    }

    setCallback(&updateStoryMapRareEventSledding);
}

void updateStoryMapRareEventSledding(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 completedCount;
    s32 i;
    Func297D8Arg *element;
    Vec3i newPosition;
    Vec3i currentPosition;
    Transform3D localMatrix;
    Transform3D worldMatrix;
    Vec3i *pCurrentPosition;
    Vec3i *pNewPosition;
    s16 movementSpeed;
    s16 facingAngle;
    s16 angle2;
    Vec3i *translationPtr;

    gameState = getCurrentAllocation();
    completedCount = 0;

    {
        u16 temp = arg0->unkCC[1];
        if (temp != 0) {
            u16 newTemp = temp + 1;
            arg0->unkCC[1] = newTemp;
            if ((newTemp & 0xFFFF) == 0x1E) {
                playSoundEffect(0xC9);
                arg0->unkCC[1] = 0;
            }
        }
    }

    for (i = 0; i < arg0->unkD5; i++) {
        pCurrentPosition = &currentPosition;
        pNewPosition = &newPosition;
        element = &arg0->elements[i];

        memcpy(&localMatrix, &identityMatrix, sizeof(Transform3D));
        memcpy(&worldMatrix, &element->matrix, sizeof(Transform3D));

        switch (element->unk5E) {
            case 0:
                translationPtr = &element->matrix.translation;
                memcpy(pCurrentPosition, translationPtr, sizeof(Vec3i));
                memcpy(pNewPosition, pCurrentPosition, sizeof(Vec3i));

                movementSpeed = -element->unk5A;

                facingAngle = 0x4400;
                if (i == 0) {
                    facingAngle = 0x5200;
                }

                newPosition.x += ((movementSpeed * (currentPosition.z >> 8)) / facingAngle) << 12;
                newPosition.z += ((-movementSpeed * (currentPosition.x >> 8)) / facingAngle) << 12;
                memcpy(translationPtr, pNewPosition, sizeof(Vec3i));
                break;
            case 1:
                completedCount++;
                break;
        }

        if (element->unk5E != 1) {
            createYRotationMatrix(&localMatrix, element->rotation);
            facingAngle = atan2Fixed(element->matrix.translation.x, element->matrix.translation.z);
            createYRotationMatrix(&worldMatrix, facingAngle & 0xFFFF);
            memcpy(&worldMatrix.translation, &element->matrix.translation, 0xC);
            func_8006B084_6BC84(&localMatrix, &worldMatrix, &element->matrix);
            updateStoryMapNpcModel(element);
            gameState->npcPosX[i] = element->matrix.translation.x;
            gameState->npcPosZ[i] = element->matrix.translation.z;
            if ((u16)((facingAngle - 0x1001) & 0xFFFF) < 0x468) {
                element->unk5E = 1;
            }
        } else if ((completedCount & 0xFF) == 2) {
            terminateCurrentTask();
        }
    }

    if ((completedCount & 0xFF) != 2) {
        facingAngle = atan2Fixed(arg0->elements[0].matrix.translation.x, arg0->elements[0].matrix.translation.z);
        angle2 = atan2Fixed(arg0->elements[1].matrix.translation.x, arg0->elements[1].matrix.translation.z);

        if (facingAngle >= 0x1001) {
            facingAngle -= 0x2000;
        }
        if (angle2 >= 0x1001) {
            angle2 -= 0x2000;
        }

        {
            s32 diff = facingAngle - angle2;
            if (diff < 0) {
                diff = -diff;
            }

            if (diff >= 0x109) {
                if (angle2 < facingAngle) {
                    spawnSpriteEffectEx(
                        arg0->elements[0].model,
                        0,
                        0x24,
                        0x14,
                        &arg0->elements[0].unk40,
                        0x10000,
                        1,
                        2,
                        0,
                        0
                    );
                    arg0->elements[0].unk5A = arg0->elements[1].unk5A + 0x10;
                    {
                        u16 temp = arg0->unkCC[0] + 1;
                        arg0->unkCC[0] = temp;
                        if ((temp & 0xFFFF) == 1) {
                            arg0->unkCC[1] = 1;
                            playSoundEffect(0x9B);
                        }
                    }
                } else {
                    arg0->elements[1].unk5A = (u16)(arg0->elements[0].unk5A + 0x10);
                    spawnSpriteEffectEx(
                        arg0->elements[1].model,
                        0,
                        0x24,
                        0x14,
                        &arg0->elements[1].unk40,
                        0x10000,
                        1,
                        2,
                        0,
                        0
                    );
                }
            }
        }
    }
}

void updateStoryMapRareEventSnowman(Func2E024Arg *arg0);

void initStoryMapRareEventSnowman(Func2E024Arg *container) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *elements;

    gameState = getCurrentAllocation();
    elements = container->elements;

    for (i = 0; i < container->unkD5; i++) {
        Func297D8Arg *element = &elements[i];

        memcpy(&elements[i].matrix, &identityMatrix, 0x20);
        element->unk62 = 0;

        if (i == 0) {
            container->elements[0].matrix.translation.x = 0xFFFC0000;
            container->elements[0].matrix.translation.z = 0xFFBC0000;
            setAnimationIndex(container->elements[0].model, 4);
            container->elements[0].unk50 = 0x11;
            container->elements[0].unk5E = 0x63;
            container->elements[0].unk44 = 0x1D0000;
            container->elements[0].unk40 = 0xFFFC0000;
            container->elements[0].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[0].model,
                0,
                0x2F,
                -1,
                &container->elements[0].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        } else {
            container->elements[1].matrix.translation.x = 0x200000;
            container->elements[1].unk5E = 0x62;
            container->elements[1].unk44 = 0x260000;
            container->elements[1].matrix.translation.z = 0xFFBC0000;
            container->elements[1].unk50 = 0;
            container->elements[1].unk5A = 0;
            container->elements[1].unk40 = 0;
            container->elements[1].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[1].model,
                0,
                7,
                0x2D,
                &container->elements[1].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        }

        element->rotation = 0x1800;
        element->unk2E = 0x1800;
        element->unk52 = element->unk50;
        createYRotationMatrix(&elements[i].matrix, element->rotation);
        setupStoryMapNpcModel(element);
        gameState->npcPosX[i] = elements[i].matrix.translation.x;
        gameState->npcPosZ[i] = elements[i].matrix.translation.z;
        gameState->npcCollisionRadius[i] = gNpcCollisionRadii[element->unk5C];
    }

    setCallback(updateStoryMapRareEventSnowman);
}

void updateStoryMapRareEventSnowman(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *elements;

    gameState = getCurrentAllocation();

    switch (arg0->elements[1].unk5E) {
        case 0x62:
            arg0->elements[1].unk5A++;
            if (arg0->elements[1].unk5A == 0x2D) {
                arg0->elements[1].unk50 = 0x10;
                arg0->elements[1].unk5A = 0;
                arg0->elements[1].unk5E = 1;
            }
            break;
        case 1:
            if (arg0->elements[1].unk62 != 0) {
                arg0->elements[1].unk50 = 0x11;
                arg0->elements[1].unk5E = 0x63;
            }
            break;
        case 0x63:
            arg0->elements[0].unk62 = 0;
            arg0->elements[1].unk62 = 0;
            break;
    }

    elements = arg0->elements;
    for (i = 0; i < arg0->unkD5; i++) {
        updateStoryMapNpcModel(&elements[i]);
        gameState->npcPosX[i] = elements[i].matrix.translation.x;
        gameState->npcPosZ[i] = elements[i].matrix.translation.z;
    }

    if (gameState->unk42A == 0x11) {
        prepareStoryMapNpcDialogue(arg0);
        arg0->callback = updateStoryMapRareEventSnowman;
        setCallback(updateStoryMapNpcDialogue);
    }
}

void updateStoryMapRareEventCheering(Func2E024Arg *);

void initStoryMapRareEventCheering(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;

    allocation = getCurrentAllocation();
    i = 0;
    arg0->unkD2 = 0;

    for (; i < arg0->unkD5; i++) {
        memcpy(&arg0->elements[i].matrix, &identityMatrix, sizeof(Transform3D));
        arg0->elements[i].unk62 = 0;
        arg0->elements[i].unk5A = 0;

        if (i == 0) {
            arg0->elements[0].matrix.translation.x = -0x60000;
            arg0->elements[0].matrix.translation.z = -0x4E0000;
            arg0->elements[0].unk50 = 0;
            arg0->elements[0].unk5E = 3;
        } else {
            arg0->elements[1].matrix.translation.x = 0x1E0000;
            arg0->elements[1].matrix.translation.z = -0x4E0000;
            arg0->elements[1].unk50 = 0x13;
            arg0->elements[1].unk5E = 0;
        }

        arg0->elements[i].unk48 = 0;
        arg0->elements[i].unk40 = 0;

        if (i == 0) {
            arg0->elements[0].unk44 = 0x290000;
            arg0->elements[0].unk40 += -0x90000;
        } else {
            arg0->elements[i].unk40 = 0x60000;
            arg0->elements[i].unk44 = 0x290000;
        }

        arg0->elements[i].rotation = 0x800 + i * 0x1000;
        arg0->elements[i].unk2E = 0x800 + i * 0x1000;
        arg0->elements[i].unk52 = arg0->elements[i].unk50;
        createYRotationMatrix(&arg0->elements[i].matrix, arg0->elements[i].rotation);
        setupStoryMapNpcModel(&arg0->elements[i]);
        allocation->npcPosX[i] = arg0->elements[i].matrix.translation.x;
        allocation->npcPosZ[i] = arg0->elements[i].matrix.translation.z;
        allocation->npcCollisionRadius[i] = gNpcCollisionRadii[arg0->elements[i].unk5C];
    }

    arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
    arg0->unkCC[1] = 0;
    setCallback(updateStoryMapRareEventCheering);
}

void updateStoryMapRareEventCheering(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func297D8Arg *ptr;
    int new_var;
    allocation = getCurrentAllocation();
    for (i = 0; i < arg0->unkD5; i++) {
        ptr = &arg0->elements[i];
        switch (ptr->unk5E) {
            case 0:
                if (ptr->unk62 != 0) {
                    ptr->unk62 = 0;
                    ptr->unk50 = 0x14;
                    ptr->unk5E = 1;
                }
                break;

            case 1:
                ptr->unk5A++;
                if ((ptr->unk5A) >= 0x3C) {
                    if (ptr->unk62 != 0) {
                        ptr->unk5A = 0;
                        ptr->unk62 = 0;
                        ptr->unk50 = 0x15;
                        ptr->unk5E = 2;
                        if (arg0->unkD2 == 2) {
                            arg0->elements[0].unk5E = 7;
                            arg0->elements[0].unk50 = 0xA;
                        }
                    }
                }
                break;

            case 2:
                if (ptr->unk62 != 0) {
                    u8 d2;
                    ptr->unk62 = 0;
                    d2 = arg0->unkD2;
                    if (d2 < 2) {
                        arg0->elements[d2].unk5E = 4;
                        ptr->unk50 = 0;
                        ptr->unk5E = 3;
                    } else {
                        ptr->unk50 = 0x13;
                        ptr->unk5E = 5;
                    }
                    arg0->unkD2++;
                }
                break;

            case 4:
                ptr->unk5E = 0;
                ptr->unk5A = 0;
                ptr->unk50 = 0x13;
                break;

            case 5:
                if (ptr->unk62 != 0) {
                    ptr->unk62 = 0;
                    ptr->unk50 = 0x14;
                    ptr->unk5E = 6;
                }
                break;

            case 7:
                if (arg0->elements[0].unk62 != 0) {
                    arg0->elements[0].unk50++;
                    if ((arg0->elements[0].unk50) >= 0xD) {
                        arg0->elements[0].unk50 = 0;
                        arg0->elements[0].unk5E = 8;
                    }
                }
                break;

            case 8:
                if (arg0->elements[1].unk5E == 6) {
                    arg0->elements[0].unk50 = 0x20;
                    arg0->elements[0].unk5E = 9;
                }
                break;

            case 6:

            case 9:
                ptr->unk62 = 0;
                break;
        }

        updateStoryMapNpcModel(ptr);
        allocation->npcPosX[i] = ptr->matrix.translation.x;
        allocation->npcPosZ[i] = ptr->matrix.translation.z;
    }

    if (arg0->unkCC[0] != 0) {
        arg0->unkCC[0]--;
        if (!(arg0->unkCC[0] & 0xFFFF)) {
            unsigned char temp_v0_5 = randB() & 0x1F;
            temp_v0_5 &= 0xFF;
            new_var = 0x10000;
            arg0->unkCC[1] = temp_v0_5 + 0x1E;
            spawnSpriteEffectEx(
                arg0->elements[0].model,
                0,
                0x3E,
                temp_v0_5 + 0x1A,
                &arg0->elements[0].unk40,
                new_var,
                0,
                2,
                0,
                0x400
            );
            spawnSpriteEffectEx(
                arg0->elements[1].model,
                0,
                0x3E,
                (s16)(arg0->unkCC[1] - 4),
                &arg0->elements[1].unk40,
                new_var,
                0,
                2,
                0,
                0x1F00
            );
        }
    } else {
        u16 temp2 = arg0->unkCC[1];
        s16 temp_v0_7;
        if (temp2 != 0) {
            temp_v0_7 = temp2 - 1;
            arg0->unkCC[1] = temp_v0_7;
            if (!(temp_v0_7 & 0xFFFF)) {
                arg0->unkCC[0] = (randB() & 0x1F) + 0x2D;
            }
        }
    }
    do {
        if (allocation->unk42A == 0x11) {
            prepareStoryMapNpcDialogue(arg0);
            allocation->unk42E = 1;
            arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
            arg0->unkCC[1] = 0;
            arg0->callback = updateStoryMapRareEventCheering;
            setCallback(updateStoryMapNpcDialogue);
        }
    } while (0);
}

void initStoryMapRareEventSkating(Func2E024Arg *arg0) {
    Transform3D localMatrix;
    Transform3D worldMatrix;
    GameState *gameState;
    Transform3D *localMatrixPtr;
    s32 i;
    s32 radius;
    s32 negRadius;
    s32 result;
    s16 angle;
    Transform3D *worldMatrixPtr;

    gameState = getCurrentAllocation();
    angle = 0xB98;

    for (i = 0; i < arg0->unkD5; i++) {
        worldMatrixPtr = &worldMatrix;
        localMatrixPtr = &localMatrix;

        memcpy(&worldMatrix, &identityMatrix, sizeof(Transform3D));
        memcpy(&localMatrix, worldMatrixPtr, sizeof(Transform3D));
        memcpy(&arg0->elements[i].matrix, localMatrixPtr, sizeof(Transform3D));

        radius = 0x4400;
        if (i == 0) {
            radius = 0x5200;
            arg0->elements[0].unk50 = 9;
            arg0->unkCC[0] = 0x3C;
        } else {
            arg0->elements[i].unk50 = 0x12;
            setAnimationIndex(arg0->elements[i].model, 2);
            arg0->unkCC[1] = 0x78;
        }

        negRadius = -radius;
        result = approximateSin(angle) * negRadius;
        if (result < 0) {
            result += 0x1FFF;
        }
        arg0->elements[i].matrix.translation.x = (result >> 13) << 8;

        result = approximateCos(angle) * negRadius;
        if (result < 0) {
            result += 0x1FFF;
        }
        arg0->elements[i].matrix.translation.z = (result >> 13) << 8;

        arg0->elements[i].unk5A = (randB() & 0x1F) + 1;
        arg0->elements[i].unk5E = 0;
        arg0->elements[i].rotation = 0x800;
        arg0->elements[i].unk2E = 0x800;
        arg0->elements[i].unk52 = arg0->elements[i].unk50;
        memcpy(&worldMatrix.translation, &arg0->elements[i].matrix.translation, 0xC);
        createYRotationMatrix(&localMatrix, arg0->elements[i].rotation);
        angle = atan2Fixed(worldMatrix.translation.x, worldMatrix.translation.z);
        createYRotationMatrix(&worldMatrix, angle & 0xFFFF);
        func_8006B084_6BC84(&localMatrix, worldMatrixPtr, &arg0->elements[i].matrix);
        setupStoryMapNpcModel(&arg0->elements[i]);
        gameState->npcPosX[i] = arg0->elements[i].matrix.translation.x;
        gameState->npcPosZ[i] = arg0->elements[i].matrix.translation.z;
        gameState->npcCollisionRadius[i] = gNpcCollisionRadii[arg0->elements[i].unk5C];
    }

    setCallback(updateStoryMapRareEventSkating);
}

void updateStoryMapRareEventSkating(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 completedCount;
    s32 i;
    Func297D8Arg *element;
    Vec3i newPosition;
    Vec3i currentPosition;
    Transform3D localMatrix;
    Transform3D worldMatrix;
    s16 movementSpeed;
    s32 radiusFactor;
    s16 facingAngle;
    s32 unused;

    gameState = getCurrentAllocation();
    completedCount = 0;

    for (i = 0; i < arg0->unkD5; i++) {
        element = &arg0->elements[i];

        memcpy(&localMatrix, &identityMatrix, sizeof(Transform3D));
        memcpy(&worldMatrix, &element->matrix, sizeof(Transform3D));

        switch (element->unk5E) {
            case 0:
                element->unk5A--;
                if ((element->unk5A << 16) == 0) {
                    element->unk5E = 1;
                }
                break;
            case 1:
                memcpy(&currentPosition, &element->matrix.translation, sizeof(Vec3i));
                memcpy(&newPosition, &currentPosition, sizeof(Vec3i));

                if (i == 0) {
                    radiusFactor = 0x5200;
                    movementSpeed = -0x28;
                } else {
                    radiusFactor = 0x4400;
                    movementSpeed = -0x18;
                }

                newPosition.x += ((movementSpeed * (currentPosition.z >> 8)) / radiusFactor) << 12;
                newPosition.z += ((-movementSpeed * (currentPosition.x >> 8)) / radiusFactor) << 12;
                memcpy(&element->matrix.translation, &newPosition, sizeof(Vec3i));
                break;
            case 2:
                completedCount++;
                break;
        }

        {
            u16 temp = arg0->unkCC[0];
            if (temp != 0) {
                temp--;
                arg0->unkCC[0] = temp;
                if ((temp & 0xFFFF) == 0) {
                    playSoundEffect(0xF5);
                }
            }
        }

        {
            u16 temp = arg0->unkCC[1];
            if (temp != 0) {
                temp--;
                arg0->unkCC[1] = temp;
                if ((temp & 0xFFFF) == 0) {
                    playSoundEffect(0x10E);
                }
            }
        }

        if (element->unk5E != 2) {
            createYRotationMatrix(&localMatrix, element->rotation);
            facingAngle = atan2Fixed(element->matrix.translation.x, element->matrix.translation.z);
            createYRotationMatrix(&worldMatrix, facingAngle & 0xFFFF);
            memcpy(&worldMatrix.translation, &element->matrix.translation, 0xC);
            func_8006B084_6BC84(&localMatrix, &worldMatrix, &element->matrix);
            updateStoryMapNpcModel(element);
            gameState->npcPosX[i] = element->matrix.translation.x;
            gameState->npcPosZ[i] = element->matrix.translation.z;
            if (((u16)((facingAngle - 0x1001) & 0xFFFF) < 0x468)) {
                element->unk5E = 2;
            }
        } else if ((completedCount & 0xFF) == element->unk5E) {
            terminateCurrentTask();
        }
    }
}

u16 gNpcCollisionRadii[] = {
    0x000F, 0x000F, 0x0013, 0x0013, 0x0013, 0x000F, 0x000F, 0x000F, 0x0013, 0x0000, 0x0002, 0x0304, 0x0506, 0x0907,
    0x0001, 0x0304, 0x0506, 0x0809, 0x0001, 0x0204, 0x0506, 0x0907, 0x0001, 0x0203, 0x0506, 0x0807, 0x0001, 0x0203,
    0x0406, 0x0907, 0x0001, 0x0203, 0x0405, 0x0807, 0x000D, 0x0602, 0x000D, 0x0602, 0x000D, 0x0602, 0x000D, 0x0602,
    0x0306, 0x090D, 0x0306, 0x090D, 0x0306, 0x090D, 0x0306, 0x090D, 0x020E, 0x0F07, 0x020E, 0x0F07, 0x020E, 0x0F07,
    0x020E, 0x0F07, 0x080E, 0x0407, 0x080E, 0x0407, 0x080E, 0x0407, 0x080E, 0x0407, 0x0105, 0x0A0B, 0x0105, 0x0A0B,
    0x0105, 0x0A0B, 0x0105, 0x0A0B, 0x090A, 0x0E0F, 0x090A, 0x0E0F, 0x090A, 0x0E0F, 0x090A, 0x0E0F, 0x0C02, 0x0304,
    0x0C02, 0x0304, 0x0C02, 0x0304, 0x0C02, 0x0304, 0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D,
    0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D,
};

// Array indexed by (rareEventId * 2 + npcIndex)
// Only byte 0 is read by prepareStoryMapNpcDialogue (face-player flag)
// Bytes 1-15 may be unused or referenced by unmapped code
u8 dialogueNpcFacesPlayer[] = {
    0x00, // [event 0, NPC 0] face player flag
    0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00
};

s8 dialogueNpcAnimations[] = {
    0x01, 0x01, 0x00, -1,   -1,   0x04, 0x01, 0x02, -1,   -1,   0x04, -1,   0x04, 0x00,
    -1,   -1,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void prepareStoryMapNpcDialogue(void *ptr) {
    Func2E024Arg *state = ptr;
    s32 i;

    for (i = 0; i < 2; i++) {
        state->elements[i].unk56 = state->elements[i].unk50;
        state->elements[i].unk5F = getAnimationIndex(state->elements[i].model);

        if (((state->unkD4 == 1) & (i == 0)) && D_800AFE8C_A71FC->playerBoardIds[0] == 3) {
            setAnimationIndex(state->elements[i].model, -1);
            state->elements[i].unk48 = 0;
            state->elements[i].unk40 = 0;
            state->elements[i].unk44 = 0x240000;
        } else if (((state->unkD4 == 6) & (i == 1)) && D_800AFE8C_A71FC->playerBoardIds[0] == 3) {
            setAnimationIndex(state->elements[i].model, -1);
        } else {
            setAnimationIndex(state->elements[i].model, dialogueNpcAnimations[state->unkD4 * 2 + i]);
        }

        if (dialogueNpcFacesPlayer[state->unkD4 * 2 + i] != 0) {
            state->elements[i].unk50 = 0;
            state->ctrl[i] = 1;
        } else {
            state->elements[i].unk30 = state->elements[i].rotation;
            state->ctrl[i] = 3;
        }
    }

    state->unkD3 = 0;
}

s32 getPlayerGold(void) {
    return D_800AFE8C_A71FC->gold;
}

void addPlayerGold(s32 amount) {
    s32 maxGold = 0x98967F;

    D_800AFE8C_A71FC->gold += amount;

    if (D_800AFE8C_A71FC->gold > maxGold) {
        D_800AFE8C_A71FC->gold = maxGold;
    }

    if (D_800AFE8C_A71FC->gold < 0) {
        D_800AFE8C_A71FC->gold = 0;
    }
}
