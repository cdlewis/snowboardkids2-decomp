#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "assets.h"
#include "common.h"
#include "core/session_manager.h"
#include "graphics/graphics.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/controller_io.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"

typedef struct {
    u8 pad[0xD0];
    u8 slotIndex;
} SaveSlotStatSpritesReturnType;

typedef struct {
    u8 pad[0x1C];
    u8 slotIndex;
} SaveSlotNameTextReturnType;

typedef struct {
    u8 pad[0xF0];
    u8 slotIndex;
} SaveSlotItemLabelsReturnType;

typedef struct {
    u8 pad[0x48];
    u8 particleIndex;
} SaveSlotParticlesReturnType;

typedef struct {
    ViewportNode unk0;
    ViewportNode slotModels[4];
    EepromSaveData_type slotData[4];
    void *unkAA8;
    void *unkAAC;
    void *unkAB0;
    void *unkAB4;
    void *unkAB8;
    s16 slideOffset;
    s16 unkABE;
    s16 nameEntryCursorY;
    u16 nameEntryCursorBaseY;
    u16 selectionAnimState;
    u16 saveSlotMenuState;
    u8 selectedSaveSlot;
    u8 hasCurrentSaveData;
    u8 eepromOperationStatus;
    u8 mainPromptIndex;
    u8 numValidSlots;
    u8 eepromErrorStatus;
    u8 slotHasData[3];
    u8 originalSlotDataFlag;
    u8 writeSlotIndexPlusOne;
    u8 menuAnimOffsetX;
    u8 menuChoiceIndex;
    u8 saveSlotDialogType;
    u8 saveSlotDialogSelection;
    u8 savedSlotIndex;
    u8 animDelayCounter;
} SaveSlotScreenState;

u8 eeprom_save_magic[16] = "SNOW2EEP";

extern void initSaveSlotStatSprites(void *);
extern void initSaveSlotItemIcons(void *);
extern void initSaveSlotItemLabels(void *);
extern void initSaveSlotNameText(void *);
extern void initSaveSlotPromptText(void *);
extern void initSaveSlotSelectionParticles(void *);
extern void initSaveSlotNameEntryGrid(void *);
extern void initSaveSlotDeleteArrow(void *);
extern void initSaveSlotGoldDisplay(void *);
extern void initSaveSlotConfirmationIndicator(void *);
extern void initSaveSlotDeleteText(void *);

extern void eepromWriteAsync(s32 slotIndex);
extern s32 pollEepromWriteAsync(void);

void updateSaveSlotSelectionScreen(void);
void initSaveSlotSelection(void);
void onSaveSlotSelectionConfirm(void);
void onSaveSlotSelectionCancel(void);
void cleanupSaveSlotSelectionAndExit(void);
void updateNameEntryCursorWiggle(void);
void onSaveSlotReadComplete(u16, s32);
void verifySaveSlotChecksum(void);
void updateSlotSelectionSlide(void);
s32 sanitizeSaveSlotData(EepromSaveData_type *saveData);

