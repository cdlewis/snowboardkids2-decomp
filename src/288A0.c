#include "288A0.h"

extern s32 func_8006FE48_70A48(u16, s32, s32, s32, s32, s32);
extern void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);
extern void func_8006FD3C_7093C(u16, void *);

extern void *D_8008DEF0_8EAF0;
extern void *D_8008DEF8_8EAF8;

void func_80027CA0_288A0(Node_70B00 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s8 sp20[0x20];
    s32 temp_a3;

    temp_a3 = arg3 & 0xFF;
    if (temp_a3 == 0) {
        func_8006FAA4_706A4(arg0, 0, arg1 & 0xFF, arg2 & 0xFF, 1);
    } else {
        func_8006FAA4_706A4(arg0, 0, arg1 & 0xFF, arg2 & 0xFF, 0);
    }

    func_8006F9BC_705BC(arg0, 1.0f, 1.0f);
    func_8006FEF8_70AF8(arg0, ((arg1 & 0xFF) + 1));
    setModelCameraTransform(arg0, 0, 0, -0x98, -0x70, 0x97, 0x6F);
    func_8006FA0C_7060C(arg0, 50.0f, 1.3333334f, 10.0f, 4000.0f);
    func_8006FE48_70A48(arg0->id, 0x3D4, 0x3E6, 0x64, 0x64, 0x64);
    func_8006BEDC_6CADC(&sp20, 0, 0, 0x200000, 0, 0, 0);
    func_8006FD3C_7093C(arg0->id, &sp20);
    func_8006FE28_70A28(arg0, 0, 0, 0);
    func_8006FDA0_709A0(arg0, 0, 0);
    func_8006FC70_70870(arg0->id, 1, &D_8008DEF0_8EAF0, &D_8008DEF8_8EAF8);
}

INCLUDE_ASM("asm/nonmatchings/288A0", func_80027E04_28A04);

INCLUDE_ASM("asm/nonmatchings/288A0", func_80028074_28C74);

INCLUDE_ASM("asm/nonmatchings/288A0", func_80028480_29080);

INCLUDE_ASM("asm/nonmatchings/288A0", func_800285EC_291EC);
