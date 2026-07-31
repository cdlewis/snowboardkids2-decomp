#include "ui/save_data.h"
#include "gamestate.h"

void processRaceUnlocks(s16 raceResult) {
    u8 saveSlot;

    saveSlot = gGameSessionContext->currentLevel;
    if (saveSlot == 0xB) {
        EepromSaveData->specialBoardUnlocked[0] = 1;
        tryAddUnlockedBoardId(0xF);
    }

    saveSlot = gGameSessionContext->currentLevel;
    if (saveSlot == 0xE) {
        EepromSaveData->specialBoardUnlocked[1] = 1;
        tryAddUnlockedBoardId(0xD);
    }

    saveSlot = gGameSessionContext->currentLevel;
    if (saveSlot == 0xD) {
        if (raceResult == 7) {
            EepromSaveData->specialBoardUnlocked[2] = 1;
            tryAddUnlockedBoardId(0xE);
        }
    }

    if (gGameSessionContext->modeState.isStoryMode != 0) {
        if (gGameSessionContext->currentLevel == 1) {
            if (tryAddUnlockedCutsceneId(0xA) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 2;
            }
        }

        if (gGameSessionContext->currentLevel == 3) {
            if (tryAddUnlockedCutsceneId(0xB) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 3;
            }
        }

        if (gGameSessionContext->currentLevel == 9) {
            if (tryAddUnlockedCutsceneId(0xC) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 4;
            }
        }

        if (gGameSessionContext->currentLevel == 5) {
            if (tryAddUnlockedCutsceneId(0xE) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 6;
            }
        }

        if (gGameSessionContext->currentLevel == 8) {
            if (tryAddUnlockedCutsceneId(0xF) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 7;
            }
        }

        if (gGameSessionContext->currentLevel == 0xB) {
            if (tryAddUnlockedCutsceneId(0x10) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 8;
            }
        }

        if (gGameSessionContext->currentLevel == 0) {
            if (tryAddUnlockedCutsceneId(0xD) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 5;
            }
        }

        if (gGameSessionContext->currentLevel == 4) {
            if (tryAddUnlockedCutsceneId(9) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 1;
            }
        }

        if (gGameSessionContext->currentLevel == 7) {
            if (tryAddUnlockedCutsceneId(0x11) & 0xFF) {
                gGameSessionContext->pendingUnlockCutscene = 9;
            }
        }
    }
}

s32 areCharacterPalettesUnlocked(s32 count) {
    s32 result = 1;
    s32 i = 0;
    EepromSaveDataBlock *saveData;
    s32 pad[2];

    if (count > 0) {
        saveData = EepromSaveData;
        do {
            if (saveData->characterPaletteIds[i] == 0) {
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
        u8 currentId = EepromSaveData->unlockedCutsceneIds[i];
        if (currentId == 0) {
            EepromSaveData->unlockedCutsceneIds[i] = cutsceneId;
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
        u8 currentId = EepromSaveData->unlockedBoardIds[i];
        if (currentId == 0) {
            EepromSaveData->unlockedBoardIds[i] = boardId;
            return;
        }
        if (currentId == targetId) {
            return;
        }
    }
}
