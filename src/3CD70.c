#include "common.h"
#include "task_scheduler.h"

void func_8003C170_3CD70(void);

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003C170_3CD70);

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003C2BC_3CEBC);

void func_8003CEC4_3DAC4(u8 arg0) {
    Node *node = scheduleTask(func_8003C170_3CD70, 1, 0, 0xD2);
    if (node != NULL) {
        node->unkC = arg0;
        node->field_D = arg0;
    }
}

void func_8003CF40_3DB40(void);

typedef struct {
    s16 unk0;
} func_8003CF0C_arg;

void func_8003CF0C_3DB0C(func_8003CF0C_arg *arg0) {
    getCurrentAllocation();
    arg0->unk0 = 0;
    setCallbackWithContinue(func_8003CF40_3DB40);
}

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003CF40_3DB40);

void func_8003D0C8_3DCC8(void) {
    scheduleTask(func_8003CF0C_3DB0C, 1, 0, 0xF0);
}

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003D0F4_3DCF4);

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003D210_3DE10);

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003D51C_3E11C);
