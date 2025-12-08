#include "297B0.h"
#include "2DD40.h"
#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002C8C0_2D4C0);

extern void func_8002BEF4_2CAF4(void *);

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x2C];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad52[0x10];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad63[0x65];
    /* 0xC8 */ void *unkC8;
    /* 0xCC */ u8 padCC[0x9];
    /* 0xD5 */ u8 unkD5;
} Func8002CA8CArg;

void func_8002CA8C_2D68C(Func8002CA8CArg *);

void func_8002CA8C_2D68C(Func8002CA8CArg *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CA8CArg *ptr;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = (Func8002CA8CArg *)((u8 *)arg0 + i * 0x64);

        if (ptr->unk50 != 0x19) {
            if (ptr->unk50 == 0x1A) {
                ptr->unk62 = 0;
            }
        } else {
            if (ptr->unk62 != 0) {
                ptr->unk50 = 0x1A;
            }
        }

        func_8002A2D0_2AED0(ptr);
        allocation->unk408[i] = ptr->unk18;
        allocation->unk410[i] = ptr->unk20;
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002CA8C_2D68C;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002CB88_2D788);

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002CD3C_2D93C);

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002CFAC_2DBAC);