void initSaveSlotScreen(void) {
    SaveSlotScreenState *state;
    s32 i;
    s32 xOffset;
    s32 xPos;
    SaveSlotStatSpritesReturnType *statTask;
    SaveSlotNameTextReturnType *nameTask;
    void (*handler)(void);
    s32 probeResult;

    state = (SaveSlotScreenState *)allocateTaskMemory(0xAE0);
    setupTaskSchedulerNodes(0x14, 0x14, 0x14, 0, 0, 0, 0, 0);
    clearMemory((s8 *)state, 0xAE0);

    state->saveSlotMenuState = 0x28;
    state->selectedSaveSlot = 3;
    state->slideOffset = -0x140;
    state->unkABE = -0x58;
    state->selectionAnimState = 0;
    state->numValidSlots = 0;
    state->nameEntryCursorY = -0xE0;
    state->writeSlotIndexPlusOne = 0;
    state->menuChoiceIndex = 0;
    state->menuAnimOffsetX = 0;
    state->saveSlotDialogType = 0;
    state->saveSlotDialogSelection = 0;

    state->unkAA8 = loadCompressedData(&uiFontSpriteSheet_ROM_START, &font_main_ROM_START, 0x2278);
    state->unkAB4 = loadTextRenderAsset(1);
    state->unkAB8 = loadCompressedData(&goldIconSprite_ROM_START, &digit_sprite_ROM_START, 0x388);
    state->unkAAC = loadCompressedData(&okPromptSprites_ROM_START, &characterSelectBoardTexture_ROM_START, 0x1B48);
    state->unkAB0 = loadCompressedData(&snowflakeSprite_ROM_START, &tiledSnowmanAsset_ROM_START, 0x9488);

    initMenuCameraNode((ViewportNode *)state, 8, 0xA, 1);

    i = 0;
    xOffset = (s32)0xFFD00000;
    do {
        initViewportNode(&state->slotModels[i], NULL, (u16)(i + 9), 5, 0);
        setViewportId(&state->slotModels[i], (u16)(i + 0xA));

        if (i & 1) {
            xPos = -state->slideOffset;
        } else {
            xPos = (u16)state->slideOffset;
        }

        setModelCameraTransform(&state->slotModels[i], xPos, (s16)(xOffset >> 16), -0xA0, -0x18, 0xA0, 0x18);
        setViewportFadeValue(&state->slotModels[i], 0, 0);
        setViewportFadeValue(NULL, 0, 0);

        xOffset += 0x380000;
        state->slotHasData[i] = 0;
        i++;
    } while (i < 3);

    i = 0;
    do {
        eepromProbeAsync();
        do {
            probeResult = (s32)pollEepromProbeAsync();
        } while (probeResult == -1);

        if (probeResult == 1) {
            state->eepromErrorStatus = 0;
            goto eeprom_done;
        }
        if (probeResult == 2) {
            state->eepromErrorStatus = 0x63;
            goto eeprom_done;
        }
        i++;
        if (probeResult == 0) {
            state->eepromErrorStatus = 0x63;
            goto eeprom_done;
        }
        state->eepromErrorStatus = state->eepromErrorStatus + 1;
    } while (i < 3);

eeprom_done:
    for (i = 0; i < 3; i++) {
        statTask = (SaveSlotStatSpritesReturnType *)scheduleTask(initSaveSlotStatSprites, 0, 0, 0x5A);
        statTask->slotIndex = i;
        if (state->eepromErrorStatus != 0) {
            nameTask = (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotNameText, 0, 0, 0x5B);
            nameTask->slotIndex = i;
        }
    }

    if (state->eepromErrorStatus != 0) {
        state->saveSlotMenuState = 0;
    } else {
        scheduleTask(initSaveSlotDeleteArrow, 0, 0, 0x5B);
    }

    i = 0;
    state->hasCurrentSaveData = 0;
    {
        u8 *saveDataPtr = (u8 *)EepromSaveData;

        do {
            if (*saveDataPtr != 0) {
                state->hasCurrentSaveData = 1;
                goto after_loop;
            }
            i++;
            saveDataPtr++;
        } while (i < 8);
    }
after_loop:

    scheduleTask(initSaveSlotGoldDisplay, 0, 0, 0x5B);
    scheduleTask(initSaveSlotConfirmationIndicator, 0, 0, 0x5B);
    scheduleTask(initSaveSlotDeleteText, 0, 0, 0x5B);

    if (state->hasCurrentSaveData == 1) {
        statTask = (SaveSlotStatSpritesReturnType *)scheduleTask(initSaveSlotStatSprites, 0, 0, 0x5A);
        statTask->slotIndex = 3;
        state->originalSlotDataFlag = 0;
        initViewportNode(&state->slotModels[3], NULL, 0xC, 6, 0);
        setViewportId(&state->slotModels[3], 0xD);
        setModelCameraTransform(&state->slotModels[3], -0x140, -0x30, -0xA0, -0x18, 0xA0, 0x18);
        EepromSaveData->slotGold = gGameSessionContext->gold;
        handler = initSaveSlotSelection;
    } else {
        handler = updateSaveSlotSelectionScreen;
    }

    setGameStateHandler(handler);
    getFreeNodeCount(0);
}

void initSaveSlotSelection(void) {
    SaveSlotScreenState *allocation = (SaveSlotScreenState *)getCurrentAllocation();
    u8 saveSlot = gGameSessionContext->previousSaveSlot;
    allocation->selectedSaveSlot = saveSlot;
    setGameStateHandler(updateSaveSlotSelectionScreen);
}

