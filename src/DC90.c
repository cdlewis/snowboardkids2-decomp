#include "56910.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void *func_80035F80_36B80(s32);
extern void *func_8000B6B8_C2B8(s32);

extern void func_8000D2C8_DEC8(void);

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u8 pad12[0x16];
    /* 0x28 */ void *unk28;
    /* 0x2C */ u8 pad2C[0x8];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
} DC90TaskStruct;

void func_8000D448_E048(DC90TaskStruct *arg0);

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D090_DC90);

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D0BC_DCBC);

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D100_DD00);

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D144_DD44);

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D1BC_DDBC);

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D200_DE00);

void func_8000D244_DE44(DC90TaskStruct *arg0) {
    arg0->unkC = 0;
    arg0->unk10 = 0;
    arg0->unk4 = loadAsset_34F7E0();
    arg0->unk28 = func_80035F80_36B80(1);
    arg0->unk8 = func_8000B6B8_C2B8(2);
    arg0->unk34 = 1;
    arg0->unk38 = 1;
    arg0->unk3C = 0xFF;
    arg0->unk3E = 0xC0;
    func_800585C8_591C8(0x2C);
    setCleanupCallback(&func_8000D448_E048);
    setCallback(&func_8000D2C8_DEC8);
}

INCLUDE_ASM("asm/nonmatchings/DC90", func_8000D2C8_DEC8);

void func_8000D448_E048(DC90TaskStruct *arg0) {
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}
