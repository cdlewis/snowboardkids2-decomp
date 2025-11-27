#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void *freeNodeMemory(void *);

void func_8000DB4C_E74C(void);

typedef struct {
    u8 unk0[0x18];
} E090_InnerStruct;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    u8 pad4[0x4];
    Node_70B00 unk8;
    void *unk1E0;
    void *unk1E4;
    void *unk1E8;
    u8 unk1EC[0x2C];
    u8 unk218[0x2C];
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

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D518_E118);

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

extern void func_8000D818_E418(void);

void func_8000D7F8_E3F8(void) {
    setGameStateHandler(func_8000D818_E418);
}

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D818_E418);

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
        debugEnqueueCallback(0, 4, func_80038420_39020, arg0->unk1EC);
    } else if (state >= 2) {
        if (state < 7) {
            if (state >= 4) {
                debugEnqueueCallback(0, 4, func_80038420_39020, arg0->unk218);

                for (i = 0; i < arg0->unk1; i++) {
                    arg0->unk2D4[i] += 0x330000;
                    if (arg0->unk2D4[i] > 0xFFFFFF) {
                        arg0->unk2D4[i] = 0xFF0000;
                    }
                    arg0->unk244[i].unk0[0x14] = (s8)(arg0->unk2D4[i] >> 16);
                    debugEnqueueCallback(0, 3, func_80011924_12524, &arg0->unk244[i]);
                }
            }
        }
    }
}

void func_8000DB4C_E74C(void) {
    func_800697F4_6A3F4(1);
}
