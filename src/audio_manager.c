#include "common.h"
#include "sched.h"
#include "thread_manager.h"
#include "ucode.h"

typedef struct {
    void *buf;
    s16 len;
} AudioBuffer;
typedef struct {
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
    void *dest_ptr;
} AudioNode;

typedef struct {
    u16 type;
    u16 pad;
    void *data;
} Msg;

typedef union {
    struct {
        short type;
    } gen;

    struct {
        short type;
        struct AudioInfo_s *info;
    } done;

    OSScMsg app;

} AudioMsg;

typedef struct AudioInfo_s {
    short *data;
    u8 padding[0x4C];
    short frameSamples;
    void *msg;
    u8 padding2[0x18];
} AudioInfo;

typedef struct {
    AudioInfo *audioInfo[4];
    OSThread thread;
    OSMesgQueue audioReplyMsgQ;
    OSMesg audioReplyMsgBuf[8];
    OSMesgQueue audioFrameMsgQ;
    OSMesg audioFrameMsgBuf[8];
} AMAudioMgr;

extern AMAudioMgr gAudioManager;

extern Acmd *gAudioCmdBuffers[];
extern ALGlobals __libmus_alglobals;
extern AudioNode *D_800A6468_A7068;
extern AudioNode *gActiveListHead;
extern AudioNode *gAudioNodePool;
extern int loadAudioDataWithCache(s32 /*romAddr*/, s32 /*requestSize*/);
extern OSIoMesg *gAudioDmaMessages;
extern OSMesgQueue gAudioMsgQueue;
extern OSPiHandle *gCartRomHandle;
extern OSPiHandle *gDriveRomHandle;
extern s32 D_800A6474_A7074;
extern s32 __muscontrol_flag;
extern s32 gAudioBufferPadding;
extern s32 gAudioBufferSize;
extern s32 gAudioRspCmdCount;
extern u32 gMaxVoices;
extern u32 gMinAudioFrameSize;
extern u8 gDriveRomInitialized;
extern void **gAudioMsgBuffer;
extern void addViConfig(ViConfig *, OSMesgQueue *, s32);
extern void handleAudioUnderrun(void * /*arg0*/);
extern void processAudioNodeList(void);
s32 audioCreateAndScheduleTask(AudioStruct *, AudioBuffer *);
void *initAudioDriveAndGetLoader(void *arg0);
void audioManagerThread(void * /*arg*/);
void sendMessageToEventQueue2(OSMesg);

typedef struct AudioParams_s {
    u32 syn_output_rate;
    s32 sample_rate;
    s32 syn_rsp_cmds;
} AudioParams;

// data
u32 gCurrentFrame = 0;
u32 D_8009B034_9BC34 = 0;
s32 gAudioCmdBufferToggle = 0;
s32 gAudioThreadCreated = 0;
void *gPendingMessages = NULL;
s32 gAudioUnderrunFlag = 1;

// rodata
// const f64 D_8009EA60_9F660 = 4294967296;
// const f64 D_8009EA68_9F668 = 4294967296;

