#include "graphics.h"
#include "common.h"
#include "displaylist.h"
#include "player.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_6A83F0);

typedef struct {
    void *musicBankBuffer;
    void *ptrBank;
    void *waveBank;
    s32 soundId;
    s32 soundSequence;
    s32 volume;
    s32 pan;
    void *audioChannel;
    s32 stoppingSpeed;
    f32 position;
    s32 voiceIndex;
    s32 unk2C;
    s32 unk30;
} GraphicsCommand;
extern GraphicsCommand gGraphicsCommand;

typedef struct {
    s16 soundId;
    s16 duration;
    s16 priority;
    s16 hasVolume;
    f32 volume;
    Vec3i position;
    s16 flags;
    u8 padding[2];
} RenderQueueItem;

typedef struct {
    /* 0x00 */ void *ptrBank;
    /* 0x04 */ void *musicDataBuffer;
    /* 0x08 */ void *musicBankBuffer;
    /* 0x0C */ void *currentAudioChannel;
    /* 0x10 */ s16 pendingMusicId;
    /* 0x12 */ u16 currentMusicId;
    /* 0x14 */ s16 musicFadeOutDuration;
    /* 0x16 */ s16 currentMusicVolume;
    /* 0x18 */ s16 targetMusicVolume;
    /* 0x1A */ s16 fadeCounter;
    /* 0x1C */ s8 musicVoiceIndex;
    /* 0x1D */ u8 musicFadeState;
    /* 0x1E */ u8 isFadingOut;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ s32 soundSequence;
    /* 0x24 */ void *soundEffectChannels[0xF];
    /* 0x60 */ s32 padding3;
    /* 0x64 */ s16 soundEffectIds[0x10];
    /* 0x84 */ s32 renderQueueCount;
    /* 0x88 */ RenderQueueItem renderQueue[32];
    /* 0x408 */ s32 bufferCount;
    /* 0x40C */ u8 bufferData[8][0x20];
    /* 0x50C */ s8 bufferIds[0x8];
    /* 0x514 */ s32 bufferFlags[8];
    /* 0x534 */ s32 audioInnerDistance;
    /* 0x538 */ s32 audioOuterDistance;
} GraphicsManager;
extern GraphicsManager *gGraphicsManager;

extern OSMesgQueue gfxTaskQueue;
extern OSMesgQueue gfxResultQueue;
extern s32 D_800937EC_943EC[];
extern s32 D_800937F0_943F0[];
extern s32 D_80093978_94578[];
extern s32 D_8009397C_9457C[];
extern s32 D_80093B00_94700;
extern s32 D_80092E28_93A28;
extern s32 D_80093308_93F08;

USE_ASSET(wavetables);

extern OSMesg gfxTaskQueueBuffer;
extern OSMesg gfxResultQueueBuffer;
extern u8 gMusicTrackVoiceMap[];
extern u8 D_80093BA5_947A5;
extern u8 D_80093BA6_947A6;
extern OSThread gfxCommThread;

void func_8005628C_56E8C(void);

void *startSoundEffect(s32, s32, s32, s32, s32);
void *func_80072960_73560(void *, s32);
void *func_80072A14_73614(void *);
void *func_80072CDC_738DC(void *, void *);

typedef struct {
    void *start;
    void *end;
    s32 size;
} GraphicsDataSegment;
extern GraphicsDataSegment D_800937E8_943E8[];
extern GraphicsDataSegment D_80093974_94574[];

void allocateAudioResources(void) {
    u8 sp10;

    gGraphicsManager = allocateTaskMemory(0x53C);
    gGraphicsManager->ptrBank = loadCompressedData(&_6A83F0_ROM_START, &_6A83F0_ROM_END, 0xCFD8);
    gGraphicsManager->musicBankBuffer = allocateMemoryNode(0, 0x9000, &sp10);
    gGraphicsManager->musicDataBuffer = allocateMemoryNode(0, 0x8000, &sp10);
    func_8006983C_6A43C(&initializeMusicSystem);
}

