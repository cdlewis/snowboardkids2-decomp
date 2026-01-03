#include "common.h"

INCLUDE_ASM("asm/nonmatchings/39020", func_80038420_39020);

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    void *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
} StructA;

typedef struct {
    u16 unk00;
    u16 unk02;
    u8 unk04;
    u8 unk05;
    u16 unk06;
    u8 pad08[2];
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u8 data10[1];
} StructB;

void func_800394BC_3A0BC(StructA *arg0, StructB *arg1) {
    StructB *temp;
    s32 padding1;
    s32 padding2;
    temp = arg1;

    arg0->unk00 = 0;
    arg0->unk02 = 0;
    arg0->unk08 = arg1->unk00;
    arg0->unk0A = arg1->unk02;
    arg0->unk0C = arg0->unk08;
    arg0->unk0E = arg0->unk0A;
    arg0->unk04 = arg1->unk04;
    arg0->unk06 = arg1->unk05;
    arg0->unk18 = arg1->unk06;
    arg0->unk10 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0x140;
    arg0->unk16 = 0xF0;
    arg0->unk20 = (u8 *)temp + arg1->unk0A;
    arg0->unk1C = (u8 *)temp + arg1->unk0E;
    arg0->unk24 = &arg1->data10;
    arg0->unk28 = (u8 *)temp + arg1->unk0C;
}