INCLUDE_ASM("asm/nonmatchings/audio_manager", initAudioManager);
/*
void initAudioManager(ALSynConfig *config, OSId id, AudioParams *audioParams, s32 maxChannels, s32 maxVoices, s32
sampleRate) { u32 *new_var; u32 i; f64 tempDouble; f32 tempFloat; f32 divResult; OSPiHandle *tempDriveRomHandle; u32
outputRate;

    gCartRomHandle = osCartRomInit();
    tempDriveRomHandle = osDriveRomInit();
    gDriveRomInitialized = FALSE;
    new_var = &audioParams->syn_output_rate;
    outputRate = *new_var;
    gDriveRomHandle = tempDriveRomHandle;

    gMaxVoices = maxVoices;

    if (outputRate < 0x5622) {
        gAudioBufferPadding = 0x50;
    } else {
        gAudioBufferPadding = 0xF0;
    }

    config->dmaproc = &initAudioDriveAndGetLoader;
    config->outputRate = osAiSetFrequency(audioParams->syn_output_rate);
    gAudioNodePool = (AudioNode *)alHeapAlloc(config->heap, 1, sizeof(AudioNode) * maxChannels);
    gAudioDmaMessages = (OSIoMesg *)alHeapAlloc(config->heap, 1, (sizeof(OSIoMesg) * maxChannels) * 2);
    gAudioMsgBuffer = (void **)alHeapAlloc(config->heap, 1, (sizeof(void **) * maxChannels) * 2);

    // Calculate audio buffer size
    tempDouble = (f64)audioParams->sample_rate;
    if (audioParams->sample_rate < 0) {
        tempDouble += D_8009EA60_9F660;
    }
    tempFloat = (f32)tempDouble;
    tempFloat = tempFloat * (f32)config->outputRate;
    divResult = tempFloat / (f32)sampleRate;
    gAudioBufferSize = divResult;

    // Verify audio buffer size
    tempDouble = (f64)gAudioBufferSize;
    if (gAudioBufferSize < 0) {
        tempDouble += D_8009EA68_9F668;
    }
    if ((f32)tempDouble < divResult) {
        gAudioBufferSize++;
    }

    // Align buffer size to 16-byte boundary
    if (gAudioBufferSize & 0xF) {
        gAudioBufferSize = (gAudioBufferSize & (~0xF)) + 0x10;
    }

    gMinAudioFrameSize = gAudioBufferSize - 0x10;
    D_800A6474_A7074 = (gAudioBufferSize + gAudioBufferPadding) + 0x10;

    CustomInit(&__libmus_alglobals, (ALSynConfig *)config);

    gAudioNodePool->l.prev = NULL;
    gAudioNodePool->l.next = NULL;
    for (i = 0; i < (maxChannels - 1); i++) {
        alLink((ALLink *)(&gAudioNodePool[i + 1]), (ALLink *)(&gAudioNodePool[i]));
        gAudioNodePool[i].dest_ptr = alHeapAlloc(config->heap, 1, maxVoices);
    }

    gAudioNodePool[i].dest_ptr = alHeapAlloc(config->heap, 1, maxVoices);
    for (i = 0; i < 2; i++) {
        gAudioCmdBuffers[i] = (Acmd *)alHeapAlloc(config->heap, 1, audioParams->syn_rsp_cmds * 8);
    }

    gAudioRspCmdCount = audioParams->syn_rsp_cmds;
    for (i = 0; i < 4; i++) {
        gAudioManager.audioInfo[i] = alHeapAlloc(config->heap, 1, sizeof(AudioInfo));
        gAudioManager.audioInfo[i] = gAudioManager.audioInfo[i];
        gAudioManager.audioInfo[i]->frameSamples = 9;
        gAudioManager.audioInfo[i]->msg = gAudioManager.audioInfo[i];
        gAudioManager.audioInfo[i]->data = alHeapAlloc(config->heap, 1, (sizeof(s32)) * D_800A6474_A7074);
    }

    osCreateMesgQueue(&gAudioManager.audioFrameMsgQ, gAudioManager.audioFrameMsgBuf, 8);
    osCreateMesgQueue(&gAudioManager.audioReplyMsgQ, gAudioManager.audioReplyMsgBuf, 8);
    osCreateMesgQueue(&gAudioMsgQueue, (OSMesg *)gAudioMsgBuffer, maxChannels * 2);
    if (!gAudioThreadCreated) {
        osCreateThread(&gAudioManager.thread, 3, audioManagerThread, 0, &gDriveRomInitialized, id);
    }
    osStartThread(&gAudioManager.thread);

    gAudioThreadCreated = 1;
}*/