void updateSaveSlotSelectionScreen(void) {
    SaveSlotScreenState *state;
    u8 slotIdx;
    s32 i;
    SaveSlotNameTextReturnType *nameTask;
    SaveSlotItemLabelsReturnType *labelsTask;
    SaveSlotParticlesReturnType *particleTask;
    void *taskResult;

    state = (SaveSlotScreenState *)getCurrentAllocation();

    switch (state->saveSlotMenuState) {
        case 0x28: {
            u16 counter = state->selectionAnimState;
            eepromReadAsync(counter & 0xFF, (u8 *)&state->slotData[counter & 0xFFFF]);
        }
            state->saveSlotMenuState = 0x29;
            break;
        case 0x29:
            taskResult = pollEepromReadAsync();
            if (taskResult == (void *)-1) {
                break;
            }
            onSaveSlotReadComplete(state->selectionAnimState, (s32)taskResult);
            if (state->eepromErrorStatus == 0) {
                u16 counter;
                counter = state->selectionAnimState;
                labelsTask = NULL;
                if (state->slotHasData[counter] != 0) {
                    nameTask =
                        (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotItemIcons, 2, counter & 0xFF, 0x5B);
                    labelsTask = (SaveSlotItemLabelsReturnType *)
                        scheduleTask(initSaveSlotItemLabels, 2, (u8)state->selectionAnimState, 0x5B);
                } else {
                    nameTask =
                        (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotNameText, 2, counter & 0xFF, 0x5B);
                }
                if (nameTask != NULL) {
                    nameTask->slotIndex = (u8)state->selectionAnimState;
                }
                if (labelsTask != NULL) {
                    labelsTask->slotIndex = (u8)state->selectionAnimState;
                }
            }
            state->selectionAnimState += 1;
            if ((state->selectionAnimState & 0xFFFF) == 3) {
                state->selectionAnimState = 0;
                state->saveSlotMenuState = 0;
            } else {
                state->saveSlotMenuState = 0x28;
            }
            break;
        case 0:
            state->slideOffset += 0x20;
            i = 0;
            if ((state->slideOffset << 16) == 0) {
                if ((state->eepromErrorStatus >= 0x62) && (state->hasCurrentSaveData == 0)) {
                    state->saveSlotMenuState = 0x32;
                    if (state->eepromErrorStatus == 0x62) {
                        state->saveSlotDialogType = 9;
                    } else {
                        state->saveSlotDialogType = 8;
                    }
                } else {
                    state->saveSlotMenuState = 0x32;
                    if (state->hasCurrentSaveData == 0) {
                        state->mainPromptIndex = 0;
                        state->saveSlotDialogType = 0;
                        if (state->numValidSlots != 0) {
                            state->saveSlotDialogSelection = 1;
                        } else {
                            goto set_ad6_zero;
                        }
                    } else {
                        state->mainPromptIndex = 9;
                        if (gGameSessionContext->creditsCompleted != 0) {
                            state->saveSlotDialogType = 2;
                        } else {
                            state->saveSlotDialogType = 1;
                        }
                    set_ad6_zero:
                        state->saveSlotDialogSelection = 0;
                    }
                }
                scheduleTask(initSaveSlotPromptText, 0, 0, 0x59);
                i = 0;
            }
            {
#ifdef CC_CHECK
                s32 constNeg18 = -0x18;
                s32 constA0 = 0xA0;
                s32 const18 = 0x18;
                s32 yPos = (s32)0xFFD00000;
                s32 slotOffset = 0x1D8;
#else
                register s32 constNeg18 __asm__("$22") = -0x18;
                register s32 constA0 __asm__("$21") = 0xA0;
                register s32 const18 __asm__("$20") = 0x18;
                register s32 yPos __asm__("$19") = (s32)0xFFD00000;
                register s32 slotOffset __asm__("$18") = 0x1D8;
#endif
                do {
                    s32 xPos;
                    s32 inc;
                    if (i & 1) {
                        xPos = -state->slideOffset;
                    } else {
                        xPos = (u16)state->slideOffset;
                    }
                    setModelCameraTransform(
                        (ViewportNode *)((u8 *)state + slotOffset),
                        xPos,
                        (s16)(yPos >> 16),
                        -0xA0,
                        constNeg18,
                        constA0,
                        const18
                    );
                    __asm__("lui %0, 0x38" : "=r"(inc) : "r"(yPos));
                    yPos += inc;
                    i += 1;
                    slotOffset += 0x1D8;
                } while (i < 3);
            }
            break;
        case 1:
            slotIdx = state->selectedSaveSlot;
            if (gControllerInputs[0] & (STICK_UP | CONT_UP)) {
                if (slotIdx != 0) {
                    state->selectedSaveSlot = slotIdx - 1;
                }
            } else if (gControllerInputs[0] & (STICK_DOWN | CONT_DOWN)) {
                if (slotIdx < 2) {
                    state->selectedSaveSlot = slotIdx + 1;
                }
            } else if (gControllerInputs[0] & CONT_A) {
                playSoundEffect(0x2C);
                if (state->hasCurrentSaveData != 0) {
                    goto case1_confirm;
                }
                if (state->slotHasData[state->selectedSaveSlot] != 1) {
                    state->saveSlotDialogSelection = 0x63;
                    state->mainPromptIndex = 2;
                    state->saveSlotMenuState = 0x3C;
                } else {
                case1_confirm:
                    state->saveSlotMenuState = 2;
                    state->selectionAnimState = 0;
                }
            } else if (gControllerInputs[0] & CONT_B) {
                playSoundEffect(0x2E);
                if (state->hasCurrentSaveData == 0) {
                    state->saveSlotMenuState = 0x32;
                    state->saveSlotDialogType = 0;
                    state->saveSlotDialogSelection = 1;
                    state->mainPromptIndex = 0;
                } else {
                    state->mainPromptIndex = 9;
                    if (gGameSessionContext->creditsCompleted != 0) {
                        state->saveSlotDialogType = 2;
                    } else {
                        state->saveSlotDialogType = 1;
                    }
                    state->saveSlotMenuState = 0x32;
                }
            }
            if (slotIdx != state->selectedSaveSlot) {
                goto play_cursor_sound;
            }
            break;
        case 2:
            state->selectionAnimState += 1;
            if ((state->selectionAnimState & 0xFFFF) == 0x11) {
                state->selectionAnimState = 0;
                if (state->slotData[state->selectedSaveSlot].save_slot_status[0xB] == 1) {
                    if (state->hasCurrentSaveData != 0) {
                        state->saveSlotMenuState = 3;
                        break;
                    }
                    if (gGameSessionContext->gameMode == 0) {
                        state->saveSlotMenuState = 0x32;
                        state->saveSlotDialogType = 0xA;
                        state->mainPromptIndex = 0xB;
                        state->saveSlotDialogSelection = 0;
                        break;
                    }
                }
                if (state->hasCurrentSaveData == 0) {
                    state->saveSlotDialogType = 0;
                }
                state->saveSlotMenuState = 3;
            }
            break;
        case 3:
            if (gControllerInputs[0] & CONT_A) {
                playSoundEffect(0x2C);
                if (state->hasCurrentSaveData == 0) {
                    gGameSessionContext->previousSaveSlot = state->selectedSaveSlot;
                    state->mainPromptIndex = 1;
                    state->saveSlotMenuState = 5;
                    memcpy(EepromSaveData, (void *)(state->selectedSaveSlot * 0x5C + (s32)(u8 *)state + 0x938), 0x5C);
                    gGameSessionContext->gold = state->slotData[state->selectedSaveSlot].slotGold;
                } else {
                    gGameSessionContext->previousSaveSlot = state->selectedSaveSlot;
                    state->saveSlotMenuState = 0x18;
                    state->animDelayCounter = 3;
                    state->originalSlotDataFlag = state->slotHasData[state->selectedSaveSlot];
                    memcpy(
                        (u8 *)state + 0xA4C,
                        (void *)(state->selectedSaveSlot * 0x5C + (s32)(u8 *)state + 0x938),
                        0x5C
                    );
                    memcpy((void *)(state->selectedSaveSlot * 0x5C + (s32)(u8 *)state + 0x938), EepromSaveData, 0x5C);
                    state->writeSlotIndexPlusOne = state->selectedSaveSlot + 1;
                    if (state->slotHasData[state->selectedSaveSlot] == 0) {
                        state->slotHasData[state->selectedSaveSlot] = 1;
                        state->numValidSlots = state->numValidSlots + 1;
                    } else {
                        state->originalSlotDataFlag = 1;
                    }
                    labelsTask = NULL;
                    eepromWriteAsync(state->selectedSaveSlot);
                    state->eepromErrorStatus = 0;
                    state->eepromOperationStatus = 0;
                    if (state->originalSlotDataFlag != 0) {
                        nameTask = (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotItemIcons, 1, 0, 0x5A);
                        labelsTask = (SaveSlotItemLabelsReturnType *)scheduleTask(initSaveSlotItemLabels, 1, 0, 0x5A);
                    } else {
                        nameTask = (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotNameText, 1, 0, 0x5A);
                    }
                    if (nameTask != NULL) {
                        nameTask->slotIndex = 3;
                    }
                    if (labelsTask != NULL) {
                        labelsTask->slotIndex = 3;
                    }
                    i = 0;
                    do {
                        getFreeNodeCount(0);
                        particleTask =
                            (SaveSlotParticlesReturnType *)scheduleTask(initSaveSlotSelectionParticles, 1, 0, 0x59);
                        if (particleTask != NULL) {
                            particleTask->particleIndex = i;
                        }
                        i += 1;
                    } while (i < 2);
                    {
                        ViewportNode *slotModel = &state->slotModels[3];
                        state->slideOffset = 0x78;
                        setModelCameraTransform(
                            slotModel,
                            0,
                            (s16)(state->selectedSaveSlot * 0x38 - 0x30),
                            -0x78,
                            -0x18,
                            (s16)(s32)state->slideOffset,
                            0x18
                        );
                        disableViewportOverlay(slotModel);
                    }
                }
            } else if (gControllerInputs[0] & CONT_B) {
                playSoundEffect(0x2E);
                if (state->saveSlotDialogType == 0xA) {
                    gGameSessionContext->isStoryMode = 0;
                    state->saveSlotMenuState = 0x32;
                    state->mainPromptIndex = 0xB;
                } else {
                    state->saveSlotMenuState = 1;
                }
            }
            break;
        case 5:
            scheduleTask(initSaveSlotNameEntryGrid, 0, 0, 0x5A);
            state->saveSlotMenuState = 6;
            break;
        case 6: {
            u16 counter;
            counter = state->nameEntryCursorY + 0x1E;
            state->nameEntryCursorY = counter;
            if ((s16)counter >= -0x28) {
                state->nameEntryCursorY = -0x28;
                state->nameEntryCursorBaseY = state->nameEntryCursorY;
                state->saveSlotMenuState = 7;
                state->selectionAnimState = 0;
            }
        } break;
        case 7:
            state->selectionAnimState += 1;
            if ((state->selectionAnimState & 0xFFFF) == 5) {
                state->selectionAnimState = 0;
                state->saveSlotMenuState = 8;
            }
            break;
        case 8:
            updateNameEntryCursorWiggle();
            if (gControllerInputs[0] & CONT_A) {
                playSoundEffect(0x2C);
                state->saveSlotMenuState = 9;
                state->selectionAnimState = 0;
                state->nameEntryCursorY = state->nameEntryCursorBaseY;
            }
            break;
        case 9:
        case 0xA:
            state->selectionAnimState += 1;
            if ((state->selectionAnimState & 0xFFFF) == 0x10) {
                state->selectionAnimState = 0;
                if (state->saveSlotMenuState == 0xA) {
                    state->selectionAnimState = 1;
                    state->saveSlotMenuState = 0xC;
                } else {
                    state->saveSlotMenuState = 0xB;
                }
            }
            break;
        case 0xB:
            updateNameEntryCursorWiggle();
            if (gControllerInputs[0] & CONT_A) {
                state->selectionAnimState = 0;
                state->nameEntryCursorY = state->nameEntryCursorBaseY;
                playSoundEffect(0x2D);
                state->saveSlotMenuState = 0xA;
            }
            break;
        case 0x3C:
            if (gControllerInputs[0] & (CONT_A | CONT_START)) {
                playSoundEffect(0x2C);
                state->saveSlotMenuState = 5;
                state->selectedSaveSlot = 4;
            } else if (gControllerInputs[0] & CONT_B) {
                playSoundEffect(0x2E);
                if (state->saveSlotDialogSelection != 0x63) {
                    state->saveSlotMenuState = 0x32;
                    state->mainPromptIndex = 0;
                    state->selectedSaveSlot = state->savedSlotIndex;
                } else {
                    state->saveSlotMenuState = 1;
                    state->mainPromptIndex = 0xA;
                }
            }
            break;
        case 0x14:
            eepromWriteAsync(state->selectedSaveSlot);
            updateSlotSelectionSlide();
            state->saveSlotMenuState = 0x15;
            break;
        case 0x18:
            state->animDelayCounter -= 1;
            if (!(state->animDelayCounter & 0xFF)) {
                playSoundEffect(0xB);
                state->saveSlotMenuState = 0x15;
                terminateTasksByTypeAndID(2, (s32)state->selectedSaveSlot);
                nameTask =
                    (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotItemIcons, 2, state->selectedSaveSlot, 0x5B);
                labelsTask = (SaveSlotItemLabelsReturnType *)
                    scheduleTask(initSaveSlotItemLabels, 2, state->selectedSaveSlot, 0x5B);
                if (nameTask != NULL) {
                    nameTask->slotIndex = state->selectedSaveSlot;
                }
                if (labelsTask != NULL) {
                    labelsTask->slotIndex = state->selectedSaveSlot;
                }
                setViewportOverlayRgbAndEnable(&state->slotModels[3], 0, 0, 0);
            }
            break;
        case 0x15:
            if ((state->eepromOperationStatus == 0) && (pollEepromWriteAsync() != -1)) {
                verifySaveSlotChecksum();
                if (state->selectionAnimState != 0) {
                    state->eepromErrorStatus += 1;
                    if ((u32)(state->eepromErrorStatus & 0xFF) < 3) {
                        state->saveSlotMenuState = 0x14;
                    } else {
                        state->eepromErrorStatus = 0;
                        state->eepromOperationStatus = 2;
                    }
                } else {
                    state->eepromOperationStatus = 1;
                }
            }
            updateSlotSelectionSlide();
            if (state->slideOffset == 0) {
                if (state->eepromOperationStatus != 0) {
                    state->saveSlotMenuState = 0x32;
                    if (state->eepromOperationStatus == 1) {
                        state->mainPromptIndex = 8;
                    } else {
                        state->mainPromptIndex = 0xE;
                        state->slotHasData[state->selectedSaveSlot] = state->originalSlotDataFlag;
                        memcpy(
                            (void *)(state->selectedSaveSlot * 0x5C + (s32)(u8 *)state + 0x938),
                            (u8 *)state + 0xA4C,
                            0x5C
                        );
                        terminateTasksByTypeAndID(2, (s32)state->selectedSaveSlot);
                        slotIdx = state->selectedSaveSlot;
                        labelsTask = NULL;
                        if (state->slotHasData[slotIdx] != 0) {
                            nameTask =
                                (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotItemIcons, 2, slotIdx, 0x5B);
                            labelsTask = (SaveSlotItemLabelsReturnType *)
                                scheduleTask(initSaveSlotItemLabels, 2, state->selectedSaveSlot, 0x5B);
                        } else {
                            nameTask =
                                (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotNameText, 2, slotIdx, 0x5B);
                        }
                        if (nameTask != NULL) {
                            nameTask->slotIndex = state->selectedSaveSlot;
                        }
                        if (labelsTask != NULL) {
                            labelsTask->slotIndex = state->selectedSaveSlot;
                        }
                    }
                    terminateTasksByType(1);
                }
            }
            break;
        case 0x16:
            if (gControllerInputs[0] & CONT_A) {
                playSoundEffect(0x2C);
                state->saveSlotMenuState = 0x17;
                state->mainPromptIndex = 7;
            }
            break;
        case 0x17:
            if (gControllerInputs[0] & CONT_A) {
                playSoundEffect(0x2D);
                state->saveSlotMenuState = 0xC;
                state->selectionAnimState = 0;
                state->selectedSaveSlot = 0x63;
            } else if (gControllerInputs[0] & CONT_B) {
                {
                    playSoundEffect(0x2E);
                    state->mainPromptIndex = 9;
                    state->saveSlotMenuState = 0x32;
                    if (gGameSessionContext->creditsCompleted != 0) {
                        state->saveSlotDialogType = 2;
                    } else {
                        state->saveSlotDialogType = 1;
                    }
                }
                state->selectedSaveSlot = state->savedSlotIndex;
            }
            break;
        case 0x32: {
            s32 maxChoice;
            maxChoice = 6;
            if (state->saveSlotDialogType == 0xA) {
                maxChoice = 4;
                state->menuAnimOffsetX += 3;
            } else {
                state->menuAnimOffsetX += 2;
            }
            state->menuChoiceIndex += 1;
            if ((state->menuChoiceIndex & 0xFF) == maxChoice) {
                if (state->eepromErrorStatus == 0) {
                    state->saveSlotMenuState = 0x33;
                } else if (state->eepromErrorStatus == 0x63) {
                    state->saveSlotMenuState = 0x36;
                } else {
                    state->saveSlotMenuState = 0x35;
                }
            }
        } break;
        case 0x33: {
            s32 maxChoice;
            slotIdx = state->saveSlotDialogSelection;
            maxChoice = 2;
            if ((state->saveSlotDialogType == 0xA) | (state->saveSlotDialogType == 2)) {
                maxChoice = 1;
            }
            if (gControllerInputs[0] & (STICK_UP | CONT_UP)) {
                if (slotIdx != 0) {
                    state->saveSlotDialogSelection = slotIdx - 1;
                }
            } else if ((gControllerInputs[0] & (STICK_DOWN | CONT_DOWN)) && (slotIdx != maxChoice)) {
                state->saveSlotDialogSelection = slotIdx + 1;
            }
            if (slotIdx != state->saveSlotDialogSelection) {
            play_cursor_sound:
                playSoundEffect(0x2B);
                break;
            }
            if (gControllerInputs[0] & (CONT_A | CONT_START)) {
                playSoundEffect(0x2C);
                state->saveSlotMenuState = 0x34;
                state->selectionAnimState = 0;
            } else if (gControllerInputs[0] & CONT_B) {
                playSoundEffect(0x2E);
                if (state->saveSlotDialogType == 0xA) {
                    state->saveSlotMenuState = 1;
                    state->mainPromptIndex = 0xA;
                    state->menuChoiceIndex = 0;
                    state->menuAnimOffsetX = 0;
                } else if (gGameSessionContext->creditsCompleted != 0) {
                    {
                        u8 oldSlot = state->selectedSaveSlot;
                        state->mainPromptIndex = 7;
                        state->saveSlotMenuState = 0x17;
                        state->selectedSaveSlot = 0x63;
                        state->menuChoiceIndex = 0;
                        state->menuAnimOffsetX = 0;
                        state->savedSlotIndex = oldSlot;
                    }
                } else {
                    state->saveSlotMenuState = 0xC;
                    state->selectedSaveSlot = 0x63;
                    if (state->hasCurrentSaveData == 0) {
                        state->selectionAnimState = 0;
                    } else {
                        state->selectionAnimState = 1;
                    }
                }
            }
        } break;
        case 0x34:
            state->selectionAnimState += 1;
            if ((state->selectionAnimState & 0xFFFF) == 0x11) {
                state->selectionAnimState = 0;
                state->saveSlotMenuState = 0x37;
            }
            break;
        case 0x37:
            if (state->saveSlotDialogType == 0xA) {
                state->menuAnimOffsetX -= 3;
            } else {
                state->menuAnimOffsetX -= 2;
            }
            state->menuChoiceIndex -= 1;
            if (!(state->menuChoiceIndex & 0xFF)) {
                if (state->eepromErrorStatus == 0x62) {
                    state->saveSlotMenuState = 5;
                    state->mainPromptIndex = 2;
                    state->selectedSaveSlot = 4;
                } else {
                    state->saveSlotMenuState = 0x38;
                }
            }
            break;
        case 0x38: {
            u8 oldSlot;
            if (state->hasCurrentSaveData == 0) {
                if (state->saveSlotDialogType == 0xA) {
                    state->saveSlotMenuState = 3;
                    if (state->saveSlotDialogSelection == 0) {
                        gGameSessionContext->isStoryMode = 1;
                        state->mainPromptIndex = 0xC;
                    } else {
                        gGameSessionContext->isStoryMode = 0;
                        state->mainPromptIndex = 0xD;
                    }
                } else {
                    if (state->saveSlotDialogSelection == 0) {
                        oldSlot = state->selectedSaveSlot;
                        state->saveSlotMenuState = 0x3C;
                        state->mainPromptIndex = 2;
                        goto set_slot_63_save_old;
                    }
                    if (state->saveSlotDialogSelection == 1) {
                        if (state->numValidSlots == 0) {
                            state->saveSlotMenuState = 0x3C;
                            state->mainPromptIndex = 2;
                        } else {
                            state->saveSlotMenuState = 1;
                            state->mainPromptIndex = 0xA;
                        }
                    } else {
                        state->saveSlotMenuState = 0xC;
                        state->selectionAnimState = 0;
                        state->selectedSaveSlot = 0x63;
                    }
                }
            } else {
                if (state->saveSlotDialogType == 2) {
                    if (state->saveSlotDialogSelection == 0) {
                        goto set_state1_acb6;
                    }
                    goto set_acb7_17;
                }
                if (state->saveSlotDialogSelection == 0) {
                    if (state->eepromErrorStatus == 0) {
                    set_state1_acb6:
                        state->saveSlotMenuState = 1;
                        state->mainPromptIndex = 6;
                    } else {
                        state->saveSlotMenuState = 0x32;
                        state->saveSlotDialogType = 6;
                    }
                } else if (state->saveSlotDialogSelection == 1) {
                    playSoundEffect(0x2D);
                    state->saveSlotMenuState = 0xC;
                    state->selectionAnimState = 1;
                    state->selectedSaveSlot = 0x63;
                } else {
                set_acb7_17:
                    oldSlot = state->selectedSaveSlot;
                    state->mainPromptIndex = 7;
                    state->saveSlotMenuState = 0x17;
                set_slot_63_save_old:
                    state->selectedSaveSlot = 0x63;
                    state->savedSlotIndex = oldSlot;
                }
            }
        } break;
        case 0x35:
            if (gControllerInputs[0] & (CONT_A | CONT_START)) {
                playSoundEffect(0x2C);
                state->saveSlotMenuState = 0x37;
            }
            break;
        case 0x36:
            break;
    }

    if (state->saveSlotMenuState == 0xC) {
        setViewportFadeValue(NULL, 0xFF, 0x10);
        setGameStateHandler(cleanupSaveSlotSelectionAndExit);
    }
}

