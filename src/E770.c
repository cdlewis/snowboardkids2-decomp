#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void *freeNodeMemory(void *);

typedef struct {
    s8 unk0;
    u8 pad1[0x7];
    void *unk8;
    void *unkC;
    u8 pad10[0x2];
    s16 unk12;
    u8 pad14[0x4];
    void *unk18;
    u8 pad1C[0x4];
    Node_70B00 unk20;
    Node_70B00 unk1F8;
    Node_70B00 unk3D0;
    u8 pad5A8[0x1C];
    void *unk5C4;
    u8 pad5C8[0x2C];
    void *unk5F4;
    u8 pad5F8[0x2C];
    SceneModel *unk624;
} E770_struct;

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DB70_E770);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DBA8_E7A8);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DBE0_E7E0);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DC18_E818);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DC44_E844);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DC88_E888);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DCD8_E8D8);

void func_8000E154_ED54(E770_struct *arg0) {
    func_80002014_2C14(arg0->unk624);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk5C4 = freeNodeMemory(arg0->unk5C4);
    arg0->unk5F4 = freeNodeMemory(arg0->unk5F4);
    unlinkNode(&arg0->unk3D0);
    unlinkNode(&arg0->unk20);
    unlinkNode(&arg0->unk1F8);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E1D0_EDD0);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E240_EE40);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E2AC_EEAC);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E4CC_F0CC);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E56C_F16C);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E5B0_F1B0);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E614_F214);

extern s16 D_8008CE40_8DA40;
extern void func_8000E6E0_F2E0(void);

void func_8000E680_F280(void) {
    allocateTaskMemory(0xCC0);
    D_8008CE40_8DA40 = 5;
    setupTaskSchedulerNodes(0x40, 4, 0, 0, 0, 0, 0, 0);
    func_800585C8_591C8(0xD0);
    setGameStateHandler(func_8000E6E0_F2E0);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E6E0_F2E0);

extern s16 func_80069810_6A410(void);
extern void func_8000E1D0_EDD0(E770_struct *);
extern void func_8000E240_EE40(E770_struct *);
extern void func_8000E2AC_EEAC(E770_struct *);
extern void func_8000E4CC_F0CC(E770_struct *);
extern void func_8000E56C_F16C(E770_struct *);
extern void func_8000E5B0_F1B0(E770_struct *);
extern s32 func_8000E614_F214(E770_struct *);
extern void func_8000DCD8_E8D8(E770_struct *);

void func_8000EC98_F898(void) {
    E770_struct *s0;

    s0 = getCurrentAllocation();
    switch (s0->unk0) {
        case 0:
            func_8000E1D0_EDD0(s0);
            break;
        case 1:
            func_8000E240_EE40(s0);
            break;
        case 2:
            func_8000E2AC_EEAC(s0);
            break;
        case 3:
            func_8000E4CC_F0CC(s0);
            break;
        case 4:
            func_8000E56C_F16C(s0);
            break;
        case 5:
        default:
            func_8000E5B0_F1B0(s0);
            break;
        case 6:
            if (func_8000E614_F214(s0)) {
                return;
            }
            break;
        case 7:
            if ((s16)func_80069810_6A410()) {
                setGameStateHandler(func_8000E6E0_F2E0);
            }
            return;
    }
    func_8000DCD8_E8D8(s0);
}

void func_8000ED88_F988(void) {
    D_8008CE40_8DA40 = 5;
    func_800697F4_6A3F4(1);
}

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB0_9EAB0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB4_9EAB4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB8_9EAB8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEBC_9EABC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC0_9EAC0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC4_9EAC4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC8_9EAC8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DECC_9EACC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED0_9EAD0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED4_9EAD4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED8_9EAD8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEDC_9EADC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE0_9EAE0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE4_9EAE4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE8_9EAE8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEEC_9EAEC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF0_9EAF0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF4_9EAF4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF8_9EAF8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEFC_9EAFC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF00_9EB00);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF04_9EB04);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF08_9EB08);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF0C_9EB0C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF10_9EB10);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF14_9EB14);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF18_9EB18);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF1C_9EB1C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF20_9EB20);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF24_9EB24);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF28_9EB28);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF2C_9EB2C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF30_9EB30);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF34_9EB34);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000EDB0_F9B0);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000EE88_FA88);

extern void func_8000DBE0_E7E0(void *);

void func_8000F4BC_100BC(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    arg0->unk0 = 1;
    func_8000DBE0_E7E0(alloc);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000F4F0_100F0);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000F564_10164);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000F690_10290);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000F7B0_103B0);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000F884_10484);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000FA90_10690);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF6C_9EB6C);

INCLUDE_ASM("asm/nonmatchings/E770", func_8000FBBC_107BC);

extern void func_8000DBA8_E7A8(void *);

void func_8000FD1C_1091C(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    func_8000DBA8_E7A8(alloc);
    arg0->unk0 = 4;
}

s32 func_8000FD50_10950(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
    s16 temp = alloc->unk12;

    if (temp == 0) {
        func_80069CF8_6A8F8();
        return 1;
    }
    alloc->unk12 = temp - 1;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000FD98_10998);

extern void func_8000F4F0_100F0(E770_struct *);
extern void func_8000FBBC_107BC(E770_struct *);
extern void func_8000EE88_FA88(E770_struct *);

void func_8000FE00_10A00(E770_struct *arg0) {
    getCurrentAllocation();
    switch (arg0->unk0) {
        case 0:
            func_8000F4BC_100BC(arg0);
            break;
        case 1:
            func_8000F4F0_100F0(arg0);
            break;
        case 2:
            func_8000FBBC_107BC(arg0);
            break;
        case 3:
            func_8000FD1C_1091C(arg0);
            break;
        case 4:
            if (func_8000FD50_10950(arg0)) {
                return;
            }
            break;
    }
    func_8000EE88_FA88(arg0);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000FEA0_10AA0);
