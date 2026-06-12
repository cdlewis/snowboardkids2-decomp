#include "common.h"
#include "core/mainEntrypoint.h"
#include "os_system.h"
#include "os_thread.h"

void idleThreadEntrypoint(void *);
OSThread idleThread BSS;
static u8 idleThreadStack[0x180] BSS;
OSThread mainThread BSS;
extern u8 mainThreadStack[0x820];

#define IDLE_THREAD_ID 1
#define MAIN_THREAD_ID 2

void mainproc(void) {
    osInitialize();
    osCreateThread(
        &idleThread,
        IDLE_THREAD_ID,
        idleThreadEntrypoint,
        NULL,
        idleThreadStack + sizeof(idleThreadStack),
        2
    );
    osStartThread(&idleThread);
}

void idleThreadEntrypoint(void *arg /* NULL */) {
    osCreateThread(
        &mainThread,
        MAIN_THREAD_ID,
        mainThreadEntrypoint,
        arg,
        mainThreadStack + sizeof(mainThreadStack),
        2
    );
    osStartThread(&mainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE) {}
}
