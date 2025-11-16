#include "common.h"
#include "displaylist.h"
#include "overlay.h"
#include "player.h"
#include "task_scheduler.h"

USE_ASSET(_6A83F0);

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    void *unk1C;
    s32 unk20;
    f32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
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
    /* 0x0C */ void *unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 flags;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ void *unk24[0xF];
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
extern GraphicsManager *gGraphicsManager;

extern OSMesgQueue gfxTaskQueue;
extern OSMesgQueue gfxResultQueue;
extern s32 D_800937EC_943EC[];
extern s32 D_800937F0_943F0[];
extern s32 D_80093978_94578[];
extern s32 D_8009397C_9457C[];
extern s32 D_80093B00_94700;
extern s32 D_80092E28_93A28;
extern s32 D_80093308_93F08;

USE_ASSET(wavetables);

extern void *D_800A2CE8_A38E8;
extern void *D_800A2D08_A3908;
extern u8 D_80093B84_94784[];
extern u8 D_80093BA5_947A5;
extern u8 D_80093BA6_947A6;
extern OSThread D_800A2998_A3598;

void *func_80058638_59238(void);
void func_800570BC_57CBC(void);
void func_80057124_57D24(void);
void func_800579E8_585E8(void *, s32);
void func_80057B70_58770(s32, s32, s32, f32, s32, s32, s32);
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
void func_8005758C_5818C(void);
void func_800578DC_584DC(void *, s32);
void *func_80057A34_58634(void *);
void func_80057470_58070(void);
void *func_80057974_58574(void *arg0, void *arg1, s32 arg2);
void func_800572B0_57EB0(void *arg);
void func_80057928_58528(void *, s32);
void func_800573F8_57FF8(void);
void *func_800725F4_731F4(s32, s32, s32, s32, s32);
void *func_800728E0_734E0(s32);
void *func_80072960_73560(void *, s32);
void *func_80072A14_73614(void *);
void *func_80072CDC_738DC(void *, void *);
void func_80057614_58214(void *);
void func_80057214_57E14(void);
void func_800571D0_57DD0(void(callback)(void *));

typedef struct {
    void *start;
    void *end;
    s32 size;
} GraphicsDataSegment;
extern GraphicsDataSegment D_800937E8_943E8[];
extern GraphicsDataSegment D_80093974_94574[];

void func_80056080_56C80(void) {
    u8 sp10;

    gGraphicsManager = allocateTaskMemory(0x53C);
    gGraphicsManager->unk0 = dmaRequestAndUpdateStateWithSize(&_6A83F0_ROM_START, &_6A83F0_ROM_END, 0xCFD8);
    gGraphicsManager->unk8 = allocateMemoryNode(0, 0x9000, &sp10);
    gGraphicsManager->unk4 = allocateMemoryNode(0, 0x8000, &sp10);
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
    mgr = gGraphicsManager;
    config.fifo_length = 0x40;
    config.ptr = mgr->unk0;
    config.wbk = (u8 *)&wavetables_ROM_START;
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
    gGraphicsManager->unk1D = 0;
    gGraphicsManager->unk10 = negativeOne;
    gGraphicsManager->unk12 = 0xFFFF;
    gGraphicsManager->unk14 = 0;
    scheduleTask(func_800570E0_57CE0, 0, 0, 0x64);
    gGraphicsManager->unk20 = 0;
    for (i = 0xF; i >= 0; i--) {
        gGraphicsManager->unk24[i] = 0;
    }

    gGraphicsManager->unk534 = 0x20;
    gGraphicsManager->renderQueueCount = 0;
    gGraphicsManager->unk408 = 0;
    gGraphicsManager->unk538 = 0xC80;
    func_8005758C_5818C();
    func_8006983C_6A43C(&func_8005628C_56E8C);
}

INCLUDE_ASM("asm/nonmatchings/graphics", func_8005628C_56E8C);

void func_80056914_57514(void *source) {
    if (gGraphicsManager->unk408 < 8) {
        u8(*dest)[0x20];

        gGraphicsManager->bufferIds[gGraphicsManager->unk408] = -1;
        dest = &gGraphicsManager->bufferData[gGraphicsManager->unk408];
        dest++;
        dest--;

        memcpy(dest, source, 0x20);

        gGraphicsManager->bufferFlags[gGraphicsManager->unk408] = 0;
        gGraphicsManager->unk408 += 1;
    }
}

