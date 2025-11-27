#include "common.h"

extern void *freeNodeMemory(void *);
extern void func_80069CF8_6A8F8(void);
extern void enqueueDisplayListObject(s32 arg0, void *arg1);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_8000BBA8_C7A8_arg;

typedef struct {
    u8 _pad0[0x18];
    s32 unk18;
    u8 _pad1[0x60];
    s32 unk7C;
} func_8000BB48_C748_arg;

INCLUDE_ASM("asm/nonmatchings/C570", func_8000B970_C570);

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BA08_C608);

void func_8000BB48_C748(func_8000BB48_C748_arg *arg0) {
    s32 pad[8];
    arg0->unk7C -= 0x8000;
    if (arg0->unk7C <= (s32)0xFFF80000) {
        func_80069CF8_6A8F8();
    }
    arg0->unk18 += arg0->unk7C;
    enqueueDisplayListObject(0, arg0);
}

void func_8000BBA8_C7A8(func_8000BBA8_C7A8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

extern void func_8000BC10_C810(void *, s16, u8, u8, s32);

void func_8000BBE0_C7E0(void *arg0, s16 arg1) {
    func_8000BC10_C810(arg0, arg1, 1, 0, 100);
}

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BC10_C810);
