#include "6E840.h"
#include "common.h"
#include "displaylist.h"

void func_80004920_5520();
void func_80005094_5C94();
void func_800634E8_640E8(s32);
void func_80063534_64134(s32);
void func_80063580_64180(s32);
void func_800638C0_644C0(s32);

INCLUDE_ASM("asm/nonmatchings/5520", func_80004920_5520);

void func_80004F74_5B74(s32 arg0) {
    func_80004920_5520();
    func_800634E8_640E8(arg0);
}

void func_80004FA0_5BA0(s32 arg0) {
    func_80004920_5520();
    func_80063534_64134(arg0);
}

void func_80004FCC_5BCC(s32 arg0) {
    func_80004920_5520();
    func_80063580_64180(arg0);
}

typedef struct {
    u8 padding[0x20];
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
    } *unk20;
    u8 padding2[0xC];
    s32 unk30;
} func_80004FF8_5BF8_arg1;
void func_80004FF8_5BF8(s32 arg0, func_80004FF8_5BF8_arg1 *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback((s16)arg0, 1, &func_80004F74_5B74, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback((s16)arg0, 3, &func_80004FA0_5BA0, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback((s16)arg0, 5, &func_80004FCC_5BCC, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/5520", func_80005094_5C94);

void func_800056E8_62E8(DisplayListObject *arg0) {
    func_80005094_5C94();
    buildDisplayListSegment(arg0);
}

void func_80005714_6314(s32 arg0) {
    func_80005094_5C94();
    func_800638C0_644C0(arg0);
}

INCLUDE_ASM("asm/nonmatchings/5520", func_80005740_6340);

INCLUDE_ASM("asm/nonmatchings/5520", func_8000576C_636C);

INCLUDE_ASM("asm/nonmatchings/5520", func_80005808_6408);

INCLUDE_ASM("asm/nonmatchings/5520", func_8000595C_655C);

INCLUDE_ASM("asm/nonmatchings/5520", func_80005D30_6930);

INCLUDE_RODATA("asm/nonmatchings/5520", jtbl_8009DD38_9E938);
