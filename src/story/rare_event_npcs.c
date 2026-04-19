#include "D_800AFE8C_A71FC_type.h"
#include "audio/audio.h"
#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"
#include "race/track_geometry.h"
#include "story/npc_dialogue.h"
#include "story/rare_events.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

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

extern u8 dialogueNpcFacesPlayer[];
void func_8002BFEC_2CBEC(Func2E024Arg *arg0);
extern void updateStoryMapNpcTurnToTarget(StoryMapDialogueState *);
extern void spawnSpriteEffectEx(s32, s32, s32, s32, void *, s32, s32, s32, s32, s32);

extern void initStoryMapRareEventIdle(void *);
extern void initStoryMapRareEventMagicShow(void *);
extern void initStoryMapRareEventJuggling(void *);
extern void initStoryMapRareEventSledding(void *);
extern void initStoryMapRareEventSnowman(void *);
extern void initStoryMapRareEventCheering(void *);
extern void initStoryMapRareEventSkating(void *);

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ u8 pad4[0x3C];
    /* 0x40 */ s32 spriteEffectPosX;
    /* 0x44 */ s32 spriteEffectPosY;
    /* 0x48 */ u8 pad48[0x4];
    /* 0x4C */ void *spriteEffectParams;
    /* 0x50 */ u8 pad50[0xC];
    /* 0x5C */ u8 characterIndex;
    /* 0x5D */ u8 pad5D[0x7];
} StoryMapRareEventNpc; // size 0x64

typedef struct {
    /* 0x00 */ StoryMapRareEventNpc npcs[2];
    /* 0xC8 */ u8 padC8[0xC];
    /* 0xD4 */ u8 eventTypeIndex;
    /* 0xD5 */ u8 npcCount;
} StoryMapRareEventState;

u8 rareEventNpcTable[] = {
    0x02, 0x00, 0x02, 0x02, 0x02, 0x05, 0x02, 0x01, 0x04, 0x02, 0x03, 0x00, 0x02, 0x03, 0x05, 0x02,
    0x06, 0x01, 0x02, 0x04, 0x00, 0x02, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 rareEventModelModes[] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
};

u8 D_8008EB18_8F718[] = {
    0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
};

u16 D_8008EB28_8F728[][12] = {
    0x0000, 0x0000, 0x0062, 0x0091, 0x0000, 0x0000, 0x0062, 0x0091, 0x0062, 0x0091, 0x0062, 0x0091, 0x008A, 0x00D0,
    0x008A, 0x00D1, 0x0000, 0x0000, 0x0000, 0x00D0, 0x008A, 0x00D0, 0x0000, 0x0000, 0x0079, 0x0000, 0x0000, 0x0000,
    0x0079, 0x0000, 0x0079, 0x0000, 0x0000, 0x0000, 0x0079, 0x0000, 0x0000, 0x0000, 0x00A9, 0x005D, 0x00A9, 0x005D,
    0x0000, 0x0000, 0x00A9, 0x005D, 0x00A9, 0x005D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0112, 0x0074, 0x0000, 0x0000, 0x0112, 0x0074, 0x0112, 0x0074, 0x0112, 0x0074,
    0x0112, 0x0074, 0x0000, 0x0000, 0x00BB, 0x005C, 0x00BB, 0x005C, 0x00BB, 0x0060, 0x0000, 0x0000, 0x00BB, 0x005C,
};

u8 D_8008EBD0_8F7D0[] = {
    0x14, 0x14, 0x01, 0x1E, 0x01, 0x1E, 0x1B, 0x1B, 0x00, 0x00, 0x23, 0x01, 0x01, 0x01, 0x00, 0x00,
};

u8 rareEventEffectDurations[] = {
    0x01, 0x01, 0x28, 0x01, 0x10, 0x14, 0x14, 0x14, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
};

