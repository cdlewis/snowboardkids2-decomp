#include "system/thread_manager.h"

#include "R4300.h"
#include "common.h"
#include "os_cache.h"
#include "os_exception.h"
#include "os_message.h"
#include "os_thread.h"
#include "os_vi.h"
#include "sptask.h"

// Structs

typedef struct {
    char padding[61];
    OSMesg messageQueue;
    OSMesg message;
    void *framebuffer;
    u16 frameIndex;
    u16 flags;
    u32 padding2;
} GfxTask;

typedef struct {
    u32 type;
    OSTask *gfxTask;
    OSTask *audioTask;
} SchedMsg;

// data
int viRetraceMsg[3] = { 0x5, 0, 0 };
int spTaskDoneMsg[3] = { 0x6, 0, 0 };
int dpDoneMsg[3] = { 0x8, 0, 0 };
int prenmiMsg[3] = { 0xA, 0, 0 };
int gPrenmiReceived = 0;
int gPrenmiCountdown = 0x1E;

// bss
extern OSThread viThread;
extern OSThread schedThread;
extern OSThread audioThread;
extern OSThread displayThread;
extern OSMesg audioSpDoneQueueMsgs;
extern OSMesg viQueueMsgs;
extern OSMesg schedQueueMsgs;
extern OSMesg audioTaskQueueMsgs;
extern OSMesg displayQueueMsgs;
extern OSMesg dpDoneQueueMsgs;
extern OSMesg displaySpDoneQueueMsgs;
extern OSMesg displayViFrameQueueMsgs;
extern char viThreadStack[0x180];
extern char schedThreadStack[0x180];
extern char audioThreadStack[0x180];
extern char displayThreadStack[0x180];
extern s16 frameCounter;
extern s16 bootBlackoutFrames;
extern ViConfig *currentViConfig;
extern OSMesgQueue viEventQueue;
extern OSMesgQueue schedQueue;
extern OSMesgQueue audioTaskQueue;
extern OSMesgQueue displayQueue;
extern OSMesgQueue dpDoneQueue;
extern OSMesgQueue displaySpDoneQueue;
extern OSMesgQueue audioSpDoneQueue;
extern OSMesgQueue displayViFrameQueue;

// Function declarations (static - internal use only)

static void viThreadEntry(void *);
static void schedThreadEntry(void *);
static void audioThreadEntry(void *);
static void displayThreadEntry(void *);

void initThreadManager(s32 viMode) {
    osCreateViManager(OS_PRIORITY_VIMGR);

    osViSetMode(&osViModeTable[(u8)viMode]);
    osViBlack(TRUE);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_OFF | OS_VI_DITHER_FILTER_ON);
    frameCounter = 0;
    bootBlackoutFrames = 0x1E;
    currentViConfig = NULL;
    osCreateMesgQueue(&viEventQueue, &viQueueMsgs, 4);
    osViSetEvent(&viEventQueue, &viRetraceMsg, 1U);
    osSetEventMesg(OS_EVENT_PRENMI, &viEventQueue, &prenmiMsg);

    osCreateThread(&viThread, 4, viThreadEntry, 0, viThreadStack + sizeof(viThreadStack), 9);

    osCreateMesgQueue(&schedQueue, &schedQueueMsgs, 8);

    osSetEventMesg(OS_EVENT_SP, &schedQueue, &spTaskDoneMsg);
    osCreateThread(&schedThread, 0xA, schedThreadEntry, 0, schedThreadStack + sizeof(schedThreadStack), 8);
    osCreateMesgQueue(&displaySpDoneQueue, &displaySpDoneQueueMsgs, 4);

    osCreateMesgQueue(&displayQueue, &displayQueueMsgs, 0x20);
    osCreateMesgQueue(&dpDoneQueue, &dpDoneQueueMsgs, 4);
    osSetEventMesg(OS_EVENT_DP, &dpDoneQueue, &dpDoneMsg);
    osCreateMesgQueue(&displayViFrameQueue, &displayViFrameQueueMsgs, 1);
    osCreateThread(&displayThread, 5, displayThreadEntry, 0, displayThreadStack + sizeof(displayThreadStack), 4);

    osCreateMesgQueue(&audioSpDoneQueue, &audioSpDoneQueueMsgs, 4);
    osCreateMesgQueue(&audioTaskQueue, &audioTaskQueueMsgs, 8);
    osCreateThread(&audioThread, 9, audioThreadEntry, 0, audioThreadStack + sizeof(audioThreadStack), 7);

    osStartThread(&viThread);
    osStartThread(&schedThread);
    osStartThread(&audioThread);
    osStartThread(&displayThread);
}