void cleanupSaveSlotSelectionAndExit(void) {
    SaveSlotScreenState *allocation;
    s32 i;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    unlinkNode(&allocation->unk0);

    allocation->unkAA8 = freeNodeMemory(allocation->unkAA8);
    allocation->unkAAC = freeNodeMemory(allocation->unkAAC);
    allocation->unkAB0 = freeNodeMemory(allocation->unkAB0);
    allocation->unkAB4 = freeNodeMemory(allocation->unkAB4);
    allocation->unkAB8 = freeNodeMemory(allocation->unkAB8);

    if (allocation->hasCurrentSaveData == 0) {
        for (i = 0; i < 3; i++) {
            unlinkNode(&allocation->slotModels[i]);
        }

        for (i = 0; i < 8; i++) {
            EepromSaveData->header_data[i] = eeprom_save_magic[i];
        }
    } else {
        for (i = 0; i < 4; i++) {
            unlinkNode(&allocation->slotModels[i]);
        }
    }

    if (allocation->selectionAnimState == 1) {
        terminateSchedulerWithCallback(onSaveSlotSelectionConfirm);
    } else {
        terminateSchedulerWithCallback(onSaveSlotSelectionCancel);
    }
}

void onSaveSlotSelectionConfirm(void) {
    returnToParentScheduler(1);
}

