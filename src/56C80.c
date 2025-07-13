#include "common.h"
#include "task_scheduler.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    f32 unk8;
    u8 data[12];
    s16 flags;
    u8 padding[2];
} RenderQueueItem;

typedef struct {
    s32 padding;
    void *unk4;
    void *unk8;
    u8 padding2[0x4];
    s16 unk10;
    u16 unk12;
    s16 unk14;
    s16 unk16;
    s16 flags;
    s16 unk1A;
    s8 unk1C;
    u8 unk1D;
    s32 unk20;
    s32 unk24;
    u8 padding8[0x3C];
    s16 unk64;
    u8 padding7[0x1D];
    s32 renderQueueCount;
    RenderQueueItem renderQueue[32];
    s32 unk408;
    u8 padding6[0x100];
    s8 bufferIds[0x8];
    s32 bufferFlags[8];
    s32 unk534;
    s32 unk538;
} GraphicsManager;
extern GraphicsManager *D_800A2990_A3590;

extern OSMesgQueue D_800A2CD0_A38D0;
extern OSMesgQueue D_800A2CF0_A38F0;
extern s32 D_800937EC_943EC[];
extern s32 D_800937F0_943F0[];
extern s32 D_80093978_94578[];
extern s32 D_8009397C_9457C[];
extern s32 D_80093B00_94700;

// This seems likely to be one struct
extern void *D_800A2D10_A3910;
extern void *D_800A2D14_A3914;
extern s32 D_800A2D18_A3918;
extern s32 D_800A2D1C_A391C;
extern s32 D_800A2D20_A3920;
extern s32 D_800A2D24_A3924;
extern s32 D_800A2D28_A3928;
extern s32 D_800A2D2C_A392C;
extern s32 D_800A2D30_A3930;
extern s32 D_800A2D38_A3938;
// End probable struct

extern u8 D_80093B84_94784[];
extern u8 D_80093BA5_947A5;
extern u8 D_80093BA6_947A6;
extern void func_800571D0_57DD0();
extern void func_80057214_57E14();
void *func_80058638_59238();
void func_800570BC_57CBC();
void func_80057124_57D24();
void func_800579E8_585E8(void *, s32);
void func_80057B70_58770(s32, s32, s32, s32, s32, s32, s32);
void func_80057CE4_588E4(s32, s32, s32, s32, s32, s32);
void func_80057E18_58A18(s32, s32, s32, s32, s32);
void func_80057F48_58B48(s32, s32, s32, s32);
void func_80058064_58C64(s32, s32, s32, s32);
void func_800582DC_58EDC(s32, s32, s32, s32, s32);
void func_80058414_59014(s32, s32, s32);
void func_800584AC_590AC(s32, s32, s32);
void func_8005854C_5914C(s32, s32);
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} D_800937E8_943E8_type;
extern D_800937E8_943E8_type D_800937E8_943E8[];
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} D_80093974_94574_type;
extern D_80093974_94574_type D_80093974_94574[];

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056080_56C80);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005610C_56D0C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005628C_56E8C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056914_57514);

void func_80056990_57590(s32 arg0, s32 arg1) {
    D_800A2990_A3590->unk538 = arg1;
    D_800A2990_A3590->unk534 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800569A4_575A4);

void func_80056A88_57688(void *arg0, u8 arg1, s32 arg2) {
    s32 i;
    s8 id;
    void *dst;
    volatile u8 padding[0x2];

    i = 0;
    id = arg2;
    if (D_800A2990_A3590->unk408 > 0) {
        do {
            if (D_800A2990_A3590->bufferIds[i] == id) {
                dst = (void *)((i << 5) + (s32)D_800A2990_A3590 + 0x40C);
                memcpy(dst, arg0, 0x20);
                return;
            }
            i++;
        } while (i < D_800A2990_A3590->unk408);
    }
    if (D_800A2990_A3590->unk408 < 8) {
        D_800A2990_A3590->bufferIds[D_800A2990_A3590->unk408] = id;
        dst = (void *)(((D_800A2990_A3590->unk408) << 5) + (s32)D_800A2990_A3590 + 0x40C);
        memcpy(dst, arg0, 0x20);
        D_800A2990_A3590->bufferFlags[D_800A2990_A3590->unk408] = arg1;
        D_800A2990_A3590->unk408++;
    }
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056B7C_5777C);

void func_80056C6C_5786C(void *arg0, unsigned int arg1, s16 arg2) {
    if (D_800A2990_A3590->renderQueueCount < 0x20) {
        RenderQueueItem *arrayCopy = D_800A2990_A3590->renderQueue;

        memcpy(arrayCopy[D_800A2990_A3590->renderQueueCount].data, arg0, 0xC);

        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk0 = arg1;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk2 = arg2;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk4 = 4;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk6 = 0;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].flags = 0x80;
        D_800A2990_A3590->renderQueueCount++;
    }
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056D64_57964);

