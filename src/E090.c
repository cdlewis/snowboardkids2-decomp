#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern u32 D_67AB10;
extern u32 D_67BEB0;
extern u32 D_67BEB0_copy;
extern u32 D_67DB80;
extern u32 D_67DB80_copy;
extern u32 D_67E860;

extern void *freeNodeMemory(void *);
extern void func_800394BC_3A0BC(void *, s32);
extern void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 pad6;
} E090_DataEntry;

extern E090_DataEntry D_8008CE10_8DA10[];

void func_8000DB4C_E74C(void);

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 pad15[3];
} E090_InnerStruct;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    u8 pad14[0x18];
} E090_SubStruct;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    u8 pad4[0x4];
    Node_70B00 unk8;
    void *unk1E0;
    void *unk1E4;
    void *unk1E8;
    E090_SubStruct unk1EC;
    E090_SubStruct unk218;
    E090_InnerStruct unk244[6];
    s32 unk2D4[6];
} E090_struct;

void func_8000D518_E118(E090_struct *arg0);
void func_8000D5F0_E1F0(E090_struct *arg0);
void func_8000D624_E224(E090_struct *arg0);
void func_8000D68C_E28C(E090_struct *arg0);
void func_8000D6C0_E2C0(E090_struct *arg0);
void func_8000D6F8_E2F8(E090_struct *arg0);
void func_8000D750_E350(E090_struct *arg0);
void func_8000D7A0_E3A0(E090_struct *arg0);

void func_8000D490_E090(E090_struct *arg0) {
    func_8006FE28_70A28(&arg0->unk8, 0, 0, 0);
    func_8006FDA0_709A0(&arg0->unk8, 0xFF, 0xF);
}

void func_8000D4D4_E0D4(E090_struct *arg0) {
    func_8006FE28_70A28(&arg0->unk8, 0, 0, 0);
    func_8006FDA0_709A0(&arg0->unk8, 0, 0xF);
}

void func_8000D518_E118(E090_struct *arg0) {
    s32 i;

    func_800394BC_3A0BC(&arg0->unk1EC, (s32)arg0->unk1E0);
    func_800394BC_3A0BC(&arg0->unk218, (s32)arg0->unk1E4);

    arg0->unk1EC.unk10 = 0;
    arg0->unk1EC.unk12 = 0;
    arg0->unk218.unk10 = 0;
    arg0->unk218.unk12 = 0;

    for (i = 0; i < 6; i++) {
        arg0->unk244[i].unk0 = D_8008CE10_8DA10[i].unk0;
        arg0->unk244[i].unk2 = D_8008CE10_8DA10[i].unk2;
        arg0->unk244[i].unk4 = (s32)arg0->unk1E8;
        arg0->unk244[i].unk8 = D_8008CE10_8DA10[i].unk4;
        arg0->unk244[i].unkA = 0x400;
        arg0->unk244[i].unkC = 0x400;
        arg0->unk244[i].unkE = 0;
        arg0->unk244[i].unk10 = 0xFF;
        arg0->unk244[i].unk12 = 0;
        arg0->unk244[i].unk13 = 0;
        arg0->unk244[i].unk14 = 0;
        arg0->unk2D4[i] = 0;
    }

    arg0->unk0 = 1;
}

void func_8000D5F0_E1F0(E090_struct *arg0) {
    func_8000D4D4_E0D4(arg0);
    arg0->unk2 = 100;
    arg0->unk0 = 2;
}

void func_8000D624_E224(E090_struct *arg0) {
    arg0->unk2--;
    if (arg0->unk2 == 15) {
        func_8000D490_E090(arg0);
    } else if (arg0->unk2 == 0) {
        func_8000D4D4_E0D4(arg0);
        arg0->unk0 = 3;
    }
}

void func_8000D68C_E28C(E090_struct *arg0) {
    func_8000D4D4_E0D4(arg0);
    arg0->unk2 = 100;
    arg0->unk0 = 4;
}

void func_8000D6C0_E2C0(E090_struct *arg0) {
    arg0->unk2--;
    if (arg0->unk2 == 0x50) {
        arg0->unk0 = 5;
        arg0->unk1++;
    }
}

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D6F8_E2F8);

