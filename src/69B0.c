#include "common.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
} Struct69B0;

extern void func_8000605C_6C5C(void *);
void func_800061C0_6DC0(void);

INCLUDE_ASM("asm/nonmatchings/69B0", func_80005DB0_69B0);

INCLUDE_ASM("asm/nonmatchings/69B0", func_80005E54_6A54);

INCLUDE_ASM("asm/nonmatchings/69B0", func_80005FBC_6BBC);

void func_80006010_6C10(Struct69B0 *arg0) {
    setCleanupCallback(func_800061C0_6DC0);
    arg0->unkA = 0;
    arg0->unk8 = 0;
    arg0->unk6 = 0;
    arg0->unk4 = 0;
    setCallback(func_8000605C_6C5C);
}

INCLUDE_ASM("asm/nonmatchings/69B0", func_8000605C_6C5C);

void func_800061C0_6DC0(void) {
}
