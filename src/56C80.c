#include "common.h"
#include "displaylist.h"
#include "player.h"
#include "task_scheduler.h"

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    s32 unk24;
    s32 unk28;
} GraphicsCommand;
extern GraphicsCommand D_800A2D10_A3910;

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
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u8 padding1[0x4];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 flags;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 padding2[0x2];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24[0xF];
    /* 0x60 */ s32 padding3;
    /* 0x64 */ s16 unk64[0x10];
    /* 0x84 */ s32 renderQueueCount;
    /* 0x88 */ RenderQueueItem renderQueue[32];
    /* 0x408 */ s32 unk408;
    /* 0x40C */ u8 bufferData[8][0x20];
    /* 0x50C */ s8 bufferIds[0x8];
    /* 0x514 */ s32 bufferFlags[8];
    /* 0x534 */ s32 unk534;
    /* 0x538 */ s32 unk538;
} GraphicsManager;
extern GraphicsManager *D_800A2990_A3590;

extern OSMesgQueue D_800A2CD0_A38D0;
extern OSMesgQueue D_800A2CF0_A38F0;
extern s32 D_800937EC_943EC[];
extern s32 D_800937F0_943F0[];
extern s32 D_80093978_94578[];
extern s32 D_8009397C_9457C[];
extern s32 D_80093B00_94700;
extern s32 D_80092E28_93A28;
extern s32 D_80093308_93F08;
void func_8006983C_6A43C(void *);
extern s32 D_6A83F0;
extern s32 D_6B6410;
extern s32 D_6A83F0;
extern s32 D_6B6410;
extern s32 D_6C6300;

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
void func_8005628C_56E8C(void);
void func_8005610C_56D0C(void);
void func_800570E0_57CE0(void *arg);
void func_8005758C_5818C(s32);

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

void func_80056080_56C80(void) {
    u8 sp10;

    D_800A2990_A3590 = allocateTaskMemory(0x53C);
    D_800A2990_A3590->unk0 = dmaRequestAndUpdateStateWithSize(&D_6A83F0, &D_6B6410, 0xCFD8);
    D_800A2990_A3590->unk8 = allocateMemoryNode(0, 0x9000, &sp10);
    D_800A2990_A3590->unk4 = allocateMemoryNode(0, 0x8000, &sp10);
    func_8006983C_6A43C(&func_8005610C_56D0C);
}

void func_8005610C_56D0C(void) {
    musConfig config;
    GraphicsManager *mgr;
    u16 negativeOne;
    void *newNode;
    s32 i;
    u8 alreadyAllocated;

    setupTaskSchedulerNodes(1, 0, 0, 0, 0, 0, 0, 0);
    config.control_flag = 0;
    config.channels = 0x18;
    config.thread_priority = 5;
    newNode = allocateMemoryNode(0, 0x37A20, &alreadyAllocated);
    config.heap = newNode;
    config.heap_length = 0x37A20;
    mgr = D_800A2990_A3590;
    config.fifo_length = 0x40;
    config.ptr = mgr->unk0;
    config.wbk = (u8 *)(&D_6C6300);
    config.sched = &D_80092E28_93A28;
    config.default_fxbank = &D_80093308_93F08;
    config.syn_rsp_cmds = 0x5622;
    config.syn_output_rate = 0x100;
    config.syn_retraceCount = 0x1000;
    config.syn_num_dma_bufs = 1;
    config.syn_dma_buf_size = 0x64;
    config.diskrom_handle = (OSPiHandle *)0x400;
    MusInitialize(&config);

    D_80093BA5_947A5 = 1;
    negativeOne = -1;
    D_800A2990_A3590->unk1D = 0;
    D_800A2990_A3590->unk10 = negativeOne;
    D_800A2990_A3590->unk12 = 0xFFFF;
    D_800A2990_A3590->unk14 = 0;
    scheduleTask(func_800570E0_57CE0, 0, 0, 0x64);
    D_800A2990_A3590->unk20 = 0;
    for (i = 0xF; i >= 0; i--) {
        D_800A2990_A3590->unk24[i] = 0;
    }

    D_800A2990_A3590->unk534 = 0x20;
    D_800A2990_A3590->renderQueueCount = 0;
    D_800A2990_A3590->unk408 = 0;
    D_800A2990_A3590->unk538 = 0xC80;
    func_8005758C_5818C(i);
    func_8006983C_6A43C(&func_8005628C_56E8C);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005628C_56E8C);