void onSaveSlotSelectionCancel(void) {
    returnToParentScheduler(0xFE);
}

void updateNameEntryCursorWiggle(void) {
    SaveSlotScreenState *allocation = (SaveSlotScreenState *)getCurrentAllocation();
    u16 counter = allocation->selectionAnimState;
    u16 temp;

    if (counter < 15) {
        if (counter & 1) {
            allocation->nameEntryCursorY = allocation->nameEntryCursorBaseY - 2;
        } else {
            allocation->nameEntryCursorY = allocation->nameEntryCursorBaseY + 2;
        }
        counter = allocation->selectionAnimState;
    }

    temp = ((counter + 1) & 0xFFFF) % 25U;
    allocation->selectionAnimState = temp;
    temp &= 0xFFFF;

    if (temp == 15) {
        allocation->nameEntryCursorY = allocation->nameEntryCursorBaseY;
    }
}

void onSaveSlotReadComplete(u16 arg0, s32 arg1) {
    SaveSlotScreenState *allocation;
    s32 i;
    s32 sum;
    s32 savedChecksum;
    u16 slotIndex;
    u8 temp;
    s32 funcResult;
    u8 *dataPtr;
    s32 limit;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();

    if (arg1 == 0) {
        i = 0;
        slotIndex = arg0 & 0xFFFF;

        while (i < 8) {
            if (allocation->slotData[slotIndex].header_data[i] != eeprom_save_magic[i]) {
                allocation->eepromErrorStatus++;
                break;
            }
            i++;
        }

        if (i == 8) {
            limit = 0x58;
            slotIndex = arg0 & 0xFFFF;
            dataPtr = allocation->slotData[slotIndex].header_data;
            savedChecksum = allocation->slotData[slotIndex].checksum;
            sum = 0;
            i = 0;
            allocation->slotData[slotIndex].checksum = 0;

            while (i < limit) {
                sum += *dataPtr;
                i++;
                dataPtr++;
            }

            slotIndex = arg0 & 0xFFFF;
            if (sum != savedChecksum) {
                allocation->eepromErrorStatus++;

            } else {

                funcResult = sanitizeSaveSlotData(&allocation->slotData[slotIndex]);
                allocation->eepromErrorStatus = 0;
                if (!(funcResult & 0xFF)) {
                    allocation->numValidSlots++;
                    allocation->slotHasData[slotIndex] = 1;

                    if (allocation->selectedSaveSlot >= 3) {
                        allocation->selectedSaveSlot = arg0;
                    }
                } else {
                    allocation->slotHasData[slotIndex] = 0;
                }
            }
        }
    } else {
        allocation->eepromErrorStatus++;
    }

    temp = allocation->eepromErrorStatus;
    if (temp != 0) {
        if (temp < 3) {
            allocation->selectionAnimState = allocation->selectionAnimState - 1;
            return;
        }
        allocation->eepromErrorStatus = 0;
        allocation->slotHasData[arg0 & 0xFFFF] = 0;
    }
}