void func_80056990_57590(s32 arg0, s32 arg1) {
    gGraphicsManager->unk538 = arg1;
    gGraphicsManager->unk534 = arg0;
}

void func_800569A4_575A4(u8 *src_data, s8 search_id) {
    s32 i;
    void *bufferPtr;

    for (i = 0; i < gGraphicsManager->unk408; i++) {
        if (gGraphicsManager->bufferIds[i] == search_id) {
            bufferPtr = (void *)((i << 5) + (s32)gGraphicsManager + 0x40C);
            memcpy(bufferPtr, src_data, 0x20);
            return;
        }
    }

    // write to the next available buffer if there's space
    if (gGraphicsManager->unk408 < 8) {
        gGraphicsManager->bufferIds[gGraphicsManager->unk408] = search_id;
        bufferPtr = (void *)((gGraphicsManager->unk408 << 5) + (s32)gGraphicsManager + 0x40C);
        memcpy(bufferPtr, src_data, 0x20);
        gGraphicsManager->bufferFlags[gGraphicsManager->unk408] = 0;
        gGraphicsManager->unk408++;
    }
}

void setBufferData(void *source, u8 arg1, s32 arg2) {
    s32 i;
    s8 id;
    void *bufferPtr;

    // Search for existing buffer
    id = arg2;
    for (i = 0; i < gGraphicsManager->unk408; i++) {
        if (gGraphicsManager->bufferIds[i] == id) {
            bufferPtr = (void *)((i << 5) + (s32)gGraphicsManager + 0x40C);
            memcpy(bufferPtr, source, 0x20);
            return;
        }
    }

    // Add new buffer if space available
    if (gGraphicsManager->unk408 < 8) {
        gGraphicsManager->bufferIds[gGraphicsManager->unk408] = id;

        bufferPtr = (void *)(((gGraphicsManager->unk408) << 5) + (s32)gGraphicsManager + 0x40C);

        memcpy(bufferPtr, source, 0x20);

        gGraphicsManager->bufferFlags[gGraphicsManager->unk408] = arg1;
        gGraphicsManager->unk408++;
    }
}

void func_80056B7C_5777C(void *arg0, s16 arg1) {
    s32 index = gGraphicsManager->renderQueueCount;
    if (index < 0x20) {
        RenderQueueItem *renderQueue = gGraphicsManager->renderQueue;
        memcpy(renderQueue[index].data, arg0, 0xC);
        index = arg1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk0 = index;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk2 = -1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk4 = 4;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk6 = 0;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount++;
    }
}

void func_80056C6C_5786C(void *arg0, unsigned int arg1, s16 arg2) {
    if (gGraphicsManager->renderQueueCount < 0x20) {
        RenderQueueItem *arrayCopy = gGraphicsManager->renderQueue;

        memcpy(arrayCopy[gGraphicsManager->renderQueueCount].data, arg0, 0xC);

        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk0 = arg1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk2 = arg2;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk4 = 4;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk6 = 0;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount++;
    }
}

void func_80056D64_57964(void *arg0, s16 arg1, s16 arg2, s16 arg3) {
    RenderQueueItem *renderQueue;
    s32 index = gGraphicsManager->renderQueueCount;
    if (index < 0x20) {
        renderQueue = gGraphicsManager->renderQueue;
        memcpy(renderQueue[index].data, arg0, 0xC);
        index = arg1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk0 = index;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk2 = arg3;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk4 = arg2;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk6 = 0;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount += 1;
    }
}

void func_80056E64_57A64(void *arg0, int arg1, f32 arg2, s16 arg3, s32 arg4) {
    RenderQueueItem(*dest)[32];
    if (gGraphicsManager->renderQueueCount < 0x20) {
        dest = &gGraphicsManager->renderQueue;
        memcpy((*dest)[gGraphicsManager->renderQueueCount].data, arg0, 0xC);
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk0 = arg1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk2 = (s16)arg4;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk4 = arg3;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk6 = 1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk8 = arg2;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount++;
    }
}

void func_80056F8C_57B8C(void *arg0, int arg1, f32 arg2, s16 arg3, s32 arg4, s32 arg5) {
    s32 index = gGraphicsManager->renderQueueCount;
    if (index < 0x20) {
        RenderQueueItem *renderQueue = gGraphicsManager->renderQueue;
        memcpy(renderQueue[index].data, arg0, 0xC);
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk0 = arg1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk2 = (s16)arg4;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk4 = arg3;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk6 = 1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].unk8 = arg2;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = (s16)arg5;
        gGraphicsManager->renderQueueCount++;
    }
}

