#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_800145E0_151E0(void);
extern void func_8001474C_1534C(void);
extern void func_80014808_15408(void);
extern void func_8001489C_1549C(void);
extern void func_80014900_15500(void);
extern void func_80014990_15590(void);
extern void func_80014660_15260(void);

typedef struct {
    s16 unk0;
    u8 unk2;
    u8 pad;
} TaskData_1512C;

INCLUDE_ASM("asm/nonmatchings/15080", func_80014480_15080);

INCLUDE_ASM("asm/nonmatchings/15080", func_800144DC_150DC);

void func_8001452C_1512C(void) {
    TaskData_1512C *data = allocateTaskMemory(4);
    func_8006FDA0_709A0(NULL, 0, 0);
    data->unk0 = 0;
    data->unk2 = 0;
    func_80014990_15590();
    func_8006983C_6A43C(func_80014660_15260);
}

INCLUDE_ASM("asm/nonmatchings/15080", func_8001457C_1517C);

INCLUDE_ASM("asm/nonmatchings/15080", func_800145AC_151AC);

INCLUDE_ASM("asm/nonmatchings/15080", func_800145E0_151E0);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014620_15220);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014660_15260);

void func_80014690_15290(void) {
    s16 result = func_80069810_6A410();

    if (result == 0) {
        return;
    }

    if (result == 4) {
        D_800AFE8C_A71FC_type *ptr = D_800AFE8C_A71FC;
        u8 val = ptr->unk5;

        if (val >= 3) {
            ptr->unk5 = val & 0xF;
            setGameStateHandler(func_800145E0_151E0);
        } else {
            setGameStateHandler(func_8001474C_1534C);
        }
    } else if (result == 2) {
        setGameStateHandler(func_80014808_15408);
    } else if (result == 3) {
        setGameStateHandler(func_8001489C_1549C);
    } else {
        terminateSchedulerWithCallback(func_80014900_15500);
    }
}

INCLUDE_ASM("asm/nonmatchings/15080", func_8001474C_1534C);

INCLUDE_ASM("asm/nonmatchings/15080", func_8001478C_1538C);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014808_15408);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014854_15454);

INCLUDE_ASM("asm/nonmatchings/15080", func_8001489C_1549C);

INCLUDE_ASM("asm/nonmatchings/15080", func_800148CC_154CC);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014900_15500);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014958_15558);

INCLUDE_ASM("asm/nonmatchings/15080", func_80014990_15590);
