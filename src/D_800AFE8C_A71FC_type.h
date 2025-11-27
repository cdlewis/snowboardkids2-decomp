#include "common.h"

typedef struct {
    s32 padding;
    u8 unk4;
    u8 unk5;
    char padding2[0x1];
    u8 saveSlotIndex;
    u8 unk8;
    u8 unk9;
    u8 padding3[0x19];
    u8 errorFlag;
    u8 unk24;
    u8 unk25;
} D_800AFE8C_A71FC_type;

extern D_800AFE8C_A71FC_type *D_800AFE8C_A71FC;
