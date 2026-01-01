#include "common.h"

typedef struct {
    s32 gold;
    u8 unk4;
    u8 unk5;
    char padding2[0x1];
    u8 saveSlotIndex;
    u8 numPlayers;
    u8 unk9[0x15];
    u8 previousSaveSlot;
    u8 unk1F;
    u8 unk20;
    u8 customLapEnabled;
    u8 customLapCount;
    u8 pendingUnlockCutscene;
    u8 isStoryMode;
    u8 creditsCompleted;
} D_800AFE8C_A71FC_type;

extern D_800AFE8C_A71FC_type *D_800AFE8C_A71FC;