void *D_8008EBF0_8F7F0[] = {
    (void *)initStoryMapRareEventWave,
    (void *)initStoryMapRareEventIdle,
    (void *)initStoryMapRareEventMagicShow,
    (void *)initStoryMapRareEventJuggling,
    (void *)initStoryMapRareEventSledding,
    (void *)initStoryMapRareEventSnowman,
    (void *)initStoryMapRareEventCheering,
    (void *)initStoryMapRareEventSkating,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (void *)0x0000005C,
    (void *)0x005C0060,
    (void *)0x005B005C,
    (void *)0x005E0074,
    (void *)0x00000074,
    (void *)0x00740074,
    (void *)0x00740076,
    (void *)0x008A008A,
    (void *)0x00000090,
    (void *)0x008A008A,
    (void *)0x008D00AA,
    (void *)0x00A200AA,
    (void *)0x000000A2,
    (void *)0x00A200A5,
    (void *)0x00BB00BB,
    (void *)0x00BB00BB,
    (void *)0x000000BB,
    (void *)0x00BE00D0,
    (void *)0x00D100D1,
    (void *)0x00D100D0,
    (void *)0x000000D4,
    (void *)0x0000005C,
    (void *)0x005C0060,
    (void *)0x005B005C,
    (void *)0x00740000,
    (void *)0x00740074,
    (void *)0x00740074,
    (void *)0x008A008A,
    (void *)0x00000090,
    (void *)0x008A008A,
    (void *)0x00A700A2,
    (void *)0x00A70000,
    (void *)0x00A200A2,
    (void *)0x00BB00BB,
    (void *)0x00BB00BB,
    (void *)0x000000BB,
    (void *)0x00D000D1,
    (void *)0x00D100D1,
    (void *)0x00D00000,
    (void *)0x010B010B,
    (void *)0x010B010B,
    (void *)0x010B010B,
};

s16 D_8008ECD8_8F8D8[] = { 0x0D, 0x0E, 0x0F, -1 };
s16 D_8008ECE0_8F8E0[] = { 0x0A, 0x0B, 0x0C, -1 };
s16 D_8008ECE8_8F8E8[] = { 0x14, 0x15, 0x16, -1 };
s16 D_8008ECF0_8F8F0[] = { 0x16, 0x17, 0x18, -1 };
s16 D_8008ECF8_8F8F8[] = { 0x11, 0x12, 0x13, -1 };

void *D_8008ED00_8F900[] = {
    (void *)0x0000FFFF,
    (void *)0x001B001B,
    (void *)0xFFFF0000,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECF0_8F8F0,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE0_8F8E0,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE0_8F8E0,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE0_8F8E0,
    (void *)D_8008ECF8_8F8F8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECF8_8F8F8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE0_8F8E0,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE0_8F8E0,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECE0_8F8E0,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECF0_8F8F0,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE8_8F8E8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECE8_8F8E8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
};

void *D_8008EE5C_8FA5C[] = {
    NULL,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ED00_8F900,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ED00_8F900,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ED00_8F900,
    (void *)D_8008ED00_8F900,
    (void *)D_8008ED00_8F900,
    (void *)D_8008ED00_8F900,
    (void *)D_8008ECD8_8F8D8,
    (void *)D_8008ED00_8F900,
    (void *)0x0000005C,
    (void *)0x005E0000,
    (void *)0x0000005E,
    NULL,
    (void *)0x00790074,
    (void *)0x00000077,
    NULL,
    (void *)0x00740000,
    NULL,
    (void *)0x008D008E,
    (void *)0x0000008D,
    (void *)0x008A0000,
    (void *)0x00AB00A2,
    (void *)0x00A50000,
    (void *)0x000000A5,
    (void *)0x00A20000,
    (void *)0x00C000BB,
    (void *)0x000000BF,
    (void *)0x000000BE,
    NULL,
    (void *)0x00D60000,
    (void *)0x00D400D5,
    (void *)0x000000D4,
    (void *)0x00D00000,
    (void *)0x0051006D,
    NULL,
    NULL,
    (void *)0x000000F5,
    (void *)0x00E30000,
    NULL,
    (void *)0x00830099,
    (void *)0x000000C9,
    (void *)0x000000F4,
    (void *)0x00E10000,
    NULL,
    (void *)0x00830000,
    (void *)0x000000C9,
    (void *)0x000000F2,
    (void *)0x00E30000,
    (void *)0x00590072,
    (void *)0x008800A0,
    (void *)0x00B900CE,
    (void *)0x03030303,
    (void *)0x03010000,
    NULL,
    NULL,
    NULL,
};