void func_800570BC_57CBC(void) {
    gGraphicsManager->unk20 = (s32)((gGraphicsManager->unk20 + 1) & 0xFFFFFF);
}

void func_800570E0_57CE0(void *arg) {
    setCleanupCallback(func_800570E0_57CE0);
    if (gGraphicsManager->unk1D != 0) {
        setCallback(&func_80057124_57D24);
    }
}

void func_80057124_57D24(void) {
    gGraphicsManager->unk12 = gGraphicsManager->unk10;

    loadDataSegment(
        D_800937E8_943E8[gGraphicsManager->unk12].start,
        D_800937E8_943E8[gGraphicsManager->unk12].end,
        D_800937E8_943E8[gGraphicsManager->unk12].size,
        gGraphicsManager->unk8
    );

    loadDataSegment(
        D_80093974_94574[gGraphicsManager->unk12].start,
        D_80093974_94574[gGraphicsManager->unk12].end,
        D_80093974_94574[gGraphicsManager->unk12].size,
        gGraphicsManager->unk4
    );

    setCallback(&func_800571D0_57DD0);
}

void func_800571D0_57DD0(void(callback)(void *)) {
    func_800579E8_585E8(gGraphicsManager->unk4, *(&D_80093B00_94700 + (gGraphicsManager->unk12)));
    setCallback(&func_80057214_57E14);
}

void func_80057214_57E14(void) {
    s16 temp_a1;
    void *temp_v0;

    if ((u16)gGraphicsManager->unk10 != gGraphicsManager->unk12) {
        setCallbackWithContinue(func_800570E0_57CE0);
    } else if (gGraphicsManager->unk1D == 2) {
        temp_v0 = func_80057974_58574(gGraphicsManager->unk4, gGraphicsManager->unk8, (u8)gGraphicsManager->unk1C);
        gGraphicsManager->unkC = temp_v0;
        if (temp_v0 != 0) {
            temp_a1 = gGraphicsManager->unk16;
            if (temp_a1 != 0x80) {
                func_80057928_58528(temp_v0, temp_a1);
            }
            gGraphicsManager->unk1E = 0;
            setCallbackWithContinue(func_800572B0_57EB0);
        }
    }
}

void func_800572B0_57EB0(void *arg) {
    s16 temp_a1;

    temp_a1 = gGraphicsManager->flags;
    if (gGraphicsManager->unk16 != temp_a1) {
        if ((u16)gGraphicsManager->unk1A != 0) {
            gGraphicsManager->unk16 = gGraphicsManager->unk16 +
                                      ((s32)(temp_a1 - gGraphicsManager->unk16) / (s32)(u16)gGraphicsManager->unk1A);
            func_80057928_58528(gGraphicsManager->unkC, (s32)gGraphicsManager->unk16);
            gGraphicsManager->unk1A = (u16)gGraphicsManager->unk1A - 1;
        } else {
            gGraphicsManager->unk16 = temp_a1;
            func_80057928_58528(gGraphicsManager->unkC, (s32)temp_a1);
        }
    }

    if ((u16)gGraphicsManager->unk10 != gGraphicsManager->unk12) {
        gGraphicsManager->unk1E = 1;
        func_800578DC_584DC(gGraphicsManager->unkC, 8);
        setCallbackWithContinue(func_800573F8_57FF8);
    } else {
        if (gGraphicsManager->unk1D == 0) {
            gGraphicsManager->unk1E = 1;
            func_800578DC_584DC(gGraphicsManager->unkC, (s32)(u16)gGraphicsManager->unk14);
            setCallbackWithContinue(func_800573F8_57FF8);
        }
        if (func_80057A34_58634(gGraphicsManager->unkC) == NULL) {
            gGraphicsManager->unk1D = 0;
            setCallbackWithContinue(func_80057470_58070);
        }
    }
}

void func_800573F8_57FF8(void) {
    if (func_80057A34_58634(gGraphicsManager->unkC) == 0) {
        setCallbackWithContinue(&func_80057470_58070);
        return;
    }
    if ((gGraphicsManager->unk1D != 0) && (gGraphicsManager->unk1E == 0)) {
        gGraphicsManager->unk1E = 1;
        func_800578DC_584DC(gGraphicsManager->unkC, 8);
    }
}

