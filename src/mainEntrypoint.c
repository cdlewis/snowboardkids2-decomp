#include "3A1F0.h"
#include "69EF0.h"
#include "PRinternal/viint.h"
#include "common.h"
#include "displaylist.h"
#include "memory_allocator.h"
#include "thread_manager.h"

extern OSMesg mainMessageQueueBuffer[32];
extern OSMesgQueue mainMessageQueue;
extern u16 D_8009ADE0_9B9E0;
extern u8 D_800AFCE2_A7052;
extern void D_800A32D0_A3ED0;
extern void func_800395F0_3A1F0(void);
extern void func_80039B88_3A788(void);
extern void func_80039C34_3A834(void);
extern void func_8003AC38_3B838(void);
extern void func_8003AF6C_3BB6C(void);
extern void func_80056080_56C80(void);
extern void func_80057870_58470(s32);
extern void func_800692F0_69EF0(void);
extern void func_800693C4_69FC4(void *, s32);
extern void func_8006DC40_6E840(void);
extern void func_8006E000_6EC00(u16);
extern void func_8006E02C_6EC2C(void);
extern void func_8006E054_6EC54(u16);

void mainThreadEntrypoint(void *arg) {
    u16 message;
    s32 hasSpecialMessage;
    s32 isPaused;
    u32 messageData;

    isPaused = 0;

    if (osTvType == 1) {
        initialize_video_and_threads(2);
    } else {
        initialize_video_and_threads(0x1E);
    }

    func_800395F0_3A1F0();
    initializeMemoryAllocatorRegion();
    func_8006DC40_6E840();
    initPiManager();
    func_800692F0_69EF0();

    osCreateMesgQueue(&mainMessageQueue, mainMessageQueueBuffer, 0x20);

    addViConfig(&D_800A32D0_A3ED0, &mainMessageQueue, 2);
    func_800693C4_69FC4(&initializeOverlaySystem, 0x64);
    func_800693C4_69FC4(&func_80056080_56C80, 0xFA);

    while (TRUE) {
        osRecvMesg(&mainMessageQueue, (OSMesg *)&message, 1);

        hasSpecialMessage = 0;

        do {
            switch (message) {
                case 5:
                    messageData = *(u32 *)&message;
                    hasSpecialMessage = 1;
                    break;
                case 0xA:
                    isPaused = 1;
                    func_80057870_58470(0x14);
                    break;
                case 0:
                case 1:
                    func_8006E000_6EC00(message);
                    if (hasSpecialMessage == FALSE) {
                        func_8006E02C_6EC2C();
                    }
                    break;
            }

        } while (osRecvMesg(&mainMessageQueue, (OSMesg *)&message, 0) != -1);

        if (hasSpecialMessage == FALSE) {
            continue;
        }

        if (isPaused == FALSE) {
            func_80039C34_3A834();
            func_80039B88_3A788();
            D_8009ADE0_9B9E0++;
            cleanupUnusedNodes();
            runTaskSchedulers();
            func_8003AC38_3B838();
            func_8006E054_6EC54(messageData);
        } else {
            D_800AFCE2_A7052 = 1;
            func_8003AF6C_3BB6C();
            func_8003AC38_3B838();
        }
    }
}
