#include "1DFAA0.h"
#include "20F0.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "graphics.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_1FB4E0);
USE_ASSET(_4C9E70);

typedef struct {
    u8 _pad0[0x20];
    StateEntry *unk20;
    StateEntry *unk24;
    StateEntry *unk28;
    u32 unk2C;
    u8 _pad30[0x2C];
    StateEntry *unk5C;
    StateEntry *unk60;
    StateEntry *unk64;
    u32 unk68;
    u8 _pad6C[0x18];
    s16 unk84;
} FanEffectTaskState;

typedef struct {
    u8 _pad0[0x18];
    s32 unk18;
    u8 _pad1C[0x20];
    u8 unk3C[0x40];
    s32 unk7C;
    u8 _pad80[0x2];
    u16 unk82;
} FanEffectFadeState;

typedef struct {
    u8 _pad0[0x3C];
    u8 unk3C[0x3C];
    void **unk78;
    s32 unk7C;
    u16 unk80;
    u16 unk82;
    s16 unk84;
    s16 unk86;
} FanEffectGrowState;

typedef struct {
    u8 padding[0x78];
    s32 unk78;
    u8 padding2[4];
    s16 unk80;
    u8 padding3[4];
    s16 unk86;
} FanEffectTask;

void updateFanEffectGrow(FanEffectGrowState *);
void cleanupFanEffectTask(SceneModel_unk98 *);
void updateFanEffectFade(FanEffectFadeState *arg0);

extern Transform3D D_8009A8B0_9B4B0;
extern StateEntry D_80088650;
extern StateEntry D_80088660;
extern s16 gFanSoundCount;
extern s16 gFanSoundIds[];

s16 getFanSoundCount(void) {
    return gFanSoundCount;
}

s16 getFanSoundId(s16 fanIndex) {
    s16 count = gFanSoundCount;
    s16 index;

    if (fanIndex < count) {
        index = fanIndex;
    } else {
        index = 0;
    }

    return gFanSoundIds[index];
}

void playFanSoundEffect(s16 fanIndex, s16 volume, s16 pan, s16 channel) {
    s16 soundId;

    if (channel <= 0) {
        soundId = getFanSoundId(fanIndex);
        playSoundEffectWithPriorityAndPan(soundId, volume, pan + 0x80, 0);
    } else {
        soundId = getFanSoundId(fanIndex);
        playSoundEffectOnChannel(soundId, volume, pan + 0x80, 0, channel);
    }
}

void playFanSoundAtPosition(s16 fanIndex, s16 arg1, s16 duration, CutsceneSlotData *cutsceneSlotData) {
    Vec3i position;
    s16 soundId;

    if (cutsceneSlotData != NULL) {
        memcpy(&position, &cutsceneSlotData->unk2C, sizeof(Vec3i));
    } else {
        position.x = 0;
        position.y = 0;
        position.z = 0;
    }

    if (duration <= 0) {
        soundId = getFanSoundId(fanIndex);
        queueSoundAtPosition(&position, soundId);
        return;
    }

    soundId = getFanSoundId(fanIndex);
    queueSoundAtPositionWithPriority(&position, soundId, 0, duration);
}

void playFanSoundOnChannel0(s16 fanIndex) {
    s16 soundId;

    soundId = getFanSoundId(fanIndex);
    playSoundEffectOnChannelNoPriority(soundId, 0);
}

void stopFanSoundOnChannel0(void) {
    stopSoundEffectChannel(0, 0);
}

void initFanEffectTask(FanEffectTaskState *arg0) {
    StateEntry **temp_a1;

    arg0->unk20 = &D_80088650;
    arg0->unk24 = loadUncompressedData(&_1FB4E0_ROM_START, &_1FB4E0_ROM_END);

    arg0->unk28 = loadCompressedData(&_4C9E70_ROM_START, &_4C9E70_ROM_END, 0xA10);
    temp_a1 = &arg0->unk24;

    arg0->unk5C = &D_80088660;
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk84 = 0x200;
    arg0->unk60 = *temp_a1;
    arg0->unk64 = arg0->unk28;

    setCleanupCallback(&cleanupFanEffectTask);
    setCallbackWithContinue(&updateFanEffectGrow);
}

void updateFanEffectGrow(FanEffectGrowState *arg0) {
    Transform3D sp10;
    void *temp_v0;
    s16 temp_v0_2;
    s16 temp_a0;

    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);

    temp_v0 = (void *)((u8 *)(*arg0->unk78) + 0x3C0);

    func_8006B084_6BC84(&D_8009A8B0_9B4B0, temp_v0, arg0);

    scaleMatrix((Transform3D *)arg0, arg0->unk84, arg0->unk84, arg0->unk84);

    temp_v0_2 = arg0->unk82 + 0x300;
    arg0->unk82 = temp_v0_2;

    createZRotationMatrix(&sp10, temp_v0_2);

    sp10.translation.y = 0xBB333;
    sp10.translation.x = 0;
    sp10.translation.z = 0xFFEA0000;

    func_8006B084_6BC84(&sp10, arg0, &arg0->unk3C);

    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    enqueueDisplayListObject(0, (DisplayListObject *)&arg0->unk3C);

    if (arg0->unk86 != 0) {
        arg0->unk86--;
        temp_a0 = arg0->unk84;
        if (temp_a0 != 0x2000) {
            arg0->unk84 = temp_a0 + 0x200;
        }
    } else {
        arg0->unk7C = 0x40000;
        setCallback(&updateFanEffectFade);
    }
}

void updateFanEffectFade(FanEffectFadeState *arg0) {
    Transform3D matrix;
    s32 temp_v0;

    temp_v0 = arg0->unk7C - 0x8000;
    arg0->unk7C = temp_v0;

    if ((s32)0xFFF80000 >= temp_v0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk18 += arg0->unk7C;
    createZRotationMatrix(&matrix, arg0->unk82);

    matrix.translation.y = 0xBB333;
    matrix.translation.x = 0;
    matrix.translation.z = 0xFFEA0000;

    func_8006B084_6BC84(&matrix, (DisplayListObject *)arg0, (DisplayListObject *)&arg0->unk3C);
    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    enqueueDisplayListObject(0, (DisplayListObject *)&arg0->unk3C);
}

void cleanupFanEffectTask(SceneModel_unk98 *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void spawnFanEffect(s32 displayList, s16 frames) {
    FanEffectTask *task = (FanEffectTask *)scheduleTask(&initFanEffectTask, 1, 0, 0x64);

    if (task != NULL) {
        task->unk78 = displayList;
        task->unk80 = 0;
        task->unk86 = frames;
    }
}