void initializeMusicSystem(void) {
    musConfig config;
    GraphicsManager *mgr;
    u16 negativeOne;
    void *newNode;
    s32 i;
    u8 alreadyAllocated;

    setupTaskSchedulerNodes(1, 0, 0, 0, 0, 0, 0, 0);
    config.control_flag = 0;
    config.channels = 0x18;
    config.thread_priority = 5;
    newNode = allocateMemoryNode(0, 0x37A20, &alreadyAllocated);
    config.heap = newNode;
    config.heap_length = 0x37A20;
    mgr = gGraphicsManager;
    config.fifo_length = 0x40;
    config.ptr = mgr->ptrBank;
    config.wbk = (u8 *)&wavetables_ROM_START;
    config.sched = &D_80092E28_93A28;
    config.default_fxbank = &D_80093308_93F08;
    config.syn_rsp_cmds = 0x5622;
    config.syn_output_rate = 0x100;
    config.syn_retraceCount = 0x1000;
    config.syn_num_dma_bufs = 1;
    config.syn_dma_buf_size = 0x64;
    config.diskrom_handle = (OSPiHandle *)0x400;
    MusInitialize(&config);

    D_80093BA5_947A5 = 1;
    negativeOne = -1;
    gGraphicsManager->musicFadeState = 0;
    gGraphicsManager->pendingMusicId = negativeOne;
    gGraphicsManager->currentMusicId = 0xFFFF;
    gGraphicsManager->musicFadeOutDuration = 0;
    scheduleTask(checkMusicLoadRequest, 0, 0, 0x64);
    gGraphicsManager->soundSequence = 0;
    for (i = 0xF; i >= 0; i--) {
        gGraphicsManager->soundEffectChannels[i] = 0;
    }

    gGraphicsManager->audioInnerDistance = 0x20;
    gGraphicsManager->renderQueueCount = 0;
    gGraphicsManager->bufferCount = 0;
    gGraphicsManager->audioOuterDistance = 0xC80;
    initializeGfxCommThread();
    func_8006983C_6A43C(&func_8005628C_56E8C);
}

INCLUDE_ASM("asm/nonmatchings/graphics", func_8005628C_56E8C);

void queueAnonymousBufferData(void *source) {
    if (gGraphicsManager->bufferCount < 8) {
        u8(*dest)[0x20];

        gGraphicsManager->bufferIds[gGraphicsManager->bufferCount] = -1;
        dest = &gGraphicsManager->bufferData[gGraphicsManager->bufferCount];
        dest++;
        dest--;

        memcpy(dest, source, 0x20);

        gGraphicsManager->bufferFlags[gGraphicsManager->bufferCount] = 0;
        gGraphicsManager->bufferCount += 1;
    }
}

void setAudioDistanceLimits(s32 innerDistance, s32 outerDistance) {
    gGraphicsManager->audioOuterDistance = outerDistance;
    gGraphicsManager->audioInnerDistance = innerDistance;
}

void queueBufferDataNoFlags(u8 *source, s8 bufferId) {
    s32 i;
    void *dest;

    for (i = 0; i < gGraphicsManager->bufferCount; i++) {
        if (gGraphicsManager->bufferIds[i] == bufferId) {
            dest = (void *)((i << 5) + (s32)gGraphicsManager + 0x40C);
            memcpy(dest, source, 0x20);
            return;
        }
    }

    if (gGraphicsManager->bufferCount < 8) {
        gGraphicsManager->bufferIds[gGraphicsManager->bufferCount] = bufferId;
        dest = (void *)((gGraphicsManager->bufferCount << 5) + (s32)gGraphicsManager + 0x40C);
        memcpy(dest, source, 0x20);
        gGraphicsManager->bufferFlags[gGraphicsManager->bufferCount] = 0;
        gGraphicsManager->bufferCount++;
    }
}

void setBufferData(void *source, u8 arg1, s32 arg2) {
    s32 i;
    s8 id;
    void *bufferPtr;

    // Search for existing buffer
    id = arg2;
    for (i = 0; i < gGraphicsManager->bufferCount; i++) {
        if (gGraphicsManager->bufferIds[i] == id) {
            bufferPtr = (void *)((i << 5) + (s32)gGraphicsManager + 0x40C);
            memcpy(bufferPtr, source, 0x20);
            return;
        }
    }

    // Add new buffer if space available
    if (gGraphicsManager->bufferCount < 8) {
        gGraphicsManager->bufferIds[gGraphicsManager->bufferCount] = id;

        bufferPtr = (void *)(((gGraphicsManager->bufferCount) << 5) + (s32)gGraphicsManager + 0x40C);

        memcpy(bufferPtr, source, 0x20);

        gGraphicsManager->bufferFlags[gGraphicsManager->bufferCount] = arg1;
        gGraphicsManager->bufferCount++;
    }
}

void queueSoundAtPosition(Vec3i *position, s16 soundId) {
    s32 index = gGraphicsManager->renderQueueCount;
    if (index < 0x20) {
        RenderQueueItem *renderQueue = gGraphicsManager->renderQueue;
        memcpy(&renderQueue[index].position, position, sizeof(Vec3i));
        index = soundId;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].soundId = index;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].duration = -1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].priority = 4;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].hasVolume = 0;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount++;
    }
}