void func_80056914_57514(void *source) {
    if (D_800A2990_A3590->unk408 < 8) {
        u8(*dest)[0x20];

        D_800A2990_A3590->bufferIds[D_800A2990_A3590->unk408] = -1;
        dest = &D_800A2990_A3590->bufferData[D_800A2990_A3590->unk408];
        dest++;
        dest--;

        memcpy(dest, source, 0x20);

        D_800A2990_A3590->bufferFlags[D_800A2990_A3590->unk408] = 0;
        D_800A2990_A3590->unk408 += 1;
    }
}

void func_80056990_57590(s32 arg0, s32 arg1) {
    D_800A2990_A3590->unk538 = arg1;
    D_800A2990_A3590->unk534 = arg0;
}

void func_800569A4_575A4(u8 *src_data, s8 search_id) {
    s32 i;
    void *bufferPtr;

    for (i = 0; i < D_800A2990_A3590->unk408; i++) {
        if (D_800A2990_A3590->bufferIds[i] == search_id) {
            bufferPtr = (void *)((i << 5) + (s32)D_800A2990_A3590 + 0x40C);
            memcpy(bufferPtr, src_data, 0x20);
            return;
        }
    }

    // write to the next available buffer if there's space
    if (D_800A2990_A3590->unk408 < 8) {
        D_800A2990_A3590->bufferIds[D_800A2990_A3590->unk408] = search_id;
        bufferPtr = (void *)((D_800A2990_A3590->unk408 << 5) + (s32)D_800A2990_A3590 + 0x40C);
        memcpy(bufferPtr, src_data, 0x20);
        D_800A2990_A3590->bufferFlags[D_800A2990_A3590->unk408] = 0;
        D_800A2990_A3590->unk408++;
    }
}

void setBufferData(void *source, u8 arg1, s32 arg2) {
    s32 i;
    s8 id;
    void *bufferPtr;

    // Search for existing buffer
    id = arg2;
    for (i = 0; i < D_800A2990_A3590->unk408; i++) {
        if (D_800A2990_A3590->bufferIds[i] == id) {
            bufferPtr = (void *)((i << 5) + (s32)D_800A2990_A3590 + 0x40C);
            memcpy(bufferPtr, source, 0x20);
            return;
        }
    }

    // Add new buffer if space available
    if (D_800A2990_A3590->unk408 < 8) {
        D_800A2990_A3590->bufferIds[D_800A2990_A3590->unk408] = id;

        bufferPtr = (void *)(((D_800A2990_A3590->unk408) << 5) + (s32)D_800A2990_A3590 + 0x40C);

        memcpy(bufferPtr, source, 0x20);

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

void func_80057870_58470(void *arg0) {
    void *message;

    if ((D_80093BA5_947A5 != NULL) && (D_80093BA6_947A6 == NULL)) {
        D_80093BA6_947A6++;
        D_800A2D10_A3910.unk20 = arg0;
        osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xD, OS_MESG_BLOCK);
        osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    }
}

void func_800578DC_584DC(s32 arg0, void *arg1) {
    void *message;

    D_800A2D10_A3910.unk1C = arg0;
    D_800A2D10_A3910.unk20 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)7, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void func_80057928_58528(s32 arg0, s32 arg1) {
    void *message;

    D_800A2D10_A3910.unk1C = arg0;
    D_800A2D10_A3910.unk14 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xC, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void *func_80057974_58574(void *arg0, void *arg1, s32 arg2) {
    void *sp10;

    D_800A2D10_A3910.unk4 = arg0;
    D_800A2D10_A3910.unk0 = arg1;
    D_800A2D10_A3910.unk28 = arg2;
    osSendMesg(&D_800A2CD0_A38D0, (void *)0xB, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &sp10, OS_MESG_BLOCK);
    return sp10;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800579CC_585CC);

void func_800579E8_585E8(void *arg0, s32 arg1) {
    void *message;

    D_800A2D10_A3910.unk4 = arg0;
    D_800A2D10_A3910.unk8 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xA, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void *func_80057A34_58634(s32 arg0) {
    void *message;

    D_800A2D10_A3910.unk1C = arg0;
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

void func_80057B1C_5871C(void *arg0) {
    void *message;

    D_800A2D10_A3910.unk20 = arg0;
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

void func_80057E18_58A18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 new_var;
    s32 *new_var3;
    void *sp10;

    D_800A2D10_A3910.unk1C = D_800A2990_A3590->unk24[arg3];
    new_var3 = &D_800A2990_A3590->unk20;

    if (arg1 > 0) {
        D_800A2D10_A3910.unkC = arg0;
        D_800A2D10_A3910.unk14 = arg1;
        D_800A2D10_A3910.unk18 = 0x80;
        new_var = *new_var3;
        D_800A2D10_A3910.unk28 = arg4;
        D_800A2D10_A3910.unk10 = new_var + (arg2 << 0x18);
        osSendMesg(&D_800A2CD0_A38D0, (void *)2, 1);
        osRecvMesg(&D_800A2CF0_A38F0, &sp10, 1);
        D_800A2990_A3590->unk24[arg3] = (s32)sp10;
        D_800A2990_A3590->unk64[arg3] = (s16)arg0;
        func_800570BC_57CBC(D_800A2990_A3590);
        return;
    }

    osSendMesg(&D_800A2CD0_A38D0, (void *)3, 1);
    osRecvMesg(&D_800A2CF0_A38F0, &sp10, 1);
}

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
    GraphicsManager *temp_v1;
    s32 message;
    s32 *new_var2;
    s32 temp_v0;

    temp_v1 = D_800A2990_A3590;
    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = 0x80;
    D_800A2D10_A3910.unk18 = 0x80;

    new_var2 = &temp_v1->unk20;
    temp_v0 = *new_var2;
    D_800A2D10_A3910.unk10 = temp_v1->unk20;
    temp_v0 = temp_v1->unk24[arg1];
    D_800A2D10_A3910.unk28 = arg2;
    D_800A2D10_A3910.unk1C = temp_v0;

    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)2, 1);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)&message, 1);

    D_800A2990_A3590->unk24[arg1] = message;
    D_800A2990_A3590->unk64[arg1] = arg0;

    func_800570BC_57CBC();
}

