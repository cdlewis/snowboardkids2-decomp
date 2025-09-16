#include "thread_manager.h"

#include "common.h"

void thread_function_1(void *);
void thread_function_2(void *);
void thread_function_3(void *);
void thread_function_4(void *);

typedef struct {
    char padding[61];
    OSMesg messageQueue;
    OSMesg message;
} eventQueue2Response;

typedef struct {
    char padding2[61];
    OSMesg messageQueue;
    OSMesg message;
    void *unk48;
    u16 unk4C;
    u16 unk4E;
    u32 padding;
} FrameInfo;

typedef struct {
    u32 type;
    OSTask *task;
    OSTask *pendingTask;
} eventQueue1_message;

// data
int vertical_retrace_message[3] = { 0x5, 0, 0 };
int sp_task_done_message[3] = { 0x6, 0, 0 };
int dp_interrupt_message[3] = { 0x8, 0, 0 };
int prenmi_interrupt_message[3] = { 0xA, 0, 0 };
int D_8009B020_9BC20 = 0;
int D_8009B024_9BC24 = 0x1E;

// bss
OSThread thread_a;
OSThread thread_b;
OSThread thread_c;
OSThread thread_d;
OSMesg framebuffer_message;
OSMesg vi_message;
OSMesg event_queue_1_message;
OSMesg event_queue_2_message;
OSMesg thread_sync_message;
OSMesg task_completion_message;
OSMesg event_queue_3_message;
OSMesg event_queue_4_message;
char thread_a_stack[0x180];
char thread_b_stack[0x180];
char thread_c_stack[0x180];
char thread_d_stack[0x180];
s16 frameCounter;
s16 frameDelay;

ViConfig *currentViConfig;

OSMesgQueue viEventQueue;
OSMesgQueue eventQueue1;
OSMesgQueue eventQueue2;
OSMesgQueue threadSyncQueue;
OSMesgQueue taskCompletionQueue;
OSMesgQueue eventQueue3;
OSMesgQueue frameBufferQueue;
OSMesgQueue eventQueue4;

void initialize_video_and_threads(s32 viMode) {
    osCreateViManager(OS_PRIORITY_VIMGR);

    osViSetMode(&osViModeTable[(u8)viMode]);
    osViBlack(TRUE);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_OFF | OS_VI_DITHER_FILTER_ON);
    frameCounter = 0;
    frameDelay = 0x1E;
    currentViConfig = NULL;
    osCreateMesgQueue(&viEventQueue, &vi_message, 4);
    osViSetEvent(&viEventQueue, &vertical_retrace_message, 1U);
    osSetEventMesg(OS_EVENT_PRENMI, &viEventQueue, &prenmi_interrupt_message);

    osCreateThread(&thread_c, 4, thread_function_1, 0, thread_a_stack + sizeof(thread_a_stack), 9);

    osCreateMesgQueue(&eventQueue1, &event_queue_1_message, 8);

    osSetEventMesg(OS_EVENT_SP, &eventQueue1, &sp_task_done_message);
    osCreateThread(&thread_a, 0xA, thread_function_2, 0, thread_b_stack + sizeof(thread_b_stack), 8);
    osCreateMesgQueue(&eventQueue3, &event_queue_3_message, 4);

    osCreateMesgQueue(&threadSyncQueue, &thread_sync_message, 0x20);
    osCreateMesgQueue(&taskCompletionQueue, &task_completion_message, 4);
    osSetEventMesg(OS_EVENT_DP, &taskCompletionQueue, &dp_interrupt_message);
    osCreateMesgQueue(&eventQueue4, &event_queue_4_message, 1);
    osCreateThread(&thread_d, 5, thread_function_4, 0, thread_c_stack + sizeof(thread_c_stack), 4);

    osCreateMesgQueue(&frameBufferQueue, &framebuffer_message, 4);
    osCreateMesgQueue(&eventQueue2, &event_queue_2_message, 8);
    osCreateThread(&thread_b, 9, thread_function_3, 0, thread_d_stack + sizeof(thread_d_stack), 7);

    osStartThread(&thread_c);
    osStartThread(&thread_a);
    osStartThread(&thread_b);
    osStartThread(&thread_d);
}