s32 sanitizeSaveSlotData(EepromSaveData_type *saveData) {
    s32 coinValue;
    s32 wasModified;
    s32 i;
    u8 value;

    coinValue = saveData->slotGold;
    wasModified = 0;
    if (coinValue > 0x98967F) {
        saveData->slotGold = 0x98967F;
        wasModified = 1;
        i = 0;
    } else {
        i = 0;
        if (coinValue < 0) {
            saveData->slotGold = 0;
            wasModified = 1;
            i = 0;
        }
    }

    for (i = 0; i < 0x10; i++) {
        value = saveData->save_slot_status[i];
        if (value >= 6) {
            saveData->save_slot_status[i] = 0;
            wasModified = 1;
        }
        value = saveData->save_slot_data[i];
        if (value >= 6) {
            saveData->save_slot_data[i] = 0;
            wasModified = 1;
        }
    }

    if (saveData->save_slot_status[0] == 0) {
        saveData->save_slot_status[0] = 5;
        wasModified = 1;
    }

    for (i = 0; i < 3; i++) {
        value = saveData->save_slot_data[i];
        if (value == 0) {
            saveData->save_slot_data[i] = 5;
            wasModified = 1;
        }
        value = saveData->save_slot_data[i + 4];
        if (value == 0) {
            saveData->save_slot_data[i + 4] = 5;
            wasModified = 1;
        }
    }

    for (i = 0; i < 0x12; i++) {
        value = saveData->character_or_settings[i];
        if (value >= 0x1A) {
            saveData->character_or_settings[i] = 0;
            wasModified = 1;
        }
    }

    for (i = 0; i < 3; i++) {
        value = saveData->character_or_settings[i * 3];
        if (value == 0) {
            saveData->character_or_settings[i * 3] = i + 1;
            wasModified = 1;
        }
    }

    for (i = 0; i < 0x12; i++) {
        value = saveData->character_or_settings[i];
        if (value >= 0x1A) {
            saveData->character_or_settings[i] = 1;
            wasModified = 1;
        }
    }

    for (i = 0; i < 9; i++) {
        value = saveData->u.setting_42[i];
        if (value >= 0x12) {
            saveData->u.setting_42[i] = 0x11;
            wasModified = 1;
        }
    }

    for (i = 0; i < 3; i++) {
        value = saveData->setting_4B[i];
        if (value >= 0x10) {
            saveData->setting_4B[i] = 0xF;
            wasModified = 1;
        }
    }

    if (saveData->setting_4E >= 2) {
        saveData->setting_4E = 0;
        wasModified = 1;
    }

    if (saveData->setting_4F >= 2) {
        saveData->setting_4F = 0;
        wasModified = 1;
    }

    if (saveData->setting_50 >= 2) {
        saveData->setting_50 = 0;
        wasModified = 1;
    }

    if (saveData->unk51 >= 2) {
        saveData->unk51 = 0;
        wasModified = 1;
    }

    return wasModified;
}

