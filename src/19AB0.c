#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"

extern u8 D_800A8A98_9FE08;
extern u8 D_800A8CC8_A0038;
extern void initializeGameState(void);
extern u8 func_8001523C_15E3C(void);
void func_80018F50_19B50(void);
void func_80019078_19C78(void);
void func_80018DC0_199C0(void);

typedef struct {
    s16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} TaskAllocation;

void func_80018EB0_19AB0(void) {
    TaskAllocation *allocation;

    allocation = (TaskAllocation *)allocateTaskMemory(6);
    allocation->unk0 = 0;
    allocation->unk2 = 0;
    allocation->unk3 = 0;

    if (func_8001523C_15E3C() != 0) {
        allocation->unk2 = 1;
        D_800A8A98_9FE08 = 1;
        D_800A8CC8_A0038 = 0;
        createTaskQueue(initializeGameState, 100);
        setGameStateHandler(func_80018F50_19B50);
    } else {
        D_800A8CC8_A0038 = 4;
        createTaskQueue(func_80018DC0_199C0, 100);
        setGameStateHandler(func_80019078_19C78);
    }
}

INCLUDE_ASM("asm/nonmatchings/19AB0", func_80018F50_19B50);

INCLUDE_ASM("asm/nonmatchings/19AB0", func_80019078_19C78);

void func_800191B4_19DB4(void) {
    TaskAllocation *allocation = (TaskAllocation *)getCurrentAllocation();

    if (func_80069810_6A410() != 0) {
        EepromSaveData->unk51 = 0;
        allocation->unk2 = 1;
        D_800A8A98_9FE08 = 1;
        createTaskQueue(initializeGameState, 100);
        setGameStateHandler(func_80018F50_19B50);
    }
}

void func_80019220_19E20(void) {
    func_800697F4_6A3F4(0xFF);
}

void func_8001923C_19E3C(void) {
    func_800697F4_6A3F4(0xFE);
}

void func_80019258_19E58(void) {
    func_800697F4_6A3F4(0x44);
}
