#include "3A1F0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void *freeNodeMemory(void *);

typedef struct {
    Node_70B00 unk0;
    u8 pad1D8[0x760 - 0x1D8];
    Node_70B00 unk760;
    u8 padD38[0xEC0 - (0x760 + 0x1D8)];
    Node_70B00 unkEC0;
    u8 pad1098[0x1620 - (0xEC0 + 0x1D8)];
    Node_70B00 unk1620;
    u8 pad17F8[0x1878 - (0x1620 + 0x1D8)];
    void *unk1878;
    void *unk187C;
    u8 pad1880[0x18A0 - 0x1880];
    u16 unk18A0;
} Allocation232F0;

extern void func_80022CEC_238EC(void);
void func_80023D74_24974(void);
void func_80023DA0_249A0(void);

INCLUDE_ASM("asm/nonmatchings/232F0", func_800226F0_232F0);

void func_80022C80_23880(void) {
    Allocation232F0 *alloc = (Allocation232F0 *)getCurrentAllocation();

    alloc->unk18A0++;
    if ((u16)alloc->unk18A0 < 3) {
        return;
    }
    alloc->unk18A0 = 2;
    if (func_8003BB5C_3C75C() != 0) {
        return;
    }
    alloc->unk18A0 = 0;
    func_8006FDA0_709A0(NULL, 0, 10);
    setGameStateHandler(func_80022CEC_238EC);
}

INCLUDE_ASM("asm/nonmatchings/232F0", func_80022CEC_238EC);

INCLUDE_ASM("asm/nonmatchings/232F0", func_80022D74_23974);

void func_80023C7C_2487C(void) {
    Allocation232F0 *s1 = getCurrentAllocation();
    s32 i;

    if (func_8006FE10_70A10(0) != 0) {
        return;
    }

    unlinkNode(&s1->unk1620);

    for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
        unlinkNode((Node_70B00 *)((u8 *)s1 + i * 0x1D8));
        unlinkNode((Node_70B00 *)((u8 *)s1 + 0x760 + i * 0x1D8));
        unlinkNode((Node_70B00 *)((u8 *)s1 + 0xEC0 + i * 0x1D8));
    }

    s1->unk1878 = freeNodeMemory(s1->unk1878);
    s1->unk187C = freeNodeMemory(s1->unk187C);

    if (s1->unk18A0 == 0x63) {
        terminateSchedulerWithCallback(func_80023DA0_249A0);
    } else {
        terminateSchedulerWithCallback(func_80023D74_24974);
    }
}

void func_80023D74_24974(void) {
    func_8006FDA0_709A0(NULL, 0, 0);
    func_800697F4_6A3F4(1);
}

void func_80023DA0_249A0(void) {
    func_8006FDA0_709A0(NULL, 0, 0);
    func_800697F4_6A3F4(0xFF);
}

INCLUDE_ASM("asm/nonmatchings/232F0", func_80023DCC_249CC);

INCLUDE_RODATA("asm/nonmatchings/232F0", D_8009E288_9EE88);