void func_80058220_58E20(s32 arg0, s32 arg1) {
    func_80058154_58D54(arg0, arg1, 0xC);
}

void func_8005823C_58E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;
    GraphicsManager *new_var;
    s32 *new_var2;
    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = arg1;
    D_800A2D10_A3910.unk18 = 0x80;
    new_var = D_800A2990_A3590;
    new_var2 = &new_var->unk20;
    D_800A2D10_A3910.unk10 = (*new_var2) + (arg2 << 0x18);
    D_800A2D10_A3910.unk28 = arg3;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_800582C0_58EC0(s32 arg0, s32 arg1, s32 arg2) {
    func_8005823C_58E3C(arg0, arg1, arg2, 0xC);
}

void func_800582DC_58EDC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    void *message;

    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = arg1;
    D_800A2D10_A3910.unk18 = arg2;
    D_800A2D10_A3910.unk10 = D_800A2990_A3590->unk20 + (arg3 << 0x18);
    D_800A2D10_A3910.unk28 = arg4;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_80058360_58F60(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800582DC_58EDC(arg0, arg1, arg2, arg3, 0xC);
}

void func_80058380_58F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *new_var;
    void *message;
    s32 new_var2;
    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = arg1;
    D_800A2D10_A3910.unk18 = arg2;
    new_var = &D_800A2990_A3590->unk20;
    new_var2 = *new_var;
    D_800A2D10_A3910.unk28 = arg3;
    D_800A2D10_A3910.unk10 = new_var2;
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
    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = arg1;
    D_800A2D10_A3910.unk18 = 0x80;
    new_var = &D_800A2990_A3590->unk20;
    new_var2 = *new_var;
    D_800A2D10_A3910.unk28 = arg2;
    D_800A2D10_A3910.unk10 = new_var2;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_80058490_59090(s32 arg0, s32 arg1) {
    func_80058414_59014(arg0, arg1, 0xC);
}

void func_800584AC_590AC(s32 arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 mesg;

    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = 0x80;
    D_800A2D10_A3910.unk18 = 0x80;
    v0 = *(s32 *)((s32)D_800A2990_A3590 + 0x20);
    D_800A2D10_A3910.unk28 = arg2;
    D_800A2D10_A3910.unk10 = v0 + (arg1 << 24);

    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)&mesg, OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_80058530_59130(s32 arg0, s32 arg1) {
    func_800584AC_590AC(arg0, arg1, 0xC);
}

void func_8005854C_5914C(s32 arg0, s32 arg1) {
    s32 new_var2;
    s32 *new_var;
    void *sp10;

    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = 0x80;
    D_800A2D10_A3910.unk18 = 0x80;
    new_var2 = *(new_var = &D_800A2990_A3590->unk20);
    D_800A2D10_A3910.unk28 = arg1;
    D_800A2D10_A3910.unk10 = new_var2;
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
