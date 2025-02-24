#include "common.h"

typedef struct {
    char padding2[61];
    OSMesg messageQueue;
    OSMesg message;
    void *unk48;
    u16 unk4C;
    u16 unk4E;
    u32 padding;
} FrameInfo;

void thread_function_4(void *arg) {
    s32 temp_v0;
    s32 delayCounter;
    s32 var_v0;
    u16 temp_v0_2;
    s32 frameIndex;
    void *frameBuffer;
    struct
    {
        s32 sp10;
        FrameInfo *frameInfo;
        s32 temp_a0;
        char padding2[4];
        ViConfig *sp20;
        char padding3[10];
        OSMesg *sp30;
        void *sp34;
    } stack;
    delayCounter = 1;
    stack.sp10 = 7;
    addViConfig(&stack.sp20, &eventQueue4, 1);
    while (1) {
        osRecvMesg(&threadSyncQueue, (OSMesg *)(&stack.frameInfo), 1);
        frameIndex = stack.frameInfo->unk4C;
        frameBuffer = stack.frameInfo->unk48;
        while ((osViGetCurrentFramebuffer() == frameBuffer) || (osViGetNextFramebuffer() == frameBuffer)) {
            osRecvMesg(&eventQueue4, &stack.sp30, 1);
        }

        osSendMesg(&eventQueue1, &stack.sp10, 1);
        osRecvMesg(&eventQueue3, &stack.sp34, 1);
        osRecvMesg(&taskCompletionQueue, &stack.sp34, 1);
        if (!(stack.frameInfo->unk4E & 1)) {
            continue;
        }
        osSendMesg(stack.frameInfo->messageQueue, stack.frameInfo->message, 1);
        temp_v0 = frameCounter;
        var_v0 = frameIndex << 0x10;
        goto check_condition;
        do {
            frameIndex = var_v0;
            var_v0 = var_v0 << 0x10;  // used to be (s16) var_v0
        check_condition:
            var_v0 = var_v0 >> 0x10;
            var_v0 = temp_v0 - var_v0;
            var_v0 &= 0xFFF;

            temp_v0 = var_v0 != 0;
            var_v0 = var_v0 < 0x800;
            temp_v0 = temp_v0 && var_v0;

            var_v0 = frameIndex + 1;
        } while (temp_v0);

        while ((frameCounter - (s16)(frameIndex) & 0xFFF) >= 0x801) {
            osRecvMesg(&eventQueue4, &stack.sp30, 1);
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