void queueSoundAtPositionWithDuration(Vec3i *position, u32 soundId, s16 duration) {
    if (gGraphicsManager->renderQueueCount < 0x20) {
        RenderQueueItem *renderQueue = gGraphicsManager->renderQueue;

        memcpy(&renderQueue[gGraphicsManager->renderQueueCount].position, position, sizeof(Vec3i));

        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].soundId = soundId;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].duration = duration;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].priority = 4;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].hasVolume = 0;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount++;
    }
}

void queueSoundAtPositionWithPriority(Vec3i *position, s32 soundId, s16 priority, s16 duration) {
    RenderQueueItem *renderQueue;
    s32 index = gGraphicsManager->renderQueueCount;
    if (index < 0x20) {
        renderQueue = gGraphicsManager->renderQueue;
        memcpy(&renderQueue[index].position, position, sizeof(Vec3i));
        index = soundId;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].soundId = index;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].duration = duration;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].priority = priority;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].hasVolume = 0;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount += 1;
    }
}

void queueSoundAtPositionWithVolume(Vec3i *position, s32 soundId, f32 volume, s16 priority, s32 duration) {
    RenderQueueItem(*dest)[32];
    if (gGraphicsManager->renderQueueCount < 0x20) {
        dest = &gGraphicsManager->renderQueue;
        memcpy(&(*dest)[gGraphicsManager->renderQueueCount].position, position, sizeof(Vec3i));
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].soundId = soundId;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].duration = (s16)duration;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].priority = priority;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].hasVolume = 1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].volume = volume;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = 0x80;
        gGraphicsManager->renderQueueCount++;
    }
}

void queueSoundAtPositionWithVolumeAndFlags(
    Vec3i *position,
    s32 soundId,
    f32 volume,
    s16 priority,
    s32 duration,
    s32 flags
) {
    s32 index = gGraphicsManager->renderQueueCount;
    if (index < 0x20) {
        RenderQueueItem *renderQueue = gGraphicsManager->renderQueue;
        memcpy(&renderQueue[index].position, position, sizeof(Vec3i));
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].soundId = soundId;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].duration = (s16)duration;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].priority = priority;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].hasVolume = 1;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].volume = volume;
        gGraphicsManager->renderQueue[gGraphicsManager->renderQueueCount].flags = (s16)flags;
        gGraphicsManager->renderQueueCount++;
    }
}

void incrementSoundSequence(void) {
    gGraphicsManager->soundSequence = (s32)((gGraphicsManager->soundSequence + 1) & 0xFFFFFF);
}

void checkMusicLoadRequest(void *arg) {
    setCleanupCallback(checkMusicLoadRequest);
    if (gGraphicsManager->musicFadeState != 0) {
        setCallback(&loadMusicTrackData);
    }
}

void loadMusicTrackData(void) {
    gGraphicsManager->currentMusicId = gGraphicsManager->pendingMusicId;

    loadDataSegment(
        D_800937E8_943E8[gGraphicsManager->currentMusicId].start,
        D_800937E8_943E8[gGraphicsManager->currentMusicId].end,
        D_800937E8_943E8[gGraphicsManager->currentMusicId].size,
        gGraphicsManager->musicBankBuffer
    );

    loadDataSegment(
        D_80093974_94574[gGraphicsManager->currentMusicId].start,
        D_80093974_94574[gGraphicsManager->currentMusicId].end,
        D_80093974_94574[gGraphicsManager->currentMusicId].size,
        gGraphicsManager->musicDataBuffer
    );

    setCallback(&initializeMusicPtrBank);
}

void initializeMusicPtrBank(void) {
    initializeMusicPtrBankAsync(
        gGraphicsManager->musicDataBuffer,
        (void *)*(&D_80093B00_94700 + (gGraphicsManager->currentMusicId))
    );
    setCallback(&startMusicPlaybackWithFadeIn);
}

void startMusicPlaybackWithFadeIn(void) {
    s16 initialVolume;
    void *audioChannel;

    if ((u16)gGraphicsManager->pendingMusicId != gGraphicsManager->currentMusicId) {
        setCallbackWithContinue(checkMusicLoadRequest);
    } else if (gGraphicsManager->musicFadeState == 2) {
        audioChannel = startMusicPlaybackWithVoice(
            gGraphicsManager->musicDataBuffer,
            gGraphicsManager->musicBankBuffer,
            (u8)gGraphicsManager->musicVoiceIndex
        );
        gGraphicsManager->currentAudioChannel = audioChannel;
        if (audioChannel != 0) {
            initialVolume = gGraphicsManager->currentMusicVolume;
            if (initialVolume != 0x80) {
                setAudioChannelVolume(audioChannel, initialVolume);
            }
            gGraphicsManager->isFadingOut = 0;
            setCallbackWithContinue(updateMusicVolumeFadeIn);
        }
    }
}

