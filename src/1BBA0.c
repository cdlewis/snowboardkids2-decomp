#include "3A1F0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "gamestate.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(_1DA660)

typedef struct {
    char padding[0x10];
    u8 unk10;
} D_800A8D14_A0084_type;

extern D_800A8D14_A0084_type *D_800A8D14_A0084;
extern void func_8001B020_1BC20(void);
extern void func_8001B3E8_1BFE8(void);
extern void func_800B00C0_9FF70(void);

void func_8001BCC0_1C8C0(void);
void func_8001BC78_1C878(void);
void func_8001B3B8_1BFB8(void);

void loadOverlay_1BBA0(void) {
    LOAD_OVERLAY(_1DA660)
    setGameStateHandler(&func_8001B020_1BC20);
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B020_1BC20);

void func_8001B368_1BF68(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->unk5B3 != 0) {
        return;
    }

    if (func_8003BB5C_3C75C() != 0) {
        return;
    }

    func_8006FDA0_709A0(NULL, 0, 0x10);

    setGameStateHandler(func_8001B3B8_1BFB8);
}

void func_8001B3B8_1BFB8(void) {
    if (!func_8006FE10_70A10(0)) {
        setGameStateHandler(&func_8001B3E8_1BFE8);
    }
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B3E8_1BFE8);

void func_8001BBE8_1C7E8(void) {
    void *var_a0;
    Node_70B00 *temp_s0 = (Node_70B00 *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        unlinkNode(&temp_s0[0]);
        unlinkNode(&temp_s0[1]);
        unlinkNode(&temp_s0[2]);

        osViExtendVStart(0);

        temp_s0[3].unk0.next = freeNodeMemory(temp_s0[3].unk0.next);
        temp_s0[3].prev = freeNodeMemory(temp_s0[3].prev);

        if (temp_s0[3].unk8.callback_selector != 0) {
            var_a0 = &func_8001BCC0_1C8C0;
        } else {
            var_a0 = &func_8001BC78_1C878;
        }

        terminateSchedulerWithCallback(var_a0);
    }
}

void func_8001BC78_1C878(void) {
    s32 var_a0;

    var_a0 = 1;
    if ((D_800AFE8C_A71FC->unk4 == 0) && (D_800A8D14_A0084->unk10 == 5)) {
        var_a0 = 0x44;
    }
    func_800697F4_6A3F4(var_a0);
}

void func_8001BCC0_1C8C0(void) {
    func_800697F4_6A3F4(0xFF);
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001BCDC_1C8DC);

void func_8001BD74_1C974(s8 *a0) {
    if (*a0 == 3) {
        *a0 = 2;
    } else if (*a0 == 8) {
        *a0 = 7;
    } else if (*a0 != 4) {
    } else {
        *a0 = 0;
    }
}
