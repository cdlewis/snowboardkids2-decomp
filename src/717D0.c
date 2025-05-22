#include "70DB0.h"
#include "common.h"
#include "ucode.h"

typedef struct
{
    void *buf;
    s16 len;
} AudioBuffer;
typedef struct
{
    /* 0x00 */ void *outputBuffer;
    /* 0x04 */ s16 frameSizeInSamples;
    s16 _pad6;
    /* 0x08 */ s32 taskType;
    /* 0x0C */ s32 taskFlags;
    /* 0x10 */ void *bootCode;
    /* 0x14 */ s32 bootCodeSize;
    /* 0x18 */ void *taskCode;
    /* 0x1C */ s32 taskCodeSize;
    /* 0x20 */ void *dataMemory;
    /* 0x24 */ s32 dataMemorySize;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    /* 0x38 */ Acmd *commandBuffer;
    /* 0x3C */ s32 commandBufferSize;
    s32 unk40;
    s32 unk44;
    void *unk48;
    void *unk4C;
    s32 unk50;
} AudioStruct;

typedef struct AudioNode_s {
    ALLink l;
    s32 data;
    s32 timestamp;
} AudioNode;

typedef struct {
    u16 type;
    u16 pad;
    void *data;
} Msg;

extern Acmd *gAudioCmdBuffers[];
extern ALGlobals __libmus_alglobals;
extern AudioNode gActiveListHead;
extern OSMesgQueue gAudioMsgQueue;
extern OSMesgQueue D_800A8F58_A02C8;
extern s32 D_8009B044_9BC44;
extern s32 D_800A6468_A7068;
extern s32 D_800A6498_A7098;
extern s32 gAudioBufferPadding;
extern s32 gAudioBufferSize;
extern s32 gAudioCmdBufferToggle;
extern s32 gAudioDataMemory;
extern u32 gCurrentFrame;
extern u32 D_8009B034_9BC34;
extern u32 gMinAudioFrameSize;
extern u8 diskrom_handle;
extern void *gPendingMessages;
extern void addViConfig(ViConfig *, OSMesgQueue *, s32);
extern void func_80071294_71E94(void *);
extern void func_800712D4_71ED4();
extern void processAudioNodeList(void);
s32 audioCreateAndScheduleTask(AudioStruct *, AudioBuffer *);
void sendMessageToEventQueue2(OSMesg);

INCLUDE_ASM("asm/nonmatchings/717D0", __MusIntAudManInit);

void func_80071000_71C00(void) {
    ViConfig cfg;
    OSMesg msg;
    OSMesg msg2;
    OSMesgQueue *q = &D_800A8F58_A02C8;
    s32 quit = 0;
    addViConfig(&cfg, q, 1);
    while (quit == 0) {
        osRecvMesg(q, &msg, 1);
        osRecvMesg(q, 0, 0 * 0);
        if (((Msg *)(&msg))->type == 5) {
            void **tbl = (void **)(((u8 *)q) - 0x1C0);
            void *ptr = tbl[gCurrentFrame & 3];
            if (audioCreateAndScheduleTask(ptr, gPendingMessages)) {
                osRecvMesg((OSMesgQueue *)(((u8 *)q) + 0x38), &msg2, 1);
                ;
                func_80071294_71E94(*((void **)(((u8 *)msg2) + 4)));
                gPendingMessages = *((void **)(((u8 *)msg2) + 4));
            }
        }
    }

    alClose(&__libmus_alglobals);
}

