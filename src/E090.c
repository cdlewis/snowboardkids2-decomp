#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void *freeNodeMemory(void *);

void func_8000DB4C_E74C(void);

typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    u8 pad4[0x4];
    Node_70B00 unk8;
    void *unk1E0;
    void *unk1E4;
    void *unk1E8;
} E090_struct;

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

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D6C0_E2C0);

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

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D974_E574);

void func_8000DB4C_E74C(void) {
    func_800697F4_6A3F4(1);
}