void func_80056E64_57A64(void *arg0, int arg1, f32 arg2, s16 arg3, s32 arg4) {
    RenderQueueItem(*dest)[32];
    if (D_800A2990_A3590->renderQueueCount < 0x20) {
        dest = &D_800A2990_A3590->renderQueue;
        memcpy((*dest)[D_800A2990_A3590->renderQueueCount].data, arg0, 0xC);
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk0 = arg1;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk2 = (s16)arg4;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk4 = arg3;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk6 = 1;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].unk8 = arg2;
        D_800A2990_A3590->renderQueue[D_800A2990_A3590->renderQueueCount].flags = 0x80;
        D_800A2990_A3590->renderQueueCount++;
    }
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056F8C_57B8C);

void func_800570BC_57CBC() {
    D_800A2990_A3590->unk20 = (s32)((D_800A2990_A3590->unk20 + 1) & 0xFFFFFF);
}

void func_800570E0_57CE0(void *arg) {
    setCleanupCallback(func_800570E0_57CE0);
    if (D_800A2990_A3590->unk1D != 0) {
        setCallback(&func_80057124_57D24);
    }
}

void func_80057124_57D24(void) {
    D_800A2990_A3590->unk12 = D_800A2990_A3590->unk10;

    loadDataSegment(
        D_800937E8_943E8[D_800A2990_A3590->unk12].unk0,
        D_800937E8_943E8[D_800A2990_A3590->unk12].unk4,
        D_800937E8_943E8[D_800A2990_A3590->unk12].unk8,
        D_800A2990_A3590->unk8
    );

    loadDataSegment(
        D_80093974_94574[D_800A2990_A3590->unk12].unk0,
        D_80093974_94574[D_800A2990_A3590->unk12].unk4,
        D_80093974_94574[D_800A2990_A3590->unk12].unk8,
        D_800A2990_A3590->unk4
    );

    setCallback(&func_800571D0_57DD0);
}

void func_800571D0_57DD0() {
    func_800579E8_585E8(D_800A2990_A3590->unk4, *(&D_80093B00_94700 + (D_800A2990_A3590->unk12)));
    setCallback(&func_80057214_57E14);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057214_57E14);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800572B0_57EB0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800573F8_57FF8);

extern void func_800570E0_57CE0(void *arg);

void func_80057470_58070(void) {
    if (func_80058638_59238() == NULL) {
        setCallbackWithContinue(&func_800570E0_57CE0);
    }
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800574A0_580A0);

void func_800574E0_580E0(s16 arg0, s8 arg1) {
    D_800A2990_A3590->unk1D = 2;
    D_800A2990_A3590->unk10 = arg0;
    D_800A2990_A3590->unk16 = 0x80;
    D_800A2990_A3590->flags = 0x80;
    D_800A2990_A3590->unk1A = 0;
    D_800A2990_A3590->unk1C = arg1;
}

void func_80057514_58114(u32 arg0, s16 arg1, s16 arg2) {
    GraphicsManager *new_var;
    D_800A2990_A3590->unk1D = 2;
    D_800A2990_A3590->unk10 = arg0;
    D_800A2990_A3590->unk16 = 0;
    D_800A2990_A3590->flags = arg1;
    D_800A2990_A3590->unk1A = arg2;
    D_800A2990_A3590->unk1C = D_80093B84_94784[arg0];
}

void func_80057550_58150(s16 arg0, s16 arg1) {
    D_800A2990_A3590->flags = arg0;
    D_800A2990_A3590->unk1A = arg1;
}

void func_80057564_58164(int arg0) {
    int var_a0;
    GraphicsManager *new_var;
    var_a0 = arg0;
    if (arg0 < 8) {
        var_a0 = 8;
    }
    D_800A2990_A3590->unk1D = 0;
    (new_var = D_800A2990_A3590)->unk14 = var_a0;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005758C_5818C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057614_58214);

void func_80057870_58470(s32 arg0) {
    void *message;

    if ((D_80093BA5_947A5 != NULL) && (D_80093BA6_947A6 == NULL)) {
        D_80093BA6_947A6++;
        D_800A2D30_A3930 = arg0;
        osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xD, OS_MESG_BLOCK);
        osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    }
}

void func_800578DC_584DC(s32 arg0, s32 arg1) {
    void *message;

    D_800A2D2C_A392C = arg0;
    D_800A2D30_A3930 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)7, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void func_80057928_58528(s32 arg0, s32 arg1) {
    void *message;

    D_800A2D2C_A392C = arg0;
    D_800A2D24_A3924 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xC, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void *func_80057974_58574(void *arg0, void *arg1, s32 arg2) {
    void *sp10;

    D_800A2D14_A3914 = arg0;
    D_800A2D10_A3910 = arg1;
    D_800A2D38_A3938 = arg2;
    osSendMesg(&D_800A2CD0_A38D0, (void *)0xB, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &sp10, OS_MESG_BLOCK);
    return sp10;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800579CC_585CC);

void func_800579E8_585E8(void *arg0, s32 arg1) {
    void *message;

    D_800A2D14_A3914 = arg0;
    D_800A2D18_A3918 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xA, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void *func_80057A34_58634(s32 arg0) {
    void *message;

    D_800A2D2C_A392C = arg0;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)9, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    return message;
}

void *func_80057A7C_5867C(void) {
    void *message;

    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)8, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    return message;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057ABC_586BC);

