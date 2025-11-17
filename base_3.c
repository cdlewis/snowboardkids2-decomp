#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} cutsceneChrRotate_exec_arg;

typedef struct {
    union {
        s32 One;
        s8 Two;
    } unk0;
    u8 unk04[0x24];
    u8 padding[0x15];
    s8 unk3D;
    u8 padding2[0x50];
    s16 angle;
    u8 padding3[0x18];
} CutsceneSlotData;

typedef struct {
    u8 padding[0x48];
    void *model;
    CutsceneSlotData slotData;
} CutsceneSlot;

typedef struct {
    u8 padding[0xA8];
    CutsceneSlot slots[16];
} CutsceneManager;

void func_800B7620_1E46D0(CutsceneSlotData*, s16, s16, s16);
void func_800021E8_2DE8(void*, s16, s16, s32);

void cutsceneChrRotate_exec(cutsceneChrRotate_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;

    slot = &arg1->slots[arg2];

    func_800B7620_1E46D0(&slot->slotData, arg0->unk0, arg0->unk6, arg0->unk8);
    func_800021E8_2DE8(slot->model, arg0->unk2, arg0->unk4, -1);
}
