#include "common.h"
#include "ultra64.h"

void mainThreadEntrypoint(void *);
void idleThreadEntrypoint(void * /*arg*/);
extern OSThread idleThread;
extern OSThread mainThread;
extern void *mainStack;

#define IDLE_THREAD_ID 1
#define MAIN_THREAD_ID 2

void mainproc(void) {
    osInitialize();
    osCreateThread(&idleThread, IDLE_THREAD_ID, idleThreadEntrypoint, NULL, &mainThread, 2);
    osStartThread(&idleThread);
}

void idleThreadEntrypoint(void *arg /* NULL */) {
    osCreateThread(&mainThread, MAIN_THREAD_ID, mainThreadEntrypoint, arg, &mainStack, 2);
    osStartThread(&mainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE) {}
}