void func_80057470_58070(void) {
    if (func_80058638_59238() == NULL) {
        setCallbackWithContinue(&func_800570E0_57CE0);
    }
}

void func_800574A0_580A0(s32 arg0) {
    gGraphicsManager->unk1D = 2;
    gGraphicsManager->unk10 = arg0;
    gGraphicsManager->unk16 = 0x80;
    gGraphicsManager->flags = 0x80;
    gGraphicsManager->unk1A = 0;
    gGraphicsManager->unk1C = (s8)D_80093B84_94784[arg0];
}

void func_800574E0_580E0(s16 arg0, s8 arg1) {
    gGraphicsManager->unk1D = 2;
    gGraphicsManager->unk10 = arg0;
    gGraphicsManager->unk16 = 0x80;
    gGraphicsManager->flags = 0x80;
    gGraphicsManager->unk1A = 0;
    gGraphicsManager->unk1C = arg1;
}

void func_80057514_58114(u32 arg0, s16 arg1, s16 arg2) {
    GraphicsManager *new_var;
    gGraphicsManager->unk1D = 2;
    gGraphicsManager->unk10 = arg0;
    gGraphicsManager->unk16 = 0;
    gGraphicsManager->flags = arg1;
    gGraphicsManager->unk1A = arg2;
    gGraphicsManager->unk1C = D_80093B84_94784[arg0];
}

void func_80057550_58150(u16 arg0, u16 arg1) {
    gGraphicsManager->flags = arg0;
    gGraphicsManager->unk1A = arg1;
}

void func_80057564_58164(s32 arg0) {
    int var_a0;
    GraphicsManager *new_var;
    var_a0 = arg0;
    if (arg0 < 8) {
        var_a0 = 8;
    }
    gGraphicsManager->unk1D = 0;
    (new_var = gGraphicsManager)->unk14 = var_a0;
}

void func_8005758C_5818C(void) {
    OSMesgQueue *queue = &gfxTaskQueue;
    osCreateMesgQueue(queue, &D_800A2CE8_A38E8, OS_MESG_BLOCK);
    queue = &gfxResultQueue;
    osCreateMesgQueue(queue, &D_800A2D08_A3908, OS_MESG_BLOCK);
    osCreateThread(&D_800A2998_A3598, 0xB, func_80057614_58214, 0, &gfxTaskQueue, 6);
    osStartThread(&D_800A2998_A3598);
}