void awaitStoryMapRareEventReady(StoryMapRareEventState *arg0);
void configureRareEventSpriteEffect(StoryMapRareEventState *rareEvent, s32 npcIndex);
void destroyStoryMapRareEventModels(StoryMapRareEventState *arg0);

void initStoryMapRareEvent(StoryMapRareEventState *arg0) {
    GameState *gameState;
    s32 i;
    u8 npcCount;
    u8 characterIndex;

    gameState = getCurrentAllocation();

    npcCount = rareEventNpcTable[arg0->eventTypeIndex * 3];
    arg0->npcCount = npcCount;
    gameState->unk41C = npcCount;

    gameState->unk421 = rareEventModelModes[arg0->eventTypeIndex];

    for (i = 0; i < arg0->npcCount; i++) {
        characterIndex = rareEventNpcTable[arg0->eventTypeIndex * 3 + i + 1];
        arg0->npcs[i].characterIndex = characterIndex;

        if (gameState->unk421 == 0) {
            if ((arg0->eventTypeIndex == 2) & (i == 1)) {
                arg0->npcs[i].model = createSceneModelEx(characterIndex + 0x50, gameState, 0, -1, 0, 5);
            } else {
                arg0->npcs[i].model = createSceneModel(characterIndex + 0x50, gameState);
            }
        } else {
            arg0->npcs[i].model = createSceneModelEx(characterIndex + 0x50, gameState, 0, 0, 0, -1);
        }
    }

    setCleanupCallback(destroyStoryMapRareEventModels);
    setCallback(awaitStoryMapRareEventReady);
}

void awaitStoryMapRareEventReady(StoryMapRareEventState *arg0) {
    GameState *gameState = (GameState *)getCurrentAllocation();

    if (gameState->unk429 == 0) {
        setCallback(D_8008EBF0_8F7F0[arg0->eventTypeIndex]);
    }
}

void nullStoryMapRareEventCallback(void) {
}

void destroyStoryMapRareEventModels(StoryMapRareEventState *arg0) {
    s32 i;

    for (i = 0; i < arg0->npcCount; i++) {
        arg0->npcs[i].model = destroySceneModel(arg0->npcs[i].model);
    }
}

void updateStoryMapNpcDialogue(StoryMapDialogueState *dialogue) {
    GameState *gameState;
    s32 i;
    StoryMapDialogueState *currentNpc;

    gameState = (GameState *)getCurrentAllocation();

    switch (dialogue->dialogueState) {
        case 0:
            func_8002BFEC_2CBEC((Func2E024Arg *)dialogue);
            for (i = 0; i < gameState->unk41C; i++) {
                currentNpc = (StoryMapDialogueState *)((u8 *)dialogue + i * 0x64);
                createYRotationMatrix(&currentNpc->matrix, currentNpc->targetRotation);
                updateStoryMapNpcModel((Func297D8Arg *)currentNpc);
            }
            break;
        case 1:
            updateStoryMapNpcTurnToTarget(dialogue);
            for (i = 0; i < gameState->unk41C; i++) {
                currentNpc = (StoryMapDialogueState *)((u8 *)dialogue + i * 0x64);
                createYRotationMatrix(&currentNpc->matrix, currentNpc->targetRotation);
                updateStoryMapNpcModel((Func297D8Arg *)currentNpc);
            }
            break;
    }

    if (gameState->dialogueTurnState == 0) {
        setCallback(dialogue->returnCallback);
    }
}