void audioManagerThread(void *arg) {
    ViConfig cfg;
    Msg *frameMsg;
    AudioMsg *taskCompleteMsg;
    OSMesgQueue *q = &gAudioManager.audioReplyMsgQ;
    s32 stop = FALSE;

    addViConfig(&cfg, &gAudioManager.audioReplyMsgQ, 1);

    while (!stop) {
        osRecvMesg(&gAudioManager.audioReplyMsgQ, (OSMesg)&frameMsg, OS_MESG_BLOCK);
        osRecvMesg(&gAudioManager.audioReplyMsgQ, NULL, OS_MESG_NOBLOCK);

        if (((Msg *)(&frameMsg))->type == 5) {
            void *currentAudioInfo = gAudioManager.audioInfo[gCurrentFrame % 4];
            if (audioCreateAndScheduleTask(currentAudioInfo, gPendingMessages)) {
                osRecvMesg(&gAudioManager.audioFrameMsgQ, (OSMesg)&taskCompleteMsg, OS_MESG_BLOCK);
                handleAudioUnderrun(taskCompleteMsg->done.info);
                gPendingMessages = taskCompleteMsg->done.info;
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
    commandBufferEnd =
        alAudioFrame(commandBuffer, &commandLength, (void *)outputBuffer, audioTaskDesc->frameSizeInSamples);
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
    audioTaskDesc->dataMemory = &aspMainDataStart;
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

void handleAudioUnderrun(void *arg0) {
    if (((osAiGetLength() >> 2) == 0) && (gAudioUnderrunFlag == 0)) {
        gAudioUnderrunFlag = 0;
    }
}

int loadAudioDataWithCache(s32 romAddr, s32 requestSize) {
    AudioNode *previousNode;
    s16 alignmentOffset;
    AudioNode *currentNode;
    s32 alignedAddress;
    OSPiHandle *piHandle;
    s32 tempValue;
    s32 v1;
    s32 tempAddress;
    OSIoMesg *dmaMsg;
    AudioNode *tempPtr;

    alignedAddress = romAddr;
    v1 = 0xFF000000;
    tempValue = alignedAddress & 0xFF000000;
    if (tempValue == 0xFF000000) {
        piHandle = gDriveRomHandle;
        alignedAddress = alignedAddress & 0xFFFFFF;
        alignedAddress = alignedAddress + 0x140000;
    } else {
        if (__muscontrol_flag & 1) {
            return osVirtualToPhysical((void *)alignedAddress);
        }
        piHandle = gCartRomHandle;
    }

    previousNode = NULL;
    requestSize = (alignedAddress + requestSize) & 0xFFFFFFFFFFFFFFFFu;
    for (currentNode = gActiveListHead; currentNode != NULL; currentNode = (AudioNode *)currentNode->l.next) {
        tempValue = currentNode->data;
        v1 = tempValue + gMaxVoices;
        tempValue = ((u32)alignedAddress) < tempValue;
        if (tempValue != 0) {
            break;
        }

        tempValue = v1 < requestSize;
        if (tempValue == 0) {
            tempValue = currentNode->data;
            v1 = gCurrentFrame;
            currentNode->timestamp = v1;

            return osVirtualToPhysical((void *)(s32)currentNode->dest_ptr + alignedAddress - tempValue);
        }

        previousNode = currentNode;
    }

    currentNode = *((AudioNode **)(s32)(&D_800A6468_A7068));
    if (currentNode == NULL) {
        return osVirtualToPhysical((void *)gActiveListHead);
    }

    tempValue = (s32)currentNode->l.next;
    *((AudioNode **)(s32)(&D_800A6468_A7068)) = (AudioNode *)tempValue;
    alUnlink((ALLink *)currentNode);

    if (previousNode != NULL) {
        alLink((ALLink *)currentNode, (ALLink *)previousNode);
    } else {
        tempPtr = gActiveListHead;
        if (tempPtr != NULL) {
            previousNode = tempPtr;
            gActiveListHead = currentNode;
            currentNode->l.next = (ALLink *)previousNode;
            currentNode->l.prev = NULL;
            previousNode->l.prev = (ALLink *)currentNode;
        } else {
            gActiveListHead = currentNode;
            currentNode->l.next = NULL;
            currentNode->l.prev = NULL;
        }
    }

    tempValue = gCurrentFrame;
    v1 = D_8009B034_9BC34;
    dmaMsg = gAudioDmaMessages;
    previousNode = (AudioNode *)(alignedAddress & 1);
    alignmentOffset = (s32)previousNode;
    alignedAddress = alignedAddress - alignmentOffset;
    currentNode->data = alignedAddress;
    currentNode->timestamp = gCurrentFrame;
    currentNode = (AudioNode *)currentNode->dest_ptr;
    tempValue = v1 + 1;
    D_8009B034_9BC34 = v1 + 1;
    tempValue = v1 << 1;
    tempValue = v1 + tempValue;
    tempValue = tempValue << 3;
    dmaMsg = (OSIoMesg *)(((s32)dmaMsg) + tempValue);
    dmaMsg->hdr.pri = 0;
    dmaMsg->dramAddr = currentNode;
    dmaMsg->hdr.retQueue = (OSMesgQueue *)(s32)(&gAudioMsgQueue);
    dmaMsg->devAddr = alignedAddress;
    dmaMsg->size = gMaxVoices;

    osEPiStartDma(piHandle, dmaMsg, 0);

    return osVirtualToPhysical((void *)currentNode) + alignmentOffset;
}

void *initAudioDriveAndGetLoader(void *arg0) {
    u8 *handle = &gDriveRomInitialized;
    if (handle[0] == 0) {
        AudioNode *value = gAudioNodePool;
        gActiveListHead = (AudioNode *)0;
        handle[0] = TRUE;
        D_800A6468_A7068 = value;
    }
    *(u8 **)arg0 = handle;
    return &loadAudioDataWithCache;
}

void processAudioNodeList(void) {
    void *message;
    u32 i;
    AudioNode *new_var;
    AudioNode *nextNode;
    AudioNode *node;
    AudioNode *headPtr;

    for (i = 0; i < D_8009B034_9BC34; i++) {
        osRecvMesg(&gAudioMsgQueue, &message, OS_MESG_NOBLOCK);
    }

    headPtr = (AudioNode *)&gActiveListHead;
    node = (AudioNode *)headPtr->l.next;
    while (node != NULL) {
        nextNode = (AudioNode *)node->l.next;
        new_var = headPtr;
        if ((node->timestamp + 1) >= gCurrentFrame) {
            node = nextNode;
        } else {
            if (new_var->l.next == ((ALLink *)node)) {
                new_var->l.next = (ALLink *)nextNode;
            }

            alUnlink((ALLink *)node);

            // This is very important for reasons I do not understand
            do {
            } while (0);

            if (new_var->l.prev != NULL) {
                alLink((ALLink *)node, new_var->l.prev);
            } else {
                new_var->l.prev = (ALLink *)node;
                node->l.next = 0;
                node->l.prev = 0;
            }
            node = nextNode;
        }
    }

    D_8009B034_9BC34 = 0;
    gCurrentFrame += 1;
}
