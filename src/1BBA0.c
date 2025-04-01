#include "D_800AFE8C_A71FC_type.h"
#include "common.h"

typedef struct {
    char padding[0x10];
    u8 unk10;
} D_800A8D14_A0084_type;

void func_800697F4_6A3F4(s32);
void func_8006982C_6A42C(void*);
s32 func_8006FE10_70A10(s32);
extern void func_8001B3E8_1BFE8();
extern D_800A8D14_A0084_type* D_800A8D14_A0084;

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001AFA0_1BBA0);

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B020_1BC20);

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B368_1BF68);

void func_8001B3B8_1BFB8() {
    if (!func_8006FE10_70A10(0)) {
        func_8006982C_6A42C(&func_8001B3E8_1BFE8);
    }
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B3E8_1BFE8);

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001BBE8_1C7E8);

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

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001BD74_1C974);