void func_8002BFEC_2CBEC(Func2E024Arg *arg0) {
    GameState *gs;
    s32 i;
    s32 completedCount;
    u8 localNibble;
    s16 angleDiff;
    s16 targetAngle;
    s32 turnSpeed;
    s32 absDiff;
    u16 nextVal;
    u16 soundId;
    u16 rotation;
    s32 new_var;

    gs = (GameState *)getCurrentAllocation();
    completedCount = 0;
    localNibble = gs->dialogueTurnState & 0xF;
    gs->dialogueTurnState &= 0xF0;
    for (i = 0; i < gs->unk41C; i++) {
        switch (arg0->ctrl[i]) {
            case 1:
                rotation = (arg0->elements[i].unk30 = arg0->elements[i].rotation);
                targetAngle = computeAngleToPosition(
                    gs->unk3EC,
                    gs->unk3F0,
                    arg0->elements[i].matrix.translation.x,
                    arg0->elements[i].matrix.translation.z
                );
                arg0->elements[i].unk32 = targetAngle;
                angleDiff = signedAngleDifference((s16)rotation, targetAngle);
                if (angleDiff < 0) {
                    arg0->elements[i].unk36 = 1;
                } else {
                    arg0->elements[i].unk36 = 0;
                }
                absDiff = angleDiff;
                if (angleDiff < 0) {
                    absDiff = -absDiff;
                }
                if (absDiff >= 0xAAB) {
                    arg0->elements[i].unk30 = (arg0->elements[i].unk30 + 0x1000) & 0x1FFF;
                    arg0->elements[i].unk50 = 1;
                    arg0->elements[i].unk37 = 0;
                    if (signedAngleDifference((s16)arg0->elements[i].unk30, arg0->elements[i].unk32) < 0) {
                        arg0->elements[i].unk36 = 1;
                    } else {
                        arg0->elements[i].unk36 = 0;
                    }
                } else {
                    arg0->elements[i].unk50 = 2;
                    arg0->elements[i].unk37 = 1;
                }
                arg0->ctrl[i] = 2;
                break;

            case 2:
                angleDiff = signedAngleDifference((s16)arg0->elements[i].unk30, arg0->elements[i].unk32);
                absDiff = (angleDiff > 0) ? (angleDiff) : (-angleDiff);
                new_var = absDiff;
                if (new_var < 0xA0) {
                    if (arg0->elements[i].unk50 == 2) {
                        arg0->elements[i].unk50 = 0;
                    }
                    angleDiff = new_var;
                    arg0->ctrl[i] = 3;
                } else {
                    angleDiff = 0xA0;
                }
                if (arg0->elements[i].unk36 != 0) {
                    s32 tmp = angleDiff;
                    angleDiff = -tmp;
                }
                arg0->elements[i].unk30 += angleDiff;
                if ((arg0->elements[i].unk50 == 1) && (arg0->elements[i].unk37 != 0)) {
                    arg0->elements[i].unk50 = 0;
                }
                break;

            case 3:
                completedCount += 1;
                if (completedCount == gs->unk41C) {
                    localNibble = 3;
                    if (dialogueNpcFacesPlayer[arg0->unkD4 * 2] == 0) {
                        arg0->elements[0].unk37 = 1;
                    }
                    if (dialogueNpcFacesPlayer[(arg0->unkD4 * 2) + 1] == 0) {
                        arg0->elements[1].unk37 = 1;
                    }
                    if (gs->unk42B == 0) {
                        arg0->ctrl[0] = 4;
                        arg0->ctrl[1] = 0x5A;
                    } else {
                        arg0->ctrl[1] = 4;
                        arg0->ctrl[0] = 0x5A;
                    }
                } else if ((arg0->elements[i].unk50 == 1) && (arg0->elements[i].unk37 != 0)) {
                    arg0->elements[i].unk50 = 0;
                }
                break;

            case 4:
                if ((gs->dialogueTurnState == 0x30) && (arg0->elements[i].unk37 != 0)) {
                    if (D_8008EB18_8F718[(arg0->unkD4 * 2) + i] != 0) {
                        arg0->elements[i].unk4C = (u16 *)D_8008EE5C_8FA5C[(arg0->unkD4 * 2) + i];
                        configureRareEventSpriteEffect((StoryMapRareEventState *)arg0, i);
                        arg0->elements[i].unk50 = (s16)(*arg0->elements[i].unk4C);
                        arg0->elements[i].unk4C++;
                    } else {
                        arg0->unkCC[i] = 0;
                        configureRareEventSpriteEffect((StoryMapRareEventState *)arg0, i);
                    }
                    arg0->elements[i].unk37 = 0;
                    soundId = D_8008EB28_8F728[arg0->unkD4][(D_800AFE8C_A71FC->playerBoardIds[0] * 2) + i];
                    if (soundId != 0) {
                        playSoundEffectOnChannelNoPriority(soundId, i + 1);
                    }
                    if (arg0->elements[i].unk50 == 0) {
                        arg0->ctrl[i] = 0xA;
                        arg0->unkCC[i] = 0;
                    } else {
                        arg0->ctrl[i] = 5;
                    }
                }
                break;

            case 5:
                if (D_8008EB18_8F718[(arg0->unkD4 * 2) + i] != 0) {
                    if (arg0->elements[i].unk37 != 0) {
                        arg0->elements[i].unk37 = 0;
                        nextVal = *arg0->elements[i].unk4C;
                        if (nextVal == 0xFFFF) {
                            arg0->ctrl[i] = 6;
                            arg0->elements[i].unk50 = 0;
                        } else {
                            arg0->elements[i].unk50 = (s16)nextVal;
                            arg0->elements[i].unk4C++;
                        }
                    }
                } else {
                    arg0->unkCC[i]++;
                    if (arg0->unkCC[i] == D_8008EBD0_8F7D0[(arg0->unkD4 * 2) + i]) {
                        arg0->unkCC[i] = 0;
                        arg0->ctrl[i] = 6;
                    }
                }
                break;

            case 6:
                arg0->ctrl[i] = 7;
                if (i == gs->unk42B) {
                    ;
                    arg0->ctrl[(i + 1) & 1] = 4;
                }
                break;

            case 7:
                completedCount += 1;
                if (completedCount == gs->unk41C) {
                    localNibble = 5;
                    arg0->ctrl[0] = 8;
                    arg0->ctrl[1] = 8;
                }
                break;

            case 8:
                rotation = gs->dialogueTurnState;
                if (rotation == 0) {
                    arg0->unkD3 = 1;
                    if (dialogueNpcFacesPlayer[(arg0->unkD4 * 2) + i] != 0) {
                        arg0->elements[i].unk32 = 0;
                    } else {
                        arg0->elements[i].unk32 = 2;
                        arg0->elements[i].unk37 = 1;
                    }
                    arg0->ctrl[i] = 9;
                    localNibble = 0xF;
                }
                break;

            case 0xA:
                new_var = i;
                arg0->unkCC[new_var]++;
                if (arg0->unkCC[i] == rareEventEffectDurations[(arg0->unkD4 * 2) + i]) {
                    arg0->unkCC[i] = 0;
                    arg0->ctrl[i] = 6;
                    arg0->elements[i].unk50 = 0;
                }
                break;

            case 0x5A:
                break;
        }
    }
    gs->dialogueTurnState |= localNibble;
}