void func_80057B1C_5871C(s32 arg0) {
    void *message;

    D_800A2D30_A3930 = arg0;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)6, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    D_800A2990_A3590->renderQueueCount = 0;
    D_800A2990_A3590->unk408 = 0;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057B70_58770);

void func_80057CB4_588B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80057B70_58770(arg0, arg1, arg2, arg3, arg4, arg5, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057CE4_588E4);

void func_80057DF0_589F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80057CE4_588E4(arg0, arg1, arg2, arg3, arg4, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057E18_58A18);

void func_80057F28_58B28(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80057E18_58A18(arg0, arg1, arg2, arg3, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057F48_58B48);

void func_80058048_58C48(s32 arg0, s32 arg1, s32 arg2) {
    func_80057F48_58B48(arg0, arg1, arg2, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058064_58C64);

void func_80058138_58D38(s32 arg0, s32 arg1, s32 arg2) {
    func_80058064_58C64(arg0, arg1, arg2, 0xC);
}

void func_80058154_58D54(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 temp_s0;
    s32 *new_var;
    GraphicsManager *temp_v1;
    s32 temp_v0;
    void *temp_ptr;

    temp_v1 = D_800A2990_A3590;
    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = 0x80;
    new_var = &temp_v1->unk20;
    D_800A2D28_A3928 = 0x80;
    temp_v0 = *new_var;

    temp_s0 = arg1 << 2;
    D_800A2D20_A3920 = temp_v1->unk20;
    temp_ptr = (void *)temp_v1;
    temp_v1 = (GraphicsManager *)((temp_s0) + (s32)temp_ptr);
    D_800A2D38_A3938 = arg2;
    temp_v0 = temp_v1->unk24;
    D_800A2D2C_A392C = temp_v1->unk24;

    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)2, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)(&sp10), OS_MESG_BLOCK);

    temp_s0 = temp_s0 + ((s32)D_800A2990_A3590);

    ((GraphicsManager *)temp_s0)->unk24 = sp10;
    ((GraphicsManager *)(arg1 + (s16 *)D_800A2990_A3590))->unk64 = arg0;

    func_800570BC_57CBC();
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058220_58E20);

void func_8005823C_58E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;
    GraphicsManager *new_var;
    s32 *new_var2;
    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = arg1;
    D_800A2D28_A3928 = 0x80;
    new_var = D_800A2990_A3590;
    new_var2 = &new_var->unk20;
    D_800A2D20_A3920 = (*new_var2) + (arg2 << 0x18);
    D_800A2D38_A3938 = arg3;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_800582C0_58EC0(s32 arg0, s32 arg1, s32 arg2) {
    func_8005823C_58E3C(arg0, arg1, arg2, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800582DC_58EDC);

void func_80058360_58F60(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800582DC_58EDC(arg0, arg1, arg2, arg3, 0xC);
}

void func_80058380_58F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *new_var;
    void *message;
    s32 new_var2;
    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = arg1;
    D_800A2D28_A3928 = arg2;
    new_var = &D_800A2990_A3590->unk20;
    new_var2 = *new_var;
    D_800A2D38_A3938 = arg3;
    D_800A2D20_A3920 = new_var2;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_800583F8_58FF8(s32 arg0, s32 arg1, s32 arg2) {
    func_80058380_58F80(arg0, arg1, arg2, 0xC);
}

void func_80058414_59014(s32 arg0, s32 arg1, s32 arg2) {
    void *sp10;
    s32 *new_var;
    s32 new_var2;
    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = arg1;
    D_800A2D28_A3928 = 0x80;
    new_var = &D_800A2990_A3590->unk20;
    new_var2 = *new_var;
    D_800A2D38_A3938 = arg2;
    D_800A2D20_A3920 = new_var2;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_80058490_59090(s32 arg0, s32 arg1) {
    func_80058414_59014(arg0, arg1, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800584AC_590AC);

void func_80058530_59130(s32 arg0, s32 arg1) {
    func_800584AC_590AC(arg0, arg1, 0xC);
}

void func_8005854C_5914C(s32 arg0, s32 arg1) {
    s32 new_var2;
    s32 *new_var;
    void *sp10;

    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = 0x80;
    D_800A2D28_A3928 = 0x80;
    new_var2 = *(new_var = &D_800A2990_A3590->unk20);
    D_800A2D38_A3938 = arg1;
    D_800A2D20_A3920 = new_var2;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_800585C8_591C8(s32 arg0) {
    func_8005854C_5914C(arg0, 0xC);
}

void *func_800585E4_591E4(void) {
    void *result;

    if (D_80093BA5_947A5 != 0) {
        osSendMesg(&D_800A2CD0_A38D0, 0, OS_MESG_BLOCK);
        osRecvMesg(&D_800A2CF0_A38F0, &result, OS_MESG_BLOCK);
        return result;
    }

    return 0;
}

void *func_80058638_59238() {
    void *message;

    if (D_80093BA5_947A5 != NULL) {
        osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xE, OS_MESG_BLOCK);
        osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);

        return message;
    }

    return NULL;
}