void updateMusicVolumeFadeIn(void *arg) {
    s16 targetVolume;

    targetVolume = gGraphicsManager->targetMusicVolume;
    if (gGraphicsManager->currentMusicVolume != targetVolume) {
        if ((u16)gGraphicsManager->fadeCounter != 0) {
            gGraphicsManager->currentMusicVolume =
                gGraphicsManager->currentMusicVolume +
                ((s32)(targetVolume - gGraphicsManager->currentMusicVolume) / (s32)(u16)gGraphicsManager->fadeCounter);
            setAudioChannelVolume(gGraphicsManager->currentAudioChannel, (s32)gGraphicsManager->currentMusicVolume);
            gGraphicsManager->fadeCounter = (u16)gGraphicsManager->fadeCounter - 1;
        } else {
            gGraphicsManager->currentMusicVolume = targetVolume;
            setAudioChannelVolume(gGraphicsManager->currentAudioChannel, (s32)targetVolume);
        }
    }

    if ((u16)gGraphicsManager->pendingMusicId != gGraphicsManager->currentMusicId) {
        gGraphicsManager->isFadingOut = 1;
        stopAudioChannelWithSpeed(gGraphicsManager->currentAudioChannel, 8);
        setCallbackWithContinue(handleMusicFadeOutTransition);
    } else {
        if (gGraphicsManager->musicFadeState == 0) {
            gGraphicsManager->isFadingOut = 1;
            stopAudioChannelWithSpeed(
                gGraphicsManager->currentAudioChannel,
                (s32)(u16)gGraphicsManager->musicFadeOutDuration
            );
            setCallbackWithContinue(handleMusicFadeOutTransition);
        }
        if (getAudioChannelActiveState(gGraphicsManager->currentAudioChannel) == NULL) {
            gGraphicsManager->musicFadeState = 0;
            setCallbackWithContinue(checkNoActiveAudioChannels);
        }
    }
}

void handleMusicFadeOutTransition(void) {
    if (getAudioChannelActiveState(gGraphicsManager->currentAudioChannel) == 0) {
        setCallbackWithContinue(&checkNoActiveAudioChannels);
        return;
    }
    if ((gGraphicsManager->musicFadeState != 0) && (gGraphicsManager->isFadingOut == 0)) {
        gGraphicsManager->isFadingOut = 1;
        stopAudioChannelWithSpeed(gGraphicsManager->currentAudioChannel, 8);
    }
}

void checkNoActiveAudioChannels(void) {
    if (getActiveSongChannelCount() == NULL) {
        setCallbackWithContinue(&checkMusicLoadRequest);
    }
}

void playMusicTrack(s32 musicTrackId) {
    gGraphicsManager->musicFadeState = 2;
    gGraphicsManager->pendingMusicId = musicTrackId;
    gGraphicsManager->currentMusicVolume = 0x80;
    gGraphicsManager->targetMusicVolume = 0x80;
    gGraphicsManager->fadeCounter = 0;
    gGraphicsManager->musicVoiceIndex = (s8)gMusicTrackVoiceMap[musicTrackId];
}

void playMusicTrackWithVoice(s16 musicTrackId, s8 voiceIndex) {
    gGraphicsManager->musicFadeState = 2;
    gGraphicsManager->pendingMusicId = musicTrackId;
    gGraphicsManager->currentMusicVolume = 0x80;
    gGraphicsManager->targetMusicVolume = 0x80;
    gGraphicsManager->fadeCounter = 0;
    gGraphicsManager->musicVoiceIndex = voiceIndex;
}

void playMusicTrackWithFadeIn(u32 musicTrackId, u16 targetVolume, u16 fadeDuration) {
    gGraphicsManager->musicFadeState = 2;
    gGraphicsManager->pendingMusicId = musicTrackId;
    gGraphicsManager->currentMusicVolume = 0;
    gGraphicsManager->targetMusicVolume = targetVolume;
    gGraphicsManager->fadeCounter = fadeDuration;
    gGraphicsManager->musicVoiceIndex = gMusicTrackVoiceMap[musicTrackId];
}

void setMusicVolumeFade(u16 targetVolume, u16 fadeDuration) {
    gGraphicsManager->targetMusicVolume = targetVolume;
    gGraphicsManager->fadeCounter = fadeDuration;
}

void setMusicFadeOut(s32 fadeOutDuration) {
    s32 duration;

    duration = fadeOutDuration;
    if (fadeOutDuration < 8) {
        duration = 8;
    }
    gGraphicsManager->musicFadeState = 0;
    gGraphicsManager->musicFadeOutDuration = duration;
}

