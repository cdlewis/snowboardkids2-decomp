#include "audio/audio_manager.h"

#include "abi.h"
#include "common.h"
#include "libaudio.h"
#include "mbi.h"
#include "os_ai.h"
#include "os_convert.h"
#include "os_message.h"
#include "os_pi.h"
#include "os_thread.h"
#include "system/thread_manager.h"
#include "ucode.h"

typedef struct {
    ALLink link;
    s32 startAddress;
    s32 lastFrameUsed;
    void *buffer;
} AudioDmaBuffer;

extern ALGlobals __libmus_alglobals;
extern s32 __muscontrol_flag;

// bss
char gAudioManagerStack[0x2000] __attribute__((section(".bss")));
u8 gDriveRomInitialized __attribute__((section(".bss")));
AudioDmaBuffer *gActiveListHead __attribute__((section(".bss")));
AudioDmaBuffer *D_800A6468_A7048 __attribute__((section(".bss")));
u32 gMinAudioFrameSize __attribute__((section(".bss")));
s32 gAudioBufferSize __attribute__((section(".bss")));
s32 D_800A6474_A7054 __attribute__((section(".bss")));
s32 gAudioRspCmdCount __attribute__((section(".bss")));
static s32 D_800A647C_A705C __attribute__((section(".bss")));
OSMesgQueue gAudioMsgQueue __attribute__((section(".bss")));
AudioDmaBuffer *gAudioNodePool __attribute__((section(".bss")));
OSIoMesg *gAudioDmaMessages __attribute__((section(".bss")));
void **gAudioMsgBuffer __attribute__((section(".bss")));
u32 gMaxVoices __attribute__((section(".bss")));
s32 gAudioBufferPadding __attribute__((section(".bss")));
OSPiHandle *gCartRomHandle __attribute__((section(".bss")));
OSPiHandle *gDriveRomHandle __attribute__((section(".bss")));
static u8 D_800A64B4_A7094[0xC] __attribute__((section(".bss")));

// data
u32 gCurrentFrame = 0;
u32 D_8009B034_9BC34 = 0;
s32 gAudioCmdBufferToggle = 0;
s32 gAudioThreadCreated = 0;
AudioInfo *gPendingMessages = NULL;
s32 gAudioUnderrunFlag = 1;

extern void CustomInit(void *, ALSynConfig *);

s32 loadAudioDataWithCache(s32, s32);
void handleAudioUnderrun(void *);
void processAudioNodeList(void);
s32 audioCreateAndScheduleTask(AudioInfo *, AudioInfo *);
void *initAudioDriveAndGetLoader(void *arg0);
void audioManagerThread(void *);

