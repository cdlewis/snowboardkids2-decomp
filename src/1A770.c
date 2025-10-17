#include "1A770.h"

#include "20F0.h"
#include "3A1F0.h"
#include "3E160.h"
#include "4050.h"
#include "68CF0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x1D8];
    s16 unk1D8;
} func_8001AE80_1BA80_task_memory;

typedef struct {
    u8 padding[0x14];
} D_8008D7FC_8E3FC_item;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    D_8008D7FC_8E3FC_item *unk8;
} func_80019CD0_1A8D0_arg;

extern void func_8001A0F4_1ACF4(void);
extern void func_8001AF18_1BB18(void);
extern void func_80019BBC_1A7BC(void);
extern void func_80019CB4_1A8B4(applyTransformToModel_arg1 *);
extern void func_80019BF0_1A7F0(applyTransformToModel_arg1 *);
extern s32 identityMatrix;
extern D_8008D7FC_8E3FC_item D_8008D7FC_8E3FC[];
extern u8 D_800A8CC8_A0038;
extern void func_8001E590_1F190(void);
extern void func_80019E18_1AA18(void);
extern void func_8001A0F4_1ACF4(void);
extern void func_800226F0_232F0(void);
extern void func_80038090_38C90(void);
extern void func_80015254_15E54(void);
extern s32 gControllerInputs[4];
extern void func_80027CA0_288A0(void *, s32, s32, s32);

void func_8001A0B4_1ACB4(void);
void func_8001A070_1AC70(void);
void func_80019F60_1AB60(void);
void func_80019EFC_1AAFC(void);
void func_80019DC4_1A9C4(void);
void func_80019C7C_1A87C(applyTransformToModel_arg1 *arg0);
void func_80019D24_1A924(TextData *arg0);
void func_80019D84_1A984(void);
void func_800154A8_160A8(void *, s32);
void func_80019E60_1AA60(void);
void func_80019EA8_1AAA8(void);
void func_80019F30_1AB30(void);
void func_8001AF80_1BB80(void);
void func_8001A434_1B034(void);

void func_80019B70_1A770(func_80019B70_1A770_arg *arg0) {
    arg0->unk20 = func_8000198C_258C(0x13, getCurrentAllocation());
    setCleanupCallback(&func_80019CB4_1A8B4);
    setCallback(&func_80019BBC_1A7BC);
}

void func_80019BBC_1A7BC(void) {
    if (((GameState *)getCurrentAllocation())->unk429 == 0) {
        setCallback(&func_80019BF0_1A7F0);
    }
}

void func_80019BF0_1A7F0(applyTransformToModel_arg1 *arg0) {
    getCurrentAllocation();
    memcpy(arg0, &identityMatrix, 0x20);
    arg0->unk14 = 0x830000;
    arg0->unk18 = 0x340000;
    arg0->unk1C = 0xFF7D0000;
    applyTransformToModel(arg0->unk20, arg0);
    func_800021B8_2DB8(arg0->unk20, 0);
    updateModelGeometry(arg0->unk20);
    setCleanupCallback(&func_80019CB4_1A8B4);
    setCallback(&func_80019C7C_1A87C);
}

void func_80019C7C_1A87C(applyTransformToModel_arg1 *arg0) {
    getCurrentAllocation();
    clearModelRotation(arg0->unk20);
    updateModelGeometry(arg0->unk20);
}

void func_80019CB4_1A8B4(applyTransformToModel_arg1 *arg0) {
    func_80002014_2C14(arg0->unk20);
}

void func_80019CD0_1A8D0(func_80019CD0_1A8D0_arg *arg0) {
    arg0->unk0 = -0x28;
    arg0->unk2 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = &D_8008D7FC_8E3FC[D_800A8CC8_A0038];
    setCallback(&func_80019D24_1A924);
}

void func_80019D24_1A924(TextData *arg0) {
    debugEnqueueCallback(0, 7, &renderTextPalette, arg0);
}

void func_80019D50_1A950(void) {
    func_800154A8_160A8(allocateTaskMemory(0x1E0), 0x1E0);
    setGameStateHandler(&func_80019D84_1A984);
}

void func_80019D84_1A984(void) {
    if (func_8006FE10_70A10(0) == 0) {
        createTaskQueue(&func_8001E590_1F190, 100);
        setGameStateHandler(&func_80019DC4_1A9C4);
    }
}

void func_80019DC4_1A9C4(void) {
    s16 temp_v0 = func_80069810_6A410();
    if (temp_v0 != 0) {
        if (temp_v0 == 0xFF) {
            terminateSchedulerWithCallback(&func_8001A0F4_1ACF4);
            return;
        }

        setGameStateHandler(&func_80019E18_1AA18);
    }
}

void func_80019E18_1AA18(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_800574A0_580A0(2);
        createTaskQueue(&func_800226F0_232F0, 100);
        setGameStateHandler(&func_80019E60_1AA60);
    }
}

void func_80019E60_1AA60(void) {
    s16 temp_v0 = func_80069810_6A410();
    if (temp_v0 != 0) {
        void *var_a0 = &func_80019EA8_1AAA8;
        if (temp_v0 == 0xFF) {
            var_a0 = &func_80019D84_1A984;
        }
        setGameStateHandler(var_a0);
    }
}

void func_80019EA8_1AAA8(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_80003450_4050(D_800AFE8C_A71FC->saveSlotIndex, 0);
        createTaskQueue(&loadOverlay_1DD170, 150);
        setGameStateHandler(&func_80019EFC_1AAFC);
    }
}