void verifySaveSlotChecksum(void) {
    u8 saveBuffer[0x60];
    SaveSlotScreenState *allocation;
    void *result;
    s32 retryCount;
    u32 computedChecksum;
    u32 expectedChecksum;
    u8 *dataPtr;
    s32 dataSize;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();
    allocation->selectionAnimState = 0;

    computedChecksum = 0;
    retryCount = 0;

    while (retryCount < 3) {
        eepromReadAsync(allocation->selectedSaveSlot, saveBuffer);

        do {
            result = pollEepromReadAsync();
        } while (result == (void *)-1);

        if (result == 0) {
            break;
        }

        retryCount++;
    }

    if (retryCount < 3) {
        dataSize = 0x58;
        expectedChecksum = *(u32 *)&saveBuffer[8];
        dataPtr = saveBuffer;
        retryCount = 0;
        *(u32 *)&saveBuffer[8] = 0;

        while (retryCount < dataSize) {
            retryCount++;
            computedChecksum += *dataPtr;
            dataPtr++;
        }

        if (computedChecksum == expectedChecksum) {
            allocation->selectionAnimState = 0;
            return;
        }
    }

    allocation->selectionAnimState = 1;
}

void updateSlotSelectionSlide(void) {
    SaveSlotScreenState *allocation;
    s16 slideOffset;
    u8 slotIndex;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();
    slideOffset = allocation->slideOffset;

    if (slideOffset != 0) {
        slotIndex = allocation->selectedSaveSlot;
        slideOffset -= 3;
        allocation->slideOffset = slideOffset;

        setModelCameraTransform(
            &allocation->slotModels[3],
            0,
            (s16)((slotIndex * 7 * 8) - 48),
            (s16)-slideOffset,
            -0x18,
            allocation->slideOffset,
            0x18
        );
    }
}