void initAudioManager(
    ALSynConfig *config,
    OSId id,
    AudioParams *audioParams,
    s32 maxChannels,
    s32 maxVoices,
    s32 sampleRate
) {
    u32 i;
    f64 tempDouble;
    f32 tempFloat;
    f32 divResult;
    OSPiHandle *tempDriveRomHandle;
    u32 *new_var;
    u32 outputRate;

    gCartRomHandle = osCartRomInit();
    tempDriveRomHandle = osDriveRomInit();
    gDriveRomInitialized = FALSE;
    new_var = &audioParams->outputRate;
    outputRate = *new_var;
    gDriveRomHandle = tempDriveRomHandle;

    gMaxVoices = maxVoices;

    if (outputRate < 0x5622) {
        gAudioBufferPadding = 0x50;
    } else {
        gAudioBufferPadding = 0xF0;
    }

    config->dmaproc = &initAudioDriveAndGetLoader;
    config->outputRate = osAiSetFrequency(audioParams->outputRate);
    gAudioNodePool = (AudioDmaBuffer *)alHeapDBAlloc(0, 0, config->heap, 1, sizeof(AudioDmaBuffer) * maxChannels);
    gAudioDmaMessages = (OSIoMesg *)alHeapDBAlloc(0, 0, config->heap, 1, sizeof(OSIoMesg) / 2 * maxChannels * 4);
    gAudioMsgBuffer = (void **)alHeapDBAlloc(0, 0, config->heap, 1, sizeof(void *) * maxChannels * 2);

    tempDouble = (f64)audioParams->frameRateScale;
    if (audioParams->frameRateScale < 0) {
        tempDouble += 4294967296;
    }
    tempFloat = (f32)tempDouble;
    tempFloat = tempFloat * (f32)config->outputRate;
    divResult = tempFloat / (f32)sampleRate;
    gAudioBufferSize = (s32)divResult;

    tempDouble = (f64)gAudioBufferSize;
    if (gAudioBufferSize < 0) {
        tempDouble += 4294967296;
    }
    if ((f32)tempDouble < divResult) {
        gAudioBufferSize++;
    }

    if (gAudioBufferSize & 0xF) {
        gAudioBufferSize = (gAudioBufferSize & (~0xF)) + 0x10;
    }

    gMinAudioFrameSize = gAudioBufferSize - 0x10;
    D_800A6474_A7054 = gAudioBufferSize + gAudioBufferPadding + 0x10;

    CustomInit(&__libmus_alglobals, config);

    gAudioNodePool->link.prev = NULL;
    gAudioNodePool->link.next = NULL;
    for (i = 0; i < (maxChannels - 1); i++) {
        alLink((ALLink *)(&gAudioNodePool[i + 1]), (ALLink *)(&gAudioNodePool[i]));
        gAudioNodePool[i].buffer = alHeapDBAlloc(0, 0, config->heap, 1, maxVoices);
    }

    gAudioNodePool[i].buffer = alHeapDBAlloc(0, 0, config->heap, 1, maxVoices);

    for (i = 0; i < 2; i++) {
        gAudioManager.commandLists[i] =
            (Acmd *)alHeapDBAlloc(0, 0, config->heap, 1, audioParams->commandListCapacity * 8);
    }

    gAudioRspCmdCount = audioParams->commandListCapacity;

    for (i = 0; i < 4; i++) {
        gAudioManager.audioInfo[i] = (AudioInfo *)alHeapDBAlloc(0, 0, config->heap, 1, sizeof(AudioInfo));
        gAudioManager.audioInfo[i]->doneMessage.type = 9;
        gAudioManager.audioInfo[i]->doneMessage.info = gAudioManager.audioInfo[i];
        gAudioManager.audioInfo[i]->outputBuffer =
            (s16 *)alHeapDBAlloc(0, 0, config->heap, 1, sizeof(s32) * D_800A6474_A7054);
    }

    osCreateMesgQueue(&gAudioManager.taskDoneQueue, (OSMesg *)&gAudioManager.taskDoneMessages, 8);
    osCreateMesgQueue(&gAudioManager.retraceQueue, (OSMesg *)&gAudioManager.retraceMessages, 8);
    osCreateMesgQueue(&gAudioMsgQueue, (OSMesg *)gAudioMsgBuffer, maxChannels * 2);

    if (!gAudioThreadCreated) {
        osCreateThread(
            &gAudioManager.thread,
            3,
            audioManagerThread,
            0,
            gAudioManagerStack + sizeof(gAudioManagerStack),
            id
        );
    }

    osStartThread(&gAudioManager.thread);

    gAudioThreadCreated = TRUE;
}

