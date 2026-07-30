#pragma once

#include "common.h"
#include "math/geometry.h"

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
    f32 frequencyOffset;
    s32 voiceIndex;
    s32 padding2C;
} AudioCommand;

typedef struct {
    s16 soundId;
    s16 channelIndex;
    s16 priority;
    s16 hasFrequencyOffset;
    f32 frequencyOffset;
    Vec3i position;
    s16 baseVolume;
    u8 padding[2];
} SpatialSoundRequest;

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
    /* 0x1F */ u8 padding1F;
    /* 0x20 */ s32 soundSequence;
    /* 0x24 */ void *soundEffectChannels[16];
    /* 0x64 */ s16 soundEffectIds[16];
    /* 0x84 */ s32 spatialSoundCount;
    /* 0x88 */ SpatialSoundRequest spatialSounds[32];
    /* 0x408 */ s32 listenerCount;
    /* 0x40C */ Transform3D listenerTransforms[8];
    /* 0x50C */ s8 listenerIds[8];
    /* 0x514 */ s32 listenerVoiceIndices[8];
    /* 0x534 */ s32 audioInnerDistance;
    /* 0x538 */ s32 audioOuterDistance;
} SoundManager;

void allocateAudioResources(void);
void initializeMusicSystem(void);
void queueAnonymousBufferData(Transform3D *source);
void setAudioDistanceLimits(s32 innerDistance, s32 outerDistance);
void queueBufferDataNoFlags(Transform3D *source, s8 listenerId);
void setBufferData(Transform3D *source, u8 voiceIndex, s32 listenerId);
void queueSoundAtPosition(Vec3i *position, s16 soundId);
void queueSoundAtPositionWithDuration(Vec3i *position, u32 soundId, s16 channelIndex);
void queueSoundAtPositionWithPriority(Vec3i *position, s32 soundId, s16 priority, s16 channelIndex);
void queueSoundAtPositionWithVolume(Vec3i *position, s32 soundId, f32 frequencyOffset, s16 priority, s32 channelIndex);
void queueSoundAtPositionWithVolumeAndFlags(
    Vec3i *position,
    s32 soundId,
    f32 frequencyOffset,
    s16 priority,
    s32 channelIndex,
    s32 baseVolume
);
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
void initializeAudioCommandThread(void);
void audioCommandThreadFunc(void *arg0);
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
void playSoundEffectAtPosition(s32 soundId, s32 volume, s32 pan, f32 position, s32 priority, s32 channelIndex);
// Play sound effect on specified channel with voice control
void playSoundEffectOnChannelWithVoice(
    s32 soundId,
    s32 volume,
    s32 pan,
    s32 priority,
    s32 channelIndex,
    s32 voiceIndex
);
// Play sound effect on specified channel with default voice
void playSoundEffectOnChannel(s32 soundId, s32 volume, s32 pan, s32 priority, s32 channelIndex);
void playOrStopSoundEffectOnChannelWithVoice(s32 soundId, s32 volume, s32 priority, s32 channelIndex, s32 voiceIndex);
void playOrStopSoundEffectOnChannel(s32 soundId, s32 volume, s32 priority, s32 channelIndex);
void playOrStopSoundEffectNoPriorityWithVoice(s32 soundId, s32 volume, s32 channelIndex, s32 voiceIndex);
void playOrStopSoundEffectNoPriority(s32 soundId, s32 volume, s32 channelIndex);
void playSoundEffectOnChannelWithPriorityAndVoice(s32 soundId, s32 priority, s32 channelIndex, s32 voiceIndex);
void playSoundEffectOnChannelWithPriority(s32 soundId, s32 priority, s32 channelIndex);
void playSoundEffectOnChannelNoPriorityWithVoice(s32 soundId, s32 channelIndex, s32 voiceIndex);
void playSoundEffectOnChannelNoPriority(s32 soundId, s32 channelIndex);
void playSoundEffectWithPriorityAndVoice(s32 soundId, s32 volume, s32 priority, s32 voiceIndex);
void playSoundEffectWithPriority(s32 soundId, s32 volume, s32 priority);
void playSoundEffectWithPriorityPanAndVoice(s32 soundId, s32 volume, s32 pan, s32 priority, s32 voiceIndex);
void playSoundEffectWithPriorityAndPan(s32 soundId, s32 volume, s32 pan, s32 priority);
void playSoundEffectWithVolumePanAndVoice(s32 soundId, s32 volume, s32 pan, s32 voiceIndex);
void playSoundEffectWithVolumeAndPan(s32 soundId, s32 volume, s32 pan);
void playSoundEffectWithVolumeAndVoice(s32 soundId, s32 volume, s32 voiceIndex);
void playSoundEffectWithVolume(s32 soundId, s32 volume);
void playSoundEffectWithPriorityAndVoiceDefaultVolume(s32 soundId, s32 priority, s32 voiceIndex);
void playSoundEffectWithPriorityDefaultVolume(s32 soundId, s32 priority);
void playSoundEffectWithVoice(s32 soundId, s32 voiceIndex);
void playSoundEffect(s32 soundId);
void *getActiveAudioChannelCount(void);
void *getActiveSongChannelCount(void);
