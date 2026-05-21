#include "ui/save_data.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"

void processRaceUnlocks(s16 raceResult) {
    u8 saveSlot;

    saveSlot = gGameSessionContext->saveSlotIndex;
    if (saveSlot == 0xB) {
        EepromSaveData->setting_4E = 1;
        tryAddUnlockedBoardId(0xF);
    }

    saveSlot = gGameSessionContext->saveSlotIndex;
    if (saveSlot == 0xE) {
        EepromSaveData->setting_4F = 1;
        tryAddUnlockedBoardId(0xD);
    }

    saveSlot = gGameSessionContext->saveSlotIndex;
    if (saveSlot == 0xD) {
        if (raceResult == 7) {
            EepromSaveData->setting_50 = 1;
            tryAddUnlockedBoardId(0xE);
        }
    }

    if (gGameSessionContext->isStoryMode != 0) {
        if (gGameSessionContext->saveSlotIndex == 1) {
            if (tryAddUnlockedCutsceneId(0xA) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 2;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 3) {
            if (tryAddUnlockedCutsceneId(0xB) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 3;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 9) {
            if (tryAddUnlockedCutsceneId(0xC) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 4;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 5) {
            if (tryAddUnlockedCutsceneId(0xE) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 6;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 8) {
            if (tryAddUnlockedCutsceneId(0xF) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 7;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 0xB) {
            if (tryAddUnlockedCutsceneId(0x10) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 8;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 0) {
            if (tryAddUnlockedCutsceneId(0xD) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 5;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 4) {
            if (tryAddUnlockedCutsceneId(9) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 1;
            }
        }

        if (gGameSessionContext->saveSlotIndex == 7) {
            if (tryAddUnlockedCutsceneId(0x11) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 9;
            }
        }
    }
}

s32 areCharacterPalettesUnlocked(s32 count) {
    s32 result = 1;
    s32 i = 0;
    u8 *base;
    s32 pad[2];

    if (count > 0) {
        base = (u8 *)EepromSaveData;
        do {
            if ((base + i)[0x30] == 0) {
                result = 0;
                goto done;
            }
        } while (++i < count);
    }
done:
    return result;
}

s32 tryAddUnlockedCutsceneId(u8 cutsceneId) {
    s32 result = 1;
    s32 i = 0;
    u8 targetId = cutsceneId;

    do {
        u8 currentId = EepromSaveData->u.setting_42[i];
        if (currentId == 0) {
            EepromSaveData->u.setting_42[i] = cutsceneId;
            break;
        }
        if (currentId == targetId) {
            result = 0;
            break;
        }
        i++;
    } while (i < 9);

    return result;
}

void tryAddUnlockedBoardId(u8 boardId) {
    s32 i;
    u8 targetId = boardId;

    for (i = 0; i < 3; i++) {
        u8 currentId = EepromSaveData->setting_4B[i];
        if (currentId == 0) {
            EepromSaveData->setting_4B[i] = boardId;
            return;
        }
        if (currentId == targetId) {
            return;
        }
    }
}