void audioManagerThread(void *arg) {
    ViConfig cfg;
    ViMessage frameMessage;
    AudioTaskDoneMessage *taskCompleteMsg;
    OSMesgQueue *q = &gAudioManager.retraceQueue;
    s32 stop = FALSE;

    addViConfig(&cfg, &gAudioManager.retraceQueue, 1);

    while (!stop) {
        osRecvMesg(&gAudioManager.retraceQueue, &frameMessage.raw, OS_MESG_BLOCK);
        osRecvMesg(&gAudioManager.retraceQueue, NULL, OS_MESG_NOBLOCK);

        if (frameMessage.retrace.type == 5) {
            AudioInfo *currentAudioInfo = gAudioManager.audioInfo[gCurrentFrame % 4];
            if (audioCreateAndScheduleTask(currentAudioInfo, gPendingMessages)) {
                osRecvMesg(&gAudioManager.taskDoneQueue, (OSMesg)&taskCompleteMsg, OS_MESG_BLOCK);
                handleAudioUnderrun(taskCompleteMsg->info);
                gPendingMessages = taskCompleteMsg->info;
            }
        }
    }

    alClose(&__libmus_alglobals);
}

s32 audioCreateAndScheduleTask(AudioInfo *audioTaskDesc, AudioInfo *prevBuffer) {
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
        osAiSetNextBuffer(prevBuffer->outputBuffer, prevBuffer->frameSamples * 4);
    }

    currentSamplesInBuffer = osAiGetLength() / 4;
    samplesToProcess = (((gAudioBufferSize - currentSamplesInBuffer) + gAudioBufferPadding) + 16) & 0xFFF0;

    audioTaskDesc->frameSamples = samplesToProcess;
    if ((s16)samplesToProcess < gMinAudioFrameSize) {
        audioTaskDesc->frameSamples = gMinAudioFrameSize;
    }

    commandBuffer = gAudioManager.commandLists[gAudioCmdBufferToggle];
    commandBufferEnd = alAudioFrame(commandBuffer, &commandLength, (void *)outputBuffer, audioTaskDesc->frameSamples);
    if (commandLength == 0) {
        return 0;
    }

    audioTaskDesc->messageQueue = &gAudioManager.taskDoneQueue;
    audioTaskDesc->message = &audioTaskDesc->doneMessage;
    audioTaskDesc->task.t.data_ptr = (u64 *)gAudioManager.commandLists[gAudioCmdBufferToggle];
    commandBufferStart = gAudioManager.commandLists[gAudioCmdBufferToggle];
    commandBufferSize = commandBufferEnd - commandBufferStart;
    commandBufferSize = commandBufferSize * 8;

    audioTaskDesc->task.t.type = M_AUDTASK;
    audioTaskDesc->task.t.ucode_boot = (u64 *)rspbootTextStart;
    audioTaskDesc->task.t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;
    audioTaskDesc->task.t.ucode = (u64 *)aspMainTextStart;
    audioTaskDesc->task.t.flags = 0;
    audioTaskDesc->task.t.ucode_data = (u64 *)&aspMainDataStart;
    audioTaskDesc->task.t.ucode_data_size = 0x800;
    audioTaskDesc->task.t.dram_stack = NULL;
    audioTaskDesc->task.t.dram_stack_size = 0;
    audioTaskDesc->task.t.output_buff = NULL;
    audioTaskDesc->task.t.output_buff_size = NULL;
    audioTaskDesc->task.t.yield_data_ptr = NULL;
    audioTaskDesc->task.t.yield_data_size = 0;
    audioTaskDesc->task.t.data_size = commandBufferSize;

    submitAudioTask((OSMesg)&audioTaskDesc->task);

    gAudioCmdBufferToggle ^= 1;

    return 1;
}

void handleAudioUnderrun(void *arg0) {
    if (((osAiGetLength() >> 2) == 0) && (gAudioUnderrunFlag == 0)) {
        gAudioUnderrunFlag = 0;
    }
}

