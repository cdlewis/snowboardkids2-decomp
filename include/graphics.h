#pragma once

#include "common.h"
#include "geometry.h"

void allocateAudioResources(void);
void initializeMusicSystem(void);
void queueAnonymousBufferData(void *source);
void setAudioDistanceLimits(s32 innerDistance, s32 outerDistance);
void queueBufferDataNoFlags(u8 *source, s8 bufferId);
void setBufferData(void *source, u8 arg1, s32 arg2);
void queueSoundAtPosition(Vec3i *position, s16 soundId);
void queueSoundAtPositionWithDuration(Vec3i *position, u32 soundId, s16 duration);
void queueSoundAtPositionWithPriority(Vec3i *position, s32 soundId, s16 priority, s16 duration);
void queueSoundAtPositionWithVolume(Vec3i *position, s32 soundId, f32 volume, s16 priority, s32 duration);
void queueSoundAtPositionWithVolumeAndFlags(Vec3i *position, s32 soundId, f32 volume, s16 priority, s32 duration, s32 flags);
void incrementSoundSequence(void);
void checkMusicLoadRequest(void *arg);
void loadMusicTrackData(void);
void initializeMusicPtrBank(void);
void startMusicPlaybackWithFadeIn(void);
void updateMusicVolumeFadeIn(void *arg);
void handleMusicFadeOutTransition(void);
void checkNoActiveAudioChannels(void);
void playMusicTrack(s32 musicTrackId);
void playMusicTrackWithVoice(s16 musicTrackId, s8 voiceIndex);
void playMusicTrackWithFadeIn(u32 musicTrackId, u16 targetVolume, u16 fadeDuration);
void setMusicVolumeFade(u16 targetVolume, u16 fadeDuration);
void setMusicFadeOut(s32 fadeOutDuration);
void initializeGfxCommThread(void);
void gfxCommThreadFunc(void *arg0);
void sendStopAudioChannelsCommand(s32 stoppingSpeed);
void stopAudioChannelWithSpeed(void *audioChannel, s32 stoppingSpeed);
void setAudioChannelVolume(void *audioChannel, s32 volume);
void *startMusicPlaybackWithVoice(void *musicDataBuffer, void *musicBankBuffer, s32 voiceIndex);
void *startMusicPlayback(void *musicDataBuffer, void *musicBankBuffer);
void initializeMusicPtrBankAsync(void *ptrBank, void *waveBank);
void *getAudioChannelActiveState(void *audioChannel);
void *getActiveEffectChannelCount(void);
void stopSoundEffectChannel(s32 channelIndex, s32 stoppingSpeed);
void stopAllSoundEffectsAndClearQueues(s32 stoppingSpeed);
void playSoundEffectAtPositionWithPriority(
    s32 soundId,
    s32 volume,
    s32 pan,
    f32 position,
    s32 priority,
    s32 channelIndex,
    s32 voiceIndex
);
void playSoundEffectAtPosition(
    s32 soundId,
    s32 volume,
    s32 pan,
    f32 position,
    s32 priority,
    s32 channelIndex
);
// Play sound effect on specified channel with voice control
void func_80057CE4_588E4(s32 soundId, s32 volume, s32 pan, s32 priority, s32 channelIndex, s32 voiceIndex);
// Wrapper that uses default voice (0xC)
void func_80057DF0_589F0(s32 soundId, s32 volume, s32 pan, s32 priority, s32 channelIndex);
void func_80057E18_58A18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80057F28_58B28(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80057F48_58B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80058048_58C48(s32 arg0, s32 arg1, s32 arg2);
void func_80058064_58C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80058138_58D38(s32 arg0, s32 arg1, s32 arg2);
void func_80058154_58D54(s32 arg0, s32 arg1, s32 arg2);
void func_80058220_58E20(s32 arg0, s32 arg1);
void func_8005823C_58E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800582C0_58EC0(s32 arg0, s32 arg1, s32 arg2);
void func_800582DC_58EDC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80058360_58F60(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80058380_58F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800583F8_58FF8(s32 arg0, s32 arg1, s32 arg2);
void func_80058414_59014(s32 arg0, s32 arg1, s32 arg2);
void func_80058490_59090(s32 arg0, s32 arg1);
void func_800584AC_590AC(s32 arg0, s32 arg1, s32 arg2);
void func_80058530_59130(s32 arg0, s32 arg1);
void func_8005854C_5914C(s32 arg0, s32 arg1);
void func_800585C8_591C8(s32 arg0);
void *getActiveAudioChannelCount(void);
void *getActiveSongChannelCount(void);