void initializeGfxCommThread(void) {
    OSMesgQueue *queue = &gfxTaskQueue;
    osCreateMesgQueue(queue, &gfxTaskQueueBuffer, OS_MESG_BLOCK);
    queue = &gfxResultQueue;
    osCreateMesgQueue(queue, &gfxResultQueueBuffer, OS_MESG_BLOCK);
    osCreateThread(&gfxCommThread, 0xB, gfxCommThreadFunc, 0, &gfxTaskQueue, 6);
    osStartThread(&gfxCommThread);
}

void gfxCommThreadFunc(void *arg0) {
    void *message;
    void *result;

    while (TRUE) {
        osRecvMesg(&gfxTaskQueue, &message, 1);
        if ((u32)message < 0xF) {
            switch ((s32)message) {
                case 0:
                    result = (void *)func_800728E0_734E0(3);
                    break;
                case 1:
                    result = startSoundEffect(
                        gGraphicsCommand.soundId,
                        gGraphicsCommand.volume,
                        gGraphicsCommand.pan,
                        0,
                        gGraphicsCommand.soundSequence
                    );
                    if (gGraphicsCommand.voiceIndex != 0) {
                        func_80072C38_73838(result, gGraphicsCommand.voiceIndex);
                    }
                    break;
                case 2:
                    func_80072960_73560(gGraphicsCommand.audioChannel, 0);
                    result = startSoundEffect(
                        gGraphicsCommand.soundId,
                        gGraphicsCommand.volume,
                        gGraphicsCommand.pan,
                        0,
                        gGraphicsCommand.soundSequence
                    );
                    if (gGraphicsCommand.voiceIndex != 0) {
                        func_80072C38_73838(result, gGraphicsCommand.voiceIndex);
                    }
                    break;
                case 3:
                    result = func_80072960_73560(gGraphicsCommand.audioChannel, 0);
                    break;
                case 4:
                    func_80072960_73560(gGraphicsCommand.audioChannel, 0);
                    result = startSoundEffect(
                        gGraphicsCommand.soundId,
                        gGraphicsCommand.volume,
                        gGraphicsCommand.pan,
                        0,
                        gGraphicsCommand.soundSequence
                    );
                    func_80072B3C_7373C(result, gGraphicsCommand.position);
                    if (gGraphicsCommand.voiceIndex != 0) {
                        func_80072C38_73838(result, gGraphicsCommand.voiceIndex);
                    }
                    break;
                case 5:
                    func_80072A6C_7366C(gGraphicsCommand.audioChannel, gGraphicsCommand.volume);
                    func_80072ACC_736CC(gGraphicsCommand.audioChannel, gGraphicsCommand.pan);
                    func_80072B3C_7373C(gGraphicsCommand.audioChannel, gGraphicsCommand.position);
                    if (gGraphicsCommand.voiceIndex != 0) {
                        func_80072C38_73838(gGraphicsCommand.audioChannel, gGraphicsCommand.voiceIndex);
                    }
                    break;
                case 6:
                    func_80072814_73414(1, gGraphicsCommand.stoppingSpeed);
                    break;
                case 7:
                    result = func_80072960_73560(gGraphicsCommand.audioChannel, gGraphicsCommand.stoppingSpeed);
                    break;
                case 8:
                    result = (void *)func_800728E0_734E0(1);
                    break;
                case 9:
                    result = func_80072A14_73614(gGraphicsCommand.audioChannel);
                    break;
                case 10:
                    func_80072CC0_738C0(gGraphicsCommand.ptrBank, gGraphicsCommand.waveBank);
                    break;
                case 11:
                    result = func_80072CDC_738DC(gGraphicsCommand.ptrBank, gGraphicsCommand.musicBankBuffer);
                    if (gGraphicsCommand.voiceIndex != 0) {
                        func_80072C38_73838(result, gGraphicsCommand.voiceIndex);
                    }
                    break;
                case 12:
                    func_80072A6C_7366C(gGraphicsCommand.audioChannel, gGraphicsCommand.volume);
                    break;
                case 13:
                    func_80072814_73414(3, gGraphicsCommand.stoppingSpeed);
                    break;
                case 14:
                    result = (void *)func_800728E0_734E0(2);
                    break;
            }
        }

        osSendMesg(&gfxResultQueue, result, OS_MESG_BLOCK);
    }
}

