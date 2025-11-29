#include "56910.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_40E1C0);

extern void *func_80035F80_36B80(s32);
extern void func_800BB320_B5C00(void *);
extern void func_8006D7B0_6E3B0(s32, s16, s16, s16, s16, u8, u8, u8, u8, u8);

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    s16 unkC;
} func_800BB2B0_B5B90_state;

typedef struct {
    void *unk0;
    u8 pad4[0xC];
    s16 unk10;
    s16 unk12;
    u16 unk14;
    u8 unk16;
    u8 unk17;
} func_800BB5CC_state;

void func_800BB9E0_B62C0(func_800BB2B0_B5B90_state *arg0);

void func_800BB2B0_B5B90(func_800BB2B0_B5B90_state *arg0) {
    getCurrentAllocation();
    arg0->unk4 = func_80035F80_36B80(1);
    arg0->unk0 = loadAsset_34F7E0();
    arg0->unk8 = dmaRequestAndUpdateStateWithSize(&_40E1C0_ROM_START, &_40E1C0_ROM_END, 0x1130);
    arg0->unkC = 0;
    setCleanupCallback(func_800BB9E0_B62C0);
    setCallback(func_800BB320_B5C00);
}

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB320_B5C00);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB51C_B5DFC);

void func_800BB670_B5F50(func_800BB5CC_state *);

void func_800BB5CC_B5EAC(func_800BB5CC_state *arg0) {
    s16 temp_a7;
    s16 temp_a6;

    arg0->unk12--;
    arg0->unk14 += 0x10;

    if (arg0->unk12 == 1) {
        setCallback(func_800BB670_B5F50);
    }

    temp_a7 = arg0->unk10;
    temp_a6 = arg0->unk12;

    func_8006D7B0_6E3B0(
        (s32)arg0->unk0,
        ((-temp_a7) << 19) >> 16,
        ((-temp_a6) << 19) >> 16,
        temp_a7,
        temp_a6,
        0,
        (u8)arg0->unk14,
        arg0->unk17,
        0xC,
        0x6
    );
}

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB670_B5F50);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB724_B6004);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB87C_B615C);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB928_B6208);

void func_800BB9E0_B62C0(func_800BB2B0_B5B90_state *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}
