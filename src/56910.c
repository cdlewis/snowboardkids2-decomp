#include "56910.h"
#include "common.h"
#include "task_scheduler.h"

typedef struct {
    void *start;
    void *end;
    s32 size;
} Asset;
extern Asset D_80091250_91E50[];
extern func_80055E68_56A68_result *D_80091590_92190[];

INCLUDE_ASM("asm/nonmatchings/56910", func_80055D10_56910);

void *func_80055D34_56934(s32 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80091250_91E50[index].start,
        D_80091250_91E50[index].end,
        D_80091250_91E50[index].size
    );
}

INCLUDE_ASM("asm/nonmatchings/56910", func_80055D7C_5697C);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055DC4_569C4);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055DF8_569F8);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055E40_56A40);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055E54_56A54);

func_80055E68_56A68_result *func_80055E68_56A68(s32 index) {
    return D_80091590_92190[index];
}

INCLUDE_ASM("asm/nonmatchings/56910", func_80055E7C_56A7C);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055EA4_56AA4);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055ED0_56AD0);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055EFC_56AFC);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055F28_56B28);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055F54_56B54);

INCLUDE_ASM("asm/nonmatchings/56910", func_80055F80_56B80);