void sendStopAudioChannelsCommand(s32 stoppingSpeed) {
    void *message;

    if ((D_80093BA5_947A5 != NULL) && (D_80093BA6_947A6 == NULL)) {
        D_80093BA6_947A6++;
        gGraphicsCommand.stoppingSpeed = stoppingSpeed;
        osSendMesg(&gfxTaskQueue, (OSMesg *)0xD, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    }
}

void stopAudioChannelWithSpeed(void *audioChannel, s32 stoppingSpeed) {
    void *result;

    gGraphicsCommand.audioChannel = audioChannel;
    gGraphicsCommand.stoppingSpeed = stoppingSpeed;
    osSendMesg(&gfxTaskQueue, (OSMesg *)7, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &result, OS_MESG_BLOCK);
}

void setAudioChannelVolume(void *audioChannel, s32 volume) {
    void *message;

    gGraphicsCommand.audioChannel = audioChannel;
    gGraphicsCommand.volume = volume;
    osSendMesg(&gfxTaskQueue, (OSMesg *)0xC, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void *startMusicPlaybackWithVoice(void *musicDataBuffer, void *musicBankBuffer, s32 voiceIndex) {
    void *musicHandle;

    gGraphicsCommand.ptrBank = musicDataBuffer;
    gGraphicsCommand.musicBankBuffer = musicBankBuffer;
    gGraphicsCommand.voiceIndex = voiceIndex;
    osSendMesg(&gfxTaskQueue, (void *)0xB, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &musicHandle, OS_MESG_BLOCK);
    return musicHandle;
}

void *startMusicPlayback(void *musicDataBuffer, void *musicBankBuffer) {
    return startMusicPlaybackWithVoice(musicDataBuffer, musicBankBuffer, 0);
}

void initializeMusicPtrBankAsync(void *ptrBank, void *waveBank) {
    void *message;

    gGraphicsCommand.ptrBank = ptrBank;
    gGraphicsCommand.waveBank = waveBank;
    osSendMesg(&gfxTaskQueue, (OSMesg *)0xA, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void *getAudioChannelActiveState(void *audioChannel) {
    void *message;

    gGraphicsCommand.audioChannel = audioChannel;
    osSendMesg(&gfxTaskQueue, (OSMesg *)9, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    return message;
}

void *getActiveEffectChannelCount(void) {
    void *channelCount;

    osSendMesg(&gfxTaskQueue, (OSMesg *)8, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &channelCount, OS_MESG_BLOCK);
    return channelCount;
}

void stopSoundEffectChannel(s32 channelIndex, s32 stoppingSpeed) {
    void *channel;
    OSMesg message;

    channel = gGraphicsManager->soundEffectChannels[channelIndex];
    gGraphicsCommand.stoppingSpeed = stoppingSpeed;
    gGraphicsCommand.audioChannel = channel;
    osSendMesg(&gfxTaskQueue, (OSMesg *)7, 1);
    osRecvMesg(&gfxResultQueue, &message, 1);
}

void stopAllSoundEffectsAndClearQueues(s32 stoppingSpeed) {
    void *message;

    gGraphicsCommand.stoppingSpeed = stoppingSpeed;
    osSendMesg(&gfxTaskQueue, (OSMesg *)6, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    gGraphicsManager->renderQueueCount = 0;
    gGraphicsManager->bufferCount = 0;
}

void playSoundEffectAtPositionWithPriority(
    s32 soundId,
    s32 volume,
    s32 pan,
    f32 position,
    s32 priority,
    s32 channelIndex,
    s32 voiceIndex
) {
    void *message;

    gGraphicsCommand.audioChannel = gGraphicsManager->soundEffectChannels[channelIndex];
    if (volume > 0) {
        gGraphicsCommand.pan = pan;
        gGraphicsCommand.volume = volume;
        gGraphicsCommand.position = position;
        gGraphicsCommand.voiceIndex = voiceIndex;

        if (gGraphicsManager->soundEffectChannels[channelIndex] == 0 ||
            gGraphicsManager->soundEffectIds[channelIndex] != soundId) {
            gGraphicsCommand.soundId = soundId;
            gGraphicsCommand.soundSequence = gGraphicsManager->soundSequence + (priority << 0x18);
            osSendMesg(&gfxTaskQueue, (void *)4, 1);
            osRecvMesg(&gfxResultQueue, &message, 1);
            gGraphicsManager->soundEffectChannels[channelIndex] = message;
            gGraphicsManager->soundEffectIds[channelIndex] = (s16)soundId;
            incrementSoundSequence();
            return;
        }

        osSendMesg(&gfxTaskQueue, (OSMesg *)5, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    } else {
        osSendMesg(&gfxTaskQueue, (OSMesg *)3, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    }
}

void playSoundEffectAtPosition(s32 soundId, s32 volume, s32 pan, f32 position, s32 priority, s32 channelIndex) {
    playSoundEffectAtPositionWithPriority(soundId, volume, pan, position, priority, channelIndex, 0xC);
}

// Play sound effect on specified channel with voice control
// Called by non-matching assembly func_8005628C_56E8C
void playSoundEffectOnChannelWithVoice(
    s32 soundId,
    s32 volume,
    s32 pan,
    s32 priority,
    s32 channelIndex,
    s32 voiceIndex
) {
    void *message;

    gGraphicsCommand.audioChannel = gGraphicsManager->soundEffectChannels[channelIndex];
    if (volume > 0) {
        gGraphicsCommand.soundId = soundId;
        gGraphicsCommand.volume = volume;
        gGraphicsCommand.pan = pan;
        gGraphicsCommand.voiceIndex = voiceIndex;
        gGraphicsCommand.soundSequence = gGraphicsManager->soundSequence + (priority << 0x18);
        osSendMesg(&gfxTaskQueue, (void *)2, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
        gGraphicsManager->soundEffectChannels[channelIndex] = message;
        gGraphicsManager->soundEffectIds[channelIndex] = (s16)soundId;
        incrementSoundSequence();
        return;
    }
    osSendMesg(&gfxTaskQueue, (void *)3, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

// Play sound effect on specified channel with default voice
void playSoundEffectOnChannel(s32 soundId, s32 volume, s32 pan, s32 priority, s32 channelIndex) {
    playSoundEffectOnChannelWithVoice(soundId, volume, pan, priority, channelIndex, 0xC);
}

void func_80057E18_58A18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 new_var;
    s32 *new_var3;
    void *sp10;

    gGraphicsCommand.audioChannel = gGraphicsManager->soundEffectChannels[arg3];
    new_var3 = &gGraphicsManager->soundSequence;

    if (arg1 > 0) {
        gGraphicsCommand.soundId = arg0;
        gGraphicsCommand.volume = arg1;
        gGraphicsCommand.pan = 0x80;
        new_var = *new_var3;
        gGraphicsCommand.voiceIndex = arg4;
        gGraphicsCommand.soundSequence = new_var + (arg2 << 0x18);
        osSendMesg(&gfxTaskQueue, (void *)2, 1);
        osRecvMesg(&gfxResultQueue, &sp10, 1);
        gGraphicsManager->soundEffectChannels[arg3] = sp10;
        gGraphicsManager->soundEffectIds[arg3] = (s16)arg0;
        incrementSoundSequence();
        return;
    }

    osSendMesg(&gfxTaskQueue, (void *)3, 1);
    osRecvMesg(&gfxResultQueue, &sp10, 1);
}

void func_80057F28_58B28(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80057E18_58A18(arg0, arg1, arg2, arg3, 0xC);
}

void func_80057F48_58B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;

    gGraphicsCommand.audioChannel = gGraphicsManager->soundEffectChannels[arg2];
    if (arg1 > 0) {
        gGraphicsCommand.soundId = arg0;
        gGraphicsCommand.volume = arg1;
        gGraphicsCommand.pan = 0x80;
        gGraphicsCommand.soundSequence = gGraphicsManager->soundSequence;
        gGraphicsCommand.voiceIndex = arg3;
        osSendMesg(&gfxTaskQueue, (void *)2, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
        gGraphicsManager->soundEffectChannels[arg2] = message;
        gGraphicsManager->soundEffectIds[arg2] = (s16)arg0;
        incrementSoundSequence();
        return;
    }
    osSendMesg(&gfxTaskQueue, (void *)3, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
}

void func_80058048_58C48(s32 arg0, s32 arg1, s32 arg2) {
    func_80057F48_58B48(arg0, arg1, arg2, 0xC);
}

void func_80058064_58C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;

    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = 0x80;
    gGraphicsCommand.pan = 0x80;
    gGraphicsCommand.soundSequence = gGraphicsManager->soundSequence + (arg1 << 0x18);
    gGraphicsCommand.audioChannel = gGraphicsManager->soundEffectChannels[arg2];
    gGraphicsCommand.voiceIndex = arg3;
    osSendMesg(&gfxTaskQueue, (OSMesg *)2, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    gGraphicsManager->soundEffectChannels[arg2] = message;
    gGraphicsManager->soundEffectIds[arg2] = (s16)arg0;
    incrementSoundSequence();
}

void func_80058138_58D38(s32 arg0, s32 arg1, s32 arg2) {
    func_80058064_58C64(arg0, arg1, arg2, 0xC);
}

void func_80058154_58D54(s32 arg0, s32 arg1, s32 arg2) {
    GraphicsManager *temp_v1;
    void *message;
    s32 *new_var2;
    void *temp_v0;

    temp_v1 = gGraphicsManager;
    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = 0x80;
    gGraphicsCommand.pan = 0x80;

    new_var2 = &temp_v1->soundSequence;
    temp_v0 = (void *)*new_var2;
    gGraphicsCommand.soundSequence = temp_v1->soundSequence;
    temp_v0 = temp_v1->soundEffectChannels[arg1];
    gGraphicsCommand.voiceIndex = arg2;
    gGraphicsCommand.audioChannel = temp_v0;

    osSendMesg(&gfxTaskQueue, (OSMesg *)2, 1);
    osRecvMesg(&gfxResultQueue, (OSMesg *)&message, 1);

    gGraphicsManager->soundEffectChannels[arg1] = message;
    gGraphicsManager->soundEffectIds[arg1] = arg0;

    incrementSoundSequence();
}

void func_80058220_58E20(s32 arg0, s32 arg1) {
    func_80058154_58D54(arg0, arg1, 0xC);
}

void func_8005823C_58E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *message;
    GraphicsManager *new_var;
    s32 *new_var2;
    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = arg1;
    gGraphicsCommand.pan = 0x80;
    new_var = gGraphicsManager;
    new_var2 = &new_var->soundSequence;
    gGraphicsCommand.soundSequence = (*new_var2) + (arg2 << 0x18);
    gGraphicsCommand.voiceIndex = arg3;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    incrementSoundSequence();
}

void func_800582C0_58EC0(s32 arg0, s32 arg1, s32 arg2) {
    func_8005823C_58E3C(arg0, arg1, arg2, 0xC);
}

void func_800582DC_58EDC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    void *message;

    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = arg1;
    gGraphicsCommand.pan = arg2;
    gGraphicsCommand.soundSequence = gGraphicsManager->soundSequence + (arg3 << 0x18);
    gGraphicsCommand.voiceIndex = arg4;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    incrementSoundSequence();
}

void func_80058360_58F60(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800582DC_58EDC(arg0, arg1, arg2, arg3, 0xC);
}

void func_80058380_58F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *new_var;
    void *message;
    s32 new_var2;
    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = arg1;
    gGraphicsCommand.pan = arg2;
    new_var = &gGraphicsManager->soundSequence;
    new_var2 = *new_var;
    gGraphicsCommand.voiceIndex = arg3;
    gGraphicsCommand.soundSequence = new_var2;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, &message, OS_MESG_BLOCK);
    incrementSoundSequence();
}

void func_800583F8_58FF8(s32 arg0, s32 arg1, s32 arg2) {
    func_80058380_58F80(arg0, arg1, arg2, 0xC);
}

void func_80058414_59014(s32 arg0, s32 arg1, s32 arg2) {
    void *sp10;
    s32 *new_var;
    s32 new_var2;
    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = arg1;
    gGraphicsCommand.pan = 0x80;
    new_var = &gGraphicsManager->soundSequence;
    new_var2 = *new_var;
    gGraphicsCommand.voiceIndex = arg2;
    gGraphicsCommand.soundSequence = new_var2;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    incrementSoundSequence();
}

void func_80058490_59090(s32 arg0, s32 arg1) {
    func_80058414_59014(arg0, arg1, 0xC);
}

void func_800584AC_590AC(s32 arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 mesg;

    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = 0x80;
    gGraphicsCommand.pan = 0x80;
    v0 = *(s32 *)((s32)gGraphicsManager + 0x20);
    gGraphicsCommand.voiceIndex = arg2;
    gGraphicsCommand.soundSequence = v0 + (arg1 << 24);

    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, (OSMesg *)&mesg, OS_MESG_BLOCK);
    incrementSoundSequence();
}

void func_80058530_59130(s32 arg0, s32 arg1) {
    func_800584AC_590AC(arg0, arg1, 0xC);
}

void func_8005854C_5914C(s32 arg0, s32 arg1) {
    s32 new_var2;
    s32 *new_var;
    void *sp10;

    gGraphicsCommand.soundId = arg0;
    gGraphicsCommand.volume = 0x80;
    gGraphicsCommand.pan = 0x80;
    new_var2 = *(new_var = &gGraphicsManager->soundSequence);
    gGraphicsCommand.voiceIndex = arg1;
    gGraphicsCommand.soundSequence = new_var2;
    osSendMesg(&gfxTaskQueue, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&gfxResultQueue, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    incrementSoundSequence();
}

void func_800585C8_591C8(s32 arg0) {
    func_8005854C_5914C(arg0, 0xC);
}

void *getActiveAudioChannelCount(void) {
    void *channelCount;

    if (D_80093BA5_947A5 != 0) {
        osSendMesg(&gfxTaskQueue, 0, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &channelCount, OS_MESG_BLOCK);
        return channelCount;
    }

    return 0;
}

void *getActiveSongChannelCount(void) {
    void *channelCount;

    if (D_80093BA5_947A5 != NULL) {
        osSendMesg(&gfxTaskQueue, (OSMesg *)0xE, OS_MESG_BLOCK);
        osRecvMesg(&gfxResultQueue, &channelCount, OS_MESG_BLOCK);

        return channelCount;
    }

    return NULL;
}