void func_80057614_58214(void *arg0) {
    void *message;
    void *result;

    while (TRUE) {
        osRecvMesg(&gfxTaskQueue, &message, 1);
        if ((u32)message < 0xF) {
            switch ((s32)message) {
                case 0:
                    result = func_800728E0_734E0(3);
                    break;
                case 1:
                    result = func_800725F4_731F4(
                        D_800A2D10_A3910.unkC,
                        D_800A2D10_A3910.unk14,
                        D_800A2D10_A3910.unk18,
                        0,
                        D_800A2D10_A3910.unk10
                    );
                    if (D_800A2D10_A3910.unk28 != 0) {
                        func_80072C38_73838(result, D_800A2D10_A3910.unk28);
                    }
                    break;
                case 2:
                    func_80072960_73560(D_800A2D10_A3910.unk1C, 0);
                    result = func_800725F4_731F4(
                        D_800A2D10_A3910.unkC,
                        D_800A2D10_A3910.unk14,
                        D_800A2D10_A3910.unk18,
                        0,
                        D_800A2D10_A3910.unk10
                    );
                    if (D_800A2D10_A3910.unk28 != 0) {
                        func_80072C38_73838(result, D_800A2D10_A3910.unk28);
                    }
                    break;
                case 3:
                    result = func_80072960_73560(D_800A2D10_A3910.unk1C, 0);
                    break;
                case 4:
                    func_80072960_73560(D_800A2D10_A3910.unk1C, 0);
                    result = func_800725F4_731F4(
                        D_800A2D10_A3910.unkC,
                        D_800A2D10_A3910.unk14,
                        D_800A2D10_A3910.unk18,
                        0,
                        D_800A2D10_A3910.unk10
                    );
                    func_80072B3C_7373C(result, D_800A2D10_A3910.unk24);
                    if (D_800A2D10_A3910.unk28 != 0) {
                        func_80072C38_73838(result, D_800A2D10_A3910.unk28);
                    }
                    break;
                case 5:
                    func_80072A6C_7366C(D_800A2D10_A3910.unk1C, D_800A2D10_A3910.unk14);
                    func_80072ACC_736CC(D_800A2D10_A3910.unk1C, D_800A2D10_A3910.unk18);
                    func_80072B3C_7373C(D_800A2D10_A3910.unk1C, D_800A2D10_A3910.unk24);
                    if (D_800A2D10_A3910.unk28 != 0) {
                        func_80072C38_73838(D_800A2D10_A3910.unk1C, D_800A2D10_A3910.unk28);
                    }
                    break;
                case 6:
                    func_80072814_73414(1, D_800A2D10_A3910.unk20);
                    break;
                case 7:
                    result = func_80072960_73560(D_800A2D10_A3910.unk1C, (s32)D_800A2D10_A3910.unk20);
                    break;
                case 8:
                    result = func_800728E0_734E0(1);
                    break;
                case 9:
                    result = func_80072A14_73614(D_800A2D10_A3910.unk1C);
                    break;
                case 10:
                    func_80072CC0_738C0(D_800A2D10_A3910.unk4, D_800A2D10_A3910.unk8);
                    break;
                case 11:
                    result = func_80072CDC_738DC(D_800A2D10_A3910.unk4, D_800A2D10_A3910.unk0);
                    if (D_800A2D10_A3910.unk28 != 0) {
                        func_80072C38_73838(result, D_800A2D10_A3910.unk28);
                    }
                    break;
                case 12:
                    func_80072A6C_7366C(D_800A2D10_A3910.unk1C, D_800A2D10_A3910.unk14);
                    break;
                case 13:
                    func_80072814_73414(3, D_800A2D10_A3910.unk20);
                    break;
                case 14:
                    result = func_800728E0_734E0(2);
                    break;
            }
        }

        osSendMesg(&gfxResultQueue, result, OS_MESG_BLOCK);
    }
}

