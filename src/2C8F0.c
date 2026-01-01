#include "20F0.h"
#include "297B0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern void *D_8008EBF0_8F7F0[];
extern void func_8002BFEC_2CBEC(void *);
extern void func_8002C570_2D170(void *);

extern void spawnSpriteEffectEx(s32, s32, s32, s32, void *, s32, s32, s32, s32, s32);
extern u8 D_8008EBE0_8F7E0[];
extern void *D_8008ECF0_8F8F0;
extern void *D_8008ED00_8F900;

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ u8 pad4[0x3C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x4];
    /* 0x4C */ void *unk4C;
    /* 0x50 */ u8 pad50[0x14];
} Elem2C8F0; // size 0x64

typedef struct {
    /* 0x00 */ Elem2C8F0 elems[2];
    /* 0xC8 */ u8 padC8[0xC];
    /* 0xD4 */ u8 eventTypeIndex;
    /* 0xD5 */ u8 modelCount;
} Struct2C8F0;

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BCF0_2C8F0);

void awaitStoryMapRareEventReady(Struct2C8F0 *arg0) {
    GameState *gameState = (GameState *)getCurrentAllocation();

    if (gameState->unk429 == 0) {
        setCallback(D_8008EBF0_8F7F0[arg0->eventTypeIndex]);
    }
}

void nullStoryMapRareEventCallback(void) {
}

void destroyStoryMapRareEventModels(Struct2C8F0 *arg0) {
    s32 i;

    for (i = 0; i < arg0->modelCount; i++) {
        arg0->elems[i].model = destroySceneModel(arg0->elems[i].model);
    }
}

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ u16 targetRotation;
    /* 0x32 */ u8 pad32[0x96];
    /* 0xC8 */ void *returnCallback;
    /* 0xCC */ u8 padCC[0x7];
    /* 0xD3 */ u8 dialogueState;
} StoryMapDialogueState;

void updateStoryMapNpcDialogue(StoryMapDialogueState *arg0) {
    GameState *gameState;
    s32 i;
    StoryMapDialogueState *npc;

    gameState = (GameState *)getCurrentAllocation();

    switch (arg0->dialogueState) {
        case 0:
            func_8002BFEC_2CBEC(arg0);
            for (i = 0; i < gameState->unk41C; i++) {
                npc = (StoryMapDialogueState *)((u8 *)arg0 + i * 0x64);
                createYRotationMatrix(&npc->matrix, npc->targetRotation);
                updateStoryMapNpcModel((Func297D8Arg *)npc);
            }
            break;
        case 1:
            func_8002C570_2D170(arg0);
            for (i = 0; i < gameState->unk41C; i++) {
                npc = (StoryMapDialogueState *)((u8 *)arg0 + i * 0x64);
                createYRotationMatrix(&npc->matrix, npc->targetRotation);
                updateStoryMapNpcModel((Func297D8Arg *)npc);
            }
            break;
    }

    if (gameState->unk42A == 0) {
        setCallback(arg0->returnCallback);
    }
}

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BFEC_2CBEC);

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002C570_2D170);

void configureRareEventSpriteEffect(Struct2C8F0 *rareEvent, s32 elemIndex) {
    Elem2C8F0 *elem;
    u8 eventType;
    s32 offset;

    eventType = rareEvent->eventTypeIndex;

    if ((eventType == 1) & (elemIndex == 0)) {
        if (D_800AFE8C_A71FC->unk9[0] == 3) {
            elem = &rareEvent->elems[elemIndex];
            elem->unk4C = &D_8008ED00_8F900;
            rareEvent->elems[0].unk44 = 0x300000;
            spawnSpriteEffectEx(
                (s32)elem->model,
                0,
                0x1F,
                D_8008EBE0_8F7E0[rareEvent->eventTypeIndex * 2 + elemIndex] - 4,
                &elem->unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
            return;
        }
    }

    if ((rareEvent->eventTypeIndex == 6) & (elemIndex == 1)) {
        if (D_800AFE8C_A71FC->unk9[0] == 3) {
            offset = elemIndex * 0x64;
            ((Elem2C8F0 *)((u8 *)rareEvent + offset))->unk4C = &D_8008ECF0_8F8F0;
        }
    }
}