static void viThreadEntry(void *arg) {
    u32 temp = 0xA << 16;
    s32 *message;
    s16 temp_v0;
    s32 messageType;
    u32 var_s1;
    ViConfig *var_s0;
    ViConfig *config;

    while (TRUE) {
        osRecvMesg(&viEventQueue, (OSMesg *)&message, OS_MESG_BLOCK);
        messageType = *message;

        if (messageType != 0x5) {
            if (messageType == 0xA) {
                // run through all configs and send as message until none are left?
                var_s1 = 0xFFFF & var_s1;
                var_s1 |= temp;
                var_s1 = (var_s1 & 0xFFFF0000);
                var_s1 |= (u16)frameCounter;
                var_s0 = currentViConfig;
                while (var_s0) {
                    osSendMesg(var_s0->messageQueue, (OSMesg)var_s1, OS_MESG_NOBLOCK);
                    var_s0 = var_s0->nextConfig;
                }

                osViBlack(TRUE);
                gPrenmiReceived = 1;
            }

            continue;
        }

        // Increment frameCounter and wrap if > 0xFFF
        frameCounter = (frameCounter + 1) & 0xFFF;

        if (gPrenmiReceived && gPrenmiCountdown) {
            gPrenmiCountdown--;
        }

        var_s1 = var_s1 & 0xFFFF;
        var_s1 |= 0x50000;
        var_s1 = (var_s1 & 0xFFFF0000);
        var_s1 |= (u16)frameCounter;
        config = currentViConfig;
        while (config) {
            temp_v0 = config->frameCounter;
            if (temp_v0) {
                config->frameCounter = temp_v0 - 1;
            } else {
                config->frameCounter = config->maxFrames;
                osSendMesg(config->messageQueue, (OSMesg)var_s1, OS_MESG_NOBLOCK);
            }
            config = config->nextConfig;
        }

        if (bootBlackoutFrames == 0) {
            continue;
        }
        bootBlackoutFrames--;
    }
}

void addViConfig(ViConfig *config, OSMesgQueue *messageQueue, s32 frameCount) {
    ViConfig *prevConfig;
    u32 nextIntMask;

    nextIntMask = osSetIntMask(SR_IE);

    if (currentViConfig != NULL) {
        currentViConfig->prevConfig = config;
    }
    prevConfig = currentViConfig;
    config->prevConfig = NULL;
    config->messageQueue = messageQueue;
    config->frameCounter = 0;
    config->maxFrames = (s16)(frameCount - 1);
    currentViConfig = config;
    config->nextConfig = prevConfig;

    osSetIntMask(nextIntMask);
}

void removeViConfig(ViConfig *configs) {
    u32 previousInterruptMask;

    previousInterruptMask = osSetIntMask(SR_IE);

    // If there was a previous config, update its next pointer
    if (configs->prevConfig != NULL) {
        configs->prevConfig->nextConfig = configs->nextConfig;
    } else {
        // If this was the first node, update the global pointer
        currentViConfig = configs->nextConfig;
    }

    // If there was a next config, set it to the previous one
    if (configs->nextConfig != NULL) {
        configs->nextConfig->prevConfig = configs->prevConfig;
    }
    osSetIntMask(previousInterruptMask);
}

static void schedThreadEntry(void *arg) {
    SchedMsg *msg;
    s32 task;
    s32 next_task;
    s32 status_flags = 0;
    OSTask *current_task;
    OSTask *pending_task;
    u32 event;
    s32 mask;

    while (TRUE) {
        osRecvMesg(&schedQueue, (OSMesg *)&msg, OS_MESG_BLOCK);
        event = msg->type;

        switch (event) {
            case 7:
                if (!gPrenmiReceived) {
                    current_task = msg->gfxTask;

                    if ((status_flags & 2)) {
                        status_flags |= 4;
                    } else {
                        status_flags |= 1;
                        mask = osSetIntMask(OS_IM_NONE);
                        osWritebackDCacheAll();
                        osSetIntMask(mask);
                        osSpTaskStart(current_task);
                    }
                }
                break;
            case 0xB:
                if (gPrenmiCountdown >= 5) {
                    pending_task = msg->audioTask;

                    if (status_flags & 1) {
                        status_flags |= 8;
                        osSpTaskYield();
                    } else {
                        s32 mask = osSetIntMask(OS_IM_NONE);
                        osWritebackDCacheAll();
                        osSetIntMask(mask);
                        osSpTaskStart(pending_task);
                        status_flags |= 2;
                    }
                }
                break;
            case 6:
                if (status_flags & 2) {
                    status_flags &= ~2;

                    if (status_flags & 4) {
                        if (gPrenmiReceived == 0) {
                            s32 mask = osSetIntMask(OS_IM_NONE);
                            osWritebackDCacheAll();
                            osSetIntMask(mask);
                            osSpTaskStart(current_task);
                            status_flags |= 1;
                        }
                        status_flags &= ~4;
                    }

                    if (status_flags & 0x10) {
                        osSpTaskStart(current_task);
                        status_flags &= ~0x10;
                        status_flags |= 1;
                    }
                    osSendMesg(&audioSpDoneQueue, (OSMesg)6, OS_MESG_BLOCK);
                } else if (status_flags & 1) {
                    status_flags &= ~1;

                    if (status_flags & 8) {
                        if (osSpTaskYielded(current_task)) {
                            status_flags |= 0x10;
                        } else {
                            status_flags |= 0x20;
                        }
                        mask = osSetIntMask(OS_IM_NONE);
                        osWritebackDCacheAll();
                        osSetIntMask(mask);
                        osSpTaskStart(pending_task);
                        status_flags &= ~8;
                        status_flags |= 2;
                    } else {
                        status_flags |= 0x20;
                    }
                    if (status_flags & 0x20) {
                        status_flags &= ~0x20;
                        osSendMesg(&displaySpDoneQueue, (OSMesg)6, OS_MESG_BLOCK);
                    }
                }
                break;
        }
    }
}

