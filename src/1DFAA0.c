#include "1DFAA0.h"
#include "task_scheduler.h"

typedef struct {
    void *start;
    void *end;
    u32 size;
    u32 padding;
} D_800BA960_1E7A10_node;
extern D_800BA960_1E7A10_node D_800BA960_1E7A10[];

void func_800B477C_1E182C(void);

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s32 unk4;
    u8 padding[0xDC];
    s16 unkE4;
} TaskData;

typedef struct {
    u8 padding0[0xC];
    /* 0xC */ u16 current_index;
    u8 padding1[0x2A];
    /* 0x38 */ u16 next_index;
    u8 padding2[0x2];
    s16 unk3C;
    s8 unk3E;
    s8 unk3F;
} StateEntry;

extern StateEntry *D_800BAEBC_1E7F6C;

typedef struct {
    u8 padding[0xFF7];
    s8 unkFF7;
} func_800B29F0_1DFAA0_arg;
void func_800B29F0_1DFAA0(func_800B29F0_1DFAA0_arg *arg0) {
    arg0->unkFF7 = 1;
}

extern s32 D_800AB050_A23C0;

void func_800B29FC_1DFAAC(func_800B29F0_1DFAA0_arg *arg0) {
    if (arg0->unkFF7 != 0 && (D_800AB050_A23C0 & 0x8000)) {
        arg0->unkFF7 = 0;
    }
}

typedef struct {
    u8 padding[0xEC];
    s8 unkEC;
    u8 padding2[0x0];
    s32 unkF4;
} func_800B2A24_1DFAD4_arg;
void func_800B2A24_1DFAD4(func_800B2A24_1DFAD4_arg *arg0, s16 arg1) {
    func_800B2A24_1DFAD4_arg *new_var = arg0 + arg1;
    new_var->unkEC = 1;
}

void func_800B2A50_1DFB00(func_800B2A24_1DFAD4_arg *arg0, s16 arg1) {
    func_800B2A24_1DFAD4_arg *new_var = arg0 + arg1;
    new_var->unkEC = 0;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2A78_1DFB28);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2AA0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2C78);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2D04);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2D68_1DFE18);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2DCC_1DFE7C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2E48_1DFEF8);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2F2C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B305C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3360);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3420_1E04D0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3430_1E04E0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3440_1E04F0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3450_1E0500);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3460_1E0510);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3470_1E0520);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3480_1E0530);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3490_1E0540);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34A0_1E0550);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34B0_1E0560);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34D0_1E0580);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34E0_1E0590);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34F0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3500_1E05B0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3510_1E05C0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3540_1E05F0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3570);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B36C0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3734_1E07E4);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3790_1E0840);

u16 func_800B384C_1E08FC(void) {
    u16 index = D_800BAEBC_1E7F6C->current_index;
    StateEntry *currentEntry = &D_800BAEBC_1E7F6C[index];

    D_800BAEBC_1E7F6C->current_index = currentEntry[3].next_index;

    return index;
}

void resetScriptState(u8 *arg0) {
    s32 i;

    for (i = 0x37; i >= 0; i--) {
        arg0[i] = 0;
    }
}

void func_800B388C_1E093C(s32 arg0) {
    StateEntry *temp;

    resetScriptState(D_800BAEBC_1E7F6C[arg0 + 3].padding0);

    temp = (D_800BAEBC_1E7F6C + arg0 + 3);
    temp->unk3C = 0;

    temp = D_800BAEBC_1E7F6C + arg0 + 3;
    temp->unk3E = 0;

    temp = D_800BAEBC_1E7F6C + arg0 + 3;
    temp->unk3F = 0;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B38E4);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3B40);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3B68_1E0C18);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3C7C_1E0D2C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3D24_1E0DD4);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3E58_1E0F08);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3F48_1E0FF8);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3F64_1E1014);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3FFC_1E10AC);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B41E0_1E1290);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4258_1E1308);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4288_1E1338);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4294_1E1344);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B42B0_1E1360);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B42E8_1E1398);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4320_1E13D0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4378_1E1428);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B44A8_1E1558);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4534_1E15E4);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B462C_1E16DC);

void *func_800B4680_1E1730(s8 arg0) {
    D_800BA960_1E7A10_node *node;

    if (arg0 < 0x10) {
        node = &D_800BA960_1E7A10[arg0];
        if (D_800BA960_1E7A10[arg0].start != NULL) {
            return dmaRequestAndUpdateStateWithSize(node->start, node->end, node->size);
        }
    }

    return NULL;
}

void func_800B46E0(s32 arg0, s8 arg1, s16 arg2) {
    TaskData *task;

    if (arg2 == 0) {
        return;
    }

    if (arg1 >= 0x10) {
        return;
    }

    if (D_800BA960_1E7A10[arg1].start == NULL) {
        return;
    }

    task = (TaskData *)scheduleTask(&func_800B477C_1E182C, 1, 0, 0x64);
    if (task != NULL) {
        task->unk0 = 0;
        task->unk1 = arg1;
        task->unk2 = arg2;
        task->unk4 = arg0;
        task->unkE4 = 0xFF;
    }
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B477C_1E182C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4914_1E19C4);

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_800B4ACC_1E1B7C_arg;
void func_800B4ACC_1E1B7C(func_800B4ACC_1E1B7C_arg *arg0) {
    freeNodeMemory(arg0->unk8);
}