void func_8000D750_E350(E090_struct *arg0) {
    arg0->unk2--;
    if (arg0->unk2 == 15) {
        func_8000D490_E090(arg0);
    } else if (arg0->unk2 == 0) {
        arg0->unk0 = 7;
    }
}

void func_8000D7A0_E3A0(E090_struct *arg0) {
    arg0->unk1E0 = freeNodeMemory(arg0->unk1E0);
    arg0->unk1E4 = freeNodeMemory(arg0->unk1E4);
    arg0->unk1E8 = freeNodeMemory(arg0->unk1E8);
    unlinkNode(&arg0->unk8);
    terminateSchedulerWithCallback(func_8000DB4C_E74C);
}

void func_8000D818_E418(void);
void func_8000D974_E574(void);

void func_8000D7F8_E3F8(void) {
    setGameStateHandler(func_8000D818_E418);
}

void func_8000D818_E418(void) {
    E090_struct *temp_s0;
    Node_70B00 *s1;
    u8 sp20[0x20];

    temp_s0 = (E090_struct *)allocateTaskMemory(0x2F0);
    temp_s0->unk0 = 0;
    temp_s0->unk2 = 0;
    temp_s0->unk1 = 0;
    temp_s0->unk1E0 = dmaRequestAndUpdateStateWithSize(&D_67AB10, &D_67BEB0, 0x6350);
    temp_s0->unk1E4 = dmaRequestAndUpdateStateWithSize(&D_67BEB0_copy, &D_67DB80, 0x4320);
    temp_s0->unk1E8 = dmaRequestAndUpdateStateWithSize(&D_67DB80_copy, &D_67E860, 0x1A68);

    s1 = &temp_s0->unk8;
    func_8006FAA4_706A4(s1, 0, 0, 10, 0);
    func_8006F9BC_705BC(s1, 1.0f, 1.0f);
    setModelCameraTransform(s1, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(s1, 1);
    func_8006BEDC_6CADC(&sp20, 0, 0, 0x01000000, 0, 0, 0);
    func_8006FD3C_7093C(temp_s0->unk8.id, &sp20);
    func_8006FE28_70A28(s1, 0, 0, 0);
    func_8006FDA0_709A0(s1, 0xFF, 0);
    setGameStateHandler(func_8000D974_E574);
}

extern void func_80038420_39020(void);
extern void func_80011924_12524(void);

void func_8000D974_E574(void) {
    E090_struct *arg0 = getCurrentAllocation();
    s8 state;
    s32 i;

    state = arg0->unk0;
    switch (state) {
        case 0:
            func_8000D518_E118(arg0);
            break;
        case 1:
            func_8000D5F0_E1F0(arg0);
            break;
        case 2:
            func_8000D624_E224(arg0);
            break;
        case 3:
            func_8000D68C_E28C(arg0);
            break;
        case 4:
            func_8000D6C0_E2C0(arg0);
            break;
        case 5:
            func_8000D6F8_E2F8(arg0);
            break;
        case 6:
            func_8000D750_E350(arg0);
            break;
        case 7:
        default:
            func_8000D7A0_E3A0(arg0);
            return;
    }

    state = arg0->unk0;
    if (state == 2) {
        debugEnqueueCallback(0, 4, func_80038420_39020, &arg0->unk1EC);
    } else if (state >= 2) {
        if (state < 7) {
            if (state >= 4) {
                debugEnqueueCallback(0, 4, func_80038420_39020, &arg0->unk218);

                for (i = 0; i < arg0->unk1; i++) {
                    arg0->unk2D4[i] += 0x330000;
                    if (arg0->unk2D4[i] > 0xFFFFFF) {
                        arg0->unk2D4[i] = 0xFF0000;
                    }
                    arg0->unk244[i].unk14 = (s8)(arg0->unk2D4[i] >> 16);
                    debugEnqueueCallback(0, 3, func_80011924_12524, &arg0->unk244[i]);
                }
            }
        }
    }
}

void func_8000DB4C_E74C(void) {
    func_800697F4_6A3F4(1);
}