void updateStoryMapNpcTurnToTarget(StoryMapDialogueState *state) {
    Func297D8Arg *npcs = (Func297D8Arg *)state;
    GameState *gameState;
    s32 i;
    s32 completedCount;
    s16 angleDiff;
    s32 absAngleDiff;
    s16 turnSpeed;

    gameState = getCurrentAllocation();
    completedCount = 0;

    for (i = 0; i < gameState->unk41C; i++) {
        switch (npcs[i].unk32) {
            case 0:
                turnSpeed = signedAngleDifference(npcs[i].unk30, npcs[i].unk2E);
                angleDiff = turnSpeed;
                if (angleDiff < 0) {
                    npcs[i].unk36 = 1;
                } else {
                    npcs[i].unk36 = 0;
                }
                npcs[i].unk50 = 2;
                npcs[i].unk37 = 1;
                if (((turnSpeed > 0) ? (turnSpeed) : (-turnSpeed)) >= 0xAAB) {
                    npcs[i].unk30 = (npcs[i].unk30 + 0x1000) & 0x1FFF;
                    npcs[i].unk50 = 1;
                    npcs[i].unk37 = 0;
                    if (signedAngleDifference(npcs[i].unk30, npcs[i].unk2E) < 0) {
                        npcs[i].unk36 = 1;
                    } else {
                        npcs[i].unk36 = 0;
                    }
                }
                npcs[i].unk32 = 1;
                break;

            case 1:
                angleDiff = signedAngleDifference(npcs[i].unk30, npcs[i].unk2E);
                absAngleDiff = (angleDiff > 0) ? (angleDiff) : (-angleDiff);
                turnSpeed = 0xA0;
                if (absAngleDiff < 0xA0) {
                    if (npcs[i].unk50 == 2) {
                        npcs[i].unk50 = 0;
                    }
                    turnSpeed = absAngleDiff;
                    npcs[i].unk32 = 3;
                }
                if (npcs[i].unk36 != 0) {
                    turnSpeed = -(turnSpeed >> 0x1200);
                }
                npcs[i].unk30 = npcs[i].unk30 + turnSpeed;
                if (npcs[i].unk50 == 1) {
                    if (npcs[i].unk37 != 0) {
                        npcs[i].unk50 = 0;
                    }
                }
                break;

            case 2:
                completedCount++;
                break;

            case 3:
                if (npcs[i].unk37 != 0) {
                    npcs[i].unk50 = 0;
                    completedCount++;
                }
                break;
        }
    }

    if (((u8)completedCount) == gameState->unk41C) {
        gameState->dialogueTurnState = 0;
        for (i = 0; i < gameState->unk41C; i++) {
            npcs[i].rotation = npcs[i].unk2E;
            npcs[i].unk50 = npcs[i].unk56;
            setAnimationIndex(npcs[i].model, npcs[i].unk5F);
        }
    }
}

