#include "common.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_647F90);
USE_ASSET(_49B420);

extern void func_8000CB94_D794(void);
extern void func_8000CC84_D884(void);

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    u8 padding[0x16];
    s8 unk22;
    s32 unk24;
    s32 unk28;
    u8 padding2[0x16];
    s8 unk42;
    u8 padding3[0x10];
    s16 unk54;
    s16 unk56;
} func_8000CAF0_D6F0_arg;

void func_8000CAF0_D6F0(func_8000CAF0_D6F0_arg *arg0) {
    void *temp_v0;

    arg0->unk0 = dmaRequestAndUpdateStateWithSize(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    temp_v0 = dmaRequestAndUpdateStateWithSize(&_49B420_ROM_START, &_49B420_ROM_END, 0x240);
    arg0->unk4 = temp_v0;
    arg0->unk8 = (s32)(temp_v0 + (arg0->unk56 << 6));
    arg0->unk22 = (u8)((randA() & 0x1F) + 0x70);
    arg0->unk54 = 0;
    arg0->unk28 = (s32)arg0->unk8;
    arg0->unk42 = (u8)arg0->unk22;
    setCleanupCallback((void (*)(void *))&func_8000CC84_D884);
    setCallbackWithContinue((void (*)(void *))&func_8000CB94_D794);
}

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CB94_D794);

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CC84_D884);

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CCBC_D8BC);

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CD88_D988);

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CDB4_D9B4);

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CDE0_D9E0);

INCLUDE_ASM("asm/nonmatchings/D6F0", func_8000CDFC_D9FC);
