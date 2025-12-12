#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern volatile s32 gControllerInputs;

typedef struct {
    Node_70B00 node;
    void *unk1D8;
    void *unk1DC;
    s16 unk1E0;
    union {
        u16 unk1E2;
        struct {
            u8 unk1E2_hi;
            u8 unk1E3;
        } bytes;
    } unk1E2_union;
    u8 unk1E4;
    u8 unk1E5;
    u8 unk1E6;
} Struct22D30;

extern u8 D_8008DCC0_8E8C0[];

extern void D_800B0690(void);
extern void D_800B07A0(void);
extern void func_800B00C0_9FF70(void *);

void func_800223CC_22FCC(void);
void func_800225C8_231C8(void);
void func_800226B0_232B0(void);
void func_800226CC_232CC(void);

INCLUDE_ASM("asm/nonmatchings/22D30", func_80022130_22D30);

void func_80022304_22F04(void) {
    Struct22D30 *s0;
    s32 result;
    u8 *task;
    u16 temp;

    s0 = (Struct22D30 *)getCurrentAllocation();
    result = func_8006FE10_70A10(&s0->node);

    if (result == 0) {
        s0->unk1E0 = 0;
        setGameStateHandler(&func_800223CC_22FCC);
        scheduleTask(&D_800B0690, 0, 0, 0x5A);
        scheduleTask(&D_800B07A0, 0, 0, 0x5A);
    } else {
        temp = s0->unk1E0;
        if (temp < 4) {
            if (result == (0xC - (temp * 2))) {
                task = (u8 *)scheduleTask(&func_800B00C0_9FF70, 0, 0, 0x5A);
                task[0x2B] = (u8)s0->unk1E0;
                s0->unk1E0 = s0->unk1E0 + 1;
            }
        }
    }
}

void func_800223CC_22FCC(void) {
    Struct22D30 *s0;
    s32 inputs;

    s0 = (Struct22D30 *)getCurrentAllocation();

    switch (s0->unk1E6) {
        case 0:
            inputs = gControllerInputs;
            if (inputs & B_BUTTON) {
                func_800585C8_591C8(0x2E);
                s0->unk1E6 = 0xA;
            } else {
                if (inputs & 0x40100) {
                    if (s0->unk1E2_union.unk1E2 < (s0->unk1E4 - 1)) {
                        s0->unk1E2_union.unk1E2 = s0->unk1E2_union.unk1E2 + 1;
                        func_80058220_58E20(0x2B, 0);
                        break;
                    }
                }
                if (gControllerInputs & 0x80200) {
                    if (s0->unk1E2_union.unk1E2 != 0) {
                        s0->unk1E2_union.unk1E2 = s0->unk1E2_union.unk1E2 - 1;
                        func_80058220_58E20(0x2B, 0);
                        break;
                    }
                }
                if (gControllerInputs & A_BUTTON) {
                    s0->unk1E6 = 1;
                    s0->unk1E0 = 0;
                    func_80058220_58E20(0x2C, 0);
                }
            }
            break;
        case 1:
            s0->unk1E0 = s0->unk1E0 + 1;
            if ((u16)s0->unk1E0 == 0x11) {
                s0->unk1E0 = 0;
                s0->unk1E6 = 2;
            }
            break;
        case 2:
            inputs = gControllerInputs;
            if (inputs & B_BUTTON) {
                goto common_exit;
            } else if (inputs & A_BUTTON) {
                s0->unk1E5 = 1;
                func_80058220_58E20(0x2D, 0);
            }
            break;
        case 0xA:
            inputs = gControllerInputs;
            if (inputs & B_BUTTON) {
            common_exit:
                func_800585C8_591C8(0x2E);
                s0->unk1E6 = 0;
            } else if (inputs & A_BUTTON) {
                s0->unk1E5 = 0x63;
            }
            break;
    }

    if (s0->unk1E5 != 0) {
        func_80057564_58164(0xA);
        if (s0->unk1E5 == 0x63) {
            func_8006FDA0_709A0((Node_70B00 *)s0, 0xFF, 8);
        } else {
            func_8006FDA0_709A0((Node_70B00 *)s0, 0xFF, 0x10);
        }
        setGameStateHandler(&func_800225C8_231C8);
    }
}

void func_800225C8_231C8(void) {
    Struct22D30 *s0;
    s32 i;

    s0 = (Struct22D30 *)getCurrentAllocation();
    if (func_8006FE10_70A10(&s0->node) == 0) {
        terminateAllTasks();
        unlinkNode(&s0->node);
        s0->unk1D8 = freeNodeMemory(s0->unk1D8);
        s0->unk1DC = freeNodeMemory(s0->unk1DC);
        if (s0->unk1E5 == 1) {
            terminateSchedulerWithCallback(func_800226B0_232B0);
            D_800AFE8C_A71FC->unk8 = s0->unk1E2_union.bytes.unk1E3 + 1;
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                D_800AFE8C_A71FC->unk9[i] = D_8008DCC0_8E8C0[i];
            }
        } else {
            terminateSchedulerWithCallback(func_800226CC_232CC);
        }
    }
}

void func_800226B0_232B0(void) {
    func_800697F4_6A3F4(1);
}

void func_800226CC_232CC(void) {
    func_800697F4_6A3F4(0xFF);
}
