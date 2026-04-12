#include "core/mainEntrypoint.h"
#include "audio/audio.h"
#include "common.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "os_message.h"
#include "os_system.h"
#include "os_vi.h"
#include "system/controller_io.h"
#include "system/memory_allocator.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "system/thread_manager.h"

extern OSMesg mainMessageQueueBuffer[32];
extern OSMesgQueue mainMessageQueue;
extern u16 D_8009ADE0_9B9E0;
extern u8 gControllerPollingEnabled;
extern void D_800A32D0_A3ED0;

void mainThreadEntrypoint(void *arg) {
    u16 message;
    s32 hasSpecialMessage;
    s32 isPaused;
    u32 messageData;

    isPaused = FALSE;

    if (osTvType == OS_TV_NTSC) {
        initialize_video_and_threads(OS_VI_NTSC_LAN1);
    } else {
        initialize_video_and_threads(OS_VI_MPAL_LAN1);
    }

    initControllerSubsystem();
    initializeMemoryAllocatorRegion();
    initDisplayBuffers();
    initPiManager();
    initTaskScheduler();

    osCreateMesgQueue(&mainMessageQueue, mainMessageQueueBuffer, 0x20);

    addViConfig(&D_800A32D0_A3ED0, &mainMessageQueue, 2);
    createRootTaskScheduler(&initializeOverlaySystem, 0x64);
    createRootTaskScheduler(&allocateAudioResources, 0xFA);

    while (TRUE) {
        osRecvMesg(&mainMessageQueue, (OSMesg *)&message, OS_MESG_BLOCK);

        hasSpecialMessage = FALSE;

        do {
            switch (message) {
                case 5:
                    messageData = *(u32 *)&message;
                    hasSpecialMessage = TRUE;
                    break;
                case 0xA:
                    isPaused = TRUE;
                    sendStopAudioChannelsCommand(0x14);
                    break;
                case 0:
                case 1:
                    handleFrameBufferComplete(message);
                    if (hasSpecialMessage == FALSE) {
                        tryProcessDisplayFrameUpdate();
                    }
                    break;
            }

        } while (osRecvMesg(&mainMessageQueue, (OSMesg *)&message, OS_MESG_NOBLOCK) != -1);

        if (hasSpecialMessage == FALSE) {
            continue;
        }

        if (isPaused == FALSE) {
            processControllerInputs();
            startControllerRead();
            D_8009ADE0_9B9E0++;
            cleanupUnusedNodes();
            runTaskSchedulers();
            motorUpdate();
            renderFrame((u16)messageData);
        } else {
            gControllerPollingEnabled = TRUE;
            resumeMotorStates();
            motorUpdate();
        }
    }
}
