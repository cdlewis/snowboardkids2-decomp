#include "common.h"
#include "event_trigger.h"

void func_80069CC0_6A8C0(void *);
u8 D_8008D7D0_8E3D0[] = "WALL NEWSPAPER";
extern void *func_80019468_1A068;

void func_80019420_1A020(EventTrigger *arg0) {
    arg0->unk0 = 7;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &D_8008D7D0_8E3D0;
    func_80069CC0_6A8C0(&func_80019468_1A068);
}

INCLUDE_ASM("asm/nonmatchings/1A020", func_80019468_1A068);