s32 loadAudioDataWithCache(s32 romAddr, s32 requestSize) {
    AudioDmaBuffer *previousNode;
    s16 alignmentOffset;
    AudioDmaBuffer *currentNode;
    s32 alignedAddress;
    OSPiHandle *piHandle;
    s32 tempValue;
    s32 v1;
    s32 tempAddress;
    OSIoMesg *dmaMsg;
    AudioDmaBuffer *tempPtr;

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
    for (currentNode = gActiveListHead; currentNode != NULL; currentNode = (AudioDmaBuffer *)currentNode->link.next) {
        tempValue = currentNode->startAddress;
        v1 = tempValue + gMaxVoices;
        tempValue = ((u32)alignedAddress) < tempValue;
        if (tempValue != 0) {
            break;
        }

        tempValue = v1 < requestSize;
        if (tempValue == 0) {
            tempValue = currentNode->startAddress;
            v1 = gCurrentFrame;
            currentNode->lastFrameUsed = v1;

            return osVirtualToPhysical((void *)(s32)currentNode->buffer + alignedAddress - tempValue);
        }

        previousNode = currentNode;
    }

    currentNode = *((AudioDmaBuffer **)(s32)(&D_800A6468_A7048));
    if (currentNode == NULL) {
        return osVirtualToPhysical((void *)gActiveListHead);
    }

    tempValue = (s32)currentNode->link.next;
    *((AudioDmaBuffer **)(s32)(&D_800A6468_A7048)) = (AudioDmaBuffer *)tempValue;
    alUnlink((ALLink *)currentNode);

    if (previousNode != NULL) {
        alLink((ALLink *)currentNode, (ALLink *)previousNode);
    } else {
        tempPtr = gActiveListHead;
        if (tempPtr != NULL) {
            previousNode = tempPtr;
            gActiveListHead = currentNode;
            currentNode->link.next = (ALLink *)previousNode;
            currentNode->link.prev = NULL;
            previousNode->link.prev = (ALLink *)currentNode;
        } else {
            gActiveListHead = currentNode;
            currentNode->link.next = NULL;
            currentNode->link.prev = NULL;
        }
    }

    tempValue = gCurrentFrame;
    v1 = D_8009B034_9BC34;
    dmaMsg = gAudioDmaMessages;
    previousNode = (AudioDmaBuffer *)(alignedAddress & 1);
    alignmentOffset = (s32)previousNode;
    alignedAddress = alignedAddress - alignmentOffset;
    currentNode->startAddress = alignedAddress;
    currentNode->lastFrameUsed = gCurrentFrame;
    currentNode = (AudioDmaBuffer *)currentNode->buffer;
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
        AudioDmaBuffer *value = gAudioNodePool;
        gActiveListHead = (AudioDmaBuffer *)0;
        handle[0] = TRUE;
        D_800A6468_A7048 = value;
    }
    *(u8 **)arg0 = handle;
    return &loadAudioDataWithCache;
}

void processAudioNodeList(void) {
    void *message;
    u32 i;
    AudioDmaBuffer *new_var;
    AudioDmaBuffer *nextNode;
    AudioDmaBuffer *node;
    AudioDmaBuffer *headPtr;

    for (i = 0; i < D_8009B034_9BC34; i++) {
        osRecvMesg(&gAudioMsgQueue, &message, OS_MESG_NOBLOCK);
    }

    headPtr = (AudioDmaBuffer *)&gActiveListHead;
    node = (AudioDmaBuffer *)headPtr->link.next;
    while (node != NULL) {
        nextNode = (AudioDmaBuffer *)node->link.next;
        new_var = headPtr;
        if ((node->lastFrameUsed + 1) >= gCurrentFrame) {
            node = nextNode;
        } else {
            if (new_var->link.next == ((ALLink *)node)) {
                new_var->link.next = (ALLink *)nextNode;
            }

            alUnlink((ALLink *)node);

            // This is very important for reasons I do not understand
            do {
            } while (0);

            if (new_var->link.prev != NULL) {
                alLink((ALLink *)node, new_var->link.prev);
            } else {
                new_var->link.prev = (ALLink *)node;
                node->link.next = 0;
                node->link.prev = 0;
            }
            node = nextNode;
        }
    }

    D_8009B034_9BC34 = 0;
    gCurrentFrame += 1;
}