void func_80019EFC_1AAFC(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        setGameStateHandler(&func_80019F30_1AB30);
    }
}

void func_80019F30_1AB30(void) {
    createTaskQueue(&func_8003D560_3E160, 100);
    setGameStateHandler(&func_80019F60_1AB60);
}

void func_80019F60_1AB60(void) {
    s16 saveOperationResult;
    D_800AFE8C_A71FC_type *gameStatePtr;
    u8 saveSlotIndex;

    saveOperationResult = func_80069810_6A410();

    if (saveOperationResult == 0) {
        return;
    }

    if (saveOperationResult >= 3) {
        if (saveOperationResult == 5 || saveOperationResult >= 7) {
            saveSlotIndex = D_800AFE8C_A71FC->saveSlotIndex;
            do {
                EepromSaveData->save_slot_status[saveSlotIndex] = 1;
            } while (0);
            func_80038090_38C90();
        } else {
            gameStatePtr = D_800AFE8C_A71FC;
            saveSlotIndex = gameStatePtr->saveSlotIndex;
            if (EepromSaveData->save_slot_status[saveSlotIndex] != 1) {
                EepromSaveData->save_slot_status[saveSlotIndex] = 4;
            }
        }
    }

    func_80015254_15E54();

    if (saveOperationResult == 1) {
        setGameStateHandler(func_80019F30_1AB30);
        return;
    }

    if (D_800AFE8C_A71FC->errorFlag != 0) {
        setGameStateHandler(func_8001A070_1AC70);
        return;
    }

    terminateSchedulerWithCallback(func_8001A0F4_1ACF4);
}

void func_8001A070_1AC70(void) {
    func_80003450_4050(D_800AFE8C_A71FC->errorFlag, 2);
    createTaskQueue(&loadOverlay_1DD170, 0x64);
    setGameStateHandler(&func_8001A0B4_1ACB4);
}

void func_8001A0B4_1ACB4(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        D_800AFE8C_A71FC->errorFlag = 0;
        terminateSchedulerWithCallback(&func_8001A0F4_1ACF4);
    }
}

void func_8001A0F4_1ACF4(void) {
    func_800697F4_6A3F4(0xFF);
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A110_1AD10);

void func_8001A3D8_1AFD8(void) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    if (temp_v0->unk77C != 0U) {
        temp_v0->unk77C--;
        return;
    }
    if (func_8003BB5C_3C75C() == 0) {
        func_8006FDA0_709A0(0, 0, 0x10);
        setGameStateHandler(&func_8001A434_1B034);
    }
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A434_1B034);

INCLUDE_RODATA("asm/nonmatchings/1A770", D_8009DF50_9EB50);

INCLUDE_RODATA("asm/nonmatchings/1A770", D_8009DF6C_9EB6C);

INCLUDE_RODATA("asm/nonmatchings/1A770", jtbl_8009DF78_9EB78);

INCLUDE_RODATA("asm/nonmatchings/1A770", jtbl_8009DF90_9EB90);

INCLUDE_RODATA("asm/nonmatchings/1A770", jtbl_8009DFA8_9EBA8);

INCLUDE_RODATA("asm/nonmatchings/1A770", jtbl_8009DFC0_9EBC0);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A478_1B078);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AC30_1B830);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AC70_1B870);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001ACC8_1B8C8);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AD80_1B980);

u8 func_8001AD9C_1B99C(void) {
    GameState *state;
    u8 result;
    s32 i;

    state = getCurrentAllocation();
    result = 0;
    for (i = 0; i < 3; i++) {
        if (EepromSaveData->character_or_settings[state->unk7A1 * 3 + i] != 0) {
            result++;
        }
    }

    return result;
}

void func_8001ADFC_1B9FC(void) {
    GameState *gameState;
    s32 delta;
    s32 i;
    s8 val = 1;

    gameState = getCurrentAllocation();
    delta = ((gameState->unk79C ^ 1) != 0) ? -val : 0;
    delta = delta | 1;

    for (i = 0; i < 4; i++) {
        gameState->unk784[i] += delta | 1;
        val = gameState->unk784[i];
        if (val < 0) {
            gameState->unk784[i] = gameState->unk798 - 1;
        } else if (val == gameState->unk798) {
            gameState->unk784[i] = 0;
        }
    }
}

void func_8001AE80_1BA80(void) {
    func_8001AE80_1BA80_task_memory *temp_s0 = (func_8001AE80_1BA80_task_memory *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8 = 0;
    func_80027CA0_288A0(temp_s0, 0, 0xA, 0);
    func_8006FDA0_709A0(0, 0, 8);
    scheduleTask(&func_80019CD0_1A8D0, 0U, 0U, 0x5AU);
    setGameStateHandler(&func_8001AF18_1BB18);
}

void func_8001AF18_1BB18(void) {
    Node_70B00 *temp_v0 = (Node_70B00 *)getCurrentAllocation();

    temp_v0[1].unk0.callback_selector++;

    do {
        if (gControllerInputs[0] & A_BUTTON) {
            temp_v0[1].unk0.callback_selector = 0x3C;
        }
    } while (0);

    if (temp_v0[1].unk0.callback_selector >= 0x3C) {
        unlinkNode(temp_v0);
        terminateSchedulerWithCallback(&func_8001AF80_1BB80);
    }
}

void func_8001AF80_1BB80(void) {
    func_800697F4_6A3F4(1);
}
