#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"

s32 tryAddUnlockedCutsceneId(u8 cutsceneId);
void tryAddUnlockedBoardId(u8 boardId);

void processRaceUnlocks(s16 raceResult) {
    u8 saveSlot;

    saveSlot = D_800AFE8C_A71FC->saveSlotIndex;
    if (saveSlot == 0xB) {
        EepromSaveData->setting_4E = 1;
        tryAddUnlockedBoardId(0xF);
    }

    saveSlot = D_800AFE8C_A71FC->saveSlotIndex;
    if (saveSlot == 0xE) {
        EepromSaveData->setting_4F = 1;
        tryAddUnlockedBoardId(0xD);
    }

    saveSlot = D_800AFE8C_A71FC->saveSlotIndex;
    if (saveSlot == 0xD) {
        if (raceResult == 7) {
            EepromSaveData->setting_50 = 1;
            tryAddUnlockedBoardId(0xE);
        }
    }

    if (D_800AFE8C_A71FC->isStoryMode != 0) {
        if (D_800AFE8C_A71FC->saveSlotIndex == 1) {
            if (tryAddUnlockedCutsceneId(0xA) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 2;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 3) {
            if (tryAddUnlockedCutsceneId(0xB) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 3;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 9) {
            if (tryAddUnlockedCutsceneId(0xC) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 4;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 5) {
            if (tryAddUnlockedCutsceneId(0xE) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 6;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 8) {
            if (tryAddUnlockedCutsceneId(0xF) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 7;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 0xB) {
            if (tryAddUnlockedCutsceneId(0x10) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 8;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 0) {
            if (tryAddUnlockedCutsceneId(0xD) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 5;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 4) {
            if (tryAddUnlockedCutsceneId(9) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 1;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 7) {
            if (tryAddUnlockedCutsceneId(0x11) & 0xFF) {
                D_800AFE8C_A71FC->pendingUnlockCutscene = 9;
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

INCLUDE_ASM("asm/nonmatchings/38C90", func_80038420_39020);

INCLUDE_ASM("asm/nonmatchings/38C90", func_800394BC_3A0BC);