s32 audioCreateAndScheduleTask(AudioStruct *audioTaskDesc, AudioBuffer *prevBuffer) {
    s32 commandLength;
    s16 *outputBuffer;
    s32 commandBufferSize;
    Acmd *commandBuffer;
    Acmd *commandBufferStart;
    Acmd *commandBufferEnd;
    s32 samplesToProcess;
    s32 currentSamplesInBuffer;

    processAudioNodeList();
    outputBuffer = (s16 *)osVirtualToPhysical(audioTaskDesc->outputBuffer);
    if (prevBuffer != 0) {
        osAiSetNextBuffer(prevBuffer->buf, prevBuffer->len * 4);
    }

    currentSamplesInBuffer = osAiGetLength() / 4;
    samplesToProcess = (((gAudioBufferSize - currentSamplesInBuffer) + gAudioBufferPadding) + 16) & 0xFFF0;

    audioTaskDesc->frameSizeInSamples = samplesToProcess;
    if ((s16)samplesToProcess < gMinAudioFrameSize) {
        audioTaskDesc->frameSizeInSamples = gMinAudioFrameSize;
    }

    commandBuffer = gAudioCmdBuffers[gAudioCmdBufferToggle];
    commandBufferEnd = alAudioFrame(commandBuffer, &commandLength, (void *)outputBuffer, audioTaskDesc->frameSizeInSamples);
    if (commandLength == 0) {
        return 0;
    }

    audioTaskDesc->unk48 = &gAudioCmdBuffers[0x80];
    audioTaskDesc->unk4C = &audioTaskDesc->unk50;
    audioTaskDesc->commandBuffer = gAudioCmdBuffers[gAudioCmdBufferToggle];
    commandBufferStart = gAudioCmdBuffers[gAudioCmdBufferToggle];
    commandBufferSize = commandBufferEnd - commandBufferStart;
    commandBufferSize = commandBufferSize * 8;

    audioTaskDesc->taskType = M_AUDTASK;
    audioTaskDesc->bootCode = rspbootTextStart;
    audioTaskDesc->bootCodeSize = (u32)rspbootTextEnd - (u32)rspbootTextStart;
    audioTaskDesc->taskCode = aspMainTextStart;
    audioTaskDesc->taskFlags = 0;
    audioTaskDesc->dataMemory = &gAudioDataMemory;
    audioTaskDesc->dataMemorySize = 0x800;
    audioTaskDesc->unk28 = 0;
    audioTaskDesc->unk2C = 0;
    audioTaskDesc->unk30 = 0;
    audioTaskDesc->unk34 = 0;
    audioTaskDesc->unk40 = 0;
    audioTaskDesc->unk44 = 0;
    audioTaskDesc->commandBufferSize = commandBufferSize;

    sendMessageToEventQueue2(((OSMesg)&audioTaskDesc->taskType));

    gAudioCmdBufferToggle ^= 1;

    return 1;
}

void func_80071294_71E94(void *arg0) {
    if (((osAiGetLength() >> 2) == 0) && (D_8009B044_9BC44 == 0)) {
        D_8009B044_9BC44 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/717D0", func_800712D4_71ED4);

void *func_800714D8_720D8(void *arg0) {
    u8 *handle = (u8 *)&diskrom_handle;

    if (*handle == 0) {
        u32 value = D_800A6498_A7098;
        gActiveListHead.l.next = NULL;
        *handle = 1;
        D_800A6468_A7068 = value;
    }

    *(void **)arg0 = handle;
    return &func_800712D4_71ED4;
}

void processAudioNodeList(void) {
    void *message;
    u32 i;
    AudioNode *nextNode;
    AudioNode *node;
    AudioNode *headPtr;

    for (i = 0; i < D_8009B034_9BC34; i++) {
        osRecvMesg(&gAudioMsgQueue, &message, 0);
    }

    headPtr = &gActiveListHead;
    node = (AudioNode *)gActiveListHead.l.next;
    while (node != NULL) {
        nextNode = (AudioNode *)node->l.next;
        if ((node->timestamp + 1) >= gCurrentFrame) {
            node = nextNode;
        } else {
            if ((&gActiveListHead)->l.next == ((ALLink *)node)) {
                (&gActiveListHead)->l.next = (ALLink *)nextNode;
            }
            alUnlink((ALLink *)node);
            if ((&gActiveListHead)->l.prev != 0) {
                alLink((ALLink *)node, (&gActiveListHead)->l.prev);
            } else {
                (&gActiveListHead)->l.prev = (ALLink *)node;
                node->l.next = 0;
                node->l.prev = 0;
            }
            node = nextNode;
        }
    }

    D_8009B034_9BC34 = 0;
    gCurrentFrame += 1;
}
