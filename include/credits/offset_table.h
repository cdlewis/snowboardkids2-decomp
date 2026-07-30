#pragma once

#include "common.h"

typedef struct {
    /* 0x0 */ s16 startFrame;
    /* 0x2 */ s16 commandCount;
    /* 0x4 */ s32 commandsOffset;
} SubtitleScheduleEntry;

typedef struct {
    /* 0x0 */ s32 entriesOffset;
    /* 0x4 */ s32 entryCountOffset;
} SubtitleSchedule;

s32 getSubtitleScheduleEntryCount(SubtitleSchedule *schedule);
s16 getSubtitleScheduleEntryStartFrame(SubtitleSchedule *schedule, s32 index);
u8 *getSubtitleScheduleEntryCommands(SubtitleSchedule *schedule, s32 index);
s16 getSubtitleScheduleEntryCommandCount(SubtitleSchedule *schedule, s32 index);
