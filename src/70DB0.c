#include "common.h"

void thread_function_1(void*);
void thread_function_2(void*);
void thread_function_3(void*);
void thread_function_4(void*);

typedef struct {
    char padding[61];
    OSMesg messageQueue;
    OSMesg message;
} eventQueue2Response;

// data
int vertical_retrace_message[3] = {0x5, 0, 0};
int sp_task_done_message[3] = {0x6, 0, 0};
int dp_interrupt_message[3] = {0x8, 0, 0};
int prenmi_interrupt_message[3] = {0xA, 0, 0};

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
s32 viConfigFlags;
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
    viConfigFlags = 0;
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

INCLUDE_ASM("asm/nonmatchings/70DB0", thread_function_1);

INCLUDE_ASM("asm/nonmatchings/70DB0", func_800705D0_711D0);

INCLUDE_ASM("asm/nonmatchings/70DB0", func_80070650_71250);

INCLUDE_ASM("asm/nonmatchings/70DB0", thread_function_2);

void thread_function_3(void* arg) {
    // force specific layout of these variables on the stack
    struct {
        OSMesg eventQueueOneMessage;
        OSMesg messageQueue;
        OSMesg message;
    } stack;
    OSMesg frameBufferQueueMessage;
    eventQueue2Response* temp;

    stack.eventQueueOneMessage = (OSMesg)0xB;

    while (TRUE) {
        osRecvMesg(&eventQueue2, &stack.message, 1);
        osSendMesg(&eventQueue1, &stack.eventQueueOneMessage, 1);
        osRecvMesg(&frameBufferQueue, &frameBufferQueueMessage, 1);

        temp = stack.message;  // force a copy into v0
        osSendMesg(temp->messageQueue, temp->message, 1);
    }
}

void func_80070990_71590(void* arg0) {
    osSendMesg(&eventQueue2, arg0, 1);
}

INCLUDE_ASM("asm/nonmatchings/70DB0", thread_function_4);

void func_80070B9C_7179C(void* arg0) {
    osSendMesg(&threadSyncQueue, arg0, 1);
}