static void audioThreadEntry(void *arg) {
    // force specific layout of these variables on the stack
    struct {
        OSMesg audioStartMsg;
        OSMesgQueue *messageQueue;
        OSMesg message;
    } stack;
    OSMesg audioSpDoneMsg;
    GfxTask *temp;

    stack.audioStartMsg = (OSMesg)0xB;

    while (TRUE) {
        osRecvMesg(&audioTaskQueue, &stack.message, OS_MESG_BLOCK);
        osSendMesg(&schedQueue, &stack.audioStartMsg, OS_MESG_BLOCK);
        osRecvMesg(&audioSpDoneQueue, &audioSpDoneMsg, OS_MESG_BLOCK);

        temp = stack.message; // force a copy into v0
        osSendMesg(temp->messageQueue, temp->message, OS_MESG_BLOCK);
    }
}

void submitAudioTask(OSMesg message) {
    osSendMesg(&audioTaskQueue, message, OS_MESG_BLOCK);
}

static void displayThreadEntry(void *arg) {
    s32 temp_v0;
    s32 delayCounter;
    s16 frameIndex;
    void *frameBuffer;
    struct {
        s32 sp10;
        GfxTask *gfxTask;
        s32 temp_a0;
        char padding2[4];
        ViConfig sp20;
        void *sp30;
        void *sp34;
    } stack;
    delayCounter = 1;
    stack.sp10 = 7;
    addViConfig(&stack.sp20, &displayViFrameQueue, 1);
    while (1) {
        osRecvMesg(&displayQueue, (OSMesg *)(&stack.gfxTask), OS_MESG_BLOCK);
        frameIndex = stack.gfxTask->frameIndex;
        frameBuffer = stack.gfxTask->framebuffer;
        while ((osViGetCurrentFramebuffer() == frameBuffer) || (osViGetNextFramebuffer() == frameBuffer)) {
            osRecvMesg(&displayViFrameQueue, &stack.sp30, OS_MESG_BLOCK);
        }

        osSendMesg(&schedQueue, &stack.sp10, OS_MESG_BLOCK);
        osRecvMesg(&displaySpDoneQueue, &stack.sp34, OS_MESG_BLOCK);
        osRecvMesg(&dpDoneQueue, &stack.sp34, OS_MESG_BLOCK);
        if (!(stack.gfxTask->flags & 1)) {
            continue;
        }
        osSendMesg(stack.gfxTask->messageQueue, stack.gfxTask->message, 1);

        while (((frameCounter - frameIndex & 0xFFF) != 0) && (frameCounter - frameIndex & 0xFFF) < 0x800) {
            frameIndex++;
        }

        while ((frameCounter - frameIndex & 0xFFF) >= 0x801) {
            osRecvMesg(&displayViFrameQueue, &stack.sp30, OS_MESG_BLOCK);
        }

        osViSwapBuffer(frameBuffer);
        if ((!(delayCounter & 0xFF)) || (bootBlackoutFrames != 0)) {
            continue;
        }
        delayCounter--;
        if (gPrenmiReceived) {
            continue;
        }
        osViBlack(0);
    }
}

void submitDisplayTask(OSMesg message) {
    osSendMesg(&displayQueue, message, OS_MESG_BLOCK);
}