void configureRareEventSpriteEffect(StoryMapRareEventState *rareEvent, s32 npcIndex) {
    StoryMapRareEventNpc *npc;
    u8 eventType;
    s32 offset;

    eventType = rareEvent->eventTypeIndex;

    if ((eventType == 1) & (npcIndex == 0)) {
        if (D_800AFE8C_A71FC->playerBoardIds[0] == 3) {
            npc = &rareEvent->npcs[npcIndex];
            npc->spriteEffectParams = D_8008ED00_8F900;
            rareEvent->npcs[0].spriteEffectPosY = 0x300000;
            spawnSpriteEffectEx(
                (s32)npc->model,
                0,
                0x1F,
                rareEventEffectDurations[rareEvent->eventTypeIndex * 2 + npcIndex] - 4,
                &npc->spriteEffectPosX,
                0x10000,
                0,
                2,
                0,
                0
            );
            return;
        }
    }

    if ((rareEvent->eventTypeIndex == 6) & (npcIndex == 1)) {
        if (D_800AFE8C_A71FC->playerBoardIds[0] == 3) {
            offset = npcIndex * 0x64;
            ((StoryMapRareEventNpc *)((u8 *)rareEvent + offset))->spriteEffectParams = D_8008ECF0_8F8F0;
        }
    }
}