void func_80057870_58470(s32 arg0) {
    void *message;

    if ((D_80093BA5_947A5 != NULL) && (D_80093BA6_947A6 == NULL)) {
        D_80093BA6_947A6++;
        D_800A2D10_A3910.unk20 = arg0;
        osSendMesg(&gfxTaskQueue, (OSMesg *)0xD, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    }
}

void func_800578DC_584DC(void *arg0, s32 arg1) {
    void *message;

    D_800A2D10_A3910.unk1C = arg0;
    D_800A2D10_A3910.unk20 = arg1;
    osSendMesg(&gfxTaskQueue, (OSMesg *)7, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void func_80057928_58528(void *arg0, s32 arg1) {
    void *message;

    D_800A2D10_A3910.unk1C = arg0;
    D_800A2D10_A3910.unk14 = arg1;
    osSendMesg(&gfxTaskQueue, (OSMesg *)0xC, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void *func_80057974_58574(void *arg0, void *arg1, s32 arg2) {
    void *sp10;

    D_800A2D10_A3910.unk4 = arg0;
    D_800A2D10_A3910.unk0 = arg1;
    D_800A2D10_A3910.unk28 = arg2;
    osSendMesg(&gfxTaskQueue, (void *)0xB, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &sp10, OS_MESG_BLOCK);
    return sp10;
}

void *func_800579CC_585CC(void *arg0, void *arg1) {
    return func_80057974_58574(arg0, arg1, 0);
}

void func_800579E8_585E8(void *arg0, s32 arg1) {
    void *message;

    D_800A2D10_A3910.unk4 = arg0;
    D_800A2D10_A3910.unk8 = arg1;
    osSendMesg(&gfxTaskQueue, (OSMesg *)0xA, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void *func_80057A34_58634(void *arg0) {
    void *message;

    D_800A2D10_A3910.unk1C = arg0;
    osSendMesg(&gfxTaskQueue, (OSMesg *)9, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    return message;
}

void *func_80057A7C_5867C(void) {
    void *message;

    osSendMesg(&gfxTaskQueue, (OSMesg *)8, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    return message;
}

void func_80057ABC_586BC(s32 arg0, s32 arg1) {
    void *temp;
    OSMesg message;

    temp = gGraphicsManager->unk24[arg0];
    D_800A2D10_A3910.unk20 = arg1;
    D_800A2D10_A3910.unk1C = temp;
    osSendMesg(&gfxTaskQueue, (OSMesg *)7, 1);
    osRecvMesg(&gfxResultQueue, &message, 1);
}

void func_80057B1C_5871C(s32 arg0) {
    void *message;

    D_800A2D10_A3910.unk20 = arg0;
    osSendMesg(&gfxTaskQueue, (OSMesg *)6, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    gGraphicsManager->renderQueueCount = 0;
    gGraphicsManager->unk408 = 0;
}

void func_80057B70_58770(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    void *message;

    D_800A2D10_A3910.unk1C = gGraphicsManager->unk24[arg5];
    if (arg1 > 0) {
        D_800A2D10_A3910.unk18 = arg2;
        D_800A2D10_A3910.unk14 = arg1;
        D_800A2D10_A3910.unk24 = arg3;
        D_800A2D10_A3910.unk28 = arg6;

        if (gGraphicsManager->unk24[arg5] == 0 || gGraphicsManager->unk64[arg5] != arg0) {
            D_800A2D10_A3910.unkC = arg0;
            D_800A2D10_A3910.unk10 = gGraphicsManager->unk20 + (arg4 << 0x18);
            osSendMesg(&gfxTaskQueue, (void *)4, 1);
            osRecvMesg(&gfxResultQueue, &message, 1);
            gGraphicsManager->unk24[arg5] = message;
            gGraphicsManager->unk64[arg5] = (s16)arg0;
            func_800570BC_57CBC();
            return;
        }

        osSendMesg(&gfxTaskQueue, (OSMesg *)5, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    } else {
        osSendMesg(&gfxTaskQueue, (OSMesg *)3, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    }
}

void func_80057CB4_588B4(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5) {
    func_80057B70_58770(arg0, arg1, arg2, arg3, arg4, arg5, 0xC);
}

void func_80057CE4_588E4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    void *message;

    D_800A2D10_A3910.unk1C = gGraphicsManager->unk24[arg4];
    if (arg1 > 0) {
        D_800A2D10_A3910.unkC = arg0;
        D_800A2D10_A3910.unk14 = arg1;
        D_800A2D10_A3910.unk18 = arg2;
        D_800A2D10_A3910.unk28 = arg5;
        D_800A2D10_A3910.unk10 = gGraphicsManager->unk20 + (arg3 << 0x18);
        osSendMesg(&gfxTaskQueue, (void *)2, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
        gGraphicsManager->unk24[arg4] = message;
        gGraphicsManager->unk64[arg4] = (s16)arg0;
        func_800570BC_57CBC();
        return;
    }
    osSendMesg(&gfxTaskQueue, (void *)3, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void func_80057DF0_589F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80057CE4_588E4(arg0, arg1, arg2, arg3, arg4, 0xC);
}

void func_80057E18_58A18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 new_var;
    s32 *new_var3;
    void *sp10;

    D_800A2D10_A3910.unk1C = gGraphicsManager->unk24[arg3];
    new_var3 = &gGraphicsManager->unk20;

    if (arg1 > 0) {
        D_800A2D10_A3910.unkC = arg0;
        D_800A2D10_A3910.unk14 = arg1;
        D_800A2D10_A3910.unk18 = 0x80;
        new_var = *new_var3;
        D_800A2D10_A3910.unk28 = arg4;
        D_800A2D10_A3910.unk10 = new_var + (arg2 << 0x18);
        osSendMesg(&gfxTaskQueue, (void *)2, 1);
        osRecvMesg(&gfxResultQueue, &sp10, 1);
        gGraphicsManager->unk24[arg3] = sp10;
        gGraphicsManager->unk64[arg3] = (s16)arg0;
        func_800570BC_57CBC();
        return;
    }

    osSendMesg(&gfxTaskQueue, (void *)3, 1);
    osRecvMesg(&gfxResultQueue, &sp10, 1);
}

void func_80057F28_58B28(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80057E18_58A18(arg0, arg1, arg2, arg3, 0xC);
}

void func_80057F48_58B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;

    D_800A2D10_A3910.unk1C = gGraphicsManager->unk24[arg2];
    if (arg1 > 0) {
        D_800A2D10_A3910.unkC = arg0;
        D_800A2D10_A3910.unk14 = arg1;
        D_800A2D10_A3910.unk18 = 0x80;
        D_800A2D10_A3910.unk10 = gGraphicsManager->unk20;
        D_800A2D10_A3910.unk28 = arg3;
        osSendMesg(&gfxTaskQueue, (void *)2, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
        gGraphicsManager->unk24[arg2] = message;
        gGraphicsManager->unk64[arg2] = (s16)arg0;
        func_800570BC_57CBC();
        return;
    }
    osSendMesg(&gfxTaskQueue, (void *)3, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void func_80058048_58C48(s32 arg0, s32 arg1, s32 arg2) {
    func_80057F48_58B48(arg0, arg1, arg2, 0xC);
}

void func_80058064_58C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;

    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = 0x80;
    D_800A2D10_A3910.unk18 = 0x80;
    D_800A2D10_A3910.unk10 = gGraphicsManager->unk20 + (arg1 << 0x18);
    D_800A2D10_A3910.unk1C = gGraphicsManager->unk24[arg2];
    D_800A2D10_A3910.unk28 = arg3;
    osSendMesg(&gfxTaskQueue, (OSMesg *)2, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    gGraphicsManager->unk24[arg2] = message;
    gGraphicsManager->unk64[arg2] = (s16)arg0;
    func_800570BC_57CBC();
}

void func_80058138_58D38(s32 arg0, s32 arg1, s32 arg2) {
    func_80058064_58C64(arg0, arg1, arg2, 0xC);
}

void func_80058154_58D54(s32 arg0, s32 arg1, s32 arg2) {
    GraphicsManager *temp_v1;
    void *message;
    s32 *new_var2;
    void *temp_v0;

    temp_v1 = gGraphicsManager;
    D_800A2D10_A3910.unkC = arg0;
    D_800A2D10_A3910.unk14 = 0x80;
    D_800A2D10_A3910.unk18 = 0x80;

    new_var2 = &temp_v1->unk20;
    temp_v0 = (void *)*new_var2;
    D_800A2D10_A3910.unk10 = temp_v1->unk20;
    temp_v0 = temp_v1->unk24[arg1];
    D_800A2D10_A3910.unk28 = arg2;
    D_800A2D10_A3910.unk1C = temp_v0;

    osSendMesg(&gfxTaskQueue, (OSMesg *)2, 1);
    osRecvMesg(&gfxResultQueue, (OSMesg *)&message, 1);

    gGraphicsManager->unk24[arg1] = message;
    gGraphicsManager->unk64[arg1] = arg0;

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
    new_var = gGraphicsManager;
    new_var2 = &new_var->unk20;
    D_800A2D10_A3910.unk10 = (*new_var2) + (arg2 << 0x18);
    D_800A2D10_A3910.unk28 = arg3;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
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
    D_800A2D10_A3910.unk10 = gGraphicsManager->unk20 + (arg3 << 0x18);
    D_800A2D10_A3910.unk28 = arg4;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
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
    new_var = &gGraphicsManager->unk20;
    new_var2 = *new_var;
    D_800A2D10_A3910.unk28 = arg3;
    D_800A2D10_A3910.unk10 = new_var2;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
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
    new_var = &gGraphicsManager->unk20;
    new_var2 = *new_var;
    D_800A2D10_A3910.unk28 = arg2;
    D_800A2D10_A3910.unk10 = new_var2;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, (OSMesg *)(&sp10), OS_MESG_BLOCK);
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
    v0 = *(s32 *)((s32)gGraphicsManager + 0x20);
    D_800A2D10_A3910.unk28 = arg2;
    D_800A2D10_A3910.unk10 = v0 + (arg1 << 24);

    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, (OSMesg *)&mesg, OS_MESG_BLOCK);
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
    new_var2 = *(new_var = &gGraphicsManager->unk20);
    D_800A2D10_A3910.unk28 = arg1;
    D_800A2D10_A3910.unk10 = new_var2;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_800585C8_591C8(s32 arg0) {
    func_8005854C_5914C(arg0, 0xC);
}

void *func_800585E4_591E4(void) {
    void *result;

    if (D_80093BA5_947A5 != 0) {
        osSendMesg(&gfxTaskQueue, 0, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &result, OS_MESG_BLOCK);
        return result;
    }

    return 0;
}

void *func_80058638_59238(void) {
    void *message;

    if (D_80093BA5_947A5 != NULL) {
        osSendMesg(&gfxTaskQueue, (OSMesg *)0xE, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);

        return message;
    }

    return NULL;
}
