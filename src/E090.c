#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void *freeNodeMemory(void *);
extern void func_8000DB4C_E74C(void);

typedef struct {
    u8 pad[0x8];
    Node_70B00 unk8;
    void *unk1E0;
    void *unk1E4;
    void *unk1E8;
} E090_struct;

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D490_E090);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D4D4_E0D4);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D518_E118);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D5F0_E1F0);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D624_E224);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D68C_E28C);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D6C0_E2C0);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D6F8_E2F8);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D750_E350);

void func_8000D7A0_E3A0(E090_struct *arg0) {
    arg0->unk1E0 = freeNodeMemory(arg0->unk1E0);
    arg0->unk1E4 = freeNodeMemory(arg0->unk1E4);
    arg0->unk1E8 = freeNodeMemory(arg0->unk1E8);
    unlinkNode(&arg0->unk8);
    terminateSchedulerWithCallback(func_8000DB4C_E74C);
}

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D7F8_E3F8);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D818_E418);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000D974_E574);

INCLUDE_ASM("asm/nonmatchings/E090", func_8000DB4C_E74C);
