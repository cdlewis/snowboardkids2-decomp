#include "common.h"

extern OSMesgQueue eventQueue1;
extern OSMesgQueue eventQueue2;
extern OSMesgQueue frameBufferQueue;

typedef struct {
    char padding[61];
    OSMesg messageQueue;
    OSMesg message;
} eventQueue2Response;

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