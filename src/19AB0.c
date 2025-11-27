#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"

extern u8 D_800A8A98_9FE08;
extern void initializeGameState(void);
void func_80018F50_19B50(void);

typedef struct {
    s16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} TaskAllocation;

INCLUDE_ASM("asm/nonmatchings/19AB0", func_80018EB0_19AB0);

INCLUDE_ASM("asm/nonmatchings/19AB0", func_80018F50_19B50);

INCLUDE_ASM("asm/nonmatchings/19AB0", func_80019078_19C78);

void func_800191B4_19DB4(void) {
    TaskAllocation *allocation = (TaskAllocation *)getCurrentAllocation();

    if (func_80069810_6A410() != 0) {
        EepromSaveData->padding_51[0] = 0;
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