void thread_function_1(void *arg) {
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
                D_8009B020_9BC20 = 1;
            }

            continue;
        }

        // Increment frameCounter and wrap if > 0xFFF
        frameCounter = (frameCounter + 1) & 0xFFF;

        if (D_8009B020_9BC20 && D_8009B024_9BC24) {
            D_8009B024_9BC24--;
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

        if (frameDelay == 0) {
            continue;
        }
        frameDelay--;
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

void thread_function_2(void *arg) {
    eventQueue1_message *msg;
    s32 task;
    s32 next_task;
    s32 status_flags = 0;
    OSTask *current_task;
    OSTask *pending_task;
    u32 event;
    s32 mask;

    while (TRUE) {
        osRecvMesg(&eventQueue1, (OSMesg *)&msg, OS_MESG_BLOCK);
        event = msg->type;

        switch (event) {
            case 7:
                if (!D_8009B020_9BC20) {
                    current_task = msg->task;

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
                if (D_8009B024_9BC24 >= 5) {
                    pending_task = msg->pendingTask;

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
                        if (D_8009B020_9BC20 == 0) {
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
                    osSendMesg(&frameBufferQueue, (OSMesg)6, OS_MESG_BLOCK);
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
                        osSendMesg(&eventQueue3, (OSMesg)6, OS_MESG_BLOCK);
                    }
                }
                break;
        }
    }
}

void thread_function_3(void *arg) {
    // force specific layout of these variables on the stack
    struct {
        OSMesg eventQueueOneMessage;
        OSMesgQueue *messageQueue;
        OSMesg message;
    } stack;
    OSMesg frameBufferQueueMessage;
    eventQueue2Response *temp;

    stack.eventQueueOneMessage = (OSMesg)0xB;

    while (TRUE) {
        osRecvMesg(&eventQueue2, &stack.message, OS_MESG_BLOCK);
        osSendMesg(&eventQueue1, &stack.eventQueueOneMessage, OS_MESG_BLOCK);
        osRecvMesg(&frameBufferQueue, &frameBufferQueueMessage, OS_MESG_BLOCK);

        temp = stack.message; // force a copy into v0
        osSendMesg(temp->messageQueue, temp->message, OS_MESG_BLOCK);
    }
}

void sendMessageToEventQueue2(OSMesg message) {
    osSendMesg(&eventQueue2, message, OS_MESG_BLOCK);
}

void thread_function_4(void *arg) {
    s32 temp_v0;
    s32 delayCounter;
    s16 frameIndex;
    void *frameBuffer;
    struct {
        s32 sp10;
        FrameInfo *frameInfo;
        s32 temp_a0;
        char padding2[4];
        ViConfig sp20;
        void *sp30;
        void *sp34;
    } stack;
    delayCounter = 1;
    stack.sp10 = 7;
    addViConfig(&stack.sp20, &eventQueue4, 1);
    while (1) {
        osRecvMesg(&threadSyncQueue, (OSMesg *)(&stack.frameInfo), OS_MESG_BLOCK);
        frameIndex = stack.frameInfo->unk4C;
        frameBuffer = stack.frameInfo->unk48;
        while ((osViGetCurrentFramebuffer() == frameBuffer) || (osViGetNextFramebuffer() == frameBuffer)) {
            osRecvMesg(&eventQueue4, &stack.sp30, OS_MESG_BLOCK);
        }

        osSendMesg(&eventQueue1, &stack.sp10, OS_MESG_BLOCK);
        osRecvMesg(&eventQueue3, &stack.sp34, OS_MESG_BLOCK);
        osRecvMesg(&taskCompletionQueue, &stack.sp34, OS_MESG_BLOCK);
        if (!(stack.frameInfo->unk4E & 1)) {
            continue;
        }
        osSendMesg(stack.frameInfo->messageQueue, stack.frameInfo->message, 1);

        while (((frameCounter - frameIndex & 0xFFF) != 0) && (frameCounter - frameIndex & 0xFFF) < 0x800) {
            frameIndex++;
        }

        while ((frameCounter - frameIndex & 0xFFF) >= 0x801) {
            osRecvMesg(&eventQueue4, &stack.sp30, OS_MESG_BLOCK);
        }

        osViSwapBuffer((void *)frameBuffer);
        if ((!(delayCounter & 0xFF)) || (frameDelay != 0)) {
            continue;
        }
        delayCounter--;
        if (D_8009B020_9BC20) {
            continue;
        }
        osViBlack(0);
    }
}

void sendMessageToThreadSyncQueue(OSMesg message) {
    osSendMesg(&threadSyncQueue, message, OS_MESG_BLOCK);
}
