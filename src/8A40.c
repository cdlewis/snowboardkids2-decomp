#include "common.h"

extern void *freeNodeMemory(void *);
extern void *loadAssetGroupDisplayList(void *);
extern void *loadAssetGroupVertexData(void *);
extern u8 identityMatrix[];
extern void *D_800882B0_88EB0;
extern void func_80007EC8_8AC8(void);
extern void setCleanupCallback(void *);
extern void setCallback(void *);

typedef struct {
    void *unk0;
    u8 unk4[0x20];
    void *unk24;
    void *unk28;
    void *unk2C;
    s32 unk30;
    u8 _pad34[0x10];
    u8 unk44;
    u8 _pad45[3];
    s32 unk48;
    s32 unk4C;
} func_800080B8_8CB8_arg;

void func_800080B8_8CB8(func_800080B8_8CB8_arg *);

void func_80007E40_8A40(func_800080B8_8CB8_arg *arg0) {
    memcpy(arg0->unk4, identityMatrix, 0x20);
    arg0->unk28 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unk2C = loadAssetGroupVertexData(arg0->unk0);
    arg0->unk24 = &D_800882B0_88EB0;
    arg0->unk30 = 0;
    arg0->unk44 = 0;
    arg0->unk48 = 0;
    arg0->unk4C = 0xA3D7;
    setCleanupCallback(&func_800080B8_8CB8);
    setCallback(&func_80007EC8_8AC8);
}

INCLUDE_ASM("asm/nonmatchings/8A40", func_80007EC8_8AC8);

void func_800080B8_8CB8(func_800080B8_8CB8_arg *arg0) {
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_800081B0_8DB0(void);
extern void func_8000811C_8D1C(void);

void func_800080F0_8CF0(void) {
    setCleanupCallback(func_800081B0_8DB0);
    setCallback(func_8000811C_8D1C);
}

INCLUDE_ASM("asm/nonmatchings/8A40", func_8000811C_8D1C);

void func_800081B0_8DB0(void) {
}

INCLUDE_RODATA("asm/nonmatchings/8A40", jtbl_8009DD88_9E988);

INCLUDE_RODATA("asm/nonmatchings/8A40", jtbl_8009DDA8_9E9A8);
